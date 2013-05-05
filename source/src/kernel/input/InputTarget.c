//
// InputTarget.c 
// 
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./InputTarget.h"


void InputTargetProcessAction(InputTarget* pSelf,uint32_t Action,void* param1,void* param2,void* eventData)
{
    printf("process input action\n");
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