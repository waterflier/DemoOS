//
// UIObjTreeServer.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./UIObjTree.h"
#include "../os_interface/os_interface.h"
#include "./MsgDef.h"


int NGOS_InitRootObjTreeServer()
{
	//初始化一个共享内存，用于存放一些必要的信息，方便client连上来
	return -1;
}

int NGOS_StartRunRootObjTreeServer()
{
	//OSI_CreateMsgReciver();
	//OSI_RunMsgQueueLoop();
	return -1;
}

int NGOS_UninitRootObjTreeServer()
{
	return -1;
}