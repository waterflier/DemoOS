#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <sys/mman.h>
//#include <asm/ioctls.h>
#include <sys/ioctl.h>

#include "SkCanvas.h"
#include "SkGraphics.h"
#include "SkImageEncoder.h"
#include "SkString.h"
#include "SkImageDecoder.h"
#include "SkStream.h"

//#include <GL/glut.h>


static int OriginalVT = -1;
struct fb_fix_screeninfo FixedInfo;
struct fb_var_screeninfo OrigVarInfo;
static int FrameBufferFD = -1;
void *FrameBuffer = (void *) -1;
static int ConsoleFD = -1;
#define SIGUSR1 10
void openFBDEV(void) {
    int vtnumber, ttyfd;
    char ttystr[1000];
    /* open the framebuffer device */
    FrameBufferFD = open("/dev/fb0", O_RDWR);
    if (FrameBufferFD < 0) {
        fprintf(stderr, "Error opening /dev/fb0\n");
        exit(1);
    }
    /* Get the fixed screen info */
    if (ioctl(FrameBufferFD, FBIOGET_FSCREENINFO, &FixedInfo)) {
        fprintf(stderr, "error: ioctl(FBIOGET_FSCREENINFO) failed\n");
        exit(1);
    }
    /* get the variable screen info */
    if (ioctl(FrameBufferFD, FBIOGET_VSCREENINFO, &OrigVarInfo)) {
        fprintf(stderr, "error: ioctl(FBIOGET_VSCREENINFO) failed\n");
        exit(1);
    }
       
    if (FixedInfo.visual != FB_VISUAL_TRUECOLOR && FixedInfo.visual != FB_VISUAL_DIRECTCOLOR) {
        fprintf(stderr, "non-TRUE/DIRECT-COLOR visuals (0x%x) not supported by this demo.\n", FixedInfo.visual);
        exit(1);
    }

    /*
     * fbdev says the frame buffer is at offset zero, and the mmio region
     * is immediately after.
     */
    /* mmap the framebuffer into our address space */
    FrameBuffer = (void *) mmap(0, /* start */
        FixedInfo.smem_len, /* bytes */
        PROT_READ | PROT_WRITE, /* prot */
        MAP_SHARED, /* flags */
        FrameBufferFD, /* fd */
        0 /* offset */);
    if (FrameBuffer == (void *) - 1) {
        fprintf(stderr, "error: unable to mmap framebuffer\n");
        exit(1);
    }

     /* open the console tty */
     //sprintf(ttystr, "/dev/tty", vtnumber);  /* /dev/tty1-64 */
    /* ConsoleFD = open("/dev/tty", O_RDWR | O_NDELAY, 0);
     if (ConsoleFD < 0) {
             fprintf(stderr, "error couldn't open console fd \n", vtnumber);
         exit(1);
     }*/
     /* save current vt number */
     /*{
       struct vt_stat vts;
       if (ioctl(ConsoleFD, VT_GETSTATE, &vts) == 0)
             OriginalVT = vts.v_active;
     }*/
     /* disconnect from controlling tty */
     /*ttyfd = open("/dev/tty", O_RDWR);
     if (ttyfd >= 0) {
                ioctl(ttyfd, TIOCNOTTY, 0);
        close(ttyfd);
     }*/
     /* some magic to restore the vt when we exit */
    /* {
       struct vt_mode vt;
       if (ioctl(ConsoleFD, VT_ACTIVATE, vtnumber) != 0)
              fprintf(stderr,"ioctl VT_ACTIVATE\n");
       if (ioctl(ConsoleFD, VT_WAITACTIVE, vtnumber) != 0)
              fprintf(stderr,"ioctl VT_WAITACTIVE\n");
       
       if (ioctl(ConsoleFD, VT_GETMODE, &vt) < 0) {
              fprintf(stderr, "error: ioctl VT_GETMODE\n");
          exit(1);
       }
       vt.mode = VT_PROCESS;
       vt.relsig = SIGUSR1;
       vt.acqsig = SIGUSR1;
       if (ioctl(ConsoleFD, VT_SETMODE, &vt) < 0) {
               fprintf(stderr, "error: ioctl(VT_SETMODE) failed\n");
           exit(1);
       }
     }*/
     /* go into graphics mode */
     /*if (ioctl(ConsoleFD, KDSETMODE, KD_GRAPHICS) < 0) {
             fprintf(stderr, "error: ioctl(KDSETMODE, KD_GRAPHICS) failed\n");
         exit(1);
     }*/
}

