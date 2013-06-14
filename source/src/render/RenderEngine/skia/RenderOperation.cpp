extern "C"
{
#include "RenderOperation.h"
}
#include "SkiaDefine.h"

///skia й╣ож render operation
enum{
	SkiaMatrixTranslateX,
	SkiaMatrixTranslateY,
	SkiaMatrixScaleX,
	SkiaMatrixScaleY,
	SkiaMatrixRotate,
	SkiaMatrixCentreX,
	SkiaMatrixCentreY,
}NGRESkiaMatrixIndex;

extern "C" NGRE_RESULT NGREOpBlendBitmap(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
{
	SkCanvas canvas(NGREBitmap2CanvasInput(pBmpDest.pResource)); 
	SkIRect srcRect = NGREOpIRect2SkIRect(*pRectSrc);
	if(pParam != NULL && (((NGREOpParamHeader*)pParam)->flag & OpParamMatrix))
	{
		if(NGREOpIRectWidth(*pRectSrc) && NGREOpIRectHeight(*pRectSrc))
		{
			SkMatrix tempMatrix;
			tempMatrix.reset();
			tempMatrix.postScale(pParam->matrix[SkiaMatrixScaleX] * NGREOpIRectWidth(*pRectDest) / NGREOpIRectWidth(*pRectSrc) ,
				pParam->matrix[SkiaMatrixScaleY] * NGREOpIRectHeight(*pRectDest) / NGREOpIRectHeight(*pRectSrc));
			SkBitmap subBmpToDraw;
			NGREBitmap2BitmapInput(pBmpSrc.pResource).extractSubset(&subBmpToDraw, srcRect);
			tempMatrix.postRotate(pParam->matrix[SkiaMatrixRotate],pParam->matrix[SkiaMatrixCentreX] - srcRect.fLeft,
				pParam->matrix[SkiaMatrixCentreY] - srcRect.fTop);
			tempMatrix.postTranslate(pParam->matrix[SkiaMatrixTranslateX] + pRectDest->left, 
				pParam->matrix[SkiaMatrixTranslateY] + pRectDest->top);
			canvas.drawBitmapMatrix(subBmpToDraw, tempMatrix);
		}
	}
	else
	{
		SkRect destRect = NGREOpIRect2SkRect(*pRectDest);

		canvas.drawBitmapRect(NGREBitmap2BitmapInput(pBmpSrc.pResource), &srcRect,destRect, NULL);
	}


	return NGRE_SUCCESS;
}

NGRE_RESULT NGREOpFillRect(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam)
{
	SkCanvas canvas(NGREBitmap2CanvasInput(pBmpDest.pResource)); 
	SkIRect destRect = NGREOpIRect2SkIRect(*pRectDest);
	SkPaint paint;
	SkColor color;
	memcpy(&color, pColor, sizeof(SkColor));
	paint.setColor(color);
	paint.setStyle(SkPaint::kFill_Style);
	canvas.drawIRect(destRect, paint);

	return NGRE_SUCCESS;
}

NGRE_RESULT NGREOpEraseBitmap(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam)
{
	SkColor color;
	memcpy(&color, pColor, sizeof(SkColor));
	SkBitmap subBmp;
	SkIRect destRect = NGREOpIRect2SkIRect(*pRectDest);
	NGREBitmap2BitmapInput(pBmpDest.pResource).extractSubset(&subBmp, destRect);
	subBmp.eraseColor(color);
	return NGRE_SUCCESS;
}


void NGREOpSetMatrixTranslate(NGREOpFMatrix33 matrix, float x, float y)
{
	matrix[SkiaMatrixTranslateX] = x;
	matrix[SkiaMatrixTranslateY] = y;
}

void NGREOpSetMatrixScale(NGREOpFMatrix33 matrix, float x, float y)
{
	matrix[SkiaMatrixScaleX] = x;
	matrix[SkiaMatrixScaleY] = y;
}

void NGREOpSetMatrixRotate(NGREOpFMatrix33 matrix, float d, float cx, float cy)
{
	matrix[SkiaMatrixRotate] = d;
	matrix[SkiaMatrixCentreX] = cx;
	matrix[SkiaMatrixCentreY] = cy;
}

void NGREOpCatMatrix(NGREOpFMatrix33 resultMatrix, NGREOpFMatrix33 srcMatrix)
{
	//assert(0);
}


