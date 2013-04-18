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
�ؼ����ݽṹ

UIObject
{
	char* ID;?
	ClassInfo* pClassInfo;//������Ϣ,������Ϣ����һ��class loader,��Լ�ڴ�
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
		//ԭ��ʹ�ö༶����������ͳһʹ��UIObjTree�������������кô�ô��
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
	//�Ƿ���Ժ�TypeInfo
	pfnGetRenderScript fnGetRenderScript;
}UIObjectProvier;

typedef struct tagUIObject
{
	//CommonObjectHeader Header;
	TYPE_NGOS_PID      OwnerPID;

	//class info
	UIObjectTypeInfo*  pTypeInfo;
	UIObjectProvier*   Imp;//ָ��type info,
	//idinfo
	char* strID;

	//�������ṹ�Ĺؼ�����
	NGOS_UIOBJECT_HANDLE hSelf;
	NGOS_UIOBJECT_HANDLE hParent;
	//�����Ϊ�ؼ����߼����ӲŻ������ֵ
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
	RealRECT ObjRealRect; //demo������һ�»��ڸ�����������ϵ
	RECT ObjRect;
	RECT ObjAbsRect;
	RECT ObjDrawRect;//����Ӱ����Σ�ֻ����������Ч������DrawRect��֤������Ƶ�ViewRect��DrawRect���ཻ,��ôObjectһ����Ӱ��ViewRect
	int32_t ObjZorder;
	int32_t ObjAbsZorder;
	//����Content Update�¼���Ϊλ�ñ��ʽ�ṩ content.width,content.height ֧��
	//λ�ñ��ʽ,demoӦ�û�����Ҫ֧�ֵ�
	

	InputTarget* pInputTarget;
	UIObjectTransInfo* pTransInfo;
	UIObjectTransMeshInfo* pMeshInfo;
	UIObjectMaskInfo* pMaskInfo;
	//UIObjectBlendInfo* pBlendInfo;
	UIObjectEffectList* pEffectList;
	
	

}UIObject;

//����ʽ����������
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
//int UIObjectSetMesh(UIObject* pObject,); demo��ʱ����Ҳ��,Mesh���Լ���õ���������
int UIObjectSetVisibleFlags(UIObject* pObject,uint32_t visibleFlags);

int UIObjectGetVisibleRect(UIObject* pObject,RECT* absRect);
#endif 
