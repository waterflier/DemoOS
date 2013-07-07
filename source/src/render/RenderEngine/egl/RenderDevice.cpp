extern "C"
{
#include "RenderDevice.h"
#include "RenderResource.h"
#include <stdlib.h>
}

#include <ui/PixelFormat.h>
#include <ui/GraphicBufferAllocator.h>
#include <ui/GraphicLog.h>
#include <ui/FramebufferNativeWindow.h>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

using namespace android;

typedef struct NGREEglDevice{
	FramebufferNativeWindow* pNativeWindow;
	EGLDisplay	eglDisplay;
	EGLConfig eglConfig;
	EGLSurface eglSurface;
	EGLContext eglContext;
}NGREEglDevice;


NGREDevice* g_eglDevice = NULL;


NGRE_RESULT NGREOpenDevice(NGREDevice** ppDevice)
{
	if(g_eglDevice == NULL)
	{
		NGREEglDevice* pEglDevice = (NGREEglDevice*)malloc(sizeof(NGREEglDevice));
		memset(pEglDevice, 0, sizeof(NGREEglDevice));

		do{
			pEglDevice->pNativeWindow = new FramebufferNativeWindow();
			assert(pEglDevice->pNativeWindow != NULL);
			if(pEglDevice->pNativeWindow == NULL)
			{
				printf("EglDevice: Android FrameBuffer open failed \r\n");
				break;
			}
			pEglDevice->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			assert(pEglDevice->eglDisplay != NULL);
			if(pEglDevice->eglDisplay == NULL)
			{
				printf("EglDevice: EglDisplay open failed \r\n");
				break;
			}
			EGLBoolean eglBoolRes = eglInitialize(pEglDevice->eglDisplay, NULL, NULL);
			assert(eglBoolRes);
			if(!eglBoolRes)
			{
				printf("EglDevice: eglInitialize failed \r\n");
				break;
			}
			int pixelFormat;
			ANativeWindow* cptrNativeWindow = pEglDevice->pNativeWindow;
			cptrNativeWindow->query(cptrNativeWindow, NATIVE_WINDOW_FORMAT, &pixelFormat);
			EGLint nNumConfigs = -1;
			eglBoolRes = eglGetConfigs(pEglDevice->eglDisplay, NULL, 0, &nNumConfigs);
			assert(eglBoolRes);
			if(!eglBoolRes)
			{
				printf("EglDevice: eglGetConfigs failed \r\n");
				break;
			}
			EGLConfig* const eglConfigs = new EGLConfig[nNumConfigs];
			EGLint eglAttribs[] = {
				EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
				EGL_NONE,               0,
				EGL_NONE
			};
			eglBoolRes = eglChooseConfig(pEglDevice->eglDisplay, eglAttribs, eglConfigs, nNumConfigs, &nNumConfigs);
			assert(eglBoolRes);
			if(!eglBoolRes)
			{
				printf("EglDevice: eglChooseConfig failed \r\n");
				break;
			}
			for (int i=0 ; i<nNumConfigs ; i++) {
				EGLint nativeVisualId = 0;
				eglGetConfigAttrib(pEglDevice->eglDisplay, eglConfigs[i], EGL_NATIVE_VISUAL_ID, &nativeVisualId);
				if (nativeVisualId>0 && pixelFormat == nativeVisualId) {
					pEglDevice->eglConfig = eglConfigs[i];
					break;
				}
			}
			delete [] eglConfigs;
			assert(pEglDevice->eglConfig);
			if(pEglDevice->eglConfig == NULL)
			{
				printf("EglDevice: select eglConfig failed \r\n");
				break;
			}
			pEglDevice->eglSurface = eglCreateWindowSurface(pEglDevice->eglDisplay, pEglDevice->eglConfig, cptrNativeWindow, NULL);
			assert(pEglDevice->eglSurface);
			if(pEglDevice->eglSurface == NULL)
			{
				printf("EglDevice: eglCreateWindowSurface failed \r\n");
				break;
			}
			EGLint eglContextAttributes[] = {
				#ifdef EGL_IMG_context_priority
				#ifdef HAS_CONTEXT_PRIORITY
					#warning "using EGL_IMG_context_priority"
					EGL_CONTEXT_PRIORITY_LEVEL_IMG, EGL_CONTEXT_PRIORITY_HIGH_IMG,
				#endif
				#endif
					EGL_NONE, EGL_NONE
			};
			pEglDevice->eglContext = eglCreateContext(pEglDevice->eglDisplay, pEglDevice->eglConfig, NULL, eglContextAttributes);
			assert(pEglDevice->eglContext);
			if(pEglDevice->eglContext == NULL)
			{
				printf("EglDevice: eglCreateContext failed \r\n");
				break;
			}
		}while(0);
		if(pEglDevice->eglContext != NULL)
		{
			g_eglDevice = (NGREDevice*)malloc(sizeof(NGREDevice));
			g_eglDevice->pExtra = pEglDevice;
			g_eglDevice->pCacheBitmap = NULL;
			g_eglDevice->nClipRectCount = 0;
		}
		else
		{
			free(pEglDevice);
		}
	}
	if(g_eglDevice == NULL)
	{
		*ppDevice = NULL;
		return NGRE_DEVICE_OPENFAILED;
	}
	*ppDevice = g_eglDevice;
	return NGRE_SUCCESS;
}

