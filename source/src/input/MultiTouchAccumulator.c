//
// MultiTouchAccumulator.c
//
///////////////////////////////////////
#include <stdint.h>
#include "./MultiTouchAccumulator.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define DEFAULT_PROTOCOL (2)

MultiTouchAccumulator* CreateMultiTouchAccumulator()
{
	MultiTouchAccumulator* pResult = NULL;
	pResult = (MultiTouchAccumulator*) malloc(sizeof(MultiTouchAccumulator));
	memset(pResult,0,sizeof(MultiTouchAccumulator));
    pResult->protocolType = DEFAULT_PROTOCOL;
	return pResult;
}

int ReleaseMultiTouchAccumulator(MultiTouchAccumulator* pSelf)
{
    //TODO:
}


static MTEventSolt* GetCurrentSolt(MultiTouchAccumulator* pSelf)
{
	if(pSelf->soltInUse == 0)
	{
		pSelf->soltCount++;
		pSelf->soltInUse = 1;
	}
	
	return pSelf->currentSolts + (pSelf->soltCount - 1);
}

static void AppendSolt(MultiTouchAccumulator* pSelf)
{
	if(pSelf->soltCount == 0)
		return;
	
	//pSelf->soltCount++;
	pSelf->soltInUse = 0;
}

static MultiTouchAction* GetCurrentAction(MultiTouchAccumulator* pSelf)
{
	return pSelf->resultData+pSelf->resultEndPos;
}

static void SetLastSoltData(MultiTouchAccumulator* pSelf,MTEventData* pData)
{
	if(pSelf->pLastData)
	{
		ReleaseMTEventData(pSelf->pLastData);
	}
	
	pSelf->pLastData = pData;
	if(pData)
	{
		AddRefMTEventData(pSelf->pLastData);
	}
	
}
//TODO : Reing buffer may error?
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

	MTEventData* pLastData = pSelf->pLastData;
	if(pLastData == NULL)
	{

		if(pSelf->soltCount > 0)
		{
			//printf("Down\n");
			MultiTouchAction* pWrite = GetCurrentAction(pSelf);
			pWrite->actionType = NGOS_ACTION_TOUCH_DOWN;
			pWrite->x = pSelf->currentSolts->x;
			pWrite->y = pSelf->currentSolts->y;
			pWrite->pData =CreateMTEventData(pSelf->soltCount);
			memcpy(&(pWrite->pData->Solts),pSelf->currentSolts,sizeof(MTEventSolt)*(pSelf->soltCount));
			
			AppendAction(pSelf);
            
            SetLastSoltData(pSelf,pWrite->pData);
            //printf("Down over\n");
		}
	}
	else if(pSelf->soltCount == 0)
	{
		if(pLastData)
		{
			//printf("UP\n");
			MultiTouchAction* pWrite = GetCurrentAction(pSelf);
			pWrite->actionType = NGOS_ACTION_TOUCH_UP;	
			pWrite->x = pLastData->Solts.x;
			pWrite->y = pLastData->Solts.y;
			pWrite->pData = CreateMTEventData(1);
            pWrite->pData->Solts = pLastData->Solts;

			AppendAction(pSelf);
		}	

		SetLastSoltData(pSelf,NULL);
		
	}
	else
	{
		int i=0;
		int j=0;
		int isFind = 0;

        //1)求当前soltdata与last soltdata的交集
        //  交集部分触发一次MOVE (1次MOVE 多个soltdata)
        //2) soltdata与交集的差 触发发多次NEW_DOWN
        //3) lastsoltdata与交集的差 触发多次NEW_UP

        int findcount = 0;
        MTEventData* pMovingData = CreateMTEventData(pSelf->soltCount);
        MTEventSolt* pWriteSolt = &(pMovingData->Solts);
        pMovingData->FingerSoltCount = 0;
        for(i=0;i<pSelf->soltCount;++i)
        {
            isFind = 0;
            MTEventSolt* pNow = pSelf->currentSolts+i;
            for(j=0;j<pLastData->FingerSoltCount;++j)
            {
                MTEventSolt* pComp = &(pLastData->Solts)+j;
                if(pComp->trackerID == pNow->trackerID)
                {
                	//printf("moving\n");
                    isFind = 1;
                    pComp->trackerID |= 0x1000;//mark find
                    findcount++;
                    *pWriteSolt = *pNow;
                    pWriteSolt++;
                    pMovingData->FingerSoltCount++;
                    break;
                }
            }

            if(!isFind)
            {
                //printf("one Down\n");
                MultiTouchAction* pOneDownAction = GetCurrentAction(pSelf);
                pOneDownAction->actionType = NGOS_ACTION_TOUCH_ONE_DOWN;	
                pOneDownAction->x = pNow->x;
                pOneDownAction->y = pNow->y;
                pOneDownAction->pData =CreateMTEventData(1);
                pOneDownAction->pData->Solts = *pNow;

                AppendAction(pSelf);
            }
        }

        MultiTouchAction* pMoveAction = GetCurrentAction(pSelf);
        pMoveAction->actionType = NGOS_ACTION_TOUCH_MOVE;	
        pMoveAction->x = pMovingData->Solts.x;
        pMoveAction->y =  pMovingData->Solts.y;
        pMoveAction->pData = pMovingData;

        AppendAction(pSelf);

        if(findcount != pLastData->FingerSoltCount)
        {
            for(j=0;j<pLastData->FingerSoltCount;++j)
            {
                MTEventSolt* pSolt = &(pLastData->Solts)+j;
                if(!(pSolt->trackerID & 0xF000))
                {
                    //printf("New UP");
                    MultiTouchAction* pOneUpAction = GetCurrentAction(pSelf);
                    pOneUpAction->actionType = NGOS_ACTION_TOUCH_ONE_UP;	
                    pOneUpAction->x = pSolt->x;
                    pOneUpAction->y = pSolt->y;
                    pOneUpAction->pData =CreateMTEventData(1);
                    pOneUpAction->pData->Solts = *pSolt;

                    AppendAction(pSelf);
                }
            }
        }


        MTEventData* pTempData = CreateMTEventData(pSelf->soltCount);
		memcpy(&(pTempData->Solts),pSelf->currentSolts,sizeof(MTEventSolt)*(pSelf->soltCount));
		SetLastSoltData(pSelf,pTempData);
	}
	
}




