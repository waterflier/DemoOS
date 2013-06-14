#include <pthread.h>
#ifdef ANDROID
#include <linux/sem.h>
#include <linux/types.h>
#include <linux/ipc.h>
#include <linux/types.h>
#else
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "../../src/os_interface/os_interface.h"
 int fnMsgProc (RecivedMsg* pMsg,TYPE_NGOS_MSG_RECIVER hReciver,UserDataContext* udc)
 {
	 printf("recv msg! %d\n",pMsg->Msg);
 }

int tid1;
TYPE_NGOS_MSG_RECIVER hRecv;
void* threadProc1(void* ud)
{
	printf("this is thread proc1\n");
	hRecv = OSI_CreateMsgReciver(fnMsgProc,NULL);


	OSI_RunMsgQueueLoop();
	return NULL;
}

void* threadProc2(void* ud)
{
	printf("this is thread proc2\n");
	sleep(2);

	int i=0;
	for(i=0;i<10;i++)
	{
		OSI_PostMsg(hRecv,i,1,1,NULL);
	}
	return NULL;
}

int test_thread_and_msgqueue(int argc,char** argv)
{
	OSI_InitMsgQueue();
	
	tid1 = OSI_CreateThread(NULL,threadProc1);
	int tid2 = OSI_CreateThread(NULL,threadProc2);

	OSI_WaitThreadEnd(tid1);
	OSI_WaitThreadEnd(tid2);
	return 0;
}

int test_msgqueue(int argc,char** argv)
{
	int i = 0;
	if(argc == 1)
	{
		printf("tid=%d\n",OSI_GetThreadID());
		TYPE_NGOS_MSG_RECIVER hRecv = OSI_CreateMsgReciver(fnMsgProc,NULL);

		if(fork() == 0)
		{
			sleep(2);
			for(i=0;i<100;++i)
			{
				printf("client post %d msg\n",i);
				OSI_PostMsg(hRecv,i,1,1,NULL);
			}
			
		}
		else
		{
			
			OSI_RunMsgQueueLoop();
		}
	}
}

int test_lock(int argc,char** argv)
{
	TYPE_NGOS_MUTEX hM1;
	TYPE_NGOS_MUTEX hM2;
	TYPE_NGOS_MUTEX hM3;

	OSI_CreateMutex(&hM1);
	OSI_CreateMutex(&hM2);
	OSI_CreateMutex(&hM3);

	printf("%d,%d,%d\n",hM1,hM2,hM3);
	return 0;
}

int test_sm(int argc,char** argv)
{
	TYPE_NGOS_SHAREMEMORY hSM;
	unsigned char* pData = NULL;
	int i = 0;
	if(argc == 1)
	{
		if(OSI_CreateSM("osiinterface.sm10",4*1024,&hSM) != 0)
		{
			printf("create sm error,No.=%d\n",errno);
			return -1;
		}
		
		
		if(OSI_LockSMBuffer(hSM,&pData) != 0)
		{
			printf("server lock sm error,No.=%d\n",errno);
			return -1;
		}

		for(i=0;i<4*1024;++i)
		{
			pData[i] = 'a' + i % 32;
		}
		pData[1024] = 0;
		OSI_UnlockSMBuffer(hSM,pData);
		sleep(10);
		OSI_LockSMBuffer(hSM,&pData);
		printf("server data is %s\n",pData);
		OSI_UnlockSMBuffer(hSM,pData);
		OSI_ReleaseSM(hSM);
		
	}
	else
	{
		if(OSI_OpenSM("osiinterface.sm10",&hSM) != 0)
		{
			printf("open sm error,No.=%d\n",errno);
			return -1;
		}
		
		if(OSI_LockSMBuffer(hSM,&pData) != 0)
		{
			printf("client lock sm error,No.=%d\n",errno);
			return -1;
		}
		printf("data is %s\n",pData);
		OSI_UnlockSMBuffer(hSM,pData);
		//OSI_ReleaseSM(hSM);
	}
	return 0;
}

int main(int argc,char** argv)
{

	TYPE_NGOS_SEMAPHORE hSem;
	int result;
	int i = 0;
	//return test_lock(argc,argv);
	return test_thread_and_msgqueue(argc,argv);
	//return test_sm(argc,argv);

	if(argc==1)
	{
		printf("start osi test server.\n");
		
		result = OSI_CreateSemaphore(&hSem,"ngos.roottree",5);
		if(result != 0)
		{
			printf("Create sem error!\n");
			return -1;
		}
		
		while(i<5000)
		{
			OSI_ActiveSemaphore(&hSem);
			printf("server active %d.\n",i);
			i++;
			sleep(1);
		}
	}
	else
	{
		printf("start osi test client.\n");
		result = OSI_OpenSemaphore(&hSem,"ngos.roottree");
		if(result != 0)
		{
			printf("Open sem error!\n");
			return -1;
		}
		
		while(1)
		{
			OSI_WaitSemaphore(&hSem);
			printf("%d get it!\n",getpid());
			sleep(5);
		}
	}
	
	return 0;
	
}