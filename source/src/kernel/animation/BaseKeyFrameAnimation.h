//
// BaseKeyFrameAnimation.h 
//
//////////////////////////////////////////////////////////////////////////

#include "./BaseAnimation.h"

typedef struct tagAlphaChangeAnimation
{
	uint8_t StartAlpha;
	uint8_t EndAlpha;
}AlphaChangeAnimation;


void AlphaChangeAnimationSetAlpha(BaseAnimation* pSelf,uint8_t start,uint8_t end);
AnimationTypeInfo* GetAlphaChangeAnimationTypeInfo();

typedef struct tagPosChangeAnimation
{
	POINT StartPos;
	POINT EndPos;
	RECT StartRect;
	RECT EndRect;
}PosChangeAnimation;

void PosChangeAnimationSetPos(BaseAnimation* pSelf,int32_t startLeft,int32_t startTop,int32_t endLeft,int32_t endTop);
void PosChangeAnimationSetRect(BaseAnimation* pSelf,RECT* pStartRect,RECT* pEndRect);
AnimationTypeInfo* GetPosChangeAnimationTypeInfo();

void TransChangeAnimationSetTrans();
void MeshChangeAnimationSetMesh();
