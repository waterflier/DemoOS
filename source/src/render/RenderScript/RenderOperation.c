#include "RenderOperation.h"
#include "ResourceIdentifier.h"
#include "RenderDevice.h"

NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
{
	NGREResId idBmpSrc = pBmpSrc.idResource;
	NGRE_RESULT lResult = NGREGetBitmapFromId(idBmpSrc, &(pBmpSrc.pResource));
	NGREResId idBmpDest = pBmpDest.idResource;
	if(idBmpDest == NULL)
	{
		NGREDevice* pDevice = NULL;
		lResult = NGREOpenDevice(&pDevice);
		lResult = NGREGetBitmapFromDevice(pDevice, (LPNGREBitmap*)(&(pBmpDest.pResource)));
	}
	else
	{
		lResult = NGREGetBitmapFromId(idBmpDest, &(pBmpDest.pResource));
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
	

	lResult = NGREOpBlendBitmap(pBmpSrc, pRectSrc, pBmpDest, pRectDest, pParam);

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