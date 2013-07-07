#include "RenderDevice.h"
#include "RenderResource.h"
#include <stdlib.h>


NGRE_RESULT NGREGetBitmapFromDevice(NGREDevice* pDevice, LPNGREBitmap* ppBitmap)
{
	//assert(pDevice != NULL);
	if(pDevice->pCacheBitmap == NULL)
	{
		int nDeviceHeight, nDeviceWidth;
		NGREGetDeviceSize(pDevice, &nDeviceWidth, &nDeviceHeight);
		NGRE_RESULT lResult = NGRECreateBitmap(nDeviceWidth, nDeviceHeight, 4, &(pDevice->pCacheBitmap));
		//assert(lResult == NGRE_SUCCESS);
		lResult = NGREAllocBitmap(pDevice->pCacheBitmap, NGREAllocType_GpuTexture);
		//assert(lResult == NGRE_SUCCESS);
		*ppBitmap = pDevice->pCacheBitmap;
		return lResult;
	}
	*ppBitmap = pDevice->pCacheBitmap;
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


