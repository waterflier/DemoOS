//
// UIObjectTypeLoader.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObjectTypeLoader.h"
#include "../kernel/UIObject.h"
#include "./ImageObject.h"
#include "./TextObject.h"
#include "./TextureObject.h"
#include "../kernel/animation/BaseKeyFrameAnimation.h"

static TypeLoader s_defaultUIObjectLoader = {0};

static const char* GetLayoutObjectClassName(UIObjectTypeInfo* self)
{
	return "LayoutObject";
}

static UIObject* LayoutObjectCreateUIObject(UIObjectTypeInfo* self,const char* className,const char* id)
{
	UIObject* pResult = MallocUIObject(NULL,0);
	UIObjectInit(pResult);
	if(id)
	{
		pResult->strID = malloc(strlen(id)+1);
		strcpy(pResult->strID,id);
	}
	
	return pResult;
}


UIObjectTypeInfo s_LayoutObjecTypeInfo =
{
	.fnGetClassName = GetLayoutObjectClassName,
	.fnCraeteUIObject = LayoutObjectCreateUIObject,
	.fnGetOwnerTypeLoader = NGOS_GetDefaultTypeLoader,
	.fnAddRef = NULL,
	.fnRelease = NULL
};




static UIObjectTypeInfo* GetDefaultUIObjectTypeInfo(TypeLoader* self,const char* className)
{
	if(className)
	{
		switch(*className)
		{
		case 'I':
			if(strcmp(className,"ImageObject") == 0)
			{
				return GetImageObjectTypeInfo();
			}
			break;
		case 'T':
			if(strcmp(className,"TextObject") == 0)
			{

			}
			else if(strcmp(className,"TextureObject") == 0)
			{

			}
			break;
		case 'L':
			if(strcmp(className,"LayoutObject") == 0)
			{
				return &s_LayoutObjecTypeInfo;
			}
			break;
		}
	}

	return NULL;
}

static AnimationTypeInfo* GetDefaultAnimationTypeInfo(TypeLoader* self,const char* className)
{
	if(className)
	{
		switch(*className)
		{
		case 'A':
			if(strcmp(className,"AlphaChangeAnimation") == 0)
			{
				return GetAlphaChangeAnimationTypeInfo(); 
			
			}
			break;
		case 'P':
			if(strcmp(className,"PosChangeAnimation") == 0)
			{
				return GetPosChangeAnimationTypeInfo();
			}

		}
	}
}

TypeLoader* NGOS_GetDefaultTypeLoader()
{
	if(s_defaultUIObjectLoader.fnGetUIObjectTypeInfoByClassName == NULL)
	{
		//init it
		//s_defaultUIObjectLoader.fnCreateUIObject = CreateDefaultUIObject;
		s_defaultUIObjectLoader.fnGetUIObjectTypeInfoByClassName = GetDefaultUIObjectTypeInfo;
		s_defaultUIObjectLoader.fnGetUIObjectTypeInfo = NULL;
		s_defaultUIObjectLoader.fnGetAnimationTypeInfoByClassName = GetDefaultAnimationTypeInfo;
		s_defaultUIObjectLoader.fnAddRef = NULL;
		s_defaultUIObjectLoader.fnRelease = NULL;
		s_defaultUIObjectLoader.fnRegisterTypeInfo = NULL;
	}

	return &s_defaultUIObjectLoader;
}

NGOS_UIOBJECT_HANDLE NGOS_CreateUIObject(TypeLoader* pLoader,const char* className,const char* id)
{
	UIObjectTypeInfo* pType = pLoader->fnGetUIObjectTypeInfoByClassName(pLoader,className);
	if(pType)
	{
		if(pType->fnCraeteUIObject)
		{
			UIObject* pResultObj = pType->fnCraeteUIObject(pType,className,id);
			if(pResultObj)
			{
				pResultObj->pTypeInfo = pType;
				return pResultObj->hSelf;
			}
		}
	}

	return NULL;
}

NGOS_ANIMATION_HANDLE NGOS_CreateAnimation(TypeLoader* pLoader,const char* className)
{
	AnimationTypeInfo* pType = pLoader->fnGetAnimationTypeInfoByClassName(pLoader,className);
	if(pType)
	{
		if(pType->fnCraeteAnimation)
		{
			BaseAnimation* pResult = pType->fnCraeteAnimation(pType,className);
			if(pResult)
			{
				//
				return HandleMapEncodeAnimation(pResult);
			}
		}
	}

	return NULL;
}