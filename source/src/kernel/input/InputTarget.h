//
// InputTarget.h 
//
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_INPUT_TARGET_H_
#define _NGOS_INPUT_TARGET_H_

#include "./InputAction.h"

/*
对象的命中测试有3步 

1.矩形命中：与对象的Visible,Alpha,PosRect,Trans,Mesh,Mask有关

2.HitTest Bitmap? 命中灰度图。在上述测试通过后，可以再附加一个命中灰度图,白色的点为命中

3.User Define HitTest 在上述测试通过，如果存在一个用户自定义的命中测试函数，使用该函数


ObjTree拿到一个Action,开始查找合适的InputTarget,Fire TargetObject上的事件。
第一个被找到的InputTarget叫FirstResponseTarget
如果Target不捕获Action，那么该Action会按照一定的规律去查找下一个Target
传递到下一个Target的方法有两种a.同步传递 b.异步传递，Next InputTarget会在下一个MessageFrame处理Action

如何让这个逻辑 1.高效 2.默认逻辑易于控制 3.用户容易定义自己的规则


任何一个Action都可以过ObjTree的 PreActionFilter和PostActionFilter

不基于输入HitTest的Action，一般都需要一个“当前焦点”Target,用来寻找FirstResponseTarget

UIObjTree对于一个Action，是否需要关心其处理的返回值

*/


typedef struct tagInputTarget
{
    struct InputTarget* NextResponseTarget; 
}InputTarget;


void InputTargetProcessAction(InputTarget* pSelf,NGOS_UIOBJECT_HANDLE hObj,uint32_t Action,uint32_t param1,uint32_t param2,void* eventData);

InputTarget* CreateInputTarget();
int ReleaseInputTarget(InputTarget* pSelf);

#endif //_NGOS_INPUT_TARGET_H_