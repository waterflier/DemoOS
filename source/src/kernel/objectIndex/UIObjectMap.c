//
// UIObjectMap.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./UIObjectMap.h"

uint32_t hashstring(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

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
        
        ret = hashmap_put(pSelf->mapImp,hashstring(id),(any_t)hUIObject);
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
        ret = hashmap_get(pSelf->mapImp,hashstring(id),&result);
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
        ret = hashmap_remove(pSelf->mapImp,hashstring(id));
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

