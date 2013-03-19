//
// msgQueue.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../os_interface.h"


TYPE_NGOS_MSG_RECIVER OSI_CreateMsgReciver(pfnMsgProc fnMsgProc,UserDataContext* udc)
{
	return NULL;
}

int OSI_ReleaseMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver)
{
	return 0;
}

//msg == 0为退出消息循环消息，唯一的内置消息
int OSI_PostMsg(TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData)
{
	return;
}

int OSI_SendMsg(TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData,int32_t maxWaitTime)
{
	return;
}


TYPE_NGOS_MSGQUEUE_RUNLOOP OSI_GetMsgQueueLoop(TYPE_NGOS_TID tid)
{
	return NULL;
}

TYPE_NGOS_MSGQUEUE_RUNLOOP OSI_CloneMsgQueueRunLoop(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop)
{
	return NULL;
}

int OSI_ReleaseMsgQueueRunLoop(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop)
{
	return 1;
}

int OSI_RunMsgQueueLoop()
{
	RecivedMsg msg;
	while(OSI_GetMsg(&msg) == 0)
	{
		OSI_DispatchMsg(&msg);
		OSI_DestroyMsg(&msg);
	}

	return 0;
}

int OSI_AddMsgFilter(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop,pfnMsgFilter fnMsgFilter,UserDataContext* udc)
{
	return 0;
}

int OSI_RemoveMsgFilter(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop,int cookie)
{
	return 0;
}

//不推荐直接使用的函数，应该使用系统内置的MsgQueueLoop和Filter
int OSI_GetMsg(RecivedMsg* pResult)
{
	return 0;
}

int OSI_PeekMsg(RecivedMsg* pResult)
{
	return 0;
}

int OSI_DispatchMsg(RecivedMsg* pMsg)
{
	return 0;
}

int OSI_DestroyMsg(RecivedMsg* pMsg)
{
	return 0;
}
