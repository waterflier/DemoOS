extern "C"
{
#include "RenderDevice.h"
#include "RenderResource.h"
#include "../../resource_manager/ResourceManager.h"
#include <stdlib.h>
}

#include <ui/PixelFormat.h>
#include <ui/GraphicBufferAllocator.h>
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
	//GLuint eglTexName;

	GLint eglTexWidth;
	GLint eglTexHeight;
	GLfloat eglTexCoorX;
	GLfloat eglTexCoorY;
}NGREEglDevice;


NGREDevice* g_eglDevice = NULL;

#define NGRE_EGL_INITFAILED	1


NGRE_RESULT NGREInitEgl(NGREEglDevice* pEglDevice)
{
	pEglDevice->pNativeWindow = new FramebufferNativeWindow();
	//assert(pEglDevice->pNativeWindow != NULL);
	if(pEglDevice->pNativeWindow == NULL)
	{
		printf("EglDevice: Android FrameBuffer open failed \r\n");
		return NGRE_EGL_INITFAILED;
	}
	pEglDevice->eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	//assert(pEglDevice->eglDisplay != NULL);
	if(pEglDevice->eglDisplay == NULL)
	{
		printf("EglDevice: EglDisplay open failed \r\n");
		return NGRE_EGL_INITFAILED;
	}
	EGLBoolean eglBoolRes = eglInitialize(pEglDevice->eglDisplay, NULL, NULL);
	//assert(eglBoolRes);
	if(!eglBoolRes)
	{
		printf("EglDevice: eglInitialize failed for %d\r\n",eglGetError());
		return NGRE_EGL_INITFAILED;
	}
	int pixelFormat;
	ANativeWindow* cptrNativeWindow = pEglDevice->pNativeWindow;
	cptrNativeWindow->query(cptrNativeWindow, NATIVE_WINDOW_FORMAT, &pixelFormat);
	EGLint nNumConfigs = -1;
	eglBoolRes = eglGetConfigs(pEglDevice->eglDisplay, NULL, 0, &nNumConfigs);
	//assert(eglBoolRes);
	if(!eglBoolRes)
	{
		printf("EglDevice: eglGetConfigs failed for %d \r\n",eglGetError());
		return NGRE_EGL_INITFAILED;
	}
	EGLConfig* const eglConfigs = new EGLConfig[nNumConfigs];
	EGLint eglAttribs[] = {
		EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
		EGL_NONE,               0,
		EGL_NONE
	};
	eglBoolRes = eglChooseConfig(pEglDevice->eglDisplay, eglAttribs, eglConfigs, nNumConfigs, &nNumConfigs);
	//assert(eglBoolRes);
	if(!eglBoolRes)
	{
		printf("EglDevice: eglChooseConfig failed for %d \r\n",eglGetError());
		return NGRE_EGL_INITFAILED;
	}
	for (int i=0 ; i<nNumConfigs ; i++) {
		EGLint nativeVisualId = 0;
		eglGetConfigAttrib(pEglDevice->eglDisplay, eglConfigs[i], EGL_NATIVE_VISUAL_ID, &nativeVisualId);
		if (nativeVisualId > 0 && pixelFormat == nativeVisualId) 
		{
			pEglDevice->eglConfig = eglConfigs[i];
			break;
		}
	}
	delete [] eglConfigs;
	//assert(pEglDevice->eglConfig);
	if(pEglDevice->eglConfig == NULL)
	{
		printf("EglDevice: select eglConfig failed \r\n");
		return NGRE_EGL_INITFAILED;
	}
	pEglDevice->eglSurface = eglCreateWindowSurface(pEglDevice->eglDisplay, pEglDevice->eglConfig, cptrNativeWindow, NULL);
	//assert(pEglDevice->eglSurface);
	if(pEglDevice->eglSurface == NULL)
	{
		printf("EglDevice: eglCreateWindowSurface failed \r\n");
		return NGRE_EGL_INITFAILED;
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
	//assert(pEglDevice->eglContext);
	if(pEglDevice->eglContext == NULL)
	{
		printf("EglDevice: eglCreateContext failed \r\n");
		return NGRE_EGL_INITFAILED;
	}
	eglBoolRes = eglMakeCurrent(pEglDevice->eglDisplay, pEglDevice->eglSurface, pEglDevice->eglSurface, pEglDevice->eglContext);
	if(!eglBoolRes)
	{
		printf("EglDevice: eglMakeCurrent failed for %d \r\n",eglGetError());
		return NGRE_EGL_INITFAILED;
	}

	int nViewPortWidth, nViewPortHeight;
	eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_WIDTH,  &nViewPortWidth);
	eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_HEIGHT,  &nViewPortHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, nViewPortWidth, nViewPortHeight);
	glOrthof(-((GLfloat)nViewPortWidth)/2, ((GLfloat)nViewPortWidth)/2, -((GLfloat)nViewPortHeight)/2, ((GLfloat)nViewPortHeight)/2 , 0, 1);

			

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	return NGRE_SUCCESS;
}


GLint NGREGetEglTexSize(int nSize)
{
	unsigned int eglTexSize = 1;
	while(nSize > eglTexSize)
	{
		eglTexSize = eglTexSize << 1;
	}
	return eglTexSize;
}

