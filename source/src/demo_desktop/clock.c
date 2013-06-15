//
// clock.c
//
/////////////////////////

#include "./perheader.h"
#include "./clock.h"
#include "ResourceManager.h"

typedef struct tagClockObject
{
	NGOS_UIOBJECT_HANDLE hClockObj;
	NGOS_UIOBJECT_HANDLE hHourPtrObj;
	NGOS_UIOBJECT_HANDLE hMinPtrObj;
	NGOS_UIOBJECT_HANDLE hSecPtrObj
}ClockObject;

static ClockObject s_ClockObj;


NGOS_UIOBJECT_HANDLE CreateClockAppIcon()
{
	
	NGRM_AddResource("app_icon.clock","/system/res/apps/clock.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.clock.ptr.hour","/system/res/apps/clock.ptr.h.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.clock.ptr.min","/system/res/apps/clock.ptr.m.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.clock.ptr.sec","/system/res/apps/clock.ptr.s.png", NGRM_ResType_Bitmap);

	NGOS_UIOBJECT_HANDLE hClockObj= NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.icon");
	NGOS_ImageObjectSetImageID(hClockObj,"app_icon.clock");
	RECT pos = {52,95,52+86,95+86+28};
	//NGOS_SetUIObjectRect(hClockObj,&pos);

	NGOS_UIOBJECT_HANDLE hHourPtrObj = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.ptr.hour");
	NGOS_ImageObjectSetImageID(hHourPtrObj,"app_icon.clock.ptr.hour");
	pos.left = 43 - 3, pos.top = 43 - 41, pos.right =  43 - 3 + 6, pos.bottom = 43 - 41 + 41;
	NGOS_SetUIObjectRect(hHourPtrObj, &pos);
	NGOS_AddChild(hClockObj, hHourPtrObj);
	
	NGOS_UIOBJECT_HANDLE hMinPtrObj = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.ptr.min");
	NGOS_ImageObjectSetImageID(hMinPtrObj,"app_icon.clock.ptr.min");
	pos.left = 43 - 3, pos.top = 43 - 41, pos.right =  43 - 3 + 6, pos.bottom = 43 - 41 + 41;
	NGOS_SetUIObjectRect(hMinPtrObj, &pos);
	NGOS_AddChild(hClockObj, hMinPtrObj);

	NGOS_UIOBJECT_HANDLE hSecPtrObj = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.ptr.sec");
	NGOS_ImageObjectSetImageID(hSecPtrObj,"app_icon.clock.ptr.sec");
	pos.left = 43 - 2, pos.top = 43 - 41, pos.right =  43 - 2 + 4, pos.bottom = 43 - 41 + 41;
	NGOS_SetUIObjectRect(hSecPtrObj, &pos);
	NGOS_AddChild(hClockObj, hSecPtrObj);

	s_ClockObj.hClockObj = hClockObj;
	s_ClockObj.hHourPtrObj = hHourPtrObj;
	s_ClockObj.hMinPtrObj = hMinPtrObj;
	s_ClockObj.hSecPtrObj = hSecPtrObj;


	return hClockObj;
}

int UpdateClockTime(NGOS_UIOBJECT_HANDLE hClockObj)
{
	time_t ttime;
	time(&ttime);
	struct tm * ltime = gmtime(&ttime); 


	int nHour = ltime->tm_hour;
	int nMin = ltime->tm_min;
	int nSec = ltime->tm_sec;
	
	float fDgSec = nSec * 6;
	float fDgMin = (float)(nMin *  60 + nSec) / 10;
	float fDgHour = (float)(nHour * 3600 + nMin * 60 + nSec) / 120;

	float fRotateInfo[3];
	fRotateInfo[0] = fDgHour, fRotateInfo[1] = 3, fRotateInfo[2]= 41 - 1; 
	NGOS_SetUIObjectRotate(s_ClockObj.hHourPtrObj, fRotateInfo);
	fRotateInfo[0] = fDgMin, fRotateInfo[1] = 3, fRotateInfo[2]= 41 - 1; 
	NGOS_SetUIObjectRotate(s_ClockObj.hMinPtrObj, fRotateInfo);
	fRotateInfo[0] = fDgSec, fRotateInfo[1] = 2, fRotateInfo[2]= 41 - 1; 
	NGOS_SetUIObjectRotate(s_ClockObj.hSecPtrObj, fRotateInfo);
}