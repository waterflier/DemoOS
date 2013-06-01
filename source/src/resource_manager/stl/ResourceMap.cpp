extern "C"
{
#include "ResourceManager.h"
#include "ResourceMap.h"
}
#include <string>

#ifdef ANDROID
#include <hash_map>
typedef std::hash_map<std::string, NGRM_ResourceMapNode*> NGRM_StlResourceMap;
#else
#include <ext/hash_map>
typedef __gnu_cxx::hash_map<std::string, NGRM_ResourceMapNode*> NGRM_StlResourceMap;
namespace __gnu_cxx
{
	template<> 
	struct hash<std::string>
	{
		size_t operator()(const std::string& s) const
		{ return hash<const char*>()( s.c_str() ); }
	};
}
#endif

extern "C" NGRM_ResourceMap NGRM_NewResourceMap()
{
	return new NGRM_StlResourceMap;
}

extern "C" NGRM_ResourceMapNode* NGRM_FindNodeFromResourceMap(NGRM_ResourceMap resMap, NGRM_ResId resId)
{
	NGRM_StlResourceMap* pStlMap = reinterpret_cast<NGRM_StlResourceMap*>(resMap);
	NGRM_StlResourceMap::iterator iter = pStlMap->find(resId);
	if(iter != pStlMap->end())
	{
		return iter->second;
	}
	return NULL;
}

extern "C" NGRM_ResourceMapNode* NGRM_InsertNodeToResouceMap(NGRM_ResourceMap resMap, NGRM_ResId resId, NGRM_ResourceMapNode* pNode)
{
	NGRM_StlResourceMap* pStlMap = reinterpret_cast<NGRM_StlResourceMap*>(resMap);
	(*pStlMap)[resId] = pNode;
	return pNode;
}

extern "C" void	NGRM_DestroyResourceMap(NGRM_ResourceMap resMap)
{
	NGRM_StlResourceMap* pStlMap = reinterpret_cast<NGRM_StlResourceMap*>(resMap);
	delete pStlMap;
}

extern "C" NGRM_ResourceMapEnumtor NGRM_BeginEnumResourceMap(NGRM_ResourceMap resMap)
{
	NGRM_StlResourceMap* pStlMap = reinterpret_cast<NGRM_StlResourceMap*>(resMap);
	NGRM_StlResourceMap::iterator* pIter = new NGRM_StlResourceMap::iterator(pStlMap->begin());
	return pIter;
}

extern "C" NGRM_ResourceMapNode* NGRM_EnumResourceMapNext(NGRM_ResourceMap resMap, NGRM_ResourceMapEnumtor mapEmumtor)
{
	NGRM_StlResourceMap::iterator* pIter =  reinterpret_cast<NGRM_StlResourceMap::iterator*>(mapEmumtor);
	NGRM_StlResourceMap* pStlMap = reinterpret_cast<NGRM_StlResourceMap*>(resMap);
	if(*pIter != pStlMap->end())
	{
		return ((*pIter)++)->second;
	}
	return NULL;
}

extern "C" void NGRM_EndEnumResourceMap(NGRM_ResourceMapEnumtor mapEnumtor)
{
	NGRM_StlResourceMap::iterator* pIter =  reinterpret_cast<NGRM_StlResourceMap::iterator*>(mapEnumtor);
	delete pIter;
}
