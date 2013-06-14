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

void NGREGetRectIfNull(CLPNGREOpIRect pSrcRect, LPNGREOpIRect pRect, NGREBitmapR pBmp)
{
	if(pSrcRect == NULL)
	{
		pRect->left = 0;
		pRect->top = 0;
		pRect->right = NGREBitmapWidth(pBmp.pResource);
		pRect->bottom = NGREBitmapHeight(pBmp.pResource);
	}
	else
	{
		(*pRect) = (*pSrcRect);
	}
}

NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
{
	NGREResId idBmpSrc = pBmpSrc.idResource;
	NGRE_RESULT lResult = NGREGetBitmapFromId(idBmpSrc, &(pBmpSrc.pResource));
	NGREOpIRect rectSrc;
	NGREGetRectIfNull(pRectSrc, &rectSrc, pBmpSrc);

	NGREResId idBmpDest = pBmpDest.idResource;
	lResult = NGREGetDeviceBitmapIfNull(&pBmpDest);

	NGREOpIRect rectDest;
	rectDest = *pRectDest;
	if(pRectDest->right == NGREOpIInv)
	{
		rectDest.right = rectDest.left + rectSrc.right - rectSrc.left;
	}
	if(pRectDest->bottom == NGREOpIInv)
	{
		rectDest.bottom = rectDest.top + rectSrc.bottom - rectSrc.top;
	}
	
	/*NGREResId idMask = NULL;
	if(pParam != NULL)
	{
		if(pParam->pMask.idResource != NULL)
		{
			idMask = pParam->pMask.idResource;
			lResult = NGREGetMaskFromId(idMask, &(pParam->pMask.pResource));
		}
	}*/
	

	lResult = NGREOpBlendBitmap(pBmpSrc, &rectSrc, pBmpDest, &rectDest, pParam);

	/*if(idMask != NULL)
	{
		NGREReleaseMaskFromId(idMask);
	}*/
	if(idBmpDest != NULL)
	{
		NGREReleaseBitmapFromId(idBmpDest);
	}
	NGREReleaseBitmapFromId(idBmpSrc);


	return lResult;
}


NGRE_RESULT NGREOpFillRectR(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, NGREOpColorR pColor, CLPNGREOpParam pParam)
{
	NGREResId idBmpDest = pBmpDest.idResource;
	long lResult = NGREGetDeviceBitmapIfNull(&pBmpDest);
	NGREOpIRect rectDest;
	NGREGetRectIfNull(pRectDest, &rectDest, pBmpDest);

	NGREResId idColor = pColor.idResource;
	lResult = NGREGetColorFromId(idColor, &(pColor.pResource));

	lResult = NGREOpFillRect(pBmpDest, &rectDest, pColor.pResource, pParam);

	if(idBmpDest != NULL)
	{
		NGREReleaseBitmapFromId(idBmpDest);
	}
}

NGRE_RESULT NGREOpEraseBitmapR(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, NGREOpColorR pColor, CLPNGREOpParam pParam)
{
	NGREResId idBmpDest = pBmpDest.idResource;
	long lResult = NGREGetDeviceBitmapIfNull(&pBmpDest);
	NGREOpIRect rectDest;
	NGREGetRectIfNull(pRectDest, &rectDest, pBmpDest);

	NGREResId idColor = pColor.idResource;
	lResult = NGREGetColorFromId(idColor, &(pColor.pResource));

	lResult = NGREOpEraseBitmap(pBmpDest, &rectDest, pColor.pResource, pParam);

	if(idBmpDest != NULL)
	{
		NGREReleaseBitmapFromId(idBmpDest);
	}
}


