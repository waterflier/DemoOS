#include "RenderDevice.h"
#include "RenderResource.h"
#include <stdlib.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <fcntl.h>



typedef struct NGREFBDevice{
	struct fb_fix_screeninfo fixedInfo;
	struct fb_var_screeninfo origVarInfo;
	void * frameBuffer ;
	int fbDesc;
}NGREFBDevice;



NGREDevice* g_fbDevice = NULL;

NGRE_RESULT NGREOpenDevice(NGREDevice** ppDevice)
{
	if(g_fbDevice == NULL)
	{
		NGREFBDevice* pFbDevice = (NGREFBDevice*)malloc(sizeof(NGREFBDevice));
		memset(pFbDevice, 0, sizeof(NGREFBDevice));

		do{
		#ifdef ANDROID
			pFbDevice->fbDesc = open("/dev/graphics/fb0", O_RDWR);
		#else
			pFbDevice->fbDesc = open("/dev/fb0",O_RDWR);
		#endif
			if(pFbDevice->fbDesc < 0)
			{
				break;
			}
			if (ioctl(pFbDevice->fbDesc, FBIOGET_FSCREENINFO, &(pFbDevice->fixedInfo))) 
			{
				break;
			}
			if (ioctl(pFbDevice->fbDesc, FBIOGET_VSCREENINFO, &(pFbDevice->origVarInfo))) 
			{
				break;
			}
			pFbDevice->frameBuffer = (void *) mmap(0, pFbDevice->fixedInfo.smem_len,PROT_READ | PROT_WRITE, 
				MAP_SHARED,pFbDevice->fbDesc, 0);
		}while(0);
		if(pFbDevice->frameBuffer != NULL)
		{
			g_fbDevice = (NGREDevice*)malloc(sizeof(NGREDevice));
			g_fbDevice->pExtra = pFbDevice;
			g_fbDevice->pCacheBitmap = NULL;
			g_fbDevice->nClipRectCount = 0;
		}
		else
		{
			free(pFbDevice);
		}
	}
	if(g_fbDevice == NULL)
	{
		*ppDevice = NULL;
		return NGRE_DEVICE_OPENFAILED;
	}
	*ppDevice = g_fbDevice;
	return NGRE_SUCCESS;
}

NGRE_RESULT NGREGetDeviceSize(NGREDevice* pDevice, int *pWidth, int*pHeight)
{
	if(pDevice == NULL || (((NGREFBDevice*)(pDevice->pExtra)) == NULL))
	{
		return NGRE_DEVICE_INVALIDPARAM;
	}
	*pWidth = ((NGREFBDevice*)(pDevice->pExtra))->origVarInfo.xres;
	*pHeight = ((NGREFBDevice*)(pDevice->pExtra))->origVarInfo.yres;

	return NGRE_SUCCESS;
}

NGRE_RESULT NGREGetBitmapFromDevice(NGREDevice* pDevice, LPNGREBitmap* ppBitmap)
{
	//assert(pDevice != NULL);
	if(pDevice->pCacheBitmap == NULL)
	{
		NGREFBDevice* pFBDevice = (NGREFBDevice*)(pDevice->pExtra);
		NGRE_RESULT lResult = NGRECreateBitmap(pFBDevice->origVarInfo.xres,
			pFBDevice->origVarInfo.yres, (pFBDevice->origVarInfo.bits_per_pixel>>3), &(pDevice->pCacheBitmap));
		//assert(lResult == NGRE_SUCCESS);
		lResult = NGREAllocBitmap(pDevice->pCacheBitmap, NGREAllocType_GpuTexture);
		//assert(lResult == NGRE_SUCCESS);
		*ppBitmap = pDevice->pCacheBitmap;
		return lResult;
	}
	*ppBitmap = pDevice->pCacheBitmap;
	return NGRE_SUCCESS;
}

