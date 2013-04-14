//
// DirtyRectIndex.h 
//
//////////////////////////////////////////////////////////////////////////


#ifndef _NGOS_DIRTY_RECT_INDEX_H_
#define _NGOS_DIRTY_RECT_INDEX_H_


typedef struct tagDirtyRectIndex
{

}DirtyRectIndex;

typedef struct tagRectList
{

}RectList;

DirtyRectIndex* CreateDirtyRectIndex(uint16_t width,uint16_t height);
int DestroyDirtyRectIndex(DirtyRectIndex* pIndex);
int DirtyRectIndexPushRect(DirtyRectIndex* pIndex,RECT* pRect);

int PopAndCleanDirtyRect(DirtyRectIndex* pIndex,RectList** ppResultRectList);

#endif //_NGOS_DIRTY_RECT_INDEX_H_