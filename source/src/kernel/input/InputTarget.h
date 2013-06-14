//
// InputTarget.h 
//
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_INPUT_TARGET_H_
#define _NGOS_INPUT_TARGET_H_

#include "./InputAction.h"

/*
��������в�����3�� 

1.�������У�������Visible,Alpha,PosRect,Trans,Mesh,Mask�й�

2.HitTest Bitmap? ���лҶ�ͼ������������ͨ���󣬿����ٸ���һ�����лҶ�ͼ,��ɫ�ĵ�Ϊ����

3.User Define HitTest ����������ͨ�����������һ���û��Զ�������в��Ժ�����ʹ�øú���


ObjTree�õ�һ��Action,��ʼ���Һ��ʵ�InputTarget,Fire TargetObject�ϵ��¼���
��һ�����ҵ���InputTarget��FirstResponseTarget
���Target������Action����ô��Action�ᰴ��һ���Ĺ���ȥ������һ��Target
���ݵ���һ��Target�ķ���������a.ͬ������ b.�첽���ݣ�Next InputTarget������һ��MessageFrame����Action

���������߼� 1.��Ч 2.Ĭ���߼����ڿ��� 3.�û����׶����Լ��Ĺ���


�κ�һ��Action�����Թ�ObjTree�� PreActionFilter��PostActionFilter

����������HitTest��Action��һ�㶼��Ҫһ������ǰ���㡱Target,����Ѱ��FirstResponseTarget

UIObjTree����һ��Action���Ƿ���Ҫ�����䴦��ķ���ֵ

*/


typedef struct tagInputTarget
{
    struct InputTarget* NextResponseTarget; 
}InputTarget;


void InputTargetProcessAction(InputTarget* pSelf,NGOS_UIOBJECT_HANDLE hObj,uint32_t Action,uint32_t param1,uint32_t param2,void* eventData);

InputTarget* CreateInputTarget();
int ReleaseInputTarget(InputTarget* pSelf);

#endif //_NGOS_INPUT_TARGET_H_