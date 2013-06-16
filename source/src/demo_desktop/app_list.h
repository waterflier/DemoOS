//
// app_list.h
//
/////////////////////////////////////////////////////

#ifndef _DEMO_APP_LIST_H_
#define _DEMO_APP_LIST_H_

#define ICON_W_NUM (5)
#define ICON_H_NUM (3)
#define APP_PAGE_MAX (3)
#define FLIP_SEEP (5.0f)

NGOS_UIOBJECT_HANDLE CreateAppList();



NGOS_UIOBJECT_HANDLE CreateAppIcon(const char* appID,const char* appText);

//AppList interface  index
void AppListAddAppIcon(NGOS_UIOBJECT_HANDLE hList,NGOS_UIOBJECT_HANDLE hAppIcon,int index);
void AppListRemoveAppIcon(NGOS_UIOBJECT_HANDLE hList,const char* appID);
void UpdateAppList(NGOS_UIOBJECT_HANDLE hList);

//void ChangeAppListMode(NGOS_UIOBJECT_HANDLE hList,int mode);


#endif //_DEMO_APP_LIST_H_