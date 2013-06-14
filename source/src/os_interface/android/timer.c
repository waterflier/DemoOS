//
// timer.c 
//
//////////////////////////////////////////////////////////////////////////

#include <pthread.h>
#include <time.h>
#include <stdio.h>

#include "../os_interface.h"
#include "../utility.h"

uint32_t OSI_GetTickCount()
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}