#ifndef _NGOS_RENDER_OPERATION_H_
#define _NGOS_RENDER_OPERATION_H_
#include "RenderScript.h"
#include "ResourceIdentifier.h"
///����Ⱦʵ���ṩ�Ľӿڣ� ��script�н��������������ִ��
//���⣺ 1. ��layer��֧�ַ�ʽ������mask transform�ȣ�����operation�Ľӿ��д��루����XLGraphic��ʵ�ַ�ʽ���� ������ʽ���󲿷�ͼ�οⶼ��������
//		2. ��������ϵ��	
//      3. demo�׶���Ҫʵ�ֵĲ���������Щ�� ����kakee����Ƹ�

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