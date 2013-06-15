//
// InputTarget.c 
// 
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"

#include "./InputTarget.h"
#include "../UIObject.h"
#include "../event/EventContainer.h"

void InputTargetProcessAction(InputTarget* pSelf,NGOS_UIOBJECT_HANDLE hObj,uint32_t Action,uint32_t param1,uint32_t param2,void* eventData)
{
    printf("process input action %d \n",Action);
    uint32_t x;
    uint32_t y;
    MTEventData* pData;

    EventContainer* pEA = NULL;
    UIObject* pObj = HandleMapDecodeUIObject(hObj);
    if(pObj == NULL)
    {
        printf("hobj invalid\n");
        return;
    }

    switch(Action)
    {
    case NGOS_ACTION_TOUCH_DOWN:
    case NGOS_ACTION_TOUCH_MOVE:
    case NGOS_ACTION_TOUCH_UP:
    case NGOS_ACTION_TOUCH_ONE_DOWN:
    case NGOS_ACTION_TOUCH_ONE_UP:
        x = param1;
        y = param2;
        pData = eventData;
        pEA = UIObjectGetEventContainer(pObj,Action,FALSE);
        if(pEA)
        {
            FIRE_EVENT(pEA,CALLBACK_OnTouch,hObj,x,y,pData);
        }

        break;
    }
    

    return;
}

InputTarget* CreateInputTarget()
{
    return malloc(sizeof(InputTarget));
}

int ReleaseInputTarget(InputTarget* pSelf)
{
    if(pSelf)
    {
        free(pSelf);
    }
}