void NGRECopy2FrameBuffer(void* pSrc, void* frameBuffer, CLPNGREOpIRect pRect, unsigned long ulRowBytes, unsigned char uBPP)
{
	unsigned char* pCurLine = (unsigned char*)pSrc + ulRowBytes * pRect->top + uBPP * pRect->left;
	unsigned char* pFBCurLine = frameBuffer + ulRowBytes * pRect->top + uBPP * pRect->left;
	unsigned char* pCur = NULL;
	unsigned char* pFBCur = NULL;
	unsigned int iy , ix;
#ifdef ANDROID 
	for(iy = pRect->top; iy < pRect->bottom; ++iy)
	{
		pCur = pCurLine;
		pFBCur = pFBCurLine;
		for(ix = pRect->left; ix < pRect->right; ++ix)
		{
			pFBCur[0] = pCur[2];
			pFBCur[1] = pCur[1];
			pFBCur[2] = pCur[0];
			pFBCur[3] = pCur[3];
			pCur += uBPP;
			pFBCur += uBPP;
		}
		pCurLine += ulRowBytes;
		pFBCurLine += ulRowBytes;
	}

	/*pCurLine = (unsigned char*)pSrc + ulRowBytes * pRect->top + uBPP * pRect->left;
	pFBCurLine = frameBuffer + ulRowBytes * pRect->top + uBPP * pRect->left;
	for(iy = pRect->top; iy < pRect->bottom; ++iy)
	{
		pCur = pCurLine;
		pFBCur = pFBCurLine;
		
		pFBCur[0] = 255;
		pFBCur[1] = 0;
		pFBCur[2] = 0;
		pFBCur[3] = 255;
			
		
		pCurLine += ulRowBytes;
		pFBCurLine += ulRowBytes;
	}

	pCurLine = (unsigned char*)pSrc + ulRowBytes * pRect->top + uBPP * (pRect->right - 1);
	pFBCurLine = frameBuffer + ulRowBytes * pRect->top + uBPP * (pRect->right - 1);
	for(iy = pRect->top; iy < pRect->bottom; ++iy)
	{
		pCur = pCurLine;
		pFBCur = pFBCurLine;
		
		pFBCur[0] = 255;
		pFBCur[1] = 0;
		pFBCur[2] = 0;
		pFBCur[3] = 255;
			
		
		pCurLine += ulRowBytes;
		pFBCurLine += ulRowBytes;
	}


	pCur = (unsigned char*)pSrc + ulRowBytes * pRect->top + uBPP * (pRect->left);
	pFBCur = frameBuffer + ulRowBytes * pRect->top + uBPP * (pRect->left);
	for(ix = pRect->left; ix < pRect->right; ++ix)
	{
		pFBCur[0] = 255;
		pFBCur[1] = 0;
		pFBCur[2] = 0;
		pFBCur[3] = 255;
		pCur += uBPP;
		pFBCur += uBPP;
	}

	pCur = (unsigned char*)pSrc + ulRowBytes * (pRect->bottom - 1) + uBPP * (pRect->left);
	pFBCur = frameBuffer + ulRowBytes * (pRect->bottom - 1) + uBPP * (pRect->left);
	for(ix = pRect->left; ix < pRect->right; ++ix)
	{
		pFBCur[0] = 255;
		pFBCur[1] = 0;
		pFBCur[2] = 0;
		pFBCur[3] = 255;
		pCur += uBPP;
		pFBCur += uBPP;
	}*/
#else
	unsigned long nCopyWidth = uBPP * NGREOpIRectWidth(*pRect);
	for(iy = pRect->top; iy < pRect->bottom; ++iy)
	{
		memcpy(pFBCurLine, pCurLine, nCopyWidth);
		pCurLine += ulRowBytes;
		pFBCurLine += ulRowBytes;
	}	
#endif
}

NGRE_RESULT NGREFlushDevice(NGREDevice* pDevice)
{
	unsigned long ulRowBytes = NGREBitmapRowBytes(pDevice->pCacheBitmap);
	unsigned int uHeight = NGREBitmapHeight(pDevice->pCacheBitmap);
	unsigned int uWidth = NGREBitmapWidth(pDevice->pCacheBitmap);
	void* pCacheBuffer = NULL;
	unsigned char uBPP = NGREBitmapBytesPerPixel(pDevice->pCacheBitmap);
	NGRE_RESULT lResult = NGREGetBitmapBuffer(pDevice->pCacheBitmap, NGREAllocType_GpuTexture, &pCacheBuffer);
	//assert(pCacheBuffer != NULL);
	unsigned int nClipRectCount = NGREGetDeviceClipRectCount(pDevice);
	unsigned int cx;
	NGREOpIRect clipRect;
	
	for(cx = 0; cx < nClipRectCount; ++cx)
	{
		NGREGetDeviceClipRectByIndex(pDevice, cx, &clipRect);
		NGRECopy2FrameBuffer(pCacheBuffer, ((NGREFBDevice*)(pDevice->pExtra))->frameBuffer , &clipRect, ulRowBytes, uBPP);
	}
	NGREClearDeviceClipRect(pDevice);
	return NGRE_SUCCESS;
}


NGRE_RESULT NGREAddDeviceClipRect(NGREDevice* pDevice, CLPNGREOpIRect pRect)
{
	//assert(pDevice->nClipRectCount <= NGREMaxDeviceClipRectCount);
	if(pDevice->nClipRectCount == NGREMaxDeviceClipRectCount + 1)
	{
		return NGRE_SUCCESS;
	}
	if(pDevice->nClipRectCount == NGREMaxDeviceClipRectCount)
	{
		LPNGREBitmap pCacheBitmap;
		NGREGetBitmapFromDevice(pDevice, &pCacheBitmap);
		pDevice->clipRects[0].left = 0;
		pDevice->clipRects[0].top = 0;
		pDevice->clipRects[0].right = NGREBitmapWidth(pCacheBitmap);
		pDevice->clipRects[0].bottom = NGREBitmapHeight(pCacheBitmap);
		++ (pDevice->nClipRectCount);
		return NGRE_SUCCESS;
	}
	pDevice->clipRects[pDevice->nClipRectCount] = *pRect;
	++(pDevice->nClipRectCount);
	return NGRE_SUCCESS;
}

NGRE_RESULT NGREClearDeviceClipRect(NGREDevice* pDevice)
{
	pDevice->nClipRectCount = 0;
	return NGRE_SUCCESS;
}

NGRE_RESULT NGREGetDeviceClipRectByIndex(NGREDevice* pDevice, unsigned int nIndex, LPNGREOpIRect pRect)
{
	if(pDevice->nClipRectCount == NGREMaxDeviceClipRectCount + 1)
	{
		*pRect = pDevice->clipRects[0];
		return NGRE_SUCCESS;
	}
	else
	{
		if(nIndex >= pDevice->nClipRectCount)
		{
			return NGRE_DEVICE_INVALIDPARAM;
		}
		else
		{
			*pRect = pDevice->clipRects[nIndex];
			return NGRE_SUCCESS;
		}
	}
}

unsigned int NGREGetDeviceClipRectCount(NGREDevice* pDevice)
{
	if(pDevice->nClipRectCount == NGREMaxDeviceClipRectCount + 1)
	{
		return 1;
	}
	return pDevice->nClipRectCount;
}


void NGRECloseDevice(NGREDevice* pDevice)
{
	/*munmap(FrameBuffer, FixedInfo.smem_len);
    close(FrameBufferFD);*/
}