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
	NGOS_UIOBJECT_HANDLE hClockObj= NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.icon");
	NGOS_ImageObjectSetImageID(hClockObj,"app_icon.clock");
	RECT pos = {52,95,52+141,95+141+28};
	//NGOS_SetUIObjectRect(hClockObj,&pos);

	NGOS_UIOBJECT_HANDLE hHourPtrObj = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.ptr.hour");
	NGOS_ImageObjectSetImageID(hHourPtrObj,"app_icon.clock.ptr.hour");
	pos.left = 71 - 3, pos.top = 72 - 11 - 2, pos.right =  71 - 3 + 9, pos.bottom = 72 - 11 - 2 + 46;
	NGOS_SetUIObjectRect(hHourPtrObj, &pos);
	NGOS_AddChild(hClockObj, hHourPtrObj);
	
	NGOS_UIOBJECT_HANDLE hMinPtrObj = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.ptr.min");
	NGOS_ImageObjectSetImageID(hMinPtrObj,"app_icon.clock.ptr.min");
	pos.left = 71 - 3, pos.top = 72 - 4 , pos.right =  71 - 3 + 9, pos.bottom = 72 - 4 + 67;
	NGOS_SetUIObjectRect(hMinPtrObj, &pos);
	NGOS_AddChild(hClockObj, hMinPtrObj);

	NGOS_UIOBJECT_HANDLE hSecPtrObj = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","clock.ptr.sec");
	NGOS_ImageObjectSetImageID(hSecPtrObj,"app_icon.clock.ptr.sec");
	pos.left = 71 - 2, pos.top = 72 - 4, pos.right =  71 - 2 + 6, pos.bottom = 72- 4 + 57;
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
	//int nSec = 15;
	
	float fDgSec = nSec * 6 + 180;
	float fDgMin = (float)(nMin *  60 + nSec) / 10 + 180;
	float fDgHour = (float)(nHour * 3600 + nMin * 60 + nSec) / 120 + 180;

	float fRotateInfo[3];
	fRotateInfo[0] = fDgHour, fRotateInfo[1] = 3, fRotateInfo[2]= 11 + 2; 
	NGOS_SetUIObjectRotate(s_ClockObj.hHourPtrObj, fRotateInfo);
	fRotateInfo[0] = fDgMin, fRotateInfo[1] = 3, fRotateInfo[2]= 4; 
	NGOS_SetUIObjectRotate(s_ClockObj.hMinPtrObj, fRotateInfo);
	fRotateInfo[0] = fDgSec, fRotateInfo[1] = 2, fRotateInfo[2]= 4; 
	NGOS_SetUIObjectRotate(s_ClockObj.hSecPtrObj, fRotateInfo);
}