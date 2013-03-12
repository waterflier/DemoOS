//
// RootObjTree.h 
//
// Create:liuzhicong,2013-3-9
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_ROOTOBJ_TREE_H_
#define _NGOS_ROOTOBJ_TREE_H_
/*
namespace NGOS
{

//一个进程拥有，可被多个进程共享的对象树
//在一个进程里，最好是UI线程

//另一种模型是 对象树的孩子除了uiobject,还可以是另一个uiobjtree, uiobjtree相对于普通的uiobject,最大的区别就是持有一些索引器和管理器这样的重型资源
//这样分级索引做的好可以有效的提高性能和稳定性，并控制好锁的粒度 (有点类似目前的层对象)
//索引器里可以索引 “子索引器” 当能明确一个区域里的对象不会超过该区域时，可以为该区域建立一个子索引器。当索引器需要hittest该区域时，会继续调用子索引器的查找函数
//提供这种分级索引能让使用者多一种选择，更好的优化系统的性能

//如何使用 硬件提供的 指令原子特性来减少锁操作，提高性能

//为了提高性能，会让很多关键数据在共享内存上。如何让这些共享内存不会因为使用者对常用API的不正确的调用释放不掉，包括某些奇怪的进程崩溃流程
//伸缩性 当RootObjTree只有一个进程访问时，能非常快(disable process share)
//root obj tree的所有进程对其它进程的对象访问很简单

//这么复杂的共享内存控制，最好用C写，(可以用C++ 提供sim_class ,但貌似没太大必要性)免的把自己弄到坑里
//无进程间通信（c-s）模式设计 这样可以最大程度的避免性能损耗，但易于引起崩root obj tree owner process崩溃,这是一个必须coding的极其精巧的，支持并发访问的数据结构(会不会太复杂了?有点像内核开发)
//不过考虑到以后app framework会鼓励用户使用另一种更不容易出错的语言（而不是C/C++） kernel的各种C接口和设计还是以性能为先
//尽量减少对UIObj的decode_handle操作。 
//y
class RootObjTree
{
public:
	//对象索引器，用于的对树上的对象进行HitTest与Select
	//在添加，删除对象，更新对象的位置属性和可见属性时，需要更新索引器(当对象由于进程结束而无效时，如何从索引器里删除？最简单的方法，让索引器也属于那个进程，或者在内核里添加进程结束的通知，帮助正确的删除)
	//如果每个进程都有索引器，那么根树进行查询时，就要至少在这些索引器里都查找一次
	ObjectIndex _index;

	//脏矩形管理器，用于判断下次绘制时应绘制哪些部分
	//在改变对象属性之前，要拿到一个对应的锁
	DirtyRectManager _dirtyRectManager;

	//根据一个裁剪区域得到一段可以运行的绘制脚本
	//这个函数运行期间，会锁住UIObjTree(粒度? ) 所以必须是非常快且内部无锁的
	char* GetRenderScript(RECT clipRect)
	{
		//已经包含了zorder顺序?
		vecotr<OBJ_HANDLE> vh = _index.SelectVisibleObject(clipRect);
		for(int i=0;i<vh.size();++i)
		{
			AppendRenderScript(GetObjRenderScript(vh[i],clipRect));
		}
	}

	char* GetObjRenderScript(OBJ_HANDLE hObj,RECT clipRect)
	{

	}

	//访问对象树上的对象应该是无锁的，添加删除用了索引器锁
	//如何防止一个进程操作后进程卡住引起的整个root obj tree不响应

	void AddChild(OBJTREE_HANDLE hTree,OBJ_HANDLE hObj,OBJ_HANDLE hFather)
	{
		pObj = GetObjData(hObj);
		if(pObj == NULL)
		{
			return;
		}
		pObj->m_father = hFather;
		pObj->m_owner = hTree;

		pFather = GetObjData(hFather); 
		if(GetOwnerProcess(pFather) == getpid())
		{
			pFather->m_child.push_back(pObj);
			GetIndex(pFather)->updateIndex(pObj);
		}

		
		ReleaseObjData(hObj);
	}

	void RemoveChild(OBJ_HANDLE hObj)
	{

	}

	void MoveChild(OBJ_HANDLE hObj,OBJ_HANDLE newFather)
	{

	}
	
};

}
*/

#endif