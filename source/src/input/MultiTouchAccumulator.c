//
// MultiTouchAccumulator.c
//
///////////////////////////////////////
#include <stdint.h>
#include "./MultiTouchAccumulator.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

MultiTouchAccumulator* CreateMultiTouchAccumulator()
{
	MultiTouchAccumulator* pResult = NULL;
	pResult = (MultiTouchAccumulator*) malloc(sizeof(MultiTouchAccumulator));
	memset(pResult,sizeof(MultiTouchAccumulator),0);
	return pResult;
}

int ReleaseMultiTouchAccumulator(MultiTouchAccumulator* pSelf)
{

}
void ReleaseActionData(MTEventData* pData)
{
	pData->RefCount --;
	if(pData->RefCount = 0)
	{
		free(pData);
	}
}

static MTEventSolt* GetCurrentSolt(MultiTouchAccumulator* pSelf)
{
	if(pSelf->soltCount == 0)
	{
		pSelf->soltCount++;
	}

	return pSelf->currentSolts + (pSelf->soltCount - 1);
}

static void AppendSolt(MultiTouchAccumulator* pSelf)
{
	if(pSelf->soltCount == 0)
		return;
	
	pSelf->soltCount++;
}

static MultiTouchAction* GetCurrentAction(MultiTouchAccumulator* pSelf)
{
	return pSelf->resultData+pSelf->resultEndPos;
}

static void SetLastSoltData(MultiTouchAccumulator* pSelf,MTEventData* pData)
{
	if(pSelf->pLastData)
	{
		ReleaseActionData(pSelf->pLastData);
	}
	
	pSelf->pLastData = pData;
	if(pData)
	{
		pData->RefCount ++;
	}
	
}
//TODO : Reing buffer my error?
static void AppendAction(MultiTouchAccumulator* pSelf)
{
	pSelf->resultEndPos++;
	if(pSelf->resultEndPos == MT_ACTION_MAX_CACHE)
	{
		pSelf->resultEndPos = 0;
	}
	//if(pSelf->resultEndPos == pSelf)
}
static  void CovertSoltDataToAction(MultiTouchAccumulator* pSelf)
{
	//have enough action cache? 
	if(pSelf->resultHeaderPos < pSelf->resultEndPos)
	{
		if(pSelf->resultEndPos == pSelf->resultHeaderPos)
		{
			if(pSelf->resultEndPos == 0)
			{
				printf("Action Cache full!\n");
				return ;
			}	
		}
	}


	//compare current solt data and last solt data
	MTEventData* pLastData = pSelf->pLastData;
	if(pLastData == NULL)
	{
		if(pSelf->soltCount > 0)
		{
			printf("First Down\n");
			MultiTouchAction* pWrite = GetCurrentAction(pSelf);
			pWrite->actionType = NGOS_ACTION_TOUCH_DOWN;
			pWrite->x = pSelf->currentSolts->x;
			pWrite->y = pSelf->currentSolts->y;
			pWrite->pData =(MTEventData*) malloc (sizeof(MTEventData) + sizeof(MTEventSolt)*(pSelf->soltCount-1));
			pWrite->pData->RefCount = 1;
			pWrite->pData->FingerSoltCount = pSelf->soltCount;
			memcpy(&(pWrite->pData->Solts),pSelf->currentSolts,sizeof(MTEventSolt)*(pSelf->soltCount));
			SetLastSoltData(pSelf,pWrite->pData);

			AppendAction(pSelf);
		}
	}
	else if(pSelf->soltCount == 0)
	{
		
		if(pLastData)
		{
			printf("UP \n");
			MultiTouchAction* pWrite = GetCurrentAction(pSelf);
			pWrite->actionType = NGOS_ACTION_TOUCH_UP;	
			pWrite->x = pLastData->Solts.x;
			pWrite->y = pLastData->Solts.y;
			pWrite->pData = NULL;
			AppendAction(pSelf);
		}	

		SetLastSoltData(pSelf,NULL);
		
	}
	else
	{
		int i=0;
		int j=0;
		int isFind = 0;
		for(i=0;i<pSelf->soltCount;++i)
		{
			isFind = 0;
			MTEventSolt* pNow = pSelf->currentSolts+i;
			for(j=0;j<pLastData->FingerSoltCount;++j)
			{
				MTEventSolt* pComp = &(pLastData->Solts)+j;
				if(pComp->trackerID == pNow->trackerID)
				{
					isFind = 1;
					//move action
					printf("MOVE\n");
					break;
				}

			}

			if(!isFind)
			{
				//new down action
				printf("UP Solt\n");
			}
		}

		MTEventData* pTempData =(MTEventData*) malloc (sizeof(MTEventData) + sizeof(MTEventSolt)*(pSelf->soltCount-1));
		pTempData->RefCount = 1;
		pTempData->FingerSoltCount = pSelf->soltCount;
		memcpy(&(pTempData->Solts),pSelf->currentSolts,sizeof(MTEventSolt)*(pSelf->soltCount));
		SetLastSoltData(pSelf,pTempData);
	}
	
}

void MultiTouchAccumulatorPushRawInputEvent(MultiTouchAccumulator* pSelf, const struct input_event* pEV)
{
	//printf("MultiTouchAccumulatorPushRawInputEvent\n");
	if(pEV->type == EV_SYN)
	{
		switch(pEV->code)
		{
		case SYN_MT_REPORT:
				//OK a full finger slot
				AppendSolt(pSelf);
			break;
		case SYN_REPORT:
				//OK ,heare is a full action ,compare with last solt ,then create new action
				CovertSoltDataToAction(pSelf);
				pSelf->soltCount = 0;
			break;
		}
	}
	else if(pEV->type == EV_ABS)
	{
		MTEventSolt* pSolt = GetCurrentSolt(pSelf);
		switch(pEV->code)
		{
		case ABS_MT_POSITION_X:
			pSolt->x = pEV->value;
			break;
		case ABS_MT_POSITION_Y:
			pSolt->y = pEV->value;
			break;
		case ABS_MT_TOUCH_MAJOR:
			pSolt->major = pEV->value;
			break;
		case ABS_MT_TRACKING_ID:
			pSolt->trackerID = pEV->value;
			break;
		}
	}
	return;
}

int MultiTouchAccumulatorPopAction(MultiTouchAccumulator* pSelf,MultiTouchAction* pResult)
{
	if(pSelf->resultHeaderPos == pSelf->resultEndPos)
	{
		return 0;
	}
	
	memcpy(pResult,pSelf->resultData+pSelf->resultHeaderPos,sizeof(MultiTouchAction));
	pSelf->resultHeaderPos ++ ;
	if(pSelf->resultHeaderPos == MT_ACTION_MAX_CACHE)
	{
		pSelf->resultHeaderPos = 0;
	}
}