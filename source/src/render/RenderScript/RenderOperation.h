#ifndef _NGOS_RENDER_OPERATION_H_
#define _NGOS_RENDER_OPERATION_H_
#include "RenderScript.h"
#include "ResourceIdentifier.h"
///给渲染实现提供的接口， 从script中解析出操作后调用执行
//问题： 1. 对layer的支持方式，包括mask transform等（具体operation的接口中传入（现在XLGraphic的实现方式）， 上下文式（大部分图形库都这样））
//		2. 浮点坐标系？	
//      3. demo阶段需要实现的操作包括哪些， 依赖kakee的设计稿

#define NGREOpIInv  (0x7fffffff)
typedef struct NGREOpIRect{
	int left;
	int top;
	int right;
	int bottom;
}NGREOpIRect, *LPNGREOpIRect;
typedef const LPNGREOpIRect CLPNGREOpIRect;

typedef struct NGREOpColor{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
}NGREOpColor, *LPNGREOpColor;

typedef float NGREOpFMatrix44[4][4];

typedef struct NGREOpParam{
	unsigned int	cbSize;				
	CLPNGREOpIRect	pRectRegion;
	CLPNGREOpIRect	pRectMaskRegion;
	NGREMaskR pMask;

	void* pExtra;
}NGREOpParam, *LPNGREOpParam ;
typedef const LPNGREOpParam CLPNGREOpParam;

NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpBlendBitmap(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpFillRectR(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam);
NGRE_RESULT NGREOpFillRect(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam);


#endif