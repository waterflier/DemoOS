extern "C"{
#include "RenderResource.h"
}
#include "SkiaDefine.h"
#include <assert.h>
#include "SkImageDecoder.h"
#include "SkStream.h"


LPNGREBitmap NewSkBitmapFromSkBitmap(SkBitmap* pSkBitmap)
{
	LPNGRESkBitmap pSkBitmapG = (LPNGRESkBitmap)malloc(sizeof(NGRESkBitmap));
	pSkBitmapG->pSkGpuDevice = NULL;
	pSkBitmapG->pSkBitmap = pSkBitmap;
	LPNGREBitmap pNewBitmap = (LPNGREBitmap)malloc(sizeof(NGREBitmap));
	pNewBitmap->pExtra = pSkBitmapG;
	return pNewBitmap;
}


LPNGREBitmap NewSkBitmap(unsigned int uWidth, unsigned int uHeight, unsigned char uBytesPerPixel)
{
	SkBitmap* pSkBitmap = SkNEW(SkBitmap);
	assert(uBytesPerPixel == 4);
	pSkBitmap->setConfig(SkBitmap::kARGB_8888_Config, uWidth, uWidth);

	return NewSkBitmapFromSkBitmap(pSkBitmap);
}



extern "C" NGRE_RESULT NGRECreateBitmap(unsigned int uWidth, unsigned int uHeight, unsigned char uBytesPerPixel, LPNGREBitmap* ppBitmap)
{
	*ppBitmap = NewSkBitmap(uWidth, uHeight, uBytesPerPixel);
	return NGRE_SUCCESS;
}

extern "C" NGRE_RESULT NGREAllocBitmap(LPNGREBitmap pBitmap, NGREAllocType allocType)
{
	LPNGRESkBitmap pSkBitmap = (LPNGRESkBitmap)(pBitmap->pExtra);
	if(allocType & NGREAllocType_mem)
	{
		pSkBitmap->pSkBitmap->allocPixels();
	}
	if(allocType & NGREAllocType_GpuTexture)
	{
		if(pSkBitmap->pSkGpuDevice == NULL)
		{
			GrContextFactory contextFactory;
			//SKGLContextHelper* ctxHelper = contextFactory.getGLContext(GrContextFactory::kNative_GLContextType);
			GrContext* context = contextFactory.get(GrContextFactory::kNative_GLContextType);
			if(pSkBitmap->pSkBitmap->getPixels())
			{
				GrTextureParams texParam(SkShader::kClamp_TileMode, false);
				GrTextureDesc desc;
				desc.fConfig = kSkia8888_GrPixelConfig;
				desc.fFlags = kRenderTarget_GrTextureFlagBit;
				desc.fWidth = pSkBitmap->pSkBitmap->width();
				desc.fHeight = pSkBitmap->pSkBitmap->height();
				desc.fSampleCnt = 0;
				GrCacheID::Key cidKey;
				GrCacheID cid(GrCacheID::GenerateDomain(),cidKey);
				GrTexture* texture = context->createTexture(&texParam,desc,cid,pSkBitmap->pSkBitmap->getPixels(),pSkBitmap->pSkBitmap->rowBytes());
				pSkBitmap->pSkGpuDevice = SkNEW_ARGS(SkGpuDevice, (context, texture));
			}
			else
			{
				pSkBitmap->pSkGpuDevice = SkNEW_ARGS(SkGpuDevice, (context,SkBitmap::kARGB_8888_Config, pSkBitmap->pSkBitmap->width(), pSkBitmap->pSkBitmap->height()));
			}
		}
	}
	return NGRE_SUCCESS;
}

extern "C" unsigned int NGREBitmapWidth(LPNGREBitmap pBitmap)
{
	LPNGRESkBitmap pSkBitmap = (LPNGRESkBitmap)(pBitmap->pExtra);
	return pSkBitmap->pSkBitmap->width();
}

extern "C" unsigned int NGREBitmapHeight(LPNGREBitmap pBitmap)
{
	LPNGRESkBitmap pSkBitmap = (LPNGRESkBitmap)(pBitmap->pExtra);
	return pSkBitmap->pSkBitmap->height();
}

extern "C" unsigned long NGREBitmapRowBytes(LPNGREBitmap pBitmap)
{
	LPNGRESkBitmap pSkBitmap = (LPNGRESkBitmap)(pBitmap->pExtra);
	return pSkBitmap->pSkBitmap->rowBytes();
}

extern "C" unsigned char NGREBitmapBytesPerPixel(LPNGREBitmap pBitmap)
{
	LPNGRESkBitmap pSkBitmap = (LPNGRESkBitmap)(pBitmap->pExtra);
	return pSkBitmap->pSkBitmap->bytesPerPixel();
}


extern "C" NGRE_RESULT NGREGetBitmapBuffer(LPNGREBitmap pBitmap, NGREAllocType allocType, void** ppBitmapBuffer)
{
	return NGRE_SUCCESS;
}

NGRE_RESULT NGRELoadBitmapFromFile(const char* szFilePath, LPNGREBitmap* ppBitmap)
{
	SkImageDecoder * pngDecoder = CreatePNGImageDecoder();
	SkFILEStream    stream(szFilePath);
	SkBitmap* pSkBitmap = SkNEW(SkBitmap);
	bool bRes = pngDecoder->decode(&stream, pSkBitmap, SkBitmap::kARGB_8888_Config, SkImageDecoder::kDecodePixels_Mode);
	if(bRes)
	{
		*ppBitmap = NewSkBitmapFromSkBitmap(pSkBitmap);
		return NGRE_SUCCESS;
	}
	else
	{
		*ppBitmap = NULL;
		return NGRE_RESOURCE_INVALIDPATH;
	}
}