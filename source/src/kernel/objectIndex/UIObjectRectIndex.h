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
RectIndex主要负责选取"可见对象"，选取方法通过Rect或pointer

对象的下列属性变化必须更新索引器

Type:由于对象的类型不会动态改变，所以只需要控制对象上树的时候是否加入cache就好了
Pos
Zorder(?)
Visible(不可见对象完全删除)
Trans,Mesh 这两个仔细计算的话计算量会相当的大，可以考虑每次改变的时候计算一个包裹矩形，命中包裹矩形后再做一次select test.
           另一种方法是  

EffectList 某些效果会改变对象的 “绘制影响区域”


------------------------------
如何方便的支持多级Index?
任何一个LimitChild的Object,都应该有机会包含一个二级index,

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

//为了绘制选择对象
int SelectObjectFromUIObjectRectIndex(UIObjectRectIndex* pIndex,RECT* theRect,UIObjectVector* pResult);
int HitTestObjectFromUIObjectRectIndex(UIObjectRectIndex* pIndex,int16_t x,int16_t y,UIObjectVector* pResult);

#endif //_NGOS_UIOBJECT_RECT_INDEX_H_