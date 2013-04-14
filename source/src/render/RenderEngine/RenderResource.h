#ifndef _NGOS_RENDER_RESOURCE_H_
#define _NGOS_RENDER_RESOURCE_H_
#include "RenderDefine.h"
typedef struct NGREBitmap{
	void* pExtra;
}NGREBitmap, *LPNGREBitmap;


typedef struct NGREMask{
	void* pExtra;
}NGREMask, *LPNGREMask;

NGRE_RESULT NGRECreateBitmap(unsigned int uWidth, unsigned int uHeight, unsigned char uBytesPerPixel, LPNGREBitmap* ppBitmap);
unsigned int NGREBitmapWidth(LPNGREBitmap pBitmap);
unsigned int NGREBitmapHeight(LPNGREBitmap pBitmap);
unsigned long NGREBitmapRowBytes(LPNGREBitmap pBitmap);
unsigned char NGREBitmapBytesPerPixel(LPNGREBitmap pBitmap);

typedef long						NGREAllocType;
#define NGREAllocType_mem			0x1
#define NGREAllocType_GpuTexture	0x2
NGRE_RESULT NGREAllocBitmap(LPNGREBitmap pBitmap, NGREAllocType allocType);
NGRE_RESULT NGREGetBitmapBuffer(LPNGREBitmap pBitmap, NGREAllocType allocType, void** ppBitmapBuffer);

#endif