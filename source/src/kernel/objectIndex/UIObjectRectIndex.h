//
// UIObjectRectIndex.h 
//
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_UIOBJECT_RECT_INDEX_H_
#define _NGOS_UIOBJECT_RECT_INDEX_H_


typedef struct tagUIObjectRectIndex
{

}UIObjectRectIndex;


int AddObjectToUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);
int RemoveObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);
int UpdateUIObjectToUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject);

int SelectObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,RECT theRect,UIObjectVector* pResult);
int HitTestObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,RECT theRect,UIObjectVector* pResult);

#endif //_NGOS_UIOBJECT_RECT_INDEX_H_