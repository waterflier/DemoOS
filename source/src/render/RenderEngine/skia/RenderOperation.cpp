extern "C"
{
#include "RenderOperation.h"
}
#include "SkiaDefine.h"

///skia й╣ож render operation
#ifdef NGRE_GL
extern "C" NGRE_RESULT NGREOpBlendBitmap(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
{
	SkCanvas canvas(NGREBitmap2SkGpuDevice(pBmpDest.pResource)); 
	SkRect srcRect = NGREOpIRect2SkRect(*pRectSrc);
	SkRect destRect = NGREOpIRect2SkRect(*pRectDest);

	canvas.drawBitmapRectToRect(NGREBitmap2SkGpuDevice(pBmpSrc.pResource)->accessBitmap(false), &srcRect,destRect, NULL);

	return NGRE_SUCCESS;
}
#else
extern "C" NGRE_RESULT NGREOpBlendBitmap(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
{
	SkCanvas canvas(*NGREBitmap2SkBitmap(pBmpDest.pResource)); 
	SkRect srcRect = NGREOpIRect2SkRect(*pRectSrc);
	SkRect destRect = NGREOpIRect2SkRect(*pRectDest);

	canvas.drawBitmapRectToRect(*NGREBitmap2SkBitmap(pBmpSrc.pResource), &srcRect,destRect, NULL);

	return NGRE_SUCCESS;
}

#endif//NGRE_GL