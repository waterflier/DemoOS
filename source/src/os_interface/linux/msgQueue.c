//
// msgQueue.c 
//
// TODO:目前MsgQueue的实现太繁琐了，系统调用太多，考虑通过扩展Linux内核的方法实现
//
//////////////////////////////////////////////////////////////////////////
#include "./perheader.h"
#include "../os_interface.h"
#include <memory.h>
#include <errno.h>

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
	TYPE_NGOS_SHAREMEMORY hData;
	TYPE_NGOS_SEMAPHORE   hSem;
	TYPE_NGOS_SEMAPHORE   hSemLock;
	TYPE_NGOS_MUTEX       hWriteLock;
}MsgQueueHeader;

typedef uint32_t NGOS_HANDLE;

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




typedef struct tagHandleNode
{
	void* pObj;
	unsigned char Flag;//最高位表示是否分配,0-5bits为分配标识
}HandleNode;


static BOOL s_isCreateHandleMap = FALSE;
static HandleNode* s_pHandleBaseAddress = NULL;
static HandleNode* s_pHandleNextInsertAddress = NULL;

static int CreateHandleMap()
{
	s_pHandleBaseAddress =(HandleNode*) malloc(sizeof(HandleNode)*(0x40000));
	memset(s_pHandleBaseAddress,0,sizeof(HandleNode)*(0x40000));
	s_pHandleNextInsertAddress = s_pHandleBaseAddress;
	return 0;
}

static int DestoryHandleMap()
{
	if(s_pHandleBaseAddress)
	{
		free(s_pHandleBaseAddress);
	}
	s_pHandleBaseAddress = NULL;
	return 0;
}

static void* DecodeHandle(NGOS_HANDLE handle)
{
	unsigned long index = (unsigned long)handle & 0x3ffff;
	HandleNode* pNode = s_pHandleBaseAddress + index;
	if(((pNode->Flag)&0x80) && ( ((pNode->Flag)&0x3f) == (((unsigned long)handle>>18)&0x3f)))
	{
		return pNode->pObj;
	}
	return NULL;
}


static int DeleteObj(NGOS_HANDLE handle)
{
	unsigned long index = (unsigned long)handle & 0x3ffff;
	HandleNode* pNode = s_pHandleBaseAddress + index;
	if((pNode->Flag) & 0x80)
	{
		//printf("%x,%x\n",(pNode->Flag)&0x3f,(handle>>18)&0x3f);
		if(((pNode->Flag)&0x3f) == (((unsigned long)handle>>18)&0x3f))
		{
			pNode->pObj = NULL;
			pNode->Flag = pNode->Flag&0x7f;//把最高位清0
			return 0;
		}
	}

	//assert(0);
	return -1;

}

static NGOS_HANDLE InsertObj(void* pObj,unsigned char objType)
{

	//设置s_pHandleNextInsertAddress
	unsigned long step = 0;
	while(s_pHandleNextInsertAddress->Flag & 0x80)
	{
		s_pHandleNextInsertAddress++;
		if(s_pHandleNextInsertAddress >= s_pHandleBaseAddress + 0x40000)
		{
			s_pHandleNextInsertAddress = s_pHandleBaseAddress;
		}
		step ++;
		if(step >= 0x40000)
		{
			//assert(0);
			return NULL;
		}
	}

	unsigned char newFlag = (s_pHandleNextInsertAddress->Flag & 0x3f) + 1;
	newFlag = newFlag & 0x3F;
	NGOS_HANDLE hResult = 0;
	hResult =(NGOS_HANDLE) ((objType<<24) | (newFlag<<18) | (unsigned long)(s_pHandleNextInsertAddress - s_pHandleBaseAddress)); 
	s_pHandleNextInsertAddress->Flag = newFlag | 0x80;
	s_pHandleNextInsertAddress->pObj = pObj;
	return hResult;
}

static MsgReciverObject* DecodeMsgReciverObject(TYPE_NGOS_MSG_RECIVER hReciver)
{
	if(!s_isCreateHandleMap)
	{
		return NULL;
	}
	TYPE_NGOS_TID tid = hReciver >> 32;
	NGOS_HANDLE hObj = hReciver & 0xffffffff;
	MsgReciverObject* pResult = DecodeHandle(hObj);
	if(pResult)
	{
		if(pResult->OwnerTID == tid)
		{
			return pResult;
		}
	}

	return NULL;
}

static TYPE_NGOS_MSG_RECIVER EncodeMsgReciverObject(MsgReciverObject* pObject)
{
	if(!s_isCreateHandleMap)
	{
		CreateHandleMap();
		s_isCreateHandleMap = TRUE;
	}
	
	
	NGOS_HANDLE hObj = InsertObj((void*)pObject,1);
	TYPE_NGOS_TID tid = pObject->OwnerTID;
	pObject->LocalHandle = hObj;

	TYPE_NGOS_MSG_RECIVER hResult = tid;
	hResult = hResult << 32;
	hResult = hResult | hObj;

	return hResult;
}

