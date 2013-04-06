//
// UIObjectVector.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./UIObjectVector.h"

UIObjectVector* CreateUIObjectVector(size_t defaultSize)
{
	if(defaultSize > UIOBJECT_VECTOR_MAX_SIZE)
	{
		return NULL;
	}

	UIObjectVector* pResult = (UIObjectVector*) malloc(sizeof(UIObjectVector));
	memset(pResult,0,sizeof(UIObjectVector));

	if(defaultSize > UIOBJECT_VECTOR_INNER_SIZE)
	{
		pResult->Ptr = (NGOS_ROOT_OBJTREE_HANDLE*) malloc(sizeof(NGOS_UIOBJECT_HANDLE) * defaultSize);
		pResult->Size = defaultSize;
	}
	else
	{
		pResult->Ptr = pResult->Buffer;
		pResult->Size = UIOBJECT_VECTOR_INNER_SIZE;
	}

	return pResult;
}

void InitUIObjectVector(UIObjectVector* pVector,size_t defaultSize)
{
	if(defaultSize > UIOBJECT_VECTOR_MAX_SIZE)
	{
		defaultSize = UIOBJECT_VECTOR_MAX_SIZE;
	}


	memset(pVector,0,sizeof(UIObjectVector));

	if(defaultSize > UIOBJECT_VECTOR_INNER_SIZE)
	{
		pVector->Ptr = (NGOS_ROOT_OBJTREE_HANDLE*) malloc(sizeof(NGOS_UIOBJECT_HANDLE) * defaultSize);
		pVector->Size = defaultSize;
	}
	else
	{
		pVector->Ptr = pVector->Buffer;
		pVector->Size = UIOBJECT_VECTOR_INNER_SIZE;
	}

	return;
}

void DestroyUIObjectVector(UIObjectVector* pVector)
{
	if(pVector)
	{
		if(pVector->Size > UIOBJECT_VECTOR_INNER_SIZE)
		{
			free(pVector->Ptr);
		}

		free(pVector);
	}

	return;
}

int UIObjectVectorAdd(UIObjectVector* pVector,NGOS_UIOBJECT_HANDLE hUIObject)
{
	if(pVector == NULL)
	{
		return -1;
	}

	if(pVector->Length < pVector->Size)
	{
		pVector->Ptr[pVector->Length] = hUIObject;
		pVector->Length++;
	}
	else
	{
		NGOS_UIOBJECT_HANDLE* pTemp = pVector->Ptr;
		uint32_t newSize = pVector->Size * 2;
		if(newSize > UIOBJECT_VECTOR_MAX_SIZE)
		{
			return -1;
		}

		pVector->Ptr = malloc(newSize*sizeof(NGOS_UIOBJECT_HANDLE));
		memcpy(pVector->Ptr,pTemp,sizeof(NGOS_UIOBJECT_HANDLE)*pVector->Length);
		pVector->Size = newSize;
		if(pTemp != pVector->Buffer)
		{
			free(pTemp);
		}
		
		pVector->Ptr[pVector->Length] = hUIObject;
		pVector->Length++;
	}

	return 0;
}

int UIObjectVectorRemove(UIObjectVector* pVector,NGOS_UIOBJECT_HANDLE hUIObject)
{
	if(pVector == NULL)
	{
		return -1;
	}

	size_t i = 0;
	uint32_t len = pVector->Length;
	for(i=0;i<len;++i)
	{
		if(pVector->Ptr[i] == hUIObject)
		{
			pVector->Ptr[i] = pVector->Ptr[len];
			pVector->Ptr[len] = NULL;
			if(len > 0)
			{
				pVector->Length--;
			}

			if(pVector->Length < pVector->Size / 2)
			{
				if(pVector->Ptr != pVector->Buffer)
				{
					//TODO: ¼õÐ¡´óÐ¡?
				}
			}
		}
	}
}

int UIObjectVectorClear(UIObjectVector* pVector)
{
	if(pVector == NULL)
	{
		return -1;
	}

	if(pVector->Ptr != pVector->Buffer)
	{
		free(pVector->Ptr);
		pVector->Ptr = pVector->Buffer;
	}

	pVector->Length = 0;
	pVector->Size = UIOBJECT_VECTOR_INNER_SIZE;
}

uint32_t UIObjectVectorGetCount(UIObjectVector* pVector)
{
	return pVector->Length;
}

NGOS_UIOBJECT_HANDLE UIObjectVectorGet(UIObjectVector* pVector,uint32_t index)
{
	return pVector->Ptr[index];
}