//
// AnimationVector.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./AnimationVector.h"

AnimationtVector* CreateAnimationVector(size_t defaultSize)
{
	if(defaultSize > ANIMATION_VECTOR_MAX_SIZE)
	{
		return NULL;
	}

	AnimationtVector* pResult = (AnimationtVector*) malloc(sizeof(AnimationtVector));
	memset(pResult,0,sizeof(AnimationtVector));

	if(defaultSize > ANIMATION_VECTOR_INNER_SIZE)
	{
		pResult->Ptr = (NGOS_ANIMATION_HANDLE*) malloc(sizeof(NGOS_ANIMATION_HANDLE) * defaultSize);
		pResult->Size = defaultSize;
	}
	else
	{
		pResult->Ptr = pResult->Buffer;
		pResult->Size = ANIMATION_VECTOR_INNER_SIZE;
	}

	return pResult;
}

void InitAnimationVector(AnimationtVector* pVector,size_t defaultSize)
{
	if(defaultSize > ANIMATION_VECTOR_MAX_SIZE)
	{
		defaultSize = ANIMATION_VECTOR_MAX_SIZE;
	}


	memset(pVector,0,sizeof(AnimationtVector));

	if(defaultSize > ANIMATION_VECTOR_INNER_SIZE)
	{
		pVector->Ptr = (NGOS_ANIMATION_HANDLE*) malloc(sizeof(NGOS_ANIMATION_HANDLE) * defaultSize);
		pVector->Size = defaultSize;
	}
	else
	{
		pVector->Ptr = pVector->Buffer;
		pVector->Size = ANIMATION_VECTOR_INNER_SIZE;
	}

	return;
}

void DestroyAnimationVector(AnimationtVector* pVector)
{
	if(pVector)
	{
		if(pVector->Size > ANIMATION_VECTOR_INNER_SIZE)
		{
			free(pVector->Ptr);
		}

		free(pVector);
	}

	return;
}

int AnimationVectorAdd(AnimationtVector* pVector,NGOS_ANIMATION_HANDLE hAnimation)
{
	if(pVector == NULL)
	{
		return -1;
	}

	if(pVector->Length < pVector->Size)
	{
		pVector->Ptr[pVector->Length] = hAnimation;
		pVector->Length++;
	}
	else
	{
		NGOS_ANIMATION_HANDLE* pTemp = pVector->Ptr;
		uint32_t newSize = pVector->Size * 2;
		if(newSize > ANIMATION_VECTOR_MAX_SIZE)
		{
			return -1;
		}

		pVector->Ptr = malloc(newSize*sizeof(NGOS_ANIMATION_HANDLE));
		memcpy(pVector->Ptr,pTemp,sizeof(NGOS_ANIMATION_HANDLE)*pVector->Length);
		pVector->Size = newSize;
		if(pTemp != pVector->Buffer)
		{
			free(pTemp);
		}

		pVector->Ptr[pVector->Length] = hAnimation;
		pVector->Length++;
	}

	return 0;
}

int AnimationVectorRemoveAt(AnimationtVector* pVector,uint32_t index)
{
	if(pVector == NULL)
	{
		return -1;
	}

	uint32_t len = pVector->Length;
	if(index >= len)
	{
		return -1;
	}

	pVector->Ptr[index] = pVector->Ptr[len-1];
	pVector->Ptr[len-1] = NULL;
	if(len > 0)
	{
		pVector->Length--;
	}

	if(pVector->Length < pVector->Size / 2)
	{
		if(pVector->Ptr != pVector->Buffer)
		{
			//TODO: 减小大小?
		}
	}
}

int AnimationVectorRemove(AnimationtVector* pVector,NGOS_ANIMATION_HANDLE hAnimation)
{
	if(pVector == NULL)
	{
		return -1;
	}

	size_t i = 0;
	uint32_t len = pVector->Length;
	for(i=0;i<len;++i)
	{
		if(pVector->Ptr[i] == hAnimation)
		{
			pVector->Ptr[i] = pVector->Ptr[len-1];
			pVector->Ptr[len-1] = NULL;
			if(len > 0)
			{
				pVector->Length--;
			}

			if(pVector->Length < pVector->Size / 2)
			{
				if(pVector->Ptr != pVector->Buffer)
				{
					//TODO: 减小大小?
				}
			}
		}
	}
}

int AnimationVectorClear(AnimationtVector* pVector)
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
	pVector->Size = ANIMATION_VECTOR_INNER_SIZE;
}

uint32_t AnimationVectorGetCount(AnimationtVector* pVector)
{
	return pVector->Length;
}

NGOS_ANIMATION_HANDLE AnimationVectorGet(AnimationtVector* pVector,uint32_t index)
{
	return pVector->Ptr[index];
}