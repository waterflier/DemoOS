//
// RootObjTree.c 
//
//////////////////////////////////////////////////////////////////////////

#include <NGOS_API.h>
#include "./RootObjTree.h"

void for_each()
{
	//在root tree的创建进程，分配一块共享内存。使用连续空间存储了 树状的原始uiobject handle 关系
	

	//每个uiobject的孩子都存在uiobject创建者分配的地址空间里，遍历的时候需要反复的decode uiobject.并且在遍历的时候一致性会有问题？ 
	//每个uiobject进程有一把树修改锁？ 或者把修改树能做出lock-free设计

	//GetUIObject 查找问题: 多个进程维护1个hash_table/map ?

	//求简化设计！
	
}


