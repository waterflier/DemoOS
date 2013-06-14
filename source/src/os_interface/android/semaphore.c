//
// semaphore.c  android not support System V Semaphore interface
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"
#include "../utility.h"
#include <semaphore.h>


int OSI_CreateSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id,int maxValue)
{

	return sem_init(pResult,0,0); 
}

int OSI_OpenSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id)
{
	return -1;
}

int OSI_WaitSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{

	return sem_wait(hSem);
}

int OSI_TryWaitSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	return sem_trywait(hSem);
}

int OSI_ActiveSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	return sem_post(hSem);
}

int OSI_CloseSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	return sem_destroy(hSem);
}