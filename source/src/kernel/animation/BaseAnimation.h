//
// BaseAnimation.h 
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_BASE_ANIMATION_H_
#define _NGOS_BASE_ANIMATION_H_


#define ANIMATIN_STATE_READY (0)
#define ANIMATION_STATE_RUNNING (1)
#define ANIMATION_STATE_PAUSE (2)
#define ANIMATION_STATE_END (3)
#define ANIMATION_STATE_FINISH (4)

typedef struct tagAnimationTypeInfo
{
	
}AnimationTypeInfo;

typedef struct 
{
	//--生命周期与类型信息
	int32_t RefCount;
	uint32_t TagFlags;//typeinfo?
	struct AnimationManager* pOwnerManager;
	NGOS_UIOBJECT_HANDLE hBindObj;
	//--运行状态控制
	uint8_t State;
	uint8_t IsLoop;
	uint8_t Level;
	uint16_t LoopCount;
	uint16_t TotalLoopCount;
	uint32_t LifeTime;//in ms
	uint32_t Duration;//in ms
	uint32_t TotalTime;//in ms
	void*	 pCurve;

	//--事件支持

	//动画关联
	struct BaseAnimation* pFollowAnimation;
	struct BaseAnimation* pSyncAnimation;

	//pravite
	uint32_t m_lastUpdateTime;
	uint8_t m_createLevel;
}BaseAnimation;


BaseAnimation* MallocAnimation(size_t userDataLength);
void FreeAnimation(BaseAnimation* pAnimation);

int AddRefAnimation(BaseAnimation* pBaseAnimation);
int ReleaseAnimation(BaseAnimation* pBaseAnimation);

void AnimationUpdate(BaseAnimation* pBaseAnimation,uint32_t currentClockTick);

int AnimationStart(BaseAnimation* pBaseAnimation);
int AnimationEnd(BaseAnimation* pBaseAnimation);
void AnimationSetLoop(BaseAnimation* pBaseAnimation,BOOL isLoop,uint16_t totalLoopCount);
BOOL IsAnimationLoop(BaseAnimation* pBaseAnimation);
uint32_t GetAnimationLoopCount(BaseAnimation* pBaseAnimation);
int AnimationFinish(BaseAnimation* pBaseAnimation);
int AnimationPause(BaseAnimation* pBaseAnimation);
int AnimationResume(BaseAnimation* pBaseAnimation);

uint32_t AnimationGetDuration(BaseAnimation* pBaseAnimation);
uint32_t AnimationGetTotalTime(BaseAnimation* pBaseAnimation);

void AnimationSetTotalTime(BaseAnimation* pBaseAnimation,uint32_t totalTime);
void AnimationBindUIObject(BaseAnimation* pBaseAnimation,NGOS_UIOBJECT_HANDLE hUIObject);
//void AnimationBindUIObjects();
//添加动画的同步动画，同步动画与动画保持一样的"节奏"
int AddSyncAnimation(BaseAnimation* pAnimation,NGOS_ANIMATION_HANDLE hSyncAnimation);
//添加跟随动画，在动画到达finish状态时，会调用跟随动画的start
int AddFollowAnimation(BaseAnimation* pAnimation,NGOS_ANIMATION_HANDLE* hFollowAnimation);



typedef struct tagAnimatinManager
{

}AnimationManager;

int AnimationManagerAddAnimation(NGOS_ANIMATION_HANDLE hAnimation);



#endif