//
// lock.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"

int OSI_CreateMutex(TYPE_NGOS_MUTEX* pResult)
{
	pthread_mutex_t result;
	int return_value = pthread_mutex_init(&result,NULL);
	if(return_value)
	{
		*pResult = (TYPE_NGOS_MUTEX)result;
		return 0;
	}
	else
	{
		return -1;
	}
}

int OSI_LockMutex(TYPE_NGOS_MUTEX hMutex)
{
	return pthread_mutex_lock(&hMutex);
}

int OSI_TryLockMutex(TYPE_NGOS_MUTEX hMutex)
{
	return pthread_mutex_trylock(&hMutex);
}

int OSI_UnlockMutex(TYPE_NGOS_MUTEX hMutex)
{
	return pthread_mutex_unlock(&hMutex);
}

int OSI_ReleaseMutex(TYPE_NGOS_MUTEX hMutex)
{
	return pthread_mutex_destroy(&hMutex);
}