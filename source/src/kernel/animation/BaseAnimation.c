//
// BaseAnimation.c 
//
//////////////////////////////////////////////////////////////////////////

#include "../perheader.h"
#include "../objectIndex/AnimationVector.h"
#include "./BaseAnimation.h"
#include "../../os_interface/os_interface.h"


static float AnimationGetProgress(BaseAnimation* pBaseAnimation);

BaseAnimation* MallocAnimation(size_t userDataLength)
{
    size_t animationSize = sizeof(BaseAnimation) + userDataLength;
    BaseAnimation* pResult = malloc(animationSize);
    memset(pResult,0,animationSize);

    return pResult;
}
void FreeAnimation(BaseAnimation* pAnimation)
{
    if(pAnimation)
    {
        free(pAnimation);
    }
}
int AddRefAnimation(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        pBaseAnimation->RefCount++;
    }
    return pBaseAnimation->RefCount;
}

int ReleaseAnimation(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        pBaseAnimation->RefCount--;
        if(pBaseAnimation->RefCount == 0)
        {
            FreeAnimation(pBaseAnimation);
        }
    }
}

void* GetAnimationUserData(BaseAnimation* pAnimation)
{
    unsigned char* pData = (unsigned char*) pAnimation;
    pData += sizeof(BaseAnimation);
    return (void*) pData;
}

static void ChangeAnimationState(BaseAnimation* pBaseAnimation,uint8_t newState)
{
    if(pBaseAnimation->State != newState)
    {
        pBaseAnimation->State = newState;
        //todo :fire event
    }

    return;
}

void AnimationUpdate(BaseAnimation* pBaseAnimation,uint32_t currentClockTick)
{
    if(pBaseAnimation)
    {
        if(pBaseAnimation->State == ANIMATION_STATE_RUNNING)
        {
            if(pBaseAnimation->m_lastUpdateTime == 0)
            {
                pBaseAnimation->m_lastUpdateTime = currentClockTick;
            }
            else
            {
                pBaseAnimation->Duration += (currentClockTick - pBaseAnimation->m_lastUpdateTime);
                pBaseAnimation->m_lastUpdateTime = currentClockTick;
            }

            if(pBaseAnimation->TotalTime != 0)
            {
                if(pBaseAnimation->Duration >= pBaseAnimation->TotalTime)
                {
                    pBaseAnimation->Duration = pBaseAnimation->TotalTime;
                }
            }

            if(pBaseAnimation->Level > pBaseAnimation->m_createLevel)
            {
                if(!pBaseAnimation->IsLoop)
                {
                    //立刻执行最后一帧
                    //对Loop 特殊处理，否则的话会很奇怪
                    pBaseAnimation->Duration = pBaseAnimation->TotalTime;
                }
            }

            float progress = AnimationGetProgress(pBaseAnimation);
            printf("%f %d:%d\n",progress,pBaseAnimation->Duration,pBaseAnimation->TotalTime);
            if(pBaseAnimation->pImp)
            {
                pBaseAnimation->pImp->fnAction(pBaseAnimation,progress);
            }

            if(pBaseAnimation->Duration == pBaseAnimation->TotalTime)
            {
                AnimationEnd(pBaseAnimation);
            }
        }
    }

}



int AnimationStart(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        if(pBaseAnimation->State == ANIMATIN_STATE_READY)
        {
            pBaseAnimation->m_lastUpdateTime = 0;
            pBaseAnimation->Duration = 0;
            ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_RUNNING);	
        }
    }
}

int AnimationEnd(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        if(pBaseAnimation->IsLoop)
        {
            ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_END);
            pBaseAnimation->LoopCount++;
            if(pBaseAnimation->TotalLoopCount != 0)
            {
                if(pBaseAnimation->LoopCount >= pBaseAnimation->TotalLoopCount)
                {
                    ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_FINISH);
                }
            }

            pBaseAnimation->Duration = 0;
            ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_RUNNING);	
        }
        else
        {
            ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_FINISH);
        }
    }
}
void AnimationSetLoop(BaseAnimation* pBaseAnimation,BOOL isLoop,uint16_t totalLoopCount)
{
    if(pBaseAnimation)
    {
        pBaseAnimation->IsLoop = isLoop;
        pBaseAnimation->TotalLoopCount = totalLoopCount;
    }
}

BOOL IsAnimationLoop(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        return pBaseAnimation->IsLoop;
    }

    return FALSE;
}

