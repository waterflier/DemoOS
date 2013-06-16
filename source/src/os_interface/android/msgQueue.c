//
// msgQueue.c 
//
// in android ,only use thread-to-thread 
//
//
//////////////////////////////////////////////////////////////////////////
#include "./perheader.h"
#include "../os_interface.h"
#include <memory.h>
#include <errno.h>
#include "../../toolkit/hash_map.h"

#define MSG_QUEUE_BUFFER_SIZE (16*1024)
#define MSG_QUEUE_MAX_VALUE (1024)

typedef struct tagRunLoopFilterNode
{

}RunLoopFilterNode;

typedef struct tagThreadMsgRunLoop
{
	
}ThreadMsgRunLoop;

typedef struct tagMsgQueueHeader
{
	unsigned char* 		  pData;
	TYPE_NGOS_SEMAPHORE   hSem;
	TYPE_NGOS_MUTEX       hWriteLock;
	TYPE_NGOS_TID          OwnerTID;
}MsgQueueHeader;



typedef struct tagMsgReciverObject
{
	TYPE_NGOS_TID OwnerTID;
	TYPE_NGOS_PID OwnerPID;
	NGOS_HANDLE   LocalHandle;
	pfnMsgProc fnMsgProc;
	UserDataContext* UserContext;
}MsgReciverObject;

typedef struct tagMsgNode
{
	unsigned char Used;
	TYPE_NGOS_TID FromThread;
	TYPE_NGOS_MSG_RECIVER hReciver;
	uint32_t msg;
	TYPE_NGOS_MSG_PARAM param1;
	TYPE_NGOS_MSG_PARAM param2;
	TYPE_NGOS_MSG_DATA msgData;
}MsgNode;

static int isCreateHashMap = 0;
static TYPE_NGOS_MUTEX hHashMapLock;
static map_t msgQueueMap;

void OSI_InitMsgQueue()
{
	if(isCreateHashMap == 0)
	{
		isCreateHashMap = 1;
		OSI_CreateMutex(&hHashMapLock);
		msgQueueMap = hashmap_new();
	}	
}

static int CreateMsgQueue(MsgQueueHeader* pResult,TYPE_NGOS_TID tid)
{

	//TYPE_NGOS_PID pid = OSI_GetPID();
	//printf("create\n");
	unsigned char* pBuffer =  (unsigned char*)malloc(MSG_QUEUE_BUFFER_SIZE+1024);

	uint32_t* pIntWrite = (uint32_t*) pBuffer;
	pIntWrite[0] = (MSG_QUEUE_BUFFER_SIZE -  sizeof(uint32_t) *4) / sizeof(MsgNode);
	pIntWrite[1] = 0; //ReadPos
	pIntWrite[2] = 0; //WritePos
	pIntWrite[3] = sizeof(uint32_t) *4;//Data区起始位置


	OSI_CreateSemaphore(&(pResult->hSem),NULL,MSG_QUEUE_MAX_VALUE);
	OSI_CreateMutex(&(pResult->hWriteLock));
	pResult->pData = pBuffer;
	pResult->OwnerTID = tid;

	return 0;
}


static int GetMsgQueueHeaderFromThread(TYPE_NGOS_TID tid,MsgQueueHeader** ppResult)
{
	OSI_LockMutex(hHashMapLock);
	//printf("*** enter\n");
	void* pHeader = NULL;
	if(hashmap_get(msgQueueMap,tid,&pHeader) == MAP_OK)
	{
		//return 0;
		*ppResult = pHeader;
	}
	else
	{
		MsgQueueHeader* newHeader = (MsgQueueHeader*) malloc(sizeof(MsgQueueHeader));
		CreateMsgQueue(newHeader,tid);
		hashmap_put(msgQueueMap,tid,newHeader);
		*ppResult = newHeader;
	}
	OSI_UnlockMutex(hHashMapLock);
	//printf("*** out\n");
	return 0;
	//尝试打开share memory,sem,mutex
	//
}

static int DestroyMsgQueue()
{
	return 0;
}

static int MsgQueuePushBack(unsigned char* pData,TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData)
{
	uint32_t* pHeaderRead = (uint32_t*) pData;
	MsgNode* pNodeStart = (pData + pHeaderRead[3]);
	MsgNode* pRead = pNodeStart + pHeaderRead[1];
	MsgNode* pWrite = pNodeStart + pHeaderRead[2];

	if(pRead > pWrite)
	{
		//pwrite rollback
		if(pWrite + 1 != pRead)
		{
			pWrite->FromThread = OSI_GetThreadID();
			pWrite->hReciver = hReciver;
			pWrite->msg = msg;
			pWrite->msgData = msgData;
			pWrite->param1 = param1;
			pWrite->param2 = param2;
			pWrite++;
		}
		else
		{
			printf("%d's msgqueue is full\n",OSI_GetThreadID());
			return -1;
		}
	}
	else
	{
		
		pWrite->FromThread = OSI_GetThreadID();
		pWrite->hReciver = hReciver;
		pWrite->msg = msg;
		pWrite->msgData = msgData;
		pWrite->param1 = param1;
		pWrite->param2 = param2;
		
		if(pWrite + 1 - pNodeStart > pHeaderRead[0] - 1)
		{
			pWrite = pNodeStart;
		}
		else
		{
			pWrite++;
		}
	}

	pHeaderRead[2] = pWrite-pNodeStart;
	//printf("write ok,write pos = %d\n",pHeaderRead[2]);
	return 0;
}

