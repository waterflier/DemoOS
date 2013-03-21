//
// RootObjTree.c 
//
//////////////////////////////////////////////////////////////////////////

#include <NGOS_API.h>
#include "./RootObjTree.h"
#include "../os_interface/os_interface.h"

//void for_each()
//{
	//在root tree的创建进程，分配一块共享内存。使用连续空间存储了 树状的原始uiobject handle 关系
	

	//每个uiobject的孩子都存在uiobject创建者分配的地址空间里，遍历的时候需要反复的decode uiobject.并且在遍历的时候一致性会有问题？ 
	//每个uiobject进程有一把树修改锁？ 或者把修改树能做出lock-free设计

	//GetUIObject 查找问题: 多个进程维护1个hash_table/map ?

	//求简化设计！
	
//}

/*
第一条路 基于ShareMemory的已经不行了
问题在于安全性 和 无法切换到另一个进程执行代码
开发复杂度过高，需要大量能在连续SM下工作的定制数据结构，感觉不优雅


第三条路 向windows学习，提供内核态API
*添加对象:
--kernel---
kmalloc(sizeof(uiobject))
setvalue(uiobject)
lock(tree)
tree.add(uiobject)
unlock(tree)
fireevent(uibject,"onbind")

*fireevent
--kernel--
for_each(event_nod in uiobject.events)
{
   //1直接switch到event_nod.pid进程，然后强制jump无疑就是中断，一般的user-mode程序通常都不具备中断安全性
   //2 postmsg(event_nod.pid) 
}

*绘制
objlist = index.select(drawRect)
result = ""
for_each(uiobject in objlist)
{
	result += getfullscript(uiobject.getbasescript(),uiobject.getparmers())
}
return result;

*在user-mode遍历tree（效率很低）

内核模式缺点: fire event一样要用postmsg
uiobject在内核分配,所有访问都要经过内核，造成的system_call过多（(能不能提高owenr pid访问的速度？）
一旦出现崩溃，整个玩完
没有内核模块的实际开发经验，也许会掉坑里

===============================================================

第二条路 基于rpc+proxy object
正在尝试
目前使用的是同步rpc，害怕锁死 (server 进程一定不能等待client进程，server一卡住就全完了)
如果把api都改成异步的，rpc就会很好用 （但多了很多基础状态） 处理的好系统更不容易出现死锁的情况，但缺点是？
这种模式对于事件触发，只能采用postmsg的方法，有一些事件处理有 时序性 和 处理顺序依赖性的，会不适合，需要做比较晦涩的改造（preevet,所有的响应者都投票表示自己不handle了之后再开始真正的处理过程，相当复杂）
rpc模式更像是进化的xserver,一样可以实现在一台机器上操作另一台机器的界面（做好异步！）
*root tree server里的才是uiobject的真身，一个对象在没有被跨进程访问之前（绘制?） 可以只存在于其创建进程?这种优化有意义吗或能实现么?
如果上述优化失效的情况下,可以用本地对象快照优化:在一个消息处理过程中(一个msg frame)，对对象的一次访问会触发给该对象创建一个快照，在这个msg frame里再次访问该对象都是访问该快照，也可以手动控制 获得快照，再从快照里获得属性


void AddChild(hparent,pObject)
{
   if (getownerpid(hparent) == pid())
   {
	   //decode 属于这个进程的 uiobject
       parent = decode(hparent)
	   parent.childs.add(make_handle(pObject))
   }
   else
   {
		roottree.rpccall("addchild",hparent,make_handle(pObject)
   }
}

void ResizeObject(hobject,newRect)
{

	if(getownerpid(hobject) == pid)
	{
		pobject = decode(hobject)
	}
	else
	{
		roottree.rpccall("resize",hobject,newRect)
	}

	client向server rpc是靠谱的，因为正确的server进程能保证立即响应。这要求server端的代码不能有任何因为client的问题卡住的可能
	server不能反向等待client对某个message的处理，理由已说
	
}



void OnAddChildRequest(hparent,hobject)
{
    node = get_node(hparent);
	node.childs.add(hobject);
	get_obj_index().updateIndex(hobject);
}
 
void OnResizeObjectRequest(hobject,newRect)
{
	_index.updateObjectChanged(hobject,newRect) //索引器里的对象位置与对象decode出来的UIObject里的位置会不同步?
	postmessage(hobject.ownerthread,MSG_ONSIZE,newRect)//通知hobject所在进程更新内部数据
	
	//用SendMessage可以同步 ，但是sendMessage要对方处理完消息，对方在处理消息时有可能会发起rpc，于是就卡死了
}

//考虑在rpc roottree的情况下的事件处理
//一个对象关心另一个对象的事件，比如child关心parent的onsize事件
void AttachListener(hobject,callback|usermsg)
{
	//对象在同一个进程
	//对象在另一个进程
	//有没有同步事件： 可以简单的约定同步事件的obj-listener必须在同一个进程里

	//一种错误
	// objA (pid=1
	    objB (pid=2
		   objC (pid=2
		    objD (pid=3)
	  
	  
	  objA OnSize()
        
	    rpccall
		 postmessage(objB,onsize)
		end
       
		objB onsize 
		   objC onsize
		     rpccall
			  postmessage objD onsize


	//如果在进程pid3里同时关心objA,objC的OnSize事件，那么这两个事件的到达顺序会无法保证？ 无所谓

}

void GetOwnerControl(hobject)
{
    if(getownerpid(hobject) == pid())
	{
	    pObject = decode(hobject)
		ownrcontrol = pObject->m_ownerControl;
	}
	else
	{
		roottree.rpccall("getownercontrol",hobject);
	}
}

void GetControlObject(hparent,path)
{
	if(haveOtherProcessObject(hparent))
	{
		roottree.rpccall("getcontrolobject",hparent,path)
	}
	else
	{
		if(getownerpid(hparent) == pid())
		{
			pParent = decode(hparent);
			pParent->GetControlObject();
		}
		else
		{
			roottree.rpccall("getcontrolobject",hparent,path)
		}
	}
}


//得到另一个进程对象的 自定义属性。普通属性
string GetAttr(hobj,attrname)
{
	proxy = CreateProxy(getownerpid(hobj))
	// rpcall的返回需要对方进程是活动的，如果对方进程卡死，那么该操作也会卡死。所以谨慎使用
	return proxy.rpccall("getattr",attrname)
}

//调用另一个进程对象的自定义函数


//连接另一个进程对象的自定义事件


//对象fire自定义事件

//渲染时 遍历获取UIObject的render script
for_each proxy in tree do
	proxy:get_render_script()
end

proxy:get_render_script()
	
end

*/
static RootTreeNode* s_rootTree;

