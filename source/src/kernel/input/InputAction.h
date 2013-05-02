//
// InputAction.h 
//
// ������ֽ���Action  
//
//////////////////////////////////////////////////////////////////////////

#ifndef _NGOS_INPUT_ACTION_H_
#define _NGOS_INPUT_ACTION_H_

//ACTION_TYPE = DEVICE_TYPE | ACTION_NUMBER��DEVICE_TYPE =0 ��ʾ�ⲻ��һ�����豸ֱ�Ӵ���������Action

// OnInputAction(ACTION_NUMBER,void* param1,void* param2,void* pEvent)

//���뷨�¼�


//���
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


//����-��׼101����
#define NGOS_INPUTDEVICE_STANDARD_KEYBOARD (2)
#define NGOS_ACTION_KEYBOARD_DOWN   (32)
#define NGOS_ACTION_KEYBOARD_UP     (33)
#define NGOS_ACTION_KEYBOARD_CHAR   (34)
#define NGOS_ACTION_KEYBOARD_HOTKEY (35)


//����-��׼�豸����
#define NGOS_INPUTDEVICE_STANDARD_DEVICE_BUTTON (4)

//��������
#define NGOS_INPUTDEVICE_MAIN_TOUCH_SCREEN (8)
#define NGOS_ACTION_TOUCH_BEGIN (100)
#define NGOS_ACTION_TOUCH_MOVE  (101) 
#define NGOS_ACTION_TOUCH_END   (102)
#define NGOS_ACTION_TOUCH_BREAK (103)

//��������
#define NGOS_INPUTDEVICE_MAIN_TOUCH_PAD (16)

//��ͼ��



//һЩ��Main Input Device�йص�API
int32_t NGOS_GetMainInputDevice();
int32_t NGOS_GetAllActiveInputDevice();
void NGOS_AttachInputDeviceChangeEvent();


#endif //_NGOS_INPUT_ACTION_H_

