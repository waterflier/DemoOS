#include "ScriptCoder.h"
#include <lua.h>
#include <lauxlib.h>
#include "RenderOperation.h"


NGREDevice* NGREGetScriptDeviceL(lua_State* pLuaState)
{
	lua_getglobal(pLuaState, "ScriptDevice");
	NGREDevice* pDevice = lua_touserdata(pLuaState, -1);
	lua_pop(pLuaState, 1);
	return pDevice;
}



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
	lua_pop(pLuaState,4);
	return 1;
}

int NGREOpFMatrix33L(lua_State* pLuaState, int nIndex, NGREOpFMatrix33 refMatrix)
{
	int ix;
	for(ix = 0; ix < 8; ++ ix)
	{
		lua_rawgeti(pLuaState, nIndex, ix + 1);
		refMatrix[ix] = lua_tonumber(pLuaState, -1);
	}
	lua_pop(pLuaState, 8);
	return 1;
}

int NGREOpParamL(lua_State* pLuaState, int nIndex, LPNGREOpParam pParam)
{
	if(!lua_isnoneornil(pLuaState, nIndex))
	{
		lua_pushstring(pLuaState, "matrix");
		lua_gettable(pLuaState, nIndex);
		if(!lua_isnoneornil(pLuaState, nIndex))
		{
			((NGREOpParamHeader*)pParam)->flag |= OpParamMatrix;
			NGREOpFMatrix33L(pLuaState, lua_gettop(pLuaState), pParam->matrix);
		}
		lua_pop(pLuaState, 1);
		lua_pushstring(pLuaState, "clipRect");
		lua_gettable(pLuaState, nIndex);
		if(!lua_isnoneornil(pLuaState, nIndex))
		{
			((NGREOpParamHeader*)pParam)->flag |= OpParamClipRect;
			NGREOpIRectL(pLuaState, lua_gettop(pLuaState),&(pParam->clipRect));
		}
		lua_pop(pLuaState, 1);
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

	NGREOpParam opParam;
	opParam.header.cbSize = sizeof(NGREOpParam);
	opParam.header.flag = 0;
	NGREOpParamL(pLuaState,5,&opParam);

	NGRE_RESULT lResult = NGREOpBlendBitmapR(pBmpSrc, pRectSrc, pBmpDest,  &rectDest, &opParam);

	lua_pushnumber(pLuaState, lResult);

	return 1;
}

//NGRE_RESULT NGREOpFillRectR(NGREBitmapR pBmpDest, CLPNGREOpIRect pRectDest, LPNGREOpColor pColor, CLPNGREOpParam pParam)
//忽略dest时候用device上的bitmap
//忽略destRect时使用位图大小
int NGREOpFillRectL(lua_State* pLuaState)
{
	NGREBitmapR pBmpDest;
	pBmpDest.idResource = lua_tostring(pLuaState, 1);


	NGREOpColorR pColor;
	pColor.idResource = lua_tostring(pLuaState,2);

	LPNGREOpIRect pRectDest = NULL;
	NGREOpIRect rectDest;
	if(!lua_isnil(pLuaState,3))
	{
		NGREOpIRectL(pLuaState, 3, &rectDest);
		pRectDest = &rectDest;
	}
	NGRE_RESULT lResult = NGREOpFillRectR(pBmpDest, pRectDest, pColor,  NULL);

	lua_pushnumber(pLuaState, lResult);

	return 1;
}
//同上
int NGREOpEraseBitmapL(lua_State* pLuaState)
{
	NGREBitmapR pBmpDest;
	pBmpDest.idResource = lua_tostring(pLuaState, 1);


	NGREOpColorR pColor;
	pColor.idResource = lua_tostring(pLuaState,2);

	LPNGREOpIRect pRectDest = NULL;
	NGREOpIRect rectDest;
	if(!lua_isnil(pLuaState,3))
	{
		NGREOpIRectL(pLuaState, 3, &rectDest);
		pRectDest = &rectDest;
	}
	NGRE_RESULT lResult = NGREOpEraseBitmapR(pBmpDest, pRectDest, pColor,  NULL);

	lua_pushnumber(pLuaState, lResult);

	return 1;
}


int NGREOpAddClipRectL(lua_State* pLuaState)
{
	NGREOpIRect clipRect;
	NGREOpIRectL(pLuaState, 1, &clipRect);
	NGREDevice* pDevice = NGREGetScriptDeviceL(pLuaState);
	NGREAddDeviceClipRect(pDevice,  &clipRect);
	return 1;
}
//const luaL_Reg g_lScriptFunctions[] = {
//	{"BlendBitmap", NGREOpBlendBitmapL},
//	{NULL, NULL},
//};


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
	lua_register (g_pLuaScriptEnv->pRunStack, "FillRect", NGREOpFillRectL);
	lua_register (g_pLuaScriptEnv->pRunStack, "EraseBitmap", NGREOpEraseBitmapL);
	lua_register (g_pLuaScriptEnv->pRunStack, "AddClipRect", NGREOpAddClipRectL);

	
	
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


NGRE_RESULT	NGRERunScriptCode(NGRE_SCRIPT_CODE_HANDLE hCode, NGREDevice* pDevice)
{
	const char* szLuaCode = (const char*)hCode;
	int nStackTop = lua_gettop(g_pLuaScriptEnv->pRunStack);
	lua_pushlightuserdata(g_pLuaScriptEnv->pRunStack, pDevice);
	lua_setglobal(g_pLuaScriptEnv->pRunStack, "ScriptDevice");

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