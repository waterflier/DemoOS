//
// UIObjectMap.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_UIOBJECT_INDEX_H_
#define _NGOS_UIOBJECT_INDEX_H_

#include "../../toolkit/hash_map.h"

typedef struct tagUIObjectMap
{
    map_t mapImp;
}UIObjectMap;

UIObjectMap* CreateUIObjectMap();
void FreeUIObjectMap(UIObjectMap* pSelf);

int UIObjectMapInsert(UIObjectMap* pSelf,NGOS_UIOBJECT_HANDLE hUIObject,const char* id);
NGOS_UIOBJECT_HANDLE UIObjectMapFind(UIObjectMap* pSelf,const char* id);
int UIObjectMapErase(UIObjectMap* pSelf,const char* id);
int UIObjectMapClean(UIObjectMap* pSelf);

#endif //_NGOS_UIOBJECT_INDEX_H_