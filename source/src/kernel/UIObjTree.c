//
// UIObjTree.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObjTree.h"
#include "./RootObjTreeEnv.h"
#include "./UIObject.h"
#include "../render_engine/RenderScript.h"
#include "../uiobjects/UIObjectTypeLoader.h"

static void UpdateObjPos(UIObject* pObj);
static void UpdateAddedObj(UIObject* pObj);

RootUIObjTree* CreateRootUIObjTree(NGOS_RootTreeEnv* pEnv)
{
	NGOS_RootTreeEnv* pRealEnv = (NGOS_RootTreeEnv*) malloc(sizeof(NGOS_RootTreeEnv));
	if(pEnv)
	{
		*pRealEnv = *pEnv;
	}
	else
	{
		SetDefaultRootTreeEnv(pRealEnv);
	}

	RootUIObjTree* pResult = (RootUIObjTree*)pRealEnv->fnAlloc(NGOS_ENTITY_TYPE_TREE,pRealEnv->AllocUD,NULL,sizeof(RootUIObjTree),0);
	if(pResult)
	{
		pResult->RunEnv = pRealEnv;
		pResult->hTree = HandleMapEncodeRootTree(pResult);
		pResult->DPI = 72;
		//pResult->DirtyRectManager =
		//pResult->UIObjectRectManager = 
		//创建默认的RootUIObject
		pResult->RootUIObject = NGOS_CreateUIObject(NGOS_GetDefaultUIObjectTypeLoader(),"LayoutObject",NULL);
	}
	else
	{
		free(pRealEnv);
	}

}

int FreeRootUIObjTree(RootUIObjTree* pTree)
{
	if(pTree)
	{
		NGOS_RootTreeEnv* pEnv = pTree->RunEnv;
		pEnv->fnAlloc(NGOS_ENTITY_TYPE_TREE,pEnv->AllocUD,pTree,0,sizeof(RootUIObjTree));
		free(pEnv);
		return NGOS_RESULT_SUCCESS;
	}

	return NGOS_RESULT_INVALID_PTR;
}


int RootUIObjTreeMoveObject(RootUIObjTree* pObjTree,NGOS_ROOT_OBJTREE_HANDLE hObject,NGOS_ROOT_OBJTREE_HANDLE hNewParent)
{
	UIObject* pObj = (UIObject*) HandleMapDecodeUIObject(hObject,NULL);
	UIObject* pNewParent = (UIObject*) HandleMapDecodeUIObject(hNewParent,NULL);

	if(pObj && pNewParent)
	{
		if(pObj->hOwnerTree == pObjTree->hTree && pNewParent->hOwnerTree == pObjTree->hTree)
		{
			//TODO demo暂不实现
		}
	}
}


int RootUIObjTreePushDirtyRect(RootUIObjTree* pObjTree,RECT* pDirtyRect)
{
	if(pObjTree->DirtyRectManager)
	{
		return DirtyRectIndexPushRect(pObjTree->DirtyRectManager,pDirtyRect);
	}

	return NGOS_RESULT_INVALID_PTR;
}

NGOS_RENDER_SCRIPT_BUFFER_HANDLE RootUIObjTreeGetRenderScrpit(RootUIObjTree* pObjTree,RECT* pClipRect)
{
	UIObjectVector theVector;
	SelectObjectFromUIObjectIndex(pObjTree->UIObjectRectManager,pClipRect,&theVector);
	
	NGOS_RENDER_SCRIPT_BUFFER_HANDLE hResult = CreateRenderScript();
	int vectorCount = UIObjectVectorGetCount(&theVector);
	int i=0;
	for(i=0;i<vectorCount;++i)
	{
		NGOS_UIOBJECT_HANDLE hObj = UIObjectVectorGet(&theVector,i);
		UIObject* pObj = HandleMapDecodeUIObject(hObj,NULL);
		if(pObj)
		{
			//是否可以根据一些条件再判断obj是否需要生成render script

			if(pObj->Imp)
			{
				NGOS_RENDER_SCRIPT_BUFFER_HANDLE hObjRS = pObj->Imp->fnGetRenderScript(pObj,pClipRect);
				if(hObjRS)
				{
					AppendRenderScript(hResult,hObjRS);
					ReleaseRenderScript(hObjRS);
				}
			}
		}
	}

	return hResult;
}

