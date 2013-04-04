//
// semaphore.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"
#include "../utility.h"

union semun{
	int val;
	struct semid_ds *buf;
	unsigned short * ary;
};


int OSI_CreateSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id,int maxValue)
{
	//todo:bug 同id的无法互斥创建，下一次创建依旧能成功
	int semResult = semget(GetStringKey(id),1,0666|IPC_CREAT|IPC_EXCL);
	if(semResult < 0)
	{
		return -1;
	}

	union semun sem_union;
	sem_union.val = 0;

	if(semctl(semResult, 0, SETVAL, sem_union) == -1) 
	{
		return -1;
	}

	*pResult = (TYPE_NGOS_SEMAPHORE)semResult;
	return 0;
}

int OSI_OpenSemaphore(TYPE_NGOS_SEMAPHORE* pResult,const char* id)
{
	int semResult = semget(GetStringKey(id),0,0666);
	if(semResult < 0)
	{
		return -1;
	}

	*pResult = (TYPE_NGOS_SEMAPHORE)semResult;
	return 0;
}

int OSI_WaitSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = -1;
	sem_b.sem_flg = SEM_UNDO;

	if(semop(hSem, &sem_b, 1) == -1)
	{
		
		return -1;
	}

	return 0;
}

int OSI_TryWaitSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	return -1;
}

int OSI_ActiveSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	struct sembuf sem_b;

	sem_b.sem_num = 0;
	sem_b.sem_op = 1;
	sem_b.sem_flg = SEM_UNDO;

	if(semop(hSem, &sem_b, 1) == -1)
	{

		return -1;
	}

	return 0;
}

int OSI_CloseSemaphore(TYPE_NGOS_SEMAPHORE hSem)
{
	union semun sem_union;

	if(semctl(hSem, 0, IPC_RMID, sem_union) == -1)
	{
		return -1;
	}
	return 0;
}