void MultiTouchAccumulatorPushRawInputEventProtocolA(MultiTouchAccumulator* pSelf, const struct input_event* pEV)
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
            pSelf->soltInUse = 0;
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

            if(pEV->value & 0xf000)
            {
                printf("**** error!\n");
            }
            pSolt->trackerID = pEV->value & 0x0fff;//high bit is use for compare

            break;
        }
    }
    return;
}

static  void CovertSoltDataToActionB(MultiTouchAccumulator* pSelf)
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

    if(1)
    {
        if(pSelf->empty == 0)
        {
            pSelf->empty = 1;//**notice here

            //fire down
            //printf("FIRST DOWN\n");
            pSelf->currentSolts[0].trackerID &= 0x0fff; 
            MultiTouchAction* pWrite = GetCurrentAction(pSelf);
            pWrite->actionType = NGOS_ACTION_TOUCH_DOWN;
            pWrite->x = pSelf->currentSolts->x;
            pWrite->y = pSelf->currentSolts->y;
            pWrite->pData =CreateMTEventData(pSelf->soltCount);
            memcpy(&(pWrite->pData->Solts),pSelf->currentSolts,sizeof(MTEventSolt)*(pSelf->soltCount));

            AppendAction(pSelf);
            pSelf->validSolt++;


        }
        else
        {
            int i;
            int j=0;

            MTEventData* pMovingData=CreateMTEventData(pSelf->soltCount);
            //printf("cache solt count=%d\n",pSelf->soltCount);
            for(i=0;i<pSelf->soltCount;++i)
            {
                MTEventSolt* pMovSolt = pSelf->currentSolts+i;
                //printf("pMovSolt trackerID=%x\n",pMovSolt->trackerID);
                if(pMovSolt->trackerID != 0xffff && pMovSolt->trackerID != 0x0fff)
                {
                    if(pMovSolt->trackerID & 0xf000)
                    {
                        //printf("ONE DOWN %x\n",pMovSolt->trackerID);
                        pMovSolt->trackerID &= 0x0fff;
                        MultiTouchAction* pOneDownAction = GetCurrentAction(pSelf);
                        pOneDownAction->actionType = NGOS_ACTION_TOUCH_ONE_DOWN;
                        pOneDownAction->x = pMovSolt->x;
                        pOneDownAction->y = pMovSolt->y;
                        pOneDownAction->pData =CreateMTEventData(1);
                        pOneDownAction->pData->Solts = *pMovSolt;

                        AppendAction(pSelf);
                        pSelf->validSolt++;
                    }
                    else
                    {
                        *(&(pMovingData->Solts)+j) = *pMovSolt;
                        j++;
                    }
                }
                else
                {
                  
                   if(pMovSolt->trackerID == 0xffff)
                   {
                       pSelf->validSolt--;
                       //printf("UP %x\n",pMovSolt->trackerID);
                       pMovSolt->trackerID = 0x0fff;
                        
                       MultiTouchAction* pOneUpAction = GetCurrentAction(pSelf);
                       pOneUpAction->x = pMovSolt->x;
                       pOneUpAction->y = pMovSolt->y;
                       pOneUpAction->pData =CreateMTEventData(1);
                       pOneUpAction->pData->Solts = *pMovSolt;
                        if(pSelf->validSolt == 0)
                       {
                           //printf("FINAL UP \n");
                           pOneUpAction->actionType = NGOS_ACTION_TOUCH_UP;
                           pSelf->empty = 0;//**notice here
                           //clean data
                           pSelf->soltCount = 0;
                           pSelf->soltInUse = 0;
                           pSelf->soltCurrentIndex = 0;
                           memset(pSelf->currentSolts,MT_SOLT_MAX_CACHE*sizeof(MTEventSolt),0);
                       }
                       else
                       {
                            //printf("ONE UP\n");
                           pOneUpAction->actionType = NGOS_ACTION_TOUCH_ONE_UP;	
                       }
                       AppendAction(pSelf);

                   }
     
                }
            }   

            if(j > 0)
            {
                //printf("MOVING\n");
                MultiTouchAction* pMoveAction = GetCurrentAction(pSelf);
                pMoveAction->actionType = NGOS_ACTION_TOUCH_MOVE;	
                pMoveAction->x = pMovingData->Solts.x;
                pMoveAction->y =  pMovingData->Solts.y;
                pMoveAction->pData = pMovingData;
                pMovingData->FingerSoltCount = j;

                AppendAction(pSelf);
            }
            else
            {
                ReleaseMTEventData(pMovingData);
            }
        }
    }

}