NGRE_RESULT NGREOpenDevice(NGREDevice** ppDevice)
{
	if(g_eglDevice == NULL)
	{
		NGREEglDevice* pEglDevice = (NGREEglDevice*)malloc(sizeof(NGREEglDevice));
		memset(pEglDevice, 0, sizeof(NGREEglDevice));
		NGRE_RESULT lResult = NGRE_SUCCESS;
		do{
			lResult = NGREInitEgl(pEglDevice);
			if(lResult != NGRE_SUCCESS)
			{
				break;
			}
			glEnable(GL_TEXTURE_2D);

			//eglGetError();
			int nDeviceWidth, nDeviceHeight;
			eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_WIDTH,  &nDeviceWidth);
			eglQuerySurface(pEglDevice->eglDisplay, pEglDevice->eglSurface, EGL_HEIGHT,  &nDeviceHeight);
			pEglDevice->eglTexWidth = NGREGetEglTexSize(nDeviceWidth);
			pEglDevice->eglTexHeight = NGREGetEglTexSize(nDeviceHeight);
			pEglDevice->eglTexCoorX = (GLfloat)nDeviceWidth / pEglDevice->eglTexWidth;
			pEglDevice->eglTexCoorY = (GLfloat)nDeviceHeight / pEglDevice->eglTexHeight;

			printf("egl tex size set to %d, %d\r\n",pEglDevice->eglTexWidth, pEglDevice->eglTexHeight);
			eglGetError();
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pEglDevice->eglTexWidth, pEglDevice->eglTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			GLenum eglError = eglGetError();
			if(eglError != EGL_SUCCESS)
			{
				printf("glTexImage2D failed for %d \r\n", eglError);
				lResult = NGRE_EGL_INITFAILED;
				break;
			}

			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

			//glGenTextures(1, &(pEglDevice->eglTexName));
			/*if(pEglDevice->eglTexName == GL_INVALID_VALUE)
			{
				printf("EglDevice: eglGenTextures failed for %d \r\n",eglGetError());
				break;
			}*/
		}while(0);
		if(lResult == NGRE_SUCCESS)
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
		//exit(0);
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


NGRE_RESULT NGREFlushDevice(NGREDevice* pDevice)
{
	NGREEglDevice* pEglDevice = (NGREEglDevice*)(pDevice->pExtra); 
	unsigned int nClipRect = NGREGetDeviceClipRectCount(pDevice);
	if(nClipRect == 0)
	{
		return NGRE_SUCCESS;
	}
	NGREBitmap* pDeviceBitmap = NULL;
	void * pRes = NULL;
	NGRE_RESULT lResult = NGREGetBitmapFromDevice(pDevice, &pDeviceBitmap);
	if(pDeviceBitmap == NULL)
	{
		printf("flush denied for device bitmap is valid \r\n");
		return NGRE_SUCCESS;
	}
	//glBindTexture(GL_TEXTURE_2D, pEglDevice->eglTexName);
	
	GLvoid* pDeviceBitmapBuffer = NULL;
	lResult = NGREGetBitmapBuffer(pDeviceBitmap, NGREAllocType_mem, &pDeviceBitmapBuffer);
	////绑定到已有的name上之前的问题自动释放
	unsigned int uDeviceWidth = NGREBitmapWidth(pDeviceBitmap);
	unsigned int uDeviceHeight = NGREBitmapHeight(pDeviceBitmap);
	eglGetError();
	glTexSubImage2D(GL_TEXTURE_2D,  0,  0,  0,  uDeviceWidth,  uDeviceHeight,  GL_RGBA,  GL_UNSIGNED_BYTE,  pDeviceBitmapBuffer); 
	GLenum eglError = eglGetError();
	if(eglError != EGL_SUCCESS)
	{
		printf("glTexSubImage2D failed for %d \r\n", eglError);
	}
	
	//printf("device bitmap size is %d, %d , %d\r\n", uDeviceWidth, uDeviceHeight, (int)pDeviceBitmapBuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	GLfloat vertexCoord[] = {
		-((GLfloat)uDeviceWidth)/2,	((GLfloat)uDeviceHeight)/2,		0, 
		-((GLfloat)uDeviceWidth)/2,	-((GLfloat)uDeviceHeight)/2,	0,
		((GLfloat)uDeviceWidth)/2,	-((GLfloat)uDeviceHeight)/2,	0, 
		((GLfloat)uDeviceWidth)/2,	((GLfloat)uDeviceHeight)/2,		0};
	glVertexPointer(3, GL_FLOAT, 0, vertexCoord);
	glEnableClientState(GL_VERTEX_ARRAY);

	//GLshort texCoord[] = {0,1,0,0,1,0,1,1}; 
	GLfloat texCoord[] = {0,0,0,pEglDevice->eglTexCoorY,pEglDevice->eglTexCoorX,pEglDevice->eglTexCoorY,pEglDevice->eglTexCoorX,0}; 
	glTexCoordPointer(2, GL_FLOAT, 0, texCoord);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	GLboolean eglResult = eglSwapBuffers(pEglDevice->eglDisplay,pEglDevice->eglSurface);
	if(!eglResult)
	{
		printf("swap egl buffer err for %d", eglGetError());
	}
	
    NGREClearDeviceClipRect(pDevice);
	return NGRE_SUCCESS;
}



void NGRECloseDevice(NGREDevice* pDevice)
{
	/*munmap(FrameBuffer, FixedInfo.smem_len);
    close(FrameBufferFD);*/
}