uint32_t GetAnimationLoopCount(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        return pBaseAnimation->LoopCount;
    }

    return 0;
}

int AnimationFinish(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        if(pBaseAnimation->State == ANIMATION_STATE_RUNNING || pBaseAnimation->State == ANIMATION_STATE_PAUSE)
        {
            ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_FINISH);
        }
        return 0;
    }

    return -1;
}

int AnimationPause(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        if(pBaseAnimation->State == ANIMATION_STATE_RUNNING)
        {
            pBaseAnimation->m_lastUpdateTime = 0;

            ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_PAUSE);
            return 0;
        }
    }

    return -1;
}

int AnimationResume(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        if(pBaseAnimation->State == ANIMATION_STATE_PAUSE)
        {
            pBaseAnimation->m_lastUpdateTime = 0;

            ChangeAnimationState(pBaseAnimation,ANIMATION_STATE_RUNNING);
        }
    }

    return -1;
}

uint32_t AnimationGetDuration(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        return pBaseAnimation->Duration;
    }

    return 0;
}

uint32_t AnimationGetTotalTime(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        return pBaseAnimation->TotalTime;
    }

    return 0;
}

void AnimationSetTotalTime(BaseAnimation* pBaseAnimation,uint32_t totalTime)
{
    if(pBaseAnimation)
    {
        pBaseAnimation->TotalTime = totalTime;
    }
}

void AnimationBindUIObject(BaseAnimation* pBaseAnimation,NGOS_UIOBJECT_HANDLE hUIObject)
{
    if(pBaseAnimation)
    {
        pBaseAnimation->hBindObj = hUIObject;
    }
}
static float AnimationGetProgress(BaseAnimation* pBaseAnimation)
{
    if(pBaseAnimation)
    {
        if(pBaseAnimation->TotalTime == 0)
        {
            return pBaseAnimation->Duration;
        }

        if(pBaseAnimation->pCurve)
        {
            //TODO:加入对Curve的支持
            return ((float)pBaseAnimation->Duration / (float)pBaseAnimation->TotalTime);
        }
        else
        {
            float x = ((float)pBaseAnimation->Duration / (float)pBaseAnimation->TotalTime);
            if(x<0.5f)
            {
                return 2*x*x;
            }
            else
            {
                return 1-2*(x-1)*(x-1);
            }
            return ;
        }
    }
}

//添加动画的同步动画，同步动画与动画保持一样的"节奏"
int AddSyncAnimation(BaseAnimation* pAnimation,NGOS_ANIMATION_HANDLE hSyncAnimation)
{
    return 0;
}
//添加跟随动画，在动画到达finish状态时，会调用跟随动画的start
int AddFollowAnimation(BaseAnimation* pAnimation,NGOS_ANIMATION_HANDLE* hFollowAnimation)
{
    return 0;
}


static AnimationManager* s_pAnimationManager = NULL;
AnimationManager* GetAnimationManager()
{
    if(s_pAnimationManager == NULL)
    {
        s_pAnimationManager = (AnimationManager*) malloc(sizeof(AnimationManager));
        s_pAnimationManager->willUpdateAnimation = CreateAnimationVector(16);
    }

    return s_pAnimationManager;
}

int AnimationManagerAddAnimation(AnimationManager* pSelf,NGOS_ANIMATION_HANDLE hAnimation)
{
    BaseAnimation* pAni = HandleMapDecodeAnimation(hAnimation);
    if(pAni)
    {
        if(pAni->pOwnerManager == NULL)
        {
            AnimationVectorAdd(pSelf->willUpdateAnimation,hAnimation);
            pAni->pOwnerManager = pSelf;
            AddRefAnimation(pAni);

            return 0;
        }
    }

    return -1;
}

void AnimationManagerUpdateAnimation(AnimationManager* pSelf)
{
    int count = AnimationVectorGetCount(pSelf->willUpdateAnimation);
    int i;
    for(i=count-1;i>=0;--i)
    {
        NGOS_ANIMATION_HANDLE hAni = AnimationVectorGet(pSelf->willUpdateAnimation,i);
        BaseAnimation* pAni = HandleMapDecodeAnimation(hAni);

        if(pAni)
        {
            AnimationUpdate(pAni,OSI_GetTickCount());
            if(pAni->State == ANIMATION_STATE_FINISH)
            {
                ReleaseAnimation(hAni);
                AnimationVectorRemoveAt(pSelf->willUpdateAnimation,i);

                i++;
            }
        }
    }
}