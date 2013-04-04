//
// thread.c
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"
#include <sys/syscall.h>

TYPE_NGOS_TID OSI_GetThreadID()
{
	return syscall(SYS_gettid);
}

int OSI_CreateThread()
{
	return 0;
}

TYPE_NGOS_TID OSI_GetMainThread()
{
	return 0;
}

void OSI_WaitThreadEnd(TYPE_NGOS_TID tid)
{
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