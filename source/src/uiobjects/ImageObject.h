//
// ImageObject.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_IMAGE_OBJECT_H_
#define _NGOS_IMAGE_OBJECT_H_

UIObjectTypeInfo* GetImageObjectTypeInfo();

NGOS_API(void) NGOS_ImageObjectSetImageID(NGOS_UIOBJECT_HANDLE hImageObject,const char* imageID);
NGOS_API(const char*) NGOS_ImageObjectGetImageID(NGOS_UIOBJECT_HANDLE hImageObject);

#endif //_NGOS_IMAGE_OBJECT_H_
