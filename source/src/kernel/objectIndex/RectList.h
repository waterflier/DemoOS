//
// RectList.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_RECT_LIST_H_
#define _NGOS_RECT_LIST_H_

//定义9种点与矩形的关系
#define PT_LEFTUP_RECT    0
#define PT_MIDUP_RECT     1
#define PT_RIGHTUP_RECT   2
#define PT_LEFTMID_RECT   3
#define PT_MIDMID_RECT    4
#define PT_RIGHTMID_RECT  5
#define PT_LEFTDOWN_RECT  6
#define PT_MIDDOWN_RECT   7
#define PT_RIGHTDOWN_RECT 8


inline char PointInRect(long x,long y,const RECT* pRect);
//return 0 不相交,1,相交,2包含,3被包含
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