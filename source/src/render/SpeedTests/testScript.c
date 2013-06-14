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
	NGRM_AddResource("color.blank","0 0 0 0",NGRM_ResType_Color);

	//FILE* pScript = fopen("../demoresource/script.lua", "r");
	/*fseek(pScript, 0, SEEK_END);
	int nSize = ftell(pScript);
	fseek(pScript, 0, SEEK_SET);
	char* testScript = (char*)malloc(nSize + 1);
	fread(testScript, nSize, 1, pScript);
	testScript[nSize] = '\0';*/

	
	NGREDevice* pDevice = NULL;
	lResult = NGREOpenDevice(&pDevice);

	NGRE_SCRIPT_HANDLE hScript = NGRECreateScript();
	
	NGREAppendScript(hScript, "AddClipRect({0,0,800,480})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, nil)");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);

	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({0,0,800,480})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {0,0,800,480})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);


	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({10,10,74,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {10,10,74,74})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon1.test\",nil,nil,{10,10,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);
	
	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({10,10,74,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {10,10,74,74})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon1.test\",nil,nil,{10,10,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);

	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({10,10,74,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {10,10,74,74})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon2.test\",nil,nil,{10,10,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);

	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({10,10,74,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {10,10,74,74})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon3.test\",nil,nil,{10,10,nil,nil},{[\"clipRect\"]={10,10,74,74}})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);

	
	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({10,10,74,74})");
	NGREAppendScript(hScript, "AddClipRect({110,10,174,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {10,10,74,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {110,10,174,74})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil},{[\"clipRect\"]={110,10,174,74}})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon3.test\",nil,nil,{110,10,nil,nil},{[\"clipRect\"]={110,10,174,74}})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);

	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({110,10,174,74})");
	NGREAppendScript(hScript, "AddClipRect({210,10,274,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {110,10,174,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {210,10,274,74})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil},{[\"clipRect\"]={210,10,274,74}})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon3.test\",nil,nil,{210,10,nil,nil},{[\"clipRect\"]={210,10,274,74}})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);

	NGREClearScript(hScript);
	NGREAppendScript(hScript, "AddClipRect({210,10,274,74})");
	NGREAppendScript(hScript, "AddClipRect({310,10,374,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {210,10,274,74})");
	NGREAppendScript(hScript, "EraseBitmap(nil,nil, {310,10,374,74})");
	NGREAppendScript(hScript, "BlendBitmap(\"bkg.test\",nil,nil,{0,0,nil,nil},{[\"clipRect\"]={310,10,374,74}})");
	NGREAppendScript(hScript, "BlendBitmap(\"icon3.test\",nil,nil,{310,10,nil,nil},{[\"clipRect\"]={310,10,374,74}})");
	lResult = NGRERunScript(hScript, pDevice);
	NGREFlushDevice(pDevice);
	sleep(2);

	NGREUninit();

}