NGRE_RESULT NGREGetDeviceSize(NGREDevice* pDevice, int *pWidth, int*pHeight)
{
	if(pDevice == NULL || (((NGREEglDevice*)(pDevice->pExtra)) == NULL))
	{
		return NGRE_DEVICE_INVALIDPARAM;
	}
	NGREEglDevice* pEglDevice = (NGREEglDevice*)(pDevice->pExtra);
	eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_WIDTH,  pWidth);
	eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_HEIGHT,  pHeight);
    
	return NGRE_SUCCESS;
}

static inline uint16_t pack565(int r, int g, int b) {
    return (r<<11)|(g<<5)|b;
}

NGRE_RESULT NGREFlushDevice(NGREDevice* pDevice)
{
	////for egl env test
	NGREEglDevice* pEglDevice = (NGREEglDevice*)(pDevice->pExtra);
	GLuint                      mWormholeTexName,mProtectedTexName;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnable(GL_SCISSOR_TEST);
    glShadeModel(GL_FLAT);
    glDisable(GL_DITHER);
    glDisable(GL_CULL_FACE);

    const uint16_t g0 = pack565(0x7F,0x0F,0x7F);
    const uint16_t g1 = pack565(0x17,0x2f,0x17);
    const uint16_t wormholeTexData[4] = { g0, g1, g1, g0 };
    glGenTextures(1, &mWormholeTexName);
    glBindTexture(GL_TEXTURE_2D, mWormholeTexName);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0,
            GL_RGB, GL_UNSIGNED_SHORT_5_6_5, wormholeTexData);

    const uint16_t protTexData[] = { pack565(0x3, 0x43, 0x03) };
    glGenTextures(1, &mProtectedTexName);
    glBindTexture(GL_TEXTURE_2D, mProtectedTexName);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0,//
            GL_RGB, GL_UNSIGNED_SHORT_5_6_5, protTexData);

	int mWidth, mHeight;
	eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_WIDTH,  &mWidth);
	eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_HEIGHT,  &mHeight);
    glViewport(0, 0, mWidth, mHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // put the origin in the left-bottom corner
    glOrthof(0, mWidth, 0, mHeight, 0, 1); // l=0, r=w ; b=0, t=h
	 float vertexes[] = {

                -100.0, -200.0, 0.0, // (x1, y1, z1)

                100.0, -200.0, 0.0, // (x2, y2, z2)

                0.0, 100.0, 0.0 // (x3, y3, z3)

        };
	 short indexes[]  = {0, 1, 2};

	 eglMakeCurrent(pEglDevice->eglDisplay, pEglDevice->eglSurface, pEglDevice->eglSurface, pEglDevice->eglContext);

    while(true)
    {
		
		
                     
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

        

		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

		glVertexPointer(3, GL_FLOAT, 0, vertexes);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indexes);
        eglSwapBuffers(pEglDevice->eglDisplay,pEglDevice->eglSurface);
        sleep(1);
        printf("ok,next frame\n");
	
    }
	return NGRE_SUCCESS;
}



void NGRECloseDevice(NGREDevice* pDevice)
{
	/*munmap(FrameBuffer, FixedInfo.smem_len);
    close(FrameBufferFD);*/
}