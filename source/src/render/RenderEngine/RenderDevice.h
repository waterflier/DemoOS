#ifndef _NGOS_RENDER_DEVICE_H_
#define _NGOS_RENDER_DEVICE_H_
#include "RenderDefine.h"
#include "RenderResource.h"


#define NGRE_DEVICE_OPENFAILED  -1
#define NGRE_DEVICE_INVALIDPARAM -2
#define NGREMaxDeviceClipRectCount 20

typedef struct NGREDevice
{
	LPNGREBitmap pCacheBitmap;
	NGREOpIRect clipRects[NGREMaxDeviceClipRectCount];
	unsigned int nClipRectCount;
	void* pExtra;
}NGREDevice;


NGRE_RESULT NGREOpenDevice(NGREDevice** ppDevice);
NGRE_RESULT NGREGetBitmapFromDevice(NGREDevice* pDevice, LPNGREBitmap* ppBitmap);
NGRE_RESULT NGREFlushDevice(NGREDevice* pDevice);
void NGRECloseDevice(NGREDevice* pDevice);
NGRE_RESULT NGREAddDeviceClipRect(NGREDevice* pDevice, CLPNGREOpIRect pRect);
NGRE_RESULT NGREClearDeviceClipRect(NGREDevice* pDevice);
NGRE_RESULT NGREGetDeviceClipRectByIndex(NGREDevice* pDevice, unsigned int nIndex, LPNGREOpIRect pRect);
unsigned int NGREGetDeviceClipRectCount(NGREDevice* pDevice);

#endif