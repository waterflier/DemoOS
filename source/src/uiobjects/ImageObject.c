//
// ImageObject.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "./ImageObject.h"
#include "../kernel/UIObject.h"
#include "../kernel/UIObjTree.h"
#include "./UIObjectTypeLoader.h"
#include <RenderScript.h>

#include <stdio.h>
#include <string.h>

static char s_imageRSBuffer[256];
static NGRE_SCRIPT_HANDLE ImageObjectGetRenderScript(void* pSelf,RECT* pViewRect);
static UIObject* CreateImageObject(UIObjectTypeInfo* self,const char* className,const char* id);

static const char* GetImageObjectClassName(UIObjectTypeInfo* self)
{
	return "ImageObject";
}


UIObjectTypeInfo s_ImageObjecTypeInfo =
{
	.fnGetClassName = GetImageObjectClassName,
	.fnCraeteUIObject = CreateImageObject,
	.fnGetOwnerTypeLoader = NGOS_GetDefaultTypeLoader,
	.fnAddRef = NULL,
	.fnRelease = NULL
};

UIObjectProvier s_ImageObjectProvier = 
{
	.fnGetRenderScript = ImageObjectGetRenderScript
};

UIObjectTypeInfo* GetImageObjectTypeInfo()
{
	return &s_ImageObjecTypeInfo;
}



static UIObject* CreateImageObject(UIObjectTypeInfo* self,const char* className,const char* id)
{
	UIObject* pResult = MallocUIObject(NULL,sizeof(ImageObject));
	ImageObject* pImg = (ImageObject*)UIObjectGetUserDataStart(pResult);
	pResult->Imp = &s_ImageObjectProvier;
	pResult->pTypeInfo = GetImageObjectTypeInfo();

	UIObjectInit(pResult);
	if(id)
	{
		pResult->strID = malloc(strlen(id)+1);
		strcpy(pResult->strID,id);
	}

	pImg->strImageResID = NULL;
	pImg->DrawModeAntiAlias = 0;
	pImg->DrawModeHalign = 0;
	pImg->DrawModeIsStretch = 0;
	pImg->DrawModeHalign = 0;

	return pResult;
}

NGOS_API(void) NGOS_ImageObjectSetImageID(NGOS_UIOBJECT_HANDLE hImageObject,const char* imageID)
{
	UIObject* pObj = HandleMapDecodeUIObject(hImageObject,NULL);
	if(pObj)
	{
		ImageObject* pImg = (ImageObject*)UIObjectGetUserDataStart(pObj);
		if(pImg->strImageResID)
		{
			pImg->strImageResID = realloc(pImg->strImageResID,strlen(imageID) + 1);
		}
		else
		{
			pImg->strImageResID = malloc(strlen(imageID) + 1);
		}

		strcpy(pImg->strImageResID,imageID);
		if(pObj->hOwnerTree)
		{
			RootUIObjTree* objTree = HandleMapDecodeRootTree(pObj->hOwnerTree,NULL);
			if(objTree)
				RootUIObjTreePushDirtyRect(objTree,&(pObj->ObjAbsRect));
		}
		
	}

	return;
}

NGOS_API(const char*) NGOS_ImageObjectGetImageID(NGOS_UIOBJECT_HANDLE hImageObject)
{
	UIObject* pObj = HandleMapDecodeUIObject(hImageObject,NULL);
	if(pObj)
	{
		ImageObject* pImg = (ImageObject*)UIObjectGetUserDataStart(pObj);
		return pImg->strImageResID;
	}

	return NULL;
}

