//
// sm.c
//
//////////////////////////////////////////////////////////////////////////
#include <pthread.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>

#include "../os_interface.h"
#include "../utility.h"


int OSI_CreateSM(const char* smID,uint32_t size,TYPE_NGOS_SHAREMEMORY* pResult)
{
	//todo:bug ͬid���޷����ⴴ������һ�δ��������ܳɹ�
	int smResult = smget(GetStringKey(id),size,0666|IPC_CREAT);
	if(smResult < 0)
	{
		return -1;
	}

	*pResult = smResult;
	return 0;
}

int OSI_OpenSM(const char* smID,TYPE_NGOS_SHAREMEMORY* pResult)
{
	int smResult = smget(GetStringKey(id),0,0666|IPC_EXCL);
	if(smResult < 0)
	{
		return -1;
	}

	*pResult = smResult;
	return 0;
}

int OSI_ReleaseSM(TYPE_NGOS_SHAREMEMORY hSM)
{

	return 0;
}

int OSI_LockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char** ppResult)
{
	void* pResult = shmat(hSM,0,100);
	if((int)pResult > 0)
	{
		*ppResult= pResult;
		return 0;
	}

	return -1;
}
int OSI_UnlockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char* pBuffer)
{
	int result = smdt(pBuffer);
	return result;
}