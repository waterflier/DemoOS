//
// main.c 
//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>

#include <NGOS_Types.h>
#include <NGOS_API.h>

#include "../../src/kernel/UIObject.h"
#include "../../src/kernel/UIObjTree.h"
#include "../../src/uiobjects/UIObjectTypeLoader.h"


int main(int argc,char** argv)
{
	printf("start test kernel\n");

	NGOS_InitRootUIObjTreeEnv(NULL);
	NGOS_ROOT_OBJTREE_HANDLE hTree = NGOS_CreateRootObjTree(NULL);
	NGOS_UIOBJECT_HANDLE hRoot = NGOS_GetRootObject(hTree);

	NGOS_UIOBJECT_HANDLE hBkg = NGOS_CreateUIObject(NGOS_GetDefaultUIObjectTypeLoader(),"ImageObject","bkg");
	NGOS_ImageObjectSetImageID(hBkg,"image.bkg");
	RECT pos = {.left=0,.top=0,.right=800,.bottom=600};
	NGOS_SetUIObjectRect(hBkg,&pos);

	NGOS_UIOBJECT_HANDLE hIcon = NGOS_CreateUIObject(NGOS_GetDefaultUIObjectTypeLoader(),"ImageObject","icon1");
	NGOS_ImageObjectSetImageID(hBkg,"image.icon1");
	RECT pos2 = {10,10,10+64,10+64};
	NGOS_SetUIObjectRect(hIcon,&pos2);

	NGOS_UIOBJECT_HANDLE hIcon2 = NGOS_CreateUIObject(NGOS_GetDefaultUIObjectTypeLoader(),"ImageObject","icon2");
	NGOS_ImageObjectSetImageID(hBkg,"image.icon2");
	RECT pos3 = {78,10,78+64,10+64};
	NGOS_SetUIObjectRect(hIcon2,&pos3);

	NGOS_UIOBJECT_HANDLE hIcon3 = NGOS_CreateUIObject(NGOS_GetDefaultUIObjectTypeLoader(),"ImageObject","icon3");
	NGOS_ImageObjectSetImageID(hBkg,"image.icon3");
	RECT pos4 = {78+64+4,10,78+64+4+64,64};
	NGOS_SetUIObjectRect(hIcon3,&pos4);

	NGOS_AddChild(hBkg,hIcon);
	NGOS_AddChild(hBkg,hIcon2);
	NGOS_AddChild(hBkg,hIcon3);

	NGOS_AddChild(hRoot,hBkg);

	NGOS_UIOBJECT_HANDLE hIcon4 = NGOS_CreateUIObject(NGOS_GetDefaultUIObjectTypeLoader(),"ImageObject","icon4");
	NGOS_ImageObjectSetImageID(hBkg,"image.icon3");
	RECT pos5 = {78+64+4,300,78+64+4+64,64};
	NGOS_SetUIObjectRect(hIcon4,&pos5);
	NGOS_AddChild(hBkg,hIcon4);

	int i = 0;
	char imgID[64];
	for(i=0;i<10;i++)
	{
		sprintf(imgID,"image.icon%d",i%4);
		NGOS_ImageObjectSetImageID(hIcon,imgID);
		NGOS_UpdateRootObjTree(hRoot);
		sleep(1);
	}

	NGOS_DestoryUIObject(hBkg);
	NGOS_UpdateRootObjTree(hRoot);
	sleep(1);

	NGOS_DestoryRootObjTree(hRoot);
	NGOS_UninitRootUIObjTreeEnv(NULL);

	return 0;
}
