#include <stdlib.h>
#include "ResourceManager.h"
#include "ResourceMap.h"

typedef void* (*NGRM_LoadResourceFromPath)(NGRM_ResPath);

typedef struct NGRM_TypeNode
{
	NGRM_ResourceMap resourceMap;
	NGRM_LoadResourceFromPath fnLoader;
}NGRM_TypeNode;

typedef struct NGRM_Manager
{
	NGRM_TypeNode resourceMaps[NGRM_ResType_Count];
}NGRM_Manager;

NGRM_Manager* g_ResourceManager = NULL;

NGRM_RESULT NGRM_Init(NGRM_InitParam* pParam)
{
	assert(g_ResourceManager == NULL);
	g_ResourceManager = (NGRM_Manager*)malloc(sizeof(NGRM_Manager));
	memset(g_ResourceManager, 0, sizeof(NGRM_Manager));
	g_ResourceManager->resourceMaps[NGRM_ResType_Bitmap].resourceMap = NGRM_NewResourceMap();
}

void		NGRM_Uninit()
{
	NGRM_ResourceMap resMap = g_ResourceManager->resourceMaps[NGRM_ResType_Bitmap].resourceMap;
	NGRM_ResourceMapEnumtor resMapEnum = NGRM_BeginEnumResourceMap(resMap);
	NGRM_ResourceMapNode* pResNode = NULL;
	while(NULL != (pResNode = NGRM_EnumResourceMapNext(resMap, resMapEnum)))
	{
		////È±ÊÍ·Å×ÊÔ´
		free(pResNode->path);
		free(pResNode);
	}
	NGRM_EndEnumResourceMap(resMapEnum);

	NGRM_DestroyResourceMap(resMap);
	
	free(g_ResourceManager);
}

NGRM_RESULT NGRM_AddResource(NGRM_ResId idRes, NGRM_ResPath pathRes, NGRM_ResType typeRes)
{
	assert(typeRes < NGRM_ResType_Count);
	if(typeRes >= NGRM_ResType_Count)
	{
		return NGRM_INVALID_TYPE;
	}

	NGRM_TypeNode* pTypeNode = (g_ResourceManager->resourceMaps + typeRes);
	NGRM_ResourceMap resMap = pTypeNode->resourceMap;
	NGRM_ResourceMapNode* pMapNode = (NGRM_ResourceMapNode*)malloc(sizeof(NGRM_ResourceMapNode));
	pMapNode->path = (NGRM_ResId)malloc(strlen(pathRes) + 1);
	strcpy(pMapNode->path, pathRes);
	pMapNode->instance = NULL;
	NGRM_InsertNodeToResouceMap(resMap, idRes, pMapNode);
	return NGRM_SUCCESS;
}

NGRM_RESULT NGRM_GetResurce(NGRM_ResType typeRes, NGRM_ResId idRes, void** ppRes)
{
	assert(typeRes < NGRM_ResType_Count);
	if(typeRes >= NGRM_ResType_Count)
	{
		return NGRM_INVALID_TYPE;
	}
	NGRM_TypeNode* pTypeNode = (g_ResourceManager->resourceMaps + typeRes);
	NGRM_ResourceMap resMap = pTypeNode->resourceMap;
	NGRM_ResourceMapNode* pMapNode = NGRM_FindNodeFromResourceMap(resMap, idRes);
	if(pMapNode == NULL)
	{
		return NGRM_INAVLID_ID;
	}
	if(pMapNode->instance == NULL)
	{
		pMapNode->instance = pTypeNode->fnLoader(pMapNode->path);
	}
	if(pMapNode->instance == NULL)
	{
		return NGRM_INVALID_PATH;
	}
	*ppRes = pMapNode->instance;
	return NGRM_SUCCESS;
}

