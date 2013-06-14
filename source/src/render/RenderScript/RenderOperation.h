#ifndef _NGOS_RENDER_OPERATION_H_
#define _NGOS_RENDER_OPERATION_H_
#include "RenderScript.h"
#include "ResourceIdentifier.h"
///给渲染实现提供的接口， 从script中解析出操作后调用执行
//问题： 1. 对layer的支持方式，包括mask transform等（具体operation的接口中传入（现在XLGraphic的实现方式）， 上下文式（大部分图形库都这样））
//		2. 浮点坐标系？	
//      3. demo阶段需要实现的操作包括哪些， 依赖kakee的设计稿




typedef float NGREOpFMatrix33[9];
void NGREOpSetMatrixTranslate(NGREOpFMatrix33 matrix, float x, float y);
void NGREOpSetMatrixScale(NGREOpFMatrix33 matrix, float x, float y);
void NGREOpSetMatrixRotate(NGREOpFMatrix33 matrix, float d, float cx, float cy);
void NGREOpCatMatrix(NGREOpFMatrix33 resultMatrix, NGREOpFMatrix33 srcMatrix);

typedef enum NGREOpParamFlag{
	OpParamClipRect = 0x01,
	OpParamMask = 0x02,
	OpParamMatrix = 0x04,
}NGREOpParamFlag;
typedef struct NGREOpParamHeader{
	unsigned int	cbSize;	
	NGREOpParamFlag flag;
}NGREOpParamHeader ;

typedef struct NGREOpParam{
	NGREOpParamHeader header;
	NGREOpIRect clipRect;
	NGREOpFMatrix33 matrix;
}NGREOpParam, *LPNGREOpParam;
typedef const LPNGREOpParam CLPNGREOpParam;

NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpBlendBitmap(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpFillRectR(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, NGREOpColorR pColor, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpFillRect(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpEraseBitmapR(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, NGREOpColorR pColor, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpEraseBitmap(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam);

#endif