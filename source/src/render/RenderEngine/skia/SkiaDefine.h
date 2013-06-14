#ifndef _NGOS_RENDER_SKDEFINE_H_
#define _NGOS_RENDER_SKDEFINE_H_
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkGpuDevice.h"


typedef struct NGRESkBitmap{
	SkBitmap* pSkBitmap;
	SkGpuDevice* pSkGpuDevice;
}*LPNGRESkBitmap;

#define NGREBitmap2SkGpuDevice(p) (((LPNGRESkBitmap)((p)->pExtra))->pSkGpuDevice)
#define NGREBitmap2SkBitmap(p) (((LPNGRESkBitmap)((p)->pExtra))->pSkBitmap)
#define NGREOpIRect2SkRect(r) (SkRect::MakeLTRB((SkScalar)((r).left),(SkScalar)((r).top),(SkScalar)((r).right),(SkScalar)((r).bottom)))
#define NGREOpIRect2SkIRect(r) (SkIRect::MakeLTRB((r).left,(r).top,(r).right,(r).bottom))

#ifdef NGRE_GL
#define NGREBitmap2CanvasInput(p) (NGREBitmap2SkGpuDevice(p))
#define NGREBitmap2BitmapInput(p) (NGREBitmap2SkGpuDevice(p)->accessBitmap(false))
#else
#define NGREBitmap2CanvasInput(p) (*NGREBitmap2SkBitmap(p))
#define NGREBitmap2BitmapInput(p) (*NGREBitmap2SkBitmap(p))
#endif


#endif