//
// HandleMap.c 
//
//////////////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./HandleMap.h"
#include "./UIObject.h"
#include "./UIObjTree.h"

typedef struct tagHandleNode
{
	void* pObj;
	unsigned char Flag;//最高位表示是否分配,0-5bits为分配标识

}HandleNode;

static BOOL s_isCreateHandleMap = FALSE;
static HandleNode* s_pHandleBaseAddress = NULL;
static HandleNode* s_pHandleNextInsertAddress = NULL;

#define NGOS_OBJ_TYPE_UIOBJECT 1
#define NGOS_OBJ_TYPE_ROOTTREE 2
#define NGOS_OBJ_TYPE_ANIMATION 3

static void* DecodeHandle(NGOS_HANDLE handle)
{
	unsigned long index = (unsigned long)handle & 0x3ffff;
	HandleNode* pNode = s_pHandleBaseAddress + index;
	if(((pNode->Flag)&0x80) && ( ((pNode->Flag)&0x3f) == (((unsigned long)handle>>18)&0x3f)))
	{
		return pNode->pObj;
	}
	return NULL;
}


static int DeleteObj(NGOS_HANDLE handle)
{
	unsigned long index = (unsigned long)handle & 0x3ffff;
	HandleNode* pNode = s_pHandleBaseAddress + index;
	if((pNode->Flag) & 0x80)
	{
		//printf("%x,%x\n",(pNode->Flag)&0x3f,(handle>>18)&0x3f);
		if(((pNode->Flag)&0x3f) == (((unsigned long)handle>>18)&0x3f))
		{
			pNode->pObj = NULL;
			pNode->Flag = pNode->Flag&0x7f;//把最高位清0
			return 0;
		}
	}

	//assert(0);
	return -1;

}

static NGOS_HANDLE InsertObj(void* pObj,unsigned char objType)
{

	//设置s_pHandleNextInsertAddress
	unsigned long step = 0;
	while(s_pHandleNextInsertAddress->Flag & 0x80)
	{
		s_pHandleNextInsertAddress++;
		if(s_pHandleNextInsertAddress >= s_pHandleBaseAddress + 0x40000)
		{
			s_pHandleNextInsertAddress = s_pHandleBaseAddress;
		}
		step ++;
		if(step >= 0x40000)
		{
			//assert(0);
			return NULL;
		}
	}

	unsigned char newFlag = (s_pHandleNextInsertAddress->Flag & 0x3f) + 1;
	newFlag = newFlag & 0x3F;
	NGOS_HANDLE hResult = 0;
	hResult =(NGOS_HANDLE) ((objType<<24) | (newFlag<<18) | (unsigned long)(s_pHandleNextInsertAddress - s_pHandleBaseAddress)); 
	s_pHandleNextInsertAddress->Flag = newFlag | 0x80;
	s_pHandleNextInsertAddress->pObj = pObj;

	//void* pObj2 = DecodeHandle(hResult);
	return hResult;
}

void InitObjectHandleMap()
{
	s_pHandleBaseAddress =(HandleNode*) malloc(sizeof(HandleNode)*(0x40000));
	memset(s_pHandleBaseAddress,0,sizeof(HandleNode)*(0x40000));
	s_pHandleNextInsertAddress = s_pHandleBaseAddress;
	return 0;
}

void DestoryObjectHandleMap()
{
	if(s_pHandleBaseAddress)
	{
		free(s_pHandleBaseAddress);
	}
	s_pHandleBaseAddress = NULL;
	return 0;
}

void* HandleMapDecodeUIObject(NGOS_UIOBJECT_HANDLE hObject,TYPE_NGOS_PID* pOwner)
{
	if(hObject != NULL)
	{	
		unsigned char objType = hObject>>24;
		if(objType != NGOS_OBJ_TYPE_UIOBJECT)
		{
			return NULL;
		}
		return DecodeHandle(hObject);
	}

	return NULL;
}

void* HandleMapDecodeRootTree(NGOS_ROOT_OBJTREE_HANDLE hTree,TYPE_NGOS_PID* pOwner)
{
	if(hTree != NULL)
	{	
		unsigned char objType = hTree>>24;
		if(objType != NGOS_OBJ_TYPE_ROOTTREE)
		{
			return NULL;
		}
		return DecodeHandle(hTree);
	}

	return NULL;
}

NGOS_UIOBJECT_HANDLE HandleMapEncodeUIObject(void* pUIObject)
{
	if(pUIObject)
	{
		UIObject* pObj = (UIObject*) pUIObject;
		if(pObj->hSelf != NULL)
		{
			return NULL;
		}

		NGOS_UIOBJECT_HANDLE hResult = InsertObj(pUIObject,NGOS_OBJ_TYPE_UIOBJECT);
		pObj->hSelf = hResult;
		return hResult;
	}	

	return NULL;
}

NGOS_ROOT_OBJTREE_HANDLE HandleMapEncodeRootTree(void* pTree)
{
	if(pTree)
	{
		RootUIObjTree* pRooTree = (RootUIObjTree*) pTree;
		if(pRooTree->hTree != NULL)
		{
			return NULL;
		}

		NGOS_ROOT_OBJTREE_HANDLE hResult = InsertObj(pRooTree,NGOS_OBJ_TYPE_ROOTTREE);
		pRooTree->hTree = hResult;
		return hResult;
	}	

	return NULL;
}