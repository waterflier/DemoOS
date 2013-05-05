#include "ScriptCoder.h"
#include <lua.h>
#include <lauxlib.h>
#include "RenderOperation.h"


int NGREOpIRectL(lua_State* pLuaState, int nIndex, NGREOpIRect* pRect)
{
	lua_rawgeti(pLuaState, nIndex, 1);
	pRect->left = lua_tointeger(pLuaState, -1);	
	lua_rawgeti(pLuaState, nIndex, 2);
	pRect->top = lua_tointeger(pLuaState, -1);	
	lua_rawgeti(pLuaState, nIndex, 3);
	if(lua_isnil(pLuaState,-1))
	{
		pRect->right = NGREOpIInv;
	}
	else
	{
		pRect->right = lua_tointeger(pLuaState, -1);
	}
		
	lua_rawgeti(pLuaState, nIndex, 4);
	if(lua_isnil(pLuaState,-1))
	{
		pRect->bottom = NGREOpIInv;
	}
	else
	{
		pRect->bottom = lua_tointeger(pLuaState, -1);
	}
	return 1;
}
//NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
//忽略dest时候用device上的bitmap
//NGRE_RESULT NGREOpBlendBitmapR(NGREBitmapR pBmpSrc, CLPNGREOpIRect pRectSrc, CLPNGREOpIRect pRectDest, CLPNGREOpParam pParam)
//忽略pRectSrc时使用位图原本的大小
//忽略pRectDest的right和bottom时，使用原图的大小
int NGREOpBlendBitmapL(lua_State* pLuaState)
{
	NGREBitmapR pBmpSrc;
	pBmpSrc.idResource = lua_tostring(pLuaState, 1);

	LPNGREOpIRect pRectSrc = NULL;
	NGREOpIRect rectSrc;
	if(!lua_isnil(pLuaState,2))
	{
		NGREOpIRectL(pLuaState, 2, &rectSrc);
		pRectSrc = &rectSrc;
	}
	
	NGREBitmapR pBmpDest;
	pBmpDest.idResource = lua_tostring(pLuaState, 3);
	NGREOpIRect rectDest;
	NGREOpIRectL(pLuaState, 4, &rectDest);

	NGRE_RESULT lResult = NGREOpBlendBitmapR(pBmpSrc, pRectSrc, pBmpDest,  &rectDest, NULL);

	lua_pushnumber(pLuaState, lResult);

	return 1;
}

const luaL_Reg g_lScriptFunctions[] = {
	{"BlendBitmap", NGREOpBlendBitmapL},
	{NULL, NULL},
};


typedef struct NGRELuaScriptEnv{
	lua_State* pRunStack;
}NGRELuaScriptEnv;
NGRELuaScriptEnv* g_pLuaScriptEnv;

NGRE_RESULT	NGREInitScriptCoder(LPNGREInitScriptCoderParam param)
{
	g_pLuaScriptEnv = (NGRELuaScriptEnv*)malloc(sizeof(NGRELuaScriptEnv));

	g_pLuaScriptEnv->pRunStack = luaL_newstate();
	/*register lua funcs for render object*/
	luaL_openlibs(g_pLuaScriptEnv->pRunStack);
	//luaL_register(g_pLuaScriptEnv->pRunStack, NULL, g_lScriptFunctions);
	lua_register (g_pLuaScriptEnv->pRunStack,"BlendBitmap",NGREOpBlendBitmapL);
	return NGRE_SUCCESS;

}

void		NGREUninitScriptCoder()
{
	lua_close(g_pLuaScriptEnv->pRunStack);
	free(g_pLuaScriptEnv);
}



NGRE_RESULT	NGREDecodeScript(NGRE_SCRIPT_HANDLE hScript, NGRE_SCRIPT_CODE_HANDLE*phCode)
{
	*phCode = NGREGetScriptCode(hScript);
	return NGRE_SUCCESS;
}


NGRE_RESULT	NGRERunScriptCode(NGRE_SCRIPT_CODE_HANDLE hCode)
{
	const char* szLuaCode = (const char*)hCode;
	int nStackTop = lua_gettop(g_pLuaScriptEnv->pRunStack);
	int lResult = luaL_loadstring(g_pLuaScriptEnv->pRunStack, szLuaCode);
	if(lResult == 0)
	{
		lResult = lua_pcall(g_pLuaScriptEnv->pRunStack,0,0,0);
		if(lResult != 0)
		{
			const char* szErrInfo = lua_tostring(g_pLuaScriptEnv->pRunStack, -1);
			if(szErrInfo)
			{
				printf("%s\r\n", szErrInfo);
			}
		}
	}
	else
	{
		const char* szErrInfo = lua_tostring(g_pLuaScriptEnv->pRunStack,  -1);
		if(szErrInfo)
		{
			printf("%s\r\n", szErrInfo);
		}
	}
	lua_settop(g_pLuaScriptEnv->pRunStack, nStackTop);
	if(lResult != 0)
	{
		return NGRE_SCRIPT_RUN_FAIL;
	}

	return NGRE_SUCCESS;
}

void		NGREReleaseScriptCode(NGRE_SCRIPT_CODE_HANDLE hCode)
{
	return ;
}

const char  NGREScriptSpecChar(int nSpecChar)
{
	if(nSpecChar == NGRE_SCRIPT_SPECCHAR_SPLIT)
	{
		return '\r';
	}
	return '\t';
}