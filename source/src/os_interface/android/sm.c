//
// sm.c android not support System V share memory interface
//
//////////////////////////////////////////////////////////////////////////
#include "./perheader.h"
#include <pthread.h>
#include <stdio.h>

#include "../os_interface.h"
#include "../utility.h"


int OSI_CreateSM(const char* smID,uint32_t size,TYPE_NGOS_SHAREMEMORY* pResult)
{
	return -1;
}

int OSI_OpenSM(const char* smID,TYPE_NGOS_SHAREMEMORY* pResult)
{
	return -1;
}

int OSI_ReleaseSM(TYPE_NGOS_SHAREMEMORY hSM)
{
	return -1;
}

int OSI_LockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char** ppResult)
{
	return -1;
}
int OSI_UnlockSMBuffer(TYPE_NGOS_SHAREMEMORY hSM,unsigned char* pBuffer)
{
	return -1;
}