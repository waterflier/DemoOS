//
// BaseHeader.h 
//
// Create:liuzhicong,2013-3-9
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_BASEHEADER_H_
#define _NGOS_BASEHEADER_H_

#ifdef __cplusplus
extern "C"
{
#endif 

typedef struct tagCommonObjectHeader
{
	int32_t RefCount;
	RootTreeEnv* Env;
}CommonObjectHeader;

#ifdef __cplusplus
}
#endif 

#endif //_NGOS_BASEHEADER_H_