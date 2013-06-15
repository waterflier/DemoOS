//
// UIObject.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObject.h"
#include "./RootObjTree.h"
#include "./UIObjTree.h"
#include <math.h>

static void UpdateObjectAbsPosInfo(UIObject* pObject,UIObject* pParent)
{
	RECT OldAbsRect = pObject->ObjAbsRect;
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
	UIObjectUpdataVisibleRect(pObject, UIObjectUpdateVisibleRectMove, &OldAbsRect);
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

	if(pObject->pChildren != NULL)
	{
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
        pResult->Imp = GetUIObjectDefaultProvier();
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
		pChild->hParent = pObject->hSelf;
		

		if(pObject->hOwnerTree)
		{
			RootUIObjTree* objTree = HandleMapDecodeRootTree(pObject->hOwnerTree,NULL);
			if(!objTree)
			{
				return NGOS_RESULT_INVALID_PTR;
			}
			//从父到子将刚刚绑定的对象加入到对象树的index里,这里也是不触发事件的
			UpdateBindObjectToIndex(pChild,objTree);

			//todo: 从父到子计算位置表达式，并更新abspos
			//注意这个过程是无事件的
			UpdateObjectAbsPosInfo(pChild,pObject);
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
	*absRect = pObject->ObjVisibleRect;
	return 0;
}

int UIObjectSetRotate(UIObject* pObject, float* fRotateInfo)
{
	if(fRotateInfo == NULL)
	{
		if(pObject->pTransInfo != NULL)
		{
			DestroyUIObjectTransInfo(pObject->pTransInfo);
			pObject->pTransInfo = NULL;
		}
	}
	else
	{
		if(pObject->pTransInfo == NULL)
		{
			pObject->pTransInfo = CreateUIObjectTransInfo();
		}
		SetUIObjectTransInfoRotate(pObject->pTransInfo, fRotateInfo[0],fRotateInfo[1],fRotateInfo[2]);
	}
	UIObjectUpdataVisibleRect(pObject, UIObjectUpdateVisibleRectEffect, NULL);
	
	return 0;
}

int UIObjectUpdataVisibleRect(UIObject* pObject , uint8_t bFlag, const RECT* pOldAbsRect)
{
	///变换之类的坐标系是对象本身的坐标系
	if(bFlag & UIObjectUpdateVisibleRectMove)
	{
		if(((pOldAbsRect->right - pOldAbsRect->left) != (pObject->ObjAbsRect.right -  pObject->ObjAbsRect.left)) ||
				((pOldAbsRect->bottom - pOldAbsRect->top) != (pObject->ObjAbsRect.bottom -  pObject->ObjAbsRect.top)))
		{
			bFlag |= UIObjectUpdateVisibleRectResize;
		}
	}
	if((bFlag & UIObjectUpdateVisibleRectEffect || bFlag & UIObjectUpdateVisibleRectResize))
	{
		if(pObject->pTransInfo)
		{
			RealRECT visibleRect = {0, 0,(pObject->ObjRect.right - pObject->ObjRect.left), 
				(pObject->ObjRect.bottom - pObject->ObjRect.top)};

			uint8_t bInvalid = 0;
			bInvalid |= ((UIObjectEffectHeader*)(pObject->pTransInfo))->fnUpdateVisibleRect(pObject->pTransInfo, &visibleRect);
			//直接两边扩展一个像素的误差好了
			///有可能前后一样但是要强制刷新， 丢给dirty rect mgr去合并， 这里不管了
			if(bInvalid)
			{
				InvalidUIObject(pObject);
			}
			pObject->ObjVisibleRect.left = visibleRect.left + pObject->ObjAbsRect.left;
			pObject->ObjVisibleRect.top = visibleRect.top + pObject->ObjAbsRect.top;
			pObject->ObjVisibleRect.right = visibleRect.right + pObject->ObjAbsRect.left + 1;
			pObject->ObjVisibleRect.bottom = visibleRect.bottom + pObject->ObjAbsRect.top + 1;
			if(bInvalid)
			{
				InvalidUIObject(pObject);
			}
		}
		else
		{	
			InvalidUIObject(pObject);
			pObject->ObjVisibleRect.left = pObject->ObjAbsRect.left;
			pObject->ObjVisibleRect.top = pObject->ObjAbsRect.top;
			pObject->ObjVisibleRect.right = pObject->ObjAbsRect.right;
			pObject->ObjVisibleRect.bottom = pObject->ObjAbsRect.bottom;
			InvalidUIObject(pObject);
		}
	}
	else
	{
		if(bFlag & UIObjectUpdateVisibleRectMove)
		{
			InvalidUIObject(pObject);
		}
		pObject->ObjVisibleRect.left = pObject->ObjVisibleRect.left + (pObject->ObjAbsRect.left - pOldAbsRect->left);
		pObject->ObjVisibleRect.top = pObject->ObjVisibleRect.top + (pObject->ObjAbsRect.top - pOldAbsRect->top);
		pObject->ObjVisibleRect.right = pObject->ObjVisibleRect.right + (pObject->ObjAbsRect.left - pOldAbsRect->left);
		pObject->ObjVisibleRect.bottom = pObject->ObjVisibleRect.bottom + (pObject->ObjAbsRect.top - pOldAbsRect->top);
		InvalidUIObject(pObject);
	}
	RootUIObjTree* pTree = NULL;
	if(pObject->hOwnerTree && (pTree = HandleMapDecodeRootTree(pObject->hOwnerTree)))
	{
		UpdateUIObjectToUIObjectRectIndex(pTree->UIObjectRectManager,pObject->hSelf);
	}
	
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
	if(0 == memcmp(&(pObject->ObjRect), pNewPos, sizeof(RECT)))
	{
		return NGOS_RESULT_SUCCESS;
	}
	pObject->ObjRect = *pNewPos;
	if(pObject->hOwnerTree)
	{	
		UIObject* pParent = HandleMapDecodeUIObject(pObject->hParent,NULL);
		UpdateObjectAbsPosInfo(pObject,pParent);
	}
	//fire event OnPosChange
	
}

//int UIObjectMoveReal(UIObject* pObject,RealRECT* pNewPos)
//{
//	if(pObject == NULL || pNewPos == NULL)
//	{
//		return NGOS_RESULT_INVALID_PTR;
//	}
//
//	pObject->ObjRealRect = *pNewPos;
//	//转换坐标
//	//if(pObject->)
//}

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
    EventContainer* result = pObject->pAllEventContainer[index] ;
    if(result == NULL)
    {
        if(isAutoCreate)
        {
            pObject->pAllEventContainer[index] = CreateEventContainer(EventName & 0xffff);//TODO: Need free
        	result = pObject->pAllEventContainer[index];
        }

    }

    return result;
}

InputTarget* UIObjectGetInputTarget(UIObject* pObject,BOOL isAutoCreate)
{
	InputTarget* pResult = NULL;
	pResult = pObject->pInputTarget;
	if(pResult == NULL && isAutoCreate)
	{
		pObject->pInputTarget = CreateInputTarget();
		pResult = pObject->pInputTarget;
	}

	return pResult;
}



uint8_t UpdataVisibleRectWithUIObjectTransInfo(UIObjectEffectHeader* pEffect, RealRECT* pVisibleRect)
{
	UIObjectTransInfo* pTransInfo = (UIObjectTransInfo*)pEffect;
	if(pTransInfo != NULL)
	{
		UIObjectTransInfoRotateRect(pTransInfo, pVisibleRect);
		return 1;
	}
	return 0;
}

int GetRenderScriptWithUIObjectTransInfo(UIObjectEffectHeader* pEffect, char* szScriptCode, uint16_t uLength)
{
	UIObjectTransInfo* pTransInfo = (UIObjectTransInfo*)pEffect;
	if(pTransInfo != NULL)
	{
		char szCodeBuf[100];
		sprintf(szCodeBuf, "[\"matrix\"]={0,0,1,1,%f,%f,%f}",
			pTransInfo->fRotateInfo[0],pTransInfo->fRotateInfo[1],pTransInfo->fRotateInfo[2]);
		uint16_t uCodeLen = strlen(szCodeBuf);
		if(uCodeLen > uLength)
		{
			return uCodeLen;
		}
		else
		{
			strcpy(szScriptCode, szCodeBuf);
			return uCodeLen;
		}
	}
	return -1;
}

UIObjectTransInfo* CreateUIObjectTransInfo()
{
	UIObjectTransInfo* pNewTransInfo = NULL;
	pNewTransInfo = (UIObjectTransInfo*)malloc(sizeof(UIObjectTransInfo));
	((UIObjectEffectHeader*)pNewTransInfo)->cbSize = sizeof(UIObjectTransInfo);
	((UIObjectEffectHeader*)pNewTransInfo)->fnUpdateVisibleRect = UpdataVisibleRectWithUIObjectTransInfo;
	((UIObjectEffectHeader*)pNewTransInfo)->fnGetRenderScript = GetRenderScriptWithUIObjectTransInfo;
	return pNewTransInfo;
}
void DestroyUIObjectTransInfo(UIObjectTransInfo* pTransInfo)
{
	free(pTransInfo);
}

void UIObjectTransInfoRotatePoint(UIObjectTransInfo* pTransInfo, float* x, float* y)
{
	float rx, ry;
	float cx = *x - pTransInfo->fRotateInfo[1];
	float cy = *y - pTransInfo->fRotateInfo[2];
	rx = cx * pTransInfo->fMatrix[0] + cy * pTransInfo->fMatrix[1] + pTransInfo->fRotateInfo[1];
	ry = cy * pTransInfo->fMatrix[0] - cx * pTransInfo->fMatrix[1] + pTransInfo->fRotateInfo[2];
	*x = rx;
	*y = ry;
}

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

void UIObjectTransInfoRotateRect(UIObjectTransInfo* pTransInfo, RealRECT * rect)
{
	////先不管优化什么了
	float ltx = rect->left,lty = rect->top;
	UIObjectTransInfoRotatePoint(pTransInfo, &ltx, &lty);
	float rtx = rect->right,rty = rect->top;
	UIObjectTransInfoRotatePoint(pTransInfo, &rtx, &rty);
	float lbx = rect->left,lby = rect->bottom;
	UIObjectTransInfoRotatePoint(pTransInfo, &lbx, &lby);
	float rbx = rect->right,rby = rect->bottom;
	UIObjectTransInfoRotatePoint(pTransInfo, &rbx, &rby);
	
	rect->left = min(ltx, min(rtx, min(lbx, rbx)));
	rect->right = max(ltx, max(rtx, max(lbx, rbx)));
	rect->top = min(lty, min(rty, min(lby, rby)));
	rect->bottom = max(lty, max(rty, max(lby, rby)));

}

void SetUIObjectTransInfoRotate(UIObjectTransInfo* pTransInfo, float fDegree, float fCenterX, float fCenterY)
{
	if(pTransInfo != NULL)
	{
		pTransInfo->fRotateInfo[0] = fDegree;
		pTransInfo->fRotateInfo[1] = fCenterX;
		pTransInfo->fRotateInfo[2] = fCenterY;

		//fDegree = fDegree;
		//fDegree = fDegree - 360 * (int)(fDegree / 360);

		pTransInfo->fMatrix[0] = sin(fDegree * M_PI / 180);
		pTransInfo->fMatrix[1] = cos(fDegree * M_PI / 180);
	}
}
