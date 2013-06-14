extern "C"{
#include "RenderResource.h"
}
#include "SkiaDefine.h"
#include <assert.h>
#include "SkImageDecoder.h"
#include "SkStream.h"
#include "SkImageEncoder.h"
#ifndef ANDROID
#include "GrContextFactory.h"
#endif


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
	pSkBitmap->setConfig(SkBitmap::kARGB_8888_Config, uWidth, uHeight);

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
		if(pSkBitmap->pSkBitmap->getPixels() == NULL)
		{
			pSkBitmap->pSkBitmap->allocPixels();
		}
	}

	if(allocType & NGREAllocType_GpuTexture)
	{
	#ifdef NGRE_GL
		if(pSkBitmap->pSkGpuDevice == NULL)
		{
		#ifdef ANDROID
			static GrContext* s_pGLContext = NULL;
			if(s_pGLContext == NULL)
			{
				s_pGLContext = GrContext::CreateGLShaderContext();
			}
			GrContext* context = s_pGLContext;
			pSkBitmap->pSkGpuDevice = SkNEW_ARGS(SkGpuDevice, (context,*(pSkBitmap->pSkBitmap) ,context->getRenderTarget()));
		#else
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
		#endif
		}
	#else
		NGREAllocBitmap(pBitmap, NGREAllocType_mem);
	#endif //NGRE_GL
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
	if(allocType == NGREAllocType_mem)
	{
		LPNGRESkBitmap pSkBitmap = (LPNGRESkBitmap)(pBitmap->pExtra);
		*ppBitmapBuffer = pSkBitmap->pSkBitmap->getPixels();
	}
	else if(allocType == NGREAllocType_GpuTexture)
	{
		NGREBitmap2BitmapInput(pBitmap).getPixels();
	}
	return NGRE_SUCCESS;
}

NGRE_RESULT NGRELoadBitmapFromFile(const char* szFilePath, LPNGREBitmap* ppBitmap)
{
#ifdef ANDROID
	SkBitmap* pSkBitmap = SkNEW(SkBitmap);
	bool bRes = SkImageDecoder::DecodeFile(szFilePath, pSkBitmap,
                          SkBitmap::kARGB_8888_Config, SkImageDecoder::kDecodePixels_Mode);
#else
	SkImageDecoder * pngDecoder = CreatePNGImageDecoder();
	SkFILEStream    stream(szFilePath);
	SkBitmap* pSkBitmap = SkNEW(SkBitmap);
	bool bRes = pngDecoder->decode(&stream, pSkBitmap, SkBitmap::kARGB_8888_Config, SkImageDecoder::kDecodePixels_Mode);
	delete pngDecoder;
#endif
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

NGRE_RESULT NGREPrintBimtapToFile(LPNGREBitmap pBitmap, const char* szFilePath)
{
	bool success = SkImageEncoder::EncodeFile(szFilePath, *(((LPNGRESkBitmap)(pBitmap->pExtra))->pSkBitmap),
                               SkImageEncoder::kPNG_Type, 100);
	return NGRE_SUCCESS;
}