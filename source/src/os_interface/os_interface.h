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

//mutex (�߳��л���)
int OSI_CreateMutex(TYPE_NGOS_MUTEX* pResult);
TYPE_NGOS_MUTEX OSI_OpenMutex();
int OSI_LockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_TryLockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_UnlockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_ReleaseMutex(TYPE_NGOS_MUTEX hMutex);

//semaphore (���̼�)
int OSI_CreateSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id,int maxValue);
int OSI_OpenSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id);
int OSI_WaitSemaphore(TYPE_NGOS_SEMAPHORE hSem);
int OSI_TryWaitSemaphore(TYPE_NGOS_SEMAPHORE hSem);
int OSI_ActiveSemaphore(TYPE_NGOS_SEMAPHORE hSem);
int OSI_CloseSemaphore(TYPE_NGOS_SEMAPHORE hSem);
//spin_lock

//msg queue
//����msg frame�ĸ���  һ����msg loop������thread,һ��������Ϣѭ������һ�ε���GetMsg֮ǰ���㣩,�Ϳ���˵�ǽ�����һ����Ϣ�����frame ��һ�ε���GetMeg������¸�frame
//��Ϣ��������Ҫ������Ϣ��������Ϣ����frame�����������������㹻����Ϣ�������в�ε���Ϣ�������
//����� windows�µ� domodal�߼����Ӳ��״�
//�����ܷ�򵥵Ľ�� �����¼���ɾ���Լ������⡱
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

//msg == 0Ϊ�˳���Ϣѭ����Ϣ��Ψһ��������Ϣ
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

//���Ƽ�ֱ��ʹ�õĺ�����Ӧ��ʹ��ϵͳ���õ�MsgQueueLoop��Filter
int OSI_GetMsg(RecivedMsg* pResult);
int OSI_PeekMsg(RecivedMsg* pResult);
int OSI_DispatchMsg(RecivedMsg* pMsg);
int OSI_DestroyMsg(RecivedMsg* pMsg);

//timer

//����MsgQueue���������ú���
//void OSI_AsynCall();
//void OSI_DelayCall();


//wait queue �ȴ����У�����waitformutliobject,����������Ҫʹ����������

#endif //_NGOS_OS_INTERFACE_H_
