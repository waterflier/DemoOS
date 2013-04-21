#include "RenderEngine.h"
#include "RenderScript.h"
#include "ResourceManager.h"
#include "RenderDevice.h"
#include <stdio.h>


int main(int argc, char * const argv[]) 
{
	NGRE_RESULT lResult = NGREInit();
	
	NGRM_AddResource("bmp.test", "/root/win_work/DemoOS/source/src/render/resource/bkg.png", NGRM_ResType_Bitmap);

	FILE* pScript = fopen("/root/win_work/DemoOS/source/src/render/resource/script.lua", "r");
	fseek(pScript, 0, SEEK_END);
	int nSize = ftell(pScript);
	char* testScript = (char*)malloc(nSize);
	fread(testScript, nSize, 1, pScript);
	

	
	lResult = NGRERunScript(testScript);

	NGREDevice* pDevice = NULL;
	lResult = NGREOpenDevice(&pDevice);
	NGREFlushDevice(pDevice);

	NGREUninit();

}