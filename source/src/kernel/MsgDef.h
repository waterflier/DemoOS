#ifndef _NGOS_KERNEL_MSG_DEF_H_
#define _NGOS_KERNEL_MSG_DEF_H_


//client->server 的调用demo阶段是同步的，以后可以考虑支持异步接口 

//no return 添加对象 parm1 = obj_handle
#define MSG_ADD_OBJ    (128)
//error notify 添加对象出错 param1 = obj_handle
#define MSG_ADD_OBJ_ERROR (129)
//not return 删除对象 parm1 = obj_handle
#define MSG_REMOVE_OBJ (130)
//error notify 删除对象出错 param1 = obj_handle
#define MSG_REMOVE_OBJ_ERROR (131)

//sync 得到对象的信息 parm1=obj_handle,parm2=info_index,
#define MSG_GET_OBJINFO (132)

//not return 更新对象的信息 param1=obj_handle,param2=info_index
#define MSG_UPDATE_OBJINFO (134)
//error notify 更新对象的信息出错
#define MSG_UPDATE_OBJINFO_ERROR (135)

#define MSG_FIND_OBJ (256)


#define  MSG_USER (1000)


#endif //_NGOS_KERNEL_MSG_DEF_H_