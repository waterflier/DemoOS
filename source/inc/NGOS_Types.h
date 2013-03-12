//
// NGOS_Types.h 
//
// Create:liuzhicong,2013-3-12
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_TYPES_H_
#define _NGOS_TYPES_H_

#include <stdint.h>

typedef struct
{
	int32_t    left;
	int32_t    top;
	int32_t    right;
	int32_t    bottom;
} RECT;

typedef uint32_t TYPE_NGOS_PID;
typedef void* NGOS_ROOT_OBJTREE_HANDLE;
typedef struct
{
	char * szHostname;

}TYPE_NGOS_OBJTREE_OWNERHOST_INFO;

typedef void* NGOS_UIOBJECT_HANDLE;
typedef void* TYPE_NGOS_ID;
typedef void* NGOS_UIOBJECT_LOOKUP_ITERATOR;

typedef void* NGOS_ANIMATION_HANDLE;

#endif //_NGOS_TYPES_H_