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
#include "./objectIndex/UIObjectMap.h"

typedef struct tagRootUIObjTree
{
	NGOS_ROOT_OBJTREE_HANDLE hTree;
	NGOS_RootTreeEnv* RunEnv;
	NGOS_ROOT_OBJTREE_HANDLE RootUIObject;
	uint16_t DPI;

	DirtyRectIndex* DirtyRectManager;
	UIObjectRectIndex* UIObjectRectManager;
    UIObjectMap* NamedUIObjectMap;

    NGOS_UIOBJECT_HANDLE hTargetTouchObject;
}RootUIObjTree;



RootUIObjTree* CreateRootUIObjTree(NGOS_RootTreeEnv* pEnv);
int SetRootUIObjTreeViewPort(RootUIObjTree* pTree, uint16_t uWidth, uint16_t uHeight);

int FreeRootUIObjTree(RootUIObjTree* pTree);

int RootUIObjTreeMoveObject(RootUIObjTree* pObjTree,NGOS_ROOT_OBJTREE_HANDLE hObject,NGOS_ROOT_OBJTREE_HANDLE hNewParent);

int RootUIObjTreePushDirtyRect(RootUIObjTree* pObjTree,RECT* pDirtyRect);
int RootUIObjTreeGetRenderScrpit(RootUIObjTree* pObjTree,RECT* pClipRect,NGRE_SCRIPT_HANDLE hRenderScript);
//NGOS_RENDER_SCRIPT_BUFFER_HANDLE UIObjectGetRenderScrpit(UIObject* pObj,RECT* pClipRect);

void SendInputAcitonToUIObjTree(RootUIObjTree* pObjTree,uint32_t Action,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,void* eventData);
void UIObjTreeAddInpuatActionFilter();
NGOS_UIOBJECT_HANDLE UIObjTreeGetObjectByPath(RootUIObjTree* pObjTree,const char* path);

#endif 

