#include <pthread.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <unistd.h>

#include "../../src/os_interface/os_interface.h"


int main(int argc,char** argv)
{
	TYPE_NGOS_SEMAPHORE hSem;
	int result;
	int i = 0;
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
			OSI_ActiveSemaphore(hSem);
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
			OSI_WaitSemaphore(hSem);
			printf("%d get it!\n",getpid());
			sleep(5);
		}
	}
	
	return 0;
	
}