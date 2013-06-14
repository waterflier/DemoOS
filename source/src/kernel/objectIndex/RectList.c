//
// RectList.c
//
//////////////////////////////////////////////////////////////////////////
#include "../perheader.h"
#include "./RectList.h"

#define max(a,b) (( a > b) ? a : b)
#define min(a,b) (( a > b) ? b : a)  
//***矩形相交
//用于快速计算相交关系的表格,0 不相交,1,相交,2包含,3被包含
static char RectIntersectTable [9][9] = 
{
	{0,0,0,0,1,1,0,1,2},
	{0,0,0,1,1,1,1,1,1},
	{0,0,0,1,1,0,2,1,0},
	{0,1,1,0,1,1,0,1,1},
	{1,1,1,1,3,1,1,1,1},
	{1,1,0,1,1,0,1,1,0},
	{0,1,2,0,1,1,0,0,0},
	{0,1,1,1,1,1,0,0,0},
	{2,1,0,1,1,0,0,0,0}
};


inline char PointInRect(long x,long y,const RECT* pRect)
{
	if(x < pRect->left)
	{
		if(y < pRect->top)
		{
			return PT_LEFTUP_RECT;
		}
		else if(y >= pRect->bottom)
		{
			return PT_LEFTDOWN_RECT;
		}
		else
		{
			return PT_LEFTMID_RECT;
		}
	}
	else if(x >= pRect->right)
	{
		if(y < pRect->top)
		{
			return PT_RIGHTUP_RECT;
		}
		else if(y >= pRect->bottom)
		{
			return PT_RIGHTDOWN_RECT;
		}
		else
		{
			return PT_RIGHTMID_RECT;
		}
	}
	else
	{
		if(y < pRect->top)
		{
			return PT_MIDUP_RECT;
		}
		else if(y >= pRect->bottom)
		{
			return PT_MIDDOWN_RECT;
		}
		else
		{
			return PT_MIDMID_RECT;
		}
	}
}

long IsRectIntersect(const RECT* pRect1,const RECT* pRect2)
{
	char ptInRectLeftTop = PointInRect(pRect2->left,pRect2->top,pRect1);
	char ptInRectRightBottom = PointInRect(pRect2->right,pRect2->bottom,pRect1);

	return RectIntersectTable[ptInRectLeftTop][ptInRectRightBottom];
}

void UnionRect(RECT* pResult,RECT* pSrc1,RECT* pSrc2)
{
	pResult->left = min(pSrc1->left,pSrc2->left);
	pResult->top = min(pSrc1->top,pSrc2->top);
	pResult->right = max(pSrc1->right,pSrc2->right);
	pResult->bottom = max(pSrc1->bottom,pSrc2->bottom);
}



RectList* CreateRectList()
{
	RectList* pResult = malloc(sizeof(RectList));
	pResult->Length = 0;
	pResult->Size = 16;
	pResult->Ptr = malloc(sizeof(RECT) * 16);
}

void ReleaseRectList(RectList* pList)
{
	free(pList->Ptr);
	free(pList);
}

int AddRectToList(RectList* pList,RECT* pNewRect)
{
	if(pList->Length == pList->Size)
	{
		RECT* pTemp = pList->Ptr;
		pList->Ptr = malloc(sizeof(RECT) * pList->Size * 2);
		memcpy(pList->Ptr,pTemp,sizeof(RECT) * pList->Length);
	}

	pList->Ptr[pList->Length] = *pNewRect;
	pList->Length++;

	return 0;
}

RECT* GetRectAtIndex(RectList* pList,size_t index)
{
	return pList->Ptr + index;
}

size_t GetRectListCount(RectList* pList)
{
	return pList->Length;
}

void CleanRectList(RectList* pList)
{
	pList->Length = 0;
	return;
}