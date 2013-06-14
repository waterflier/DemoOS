//
// InputAction.c
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "./InputAction.h"

void ReleaseMTEventData(MTEventData* pData)
{
    pData->RefCount --;
    if(pData->RefCount = 0)
    {
        free(pData);
    }
}
MTEventData* CreateMTEventData(int soltSize)
{
    MTEventData* pResult = (MTEventData*) malloc (sizeof(MTEventData) + sizeof(MTEventSolt)*(soltSize-1));
    pResult->RefCount = 1;
    pResult->FingerSoltCount = soltSize;
    return pResult;
}

int AddRefMTEventData(MTEventData* pData)
{
    pData->RefCount++;
    return pData->RefCount;
}

MTEventSolt* GetFingerSoltData(MTEventData* pData,uint16_t index)
{
    MTEventSolt* pStart = &(pData->Solts);
    return pStart + index;
}