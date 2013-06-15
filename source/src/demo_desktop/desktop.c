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
	 printf("main msgqueue recv msg! %d\n",pMsg->Msg);
     switch(pMsg->Msg)
     {
     case MSG_INPUT_ACTION:
     	SendInputAcitonToUIObjTree(pMsg->Param1,(uint32_t)pMsg->Param2>>16,(uint32_t)pMsg->Param2&0xffff,pMsg->MsgData);
        break;
     case MSG_USER_TIMER:
        if(pMsg->Param1 == TIMER_ID_UPDATE_TREE)   
        {
        	//todo: 
            //NGOS_UpdateRootObjTree(hTree);
			NGREClearScript(hRenderScript);
			NGOS_UpdateRootObjTree(hTree, hRenderScript);
			//printf("%s\r\n",NGREGetScriptCode(hRenderScript));
			NGRERunScript(hRenderScript, pDevice);
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
	RECT pos = {.left=0,.top=0,.right=screenWidth,.bottom=screenHeight};
	NGOS_SetUIObjectRect(hResult,&pos);

	return hResult;
}


void InitRes()
{
	NGRE_RESULT lResult = NGREInit();
	NGRM_AddResource("bkg", "/system/res/bkg.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.call","/system/res/call.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.camer","/system/res/camer.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("app_icon.clock","/system/res/clock.png", NGRM_ResType_Bitmap);
}

static int OnTouchDownApp (UserDataContext* pUserData,NGOS_UIOBJECT_HANDLE hObj,uint32_t x,uint32_t y,MTEventData* pData)
{
	printf("You touch me!\n");
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
	
	
	//create uiobjtree
	hTree = NGOS_CreateRootObjTree(NULL);
	NGOS_UIOBJECT_HANDLE hRoot = NGOS_GetRootObject(hTree);

	NGOS_UIOBJECT_HANDLE hBkg = CreateDesktopBKG();
	NGOS_AddChild(hRoot,hBkg);
	//*****************test*******************
	NGOS_UIOBJECT_HANDLE hApp1 = CreateAppIcon("call","call");
	RECT posIcon = {20,20,20+256,20+256+44};
	NGOS_SetUIObjectRect(hApp1,&posIcon);
	NGOS_AddChild(hBkg,hApp1);
	UIObject* pAppObj = HandleMapDecodeUIObject(hApp1);
	if(pAppObj)
	{
		UIObjectGetInputTarget(pAppObj,1);
		EventContainer* pEA = UIObjectGetEventContainer(pAppObj,NGOS_ACTION_TOUCH_DOWN,TRUE);
		AttachEvent(pEA,(void*)OnTouchDownApp,NULL);
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
	StartTimerThread();
	//start userinput thread
	StartInputEventThread();
	//enter main loop	
	OSI_RunMsgQueueLoop();

}	