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

typedef struct tagUIObjectRectIndexNode
{
	NGOS_UIOBJECT_HANDLE hObject;
	RECT AbsRect;
	int32_t ZOrder;
}UIObjectRectIndexNode;

typedef struct tagUIObjectRectIndex
{
	UIObjectRectIndexNode* pList;
	size_t Size;
	size_t Length;
}UIObjectRectIndex;

UIObjectRectIndex* CreateUIObjectRectIndex();
void DestroyUIObjectRectIndex(UIObjectRectIndex* pIndex);

int AddObjectToUIObjectRectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);
int RemoveObjectFromUIObjectRectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);
int UpdateUIObjectToUIObjectRectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);

//Ϊ�˻���ѡ�����
int SelectObjectFromUIObjectRectIndex(UIObjectRectIndex* pIndex,RECT* theRect,UIObjectVector* pResult);
int HitTestObjectFromUIObjectRectIndex(UIObjectRectIndex* pIndex,int16_t x,int16_t y,UIObjectVector* pResult);

#endif //_NGOS_UIOBJECT_RECT_INDEX_H_