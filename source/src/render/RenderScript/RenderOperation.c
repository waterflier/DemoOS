#include "RenderOperation.h"
#include "ResourceIdentifier.h"
#include "RenderDevice.h"


NGRE_RESULT NGREGetDeviceBitmapIfNull(NGREBitmapR* ppBmp)
{
	NGREResId idBmpDest = ppBmp->idResource;
	NGRE_RESULT lResult;
	if(idBmpDest == NULL)
	{
		NGREDevice* pDevice = NULL;
		lResult = NGREOpenDevice(&pDevice);
		lResult = NGREGetBitmapFromDevice(pDevice, (LPNGREBitmap*)(&(ppBmp->pResource)));
	}
	else
	{
		lResult = NGREGetBitmapFromId(idBmpDest, &(ppBmp->pResource));
	}
	return lResult;
}

NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
{
	NGREResId idBmpSrc = pBmpSrc.idResource;
	NGRE_RESULT lResult = NGREGetBitmapFromId(idBmpSrc, &(pBmpSrc.pResource));
	NGREOpIRect rectSrc;
	LPNGREOpIRect pTmpRectSrc = (LPNGREOpIRect)pRectSrc;
	if(pRectSrc == NULL)
	{
		rectSrc.left = 0;
		rectSrc.top = 0;
		rectSrc.right = NGREBitmapWidth(pBmpSrc.pResource);
		rectSrc.bottom = NGREBitmapHeight(pBmpSrc.pResource);
		pTmpRectSrc = &rectSrc;
	}

	NGREResId idBmpDest = pBmpDest.idResource;
	lResult = NGREGetDeviceBitmapIfNull(&pBmpDest);

	NGREOpIRect rectDest;
	rectDest = *pRectDest;
	if(pRectDest->right == NGREOpIInv)
	{
		rectDest.right = rectDest.left + pTmpRectSrc->right - pTmpRectSrc->left;
	}
	if(pRectDest->bottom == NGREOpIInv)
	{
		rectDest.bottom = rectDest.top + pTmpRectSrc->bottom - pTmpRectSrc->top;
	}
	
	NGREResId idMask = NULL;
	if(pParam != NULL)
	{
		if(pParam->pMask.idResource != NULL)
		{
			idMask = pParam->pMask.idResource;
			lResult = NGREGetMaskFromId(idMask, &(pParam->pMask.pResource));
		}
	}
	

	lResult = NGREOpBlendBitmap(pBmpSrc, pTmpRectSrc, pBmpDest, &rectDest, pParam);

	if(idMask != NULL)
	{
		NGREReleaseMaskFromId(idMask);
	}
	if(idBmpDest != NULL)
	{
		NGREReleaseBitmapFromId(idBmpDest);
	}
	NGREReleaseBitmapFromId(idBmpSrc);


	return lResult;
}


NGRE_RESULT NGREOpFillRectR(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam)
{

}