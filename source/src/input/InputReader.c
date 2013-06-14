//
// InputReader.c
//
//////////////////////////////////////////////

#include <pthread.h>
#ifdef ANDROID
#include <linux/sem.h>
#include <linux/types.h>
#include <linux/ipc.h>
#include <linux/types.h>
#else
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/types.h>
#endif
#include <unistd.h>

#include "./InputReader.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/input.h>
#include <dirent.h>
#include <sys/epoll.h>


struct InputDeviceIdentifier {
    char name[128];
    char location[128];
    char uniqueId[128];
    uint16_t bus;
    uint16_t vendor;
    uint16_t product;
    uint16_t version;
};


/*
 * Input device classes.
 */
enum {
    /* The input device is a keyboard or has buttons. */
    INPUT_DEVICE_CLASS_KEYBOARD      = 0x00000001,

    /* The input device is an alpha-numeric keyboard (not just a dial pad). */
    INPUT_DEVICE_CLASS_ALPHAKEY      = 0x00000002,

    /* The input device is a touchscreen or a touchpad (either single-touch or multi-touch). */
    INPUT_DEVICE_CLASS_TOUCH         = 0x00000004,

    /* The input device is a cursor device such as a trackball or mouse. */
    INPUT_DEVICE_CLASS_CURSOR        = 0x00000008,

    /* The input device is a multi-touch touchscreen. */
    INPUT_DEVICE_CLASS_TOUCH_MT      = 0x00000010,

    /* The input device is a directional pad (implies keyboard, has DPAD keys). */
    INPUT_DEVICE_CLASS_DPAD          = 0x00000020,

    /* The input device is a gamepad (implies keyboard, has BUTTON keys). */
    INPUT_DEVICE_CLASS_GAMEPAD       = 0x00000040,

    /* The input device has switches. */
    INPUT_DEVICE_CLASS_SWITCH        = 0x00000080,

    /* The input device is a joystick (implies gamepad, has joystick absolute axes). */
    INPUT_DEVICE_CLASS_JOYSTICK      = 0x00000100,

    /* The input device is external (not built-in). */
    INPUT_DEVICE_CLASS_EXTERNAL      = 0x80000000,
};

  struct Device {
        int fd; // may be -1 if device is virtual
        const int32_t id;
        char path[256];
        struct InputDeviceIdentifier identifier;

        uint32_t classes;

        uint8_t keyBitmask[(KEY_MAX + 1) / 8];
        uint8_t absBitmask[(ABS_MAX + 1) / 8];
        uint8_t relBitmask[(REL_MAX + 1) / 8];
        uint8_t swBitmask[(SW_MAX + 1) / 8];
        uint8_t ledBitmask[(LED_MAX + 1) / 8];
        uint8_t ffBitmask[(FF_MAX + 1) / 8];
        uint8_t propBitmask[(INPUT_PROP_MAX + 1) / 8];
    };

#define test_bit(bit, array)    (array[bit/8] & (1<<(bit%8)))
#define sizeof_bit_array(bits)  ((bits + 7) / 8)

static int containsNonZeroByte(const uint8_t* array, uint32_t startIndex, uint32_t endIndex) {
    const uint8_t* end = array + endIndex;
    array += startIndex;
    while (array != end) {
        if (*(array++) != 0) {
            return 1;
        }
    }
    return 0;
}

