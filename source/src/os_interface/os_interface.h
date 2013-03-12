//
// os_interface.h 
//
// create:liuzhicong,2013-3-9
//
//////////////////////////////////////////////////////////////////////////
#ifndef _NGOS_OS_INTERFACE_H_
#define _NGOS_OS_INTERFACE_H_

typedef void* TYPE_NGOS_SHAREMEMORY;
typedef void* TYPE_NGOS_MUTEX;
typedef void* TYPE_NGOS_MSGQUEUE_RUNLOOP;
//process
TYPE_NGOS_PID OSI_GetPID();
BOOL OSI_IsProcessExist(TYPE_NGOS_PID pid);

//thread

//sm
TYPE_NGOS_SHAREMEMORY OSI_OpenSM();
TYPE_NGOS_SHAREMEMORY OSI_CreateSM();
int OSI_ReleaseSM(TYPE_NGOS_SHAREMEMORY hSM);
unsigned char* OSI_LockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM);
int OSI_QuerySMInfo();

//mutex
TYPE_NGOS_MUTEX OSI_CreateMutex();
TYPE_NGOS_MUTEX OSI_OpenMutex();
int OSI_LockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_TryLockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_UnlockMutex(TYPE_NGOS_MUTEX hMutex);
int OSI_ReleaseMutex(hMutex);

//semphone

//spin_lock

//msg queue
void OSI_RunMsgQueueRunLoop();
void OSI_RegisterMsgFilter();
void OSI_PostMsg();
void OSI_SendMsg();
void OSI_EndMsgQueueRunLoop();


#endif //_NGOS_OS_INTERFACE_H_
