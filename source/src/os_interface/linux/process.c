//
// process.c 
//
//////////////////////////////////////////////////////////////////////////

#include "./perheader.h"
#include "../os_interface.h"


TYPE_NGOS_PID OSI_GetPID()
{

	return getpid();
}

BOOL OSI_IsProcessExist(TYPE_NGOS_PID pid)
{
	if(kill(pid,0) == 0)
	{
		return TRUE;
	}

	return FALSE;
}