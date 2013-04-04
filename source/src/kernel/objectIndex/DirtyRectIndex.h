//
// DirtyRectIndex.h 
//
//////////////////////////////////////////////////////////////////////////


#ifndef _NGOS_DIRTY_RECT_INDEX_H_
#define _NGOS_DIRTY_RECT_INDEX_H_


typedef struct tagDirtyRectIndex
{

}DirtyRectIndex;

DirtyRectIndex* CreateDirtyRectIndex(uint16_t width,uint16_t height);
int DestroyDirtyRectIndex(DirtyRectIndex* pIndex);
int DirtyRectIndexPushRect(DirtyRectIndex* pIndex,RECT* pRect);

#endif //_NGOS_DIRTY_RECT_INDEX_H_