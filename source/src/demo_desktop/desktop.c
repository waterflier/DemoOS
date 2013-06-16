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

int isHaveTimer = 0;

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
	int count=0;
	while(1)
	{
		usleep(25000);
		count++;

		/*if(isHaveTimer == 0)
		{*/
			if(count % 40 == 0)
			{
				OSI_PostMsg(hMainMsgRecv,MSG_USER_TIMER,TIMER_ID_CLOCK_TICK,0,NULL);
			}
			OSI_PostMsg(hMainMsgRecv,MSG_USER_TIMER,TIMER_ID_UPDATE_TREE,0,NULL);
			isHaveTimer = 1;
		/*}*/
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
     	//printf("get action=%d,x=%u,y=%u\n",pMsg->Param1,x,y);
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
			if(!strlen(NGREGetScriptCode(hRenderScript)))
			{
				//printf("empty script\n");
			}
			
			//printf("run\n");
			NGRERunScript(hRenderScript, pDevice);
			//printf("flush\n");
			NGREFlushDevice(pDevice);
			
			
        }
        else if(pMsg->Param1 == TIMER_ID_CLOCK_TICK)
        {
            //一秒钟过去了...
			UpdateClockTime(NULL);
        }
        isHaveTimer = 0;
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

	NGRM_AddResource("sb_bkg","/system/res/sb_bkg.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("sb_weather","/system/res/sb_weather.png", NGRM_ResType_Bitmap);
	//need time
	NGRM_AddResource("sb_date","/system/res/sb_date.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("sb_blue","/system/res/sb_blue.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("sb_wifi","/system/res/sb_wifi.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("sb_radio","/system/res/sb_radio.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("sb_battle","/system/res/sb_battle.png", NGRM_ResType_Bitmap);

	//need apptext
	NGRM_AddResource("head_line","/system/res/head_line.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("head_dot","/system/res/head_dot.png", NGRM_ResType_Bitmap);



	NGRM_AddResource("app_icon.call","/system/res/apps/call.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.camer","/system/res/apps/camer.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.contact","/system/res/apps/contact.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.e-mail","/system/res/apps/e-mail.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.folder","/system/res/apps/folder.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.map","/system/res/apps/map.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.music","/system/res/apps/music.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.note","/system/res/apps/note.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.sm","/system/res/apps/sm.png", NGRM_ResType_Bitmap);

	NGRM_AddResource("app_add","/system/res/app_add.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_del","/system/res/app_del.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_pao","/system/res/app_pao.png", NGRM_ResType_Bitmap);
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
		//printf("find obj\n");
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
	
	//init librarys.
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
	//hApp1 = CreateAppIcon("call","call");
	//RECT posIcon = {52,95,52+86,95+86+28};
	//NGOS_SetUIObjectRect(hApp1,&posIcon);
	//NGOS_AddChild(hBkg,hApp1);
	//UIObject* pAppObj = HandleMapDecodeUIObject(hApp1);
	//if(pAppObj)
	//{
	//	UIObjectGetInputTarget(pAppObj,1);
	//	EventContainer* pEA = UIObjectGetEventContainer(pAppObj,NGOS_ACTION_TOUCH_DOWN,TRUE);
	//	AttachEvent(pEA,(void*)OnTouchDownApp,NULL);

	//	EventContainer* pEA2 = UIObjectGetEventContainer(pAppObj,NGOS_ACTION_TOUCH_DOWN,FALSE);
	//}

	//****************************************
	NGOS_UIOBJECT_HANDLE hStatusBar = CreateStatusBar(0,0,800,35);
	NGOS_AddChild(hBkg,hStatusBar);

	NGOS_UIOBJECT_HANDLE hAppHeader = CreateAppHeader(0,36,800,51);
	NGOS_AddChild(hBkg,hAppHeader);

	NGOS_UIOBJECT_HANDLE hAppList = CreateAppList(0,49+35+2,800,480-(49+35+2));
	NGOS_AddChild(hBkg,hAppList);

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