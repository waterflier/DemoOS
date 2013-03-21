//
// NGOS_Errors.h 
//
// Create:liuzhicong,2013-3-12
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_ERROR_H_
#define _NGOS_ERROR_H_

enum NGOS_Result
{
	NGOS_RESULT_SUCCESS = 0,
	NGOS_RESULT_INVALID_PTR = 1,


	NGOS_RESULT_OBJECT_BINDED = 32,
	NGOS_RESULT_OBJECT_DIFF_TREE = 33,
	NGOS_RESULT_NOT_MY_CHILD = 34
};


#endif //_NGOS_ERROR_H_
