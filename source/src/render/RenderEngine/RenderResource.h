#ifndef _NGOS_RENDER_RESOURCE_H_
#define _NGOS_RENDER_RESOURCE_H_
#include "RenderDefine.h"

#define NGRE_RESOURCE_INVALIDPATH	1;

typedef struct NGREBitmap{
	void* pExtra;
}NGREBitmap, *LPNGREBitmap;

#define NGREOpIInv  (0x7fffffff)
typedef struct NGREOpIRect{
	int left;
	int top;
	int right;
	int bottom;
}NGREOpIRect, *LPNGREOpIRect;
typedef const LPNGREOpIRect CLPNGREOpIRect;
#define NGREOpIRectWidth(rect) ((rect).right - (rect).left)
#define NGREOpIRectHeight(rect) ((rect).bottom - (rect).top)


//typedef struct NGREMask{
//	void* pExtra;
//}NGREMask, *LPNGREMask;

typedef struct NGREOpColor{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
}NGREOpColor, *LPNGREOpColor;

NGRE_RESULT NGRECreateBitmap(unsigned int uWidth, unsigned int uHeight, unsigned char uBytesPerPixel, LPNGREBitmap* ppBitmap);
unsigned int NGREBitmapWidth(LPNGREBitmap pBitmap);
unsigned int NGREBitmapHeight(LPNGREBitmap pBitmap);
unsigned long NGREBitmapRowBytes(LPNGREBitmap pBitmap);
unsigned char NGREBitmapBytesPerPixel(LPNGREBitmap pBitmap);
NGRE_RESULT NGRELoadBitmapFromFile(const char* szFilePath, LPNGREBitmap* ppBitmap);

typedef long						NGREAllocType;
#define NGREAllocType_mem			0x1
#define NGREAllocType_GpuTexture	0x2
NGRE_RESULT NGREAllocBitmap(LPNGREBitmap pBitmap, NGREAllocType allocType);
NGRE_RESULT NGREGetBitmapBuffer(LPNGREBitmap pBitmap, NGREAllocType allocType, void** ppBitmapBuffer);


///for testScript
NGRE_RESULT NGREPrintBimtapToFile(LPNGREBitmap pBitmap, const char* szFilePath);

#endif