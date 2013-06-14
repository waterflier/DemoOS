//
// UIObjectMap.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./UIObjectMap.h"

UIObjectMap* CreateUIObjectMap()
{
    UIObjectMap* pResult =(UIObjectMap*) malloc(sizeof(UIObjectMap));
    pResult->mapImp = hashmap_new();
    return pResult;
}

void FreeUIObjectMap(UIObjectMap* pSelf)
{
    if(pSelf)
    {
        hashmap_free(pSelf->mapImp);
        free(pSelf);
    }
}

int UIObjectMapInsert(UIObjectMap* pSelf,NGOS_UIOBJECT_HANDLE hUIObject,const char* id)
{
    int ret;
    if(hUIObject && id && pSelf)
    {
        
        ret = hashmap_put(pSelf->mapImp,id,(any_t)hUIObject);
        if(ret == MAP_OK)
        {
            return 0;
        }
    }

    return -1;
}

NGOS_UIOBJECT_HANDLE UIObjectMapFind(UIObjectMap* pSelf,const char* id)
{
    int ret;
    if(pSelf && id)
    {
        any_t result;
        ret = hashmap_get(pSelf->mapImp,id,&result);
        if(ret == MAP_OK)
        {
            return result;
        }
    }
    
    return NULL;
}

int UIObjectMapErase(UIObjectMap* pSelf,const char* id)
{
    int ret;
    if(pSelf && id)
    {
        ret = hashmap_remove(pSelf->mapImp,id);
        if(ret == MAP_OK)
        {
            return 0;
        }
    }

    return NULL;
}

int UIObjectMapClean(UIObjectMap* pSelf)
{
    int len = hashmap_length(pSelf->mapImp);
    int i=0;
    for(i=0;i<len;i++)
    {
        any_t result;
        hashmap_get_one(pSelf, &result, 1);
    }

    return 0;
}

