#ifndef _NGOS_KERNEL_MSG_DEF_H_
#define _NGOS_KERNEL_MSG_DEF_H_


//client->server �ĵ���demo�׶���ͬ���ģ��Ժ���Կ���֧���첽�ӿ� 

//no return ��Ӷ��� parm1 = obj_handle
#define MSG_ADD_OBJ    (128)
//error notify ��Ӷ������ param1 = obj_handle
#define MSG_ADD_OBJ_ERROR (129)
//not return ɾ������ parm1 = obj_handle
#define MSG_REMOVE_OBJ (130)
//error notify ɾ��������� param1 = obj_handle
#define MSG_REMOVE_OBJ_ERROR (131)

//sync �õ��������Ϣ parm1=obj_handle,parm2=info_index,
#define MSG_GET_OBJINFO (132)

//not return ���¶������Ϣ param1=obj_handle,param2=info_index
#define MSG_UPDATE_OBJINFO (134)
//error notify ���¶������Ϣ����
#define MSG_UPDATE_OBJINFO_ERROR (135)

#define MSG_FIND_OBJ (256)


#define  MSG_USER (1000)


#endif //_NGOS_KERNEL_MSG_DEF_H_