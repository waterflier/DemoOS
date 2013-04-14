#ifndef _NGOS_RENDER_RESOURCE_IDENTIFIER_H_
#define _NGOS_RENDER_RESOURCE_IDENTIFIER_H_
/*
�ṩ�ű���resource id��λ��λͼ��Դ��ʵ��
��resource manager�ṩ�����Ȳ�ͬ�� resource manager�ṩ�κ�id���ڴ��ӳ�䣬 ����Ҫ����Ҫ���ڴ����Ϊbitmap�ṹ������
����bitmap �ڴ浽�Դ��ӳ��
*/

typedef const char* NGREResId;

#define NGRE_DECLARE_RESOURCE(restype) typedef union restype##R{ \
	restype* pResource; \
	NGREResId idResource; \
	}restype##R


typedef struct NGREBitmap{
	void* pExtra;
}NGREBitmap, *LPNGREBitmap;

NGRE_DECLARE_RESOURCE(NGREBitmap);




typedef struct NGREMask{
	void* pExtra;
}NGREMask, *LPNGREMask;

NGRE_DECLARE_RESOURCE(NGREMask);



NGRE_RESULT NGREInitResIdentifier();
NGRE_RESULT NGREGetBitmapFromId(NGREResId idRes, LPNGREBitmap* ppBitmap);
void		NGREReleaseBitmapFromId(NGREResId idRes); 
NGRE_RESULT NGREGetMaskFromId(NGREResId idRes, LPNGREMask* ppMask);
void		NGREReleaseMaskFromId(NGREResId idRes); 
NGRE_RESULT NGREUninitResIdentifier();


#endif