void closeFBDEV(void) {
        struct vt_mode VT;
    /* restore original variable screen info */
    if (ioctl(FrameBufferFD, FBIOPUT_VSCREENINFO, &OrigVarInfo)) {
            fprintf(stderr, "ioctl(FBIOPUT_VSCREENINFO failed)\n");
        exit(1);
    }
    munmap(FrameBuffer, FixedInfo.smem_len);
    close(FrameBufferFD);
    /* restore text mode */
    //ioctl(ConsoleFD, KDSETMODE, KD_TEXT);
    ///* set vt */
    //if (ioctl(ConsoleFD, VT_GETMODE, &VT) != -1) {
    //        VT.mode = VT_AUTO;
    //    ioctl(ConsoleFD, VT_SETMODE, &VT);
    //}
    ///* restore original vt */
    //if (OriginalVT >= 0) {
    //        ioctl(ConsoleFD, VT_ACTIVATE, OriginalVT);
    //    OriginalVT = -1;
    //}
    //close(ConsoleFD);
}


void DoSkiaPaint()
{
	SkAutoGraphics ag;
    /*SkString path("skhello.png");
    SkString text("Hello");*/

	bool bres = true;
	SkBitmap skBkg;
	SkImageDecoder * pngDecoder = CreatePNGImageDecoder();
	SkFILEStream    stream("/root/win_work/DemoOS/source/src/render/resource/bkg.png");
	bres = pngDecoder->decode(&stream, &skBkg, SkBitmap::kARGB_8888_Config, SkImageDecoder::kDecodePixels_Mode);
	bres = SkImageDecoder::DecodeFile("/root/win_work/DemoOS/source/src/render/resource/bkg.png",&skBkg,SkBitmap::kARGB_8888_Config, SkImageDecoder::kDecodePixels_Mode);

	/*SkPaint paint;
    paint.setAntiAlias(true);
    paint.setTextSize(SkIntToScalar(30));
    SkScalar width = paint.measureText(text.c_str(), text.size());
    SkScalar spacing = paint.getFontSpacing();

    int w = SkScalarRound(width) + 30;
    int h = SkScalarRound(spacing) + 30;
    SkBitmap skBkg;
    skBkg.setConfig(SkBitmap::kARGB_8888_Config, w, h);
    skBkg.allocPixels();

    SkCanvas canvas(skBkg);
    canvas.drawColor(SK_ColorWHITE);

    paint.setTextAlign(SkPaint::kCenter_Align);
    canvas.drawText(text.c_str(), text.size(),
                    10, 10,
                    paint);*/

	fprintf(stderr, "image size %d %d %d \n",skBkg.width(),skBkg.height(),skBkg.rowBytes());
	SkBitmap screenBkg;
	SkIRect screenRect = SkIRect::MakeWH(OrigVarInfo.xres,OrigVarInfo.yres);
	bres = skBkg.extractSubset(&screenBkg, screenRect);
	if(!bres)
	{
		fprintf(stderr, "can't create screen size bitmap\n");
	}
	bres = screenBkg.copyPixelsTo(FrameBuffer, FixedInfo.smem_len, FixedInfo.line_length);

	if(!bres)
	{
		fprintf(stderr, "can't copy bytes from imge to fb\n");
	}
	
}

//class teststatic
//{
//public:
//	teststatic()
//	{
//		fprintf(stderr, "init teststatic");
//	}
//};
//
//static teststatic _teststatic;



int main(int argc, char * const argv[]) {
    openFBDEV();
    fprintf(stderr, "openFBDEV finish\n");
	int i=0;
	
	while(true)
	{
		DoSkiaPaint();
		sleep(1);
	}


    closeFBDEV();
    fprintf(stderr, "closeFBDEV finish\n");
    return 0;
}

