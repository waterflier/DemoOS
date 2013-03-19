//
// NGOS_Types.h 
//
// Create:liuzhicong,2013-3-12
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_TYPES_H_
#define _NGOS_TYPES_H_

#include <stdint.h>

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

}Matrix3X2;

#ifndef BOOL
#define BOOL int
#define TRUE (1)
#define FALSE (0)
#endif

typedef uint32_t TYPE_NGOS_PID;
typedef uint32_t TYPE_NGOS_TID;

typedef void* TYPE_NGOS_SHAREMEMORY;
typedef void* TYPE_NGOS_MUTEX;
typedef void* TYPE_NGOS_MSGQUEUE_RUNLOOP;
typedef void* TYPE_NGOS_MSG_RECIVER;
typedef void* TYPE_NGOS_MSG_PARAM;
typedef void* TYPE_NGOS_MSG_DATA;


typedef void* NGOS_ROOT_OBJTREE_HANDLE;
typedef struct
{
	char * szHostname;

}TYPE_NGOS_OBJTREE_OWNERHOST_INFO;

typedef void* NGOS_UIOBJECT_HANDLE;
typedef void* TYPE_NGOS_ID;
typedef void* NGOS_UIOBJECT_LOOKUP_ITERATOR;

typedef void* NGOS_ANIMATION_HANDLE;

//所有的ud都要类似这个结构
typedef struct tagUserDataContext
{
	void* UserData;
	int (*fnReleaseUserData) (void* pUserData);
	int (*fnAddRefReleaseUserData) (void* pUserData);
}UserDataContext;

#endif //_NGOS_TYPES_H_