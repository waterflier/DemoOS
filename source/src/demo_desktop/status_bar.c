//
// status_bar.c
//
///////////////////////////////
#include "./perheader.h"
#include "./status_bar.h"

const char* GetDateImageID(int date)
{
    return "sb_date";
}

const char* GetBatteryImageID(int status)
{
    return "sb_battle";
}



NGOS_UIOBJECT_HANDLE CreateStatusBar(int left,int top,int width,int height)
{
    NGOS_UIOBJECT_HANDLE hStatusBar = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject","statusbar");
    RECT pos = {left,top,left+width,top+height}; 
    NGOS_SetUIObjectRect(hStatusBar,&pos);

    NGOS_UIOBJECT_HANDLE hStatusBarBkg = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","sb.bkg");
    
    NGOS_ImageObjectSetImageID(hStatusBarBkg,"sb_bkg");
    pos.left = 0;
    pos.top = 0;
    pos.right = width;
    pos.bottom = height;
    NGOS_SetUIObjectRect(hStatusBarBkg,&pos);
    NGOS_AddChild(hStatusBar,hStatusBarBkg);

    NGOS_UIOBJECT_HANDLE hWeather = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","sb.weather");
    pos.top = (height-37)/2;
    pos.left = 18;
    pos.right = pos.left+44;
    pos.bottom = pos.top+37;
    NGOS_ImageObjectSetImageID(hWeather,"sb_weather");
    NGOS_SetUIObjectRect(hWeather,&pos);
    NGOS_AddChild(hStatusBarBkg,hWeather);


    //NGOS_UIOBJECT_HANDLE hTime = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"TextObject","sb.time");
    //pos.top = 10;
    //pos.left = 20+37+4;
    //pos.right = pos.top+78;
    //pos.bottom = pos.top+28;
    //NGOS_SetUIObjectRect(hTime,&pos);
    //NGOS_AddChild(hStatusBarBkg,hTime);

    NGOS_UIOBJECT_HANDLE hDate = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","sb.date");
    pos.top = 13;
    pos.left = 158;
    pos.right = pos.left + 28;
    pos.bottom = pos.top + 28;
    NGOS_ImageObjectSetImageID(hDate,GetDateImageID(1));
    NGOS_SetUIObjectRect(hDate,&pos);
    NGOS_AddChild(hStatusBarBkg,hDate);

    NGOS_UIOBJECT_HANDLE hBattery = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","sb.battery");
    pos.top = 12;
    pos.left = width-114;
    pos.right = pos.left + 47;
    pos.bottom = pos.top + 27;
    NGOS_ImageObjectSetImageID(hBattery,GetBatteryImageID(GetBatteryStatus()));
    NGOS_SetUIObjectRect(hBattery,&pos);
    NGOS_AddChild(hStatusBarBkg,hBattery);

    NGOS_UIOBJECT_HANDLE hRadio = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","sb.radio");
    pos.top = 12;
    pos.left = width-114-8-47;
    pos.right = pos.left+47;
    pos.bottom = pos.top + 27;
    NGOS_ImageObjectSetImageID(hRadio,"sb_radio");
    NGOS_SetUIObjectRect(hRadio,&pos);
    NGOS_AddChild(hStatusBarBkg,hRadio);


    NGOS_UIOBJECT_HANDLE hWifi = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","sb.wifi");
    pos.top = 8;
    pos.left = width-114-8-47-8-36;
    pos.right = pos.left+36;
    pos.bottom = pos.top + 33;
    NGOS_ImageObjectSetImageID(hWifi,"sb_wifi");
    NGOS_SetUIObjectRect(hWifi,&pos);
    NGOS_AddChild(hStatusBarBkg,hWifi);

    NGOS_UIOBJECT_HANDLE hBlue = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","sb.blue");
    pos.top = 7;
    pos.left = width-114-8-47-8-36-8-18;
    pos.right = pos.left+18;
    pos.bottom = pos.top + 31;
    NGOS_ImageObjectSetImageID(hBlue,"sb_blue");
    NGOS_SetUIObjectRect(hBlue,&pos);
    NGOS_AddChild(hStatusBarBkg,hBlue);

	return hStatusBar;
}