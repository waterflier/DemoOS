//
// sm.c
//
//////////////////////////////////////////////////////////////////////////
#include "./perheader.h"
#include <pthread.h>
#include <stdio.h>

#include "../os_interface.h"
#include "../utility.h"

///android不支持sm这些  ， 先去掉

int OSI_CreateSM(const char* smID,uint32_t size,TYPE_NGOS_SHAREMEMORY* pResult)
{
	//todo:bug 同id的无法互斥创建，下一次创建依旧能成功
#ifndef ANDROID
	int intid = GetStringKey(smID);
	printf("sm create:%s = %X\n",smID,intid);
	int smResult = shmget(intid,size,0640|IPC_CREAT|IPC_EXCL);
	if(smResult < 0)
	{
		return -1;
	}

	*pResult = smResult;
#endif
	return 0;
}

int OSI_OpenSM(const char* smID,TYPE_NGOS_SHAREMEMORY* pResult)
{
#ifndef ANDROID
	int intid = GetStringKey(smID);
	printf("sm open:%s = %X\n",smID,intid);
	int smResult = shmget(intid,0,0640);
	if(smResult < 0)
	{
		return -1;
	}

	*pResult = smResult;
#endif
	return 0;
}

int OSI_ReleaseSM(TYPE_NGOS_SHAREMEMORY hSM)
{
#ifndef ANDROID
	return shmctl(hSM,IPC_RMID,NULL);
#endif
	return 0;
}

int OSI_LockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char** ppResult)
{
#ifndef ANDROID
	void* pResult = shmat(hSM,0,100);
	if((int)pResult != -1)
	{
		*ppResult= pResult;
		return 0;
	}

	return -1;
#endif
	return 0;
}
int OSI_UnlockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char* pBuffer)
{
#ifndef ANDROID
	int result = shmdt(pBuffer);
	return result;
#endif
	return 0;
}