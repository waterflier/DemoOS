#include "RenderOperation.h"
#include "ResourceIdentifier.h"

NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
{
	NGREResId idBmpSrc = pBmpSrc.idResource;
	NGRE_RESULT lResult = NGREGetBitmapFromId(idBmpSrc, &(pBmpSrc.pResource));
	NGREResId idBmpDest = pBmpDest.idResource;
	lResult = NGREGetBitmapFromId(idBmpDest, &(pBmpDest.pResource));
	NGREResId idMask = pParam->pMask.idResource;
	lResult = NGREGetMaskFromId(idMask, &(pParam->pMask.pResource));
	

	lResult = NGREOpBlendBitmap(pBmpSrc, pRectSrc, pBmpDest, pRectDest, pParam);


	NGREReleaseMaskFromId(idMask);
	NGREReleaseBitmapFromId(idBmpDest);
	NGREReleaseBitmapFromId(idBmpSrc);


	return lResult;
}