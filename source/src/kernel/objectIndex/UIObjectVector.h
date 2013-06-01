//
// UIObjectVector.h
//  
// 用于UIObject保存其孩子,对顺序不敏感
// 删除只是把删除对象与末尾对象置换，
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_UIOBJECT_VECTOR_H_
#define _NGOS_UIOBJECT_VECTOR_H_

#define UIOBJECT_VECTOR_MAX_SIZE (65535)
#define UIOBJECT_VECTOR_INNER_SIZE (16)

typedef struct tagUIObjectVector
{
	uint32_t Size;
	uint32_t Length;
	NGOS_UIOBJECT_HANDLE* Ptr;
	NGOS_UIOBJECT_HANDLE Buffer[UIOBJECT_VECTOR_INNER_SIZE];	 
}UIObjectVector;


UIObjectVector* CreateUIObjectVector(size_t defaultSize);
void DestroyUIObjectVector(UIObjectVector* pVector);

void InitUIObjectVector(UIObjectVector* pVector,size_t defautSize);
int UIObjectVectorClean(UIObjectVector* pVector);

int UIObjectVectorAdd(UIObjectVector* pVector,NGOS_UIOBJECT_HANDLE hUIObject);
int UIObjectVectorRemove(UIObjectVector* pVector,NGOS_UIOBJECT_HANDLE hUIObject);
uint32_t UIObjectVectorGetCount(UIObjectVector* pVector);
NGOS_UIOBJECT_HANDLE UIObjectVectorGet(UIObjectVector* pVector,uint32_t index);

#endif //_NGOS_UIOBJECT_VECTOR_H_
