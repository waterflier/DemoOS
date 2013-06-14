#ifndef _NGOS_RENDER_RESOURCE_IDENTIFIER_H_
#define _NGOS_RENDER_RESOURCE_IDENTIFIER_H_
/*
提供脚本中resource id定位到位图资源的实现
跟resource manager提供的粒度不同， resource manager提供任何id到内存的映射， 这里要不仅要把内存解释为bitmap结构还可能
创建bitmap 内存到显存的映射
*/
#include "RenderDefine.h"
#include "RenderResource.h"

#define NGRE_RESOURCE_INVALIDID		1

typedef const char* NGREResId;

#define NGRE_DECLARE_RESOURCE(restype) typedef union restype##R{ \
	restype* pResource; \
	NGREResId idResource; \
	}restype##R


NGRE_DECLARE_RESOURCE(NGREBitmap);


//NGRE_DECLARE_RESOURCE(NGREMask);


NGRE_DECLARE_RESOURCE(NGREOpColor);



NGRE_RESULT NGREInitResIdentifier();
NGRE_RESULT NGREGetBitmapFromId(NGREResId idRes, LPNGREBitmap* ppBitmap);
void		NGREReleaseBitmapFromId(NGREResId idRes); 
//NGRE_RESULT NGREGetMaskFromId(NGREResId idRes, LPNGREMask* ppMask);
//void		NGREReleaseMaskFromId(NGREResId idRes); 
NGRE_RESULT NGREGetColorFromId(NGREResId idRes, NGREOpColor** ppColor);
void		NGREReleaseColorFromId(NGREResId idRes);
NGRE_RESULT NGREUninitResIdentifier();


#endif