static void SetCurrentSlotIndex(MultiTouchAccumulator* pSelf,int newIndex)
{
    if(newIndex >= pSelf->soltCount-1)
    {
        pSelf->soltCount = newIndex+1;
    }
    pSelf->soltCurrentIndex = newIndex;
    //return pSelf->currentSolts + (pSelf->soltCount - 1);
}

static MTEventSolt* GetCurrentSoltByIndex(MultiTouchAccumulator* pSelf)
{
    if(pSelf->soltCount == 0)
    {
        pSelf->soltCount = 1;
    }
    return pSelf->currentSolts + (pSelf->soltCurrentIndex);
}

void MultiTouchAccumulatorPushRawInputEventProtocolB(MultiTouchAccumulator* pSelf, const struct input_event* pEV)
{
    //printf("MultiTouchAccumulatorPushRawInputEvent\n");
    if(pEV->type == EV_SYN)
    {
        switch(pEV->code)
        {
        case SYN_REPORT:
            //OK ,heare is a full action ,compare with last solt ,then create new action
            CovertSoltDataToActionB(pSelf);
            pSelf->soltCurrentIndex = 0;
            break;
        }
    }
    else if(pEV->type == EV_ABS)
    {
        MTEventSolt* pSolt = GetCurrentSoltByIndex(pSelf);
        switch(pEV->code)
        {
        case ABS_MT_SLOT:
            SetCurrentSlotIndex(pSelf,pEV->value);
            //AppendSolt(pSelf);
            break;
        case ABS_PRESSURE:
            //do nothing?
            break;
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
            pSolt->trackerID = pEV->value ;
            pSolt->trackerID |= 0xf000; 
    

            break;
        }
    }
    return;
}
void MultiTouchAccumulatorPushRawInputEvent(MultiTouchAccumulator* pSelf, const struct input_event* pEV)
{
	//printf("MultiTouchAccumulatorPushRawInputEvent\n");
    if(pSelf->protocolType == 0)
    {
        if(pEV->type == EV_ABS)
        {

            if(pEV->code == ABS_MT_SLOT)
            {
                printf("Use protocol B\n");
                pSelf->protocolType = 2;
            }
            if(pEV->code == ABS_MT_TRACKING_ID)
            {
                if(pEV->value == 0xffffffff)
                {
                    printf("Use protocol B\n");
                    pSelf->protocolType = 2;
                }
            }
        }
        else if(pEV->type == EV_SYN)
        {
            if(pEV->code == SYN_MT_REPORT)
            {
                printf("Use protocol A\n");
                pSelf->protocolType = 1;
            }
        }

    }
    else
    {
        if(pSelf->protocolType == 1)
        {
            //protocol A
            MultiTouchAccumulatorPushRawInputEventProtocolA(pSelf,pEV);
        }
        else
        {
            //protocol B
            MultiTouchAccumulatorPushRawInputEventProtocolB(pSelf,pEV);
        }
    }

}

int MultiTouchAccumulatorPopAction(MultiTouchAccumulator* pSelf,MultiTouchAction* pResult)
{
	if(pSelf->resultHeaderPos == pSelf->resultEndPos)
	{
		return 0;
	}
	
	//printf("PopAction");
	memcpy(pResult,pSelf->resultData+pSelf->resultHeaderPos,sizeof(MultiTouchAction));
	if(pResult->pData)
	{
		//TODO: maybe bug
        pResult->pData->RefCount = 1;
	}
	pSelf->resultHeaderPos ++ ;
	if(pSelf->resultHeaderPos == MT_ACTION_MAX_CACHE)
	{
		pSelf->resultHeaderPos = 0;
	}
}