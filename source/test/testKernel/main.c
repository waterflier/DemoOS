//
// main.c 
//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>
//#include <sys/sem.h>
//#include <sys/types.h>
//#include <sys/ipc.h>
//#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

#include <NGOS_Types.h>
#include <NGOS_API.h>

#include "../../src/kernel/UIObject.h"
#include "../../src/kernel/UIObjTree.h"
#include "../../src/uiobjects/UIObjectTypeLoader.h"
#include "../../src/kernel/animation/BaseKeyFrameAnimation.h"

#include "RenderEngine.h"
#include "RenderScript.h"
#include "ResourceManager.h"
#include "RenderDevice.h"



int main(int argc,char** argv)
{
	printf("start test kernel\n");
	
	
	NGRE_RESULT lResult = NGREInit();
	NGRM_AddResource("image.bkg", "/system/bin/demoresource/bkg.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("image.icon0","/system/bin/demoresource/icon1.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("image.icon1","/system/bin/demoresource/icon2.png", NGRM_ResType_Bitmap);
	NGRM_AddResource("image.icon2","/system/bin/demoresource/icon3.png", NGRM_ResType_Bitmap);

	NGREDevice* pDevice = NULL;
	lResult = NGREOpenDevice(&pDevice);
	int nDeviceWidth, nDeviceHeight;
	lResult = NGREGetDeviceSize(pDevice, &nDeviceWidth, &nDeviceHeight);
	
	NGOS_InitRootUIObjTreeEnv(NULL);
	NGOS_ROOT_OBJTREE_HANDLE hTree = NGOS_CreateRootObjTree(NULL);
	NGOS_SetRootUIObjTreeViewPort(hTree, nDeviceWidth, nDeviceHeight);
	NGOS_UIOBJECT_HANDLE hRoot = NGOS_GetRootObject(hTree);

	NGOS_UIOBJECT_HANDLE hBkg = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","bkg");
	NGOS_ImageObjectSetImageID(hBkg,"image.bkg");
	RECT pos = {.left=0,.top=0,.right=800,.bottom=300};
	NGOS_SetUIObjectRect(hBkg,&pos);

	NGOS_UIOBJECT_HANDLE hIcon = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","icon0");
	NGOS_ImageObjectSetImageID(hIcon,"image.icon0");
	RECT pos2 = {10,10,10+64+50,10+64+50};
	//RECT pos2 = {76,76,141,141};
	NGOS_SetUIObjectRect(hIcon,&pos2);

	NGOS_UIOBJECT_HANDLE hIcon2 = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","icon1");
	NGOS_ImageObjectSetImageID(hIcon2,"image.icon1");
	RECT pos3 = {78,10,78+64,10+64};
	NGOS_SetUIObjectRect(hIcon2,&pos3);

	NGOS_UIOBJECT_HANDLE hIcon3 = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","icon2");
	NGOS_ImageObjectSetImageID(hIcon3,"image.icon2");
	RECT pos4 = {78+64+4,10,78+64+4+64,64};
	NGOS_SetUIObjectRect(hIcon3,&pos4);

	NGOS_AddChild(hBkg,hIcon);
	NGOS_AddChild(hBkg,hIcon2);
	NGOS_AddChild(hBkg,hIcon3);

	NGOS_AddChild(hRoot,hBkg);

	NGOS_UIOBJECT_HANDLE hIcon4 = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject","icon3");
	NGOS_ImageObjectSetImageID(hIcon4,"image.icon2");
	RECT pos5 = {78+64+4,300,78+64+4+64,64};
	NGOS_SetUIObjectRect(hIcon4,&pos5);
	NGOS_AddChild(hBkg,hIcon4);
	
	NGOS_ANIMATION_HANDLE hPosAni = NGOS_CreateAnimation(NGOS_GetDefaultTypeLoader(),"PosChangeAnimation");
	BaseAnimation* pPosAni = HandleMapDecodeAnimation(hPosAni,NULL);
	AnimationSetTotalTime(pPosAni,3000);
	AnimationBindUIObject(pPosAni,hIcon);
	PosChangeAnimationSetPos(pPosAni,10,10,500,500);
	AnimationManagerAddAnimation(GetAnimationManager(),hPosAni);
	//AnimationStart(pPosAni);

	
	
	int i = 0;
	char imgID[64];
	
	float fRoate[3] = {0,31,64};

	NGRE_SCRIPT_HANDLE hRenderScript = NGRECreateScript();
	for(i=0;i<10000;i++)
	{
		NGREClearScript(hRenderScript);
		//sprintf(imgID,"image.icon%d",i%3);
		//NGOS_ImageObjectSetImageID(hIcon,imgID);
		//++pos2.left; 
		//++pos2.top; 
		//++pos2.right; 
		//++pos2.bottom; 
		//NGOS_SetUIObjectRect(hIcon,&pos2);
		NGOS_SetUIObjectRotate(hIcon2, fRoate);
		NGOS_UpdateRootObjTree(hTree, hRenderScript);
		printf("%s\r\n",NGREGetScriptCode(hRenderScript));
		NGRERunScript(hRenderScript, pDevice);
		NGREFlushDevice(pDevice);
		fRoate[0] += 10;
		//usleep(1000);
		sleep(1);
	}
	
	

	NGOS_DestoryUIObject(hBkg);
	NGOS_UpdateRootObjTree(hRoot, hRenderScript);
	sleep(1);

	NGRECloseScript(hRenderScript);
	NGOS_DestoryRootObjTree(hRoot);
	NGOS_UninitRootUIObjTreeEnv(NULL);

	
	NGREUninit();
	return 0;
}
