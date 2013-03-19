//
//  LocalUIObjTreeImp.h 
//
//  Create:liuzhicong,2013.3.16
//  
//  进程内对象树的实现。用于支持UIObjServer的实现 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_LOCAL_UI_OBJTREE_IMP_H_
#define _NGOS_LOCAL_UI_OBJTREE_IMP_H_


#include "./RootObjTreeEnv.h"

typedef struct tagRootUIObjTree
{
	RootTreeEnv* RunEnv;
	NGOS_ROOT_OBJTREE_HANDLE RootUIObject;
	uint16_t DPI;

	DirtyRectIndex* DirtyRectManager;
	UIObjectRectIndex* UIObjectRectManager;
}RootUIObjTree;




RootUIObjTree* CreateRootUIObjTree(RootTreeEnv* pEnv);
int FreeRootUIObjTree(RootUIObjTree* pNode);

int RootUIObjTreeAddChild(RootUIObjTree* pObjTree,NGOS_ROOT_OBJTREE_HANDLE hParent,NGOS_ROOT_OBJTREE_HANDLE hChild);
int RootUIObjTreeMoveObject(RootUIObjTree* pObjTree,NGOS_ROOT_OBJTREE_HANDLE hObject,NGOS_ROOT_OBJTREE_HANDLE hNewParent);
int RootUIObjTreeRemoveObject(RootUIObjTree* pObjTree,NGOS_ROOT_OBJTREE_HANDLE hObject);

int RootUIObjTreeGetRenderScrpit(RootUIObjTree* pObjTree,RECT* pClipRect);

#endif 

