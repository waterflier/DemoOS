//
// InputTarget.h 
//
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_INPUT_TARGET_H_
#define _NGOS_INPUT_TARGET_H_



/*
对象的命中测试有3步

1.矩形命中：与对象的Visible,Alpha,PosRect,Trans,Mesh,Mask有关

2.HitTest Bitmap? 命中灰度图。在上述测试通过后，可以再附加一个命中灰度图,白色的点为命中

3.User Define HitTest 在上述测试通过，如果存在一个用户自定义的命中测试函数，使用该函数

*/

typedef struct tagInputTarget
{

}InputTarget;

#endif //_NGOS_INPUT_TARGET_H_