#include "RenderEngine.h"
#include "RenderScript.h"
#include "RenderDevice.h"

NGREDevice* g_pDevice = NULL;

NGRE_RESULT NGREInit()
{
	NGRE_RESULT lResult = NGREOpenDevice(&g_pDevice);
	
	lResult = NGREScriptInit(NULL);
}

void		NGREUninit()
{
	NGREScriptUninit();

	NGRECloseDevice(g_pDevice);
}

