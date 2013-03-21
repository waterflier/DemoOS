//
// RootObjTree.c 
//
//////////////////////////////////////////////////////////////////////////

#include <NGOS_API.h>
#include "./RootObjTree.h"
#include "../os_interface/os_interface.h"

//void for_each()
//{
	//��root tree�Ĵ������̣�����һ�鹲���ڴ档ʹ�������ռ�洢�� ��״��ԭʼuiobject handle ��ϵ
	

	//ÿ��uiobject�ĺ��Ӷ�����uiobject�����߷���ĵ�ַ�ռ��������ʱ����Ҫ������decode uiobject.�����ڱ�����ʱ��һ���Ի������⣿ 
	//ÿ��uiobject������һ�����޸����� ���߰��޸���������lock-free���

	//GetUIObject ��������: �������ά��1��hash_table/map ?

	//�����ƣ�
	
//}

/*
��һ��· ����ShareMemory���Ѿ�������
�������ڰ�ȫ�� �� �޷��л�����һ������ִ�д���
�������Ӷȹ��ߣ���Ҫ������������SM�¹����Ķ������ݽṹ���о�������


������· ��windowsѧϰ���ṩ�ں�̬API
*��Ӷ���:
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
   //1ֱ��switch��event_nod.pid���̣�Ȼ��ǿ��jump���ɾ����жϣ�һ���user-mode����ͨ�������߱��жϰ�ȫ��
   //2 postmsg(event_nod.pid) 
}

*����
objlist = index.select(drawRect)
result = ""
for_each(uiobject in objlist)
{
	result += getfullscript(uiobject.getbasescript(),uiobject.getparmers())
}
return result;

*��user-mode����tree��Ч�ʺܵͣ�

�ں�ģʽȱ��: fire eventһ��Ҫ��postmsg
uiobject���ں˷���,���з��ʶ�Ҫ�����ںˣ���ɵ�system_call���ࣨ(�ܲ������owenr pid���ʵ��ٶȣ���
һ�����ֱ�������������
û���ں�ģ���ʵ�ʿ������飬Ҳ��������

===============================================================

�ڶ���· ����rpc+proxy object
���ڳ���
Ŀǰʹ�õ���ͬ��rpc���������� (server ����һ�����ܵȴ�client���̣�serverһ��ס��ȫ����)
�����api���ĳ��첽�ģ�rpc�ͻ�ܺ��� �������˺ܶ����״̬�� ����ĺ�ϵͳ�������׳����������������ȱ���ǣ�
����ģʽ�����¼�������ֻ�ܲ���postmsg�ķ�������һЩ�¼������� ʱ���� �� ����˳�������Եģ��᲻�ʺϣ���Ҫ���Ƚϻ�ɬ�ĸ��죨preevet,���е���Ӧ�߶�ͶƱ��ʾ�Լ���handle��֮���ٿ�ʼ�����Ĵ�����̣��൱���ӣ�
rpcģʽ�����ǽ�����xserver,һ������ʵ����һ̨�����ϲ�����һ̨�����Ľ��棨�����첽����
*root tree server��Ĳ���uiobject������һ��������û�б�����̷���֮ǰ������?�� ����ֻ�������䴴������?�����Ż������������ʵ��ô?
��������Ż�ʧЧ�������,�����ñ��ض�������Ż�:��һ����Ϣ���������(һ��msg frame)���Զ����һ�η��ʻᴥ�����ö��󴴽�һ�����գ������msg frame���ٴη��ʸö����Ƿ��ʸÿ��գ�Ҳ�����ֶ����� ��ÿ��գ��ٴӿ�����������


void AddChild(hparent,pObject)
{
   if (getownerpid(hparent) == pid())
   {
	   //decode ����������̵� uiobject
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

	client��server rpc�ǿ��׵ģ���Ϊ��ȷ��server�����ܱ�֤������Ӧ����Ҫ��server�˵Ĵ��벻�����κ���Ϊclient�����⿨ס�Ŀ���
	server���ܷ���ȴ�client��ĳ��message�Ĵ���������˵
	
}



void OnAddChildRequest(hparent,hobject)
{
    node = get_node(hparent);
	node.childs.add(hobject);
	get_obj_index().updateIndex(hobject);
}
 
void OnResizeObjectRequest(hobject,newRect)
{
	_index.updateObjectChanged(hobject,newRect) //��������Ķ���λ�������decode������UIObject���λ�û᲻ͬ��?
	postmessage(hobject.ownerthread,MSG_ONSIZE,newRect)//֪ͨhobject���ڽ��̸����ڲ�����
	
	//��SendMessage����ͬ�� ������sendMessageҪ�Է���������Ϣ���Է��ڴ�����Ϣʱ�п��ܻᷢ��rpc�����ǾͿ�����
}

//������rpc roottree������µ��¼�����
//һ�����������һ��������¼�������child����parent��onsize�¼�
void AttachListener(hobject,callback|usermsg)
{
	//������ͬһ������
	//��������һ������
	//��û��ͬ���¼��� ���Լ򵥵�Լ��ͬ���¼���obj-listener������ͬһ��������

	//һ�ִ���
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


	//����ڽ���pid3��ͬʱ����objA,objC��OnSize�¼�����ô�������¼��ĵ���˳����޷���֤�� ����ν

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


//�õ���һ�����̶���� �Զ������ԡ���ͨ����
string GetAttr(hobj,attrname)
{
	proxy = CreateProxy(getownerpid(hobj))
	// rpcall�ķ�����Ҫ�Է������ǻ�ģ�����Է����̿�������ô�ò���Ҳ�Ῠ�������Խ���ʹ��
	return proxy.rpccall("getattr",attrname)
}

//������һ�����̶�����Զ��庯��


//������һ�����̶�����Զ����¼�


//����fire�Զ����¼�

//��Ⱦʱ ������ȡUIObject��render script
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
//ǿ���ͷ����٣�ֻ��RootObjTree�Ĵ������̲����ʸ�
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
//�õ�root ui object,���objectһ����һ����id��layoutobject,�ڴ���objtree��ʱ����objtree����
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

