//
// UIObject.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObject.h"
#include "./RootObjTree.h"
#include "./UIObjTree.h"

static void UpdateObjectPosInfo(UIObject* pObject)
{

}
static void UpdateBindObjectToIndex(UIObject* pObject,RootUIObjTree* pTree)
{

}
static void FireCreateEvent(UIObject* pObject)
{

}



UIObject* MallocUIObject(NGOS_RootTreeEnv* pEnv)
{
	if(pEnv)
	{
		UIObject* pResult;
		pResult = (UIObject*)pEnv->fnAlloc(NGOS_ENTITY_TYPE_UIOBJ,pEnv->AllocUD,NULL,sizeof(UIObject),0);
		if(pResult)
		{
			//pResult->Header.Env = pEnv;
		}
	}

	return NULL;
}

int UIObjectInit(UIObject* pObj)
{
	pObj->pChildren = NULL;
	pObj->pLogicControlChildren = NULL;
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

int UIObjectAddChild(UIObject* pObject,NGOS_UIOBJECT_HANDLE hChild,BOOL isLogicChild)
{
	UIObject* pChild = (UIObject*) HandleMapDecodeUIObject(hChild,NULL);
	if(pChild)
	{
		if(pChild->hOwnerTree || pChild->hParent)
		{
			return NGOS_RESULT_OBJECT_BINDED;
		}

	
		if(pObject->hOwnerTree)
		{
			
			RootUIObjTree* objTree = HandleMapDecodeRootTree(pObject->hOwnerTree,NULL);
			if(!objTree)
			{
				return NGOS_RESULT_INVALID_PTR;
			}

			if(pObject->pChildren == NULL)
			{
				pObject->pChildren = CreateUIObjectVector(0);
			}
			UIObjectVectorAdd(pObject->pChildren,hChild);

			//todo: 从父到子计算位置表达式，并更新abspos
			//注意这个过程是无事件的
			UpdateObjectPosInfo(pChild);
		
			//从父到子将刚刚绑定的对象加入到对象树的index里,这里也是不触发事件的
			UpdateBindObjectToIndex(pChild,objTree);
			
			FireCreateEvent(pChild);

			//重画
			RECT absViewRect;
			UIObjectGetVisibleRect(pChild,&absViewRect);
			RootUIObjTreePushDirtyRect(objTree,&absViewRect);
			
			return NGOS_RESULT_SUCCESS;
		}
		else
		{
			UIObjectVectorAdd(pObject->pChildren,hChild);
			return NGOS_RESULT_SUCCESS;
		}
	}

	return NGOS_RESULT_INVALID_PTR;
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
		UpdateObjectPosInfo(pObject);
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
