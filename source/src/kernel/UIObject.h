//
// UIObject.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_UIOBJECT_H_
#define _NGOS_UIOBJECT_H_

#include "./TypeLoader.h"
#include "../render_engine/RenderScript.h"
#include "./objectIndex/UIObjectVector.h"
#include "./input/InputTarget.h"

/*
关键数据结构

UIObject
{
	char* ID;?
	ClassInfo* pClassInfo;//类型信息,类型信息属于一个class loader,节约内存
    UIObject* parent;
	UIObjectTree* owner;
	UIObjectVector children;
	UIObjectVector LogicControlChildren;
	
	FindObject(string path)
	{
		if(owner)
		{
			return owner->FindObject(GetFullPath(path));
		}
		else
		{
		     for_each(child in LogicControlChildren)
			 {
				if(child.id == path)
				{
					return child;
				}
			 }
		}
	}
}


UIObjectTree
{
	UIObject* RootObject
	UIObjectMap<string,UIObject*> allUIObject;
	UIObject* FindObject(string path)
	{
		//原来使用多级索引，现在统一使用UIObjTree的名字索引，有好处么？
		return allUIObject.find(path);
	}


}
*/



typedef struct tagUIObjectTransInfo
{

}UIObjectTransInfo;

typedef struct tagUIObjectTransMeshInfo
{

}UIObjectTransMeshInfo;

typedef struct tagUIObjectMaskInfo
{

}UIObjectMaskInfo;

typedef struct tagUIObjectBlendInfo
{

}UIObjectBlendInfo;

typedef struct tagUIObjectEffectList
{

}UIObjectEffectList;



typedef struct tagUIObjectProvier
{
	//是否可以和TypeInfo
	pfnGetRenderScript fnGetRenderScript;
}UIObjectProvier;

typedef struct tagUIObject
{
	//CommonObjectHeader Header;
	TYPE_NGOS_PID      OwnerPID;

	//class info
	UIObjectTypeInfo*  pTypeInfo;
	UIObjectProvier*   Imp;//指向type info,
	//idinfo
	char* strID;

	//构成树结构的关键属性
	NGOS_UIOBJECT_HANDLE hSelf;
	NGOS_UIOBJECT_HANDLE hParent;
	//必须成为控件的逻辑孩子才会有这个值
	NGOS_UIOBJECT_HANDLE hOwnerControl;
	NGOS_ROOT_OBJTREE_HANDLE hOwnerTree;
	
	UIObjectVector* pChildren;
	UIObjectVector* pLogicControlChildren;

	//flags 
	// self-visible parent-visible children-visible logic-children-visible is-limnit-child 
	uint32_t VisibleFlags;
	//self-enable parent-enalbe 
	uint32_t EnableFlags;
	//[4bits uiobject|halfdarwobject|drawobject] 
	uint32_t TypeFlags;
	uint8_t	 Alpha;

	//postion info
	//uint8_t	CoordinateType; 
	//uint16_t DPI;
	RealRECT ObjRealRect; //demo可以试一下基于浮点数的坐标系
	RECT ObjRect;
	RECT ObjAbsRect;
	RECT ObjDrawRect;//绘制影响矩形，只对索引器有效。更新DrawRect保证如果绘制的ViewRect与DrawRect不相交,那么Object一定会影响ViewRect
	int32_t ObjZorder;
	int32_t ObjAbsZorder;
	//加入Content Update事件，为位置表达式提供 content.width,content.height 支持
	//位置表达式,demo应该还是需要支持的
	

	InputTarget* pInputTarget;
	UIObjectTransInfo* pTransInfo;
	UIObjectTransMeshInfo* pMeshInfo;
	UIObjectMaskInfo* pMaskInfo;
	//UIObjectBlendInfo* pBlendInfo;
	UIObjectEffectList* pEffectList;
	
	

}UIObject;

//两段式构造与销毁
UIObject* MallocUIObject(NGOS_RootTreeEnv* pEnv,size_t userDataLen);
int UIObjectInit(UIObject* pObj);
int UIObjectUninit(UIObject* pObj);
int FreeUIObject(UIObject* pObj);
void* UIObjectGetUserDataStart(UIObject* pObj);

int UIObjectAddChild(UIObject* pObject,NGOS_UIOBJECT_HANDLE hChild,BOOL isLogicChild);
int UIObjectRemoveChild(UIObject* pObject,NGOS_UIOBJECT_HANDLE hChild);
int UIObjectMove(UIObject* pObject,RECT* pNewPos);
int UIObjectMoveReal(UIObject* pObject,RealRECT* pNewPos);

int UIObjectSetTrans(UIObject* pObject,Matrix3X2* pTransMartix);
//int UIObjectSetMesh(UIObject* pObject,); demo暂时不做也可,Mesh可以计算得到包含矩形
int UIObjectSetVisibleFlags(UIObject* pObject,uint32_t visibleFlags);

int UIObjectGetVisibleRect(UIObject* pObject,RECT* absRect);
#endif 
