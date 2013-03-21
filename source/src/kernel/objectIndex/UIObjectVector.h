//
// UIObjectVector.h
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_UIOBJECT_VECTOR_H_
#define _NGOS_UIOBJECT_VECTOR_H_

typedef void* UIObjectVector;

int UIObjectVectorPushBack(UIObjectVector* pVector,NGOS_ROOT_OBJTREE_HANDLE hUIObject);
int UIObjectVectorRemove(UIObjectVector* pVector,NGOS_UIOBJECT_HANDLE hUIObject);
int UIObjectVectorClear(UIObjectVector* pVector);
uint32_t UIObjectVectorGetCount(UIObjectVector* pVector);
NGOS_UIOBJECT_HANDLE UIObjectVectorGet(UIObjectVector* pVector,uint32_t index);

#endif //_NGOS_UIOBJECT_VECTOR_H_
