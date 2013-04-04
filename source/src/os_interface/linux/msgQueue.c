//
// msgQueue.c 
//
//////////////////////////////////////////////////////////////////////////
#include "./perheader.h"
#include "../os_interface.h"



typedef struct tagRunLoopFilterNode
{

}RunLoopFilterNode;

typedef struct tagThreadMsgRunLoop
{
	
}ThreadMsgRunLoop;

typedef struct tagMsgQueueHeader
{
	TYPE_NGOS_SHAREMEMORY hData;
	TYPE_NGOS_SEMAPHORE   hSem;
	TYPE_NGOS_MUTEX       hWriteLock;
}MsgQueueHeader;

typedef struct tagMsgReciverObject
{
	TYPE_NGOS_TID OwnerTID;
	TYPE_NGOS_PID OwnerPID;
	pfnMsgProc fnMsgProc;
	UserDataContext* UserContext;
}MsgReciverObject;//内核对象

static MsgReciverObject* DecodeMsgReciverObject(TYPE_NGOS_MSG_RECIVER hReciver)
{

}

static TYPE_NGOS_MSG_RECIVER EncodeMsgReciverObject(MsgReciverObject* pObject)
{
	static TYPE_NGOS_SHAREMEMORY hHandleMapSM;

}

static int InvalidMsgReciverHandle(TYPE_NGOS_MSG_RECIVER hReciver)
{

}

static TYPE_NGOS_TID GetThreadIDFromMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver)
{
	MsgReciverObject* pObject = DecodeMsgReciverObject(hReciver);
	if(pObject)
	{
		return pObject->OwnerTID;
	}

	return -1;
}

static int GetMsgQueueHeaderFromThread(TYPE_NGOS_TID tid，MsgQueueHeader* pResult)
{
	//通过tid拼出一个key

	//尝试打开share memory,sem,mutex
	//
}


static int MsgQueuePushBack(MsgQueueHeader* pQueueHeader,TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData)
{

}

static int MsgQueuePopFront(MsgQueueHeader* pQueueHeader,RecivedMsg* pResult)
{

}

TYPE_NGOS_MSG_RECIVER OSI_CreateMsgReciver(pfnMsgProc fnMsgProc,UserDataContext* udc)
{
	if(fnMsgProc == NULL)
	{
		return NULL;
	}

	MsgReciverObject* pObject = (MsgReciverObject*) malloc(sizeof(MsgReciverObject));
	pObject->OwnerTID = OSI_GetThreadID();
	pObject->OwnerPID = OSI_GetPID();
	pObject->fnMsgProc = fnMsgProc;
	pObject->UserContext = udc;
	return EncodeMsgReciverObject(pObject);
}

int OSI_ReleaseMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver)
{
	MsgReciverObject* pObj = DecodeMsgReciverObject(hReciver);
	if(pObj)
	{
		if(pObj->UserContext)
		{
			if(pObj->UserContext->fnReleaseUserData)
			{
				pObj->UserContext->fnReleaseUserData(pObj->UserContext->UserData);
			}
		}
		InvalidMsgReciverHandle(hReciver);
		free(pObj);
	}

	return 0;
}

//msg == 0为退出消息循环消息，唯一的内置消息
int OSI_PostMsg(TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData)
{
	//get hReciver's tid
	TYPE_NGOS_TID tid = GetThreadIDFromMsgReciver(hReciver);
	if(tid < 1)
	{
		return -1;
	}
	//get struct msgqueue from tid
	MsgQueueHeader header;
	GetMsgQueueHeaderFromThread(tid,&header);
	if(header.hData == NULL)
	{
		return -1;
	}

	OSI_LockMutex(header.hWriteLock);
	if(MsgQueuePushBack(&header,hReciver,msg,param1,param2,msgData) != 0)
	{
		OSI_UnlockMutex(header.hWriteLock);
		return -1;
	}

	OSI_UnlockMutex(header.hWriteLock);
	OSI_ActiveSemaphore(header.hSem);
	
	return 0;
}

int OSI_SendMsg(TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData,int32_t maxWaitTime)
{
	//暂时不实现，跨进程的sendmsg太不可靠了
	return -1;
}


TYPE_NGOS_MSGQUEUE_RUNLOOP OSI_GetMsgQueueLoop(TYPE_NGOS_TID tid)
{

	if(tid == 0)
	{
		//get current msg queue loop
		ThreadMsgRunLoop* pResult = OSI_GetTLSValue(tid,1984);
		if(pResult==NULL)
		{
			//create
		}

		return pResult;
	}
	else
	{
		//暂不支持...
		return NULL;
	}
	return NULL;
}

TYPE_NGOS_MSGQUEUE_RUNLOOP OSI_CloneMsgQueueRunLoop(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop)
{
	//为了DoModel，暂不实现
	return NULL;
}

int OSI_ReleaseMsgQueueRunLoop(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop)
{
	return 1;
}

int OSI_RunMsgQueueLoop()
{
	//no msg filter support:
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
	int result = -1;
	TYPE_NGOS_TID ctid = OSI_GetThreadID();
	MsgQueueHeader header;
	GetMsgQueueHeaderFromThread(ctid,&header);
	if(header.hData == NULL)
	{
		//首次使用,创建
	}

	if(header.hData)
	{
		if(OSI_WaitSemaphore(header.hSem) == 0)
		{
			OSI_LockMutex(header.hWriteLock);
			result = MsgQueuePopFront(&header,pResult)==0;
			OSI_UnlockMutex(header.hWriteLock)
		}
	}

	return result;
}

int OSI_PeekMsg(RecivedMsg* pResult)
{
	return 0;
}

int OSI_DispatchMsg(RecivedMsg* pMsg)
{
	if(pMsg->TargetReciver)
	{
		MsgReciverObject* pObj = DecodeMsgReciverObject(pMsg->TargetReciver);
		if(pObj)
		{
			if(pObj->fnMsgProc)
			{
				pObj->fnMsgProc(pMsg,pMsg->TargetReciver,pObj->UserContext);
			}
		}
	}
	else
	{
		//call thread msg ?
	}

	return 0;
}

int OSI_DestroyMsg(RecivedMsg* pMsg)
{
	if(pMsg)
	{
		if(pMsg->MsgData)
		{
			OSI_ReleaseMsgData(pMsg->MsgData);
		}
	}
	return 0;
}
