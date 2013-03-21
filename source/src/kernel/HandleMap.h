//
// HandleMap.h 
//
// create:liuzhicong,2013-3-13
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_HANDLE_MAP_H_
#define _NGOS_HANDLE_MAP_H_

void* HandleMapDecodeUIObject(NGOS_UIOBJECT_HANDLE hObject,TYPE_NGOS_PID* pOwner);
void* HandleMapDecodeRootTree(NGOS_ROOT_OBJTREE_HANDLE hTree,TYPE_NGOS_PID* pOwner);

#endif //_NGOS_HANDLE_MAP_H_