static int OpenInputDevices(InputReader* pSelf,char* devicePath)
{
	char buffer[80]={0};
	printf("start open input device %s\n",devicePath);

    int fd = open(devicePath, O_RDWR);
    if(fd < 0) {
        printf("could not open %s, %s\n", devicePath, strerror(errno));
        return -1;
    }

    struct InputDeviceIdentifier identifier;

    // Get device name.
    if(ioctl(fd, EVIOCGNAME(sizeof(buffer) - 1), &buffer) < 1) {
        printf("could not get device name for %s, %s\n", devicePath, strerror(errno));
    } else {
        buffer[sizeof(buffer) - 1] = '\0';
        strcpy(identifier.name,buffer);
    }

    // Get device driver version.
    int driverVersion;
    if(ioctl(fd, EVIOCGVERSION, &driverVersion)) {
        printf("could not get driver version for %s, %s\n", devicePath, strerror(errno));
        close(fd);
        return -1;
    }

    // Get device identifier.
    struct input_id inputId;
    if(ioctl(fd, EVIOCGID, &inputId)) {
        printf("could not get device input id for %s, %s\n", devicePath, strerror(errno));
        close(fd);
        return -1;
    }

    identifier.bus = inputId.bustype;
    identifier.product = inputId.product;
    identifier.vendor = inputId.vendor;
    identifier.version = inputId.version;

    // Get device physical location.
    if(ioctl(fd, EVIOCGPHYS(sizeof(buffer) - 1), &buffer) < 1) {
        printf("could not get location for %s, %s\n", devicePath, strerror(errno));
    } else {
        buffer[sizeof(buffer)-1] = '\0';
        strcpy(identifier.location,buffer);
    }

    // Get device unique id.
    if(ioctl(fd, EVIOCGUNIQ(sizeof(buffer) - 1), &buffer) < 1) {
        //fprintf(stderr, "could not get idstring for %s, %s\n", devicePath, strerror(errno));
    } else {
        buffer[sizeof(buffer) - 1] = '\0';
        strcpy(identifier.uniqueId,buffer);
    }

    // Make file descriptor non-blocking for use with poll().ABS_MT_POSITION_X
    if (fcntl(fd, F_SETFL, O_NONBLOCK)) {
        printf("Error %d making device file descriptor non-blocking.", errno);
        close(fd);
        return -1;
    }

    // Allocate device.  (The device object takes ownership of the fd at this point.)
    //int32_t deviceId = mNextDeviceId++;
    struct Device* device = (struct Device*) malloc(sizeof(struct Device));


    printf("found device : %s\n", devicePath);
    printf("  bus:       %04x\n"
         "  vendor     %04x\n"
         "  product    %04x\n"
         "  version    %04x\n",
        identifier.bus, identifier.vendor, identifier.product, identifier.version);
    printf("  name:      \"%s\"\n", identifier.name);
    printf("  location:  \"%s\"\n", identifier.location);
    printf("  unique id: \"%s\"\n", identifier.uniqueId);
  


    // Load the configuration file for the device.
    //loadConfigurationLocked(device);

    // Figure out the kinds of events the device reports.
    ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(device->keyBitmask)), device->keyBitmask);
    ioctl(fd, EVIOCGBIT(EV_ABS, sizeof(device->absBitmask)), device->absBitmask);
    ioctl(fd, EVIOCGBIT(EV_REL, sizeof(device->relBitmask)), device->relBitmask);
    ioctl(fd, EVIOCGBIT(EV_SW, sizeof(device->swBitmask)), device->swBitmask);
    ioctl(fd, EVIOCGBIT(EV_LED, sizeof(device->ledBitmask)), device->ledBitmask);
    ioctl(fd, EVIOCGPROP(sizeof(device->propBitmask)), device->propBitmask);

    // See if this is a keyboard.  Ignore everything in the button range except for
    // joystick and gamepad buttons which are handled like keyboards for the most part.
    int haveGamepadButtons = containsNonZeroByte(device->keyBitmask, sizeof_bit_array(BTN_MISC),
                    sizeof_bit_array(BTN_MOUSE))
            || containsNonZeroByte(device->keyBitmask, sizeof_bit_array(BTN_JOYSTICK),
                    sizeof_bit_array(BTN_DIGI));

    //if (haveKeyboardKeys || haveGamepadButtons) {
    //    device->classes |= INPUT_DEVICE_CLASS_KEYBOARD;
    // }
    //printf("keymask")
    // See if this is a cursor device such as a trackball or mouse.
    if (test_bit(BTN_MOUSE, device->keyBitmask)
            && test_bit(REL_X, device->relBitmask)
            && test_bit(REL_Y, device->relBitmask)) {
        device->classes |= INPUT_DEVICE_CLASS_CURSOR;
    }

    // See if this is a touch pad.
    // Is this a new modern multi-touch driver?
    if (test_bit(ABS_MT_POSITION_X, device->absBitmask)
            && test_bit(ABS_MT_POSITION_Y, device->absBitmask)) {
        // Some joysticks such as the PS3 controller report axes that conflict
        // with the ABS_MT range.  Try to confirm that the device really is
        // a touch screen.
        if (test_bit(BTN_TOUCH, device->keyBitmask) || !haveGamepadButtons ) {
            device->classes |= INPUT_DEVICE_CLASS_TOUCH | INPUT_DEVICE_CLASS_TOUCH_MT;
            printf("***found multi-tuoch device!,devpath=%s\n",devicePath);
            pSelf->multiTouchFD = fd;
        }
    // Is this an old style single-touch driver?
    } else if (test_bit(BTN_TOUCH, device->keyBitmask)
            && test_bit(ABS_X, device->absBitmask)
            && test_bit(ABS_Y, device->absBitmask)) {
        device->classes |= INPUT_DEVICE_CLASS_TOUCH;
    }

    // Check whether this device has switches.
    int i=0;
    for (i = 0; i <= SW_MAX; i++) {
        if (test_bit(i, device->swBitmask)) {
            device->classes |= INPUT_DEVICE_CLASS_SWITCH;
            break;
        }
    }


    if (device->classes == 0) {
        free(device);
        return -1;
    }


    return 0;
}



static void ScanInputDevice(InputReader* pSelf)
{
	printf("scan input device ....\n");

	char devname[PATH_MAX];
    char *filename;
    DIR *dir;
    struct dirent *de;
    char* dirname = "/dev/input/";
    dir = opendir(dirname);
    if(dir == NULL)
        return ;

    strcpy(devname, dirname);
    filename = devname + strlen(devname);
    *filename++ = '/';

    while((de = readdir(dir)))
    {
        if(de->d_name[0] == '.' &&
           (de->d_name[1] == '\0' ||
            (de->d_name[1] == '.' && de->d_name[2] == '\0')))
            continue;

        strcpy(filename, de->d_name);
        printf("test device:%s\n",devname);
        OpenInputDevices(pSelf,devname);
    }
    closedir(dir);
    return ;
}

