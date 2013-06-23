//
// uitest.cpp
//
// init android graphic hardware
//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <utils/Atomic.h>
#include <utils/Errors.h>
#include <utils/KeyedVector.h>
#include <utils/RefBase.h>
#include <utils/SortedVector.h>
#include <utils/threads.h>
#include <utils/String8.h>
#include <utils/String16.h>
#include <utils/StopWatch.h>
#include <cutils/log.h>
#include <cutils/properties.h>

#include <binder/BinderService.h>
#include <binder/IMemory.h>

#include <ui/PixelFormat.h>
#include <ui/GraphicBufferAllocator.h>
#include <utils/Trace.h>

#include <ui/FramebufferNativeWindow.h>

#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>


#include <pixelflinger/pixelflinger.h>
#include <private/gui/SharedBufferStack.h>
#include <gui/BitTube.h>


#include "./GLExtensions.h"



using namespace android;

sp<FramebufferNativeWindow> mNativeWindow;
uint32_t        mFlags;
EGLDisplay      mDisplay;
EGLSurface      mSurface;
EGLContext      mContext;
EGLConfig       mConfig;
float           mDpiX;
float           mDpiY;
float           mRefreshRate;
float           mDensity;
int             mWidth;
int             mHeight;
PixelFormat     mFormat;

uint32_t mPageFlipCount;
GLint           mMaxViewportDims[2];
GLint           mMaxTextureSize;

sp<IMemoryHeap>             mServerHeap;

    enum {
        COPY_BITS_EXTENSION         = 0x00000008,
        BUFFER_PRESERVED            = 0x00010000,
        PARTIAL_UPDATES             = 0x00020000,   // video driver feature
        SLOW_CONFIG                 = 0x00040000,   // software
        SWAP_RECTANGLE              = 0x00080000,
    };

static status_t selectConfigForPixelFormat(
        EGLDisplay dpy,
        EGLint const* attrs,
        PixelFormat format,
        EGLConfig* outConfig)
{
    EGLConfig config = NULL;
    EGLint numConfigs = -1, n=0;
    eglGetConfigs(dpy, NULL, 0, &numConfigs);
    EGLConfig* const configs = new EGLConfig[numConfigs];
    eglChooseConfig(dpy, attrs, configs, numConfigs, &n);
    for (int i=0 ; i<n ; i++) {
        EGLint nativeVisualId = 0;
        eglGetConfigAttrib(dpy, configs[i], EGL_NATIVE_VISUAL_ID, &nativeVisualId);
        if (nativeVisualId>0 && format == nativeVisualId) {
            *outConfig = configs[i];
            delete [] configs;
            return NO_ERROR;
        }
    }
    delete [] configs;
    return NAME_NOT_FOUND;
}

static inline uint16_t pack565(int r, int g, int b) {
    return (r<<11)|(g<<5)|b;
}

