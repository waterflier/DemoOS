//
// sm.c
//
//////////////////////////////////////////////////////////////////////////
#include <pthread.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>

#include "../os_interface.h"
#include "../utility.h"


int OSI_CreateSM(const char* smID,uint32_t size,TYPE_NGOS_SHAREMEMORY* pResult)
{
	//todo:bug 同id的无法互斥创建，下一次创建依旧能成功
	int intid = GetStringKey(smID);
	printf("sm create:%s = %X\n",smID,intid);
	int smResult = shmget(intid,size,0640|IPC_CREAT|IPC_EXCL);
	if(smResult < 0)
	{
		return -1;
	}

	*pResult = smResult;
	return 0;
}

int OSI_OpenSM(const char* smID,TYPE_NGOS_SHAREMEMORY* pResult)
{
	int intid = GetStringKey(smID);
	printf("sm open:%s = %X\n",smID,intid);
	int smResult = shmget(intid,0,0640);
	if(smResult < 0)
	{
		return -1;
	}

	*pResult = smResult;
	return 0;
}

int OSI_ReleaseSM(TYPE_NGOS_SHAREMEMORY hSM)
{
	return shmctl(hSM,IPC_RMID,NULL);
}

int OSI_LockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char** ppResult)
{
	void* pResult = shmat(hSM,0,100);
	if((int)pResult != -1)
	{
		*ppResult= pResult;
		return 0;
	}

	return -1;
}
int OSI_UnlockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char* pBuffer)
{
	int result = shmdt(pBuffer);
	return result;
}