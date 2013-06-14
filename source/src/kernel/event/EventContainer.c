//
// EventContainer.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./EventContainer.h"


EventContainer* CreateEventContainer(uint16_t flag)
{
    EventContainer* pResult = (EventContainer*) malloc(sizeof(EventContainer));
    memset(pResult,0,sizeof(EventContainer));
    pResult->EventContainerFlag = flag;
    pResult->Size = EVENT_CONTAINER_INNER_SIZE;
    pResult->Ptr = pResult->Buffer;
    return pResult;
}

void DestroyEventContainer(EventContainer* pSelf)
{
    if(pSelf)
    {
        CleanEventContainer(pSelf);

        if(pSelf->IsInFireProgress)
        {
            //not free imp
        }
        else
        {    
            free(pSelf);
        }
    }
}

uint32_t AttachEvent(EventContainer* pSelf,void* pfnCallback,UserDataContext*  pUserdata)
{
    EventCallbackNode* pWriteNode = NULL;
    if(pSelf == NULL)
    {
        return 0;
    }
   

    if(pSelf->Length < pSelf->Size)
    {
        pWriteNode = pSelf->Ptr + pSelf->Length;
        pSelf->Length++;
    }
    else
    {
        EventCallbackNode* pTemp = pSelf->Ptr;
        uint32_t newSize = pSelf->Size * 2;
        if(newSize > EVENT_CONTAINER_MAX_SIZE)
        {
            return 0;
        }

        pSelf->Ptr = malloc(newSize*sizeof(EventCallbackNode));
        memcpy(pSelf->Ptr,pTemp,sizeof(EventCallbackNode)*pSelf->Length);
        pSelf->Size = newSize;
        if(pTemp != pSelf->Buffer)
        {
            free(pTemp);
        }

        pWriteNode = pSelf->Ptr + pSelf->Length;
        pSelf->Length++;
    }

    if(pWriteNode)
    {
        pSelf->CurrentCookie++;
        pWriteNode->Cookie = pSelf->CurrentCookie;
        pWriteNode->pfnCallback = pfnCallback;
        pWriteNode->pUserData = pUserdata;
        pWriteNode->IsRemoved = FALSE;
        if(pWriteNode->pUserData)
        {
            if(pWriteNode->pUserData->fnAddRefUserData)
            {
                pWriteNode->pUserData->fnAddRefUserData(pWriteNode->pUserData);
            }  
        }

        uint32_t result = pSelf->EventContainerFlag;
        result<<16;
        result |= pWriteNode->Cookie;

        return result;
    }
}

int DetachEvent(EventContainer* pSelf,uint32_t cookie)
{
    if(pSelf == NULL)
    {
        return -1;
    }
    
    if(cookie>>16 != pSelf->EventContainerFlag)
    {
        return -1;
    }

    uint16_t removeCookie = cookie & 0x0000FFFF;

    size_t i = 0;
    uint32_t len = pSelf->Length;

  
    for(i=0;i<len;++i)
    {
        if(pSelf->Ptr[i].Cookie == removeCookie)
        {
            if(pSelf->IsInFireProgress)
            {
                pSelf->Ptr[i].IsRemoved = TRUE;
                return 0;
            }

            if(pSelf->Ptr[i].pUserData)
            {
                if(pSelf->Ptr[i].pUserData->fnReleaseUserData)
                {
                    pSelf->Ptr[i].pUserData->fnReleaseUserData(pSelf->Ptr[i].pUserData);
                }  
            }

            pSelf->Ptr[i] = pSelf->Ptr[len];
            pSelf->Ptr[len].pfnCallback = NULL;

            if(len > 0)
            {
                pSelf->Length--;
            }

            return 0;
        }
    } 

    return -1;

}

void CleanEventContainer(EventContainer* pSelf)
{
    int len = pSelf->Length;
    int i = 0;
    for(i=0;i<len;++i)
    {
        EventCallbackNode* pNode = pSelf->Ptr + i;
        if(pNode->pUserData)
        {
            if(pNode->pUserData->fnReleaseUserData)
            {
                pNode->pUserData->fnReleaseUserData(pNode->pUserData);
            }
        }
        pNode->pfnCallback = NULL;
    } 

    if(pSelf->Ptr != pSelf->Buffer)
    {
        free(pSelf->Ptr);
        pSelf->Ptr = pSelf->Buffer;
    }

    pSelf->Length = 0;
    pSelf->Size = 0;
}