//static RootTreeClient* createRootTreeClient(TYPE_NGOS_PID serverPID);
//static NGOS_ROOT_OBJTREE_HANDLE createRootUIObject();
//static NGOS_ROOT_OBJTREE_HANDLE findUIObjectAtProxyMap(UIObjectProxyMap* self,const char* id);
//static int insertUIObjectToProxyMap(UIObjectProxyMap* self,NGOS_ROOT_OBJTREE_HANDLE hUIObject);

static NGOS_ROOT_OBJTREE_HANDLE clientRPCGetRootUIObject(RootTreeClient* self);

NGOS_API(int) NGOS_InitRootUIObjTreeEnv(void*)
{
	return 0;
}

NGOS_API(int) NGOS_UninitRootUIObjTreeEnv(void*)
{
	return 0;
}

////////////////////////////////////////////////Root Tree
NGOS_API(NGOS_ROOT_OBJTREE_HANDLE) NGOS_CreateRootObjTree()
{
	return NULL;
}

NGOS_API(int) NGOS_GetRootObjTree(TYPE_NGOS_PID ownerPID, TYPE_NGOS_OBJTREE_OWNERHOST_INFO* ownerHostInfo,NGOS_ROOT_OBJTREE_HANDLE* pResult)
{
	int resultCode = 0;
	if(ownerPID == 0)
	{
		TYPE_NGOS_SHAREMEMORY hSM = OSI_OpenSM(CONFIG_ROOTTREE_SM_NAME);
		if(hSM)
		{
			RootTreeInfoNodeInSM* pInfo = (RootTreeInfoNodeInSM*) OSI_LockSMBuffer(hSM);
			ownerPID = pInfo->PID;
		}
		else
		{
			resultCode = -1;
		}
	}
	
	if(resultCode == 0)
	{
		if(ownerPID != OSI_GetPID())
		{
			RootTreeClient* pClient = createRootTreeClient(ownerPID);
			*pResult = pClient;
		}
		else
		{
			*pResult = s_rootTree;
		}
	}
	

	if(*pResult != NULL)
	{
		return 0;
	}
	else
	{
		return resultCode;
	}

}

//NGOS_API(int) NGOS_AddRefRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
//NGOS_API(int) NGOS_ReleaseRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree);
//强制释放销毁，只有RootObjTree的创建进程才有资格
NGOS_API(int) NGOS_DestoryRootObjTree(NGOS_ROOT_OBJTREE_HANDLE hRootTree)
{
	TYPE_NGOS_PID pid = NGOS_GetRootObjTreeOwnerPID(hRootTree);
	if(pid == OSI_GetPID())
	{
		//OK

	}
	else
	{
		return -1;
	}
}

NGOS_API(TYPE_NGOS_PID) NGOS_GetRootObjTreeOwnerPID(NGOS_ROOT_OBJTREE_HANDLE hRootTree)
{
	//TODO
	return 0;
}
//得到root ui object,这个object一定是一个无id的layoutobject,在创建objtree的时候由objtree创建
NGOS_API(NGOS_UIOBJECT_HANDLE) NGOS_GetRootObject(NGOS_ROOT_OBJTREE_HANDLE hRootTree)
{
	TYPE_NGOS_PID pid = NGOS_GetRootObjTreeOwnerPID(hRootTree);
	if(pid == OSI_GetPID())
	{
		if(s_rootTree)
			return s_rootTree->RootUIObject;
	}
	else
	{
		RootTreeClient* pClient = (RootTreeClient*)HandleMapDecode((void*)hRootTree);
		if(pClient)
		{
			return clientRPCGetRootUIObject(pClient);
		}
	}

	return NULL;
}