void initHardware(uint32_t dpy)
{
    EGLint numConfigs = 0;
    EGLint dummy;
    EGLSurface surface;
    EGLContext context;
    EGLBoolean result;
    status_t err;

    mNativeWindow = new FramebufferNativeWindow();
    framebuffer_device_t const * fbDev = mNativeWindow->getDevice();
    if (!fbDev) {
        printf("Display subsystem failed to initialize. check logs. exiting...");
        exit(0);
    }

    int format;
    ANativeWindow const * const window = mNativeWindow.get();
    window->query(window, NATIVE_WINDOW_FORMAT, &format);
    mDpiX = mNativeWindow->xdpi;
    mDpiY = mNativeWindow->ydpi;
    mRefreshRate = fbDev->fps;


/* FIXME: this is a temporary HACK until we are able to report the refresh rate
 * properly from the HAL. The WindowManagerService now relies on this value.
 */
#ifndef REFRESH_RATE
    //mRefreshRate = fbDev->fps;
#else
    //mRefreshRate = REFRESH_RATE;
#warning "refresh rate set via makefile to REFRESH_RATE"
#endif



    // initialize EGL
    EGLint attribs[] = {
            EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
            EGL_NONE,               0,
            EGL_NONE
    };

    // debug: disable h/w rendering
    //char property[PROPERTY_VALUE_MAX];
    //if (property_get("debug.sf.hw", property, NULL) > 0) {
    //    if (atoi(property) == 0) {
    //        LOGW("H/W composition disabled");
    //        attribs[2] = EGL_CONFIG_CAVEAT;
    //        attribs[3] = EGL_SLOW_CONFIG;
    //    }
    // }

    // TODO: all the extensions below should be queried through
    // eglGetProcAddress().

    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, NULL, NULL);
    eglGetConfigs(display, NULL, 0, &numConfigs);

    EGLConfig config = NULL;
    err = selectConfigForPixelFormat(display, attribs, format, &config);
    ALOGE_IF(err, "couldn't find an EGLConfig matching the screen format");
    
    EGLint r,g,b,a;
    eglGetConfigAttrib(display, config, EGL_RED_SIZE,   &r);
    eglGetConfigAttrib(display, config, EGL_GREEN_SIZE, &g);
    eglGetConfigAttrib(display, config, EGL_BLUE_SIZE,  &b);
    eglGetConfigAttrib(display, config, EGL_ALPHA_SIZE, &a);

    if (mNativeWindow->isUpdateOnDemand()) {
        mFlags |= PARTIAL_UPDATES;
    }
    
    if (eglGetConfigAttrib(display, config, EGL_CONFIG_CAVEAT, &dummy) == EGL_TRUE) {
        if (dummy == EGL_SLOW_CONFIG)
            mFlags |= SLOW_CONFIG;
    }

    /*
     * Create our main surface
     */

    surface = eglCreateWindowSurface(display, config, mNativeWindow.get(), NULL);
    eglQuerySurface(display, surface, EGL_WIDTH,  &mWidth);
    eglQuerySurface(display, surface, EGL_HEIGHT, &mHeight);

    if (mFlags & PARTIAL_UPDATES) {
        // if we have partial updates, we definitely don't need to
        // preserve the backbuffer, which may be costly.
        eglSurfaceAttrib(display, surface,
                EGL_SWAP_BEHAVIOR, EGL_BUFFER_DESTROYED);
    }

    if (eglQuerySurface(display, surface, EGL_SWAP_BEHAVIOR, &dummy) == EGL_TRUE) {
        if (dummy == EGL_BUFFER_PRESERVED) {
            mFlags |= BUFFER_PRESERVED;
        }
    }
    
    /* Read density from build-specific ro.sf.lcd_density property
     * except if it is overridden by qemu.sf.lcd_density.
     */
    char property[PROPERTY_VALUE_MAX];
    //if (property_get("qemu.sf.lcd_density", property, NULL) <= 0) {
    //    if (property_get("ro.sf.lcd_density", property, NULL) <= 0) {
    //        ALOGW("ro.sf.lcd_density not defined, using 160 dpi by default.");
    //        strcpy(property, "160");
    //    }
    //} else {
        /* for the emulator case, reset the dpi values too */
        mDpiX = mDpiY = atoi(property);
    //}
    mDensity = atoi(property) * (1.0f/160.0f);


    /*
     * Create our OpenGL ES context
     */
    

    EGLint contextAttributes[] = {
#ifdef EGL_IMG_context_priority
#ifdef HAS_CONTEXT_PRIORITY
#warning "using EGL_IMG_context_priority"
        EGL_CONTEXT_PRIORITY_LEVEL_IMG, EGL_CONTEXT_PRIORITY_HIGH_IMG,
#endif
#endif
        EGL_NONE, EGL_NONE
    };
    context = eglCreateContext(display, config, NULL, contextAttributes);

    mDisplay = display;
    mConfig  = config;
    mSurface = surface;
    mContext = context;
    mFormat  = fbDev->format;
    mPageFlipCount = 0;

    /*
     * Gather OpenGL ES extensions
     */

    result = eglMakeCurrent(display, surface, surface, context);
    if (!result) {
        printf("Couldn't create a working GLES context. check logs. exiting...");
        exit(0);
    }

    GLExtensions& extensions(GLExtensions::getInstance());
    extensions.initWithGLStrings(
            glGetString(GL_VENDOR),
            glGetString(GL_RENDERER),
            glGetString(GL_VERSION),
            glGetString(GL_EXTENSIONS),
            eglQueryString(display, EGL_VENDOR),
            eglQueryString(display, EGL_VERSION),
            eglQueryString(display, EGL_EXTENSIONS));

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &mMaxTextureSize);
    glGetIntegerv(GL_MAX_VIEWPORT_DIMS, mMaxViewportDims);

    ALOGI("EGL informations:");
    printf("# of configs : %d", numConfigs);
    printf("vendor    : %s", extensions.getEglVendor());
    printf("version   : %s", extensions.getEglVersion());
    printf("extensions: %s", extensions.getEglExtension());
    printf("Client API: %s", eglQueryString(display, EGL_CLIENT_APIS)?:"Not Supported");
    printf("EGLSurface: %d-%d-%d-%d, config=%p", r, g, b, a, config);

    printf("OpenGL informations:");
    printf("vendor    : %s", extensions.getVendor());
    printf("renderer  : %s", extensions.getRenderer());
    printf("version   : %s", extensions.getVersion());
    printf("extensions: %s", extensions.getExtension());
    //LOGI("GL_MAX_TEXTURE_SIZE = %d", mMaxTextureSize);
    //LOGI("GL_MAX_VIEWPORT_DIMS = %d x %d", mMaxViewportDims[0], mMaxViewportDims[1]);
    //LOGI("flags = %08x", mFlags);

    // Unbind the context from this thread
    eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);


    // initialize the H/W composer
    //mHwc = new HWComposer(mFlinger);
    //if (mHwc->initCheck() == NO_ERROR) {
    //    mHwc->setFrameBuffer(mDisplay, mSurface);
    //}
    return;
}

