//
// UIObject.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_UIOBJECT_H_
#define _NGOS_UIOBJECT_H_

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
	(void) (*pfn) (void)
}UIObjectProvier;

typedef struct tagUIObject
{
	CommonObjectHeader Header;
	TYPE_NGOS_PID      OwnerPID:
	UIObjectProvier*   Imp;//指向type info,

	NGOS_UIOBJECT_HANDLE hSelf;
	NGOS_UIOBJECT_HANDLE hParent;
	//必须成为控件的逻辑孩子才会有这个值
	NGOS_UIOBJECT_HANDLE hOwnerControl;
	NGOS_ROOT_OBJTREE_HANDLE hOwnerTree;
	
	UIObjectVector Children;
	UIObjectVector LogicControlChildren;

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
	RealRECT ObjRealRect;
	RECT ObjRect;
	RECT ObjAbsRect;
	//位置表达式,先不管
	

	InputTarget* pInputTarget;
	UIObjectTransInfo* pTransInfo;
	UIObjectTransMeshInfo* pMeshInfo;
	UIObjectMaskInfo* pMaskInfo;
	UIObjectBlendInfo* pBlendInfo;
	UIObjectEffectList* pEffectList;
	//class info
	
	//idinfo
	uint16_t IDLength;
	char     ID;
}UIObject;

//两段式构造与销毁
UIObject* MallocUIObject(NGOS_RootTreeEnv* pEnv);
int UIObjectInit(UIObject* pObj);
int UIObjectUninit(UIObject* pObj);
int FreeUIObject(UIObject* pObj);

int UIObjectAddChild(UIObject* pObject,NGOS_UIOBJECT_HANDLE hChild,BOOL isLogicChild);
int UIObjectRemoveChild(UIObject* pObject,NGOS_UIOBJECT_HANDLE hChild);
int UIObjectMove(UIObject* pObject,RECT* pNewPos);
int UIObjectMoveReal(UIObject* pObject,RealRECT* pNewPos);

int UIObjectSetTrans(UIObject* pObject,Matrix3X2* pTransMartix);
//int UIObjectSetMesh(UIObject* pObject,); demo暂时不做也可,Mesh可以计算得到包含矩形
int UIObjectSetVisibleFlags(UIObject* pObject,uint32_t visibleFlags);

int UIObjectGetVisibleRect(UIObject* pObject,RECT* absRect);
#endif 
