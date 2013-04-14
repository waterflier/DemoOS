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
//�洢����չUIObject����Ҫ������Ϣ
//���� ��չ�ķ����б��͵�����Щ�����ķ�ʽ
//     ��չ�������б��Լ����������б�ķ�ʽ
//     ��չ���¼��б��Լ�������Щ�¼��ķ�ʽ
// 
// ��ε�Ŀ���ǽ��ԭ������һ�����������Ϳ���չ�����򣬼�ʹ����򵥵Ŀؼ�,Ҳ�޷�������XML��Lua�Ĺ�ϵ,
// �����������޷�������ɴ���kernelֱ������Lua
// �����������ܽ������ô������һ���̶��ϣ�����ͬһ��objtree�ϣ��е�control��lua,�е�control��js


typedef struct tagUIObjectTypeInfo
{
	const char* (*fnGetClassName) (struct UIObjectTypeInfo* self);
	struct UIObject* (*fnCraeteUIObject) (struct UIObjectTypeInfo* self,const char* className,const char* id);
	struct TypeLoader* (*fnGetOwnerTypeLoader) (struct UIObjectTypeInfo* self);
	int (*fnAddRef) (struct UIObjectTypeInfo* self);
	int (*fnRelease) (struct UIObjectTypeInfo* self);
}UIObjectTypeInfo;



typedef struct tagAnimationTypeInfo
{

}AnimationTypeInfo;



typedef struct tagTypeLoader
{
	
	struct UIObjectTypeInfo* (*fnGetUIObjectTypeInfoByClassName) (struct TypeLoader* self,const char* className);
	struct UIObjectTypeInfo* (*fnGetUIObjectTypeInfo) (struct TypeLoader* self, struct UIObject* pObj);
	int (*fnRegisterTypeInfo) (struct TypeLoader* self,UIObjectTypeInfo* pTypeInfo);
	int (*fnAddRef) (struct TypeLoader* self);
	int (*fnRelease) (struct TypeLoader* self);
}TypeLoader;

//type loader mananger����һ����˳����һЩtype loader���������
//���typeloader�����ͬ�����ͣ���ô�ṩ�����ͻ�İ취�����ð������������Ľ���Ƿ����߼���
//ʵ�����κ�һ�δ��룬��ִ��create instance��ʱ�򣬶�Ӧ����һ��OwnerModule?����Ĭ��namespace?�� ��Ӱ��TypeLoaderManager��ִ�н��
typedef struct tagTypeLoaderManager
{
	struct UIObject* (*fnCreateUIObject) (TypeLoader* self,const char* classPath,const char* id);
	void (*fnSearchUIObjectTypeInfoByClassName)(void);

	//void SetDefaultTypeLoader();
	//void SetFrameworkTypeLoader();
	//void SetAppTypeLoader();
	//void SetModuleTypeLoader();
}TypeLoaderManager;


#endif //_NGOS_KERNEL_TYPE_LOADER_H_