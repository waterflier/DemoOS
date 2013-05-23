//
// app_list.h
//
/////////////////////////////////////////////////////

#ifndef _DEMO_APP_LIST_H_
#define _DEMO_APP_LIST_H_

NGOS_UIOBJECT_HANDLE CreateAppList();

NGOS_UIOBJECT_HANDLE CreateAppIcon(const char* appID,const char* appText);

//AppList interface 
void AppListAddAppIcon(NGOS_UIOBJECT_HANDLE hList,NGOS_UIOBJECT_HANDLE hAppIcon);
void AppListRemoveAppIcon(NGOS_UIOBJECT_HANDLE hList,const char* appID);
void UpdateAppList(NGOS_UIOBJECT_HANDLE hList);

//void ChangeAppListMode(NGOS_UIOBJECT_HANDLE hList,int mode);


#endif //_DEMO_APP_LIST_H_