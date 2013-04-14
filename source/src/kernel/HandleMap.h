//
// HandleMap.h 
//
// create:liuzhicong,2013-3-13
//
// 先实现一个临时的，进程内的版本
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_HANDLE_MAP_H_
#define _NGOS_HANDLE_MAP_H_

void InitObjectHandleMap();
void DestoryObjectHandleMap();

void* HandleMapDecodeUIObject(NGOS_UIOBJECT_HANDLE hObject,TYPE_NGOS_PID* pOwner);
void* HandleMapDecodeRootTree(NGOS_ROOT_OBJTREE_HANDLE hTree,TYPE_NGOS_PID* pOwner);

NGOS_UIOBJECT_HANDLE HandleMapEncodeUIObject(void* pUIObject);
NGOS_ROOT_OBJTREE_HANDLE HandleMapEncodeRootTree(void* pTree);
#endif //_NGOS_HANDLE_MAP_H_