static char readBuffer[sizeof(struct input_event)*RAW_INPUT_EVENT_READ_LEN];
  

static int InputReaderGetMultTouchEvents(InputReader* pSelf)
{
	//printf("Get events\n");
    int pollResult = epoll_wait(pSelf->epollFD,pSelf->pendingEvent,EPOLL_MAX_EVENTS,10);
    if(pollResult == 0)
    {
    	//time out
        pSelf->pendingEventCount = 0;
        return;
    }
    
    if(pollResult < 0)
    {
        printf("epoll wait error\n");
        sleep(1);
        return;
    }

    pSelf->pendingEventCount = pollResult;
    //printf("epoll_wait return %d events\n",pendingEventCount);
    usleep(1);

    //read event
    int32_t readSize = read(pSelf->multiTouchFD, readBuffer,
                        sizeof(struct input_event) * RAW_INPUT_EVENT_READ_LEN);
    //printf("read %d byte event data,about %d input ,mod =%d\n",readSize,
     //   readSize / sizeof(struct input_event),readSize % (sizeof(struct input_event)));

    int count = readSize / sizeof(struct input_event);
    int j = 0;
    struct input_event* pRead = (struct input_event*)readBuffer;
    for(j=0;j<count;++j)
    {
        //printf("event type=%u,code=%u,value=%X \n",pRead->type,pRead->code,pRead->value);
        MultiTouchAccumulatorPushRawInputEvent(pSelf->pMTAccumulator, pRead);
        pRead ++;
    }
}

static const char* GetActionType(int typeCode)
{
    switch(typeCode)
    {
    case  NGOS_ACTION_TOUCH_DOWN:  //value x|y param:null data:null
        return "NGOS_ACTION_TOUCH_DOWN";
    case NGOS_ACTION_TOUCH_MOVE: //value x|y param:null data:null
        return "NGOS_ACTION_TOUCH_MOVE";
    case NGOS_ACTION_TOUCH_UP: //value x|y param:null data:null
        return "NGOS_ACTION_TOUCH_UP";
    case  NGOS_ACTION_TOUCH_ONE_DOWN: //value  x|y
        return "NGOS_ACTION_TOUCH_ONE_DOWN";
    case NGOS_ACTION_TOUCH_ONE_UP: //value x|y
        return "NGOS_ACTION_TOUCH_ONE_UP";
    }

    return "unknown action";
}


static  void* InputReaderThreadProc(void* ud)
{
	InputReader* pSelf = (InputReader*) ud;

	ScanInputDevice(pSelf);

	if(pSelf->multiTouchFD)
	{
		pSelf->epollFD = epoll_create(EPOLL_SIZE_HINT);

        struct epoll_event eventItem;
        memset(&eventItem,0,sizeof(eventItem));
        eventItem.events = EPOLLIN;
        eventItem.data.u32 = 0;
        if(epoll_ctl(pSelf->epollFD,EPOLL_CTL_ADD,pSelf->multiTouchFD,&eventItem))
        {
            printf("epoll add multi-touch device fd error\n");
        }

        printf("enter multi-touch read loop\n");
        
        while(1)
        {
        	InputReaderGetMultTouchEvents(pSelf);
            MultiTouchAction theAction;
        	while(MultiTouchAccumulatorPopAction(pSelf->pMTAccumulator,&theAction))
        	{
        		//printf action;
                //printf("action type=%s,x=%d,y=%d,solt count=%d\n",GetActionType(theAction.actionType),theAction.x,theAction.y,theAction.pData->FingerSoltCount);

      
                if(pSelf->hRecv)
                {
                    OSI_PostMsg(pSelf->hRecv,MSG_INPUT_ACTION,
                        (NGOS_INPUTDEVICE_MAIN_TOUCH_SCREEN<<16) | theAction.actionType,(theAction.x<<16) | theAction.y,theAction.pData);
                }
                else
                {
                    if(theAction.pData)
                    {
                        ReleaseMTEventData(theAction.pData);
                    } 
                }
        	}
        }
	}
	else
	{
		printf("no multi-touch device,input reader end\n");
		return 0;
	}
}

InputReader* CreateInputReader()
{
    InputReader* pResult = NULL;
    pResult = (InputReader*) malloc(sizeof(InputReader));
    memset(pResult,sizeof(InputReader),0);
    pResult->pMTAccumulator = CreateMultiTouchAccumulator();


    return pResult;
}

int ReleaseInputReader(InputReader* pSelf)
{


}

int InputReaderStart(InputReader* pSelf)
{
	if(pSelf->State == 0)
	{
		pSelf->State = 1;
	}
	else
	{
		return -1;
	}

	//TODO: start thread;
    OSI_CreateThread(InputReaderThreadProc,pSelf);
    //InputReaderThreadProc(pSelf);
	return  0;
}

void SetInputReadeActionReciver(InputReader* pSelf,TYPE_NGOS_MSG_RECIVER hRecv)
{
    if(pSelf)
    {
        pSelf->hRecv = hRecv;
    }
}
