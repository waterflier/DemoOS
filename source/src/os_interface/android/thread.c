//
// thread.c
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"
#include <sys/syscall.h>
#include <unistd.h>

TYPE_NGOS_TID OSI_GetThreadID()
{
	//return syscall(SYS_gettid);
	return gettid();
}

int OSI_CreateThread(void* ud,fnOSIThreadProc proc)
{
	pthread_t resultTID;
	pthread_create(&resultTID,NULL,proc,ud);
	return resultTID;
}

TYPE_NGOS_TID OSI_GetMainThread()
{
	return 0;
}

void OSI_WaitThreadEnd(TYPE_NGOS_TID tid)
{
	void* pResult;
	pthread_join(tid,&pResult);
	return;
}

TYPE_NGOS_PID OSI_GetThreadOwnerPID(TYPE_NGOS_TID tid)
{
	return NULL;
}

void* OSI_GetTLSValue(TYPE_NGOS_TID tid,int key)
{
	return NULL;
}

int   OSI_SetTLSValue(TYPE_NGOS_TID tid,int key,void* tlsValue)
{
	return -1;
}