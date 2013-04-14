//
// UIObjectTypeLoader.h 
//
//////////////////////////////////////////////////////////////////////////
#include "../kernel/TypeLoader.h"

TypeLoader* NGOS_GetDefaultUIObjectTypeLoader();
NGOS_UIOBJECT_HANDLE NGOS_CreateUIObject(TypeLoader* pLoader,const char* className,const char* id);
