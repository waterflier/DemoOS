//
// UIObjectRectIndex.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./UIObjectRectIndex.h"
#include "../HandleMap.h"
#include "../UIObject.h"
#include <stdlib.h>

UIObjectRectIndex* CreateUIObjectRectIndex()
{
	UIObjectRectIndex* pResult = malloc(sizeof(UIObjectRectIndex));
	pResult->Length = 0;
	pResult->Size = 16;
	pResult->pList = malloc(sizeof(UIObjectRectIndexNode)*16);

	return pResult;
}

void DestroyUIObjectRectIndex(UIObjectRectIndex* pIndex)
{
	free(pIndex->pList);
	free(pIndex);
}	

static int compareUIObjectIndexNode(UIObjectRectIndexNode* left,UIObjectRectIndexNode* right)
{
	return left->ZOrder - right->ZOrder;
}

int SelectObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,RECT* theRect,UIObjectVector* pResult)
{
	int i=0;
	for(i=0;i<pIndex->Length;++i)
	{
		UIObjectRectIndexNode* pNode = pIndex->pList+i;
		if(IsRectIntersect(&(pNode->AbsRect),theRect) != 0)
		{
			UIObjectVectorAdd(pResult,pNode->hObject);
		}
	}

	//sort
	qsort(pIndex->pList,pIndex->Length,sizeof(UIObjectRectIndexNode),compareUIObjectIndexNode);
	return 0;
}

int HitTestObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,RECT* theRect,UIObjectVector* pResult)
{
	//TODO:
	return -1;
}

int AddObjectToUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject)
{
	UIObject* pObj = HandleMapDecodeUIObject(hObject,NULL);
	if(pObj == NULL)
	{
		return -1;
	}
	int i=0;
	for(i=0;i<pIndex->Length;++i)
	{
		UIObjectRectIndexNode* pNode = pIndex->pList+i;
		if(pNode->hObject == hObject)
		{
			pNode->AbsRect = pObj->ObjAbsRect;
			pNode->ZOrder = pObj->ObjAbsZorder;

			return 0;
		}
	}
	
	if(pIndex->Length == pIndex->Size)
	{
		UIObjectRectIndexNode* pTemp = pIndex->pList;
		pIndex->pList = malloc(sizeof(UIObjectRectIndexNode)*pIndex->Size*2);
		pIndex->Size = pIndex->Size*2;
		memcpy(pIndex->pList,pTemp,sizeof(UIObjectRectIndexNode)*pIndex->Length);
		free(pTemp);
	}

	pIndex->pList[pIndex->Length].AbsRect = pObj->ObjAbsRect;
	pIndex->pList[pIndex->Length].ZOrder = pObj->ObjAbsZorder;
	pIndex->pList[pIndex->Length].hObject = hObject;
	pIndex->Length++;
	return 0;
}

int RemoveObjectFromUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject)
{
	int i = 0;
	for(i=0;i<pIndex->Length;++i)
	{
		UIObjectRectIndexNode* pNode = pIndex->pList+i;
		if(pNode->hObject == hObject)
		{
			*pNode = pIndex->pList[pIndex->Length-1];
			pIndex->Length--;
			return 0;
		}

	}

	return -1;
}

int UpdateUIObjectToUIObjectIndex(UIObjectRectIndex* pIndex,NGOS_UIOBJECT_HANDLE hObject)
{
	int i = 0;
	for(i=0;i<pIndex->Length;++i)
	{
		UIObjectRectIndexNode* pNode = pIndex->pList+i;
		if(pNode->hObject == hObject)
		{
			UIObject* pObj = HandleMapDecodeUIObject(hObject,NULL);
			if(pObj)
			{
				pNode->AbsRect = pObj->ObjAbsRect;
				pNode->ZOrder = pObj->ObjAbsZorder;
			}

			return 0;
		}
	}
}
