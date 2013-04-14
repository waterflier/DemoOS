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
#include "./objectIndex/DirtyRectIndex.h"
#include "./objectIndex/UIObjectRectIndex.h"

typedef struct tagRootUIObjTree
{
	NGOS_ROOT_OBJTREE_HANDLE hTree;
	NGOS_RootTreeEnv* RunEnv;
	NGOS_ROOT_OBJTREE_HANDLE RootUIObject;
	uint16_t DPI;

	DirtyRectIndex* DirtyRectManager;
	UIObjectRectIndex* UIObjectRectManager;
}RootUIObjTree;


RootUIObjTree* CreateRootUIObjTree(NGOS_RootTreeEnv* pEnv);
int FreeRootUIObjTree(RootUIObjTree* pTree);

int RootUIObjTreeMoveObject(RootUIObjTree* pObjTree,NGOS_ROOT_OBJTREE_HANDLE hObject,NGOS_ROOT_OBJTREE_HANDLE hNewParent);


int RootUIObjTreePushDirtyRect(RootUIObjTree* pObjTree,RECT* pDirtyRect);
NGOS_RENDER_SCRIPT_BUFFER_HANDLE RootUIObjTreeGetRenderScrpit(RootUIObjTree* pObjTree,RECT* pClipRect);

#endif 

