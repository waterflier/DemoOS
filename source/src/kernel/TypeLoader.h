//
// TypeLoader.h 
// 
// �������͹���/������ ����һ������
// xar֮��ĸ�������������һ��ʵ��
//
/////////////////////////////////////////////////////////////

#ifndef _NGOS_KERNEL_TYPE_LOADER_H_
#define _NGOS_KERNEL_TYPE_LOADER_H_

struct UIObject;
struct TypeLoader;
//�洢����չUIObject����Ҫ������Ϣ
//���� ��չ�ķ����б��͵�����Щ�����ķ�ʽ
//     ��չ�������б��Լ����������б�ķ�ʽ
//     ��չ���¼��б��Լ�������Щ�¼��ķ�ʽ
// 
// ��ε�Ŀ���ǽ��ԭ������һ�����������Ϳ���չ�����򣬼�ʹ����򵥵Ŀؼ�,Ҳ�޷�������XML��Lua�Ĺ�ϵ,
// �����������޷�������ɴ���kernelֱ������Lua
// �����������ܽ������ô������һ���̶��ϣ�����ͬһ��objtree�ϣ��е�control��lua,�е�control��js
struct UIObjectTypeInfo;

typedef struct tagUIObjectTypeInfo
{
	(const char*) (*fnGetClassName) (UIObjectTypeInfo* self);
	(UIObject*) (*fnCraeteUIObject) (UIObjectTypeInfo* self,const char* className,const char* id);
	(TypeLoader*) (*fnGetOwnerTypeLoader) (UIObjectTypeInfo* self);
	(int) (*fnAddRef) (UIObjectTypeInfo* self);
	(int) (*fnRelease) (UIObjectTypeInfo* self);
}UIObjectTypeInfo;


struct AnimationTypeInfo;
typedef struct tagAnimationTypeInfo
{

}AnimationTypeInfo;



typedef struct tagTypeLoader
{
	
	typedef UIObjectTypeInfo* (*fnGetUIObjectTypeInfoByClassName) (TypeLoader* self,const char* className);
	typedef UIObjectTypeInfo* (*fnGetUIObjectTypeInfo) (TypeLoader* self, UIObject* pObj);
	typedef int (*fnRegisterTypeInfo) (TypeLoader* self,UIObjectTypeInfo* pTypeInfo);
	typedef int (*fnAddRef) (TypeLoader* self);
	typedef int (*fnRelease) (TypeLoader* self);
}TypeLoader;

//type loader mananger����һ����˳����һЩtype loader���������
//���typeloader�����ͬ�����ͣ���ô�ṩ�����ͻ�İ취�����ð������������Ľ���Ƿ����߼���
//ʵ�����κ�һ�δ��룬��ִ��create instance��ʱ�򣬶�Ӧ����һ��OwnerModule?����Ĭ��namespace?�� ��Ӱ��TypeLoaderManager��ִ�н��
typedef struct tagTypeLoaderManager
{
	typedef UIObject* (*fnCreateUIObject) (TypeLoader* self,const char* classPath,const char* id);
	typedef void (*fnSearchUIObjectTypeInfoByClassName)(void);

	//void SetDefaultTypeLoader();
	//void SetFrameworkTypeLoader();
	//void SetAppTypeLoader();
	//void SetModuleTypeLoader();
};

#endif //_NGOS_KERNEL_TYPE_LOADER_H_