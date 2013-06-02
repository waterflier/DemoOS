//
// app_list.c
//
/////////////////////////////////////////

#include "./perheader.h"
#include "./app_list.h"

#define ICON_WIDTH (256)

NGOS_UIOBJECT_HANDLE appArray[256] = {0};

NGOS_UIOBJECT_HANDLE CreateAppList(int width,int height)
{
    NGOS_UIOBJECT_HANDLE hList = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject","app_list");
    RECT pos = {0,160,width,160+height}; 
    NGOS_SetUIObjectRect(hList,&pos);

	return hList;
}



NGOS_UIOBJECT_HANDLE CreateAppIcon(const char* appID,const char* appText)
{
    char buffer[1024];
    sprintf(buffer,"%s.%s","app",appID);
    NGOS_UIOBJECT_HANDLE hApp = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject",buffer);
    RECT pos = {0,0,ICON_WIDTH,ICON_WIDTH+44};
    NGOS_SetUIObjectRect(hApp,&pos);

    sprintf(buffer,"%s.%s","app_icon",appID);
    NGOS_UIOBJECT_HANDLE hIcon = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject",buffer);
    NGOS_ImageObjectSetImageID(hIcon,appID);
    RECT pos2={0,0,ICON_WIDTH,ICON_WIDTH};
    NGOS_SetUIObjectRect(hIcon,&pos2);
    NGOS_AddChild(hApp,hIcon);
    
    //TODO Add AppText
	
    return hApp;
}

void AppListAddAppIcon(NGOS_UIOBJECT_HANDLE hList,NGOS_UIOBJECT_HANDLE hAppIcon)
{
    
}

void AppListRemoveAppIcon(NGOS_UIOBJECT_HANDLE hList,const char* appID)
{

}

void UpdateAppList(NGOS_UIOBJECT_HANDLE hList)
{

}

//event handle
void OnAppList_TouchBegin(NGOS_UIOBJECT_HANDLE hList,int x,int y)
{
    
}

void OnAppList_TouchEnd(NGOS_UIOBJECT_HANDLE hList,int x,int y)
{

}

void OnAppList_TouchMove(NGOS_UIOBJECT_HANDLE hList,int x,int y)
{

}

void OnApplist_TouchBreak(NGOS_UIOBJECT_HANDLE hList,int x,int y)
{

}

//logic touch event?
void OnAppList_TouchHold()
{

}

void OnAppList_TouchClick()
{

}
