#ifndef _NGOS_RENDER_DEVICE_H_
#define _NGOS_RENDER_DEVICE_H_
#include "RenderDefine.h"
#include "RenderResource.h"


#define NGRE_DEVICE_OPENFAILED  -1

typedef struct NGREDevice
{
	LPNGREBitmap pCacheBitmap;
	void* pExtra;
}NGREDevice;


NGRE_RESULT NGREOpenDevice(NGREDevice** ppDevice);
NGRE_RESULT NGREGetBitmapFromDevice(NGREDevice* pDevice, LPNGREBitmap* ppBitmap);
NGRE_RESULT NGREFlushDevice(NGREDevice* pDevice);
void NGRECloseDevice(NGREDevice* pDevice);


#endif