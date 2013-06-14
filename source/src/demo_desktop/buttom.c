//
// buttom.c
//
/////////////////////

#include "./perheader.h"
#include "./buttom.h"
#include "./app_list.h"

NGOS_UIOBJECT_HANDLE CreateAppBottom(int height,int width)
{
    NGOS_UIOBJECT_HANDLE hBottom = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject","app_bottom");
    RECT pos = {0,1042,width,1042+height}; 
    NGOS_SetUIObjectRect(hBottom,&pos);

    NGOS_UIOBJECT_HANDLE hPhone = CreateAppIcon("call","Call");
    RECT pos2 = {76,14,76+256,14+256+44};
    NGOS_SetUIObjectRect(hPhone,&pos2);
    NGOS_AddChild(hBottom,hPhone);

    NGOS_UIOBJECT_HANDLE hContact = CreateAppIcon("contact","Contact");
    RECT pos3 = {76+256+66,14,76+256+66+256,14+256+44};
    NGOS_SetUIObjectRect(hContact,&pos3);
    NGOS_AddChild(hBottom,hContact);

    NGOS_UIOBJECT_HANDLE hSM = CreateAppIcon("sm","Short Message");
    RECT pos4 = {76+256+66+256+66,14,76+256+66+256+66+256,14+256+44};
    NGOS_SetUIObjectRect(hSM,&pos4);
    NGOS_AddChild(hBottom,hSM);

	return hBottom;
}