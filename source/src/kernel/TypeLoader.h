//
// TypeLoader.h 
// 
// 定义类型管理/加载器 这是一个抽象
// xar之类的概念是这个抽象的一种实现
//
/////////////////////////////////////////////////////////////

#ifndef _NGOS_KERNEL_TYPE_LOADER_H_
#define _NGOS_KERNEL_TYPE_LOADER_H_

struct UIObject;
//存储了扩展UIObject的重要类型信息
//包括 扩展的方法列表，和调用这些方法的方式
//     扩展的属性列表，以及访问属性列表的方式
//     扩展的事件列表，以及访问这些事件的方式
// 
// 这次的目的是解决原来引擎一旦进入了类型可扩展的领域，即使是最简单的控件,也无法剥离与XML和Lua的关系,
// 如果这个问题无法解决，干脆在kernel直接引入Lua
// 如果这个问题能解决，那么可以在一定程度上，允许同一个objtree上，有的control用lua,有的control用js


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

//type loader mananger允许按一定的顺序在一些type loader里查找类型
//如果typeloader里存在同名类型，那么提供解决冲突的办法，并让按类型名搜索的结果是符合逻辑的
//实际上任何一段代码，在执行create instance的时候，都应该有一个OwnerModule?（或默认namespace?） 会影响TypeLoaderManager的执行结果
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