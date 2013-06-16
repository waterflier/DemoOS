#include "RenderScript.h"
#include "ScriptCoder.h"

NGRE_RESULT NGREScriptInit(LPNGREScriptInitParam pParam)
{
	NGREInitScriptCoderParam ScriptCoder;
	NGRE_RESULT lResult = NGREInitScriptCoder(&ScriptCoder);
	lResult = NGREInitResIdentifier();
	return lResult;
}

NGRE_RESULT	NGRERunScript(NGRE_SCRIPT_HANDLE hScript, NGREDevice* pDevice)
{
	NGRE_SCRIPT_CODE_HANDLE hCode = NULL;
	NGRE_RESULT lResult = NGREDecodeScript(hScript,&hCode);
	if(lResult == NGRE_SUCCESS && hCode != NULL)
	{
		lResult = NGRERunScriptCode(hCode, pDevice);
		NGREReleaseScriptCode(hCode);
	}
	return lResult;
}

void		NGREScriptUninit()
{
	NGREUninitResIdentifier();
	NGREUninitScriptCoder();
}


typedef struct NGREScript
{
	unsigned long ulSize;
	char* szCode;
}NGREScript, *LPNGREScript;



NGRE_SCRIPT_HANDLE NGRECreateScript()
{
	return NGRECreateSizedScript(1024);
}

NGRE_RESULT NGREAppendScript(NGRE_SCRIPT_HANDLE hScript, const char* szScriptCode)
{
	if(szScriptCode == NULL)
	{
		return NGRE_SUCCESS;
	}
	unsigned int nCodeSize = strlen(szScriptCode);
	if(nCodeSize == 0)
	{
		return NGRE_SUCCESS;
	}
	LPNGREScript pScript = (LPNGREScript)hScript;
	const char charSplit = NGREScriptSpecChar(NGRE_SCRIPT_SPECCHAR_SPLIT);
	unsigned int nOrgSize = strlen(pScript->szCode);
	unsigned int nAppendedSize = nOrgSize;
	int bNeedSplit = 0;
	if(szScriptCode[nCodeSize - 1] == charSplit)
	{
		nAppendedSize = nAppendedSize + nCodeSize + 1;
	}
	else
	{
		nAppendedSize = nAppendedSize + nCodeSize + 1 + 1;
		bNeedSplit = 1;
	}
	unsigned long ulNewSize = pScript->ulSize;
	while(nAppendedSize > ulNewSize)
	{
		ulNewSize =  ulNewSize << 1;
	}
	if(ulNewSize != pScript->ulSize)
	{
		char* szCode = (char*)malloc(ulNewSize);
		if(szCode == NULL)
		{
			return NGRE_SCRIPT_INVALIDSIZE;
		}
		strcpy(szCode, pScript->szCode);
		pScript->ulSize = ulNewSize;
		free(pScript->szCode);
		pScript->szCode = szCode;
	}
	strcat(pScript->szCode, szScriptCode);
	if(bNeedSplit)
	{
		pScript->szCode[nAppendedSize - 2] = charSplit;
		pScript->szCode[nAppendedSize - 1] = '\0';
	}
	return NGRE_SUCCESS;
}

void		NGREClearScript(NGRE_SCRIPT_HANDLE hScript)
{
	LPNGREScript pScript = (LPNGREScript)hScript;
	if(pScript->ulSize > 0)
	{
		memset(pScript->szCode, 0, pScript->ulSize);
		//pScript->szCode[0] = '\0';
	}
}

void		NGRECloseScript(NGRE_SCRIPT_HANDLE hScript)
{
	LPNGREScript pScript = (LPNGREScript)hScript;
	if(pScript->szCode != NULL)
	{
		free(pScript->szCode);
	}
	free(pScript);
}

const char* NGREGetScriptCode(NGRE_SCRIPT_HANDLE hScript)
{
	LPNGREScript pScript = (LPNGREScript)hScript;
	return pScript->szCode;
}

char*		NGREGetScriptBuffer(NGRE_SCRIPT_HANDLE hScript)
{
	LPNGREScript pScript = (LPNGREScript)hScript;
	return pScript->szCode;
}

NGRE_SCRIPT_HANDLE NGRECreateSizedScript(unsigned int unSize)
{
	LPNGREScript pScript = (LPNGREScript)malloc(sizeof(NGREScript));
	memset(pScript,0,sizeof(NGREScript));
	pScript->ulSize = unSize;
	pScript->szCode = (char*)malloc(unSize);
	memset(pScript->szCode, 0, unSize);
	return pScript;
}

