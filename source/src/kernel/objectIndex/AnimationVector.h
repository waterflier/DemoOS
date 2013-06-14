//
// AnimationVector.h 
//  
// 对顺序不敏感
// 删除只是把删除对象与末尾对象置换，
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_ANIMATION_VECTOR_H_
#define _NGOS_ANIMATION_VECTOR_H_

#define ANIMATION_VECTOR_MAX_SIZE (65535)
#define ANIMATION_VECTOR_INNER_SIZE (16)

typedef struct tagAnimationtVector
{
	uint32_t Size;
	uint32_t Length;
	NGOS_ANIMATION_HANDLE* Ptr;
	NGOS_ANIMATION_HANDLE Buffer[ANIMATION_VECTOR_INNER_SIZE];	 
}AnimationtVector;


AnimationtVector* CreateAnimationVector(size_t defaultSize);
void DestroyAnimationVector(AnimationtVector* pVector);

void InitAnimationVector(AnimationtVector* pVector,size_t defaultSize);
int AnimationVectorClear(AnimationtVector* pVector);

int AnimationVectorAdd(AnimationtVector* pVector,NGOS_ANIMATION_HANDLE hAnimation);
int AnimationVectorRemove(AnimationtVector* pVector,NGOS_ANIMATION_HANDLE hAnimation);
int AnimationVectorRemoveAt(AnimationtVector* pVector,uint32_t index);
uint32_t AnimationVectorGetCount(AnimationtVector* pVector);
NGOS_ANIMATION_HANDLE AnimationVectorGet(AnimationtVector* pVector,uint32_t index);

#endif //_NGOS_ANIMATION_VECTOR_H_
