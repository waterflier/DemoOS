//
// os_interface.h 
//
// create:liuzhicong,2013-3-9
//
//////////////////////////////////////////////////////////////////////////
#ifndef _NGOS_OS_INTERFACE_H_
#define _NGOS_OS_INTERFACE_H_

#include <NGOS_Types.h>

//process
TYPE_NGOS_PID OSI_GetPID();
BOOL OSI_IsProcessExist(TYPE_NGOS_PID pid);
//thread
TYPE_NGOS_TID OSI_GetThreadID();
int OSI_CreateThread();
TYPE_NGOS_TID OSI_GetMainThread();
void OSI_WaitThreadEnd(TYPE_NGOS_TID tid);
TYPE_NGOS_PID OSI_GetThreadOwnerPID(TYPE_NGOS_TID tid);
void* OSI_GetTLSValue(TYPE_NGOS_TID tid,int key);
int   OSI_SetTLSValue(TYPE_NGOS_TID tid,int key,void* tlsValue);

//sm
int OSI_CreateSM(const char* smID,uint32_t size,TYPE_NGOS_SHAREMEMORY* pResult);
int OSI_OpenSM(const char* smID,TYPE_NGOS_SHAREMEMORY* pResult);
int OSI_ReleaseSM(TYPE_NGOS_SHAREMEMORY hSM);

int OSI_LockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char** ppResult);
int OSI_UnlockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char* pBuffer);
//int OSI_QuerySMInfo();

//mutex (线程中互斥)
int OSI_CreateMutex(TYPE_NGOS_MUTEX* pResult);
TYPE_NGOS_MUTEX OSI_OpenMutex();
int OSI_LockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_TryLockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_UnlockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_ReleaseMutex(TYPE_NGOS_MUTEX hMutex);

//semaphore (进程间)
int OSI_CreateSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id,int maxValue);
int OSI_OpenSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id);
int OSI_WaitSemaphore(TYPE_NGOS_SEMAPHORE hSem);
int OSI_TryWaitSemaphore(TYPE_NGOS_SEMAPHORE hSem);
int OSI_ActiveSemaphore(TYPE_NGOS_SEMAPHORE hSem);
int OSI_CloseSemaphore(TYPE_NGOS_SEMAPHORE hSem);
//spin_lock

//msg queue
//加入msg frame的概念  一个靠msg loop驱动的thread,一旦进入消息循环（第一次调用GetMsg之前不算）,就可以说是进入了一次消息处理的frame 下一次调用GetMeg会进入下个frame
//消息处理完了要销毁消息，销毁消息会让frame结束，这样就能有足够的信息构建起有层次的消息处理过程
//如何让 windows下的 domodal逻辑更加不易错
//而且能否简单的解决 “在事件里删除自己的问题”
typedef struct tagRecivedMsg
{
	TYPE_NGOS_MSG_RECIVER TargetReciver;
	uint32_t Msg;
	TYPE_NGOS_MSG_PARAM Param1;
	TYPE_NGOS_MSG_PARAM Param2;
	TYPE_NGOS_MSG_DATA	MsgData;
}RecivedMsg;

typedef int (*pfnMsgFilter) (RecivedMsg* pMsg,UserDataContext* udc,int step);
typedef int (*pfnMsgProc) (RecivedMsg* pMsg,TYPE_NGOS_MSG_RECIVER hReciver,UserDataContext* udc);

TYPE_NGOS_MSG_RECIVER OSI_CreateMsgReciver(pfnMsgProc fnMsgProc,UserDataContext* udc);
int OSI_ReleaseMsgReciver(TYPE_NGOS_MSG_RECIVER hReciver);

//msg == 0为退出消息循环消息，唯一的内置消息
TYPE_NGOS_MSG_DATA OSI_CreateMsgData(TYPE_NGOS_MSG_RECIVER hReciver,uint32_t length);
uint8_t* OSI_LockMsgData(TYPE_NGOS_MSG_DATA hMsgData);
uint8_t* OSI_UnlockMsgData(TYPE_NGOS_MSG_DATA hMsgData);
int OSI_ReleaseMsgData(TYPE_NGOS_MSG_DATA hMsgData);

int OSI_PostMsg(TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData);
int OSI_SendMsg(TYPE_NGOS_MSG_RECIVER hReciver,uint32_t msg,TYPE_NGOS_MSG_PARAM param1,TYPE_NGOS_MSG_PARAM param2,TYPE_NGOS_MSG_DATA msgData,int32_t maxWaitTime);


TYPE_NGOS_MSGQUEUE_RUNLOOP OSI_GetMsgQueueLoop(TYPE_NGOS_TID tid);
TYPE_NGOS_MSGQUEUE_RUNLOOP OSI_CloneMsgQueueRunLoop(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop);
int OSI_ReleaseMsgQueueRunLoop(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop);

int OSI_RunMsgQueueLoop();

int OSI_AddMsgFilter(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop,pfnMsgFilter fnMsgFilter,UserDataContext* udc);
int OSI_RemoveMsgFilter(TYPE_NGOS_MSGQUEUE_RUNLOOP hRunLoop,int cookie);

//不推荐直接使用的函数，应该使用系统内置的MsgQueueLoop和Filter
int OSI_GetMsg(RecivedMsg* pResult);
int OSI_PeekMsg(RecivedMsg* pResult);
int OSI_DispatchMsg(RecivedMsg* pMsg);
int OSI_DestroyMsg(RecivedMsg* pMsg);

//timer

//基于MsgQueue的两个易用函数
//void OSI_AsynCall();
//void OSI_DelayCall();


//wait queue 等待队列，类似waitformutliobject,不过尽量不要使用这种特性

#endif //_NGOS_OS_INTERFACE_H_
