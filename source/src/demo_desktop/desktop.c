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

short screenWidth = 800;
short screenHeight = 600;
short screenDPI = 72;

TYPE_NGOS_MSG_RECIVER hMainMsgRecv;
NGOS_ROOT_OBJTREE_HANDLE hTree;


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




int main(int argc,char** argv)
{
	printf("demo_desktop start...\n");
	//get screen info
	GetScrennInfo();
	//init librarys
	NGOS_InitRootUIObjTreeEnv(NULL);
	OSI_InitMsgQueue();
	hMainMsgRecv = OSI_CreateMsgReciver(fnMainMsgProc,NULL);

	//create uiobjtree
	hTree = NGOS_CreateRootObjTree(NULL);
	NGOS_UIOBJECT_HANDLE hRoot = NGOS_GetRootObject(hTree);

	NGOS_UIOBJECT_HANDLE hBkg = CreateDesktopBKG();
	NGOS_AddChild(hRoot,hBkg);

	NGOS_UIOBJECT_HANDLE hStatusBar = CreateStatusBar(0,0,800,24);
	NGOS_AddChild(hRoot,hStatusBar);

	NGOS_UIOBJECT_HANDLE hAppHeader = CreateAppHeader(800,24);
	NGOS_AddChild(hRoot,hAppHeader);

	NGOS_UIOBJECT_HANDLE hAppList = CreateAppList(800,400);
	NGOS_AddChild(hRoot,hAppList);

	NGOS_UIOBJECT_HANDLE hAppBottom = CreateAppBottom(800,60);
	NGOS_AddChild(hRoot,hAppBottom);
	//start timer thread
	StartTimerThread();
	//start userinput thread
	StartInputEventThread();
	//enter main loop	
	OSI_RunMsgQueueLoop();

}	