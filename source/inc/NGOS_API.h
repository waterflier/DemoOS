//
// NGOS_API.h 
//
// Create:liuzhicong,2013-3-9
//
//	TODO:Ŀǰ����û�п������������û����API������κκ�Ȩ���йصĲ�������Demo�������Ҫ����������� 
//		 ���ڿ�����¼�(SendMessage)�Ĵ��ڣ�����ĳЩ����ԭ���ϻ�Ӧ���ṩ�첽�汾(����CreateUIObject),�ȼ�,DemoOS������ɺ��ٴ���
//		 
//
//////////////////////////////////////////////////////////////////////////


#ifndef _NGOS_KERNEL_API_H_
#define _NGOS_KERNEL_API_H_

#include <NGOS_Errors.h>
#include <NGOS_Types.h>

#ifdef __cplusplus
extern "C"
{
#endif 


#define NGOS_API(x) x 



NGOS_API(int) NGOS_InitRootUIObjTreeEnv(void* param);
NGOS_API(int) NGOS_UninitRootUIObjTreeEnv(void* param);

////////////////////////////////////////////////Root Tree
NGOS_API(NGOS_ROOT_OBJTREE_HANDLE) NGOS_CreateRootObjTree();
NGOS_API(int) NGOS_GetRootObjTree(TYPE_NGOS_PID ownerPID, TYPE_NGOS_OBJTREE_OWNERHOST_INFO* ownerHostInfo,NGOS_ROOT_OBJTREE_HANDLE* pResult);
//NGOS_API(int) NGOS_AddRefRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
//NGOS_API(int) NGOS_ReleaseRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
//ǿ���ͷ����٣�ֻ��RootObjTree�Ĵ������̲����ʸ�
NGOS_API(int) NGOS_DestoryRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
NGOS_API(int) NGOS_UpdateRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree, NGRE_SCRIPT_HANDLE hRenderScript);
NGOS_API(TYPE_NGOS_PID) NGOS_GetRootObjTreeOwnerPID(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
//�õ�root ui object,���objectһ����һ����id��layoutobject,�ڴ���objtree��ʱ����objtree����
NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_GetRootObject(NGOS_ROOT_OBJTREE_HANDLE hRootTree);

////////////////////////////////////////////////UIObject
//������ȫ�����Ƶ�classloader���Һ�classfactory��demo�Ȳ�����
//NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_CreateBaseUIObject(const char* id,const char* className,void* classloader);
NGOS_API(int) NGOS_DestoryUIObject(NGOS_UIOBJECT_HANDLE hUIObject);

//�õ����ͱ�ǩ,ͨ��һЩ����Եõ���ϸ�ڵ����ͣ�����һ�������������ͱ�ǩ�Ͳ�����ˡ�
NGOS_API(uint32_t) NGOS_GetUIObjectTypeFlags(NGOS_UIOBJECT_HANDLE hUIObject);
//����һ������,���������Ͳ���ı��ˡ�����ָ��ָ����ڴ����򲻹��UIObject����(��classloader),���Բ��õ��ķ���ֵ����������
NGOS_API(const char*) NGOS_GetUIObjectClassName(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(const char*) NGOS_GetUIObjectID(NGOS_UIOBJECT_HANDLE hUIObject);
//NGOS_API(int) NGOS_UpdateUIObjectID(NGOS_UIOBJECT_HANDLE hUIObject,TYPE_NGOS_ID newID);

NGOS_API(NGOS_ROOT_OBJTREE_HANDLE) NGOS_GetUIObjectOwnerRootTree(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_GetUIObjectParent(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(TYPE_NGOS_PID) NGOS_GetUIobjectOwnerProcessID(NGOS_UIOBJECT_HANDLE hUIObject);

NGOS_API(int) NGOS_AddChild(NGOS_UIOBJECT_HANDLE hParent,NGOS_UIOBJECT_HANDLE hChild);
//�ؼ����߼�����,������¸������ר�ŵ��ĵ�����������
NGOS_API(int) NGOS_AddControlChild(NGOS_UIOBJECT_HANDLE hParent,NGOS_UIOBJECT_HANDLE hChild);
NGOS_API(int) NGOS_RemoveChild(NGOS_UIOBJECT_HANDLE hUIObject,uint32_t params);
//�ؼ����߼������ǲ����ƶ���?
NGOS_API(int) NGOS_MoveObject(NGOS_UIOBJECT_HANDLE hUIObject,NGOS_UIOBJECT_HANDLE newParent);

NGOS_API(uint32_t) NGOS_GetUIObjectChildrenCount(NGOS_UIOBJECT_HANDLE hUIObject);
//���ݾ���,�������ӵĲ������ٽ��У���������Ż����Բ�����ô��λ
NGOS_API(NGOS_UIOBJECT_LOOKUP_ITERATOR) NGOS_StartLoopupChildren(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_LookupNextChild(NGOS_UIOBJECT_LOOKUP_ITERATOR hLookup);
NGOS_API(int) NGOS_EndLookupChildren(NGOS_UIOBJECT_LOOKUP_ITERATOR hLookup);

//��ͳ��GetUIObject��������������


//UIObject��ͨ������ Ϊ��������ܣ���������� 1��Ϊ�����������set+get 2:���ĳ�����Լ�������set+get ���ա��������ṩlock���ܡ�Ŀǰ���ṩ���ܼ���1��api
//���ڿ��յ���ƣ� �õ�NGOS_UIOBJECT_HANDLE->�õ�һ�����ն���->ͨ��API��������->ͨ��API�޸�����->�ύ�����ϵ��޸ģ������¼���
//                 ʹ�ÿ���Ҫ����������ǣ�������ΪRPC��Ľ��̵������⣬���¸ĵ�һ��Ķ�����ʾ������                  
//                 ���/ɾ�����Ӳ����ÿ�����ɣ���������ͨ��������ɵ����Է����޸ģ�API�Ľӿڻ���HObject
//				   �����Զ�֧���������ջ��ƣ���һ��msg frame���һ�η������Ի�ÿ��գ�msg frame������ʱ���ύ�����ٸÿ���


//�ؼ�λ������(�������Ǻ���������ص�)
NGOS_API(uint32_t) NGOS_GetUIObjectVisibleFlags(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectVisibleFlags(NGOS_UIOBJECT_HANDLE hUIObject,uint32_t newVisibleFlags);
//��������ϵ���͵ĺ������Լ��� ������������������ֱ��ת���ĺ���
NGOS_API(int) NGOS_GetUIObjectAbsRect(NGOS_UIOBJECT_HANDLE hUIObject,RECT* pRect);
NGOS_API(int) NGOS_GetUIObjectRect(NGOS_UIOBJECT_HANDLE hUIObject,RECT* pRect);
NGOS_API(int) NGOS_SetUIObjectRect(NGOS_UIOBJECT_HANDLE hUIObject,RECT* pRect);
NGOS_API(int) NGOS_ResizeUIObject(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_GetUIObjectSize(NGOS_UIOBJECT_HANDLE hUIObject);
//NGOS_API(int) NGOS_GetUIObjectRectExpString(NGOS_UIOBJECT_HANDLE hUIObject,RECT* pRect);
NGOS_API(int) NGOS_GetUIObjectPosTransMartix(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectPosTransMartix(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_GetUIObjectTransMesh(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectTransMesh(NGOS_UIOBJECT_HANDLE hUIObject);
//����zorder,�������Ҳ���Ż�����أ����ڵ�ʹ�����ǻ�ȥ�� ��Ժܴ��zorder����Ժ�С��zorder
NGOS_API(int32_t) NGOS_GetUIObjectAbsZorder(NGOS_UIOBJECT_HANDLE hUIObjec);
NGOS_API(int32_t) NGOS_GetUIObjectZorder(NGOS_UIOBJECT_HANDLE hUIObjec);
NGOS_API(int32_t) NGOS_SetUIObjectZorder(NGOS_UIOBJECT_HANDLE hUIObjec,int32_t newZorder);

//ͨ��content����
NGOS_API(uint8_t) NGOS_GetUIObjectAlpha(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectAlpha(NGOS_UIOBJECT_HANDLE hUIObject,uint8_t newAlpha);
//����effect(������Ӱ����)�Ĵ���Ŀǰ���߼��Ƕ���Ļ��������ǲ����Դ����������rect,��������߼�
//�ᵼ�����������߾��ι������Ļ��ҡ� һ�ַ����ǽ������Ϊ LogicRect��cotentRect,����������������ι������ֿ�����
//��һ�ַ�����EffectList���ܻᴴ��һЩEffectUIObject...�����ͼ򵥵Ľ��������
NGOS_API(int) NGOS_GetUIObjectContentEffectList(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_GetUIObjectBlendAttribute();
NGOS_API(int) NGOS_GetUIObjectIsLimitChild(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectIsLimitChild(NGOS_UIOBJECT_HANDLE hUIObject,BOOL isLimit);
NGOS_API(int) NGOS_GetUIObjectCotentMask(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectCotentMask(NGOS_UIOBJECT_HANDLE hUIObject);

//��ʽ��֧��
//��ʽ���е�һ����������ˣ�1����ζ�λ���ʺϵĶ����selector 2������Ϊ���Ķ������Щ��������Щ�޸�

//UIObject���¼�֧��

//����UIObject��RTTI֧��
NGOS_API(int) NGOS_SetUIObjectAttributeByString(const char* attrName,const char* attrValue,uint32_t valueLen);
NGOS_API(int) NGOS_GetUIObjectAttributeToString(const char* attrName,char* resultBuffer,uint32_t* resultBufferLen);
///////////////////////////////////////////////UIObject Type Loader





//////////////////////////////////////////////// Animation
//NGOS_API(NGOS_ANIMATION_HANDLE) NGOS_CreateAnimation(const char* id,const char* className,void* classloader);
NGOS_API(int) NGOS_AddRefAnimation(NGOS_ANIMATION_HANDLE hAni);
NGOS_API(int) NGOS_ReleaseAnimation(NGOS_ANIMATION_HANDLE hAni);



//////////////////////////////////////////////// Module Manager


//////////////////////////////////////////////// ResManager

#ifdef __cplusplus
}
#endif 

#endif //_NGOS_KERNEL_API_H_