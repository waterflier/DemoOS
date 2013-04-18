//
// RectList.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_RECT_LIST_H_
#define _NGOS_RECT_LIST_H_

inline char PointInRect(long x,long y,const RECT* pRect);
//return 0 ���ཻ,1,�ཻ,2����,3������
long IsRectIntersect(const RECT* pRect1,const RECT* pRect2);
void UnionRect(RECT* pResult,RECT* pSrc1,RECT* pSrc2);


typedef struct tagRectList
{
	uint32_t Size;
	uint32_t Length;
	RECT* Ptr;
}RectList;

RectList* CreateRectList();
void ReleaseRectList(RectList* pList);
int AddRectToList(RectList* pList,RECT* pNewRect);
RECT* GetRectAtIndex(RectList* pList,size_t index);
size_t GetRectListCount(RectList* pList);
void CleanRectList(RectList* pList);


#endif //_NGOS_RECT_LIST_H_