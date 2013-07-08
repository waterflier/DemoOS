#ifndef _NGOS_RESOURCE_MANAGER_MAP_H_
#define _NGOS_RESOURCE_MANAGER_MAP_H_

typedef void* NGRM_ResourceMap;
typedef struct NGRM_ResourceMapNode{
	NGRM_ResPath path;
	void* instance;
}NGRM_ResourceMapNode;
typedef void* NGRM_ResourceMapEnumtor;

NGRM_ResourceMap NGRM_NewResourceMap();
NGRM_ResourceMapNode* NGRM_FindNodeFromResourceMap(NGRM_ResourceMap resMap, NGRM_ResId resId);
NGRM_ResourceMapNode* NGRM_InsertNodeToResouceMap(NGRM_ResourceMap resMap, NGRM_ResId resId, NGRM_ResourceMapNode* pNode);
void	NGRM_DestroyResourceMap(NGRM_ResourceMap resMap);
NGRM_ResourceMapEnumtor NGRM_BeginEnumResourceMap(NGRM_ResourceMap resMap);
NGRM_ResourceMapNode* NGRM_EnumResourceMapNext(NGRM_ResourceMap resMap, NGRM_ResourceMapEnumtor mapEmumtor);
void NGRM_EndEnumResourceMap(NGRM_ResourceMapEnumtor mapEnumtor);

#endif