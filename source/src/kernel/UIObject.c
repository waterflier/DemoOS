//
// UIObject.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObject.h"
#include "./RootObjTree.h"
#include "./UIObjTree.h"

static void UpdateObjectAbsPosInfo(UIObject* pObject,UIObject* pParent)
{
	//update self
	if(pParent == NULL)
	{
		pObject->ObjAbsRect = pObject->ObjRect;
		pObject->ObjAbsZorder = pObject->ObjZorder;
	}
	else
	{
		pObject->ObjAbsRect.left = pParent->ObjAbsRect.left + pObject->ObjRect.left;
		pObject->ObjAbsRect.right = pObject->ObjRect.right - pObject->ObjRect.left + pObject->ObjAbsRect.left; 
		pObject->ObjAbsRect.top = pParent->ObjAbsRect.top + pObject->ObjRect.top;
		pObject->ObjAbsRect.bottom = pObject->ObjRect.bottom - pObject->ObjRect.top + pObject->ObjAbsRect.top; 
	
		pObject->ObjAbsZorder = pParent->ObjAbsZorder + 100 + pObject->ObjZorder;

		pObject->hOwnerTree = pParent->hOwnerTree;
	}

	//update children
	
	if(pObject->pChildren == NULL)
	{
		return;
	}

	int i;
	int count = UIObjectVectorGetCount(pObject->pChildren);
	for(i=0;i<count;++i)
	{
		UIObject* pChild = HandleMapDecodeUIObject(UIObjectVectorGet(pObject->pChildren,i),NULL);
		if(pChild)
		{
			UpdateObjectAbsPosInfo(pChild,pObject);
		}
	}
    

}
static void UpdateBindObjectToIndex(UIObject* pObject,RootUIObjTree* pTree)
{
	AddObjectToUIObjectRectIndex(pTree->UIObjectRectManager,pObject->hSelf);
    if(pObject->strID)
    {
        if(UIObjectMapInsert(pTree->NamedUIObjectMap,pObject->hSelf,pObject->strID) !=0 )
        {
            //TODO:
        }
    }

    if(pObject->Imp->fnOnBind)
    {
        pObject->Imp->fnOnBind(pObject);
    }

	if(pObject->pChildren == NULL)
	{
		return;
	}

	int i;
	int count = UIObjectVectorGetCount(pObject->pChildren);
	for(i=0;i<count;++i)
	{
		UIObject* pChild = HandleMapDecodeUIObject(UIObjectVectorGet(pObject->pChildren,i),NULL);
		if(pChild)
		{
			UpdateBindObjectToIndex(pChild,pTree);
		}
	}

    if(pObject->Imp->fnOnInitChild)
    {
        pObject->Imp->fnOnInitChild(pObject);
    }
}

static int UIObjectOnBind(void* pSelf)
{
    UIObject* pObj = (UIObject*) pSelf;
    NGOS_UIOBJECT_HANDLE hObj = pObj->hSelf;
    EventContainer* pEA = UIObjectGetEventContainer(pObj,EVENT_NAME_ONBIND,FALSE);
    if(pEA)
    {
        FIRE_EVENT(pEA,CALLBACK_OnBind,hObj);
    }

}

static int UIObjectOnInitChild(void* pSelf)
{
    UIObject* pObj = (UIObject*) pSelf;
    NGOS_UIOBJECT_HANDLE hObj = pObj->hSelf;
    EventContainer* pEA = UIObjectGetEventContainer(pObj,EVENT_NAME_ONBIND,FALSE);
    if(pEA)
    {
        FIRE_EVENT(pEA,CALLBACK_OnInitChild,hObj);
    }
}

static int UIObjectOnPosChanged(void* pSelf)
{
    UIObject* pObj = (UIObject*) pSelf;
    NGOS_UIOBJECT_HANDLE hObj = pObj->hSelf;
    EventContainer* pEA = UIObjectGetEventContainer(pObj,EVENT_NAME_ONBIND,FALSE);
    if(pEA)
    {
        //TODO:
    }
}


static UIObjectProvier* GetUIObjectDefaultProvier()
{
    static UIObjectProvier theResult = {
        .fnGetRenderScript = NULL,
        .fnOnInitChild = UIObjectOnInitChild,
        .fnOnBind = UIObjectOnBind,
        .fnOnPosChanged = UIObjectOnPosChanged
    };

    return &theResult;
}

UIObject* MallocUIObject(NGOS_RootTreeEnv* pEnv,size_t userDataLen)
{
	//if(pEnv)
	{
		UIObject* pResult;
		pResult = malloc(sizeof(UIObject)+userDataLen);
		memset(pResult,0,sizeof(UIObject)+userDataLen);
		//pResult = (UIObject*)pEnv->fnAlloc(NGOS_ENTITY_TYPE_UIOBJ,pEnv->AllocUD,NULL,sizeof(UIObject)+userDataLen,0);
        pResult->Imp = GetUIObjectDefaultProvier;
		if(pResult)
		{
			//pResult->Header.Env = pEnv;
			return pResult;
		}
	}

	return NULL;
}

int UIObjectInit(UIObject* pObj)
{
	if(pObj->hSelf)
	{
		return;
	}

	pObj->pChildren = NULL;
	pObj->pLogicControlChildren = NULL;

	pObj->hSelf = HandleMapEncodeUIObject(pObj);
}

