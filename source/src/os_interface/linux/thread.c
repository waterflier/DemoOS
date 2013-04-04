//
// thread.c
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"

TYPE_NGOS_TID OSI_GetThreadID()
{
	return 0;
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

}