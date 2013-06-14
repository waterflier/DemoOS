#ifndef _NGOS_RENDER_OPERATION_H_
#define _NGOS_RENDER_OPERATION_H_
#include "RenderScript.h"
#include "ResourceIdentifier.h"
///����Ⱦʵ���ṩ�Ľӿڣ� ��script�н��������������ִ��
//���⣺ 1. ��layer��֧�ַ�ʽ������mask transform�ȣ�����operation�Ľӿ��д��루����XLGraphic��ʵ�ַ�ʽ���� ������ʽ���󲿷�ͼ�οⶼ��������
//		2. ��������ϵ��	
//      3. demo�׶���Ҫʵ�ֵĲ���������Щ�� ����kakee����Ƹ�




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