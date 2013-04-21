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
			pFbDevice->fbDesc = open("/dev/fb0", O_RDWR);
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

NGRE_RESULT NGREGetBitmapFromDevice(NGREDevice* pDevice, LPNGREBitmap* ppBitmap)
{
	//assert(pDevice != NULL);
	if(pDevice->pCacheBitmap == NULL)
	{
		NGREFBDevice* pFBDevice = (NGREFBDevice*)(pDevice->pExtra);
		NGRE_RESULT lResult = NGRECreateBitmap(pFBDevice->origVarInfo.xres,
			pFBDevice->origVarInfo.yres, pFBDevice->origVarInfo.bits_per_pixel, &(pDevice->pCacheBitmap));
		//assert(lResult == NGRE_SUCCESS);
		lResult = NGREAllocBitmap(pDevice->pCacheBitmap, NGREAllocType_GpuTexture);
		//assert(lResult == NGRE_SUCCESS);
		*ppBitmap = pDevice->pCacheBitmap;
		return lResult;
	}
	*ppBitmap = pDevice->pCacheBitmap;
	return NGRE_SUCCESS;
}

NGRE_RESULT NGREFlushDevice(NGREDevice* pDevice)
{
	unsigned long ulRowBytes = NGREBitmapRowBytes(pDevice->pCacheBitmap);
	unsigned int uHeight = NGREBitmapHeight(pDevice->pCacheBitmap);
	void* pCacheBuffer = NULL;
	NGRE_RESULT lResult = NGREGetBitmapBuffer(pDevice->pCacheBitmap, NGREAllocType_GpuTexture, &pCacheBuffer);
	//assert(pCacheBuffer != NULL);
	memcpy(((NGREFBDevice*)(pDevice->pExtra))->frameBuffer, pCacheBuffer, ulRowBytes * uHeight);	
	return NGRE_SUCCESS;
}

void NGRECloseDevice(NGREDevice* pDevice)
{
	/*munmap(FrameBuffer, FixedInfo.smem_len);
    close(FrameBufferFD);*/
}