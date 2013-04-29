#ifndef _NGOS_RENDER_SKDEFINE_H_
#define _NGOS_RENDER_SKDEFINE_H_
#include "SkBitmap.h"
#include "GrContextFactory.h"
#include "SkGpuDevice.h"
typedef struct NGRESkBitmap{
	SkBitmap* pSkBitmap;
	SkGpuDevice* pSkGpuDevice;
}*LPNGRESkBitmap;

#define NGREBitmap2SkGpuDevice(p) (((LPNGRESkBitmap)((p)->pExtra))->pSkGpuDevice)
#define NGREBitmap2SkBitmap(p) (((LPNGRESkBitmap)((p)->pExtra))->pSkBitmap)
#define NGREOpIRect2SkRect(r) (SkRect::MakeLTRB((SkScalar)((r).left),(SkScalar)((r).top),(SkScalar)((r).right),(SkScalar)((r).bottom)))

#endif