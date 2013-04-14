//
// UIObjectRectIndex.h 
//
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_UIOBJECT_RECT_INDEX_H_
#define _NGOS_UIOBJECT_RECT_INDEX_H_

#include "./UIObjectVector.h"

/*
RectIndex��Ҫ����ѡȡ"�ɼ�����"��ѡȡ����ͨ��Rect��pointer

������������Ա仯�������������

Type:���ڶ�������Ͳ��ᶯ̬�ı䣬����ֻ��Ҫ���ƶ���������ʱ���Ƿ����cache�ͺ���
Pos
Zorder(?)
Visible(���ɼ�������ȫɾ��)
Trans,Mesh ��������ϸ����Ļ����������൱�Ĵ󣬿��Կ���ÿ�θı��ʱ�����һ���������Σ����а������κ�����һ��select test.
           ��һ�ַ�����  

EffectList ĳЩЧ����ı����� ������Ӱ������


------------------------------
��η����֧�ֶ༶Index?
�κ�һ��LimitChild��Object,��Ӧ���л������һ������index,

*/

typedef struct tagUIObjectRectIndex
{

}UIObjectRectIndex;


int AddObjectToUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);
int RemoveObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);
int UpdateUIObjectToUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);

//Ϊ�˻���ѡ�����
int SelectObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,RECT* theRect,UIObjectVector* pResult);
int HitTestObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,RECT* theRect,UIObjectVector* pResult);

#endif //_NGOS_UIOBJECT_RECT_INDEX_H_