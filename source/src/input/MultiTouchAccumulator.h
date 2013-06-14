//
// MultiTouchAccumlator.h
//
///////////////////////////////////////////////////

#ifndef _NGOS_MULTI_TOUCH_ACCUMLATOR_H_
#define _NGOS_MULTI_TOUCH_ACCUMLATOR_H_

#include <linux/input.h>
#include "../kernel/input/inputAction.h"

#define MT_ACTION_MAX_CACHE (256)
#define MT_SOLT_MAX_CACHE (16)

typedef struct tagMultiTouchAction
{
	uint16_t actionType;
	uint16_t x;
	uint16_t y;
	MTEventData* pData;
}MultiTouchAction;

typedef struct tagMultiTouchAccumulator
{
	int multiTouchProtocolType;//0 no solt:1 use solt

	MTEventSolt currentSolts[MT_SOLT_MAX_CACHE];
	uint16_t soltCount;
	int soltInUse;
	//struct input_event 
	MultiTouchAction resultData[MT_ACTION_MAX_CACHE]; //ring buffer
	uint16_t resultHeaderPos;
	uint16_t resultEndPos;
	uint16_t nowWritePos;
	MTEventData* pLastData;
}MultiTouchAccumulator;

MultiTouchAccumulator* CreateMultiTouchAccumulator();
int ReleaseMultiTouchAccumulator(MultiTouchAccumulator* pSelf);

void MultiTouchAccumulatorPushRawInputEvent(MultiTouchAccumulator* pSelf, const struct input_event* pEV);
int MultiTouchAccumulatorPopAction(MultiTouchAccumulator* pSelf,MultiTouchAction* pResult);

//TODO :
MTEventData* CreateMTEventData(int soltSize);
void ReleaseActionData(MTEventData* pData);

#endif //_NGOS_MULTI_TOUCH_ACCUMLATOR_H_