static NGRE_SCRIPT_HANDLE ImageObjectGetRenderScript(void* pSelf,RECT* pViewRect)
{
	NGRE_SCRIPT_HANDLE hResult;
	UIObject* pObj = (UIObject*) pSelf;
	ImageObject* pImg= (ImageObject*) UIObjectGetUserDataStart(pSelf); //todo: fix it
	//printf("%s:",pObj->strID);
	NGRE_SCRIPT_HANDLE hRenderScript = NGRECreateSizedScript(500);
	char szCodeRotate[100] = {0};
	if(pObj->pTransInfo)
	{
		((UIObjectEffectHeader*)(pObj->pTransInfo))->fnGetRenderScript(pObj->pTransInfo,  szCodeRotate, 100);
	}

	///脚本实现为在参数中带带effect之类， 就不用分支判断了,现在只有trans
	if(pImg->DrawModeIsStretch)
	{
		//AppendRenderScript(hResult,"StretchDrawBitmap(id,left,top,right,bottom,antiAlias)");
		/*printf("StretchDrawBitmap(%s,%d,%d,%d,%d,%d)\n",pImg->strImageResID,pObj->ObjAbsRect.left,pObj->ObjAbsRect.top,pObj->ObjAbsRect.right,pObj->ObjAbsRect.bottom,
			pImg->DrawModeAntiAlias);*/
		
		sprintf(NGREGetScriptBuffer(hRenderScript), "BlendBitmap(\"%s\",{0,0,%d,%d},nil,{%d,%d,%d,%d},{[\"clipRect\"]={%d,%d,%d,%d},%s})",
			pImg->strImageResID,pObj->ObjAbsRect.right - pObj->ObjAbsRect.left ,pObj->ObjAbsRect.bottom - pObj->ObjAbsRect.top ,
			pObj->ObjAbsRect.left,pObj->ObjAbsRect.top,pObj->ObjAbsRect.right,pObj->ObjAbsRect.bottom,
			pViewRect->left,pViewRect->top,pViewRect->right,pViewRect->bottom, szCodeRotate);
	}
	else
	{
		//获得对齐方式 计算坐标
		
		int left,top;
		int VAlign = pImg->DrawModeValign;
		int HAlign = pImg->DrawModeHalign;

		left = pObj->ObjAbsRect.left;
		top = pObj->ObjAbsRect.top;
		if(VAlign == 1)
		{
			//top = (width-content.width)/2
		}
		else if(VAlign == 2)
		{
			//top = width-cotent.width
		}

		//AppendRenderScript(hResult,"DrawBitmap(%s,%d,%d)");
		//printf("DrawBitmap(%s,%d,%d)\n",pImg->strImageResID,left,top);
		sprintf(NGREGetScriptBuffer(hRenderScript), "BlendBitmap(\"%s\",nil,nil,{%d,%d,nil,nil},{[\"clipRect\"]={%d,%d,%d,%d},%s})",
			pImg->strImageResID,pObj->ObjAbsRect.left,pObj->ObjAbsRect.top,pViewRect->left,pViewRect->top,pViewRect->right,pViewRect->bottom,szCodeRotate );
	}

	return hRenderScript;
}	
	//


	/*

简单的Render script的例子

root->
	bkg->FillRect(0,0,800,600)
		img->DrawBitmap(b1,100,100,200,200)
			txt->DrawText(f1,"abc" 120,120,100,24)
			txt->DrawText(f1,"good",90,90,100,24)
	
img 加入一个 阴影效果(影响孩子)

	function DrawImgID(target)
		img->DrawBitmap
		txt->DrawText
		txt->DrawText
	end

root->
	bkg->FillRect
		tmp = CreateTempBitmap(img->GetDrawRectIncludeChildren())
		DrawImgID(tmp)
		DrawBitmapShadow(tmp)
		DrawBitmap(tmp,100,100,200,200)
		

		img 加入一个不影响孩子的阴影效果(效果的参数都是位图会比较简单，
		这就要求除非是不拉伸的ImageObject，其它所有UIObject应用了阴影都要创建缓冲)
		如果img有另一个效果毛玻璃 ？ 不行，毛玻璃与对象的layerd属性冲突
root->
	bkg->FillRect(0,0,800,600)
		DrawBitmapShadow(b1)
		img->DrawBitmap(b1,100,100,200,200)
		txt->DrawText(f1,"abc" 120,120,100,24)
		txt->DrawText(f1,"good",90,90,100,24)
	
	
	


使用RenderScript的自动优化

自动层优化的例子？如何自动生成这个TempBitmap以及决定这个TempBitmap可以代替哪些语句


语句 A1,A2,A3,A4,....An   任意连续的Ax->Ay 都可以合并成一个新语句Axy
如果Ax->Ay的参数不变，那么Axy的结果是可以缓冲的

1代表和上一次发生变化的语句
1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
0 1 0 0 0 0 0 0 0 0 0 0 0 1 0 0

自动并行优化的例子？ 
语句A1，A2，A3，A4 如果知道 A4，A3的绘制结果不相交 那么
A1,A2,A3 
      A4
是可以的


自动分块优化：这个会消耗索引器的性能，但实现相对简单


	
	*/

	