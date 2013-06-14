//
// DirtyRectIndex.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./DirtyRectIndex.h"


DirtyRectIndex* CreateDirtyRectIndex(uint16_t width,uint16_t height)
{
	DirtyRectIndex* pResult = malloc(sizeof(DirtyRectIndex));
	pResult->pRectList = CreateRectList();
	pResult->Height = height;
	pResult->Width = width;

	return pResult;
}

int DestroyDirtyRectIndex(DirtyRectIndex* pIndex)
{
	ReleaseRectList(pIndex->pRectList);
	free(pIndex);


	return 0;
}

int DirtyRectIndexPushRect(DirtyRectIndex* pIndex,RECT* pRect)
{
	if(pRect->right < 0 || pRect->left >= pIndex->Width)
	{
		return 0;
	}

	if(pRect->bottom < 0 || pRect->top >= pIndex->Height)
	{
		return 0;
	}
	RECT realRect = *pRect;
	if(realRect.right > pIndex->Width)
	{
		realRect.right = pIndex->Width;
	}
	if(realRect.bottom > pIndex->Height)
	{
		realRect.bottom = pIndex->Height;
	}

	int count = GetRectListCount(pIndex->pRectList);
	int i=0;
	int result;
	RECT* pRectInList = NULL;
	for(i=0;i<count;i++)
	{
		pRectInList = GetRectAtIndex(pIndex->pRectList,i);
		result = IsRectIntersect(pRectInList,&realRect);
		if(result == 3)
		{
			return 0;
		}
		else
		{
			UnionRect(pRectInList,pRectInList,&realRect);
		}
	}

	if(count == 0)
	{
		AddRectToList(pIndex->pRectList,&realRect);
	}

	return 0;
}

int GetDirtyRectFromIndex(DirtyRectIndex* pIndex,RectList** ppResultRectList)
{
	*ppResultRectList = pIndex->pRectList;
}

int DirtyRectIndexClean(DirtyRectIndex* pIndex)
{
	CleanRectList(pIndex->pRectList);
}