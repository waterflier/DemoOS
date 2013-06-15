//
// desktop.c
// 
// main in demo 
//
//
//////////////////////////////////////////

#include "./perheader.h"
#include <stdlib.h>
#include <stdio.h>

#include "./app_header.h"
#include "./app_list.h"
#include "./buttom.h"
#include "./clock.h"
#include "./status_bar.h"
#include "../input/InputReader.h"

#include "RenderEngine.h"
#include "RenderScript.h"
#include "ResourceManager.h"
#include "RenderDevice.h"

#include "../kernel/UIObject.h"
#include "../kernel/UIObjTree.h"
short screenWidth = 800;
short screenHeight = 600;
short screenDPI = 72;

TYPE_NGOS_MSG_RECIVER hMainMsgRecv;
NGOS_ROOT_OBJTREE_HANDLE hTree;

NGRE_SCRIPT_HANDLE hRenderScript;
NGREDevice* pDevice;

#define  MSG_USER_TIMER 1

#define TIMER_ID_UPDATE_TREE 0
#define TIMER_ID_CLOCK_TICK 1

short GetBatteryStatus()
{
	
}

void GetScrennInfo()
{
	return;
}

void* TimerThread(void* ud)
{
	printf("start timer thread\n");
	while(1)
	{
		usleep(25000);
		OSI_PostMsg(hMainMsgRecv,MSG_USER_TIMER,TIMER_ID_UPDATE_TREE,0,NULL);
	}
}

void StartTimerThread()
{
	OSI_CreateThread(NULL,TimerThread);
	return;
}

void StartInputEventThread()
{
	InputReader* pReader = CreateInputReader();
	SetInputReadeActionReciver(pReader,hMainMsgRecv); 
    InputReaderStart(pReader);
	return;
}

 int fnMainMsgProc (RecivedMsg* pMsg,TYPE_NGOS_MSG_RECIVER hReciver,UserDataContext* udc)
 {
	 //printf("main msgqueue recv msg! %d\n",pMsg->Msg);
	 uint16_t x,y;
	 RootUIObjTree* pObjTree;
     switch(pMsg->Msg)
     {
     case MSG_INPUT_ACTION:
     	
     	x = (uint32_t)(pMsg->Param2)>>16;
     	y = (uint32_t)(pMsg->Param2)&0xffff;
     	printf("get action=%d,x=%u,y=%u\n",pMsg->Param1,x,y);
     	pObjTree = (RootUIObjTree*)HandleMapDecodeRootTree(hTree,NULL);
     	SendInputAcitonToUIObjTree(pObjTree,pMsg->Param1,x,y,pMsg->MsgData);
        break;
     case MSG_USER_TIMER:
        if(pMsg->Param1 == TIMER_ID_UPDATE_TREE)   
        {
        	//todo: 
            //NGOS_UpdateRootObjTree(hTree);
            //printf("clean\n");
			NGREClearScript(hRenderScript);
			//printf("update\n");
			NGOS_UpdateRootObjTree(hTree, hRenderScript);
			//printf("%s\r\n",NGREGetScriptCode(hRenderScript));
			//printf("run\n");
			NGRERunScript(hRenderScript, pDevice);
			//printf("flush\n");
			NGREFlushDevice(pDevice);
        }
        else if(pMsg->Param1 == TIMER_ID_CLOCK_TICK)
        {
            //一秒钟过去了...

        }
        break; 
     }

	 return 0;
 }


NGOS_UIOBJECT_HANDLE CreateDesktopBKG()
{
	NGOS_UIOBJECT_HANDLE hResult;
	
	hResult = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","bkg");
	NGOS_ImageObjectSetImageID(hResult,"bkg");
	RECT pos = {.left=0,.top=0,.right=800,.bottom=480};
	NGOS_SetUIObjectRect(hResult,&pos);

	return hResult;
}


