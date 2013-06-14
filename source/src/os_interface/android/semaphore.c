//
// semaphore.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"
#include "../utility.h"

union osi_semun{
	int val;
	struct semid_ds *buf;
	unsigned short * ary;
};


int OSI_CreateSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id,int maxValue)
{

	return 0;
}

int OSI_OpenSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id)
{
	return 0;
}

int OSI_WaitSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{


	return 0;
}

int OSI_TryWaitSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	return -1;
}

int OSI_ActiveSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{

	return 0;
}

int OSI_CloseSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	return 0;
}