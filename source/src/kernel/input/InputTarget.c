//
// InputTarget.c 
// 
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./InputTarget.h"


void InputTargetProcessAction(InputTarget* pSelf,uint32_t Action,uint32_t  value,void* param,void* eventData)
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