int UIObjectUninit(UIObject* pObj)
{
	//释放孩子

}

int FreeUIObject(UIObject* pObj)
{
	if(pObj)
	{

	}
}

void* UIObjectGetUserDataStart(UIObject* pObj)
{
	unsigned char* pData = (unsigned char*) pObj;
	pData += sizeof(UIObject);
	return (void*) pData;
}

int UIObjectAddChild(UIObject* pObject,NGOS_UIOBJECT_HANDLE hChild,BOOL isLogicChild)
{
	UIObject* pChild = (UIObject*) HandleMapDecodeUIObject(hChild,NULL);
	if(pChild)
	{
		if(pChild->hOwnerTree || pChild->hParent)
		{
			return NGOS_RESULT_OBJECT_BINDED;
		}

		if(isLogicChild)
		{
			if(pObject->pLogicControlChildren == NULL)
			{
				pObject->pLogicControlChildren = CreateUIObjectVector(0);
			}
			UIObjectVectorAdd(pObject->pLogicControlChildren,hChild);
		}
		if(pObject->pChildren == NULL)
		{
			pObject->pChildren = CreateUIObjectVector(0);
		}
		UIObjectVectorAdd(pObject->pChildren,hChild);


		if(pObject->hOwnerTree)
		{
			RootUIObjTree* objTree = HandleMapDecodeRootTree(pObject->hOwnerTree,NULL);
			if(!objTree)
			{
				return NGOS_RESULT_INVALID_PTR;
			}
			
			//todo: 从父到子计算位置表达式，并更新abspos
			//注意这个过程是无事件的
			UpdateObjectAbsPosInfo(pChild,pObject);

			//从父到子将刚刚绑定的对象加入到对象树的index里,这里也是不触发事件的
			UpdateBindObjectToIndex(pChild,objTree);

			
			//重画
			RECT absViewRect;
			UIObjectGetVisibleRect(pChild,&absViewRect);
			RootUIObjTreePushDirtyRect(objTree,&absViewRect);
		}

		return NGOS_RESULT_SUCCESS;
	}

	return NGOS_RESULT_INVALID_PTR;
}

void InvalidUIObject(UIObject* pObject)
{
	if(pObject->hOwnerTree == NULL)
	{
		return;
	}

	RootUIObjTree* objTree = HandleMapDecodeRootTree(pObject->hOwnerTree,NULL);
	if(objTree)
	{
		RECT absViewRect;
		UIObjectGetVisibleRect(pObject,&absViewRect);
		RootUIObjTreePushDirtyRect(objTree,&absViewRect);
	}

	return;
	
}
int UIObjectGetVisibleRect(UIObject* pObject,RECT* absRect)
{
	*absRect = pObject->ObjAbsRect;
	return 0;
}

int UIObjectRemoveChild(UIObject* pObject,NGOS_UIOBJECT_HANDLE hChild)
{
	UIObject* pChild = (UIObject*) HandleMapDecodeUIObject(hChild,NULL);
	if(pChild)
	{
		if(pChild->hParent != pObject->hSelf)
		{
			return NGOS_RESULT_NOT_MY_CHILD;
		}

		if(pChild->hOwnerTree)
		{
			//fire some event?
			//UIObjectVectorRemove(pObject->pChildren,hChild);
		}	
		
		UIObjectVectorRemove(pObject->pChildren,hChild);
		//为了支持OnDestroy事件，合适删除对象比较合适?

        //TODO: remove from some index
	}
}

int UIObjectMove(UIObject* pObject,RECT* pNewPos)
{
	if(pObject == NULL || pNewPos == NULL)
	{
		return NGOS_RESULT_INVALID_PTR;
	}
	pObject->ObjRect = *pNewPos;
	if(pObject->hOwnerTree)
	{	
		UIObject* pParent = HandleMapDecodeUIObject(pObject->hParent,NULL);
		UpdateObjectAbsPosInfo(pObject,pParent);
	}
	//fire event OnPosChange
	
}

int UIObjectMoveReal(UIObject* pObject,RealRECT* pNewPos)
{
	if(pObject == NULL || pNewPos == NULL)
	{
		return NGOS_RESULT_INVALID_PTR;
	}

	pObject->ObjRealRect = *pNewPos;
	//转换坐标
	//if(pObject->)
}

int UIObjectSetVisibleFlags(UIObject* pObject,uint32_t visibleFlags)
{
	pObject->VisibleFlags = visibleFlags;
	if(pObject->hOwnerTree)
	{
		//fire OnVisibleChange
	}

}

int UIObjectSetTrans(UIObject* pObject,Matrix3X2* pTransMartix)
{
	return -1;
}

EventContainer* UIObjectGetEventContainer(UIObject* pObject,int EventName,BOOL isAutoCreate)
{
    if(pObject->pAllEventContainer == NULL)
    {
        if(isAutoCreate)
        {
            pObject->pAllEventContainer = malloc(sizeof(EventContainer*) * EVENT_MAX);
        }
        else
        {
            return NULL;
        }
    }
    
    int index = EventName-EVENT_BEGIN;
    EventContainer* result = NULL;
    if(pObject->pAllEventContainer[index] == NULL)
    {
        if(isAutoCreate)
        {
            result = pObject->pAllEventContainer[index] = CreateEventContainer(EventName & 0xffff);//TODO: Need free
        }

    }

    return result;
}