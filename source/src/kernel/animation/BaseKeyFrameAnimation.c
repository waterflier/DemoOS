//
// BaseKeyFrameAnimation.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "../objectIndex/AnimationVector.h"
#include "../TypeLoader.h"
#include "../../uiobjects/UIObjectTypeLoader.h"
#include "./BaseKeyFrameAnimation.h"


static void AlphaChangeAnimationAction(BaseAnimation* pSelf,float progress)
{
	if(pSelf)
	{
		AlphaChangeAnimation* pAni = (AlphaChangeAnimation*) GetAnimationUserData(pSelf);
		int nowAlpha = pAni->StartAlpha + (pAni->EndAlpha - pAni->StartAlpha) * progress;
		//printf("will set alpha=%d\n",nowAlpha);
		NGOS_SetUIObjectAlpha(pSelf->hBindObj,nowAlpha);
	}
}

static AnimationProvier s_AlphaChangeAnimationProvider = 
{
	.fnAction = AlphaChangeAnimationAction
};

void AlphaChangeAnimationSetAlpha(BaseAnimation* pSelf,uint8_t start,uint8_t end)
{
	if(pSelf)
	{
		AlphaChangeAnimation* pAni = (AlphaChangeAnimation*) GetAnimationUserData(pSelf);
		pAni->EndAlpha = end;
		pAni->StartAlpha = start;
	}
}

static const char* GetAlphaChangeAnimationClassName(struct AnimationTypeInfo* self)
{
	return "AlphaChangeAnimation";
}

struct BaseAnimation* CraeteAlphaChangeAnimation (struct AnimationTypeInfo* self,const char* className)
{
	BaseAnimation* pResult = MallocAnimation(sizeof(AlphaChangeAnimation));
	pResult->pImp = &s_AlphaChangeAnimationProvider;
	pResult->RefCount = 1;
	return pResult;
};

AnimationTypeInfo s_AlphaChangeAnimationTypeInfo = 
{
	.fnGetClassName = GetAlphaChangeAnimationClassName,
	.fnCraeteAnimation = CraeteAlphaChangeAnimation,
	.fnGetOwnerTypeLoader = NGOS_GetDefaultTypeLoader,
	.fnAddRef = NULL,
	.fnRelease = NULL
};

AnimationTypeInfo* GetAlphaChangeAnimationTypeInfo()
{
	 return &s_AlphaChangeAnimationTypeInfo;
}

//-----------------------------------------------------------------------------------------------
static const char* GetPosChangeAnimationClassName(struct AnimationTypeInfo* self)
{
	return "PosChangeAnimation";
}

static void PosChangeAnimationAction(BaseAnimation* pSelf,float progress)
{
	if(pSelf)
	{
		PosChangeAnimation* pAni = (PosChangeAnimation*) GetAnimationUserData(pSelf);
		int32_t left = pAni->StartRect.left + (pAni->EndRect.left - pAni->StartRect.left)*progress;
		int32_t top =  pAni->StartRect.top + (pAni->EndRect.top - pAni->StartRect.top)*progress;
		int32_t right =  pAni->StartRect.right + (pAni->EndRect.right - pAni->StartRect.right)*progress;
		int32_t bottom = pAni->StartRect.bottom + (pAni->EndRect.bottom - pAni->StartRect.bottom)*progress;

		RECT newRect;
		newRect.left = left;
		newRect.top = top;
		newRect.right = right;
		newRect.bottom = bottom;

		NGOS_SetUIObjectRect(pSelf->hBindObj,&newRect);
	}
}

static AnimationProvier s_PosChangeAnimationProvider = 
{
	.fnAction = PosChangeAnimationAction
};

struct BaseAnimation* CreatePosChangeAnimation (struct AnimationTypeInfo* self,const char* className)
{
	BaseAnimation* pResult = MallocAnimation(sizeof(PosChangeAnimation));
	pResult->pImp = &s_PosChangeAnimationProvider;
	pResult->RefCount = 1;
	return pResult;
}

AnimationTypeInfo s_PosChangeAnimationTypeInfo = 
{
	.fnGetClassName = GetPosChangeAnimationClassName,
	.fnCraeteAnimation = CreatePosChangeAnimation,
	.fnGetOwnerTypeLoader = NGOS_GetDefaultTypeLoader,
	.fnAddRef = NULL,
	.fnRelease = NULL
};

AnimationTypeInfo* GetPosChangeAnimationTypeInfo()
{
	return &s_PosChangeAnimationTypeInfo;
}


void PosChangeAnimationSetPos(BaseAnimation* pSelf,int32_t startLeft,int32_t startTop,int32_t endLeft,int32_t endTop)
{
	RECT objRect;
	if(pSelf)
	{
		PosChangeAnimation* pAni = (PosChangeAnimation*) GetAnimationUserData(pSelf);
		NGOS_GetUIObjectAbsRect(pSelf->hBindObj,&objRect);
		int32_t width = objRect.right - objRect.left;
		int32_t height = objRect.bottom - objRect.top;

		(pAni->StartRect).left = startLeft;
		(pAni->StartRect).top = startTop;
		(pAni->StartRect).right = startLeft + width;
		(pAni->StartRect).bottom = startTop + height;
		(pAni->EndRect).left = endLeft;
		(pAni->EndRect).right = endLeft + width;
		(pAni->EndRect).top = endTop;
		(pAni->EndRect).bottom = endTop + height;
	}
}

void PosChangeAnimationSetRect(BaseAnimation* pSelf,RECT* pStartRect,RECT* pEndRect)
{
	RECT objRect;
	if(pSelf)
	{
		PosChangeAnimation* pAni = (PosChangeAnimation*) GetAnimationUserData(pSelf);
		pAni->StartRect = * pStartRect;
		pAni->EndRect = *pEndRect;
	}
}


//---------------------------------------------------------------------------------------------------
void TransChangeAnimationSetTrans()
{
	return;
}
//---------------------------------------------------------------------------------------------------
void MeshChangeAnimationSetMesh()
{
	return;
}