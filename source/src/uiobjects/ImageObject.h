//
// ImageObject.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_IMAGE_OBJECT_H_
#define _NGOS_IMAGE_OBJECT_H_

#include "../kernel/TypeLoader.h"

typedef struct tagImageObject
{
	//��Ϊ��Ⱦ�ǿ���̵ģ����������SetBitmap,Ҳ���뽫���Bitmapת��Ϊһ����app���е���ʱ��Դid
	//������Ⱦ�����õĵ�
	char* strImageResID;

	uint32_t DrawModeIsStretch:4;
	uint32_t DrawModeValign:4;
	uint32_t DrawModeHalign:4;
	uint32_t DrawModeAntiAlias:4;
}ImageObject;

UIObjectTypeInfo* GetImageObjectTypeInfo();

NGOS_API(void) NGOS_ImageObjectSetImageID(NGOS_UIOBJECT_HANDLE hImageObject,const char* imageID);
NGOS_API(const char*) NGOS_ImageObjectGetImageID(NGOS_UIOBJECT_HANDLE hImageObject);



#endif //_NGOS_IMAGE_OBJECT_H_
