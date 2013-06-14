#include "ResourceIdentifier.h"
#include "ResourceManager.h"
#include <stdlib.h>
#include <assert.h>

NGRE_RESULT NGREInitResIdentifier()
{
	NGRM_RESULT lResult = NGRM_Init(NULL);
	return NGRE_SUCCESS;
}

NGRE_RESULT NGREGetBitmapFromId(NGREResId idRes, LPNGREBitmap* ppBitmap)
{
	////先简单实现为从硬盘加载到显存
	NGREBitmap* pBitmap = NULL;
	NGRM_RESULT lResult = NGRM_GetResource(NGRM_ResType_Bitmap, idRes, &pBitmap);
	if(pBitmap == NULL)
	{
		return 	NGRE_RESOURCE_INVALIDID;
	}
	lResult = NGREAllocBitmap(pBitmap, NGREAllocType_GpuTexture);
	if(lResult != NGRE_SUCCESS)
	{
		return lResult;
	}
	*ppBitmap = pBitmap;
	return NGRE_SUCCESS;
}

void		NGREReleaseBitmapFromId(NGREResId idRes)
{
	
}

NGRE_RESULT NGREGetMaskFromId(NGREResId idRes, LPNGREMask* ppMask)
{
	assert(0);
}

void		NGREReleaseMaskFromId(NGREResId idRes)
{
	assert(0);
}

NGRE_RESULT NGREUninitResIdentifier()
{
	NGRM_Uninit();
}