GLuint                      mWormholeTexName,mProtectedTexName;



int main()
{

    int dpy = 0;

    
    // initialize the main display
    //GraphicPlane& plane(graphicPlane(dpy));
    printf("start init graphic hardware\n");
    //seteuid(0);
    //setuid(0);
    initHardware(0);
    printf("init graphic hardware ok\n");
    //DisplayHardware* const hw = new DisplayHardware(this, dpy);
    //plane.setDisplayHardware(hw);
    

    // create the shared control-block
    //mServerHeap = new MemoryHeapBase(4096,
     //       MemoryHeapBase::READ_ONLY, "uitest read-only heap");
    //LOGE_IF(mServerHeap==0, "can't create shared memory dealer");


    // initialize primary screen
    // (other display should be initialized in the same manner, but
    // asynchronously, as they could come and go. None of this is supported
    // yet).

    //const DisplayHardware& hw = plane.displayHardware();
    //const uint32_t w = hw.getWidth();
    //const uint32_t h = hw.getHeight();
    //const uint32_t f = hw.getFormat();
    //hw.makeCurrent();
    eglMakeCurrent(mDisplay, mSurface, mSurface, mContext);

    // initialize the shared control block
    //mServerCblk->connected |= 1<<dpy;
    //display_cblk_t* dcblk = mServerCblk->displays + dpy;
    //memset(dcblk, 0, sizeof(display_cblk_t));
    //dcblk->w            = plane.getWidth();
    //dcblk->h            = plane.getHeight();
    //dcblk->format       = f;
    //dcblk->orientation  = ISurfaceComposer::eOrientationDefault;
    //dcblk->xdpi         = hw.getDpiX();
    //dcblk->ydpi         = hw.getDpiY();
    //dcblk->fps          = hw.getRefreshRate();
    //dcblk->density      = hw.getDensity();

    // Initialize OpenGL|ES
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

    glViewport(0, 0, mWidth, mHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // put the origin in the left-bottom corner
    glOrthof(0, mWidth, 0, mHeight, 0, 1); // l=0, r=w ; b=0, t=h
 
    printf("end gl init,enter forver loop\n");
  
    float vertexes[] = {

                500.0, 200.0, 0.0, // (x1, y1, z1)

                400.0, 300.0, 0.0, // (x2, y2, z2)

                200.0, 100.0, 0.0 // (x3, y3, z3)

        };
    short indexes[]  = {0, 1, 2};
    //int pf = open("/dev/graphics/fb0",O_WRONLY);
    //char* pbuffer = new char[256*3*256];
    //memset (pbuffer,0x5f,256*256*3);
    //write(pf,pbuffer,256*256*3);  

    //int touchf = open("/dev/touchscreen-1wire",O_RDONLY);
     int a = 0;
    while(true)
    {
        a++;
                     
               glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT);

            vertexes[3] = 400.0 + a%200;
 
            glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

            glVertexPointer(3, GL_FLOAT, 0, vertexes);

            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, indexes);
        glViewport(0, 0, mWidth, mHeight);
        eglSwapBuffers(mDisplay,mSurface);
        usleep(12500);
        printf("next frame\n");
    }

    return 0;

}
