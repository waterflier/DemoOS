#include "RenderEngine.h"
#include "RenderScript.h"
#include "ResourceManager.h"
#include "RenderDevice.h"
#include <stdio.h>


int main(int argc, char * const argv[]) 
{
	NGRE_RESULT lResult = NGREInit();
	
	NGRM_AddResource("bkg.test", "/system/bin/demoresource/bkg.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("icon1.test","/system/bin/demoresource/icon1.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("icon2.test","/system/bin/demoresource/icon2.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("icon3.test","/system/bin/demoresource/icon3.png", NGRM_ResType_Bitmap);


	//FILE* pScript = fopen("../demoresource/script.lua", "r");
	/*fseek(pScript, 0, SEEK_END);
	int nSize = ftell(pScript);
	fseek(pScript, 0, SEEK_SET);
	char* testScript = (char*)malloc(nSize + 1);
	fread(testScript, nSize, 1, pScript);
	testScript[nSize] = '\0';*/

	

	NGRE_SCRIPT_HANDLE hScript = NGRECreateScript();
	
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon1.test\",nil,nil,{10,10,nil,nil})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon2.test\",nil,nil,{110,10,nil,nil})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon3.test\",nil,nil,{220,10,nil,nil})");
	
	lResult = NGRERunScript(hScript);

	NGREDevice* pDevice = NULL;
	lResult = NGREOpenDevice(&pDevice);

	/*LPNGREBitmap pRenderResult;
	lResult = NGRM_GetResource(NGRM_ResType_Bitmap, "bmp.test", &pRenderResult);

	lResult = NGREPrintBimtapToFile(pRenderResult, "/root/win_work/DemoOS/source/src/render/resource/renderresult.png");*/
	
	/*LPNGREBitmap pRenderResult;
	lResult = NGREGetBitmapFromDevice(pDevice, &pRenderResult);
	lResult = NGREPrintBimtapToFile(pRenderResult, "/root/win_work/DemoOS/source/src/render/resource/renderresult.png");*/
	while(1)
	{
		NGREFlushDevice(pDevice);
		sleep(1);
	}

	NGREUninit();

}