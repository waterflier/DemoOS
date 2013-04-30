#include "RenderScript.h"
#include "ScriptCoder.h"

NGRE_RESULT NGREScriptInit(LPNGREScriptInitParam pParam)
{
	NGREInitScriptCoderParam ScriptCoder;
	NGRE_RESULT lResult = NGREInitScriptCoder(&ScriptCoder);
	lResult = NGREInitResIdentifier();
	return lResult;
}

NGRE_RESULT	NGRERunScript(NGRE_SCRIPT_HANDLE hScript)
{
	NGRE_SCRIPT_CODE_HANDLE hCode = NULL;
	NGRE_RESULT lResult = NGREDecodeScript(hScript,&hCode);
	if(lResult == NGRE_SUCCESS && hCode != NULL)
	{
		lResult = NGRERunScriptCode(hCode);
		NGREReleaseScriptCode(hCode);
	}
	return lResult;
}

void		NGREScriptUninit()
{
	NGREUninitResIdentifier();
	NGREUninitScriptCoder();
}