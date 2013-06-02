//
// app_header.c
//
//////////////////////////////////////

#include "./perheader.h"
#include "./app_header.h"

NGOS_UIOBJECT_HANDLE CreateAppHeader(int width,int height)
{
    NGOS_UIOBJECT_HANDLE hHeader = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject","app_header");
    RECT pos = {0,56,width,56+height}; 
    NGOS_SetUIObjectRect(hHeader,&pos);

    NGOS_UIOBJECT_HANDLE hHeadLine = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","header.line");
    NGOS_ImageObjectSetImageID(hHeadLine,"head_line");
    pos.left = 0;
    pos.top = height-4;
    pos.right = width;
    pos.bottom = height;
    NGOS_SetUIObjectRect(hHeadLine,&pos);
    NGOS_AddChild(hHeader,hHeadLine);

    //Dot pointer

	return hHeader;
}