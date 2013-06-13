//
// InputReader.h
//
//////////////////////////////////


#ifndef _NGOS_INPUT_READERH_H_
#define _NGOS_INPUT_READERH_H_

#include <linux/input.h>
#include <sys/epoll.h>

#include "./MultiTouchAccumulator.h"

#define EPOLL_MAX_EVENTS (16)
#define EPOLL_SIZE_HINT (8)
#define RAW_INPUT_EVENT_READ_LEN (64)

typedef struct tagInputReader
{
	int State;

	//logic device fd
	int multiTouchFD;
	//epoll wait fd
	int epollFD;
	struct epoll_event pendingEvent[EPOLL_MAX_EVENTS];
	int pendingEventCount;

	//accumulator
	MultiTouchAccumulator* pMTAccumulator;

}InputReader;

InputReader* CreateInputReader();
int ReleaseInputReader(InputReader* pReader);

//void SetInputReadeActionNotify 
int InputReaderStart(InputReader* pSelf);


#endif //_NGOS_INPUT_READERH_H_