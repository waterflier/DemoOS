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

// π”√∫Í¿¥FireEvent?
//FIRE_EVENT(pSelf,returnType,arg2,arg3)
//typdef returnType (*pfn##__LINE__) 

#endif //_NGOS_EVENT_CONTAINER_H_