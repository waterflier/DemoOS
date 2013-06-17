//
// UIObjTree.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObjTree.h"
#include "./RootObjTreeEnv.h"
#include "./UIObject.h"
#include <RenderScript.h>
#include "../uiobjects/UIObjectTypeLoader.h"
#include "./input/InputAction.h"

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
		///放到视口大小改变的时候再创建
		pResult->DirtyRectManager = NULL;
		pResult->UIObjectRectManager = CreateUIObjectRectIndex(); 
		pResult->NamedUIObjectMap = CreateUIObjectMap();
        //创建默认的RootUIObject
		pResult->RootUIObject = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject",NULL);
		UIObject* pRootObj = HandleMapDecodeUIObject(pResult->RootUIObject);
		if(pRootObj)
		{
			pRootObj->hOwnerTree = pResult->hTree;
			pRootObj->ObjAbsRect = pRootObj->ObjRect;
		}
	}
	else
	{
		free(pRealEnv);
	}

	return pResult;

}

int SetRootUIObjTreeViewPort(RootUIObjTree* pTree, uint16_t uWidth, uint16_t uHeight)
{
	if(pTree)
	{
		///先不管了 只能create之后马上调用
		if(pTree->DirtyRectManager == NULL)
		{
			pTree->DirtyRectManager = CreateDirtyRectIndex(uWidth,uHeight);
		}
		return NGOS_RESULT_SUCCESS;
	}
	return NGOS_RESULT_INVALID_PTR;
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
 
int RootUIObjTreeGetRenderScrpit(RootUIObjTree* pObjTree,RECT* pClipRect, NGRE_SCRIPT_HANDLE hRenderScript)
{
	UIObjectVector theVector;
	InitUIObjectVector(&theVector,16);
	SelectObjectFromUIObjectRectIndex(pObjTree->UIObjectRectManager,pClipRect,&theVector);
	
	int vectorCount = UIObjectVectorGetCount(&theVector);
	int i=0;
	for(i=0;i<vectorCount;++i)
	{
		NGOS_UIOBJECT_HANDLE hObj = UIObjectVectorGet(&theVector,i);
		UIObject* pObj = HandleMapDecodeUIObject(hObj,NULL);
		if(pObj && pObj->Imp && pObj->Imp->fnGetRenderScript)
		{
			//是否可以根据一些条件再判断obj是否需要生成render script
			NGRE_SCRIPT_HANDLE hObjRS = pObj->Imp->fnGetRenderScript(pObj,pClipRect);
			if(hObjRS)
			{
				NGREAppendScript(hRenderScript,NGREGetScriptCode(hObjRS));
				NGRECloseScript(hObjRS);
			}

		}
	}

	return NGOS_RESULT_SUCCESS;
}



void SendInputAcitonToUIObjTree(RootUIObjTree* pObjTree,uint32_t Action,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,void* eventData)
{
    //TODO:需要大幅强化默认行为
    uint32_t DeviceType = Action >> 16;
   	Action = Action & 0x0000ffff;
    if(DeviceType == NGOS_INPUTDEVICE_MOUSE)
    {
        uint32_t PointParam = (uint32_t) param1;
        int16_t X = (PointParam & 0xffff0000)>>16;
        int16_t Y = (PointParam & 0xffff);
        
        UIObjectVector* pResult = CreateUIObjectVector(16);
        HitTestObjectFromUIObjectRectIndex(pObjTree->UIObjectRectManager,X,Y,pResult);
        int count = UIObjectVectorGetCount(pResult);
        int i = 0;
        for(i=0;i<count;++i)
        {
            NGOS_UIOBJECT_HANDLE hObj = UIObjectVectorGet(pResult,i);
            UIObject* pObj = HandleMapDecodeUIObject(hObj,NULL);
            if(pObj)
            {
                if(pObj->pInputTarget)
                {
                    //find first response
                    InputTargetProcessAction(pObj->pInputTarget,pObj->hSelf,Action,X,Y,eventData);
                    return;
                }
            }
        }
        DestroyUIObjectVector(pResult);
    }
    else if(DeviceType == NGOS_INPUTDEVICE_MAIN_TOUCH_SCREEN)
    {
    	//uint32_t PointParam = (uint32_t) param1;
        uint32_t X = (uint32_t) param1;
        uint32_t Y = (uint32_t) param2;
        MTEventData* pData = eventData;
        uint32_t theActionCode = Action & 0x0000ffff;
        if(theActionCode == NGOS_ACTION_TOUCH_DOWN)
        {
            UIObjectVector* pResult = CreateUIObjectVector(16);
            HitTestObjectFromUIObjectRectIndex(pObjTree->UIObjectRectManager,X,Y,pResult);
            int count = UIObjectVectorGetCount(pResult);
            int i = 0;
            for(i=0;i<count;++i)
            {
                NGOS_UIOBJECT_HANDLE hObj = UIObjectVectorGet(pResult,i);
                UIObject* pObj = HandleMapDecodeUIObject(hObj,NULL);
                if(pObj)
                {
                    if(pObj->pInputTarget)
                    {
                        //find first response
                        InputTargetProcessAction(pObj->pInputTarget,pObj->hSelf,Action, X,Y,eventData);
                        pObjTree->hTargetTouchObject = hObj;
                        break;
                    }
                }
            }
        }
        else
        {
            if(pObjTree->hTargetTouchObject)
            {
                UIObject* pObj = HandleMapDecodeUIObject(pObjTree->hTargetTouchObject,NULL);
                if(pObj && pObj->pInputTarget)
                {
                    //find first response
                    InputTargetProcessAction(pObj->pInputTarget,pObj->hSelf,Action,X,Y,eventData);
                   
                }
                if(theActionCode == NGOS_ACTION_TOUCH_UP)
                {
                    pObjTree->hTargetTouchObject = NULL;
                }
            }
        }
        
        if(pData)
        {
            ReleaseMTEventData(pData);
        }
        return;
    }
    
}

NGOS_UIOBJECT_HANDLE UIObjTreeGetObjectByPath(RootUIObjTree* pObjTree,const char* path)
{
    NGOS_UIOBJECT_HANDLE hResult = NULL;
    if(pObjTree->NamedUIObjectMap)
    {
        hResult = UIObjectMapFind(pObjTree->NamedUIObjectMap,path);
    }
    
    return hResult;
}