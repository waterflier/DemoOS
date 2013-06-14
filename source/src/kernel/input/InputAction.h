//
// InputAction.h 
//
// 定义各种交互Action  
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_INPUT_ACTION_H_
#define _NGOS_INPUT_ACTION_H_

//ACTION_TYPE = DEVICE_TYPE | ACTION_NUMBER，DEVICE_TYPE =0 表示这不是一个由设备直接触发的输入Action

// OnInputAction(ACTION_NUMBER,void* param1,void* param2,void* pEvent)

//输入法事件


//鼠标
#define NGOS_INPUTDEVICE_MOUSE      (1)
#define NGOS_ACTION_LBUTTON_DOWN    (1)
#define NGOS_ACTION_LBUTTON_UP      (2)
#define NGOS_ACTION_LBUTTON_DBCLICK (3)
#define NGOS_ACTION_RBUTTON_DOWN    (4)
#define NGOS_ACTION_RBUTTON_UP      (5)
#define NGOS_ACTION_RBUTTON_DBCLICK (6)
#define NGOS_ACTION_MBUTTON_DOWN    (7)
#define NGOS_ACTION_MBUTTON_UP      (8)
#define NGOS_ACTION_MBUTTON_DBCLICK (9)

#define NGOS_ACTION_MOUSE_MOVE      (10)
#define NGOS_ACTION_MOUSE_HOVER     (11)
#define NGOS_ACTION_MOUSE_ENTER     (12)
#define NGOS_ACTION_MOUSE_LEAVE     (13)

#define NGOS_ACTION_MOUSE_WHEEL     (14)


//按键-标准101键盘
#define NGOS_INPUTDEVICE_STANDARD_KEYBOARD (2)
#define NGOS_ACTION_KEYBOARD_DOWN   (32)
#define NGOS_ACTION_KEYBOARD_UP     (33)
#define NGOS_ACTION_KEYBOARD_CHAR   (34)
#define NGOS_ACTION_KEYBOARD_HOTKEY (35)


//按键-标准设备按键
#define NGOS_INPUTDEVICE_STANDARD_DEVICE_BUTTON (4)

//主触摸屏
#define NGOS_INPUTDEVICE_MAIN_TOUCH_SCREEN (8)
#define NGOS_ACTION_TOUCH_DOWN (100)  //value x|y param:null data:null
#define NGOS_ACTION_TOUCH_MOVE  (101) //value x|y param:null data:null
#define NGOS_ACTION_TOUCH_UP   (102) //value x|y param:null data:null
#define NGOS_ACTION_TOUCH_ONE_DOWN (103) //value  x|y
#define NGOS_ACTION_TOUCH_ONE_UP (104) //value x|y
typedef struct tagMTEventSolt
{
	uint16_t x;
	uint16_t y;
	uint16_t major;
	uint16_t minor;
	uint16_t trackerID;
}MTEventSolt;

typedef struct tagMTEventData
{
	int16_t RefCount;
	int16_t FingerSoltCount;
	MTEventSolt Solts; //size = sizeof (MTEventSolt) * FingerSoltCount
}MTEventData;

MTEventData* CreateMTEventData(int soltSize);
void ReleaseMTEventData(MTEventData* pData);
int AddRefMTEventData(MTEventData* pData);
MTEventSolt* GetFingerSoltData(MTEventData* pData,uint16_t index);

#define NGOS_ACTION_TOUCH_BREAK (103) //not support now

//主触摸板
#define NGOS_INPUTDEVICE_MAIN_TOUCH_PAD (16)

//绘图板



//一些和Main Input Device有关的API
int32_t NGOS_GetMainInputDevice();
int32_t NGOS_GetAllActiveInputDevice();
void NGOS_AttachInputDeviceChangeEvent();


#endif //_NGOS_INPUT_ACTION_H_

