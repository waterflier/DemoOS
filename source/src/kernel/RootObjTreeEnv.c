//
// RootObjTreeEnv.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./RootObjTreeEnv.h"

static void* DefaultAlloc(uint8_t type,void* ud,void* ptr,size_t newSize,size_t oldSize)
{
	if(newSize == 0)
	{
		free(ptr);
		return NULL;
	}

	return realloc(ptr,newSize);
}


void SetDefaultRootTreeEnv(NGOS_RootTreeEnv* pEnv)
{
	if(pEnv)
	{
		pEnv->fnAlloc = DefaultAlloc;
		pEnv->AllocUD = NULL;
	}
}