void InitRes()
{
	NGRE_RESULT lResult = NGREInit();
	NGRM_AddResource("bkg", "/system/res/bkg.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.call","/system/res/apps/call.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.camer","/system/res/apps/camer.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.clock","/system/res/apps/clock.png", NGRM_ResType_Bitmap);
}
NGOS_UIOBJECT_HANDLE hApp1;
static bSwitch = 0;
static int OnTouchDownApp (UserDataContext* pUserData,NGOS_UIOBJECT_HANDLE hObj,uint32_t x,uint32_t y,MTEventData* pData)
{
	printf("You touch me!\n");
	RootUIObjTree* pObjTree = (RootUIObjTree*)HandleMapDecodeRootTree(hTree,NULL);
	NGOS_UIOBJECT_HANDLE hIcon = UIObjTreeGetObjectByPath(pObjTree,"app_icon.call");
	if(hIcon)
	{
		printf("find obj\n");
		if(bSwitch == 0)
		{
			NGOS_ImageObjectSetImageID(hIcon,"app_icon.camer");
			bSwitch = 1;
		}
		else
		{
			NGOS_ImageObjectSetImageID(hIcon,"app_icon.clock");
			bSwitch = 0;
		}
	}

}

int main(int argc,char** argv)
{
	printf("demo_desktop start...\n");
	//get screen info
	GetScrennInfo();
	//init librarys
	NGOS_InitRootUIObjTreeEnv(NULL);
	OSI_InitMsgQueue();
	hMainMsgRecv = OSI_CreateMsgReciver(fnMainMsgProc,NULL);
	hRenderScript = NGRECreateScript();
	pDevice = NULL;
	NGREOpenDevice(&pDevice);
	int nDeviceWidth, nDeviceHeight;
	NGREGetDeviceSize(pDevice, &nDeviceWidth, &nDeviceHeight);
	InitRes();

	//create uiobjtree
	hTree = NGOS_CreateRootObjTree(NULL);
	NGOS_SetRootUIObjTreeViewPort(hTree, nDeviceWidth, nDeviceHeight);
	NGOS_UIOBJECT_HANDLE hRoot = NGOS_GetRootObject(hTree);

	NGOS_UIOBJECT_HANDLE hBkg = CreateDesktopBKG();
	NGOS_AddChild(hRoot,hBkg);
	//*****************test*******************
	hApp1 = CreateAppIcon("call","call");
	RECT posIcon = {20,20,20+256,20+256+44};
	NGOS_SetUIObjectRect(hApp1,&posIcon);
	NGOS_AddChild(hBkg,hApp1);
	UIObject* pAppObj = HandleMapDecodeUIObject(hApp1);
	if(pAppObj)
	{
		UIObjectGetInputTarget(pAppObj,1);
		EventContainer* pEA = UIObjectGetEventContainer(pAppObj,NGOS_ACTION_TOUCH_DOWN,TRUE);
		AttachEvent(pEA,(void*)OnTouchDownApp,NULL);

		EventContainer* pEA2 = UIObjectGetEventContainer(pAppObj,NGOS_ACTION_TOUCH_DOWN,FALSE);
        if(pEA2)
        {
        	printf("!!!|\n");
            //FIRE_EVENT(pEA2,CALLBACK_OnTouch,hObj,x,y,pData);
        }
        else
        {
        	printf("fuck\n");
        	return 0;
        }
	}
	RootUIObjTree* pObjTree = (RootUIObjTree*)HandleMapDecodeRootTree(hTree,NULL);
	NGOS_UIOBJECT_HANDLE hooo = UIObjTreeGetObjectByPath(pObjTree,"bkg");
	if(hooo)
	{
		printf("***************findit\n");
	}
	//****************************************
	//NGOS_UIOBJECT_HANDLE hStatusBar = CreateStatusBar(0,0,800,24);
	//NGOS_AddChild(hRoot,hStatusBar);

	//NGOS_UIOBJECT_HANDLE hAppHeader = CreateAppHeader(800,24);
	//NGOS_AddChild(hRoot,hAppHeader);

	//NGOS_UIOBJECT_HANDLE hAppList = CreateAppList(800,400);
	//NGOS_AddChild(hRoot,hAppList);

	//NGOS_UIOBJECT_HANDLE hAppBottom = CreateAppBottom(800,60);
	//NGOS_AddChild(hRoot,hAppBottom);
	//start timer thread
	//NGOS_UpdateRootObjTree(hTree, hRenderScript);
	StartTimerThread();

	//start userinput thread
	StartInputEventThread();
	//enter main loop	

	printf("enter run loop\n");
	OSI_RunMsgQueueLoop();

	return 0;
}	