static int MsgQueuePopFront(unsigned char* pData,RecivedMsg* pResult)
{
	uint32_t* pHeaderRead = (uint32_t*) pData;
	MsgNode* pNodeStart = (pData + pHeaderRead[3]);
	MsgNode* pRead = pNodeStart + pHeaderRead[1];
	MsgNode* pWrite = pNodeStart + pHeaderRead[2];

	if(pRead > pWrite)
	{
		pResult->TargetReciver = pRead->hReciver;
		pResult->MsgData = pRead->msgData;
		pResult->Msg = pRead->msg;
		pResult->Param1 = pRead->param1;
		pResult->Param2 = pRead->param2;

		if(pRead + 1 - pNodeStart > pHeaderRead[0] - 1)
		{
			pRead = pNodeStart;
		}
		else
		{
			pRead++;
		}
	}
	else
	{
		if(pRead == pWrite)
		{
			printf("%d's msgqueue is empty\n",OSI_GetThreadID());
			return -1;
		}

		pResult->TargetReciver = pRead->hReciver;
		pResult->MsgData = pRead->msgData;
		pResult->Msg = pRead->msg;
		pResult->Param1 = pRead->param1;
		pResult->Param2 = pRead->param2;

		pRead++;
	}

	pHeaderRead[1] = pRead-pNodeStart;
	//printf("read ok,read pos = %d\n",pHeaderRead[1]);
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
	return pObject;
}

int OSI_DestroyMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver)
{
	MsgReciverObject* pObj = (MsgReciverObject*) hReciver;
	if(pObj)
	{
		if(pObj->UserContext)
		{
			if(pObj->UserContext->fnReleaseUserData)
			{
				pObj->UserContext->fnReleaseUserData(pObj->UserContext->UserData);
			}
		}
		//InvalidMsgReciverHandle(hReciver);
		free(pObj);
	}

	return 0;
}

static TYPE_NGOS_TID GetThreadIDFromMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver)
{
	MsgReciverObject* pObj = (MsgReciverObject*) hReciver;
	return pObj->OwnerTID;
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
	MsgQueueHeader* pheader;
	GetMsgQueueHeaderFromThread(tid,&pheader);
	if(pheader->pData == NULL)
	{
		return -1;
	}

	OSI_LockMutex(pheader->hWriteLock);
	//printf("post\n");
	if(MsgQueuePushBack(pheader->pData,hReciver,msg,param1,param2,msgData) != 0)
	{
		OSI_UnlockMutex(pheader->hWriteLock);
		return -1;
	}
	OSI_UnlockMutex(pheader->hWriteLock);

	OSI_ActiveSemaphore(&(pheader->hSem));
	
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
		ThreadMsgRunLoop* pResult = NULL;
		//ThreadMsgRunLoop* pResult = OSI_GetTLSValue(tid,1984);
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
	//为了clone,暂不实行
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
	
	//先在这里销毁了
	DestroyMsgQueue();
	return 0;
}

int OSI_AddMsgFilter(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop,pfnMsgFilter fnMsgFilter,UserDataContext* udc)
{
	//暂不实行
	return 0;
}

int OSI_RemoveMsgFilter(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop,int cookie)
{
	//暂不实行
	return 0;
}

//不推荐直接使用的函数，应该使用系统内置的MsgQueueLoop和Filter
int OSI_GetMsg(RecivedMsg* pResult)
{
	int result = -1;
	TYPE_NGOS_TID ctid = OSI_GetThreadID();
	MsgQueueHeader* pheader;
	//todo
	GetMsgQueueHeaderFromThread(ctid,&pheader);
	if(pheader->pData)
	{
		//printf("get msg\n");
		if(OSI_WaitSemaphore(&(pheader->hSem)) == 0)
		{
			result = 0;
			//printf("get a msg\n");
			OSI_LockMutex(pheader->hWriteLock);
			MsgQueuePopFront(pheader->pData,pResult)==0;
			OSI_UnlockMutex(pheader->hWriteLock);
		}
	}

	return result;
}

int OSI_PeekMsg(RecivedMsg* pResult)
{
	//暂不实现
	return 0;
}

int OSI_DispatchMsg(RecivedMsg* pMsg)
{
	if(pMsg->TargetReciver)
	{
		MsgReciverObject* pObj = pMsg->TargetReciver;
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

int OSI_ReleaseMsgData(TYPE_NGOS_MSG_DATA hMsgData)
{
	return 0;
}