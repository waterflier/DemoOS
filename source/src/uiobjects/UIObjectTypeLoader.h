//
// UIObjectTypeLoader.h 
//
//////////////////////////////////////////////////////////////////////////
#include "../kernel/TypeLoader.h"

TypeLoader* NGOS_GetDefaultTypeLoader();
NGOS_UIOBJECT_HANDLE NGOS_CreateUIObject(TypeLoader* pLoader,const char* className,const char* id);
NGOS_ANIMATION_HANDLE NGOS_CreateAnimation(TypeLoader* pLoader,const char* className);
