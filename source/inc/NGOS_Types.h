//
// NGOS_Types.h 
//
// Create:liuzhicong,2013-3-12
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_TYPES_H_
#define _NGOS_TYPES_H_

#include <stdint.h>

#ifndef BOOL
#define BOOL int
#define TRUE (1)
#define FALSE (0)
#endif


typedef struct tagPoint
{
	int32_t X;
	int32_t Y;
}POINT;

typedef struct tagRect
{
	int32_t    left;
	int32_t    top;
	int32_t    right;
	int32_t    bottom;
} RECT;

typedef struct tagRealRect
{
	float left;
	float top;
	float right;
	float bottom;
}RealRECT;

typedef struct tagMatrix3X2
{
	float v1;
	float v2;
}Matrix3X2;


#ifdef __linux
#include <pthread.h>
#include <semaphore.h>
typedef pid_t TYPE_NGOS_PID;
typedef uint32_t TYPE_NGOS_TID;
typedef pthread_mutex_t TYPE_NGOS_MUTEX;
typedef sem_t TYPE_NGOS_SEMAPHORE;
typedef void* TYPE_NGOS_SHAREMEMORY;
typedef void* TYPE_NGOS_MSG_RECIVER;
#endif



typedef void* TYPE_NGOS_MSGQUEUE_RUNLOOP;
typedef uint32_t TYPE_NGOS_MSG_PARAM;
typedef void* TYPE_NGOS_MSG_DATA;


typedef struct
{
	char * szHostname;

}TYPE_NGOS_OBJTREE_OWNERHOST_INFO;

typedef enum 
{
	NGOS_ENTITY_TYPE_TREE = 0,
	NGOS_ENTITY_TYPE_UIOBJ =1
}NGOS_Enum_EntityObjectType;

typedef struct tagNGOS_RootTreeEnv
{
	void* AllocUD;
	void* (*fnAlloc) (uint8_t type,void* ud,void* ptr,size_t newSize,size_t oldSize);
}NGOS_RootTreeEnv;

typedef uint32_t NGOS_HANDLE;
typedef uint32_t NGOS_UIOBJECT_HANDLE;
typedef uint32_t NGOS_ROOT_OBJTREE_HANDLE;
typedef void*    NGOS_UIOBJECT_LOOKUP_ITERATOR;

typedef void* NGOS_ANIMATION_HANDLE;
typedef void* NGRE_SCRIPT_HANDLE;

//所有的ud都要类似这个结构
typedef struct tagUserDataContext
{
	void* UserData;
	int (*fnReleaseUserData) (void* pUserData);
	int (*fnAddRefUserData) (void* pUserData);
}UserDataContext;

#endif //_NGOS_TYPES_H_