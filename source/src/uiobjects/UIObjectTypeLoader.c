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

static TypeLoader s_defaultUIObjectLoader = {0};

static const char* GetLayoutObjectClassName(UIObjectTypeInfo* self)
{
	return "LayoutObject";
}

static UIObject* LayoutObjectCreateUIObject(UIObjectTypeInfo* self,const char* className,const char* id)
{

}


UIObjectTypeInfo s_LayoutObjecTypeInfo =
{
	.fnGetClassName = GetLayoutObjectClassName,
	.fnCraeteUIObject = LayoutObjectCreateUIObject,
	.fnGetOwnerTypeLoader = NGOS_GetDefaultUIObjectTypeLoader,
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



TypeLoader* NGOS_GetDefaultUIObjectTypeLoader()
{
	if(s_defaultUIObjectLoader.fnGetUIObjectTypeInfo == NULL)
	{
		//init it
		//s_defaultUIObjectLoader.fnCreateUIObject = CreateDefaultUIObject;
		s_defaultUIObjectLoader.fnGetUIObjectTypeInfo = GetDefaultUIObjectTypeInfo;
		s_defaultUIObjectLoader.fnGetUIObjectTypeInfoByClassName = NULL;
		s_defaultUIObjectLoader.fnAddRef = NULL;
		s_defaultUIObjectLoader.fnRelease = NULL;
		s_defaultUIObjectLoader.fnRegisterTypeInfo = NULL;
	}

	return &s_defaultUIObjectLoader;
}
