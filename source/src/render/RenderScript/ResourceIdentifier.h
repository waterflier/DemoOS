#ifndef _NGOS_RENDER_RESOURCE_IDENTIFIER_H_
#define _NGOS_RENDER_RESOURCE_IDENTIFIER_H_
/*
�ṩ�ű���resource id��λ��λͼ��Դ��ʵ��
��resource manager�ṩ�����Ȳ�ͬ�� resource manager�ṩ�κ�id���ڴ��ӳ�䣬 ����Ҫ����Ҫ���ڴ����Ϊbitmap�ṹ������
����bitmap �ڴ浽�Դ��ӳ��
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