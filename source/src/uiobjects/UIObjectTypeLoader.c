//
// UIObjectTypeLoader.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObjectTypeLoader.h"
#include "./ImageObject.h"
#include "./TextObject.h"
#include "./TextureObject.h"

static TypeLoader s_defaultUIObjectLoader = {0};

static UIObjectTypeInfo* GetDefaultUIObjectTypeInfo(TypeLoader* self,const char* className)
{
	if(className)
	{
		switch(*className)
		{
		case 'I':
			if(strcmp(className,"ImageObject") == 0)
			{
				//GetImageObjectTypeInfo()->Create
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
		}
	}

	return NULL;
}



TypeLoader* NGOS_GetDefaultUIObjectTypeLoader()
{
	if(s_defaultUIObjectLoader.fnCreateUIObject == NULL)
	{
		//init it
		//s_defaultUIObjectLoader.fnCreateUIObject = CreateDefaultUIObject;
		s_defaultUIObjectLoader.fnGetUIObjectTypeInfo = NULL;
		s_defaultUIObjectLoader.fnGetUIObjectTypeInfoByClassName = NULL;
		s_defaultUIObjectLoader.fnAddRef = NULL;
		s_defaultUIObjectLoader.fnRelease = NULL;
		s_defaultUIObjectLoader.fnRegisterTypeInfo = NULL;
	}

	return &s_defaultUIObjectLoader;
}
