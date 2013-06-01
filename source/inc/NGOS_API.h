//
// NGOS_API.h 
//
// Create:liuzhicong,2013-3-9
//
//	TODO:目前由于没有考虑清楚，所以没有在API里加入任何和权限有关的参数。在Demo开发完后，要考虑整体加入 
//		 由于跨进程事件(SendMessage)的存在，所以某些函数原则上还应该提供异步版本(比如CreateUIObject),先简化,DemoOS开发完成后再处理
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
//强制释放销毁，只有RootObjTree的创建进程才有资格
NGOS_API(int) NGOS_DestoryRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
NGOS_API(int) NGOS_UpdateRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree, NGRE_SCRIPT_HANDLE hRenderScript);
NGOS_API(TYPE_NGOS_PID) NGOS_GetRootObjTreeOwnerPID(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
//得到root ui object,这个object一定是一个无id的layoutobject,在创建objtree的时候由objtree创建
NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_GetRootObject(NGOS_ROOT_OBJTREE_HANDLE hRootTree);

////////////////////////////////////////////////UIObject
//建立健全更完善的classloader查找和classfactory，demo先不管了
//NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_CreateBaseUIObject(const char* id,const char* className,void* classloader);
NGOS_API(int) NGOS_DestoryUIObject(NGOS_UIOBJECT_HANDLE hUIObject);

//得到类型标签,通过一些宏可以得到更细节的类型，对象一旦创建，其类型标签就不会变了。
NGOS_API(uint32_t) NGOS_GetUIObjectTypeFlags(NGOS_UIOBJECT_HANDLE hUIObject);
//对象一旦创建,其类型名就不会改变了。这里指针指向的内存区域不归该UIObject所有(归classloader),所以不用担心返回值的生命周期
NGOS_API(const char*) NGOS_GetUIObjectClassName(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(const char*) NGOS_GetUIObjectID(NGOS_UIOBJECT_HANDLE hUIObject);
//NGOS_API(int) NGOS_UpdateUIObjectID(NGOS_UIOBJECT_HANDLE hUIObject,TYPE_NGOS_ID newID);

NGOS_API(NGOS_ROOT_OBJTREE_HANDLE) NGOS_GetUIObjectOwnerRootTree(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_GetUIObjectParent(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(TYPE_NGOS_PID) NGOS_GetUIobjectOwnerProcessID(NGOS_UIOBJECT_HANDLE hUIObject);

NGOS_API(int) NGOS_AddChild(NGOS_UIOBJECT_HANDLE hParent,NGOS_UIOBJECT_HANDLE hChild);
//控件的逻辑孩子,这个是新概念，会有专门的文档描述这个设计
NGOS_API(int) NGOS_AddControlChild(NGOS_UIOBJECT_HANDLE hParent,NGOS_UIOBJECT_HANDLE hChild);
NGOS_API(int) NGOS_RemoveChild(NGOS_UIOBJECT_HANDLE hUIObject,uint32_t params);
//控件的逻辑孩子是不能移动的?
NGOS_API(int) NGOS_MoveObject(NGOS_UIOBJECT_HANDLE hUIObject,NGOS_UIOBJECT_HANDLE newParent);

NGOS_API(uint32_t) NGOS_GetUIObjectChildrenCount(NGOS_UIOBJECT_HANDLE hUIObject);
//根据经验,遍历孩子的操作很少进行，顾这里的优化可以不用那么到位
NGOS_API(NGOS_UIOBJECT_LOOKUP_ITERATOR) NGOS_StartLoopupChildren(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_LookupNextChild(NGOS_UIOBJECT_LOOKUP_ITERATOR hLookup);
NGOS_API(int) NGOS_EndLookupChildren(NGOS_UIOBJECT_LOOKUP_ITERATOR hLookup);

//传统的GetUIObject函数族会重新设计


//UIObject的通用属性 为了提高性能，有两种设计 1：为单个属性设计set+get 2:针对某类属性集合设置set+get 快照。尽量不提供lock功能。目前先提供功能集合1的api
//基于快照的设计： 得到NGOS_UIOBJECT_HANDLE->得到一个快照对象->通过API访问属性->通过API修改属性->提交快照上的修改（触发事件）
//                 使用快照要解决的问题是：不会因为RPC间的进程调度问题，导致改到一半的对象被显示出来了                  
//                 添加/删除孩子不能用快照完成，其它不能通过快照完成的属性访问修改，API的接口还是HObject
//				   或者自动支持上述快照机制，在一个msg frame里，第一次访问属性获得快照，msg frame结束的时候提交并销毁该快照


//关键位置属性(几乎都是和索引器相关的)
NGOS_API(uint32_t) NGOS_GetUIObjectVisibleFlags(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectVisibleFlags(NGOS_UIOBJECT_HANDLE hUIObject,uint32_t newVisibleFlags);
//还有坐标系类型的函数，以及在 物理坐标与像素坐标直接转换的函数
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
//关于zorder,在设计上也有优化的余地，现在的使用总是会去设 相对很大的zorder和相对很小的zorder
NGOS_API(int32_t) NGOS_GetUIObjectAbsZorder(NGOS_UIOBJECT_HANDLE hUIObjec);
NGOS_API(int32_t) NGOS_GetUIObjectZorder(NGOS_UIOBJECT_HANDLE hUIObjec);
NGOS_API(int32_t) NGOS_SetUIObjectZorder(NGOS_UIOBJECT_HANDLE hUIObjec,int32_t newZorder);

//通用content属性
NGOS_API(uint8_t) NGOS_GetUIObjectAlpha(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectAlpha(NGOS_UIOBJECT_HANDLE hUIObject,uint8_t newAlpha);
//对于effect(考虑阴影即可)的处理，目前的逻辑是对象的绘制内容是不可以大于其自身的rect,打破这个逻辑
//会导致索引器，赃矩形管理器的混乱。 一种方法是将对象分为 LogicRect和cotentRect,这样索引器和脏矩形管理器分开更新
//另一种方法是EffectList可能会创建一些EffectUIObject...这样就简单的解决了问题
NGOS_API(int) NGOS_GetUIObjectContentEffectList(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_GetUIObjectBlendAttribute();
NGOS_API(int) NGOS_GetUIObjectIsLimitChild(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectIsLimitChild(NGOS_UIOBJECT_HANDLE hUIObject,BOOL isLimit);
NGOS_API(int) NGOS_GetUIObjectCotentMask(NGOS_UIOBJECT_HANDLE hUIObject);
NGOS_API(int) NGOS_SetUIObjectCotentMask(NGOS_UIOBJECT_HANDLE hUIObject);

//样式表支持
//样式表中的一条表像包含了：1，如何定位到适合的对象的selector 2，被定为到的对象的哪些属性做哪些修改

//UIObject的事件支持

//对于UIObject的RTTI支持
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