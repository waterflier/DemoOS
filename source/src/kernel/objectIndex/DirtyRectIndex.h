//
// DirtyRectIndex.h 
//
//////////////////////////////////////////////////////////////////////////


#ifndef _NGOS_DIRTY_RECT_INDEX_H_
#define _NGOS_DIRTY_RECT_INDEX_H_

#include "./RectList.h"

typedef struct tagDirtyRectIndex
{
	uint16_t Width;
	uint16_t Height;
	RectList* pRectList;

}DirtyRectIndex;

DirtyRectIndex* CreateDirtyRectIndex(uint16_t width,uint16_t height);
int DestroyDirtyRectIndex(DirtyRectIndex* pIndex);
int DirtyRectIndexPushRect(DirtyRectIndex* pIndex,RECT* pRect);

int GetDirtyRectFromIndex(DirtyRectIndex* pIndex,RectList** ppResultRectList);
int DirtyRectIndexClean(DirtyRectIndex* pIndex);

#endif //_NGOS_DIRTY_RECT_INDEX_H_