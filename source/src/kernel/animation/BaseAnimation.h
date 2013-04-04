//
// BaseAnimation.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_BASE_ANIMATION_H_
#define _NGOS_BASE_ANIMATION_H_

typedef struct tagBaseAnimation
{
	uint32_t Duration;//in ms
}BaseAnimation;

int ReleaseAnimation(BaseAnimation* pBaseAnimation);

int AnimationUpdate(BaseAnimation* pBaseAnimation);

int AnimationStart(BaseAnimation* pBaseAnimation);
int AnimationEnd(BaseAnimation* pBaseAnimation);
BOOL IsAnimationLoop(BaseAnimation* pBaseAnimation);
uint32_t GetAnimationLoopCount(BaseAnimation* pBaseAnimation);
int AnimationFinish(BaseAnimation* pBaseAnimation);
int AnimationPause(BaseAnimation* pBaseAnimation);
int AnimationResume(BaseAnimation* pBaseAnimation);

uint32_t AnimationGetDuration(BaseAnimation* pBaseAnimation);
uint32_t AnimationGetTotalTime(BaseAnimation* pBaseAnimation);


//��Ӷ�����ͬ��������ͬ�������붯������һ����"����"
int AddSyncAnimation(BaseAnimation* pAnimation,NGOS_ANIMATION_HANDLE hSyncAnimation);
//��Ӹ��涯�����ڶ�������finish״̬ʱ������ø��涯����start
int AddFollowAnimation(BaseAnimation* pAnimation,NGOS_ANIMATION_HANDLE* hFollowAnimation);


typedef struct tagAnimatinManager
{

}AnimationManager;

int AnimationManagerAddAnimation(NGOS_ANIMATION_HANDLE hAnimation);



#endif