static int InvalidMsgReciverHandle(TYPE_NGOS_MSG_RECIVER hReciver)
{
	if(!s_isCreateHandleMap)
	{
		return NULL;
	}
	TYPE_NGOS_TID tid = hReciver >> 32;
	NGOS_HANDLE hObj = hReciver & 0xffffffff;
	MsgReciverObject* pResult = DecodeHandle(hObj);
	if(pResult)
	{
		if(pResult->OwnerTID == tid)
		{
			DeleteObj(hObj);
			return 0;
		}
	}

	return -1;
}

static TYPE_NGOS_TID GetThreadIDFromMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver)
{
	TYPE_NGOS_TID tid = hReciver >> 32;
	return tid;
}


static int GetMsgQueueHeaderFromThread(TYPE_NGOS_TID tid,MsgQueueHeader* pResult)
{
	char buffer[32];
	sprintf(buffer,"msgqueue:%d",tid);
	//通过tid拼出一个key
	
	if(OSI_OpenSM(buffer,&(pResult->hData)) != 0)
	{
		return -1;
	}
	
	OSI_OpenSemaphore(&(pResult->hSem),buffer);
	sprintf(buffer,"msgqueue:lock:%d",tid);
	OSI_OpenSemaphore(&(pResult->hSemLock),buffer);
	return 0;
	//尝试打开share memory,sem,mutex
	//
}

static int CreateMsgQueue(MsgQueueHeader* pResult)
{
	TYPE_NGOS_TID tid = OSI_GetThreadID();
	//TYPE_NGOS_PID pid = OSI_GetPID();
	char buffer[32];
	sprintf(buffer,"msgqueue:%d",tid);
	TYPE_NGOS_SHAREMEMORY hSM;
	if(OSI_CreateSM(buffer,MSG_QUEUE_BUFFER_SIZE,&hSM) != 0)
	{
		//创建失败
		printf("create msg queue sm error,tid=%d,no=%d\n",tid,errno);
		return -1;
	}
	unsigned char* pBuffer = NULL;
	OSI_LockSMBuffer(hSM,&pBuffer);
	if(pBuffer == NULL)
	{
		printf("lock msg queue sm error,tid=%d,no=%d\n",tid,errno);
		return -1;
	}

	uint32_t* pIntWrite = (uint32_t*) pBuffer;
	pIntWrite[0] = (MSG_QUEUE_BUFFER_SIZE -  sizeof(uint32_t) *4) / sizeof(MsgNode);
	pIntWrite[1] = 0; //ReadPos
	pIntWrite[2] = 0; //WritePos
	pIntWrite[3] = sizeof(uint32_t) *4;//Data区起始位置
	OSI_UnlockSMBuffer(hSM,pBuffer);

	OSI_CreateSemaphore(&(pResult->hSem),buffer,MSG_QUEUE_MAX_VALUE);
	sprintf(buffer,"msgqueue:lock:%d",tid);
	OSI_CreateSemaphore(&(pResult->hSemLock),buffer,1);
	OSI_ActiveSemaphore(pResult->hSemLock);
	pResult->hData = hSM;

	return 0;
}

static int DestroyMsgQueue()
{
	MsgQueueHeader header;
	GetMsgQueueHeaderFromThread(OSI_GetThreadID(),&header);

	OSI_ReleaseSM(header.hData);
	OSI_CloseSemaphore(header.hSem);
	OSI_CloseSemaphore(header.hSemLock);
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
		
		if(pWrite - pNodeStart > pHeaderRead[0] - 1)
		{
			pWrite = pNodeStart;
		}
		else
		{
			pWrite++;
		}
	}

	pHeaderRead[2] = pWrite-pNodeStart;
	printf("write ok,write pos = %d\n",pHeaderRead[2]);
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

		if(pRead - pNodeStart > pHeaderRead[0] - 1)
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

int OSI_DestroyMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver)
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

	OSI_WaitSemaphore(header.hSemLock);
	unsigned char* pData;
	OSI_LockSMBuffer(header.hData,&pData);
	if(MsgQueuePushBack(pData,hReciver,msg,param1,param2,msgData) != 0)
	{
		OSI_UnlockSMBuffer(header.hData,pData);
		OSI_ActiveSemaphore(header.hSemLock);
		return -1;
	}
	OSI_UnlockSMBuffer(header.hData,pData);
	OSI_ActiveSemaphore(header.hSemLock);

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
	int result = 0;
	TYPE_NGOS_TID ctid = OSI_GetThreadID();
	MsgQueueHeader header;
	GetMsgQueueHeaderFromThread(ctid,&header);
	if(header.hData == NULL)
	{
		//首次使用,创建
		CreateMsgQueue(&header);
	}

	if(header.hData)
	{
		if(OSI_WaitSemaphore(header.hSem) == 0)
		{
			OSI_WaitSemaphore(header.hSemLock);
			unsigned char* pData;
			OSI_LockSMBuffer(header.hData,&pData);
			MsgQueuePopFront(pData,pResult)==0;
			OSI_UnlockSMBuffer(header.hData,&pData);
			OSI_ActiveSemaphore(header.hSemLock);
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

int OSI_ReleaseMsgData(TYPE_NGOS_MSG_DATA hMsgData)
{
	return 0;
}