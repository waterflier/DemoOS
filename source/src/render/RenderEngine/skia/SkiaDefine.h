#ifndef _NGOS_RENDER_SKDEFINE_H_
#define _NGOS_RENDER_SKDEFINE_H_
#include "SkBitmap.h"
#include "GrContextFactory.h"
#include "SkGpuDevice.h"

typedef struct NGRESkBitmap{
	SkBitmap* pSkBitmap;
	SkGpuDevice* pSkGpuDevice;
}*LPNGRESkBitmap;




#endif