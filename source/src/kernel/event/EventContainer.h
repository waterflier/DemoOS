//
// EventContainer.h 
//
///////////////////////////////////////////////////////////////////////////


#ifndef _NGOS_EVENT_CONTAINER_H_
#define _NGOS_EVENT_CONTAINER_H_


#ifndef EVENT_CONTAINER_INNER_SIZE
#define EVENT_CONTAINER_INNER_SIZE (8)
#endif 

#define EVENT_CONTAINER_MAX_SIZE (65535)

#define EVENT_BEGIN (100)
#define EVENT_MAX (64)
#define EVENT_NAME_ONBIND (100)
#define EVENT_NAME_ONINITCHILD (101)
#define EVENT_NAME_ONPOSCHANGED (102)

typedef struct tagEventCallbackNode
{
    void* pfnCallback;
    UserDataContext* pUserData;
    uint32_t Cookie;
    BOOL IsRemoved;
}EventCallbackNode;


typedef struct tagEventContainer
{
    BOOL IsInFireProgress;
    uint16_t EventContainerFlag;
    uint16_t CurrentCookie;
    EventCallbackNode* Ptr;
    EventCallbackNode Buffer[EVENT_CONTAINER_INNER_SIZE];
    uint32_t Size;
    uint32_t Length;
   

}EventContainer;

EventContainer* CreateEventContainer(uint16_t flag);
void DestroyEventContainer(EventContainer* pSelf);

uint32_t AttachEvent(EventContainer* pSelf,void* pfnCallback,UserDataContext*  pUserdata);
int DetachEvent(EventContainer* pSelf,uint32_t cookie);
void CleanEventContainer(EventContainer* pSelf);

//Ê¹ÓÃºêÀ´FireEvent?
#define FIRE_EVENT(pSelf,CallbackType,...) int i=0; \
    pSelf->IsInFireProgress = TRUE; \
    for(i=0;i<pSelf->Length;++i) \
    { \
        EventCallbackNode* pNode = pSelf->Ptr+i; \
        if(pNode) \
        { \
            if(pNode->pfnCallback) \ 
            { \
                CallbackType fnCallback = (CallbackType) (pNode->pfnCallback); \
                fnCallback(pNode->pUserData,__VA_ARGS__); \
            } \
        } \
    } \
    pSelf->IsInFireProgress = FALSE;

//TODO: Remove wil remove node


typedef int (*CALLBACK_OnBind) (UserDataContext* pUserData,NGOS_UIOBJECT_HANDLE hObj);
typedef int (*CALLBACK_OnInitChild) (UserDataContext* pUserData,NGOS_UIOBJECT_HANDLE hObj);
//typedef int (*pfnOnPosChanged) (NGOS_UIOBJECT_HANDLE hObj,UserDataContext* pUserData);
#endif //_NGOS_EVENT_CONTAINER_H_