//
// app_list.c
//
/////////////////////////////////////////

#include "./perheader.h"
#include "./app_list.h"
#include "../kernel/UIObject.h"
#include "../kernel/UIObjTree.h"
#include "../kernel/animation/BaseKeyFrameAnimation.h"
#include "./clock.h"

#define ICON_WIDTH (141)
#define FLIP_SEEP (200.0f)

NGOS_UIOBJECT_HANDLE appArray[ICON_W_NUM*ICON_H_NUM*APP_PAGE_MAX] = {0};

static int currentPage = 0;
static int GetPageNumByIndex(int index)
{
    int ret = 0;
    int count = ICON_W_NUM*ICON_H_NUM;
    ret = index / count;
    if(ret > APP_PAGE_MAX)
    {
        return APP_PAGE_MAX-1;
    }
    return ret;
}

static POINT GetAppIconPos(int index)
{
    POINT result;
    int pageindex = index / (ICON_W_NUM*ICON_H_NUM);
    int realIndex = index % (ICON_W_NUM*ICON_H_NUM);
    result.X = pageindex*720 + 52 + (realIndex % ICON_W_NUM)*(141+100);
    result.Y = 8 + (realIndex / ICON_W_NUM)*(141+59);

    return result;
}
//event handle
static int startX = 0;
static int startY = 0;
static uint32_t startTickCount = 0;
static int lastMovX = 0;
static int lastMovY = 0;
static RECT startAppListRect;

static void AniMoveToPageIndex(NGOS_UIOBJECT_HANDLE hAppList,int newIndex)
{
    printf("want page,%d->%d\n",currentPage,newIndex);

    if(newIndex < 0)
    {
        newIndex = 0;
    }
    if(newIndex >= APP_PAGE_MAX)
    {
        newIndex = APP_PAGE_MAX-1;
    }

    RECT nowRect;
    NGOS_GetUIObjectRect(hAppList,&nowRect);
    NGOS_ANIMATION_HANDLE hPosAni = NGOS_CreateAnimation(NGOS_GetDefaultTypeLoader(),"PosChangeAnimation");
    BaseAnimation* pPosAni = HandleMapDecodeAnimation(hPosAni,NULL);
    AnimationSetTotalTime(pPosAni,300);
    AnimationBindUIObject(pPosAni,hAppList);
    PosChangeAnimationSetPos(pPosAni,nowRect.left,nowRect.top,(-720)*newIndex,nowRect.top);
    AnimationManagerAddAnimation(GetAnimationManager(),hPosAni);
    AnimationStart(pPosAni);

    currentPage = newIndex;
}


static void OnAppList_TouchDown(UserDataContext* pUserData,NGOS_UIOBJECT_HANDLE hObj,uint32_t x,uint32_t y,MTEventData* pData)
{
    startX = x;
    startY = y;
    lastMovX = x;
    lastMovY = y;
    startTickCount = OSI_GetTickCount();
    NGOS_GetUIObjectRect(hObj,&startAppListRect);
    //TODO: hold app list(if in moving)
}

static void OnAppList_TouchUp(UserDataContext* pUserData,NGOS_UIOBJECT_HANDLE hObj,uint32_t x,uint32_t y,MTEventData* pData)
{
    if(startTickCount != 0)
    {
        uint32_t endTickCount = OSI_GetTickCount();
        float timeUse = (float)(endTickCount - startTickCount) / 1000.0f;
        float distance = (int)x - startX;
        float speed = distance / timeUse;
        if (speed > FLIP_SEEP)
        {
            printf("speed down %f\n",speed);
            AniMoveToPageIndex(hObj,currentPage-1);
        }
        else if(speed < 0-FLIP_SEEP)
        {
            printf("speed up\n",speed);
            AniMoveToPageIndex(hObj,currentPage+1);
        }
        else
        {
            if(x - startX > 200)
            {
                printf("pos down\n");
                AniMoveToPageIndex(hObj,currentPage-1);
            }
            else if(startX - x > 200)
            {
                printf("pos up\n");
                AniMoveToPageIndex(hObj,currentPage+1);
            }
            else
            {
                printf("go back\n");
                AniMoveToPageIndex(hObj,currentPage);
            }
        }
    }

}

static void OnAppList_TouchMove(UserDataContext* pUserData,NGOS_UIOBJECT_HANDLE hObj,uint32_t x,uint32_t y,MTEventData* pData)
{
    RECT appListRect;
    if(abs(x-lastMovX) < 2 && abs(y-lastMovY) < 2)
    {
        return;
    }

    lastMovX = x;
    lastMovY = y;

    if(abs(x-startX) > 4)
    {
        int dx = x - startX;
        appListRect = startAppListRect;
        appListRect.left = startAppListRect.left + dx;
        appListRect.right = appListRect.left + 720*APP_PAGE_MAX;
        NGOS_SetUIObjectRect(hObj,&appListRect);
    }
}

NGOS_UIOBJECT_HANDLE CreateAppList(int left,int top,int width,int height)
{
    NGOS_UIOBJECT_HANDLE hList = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject","app_list");
    RECT pos = {left,top,left+width*APP_PAGE_MAX,top+height}; 
    NGOS_SetUIObjectRect(hList,&pos);

    UIObject* pAppList = HandleMapDecodeUIObject(hList);
    if(pAppList)
    {
        UIObjectGetInputTarget(pAppList,1);
        EventContainer* pEA = UIObjectGetEventContainer(pAppList,NGOS_ACTION_TOUCH_DOWN,TRUE);
        AttachEvent(pEA,(void*)OnAppList_TouchDown,NULL);

        EventContainer* pEA2 = UIObjectGetEventContainer(pAppList,NGOS_ACTION_TOUCH_MOVE,TRUE);
        AttachEvent(pEA2,(void*)OnAppList_TouchMove,NULL);

        EventContainer* pEA3 = UIObjectGetEventContainer(pAppList,NGOS_ACTION_TOUCH_UP,TRUE);
        AttachEvent(pEA3,(void*)OnAppList_TouchUp,NULL);
    }

    
    AppListAddAppIcon(hList,CreateAppIcon("call","call"),0);
    AppListAddAppIcon(hList,CreateAppIcon("camer","camer"),1);
    AppListAddAppIcon(hList,CreateAppIcon("contact","contact"),2);
    AppListAddAppIcon(hList,CreateAppIcon("e-mail","e-mail"),3);
    AppListAddAppIcon(hList,CreateAppIcon("map","map"),4);
	AppListAddAppIcon(hList, CreateClockAppIcon(), 5);
	
    AppListAddAppIcon(hList,CreateAppIcon("music","music"),15);
    AppListAddAppIcon(hList,CreateAppIcon("note","note"),16);
    AppListAddAppIcon(hList,CreateAppIcon("sm","sm"),17);

    UpdateAppList(hList);
	return hList;
}



NGOS_UIOBJECT_HANDLE CreateAppIcon(const char* appID,const char* appText)
{
    char buffer[1024];
    sprintf(buffer,"%s.%s","app",appID);
    NGOS_UIOBJECT_HANDLE hApp = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"LayoutObject",buffer);
    RECT pos = {0,0,ICON_WIDTH,ICON_WIDTH+46};
    NGOS_SetUIObjectRect(hApp,&pos);

    sprintf(buffer,"%s.%s","app_icon",appID);
    NGOS_UIOBJECT_HANDLE hIcon = NGOS_CreateUIObject(NGOS_GetDefaultTypeLoader(),"ImageObject",buffer);
    NGOS_ImageObjectSetImageID(hIcon,buffer);
    RECT pos2={0,0,ICON_WIDTH,ICON_WIDTH};
    NGOS_SetUIObjectRect(hIcon,&pos2);
   
    NGOS_AddChild(hApp,hIcon);
    
    //TODO Add AppText
	
    return hApp;
}

void AppListAddAppIcon(NGOS_UIOBJECT_HANDLE hList,NGOS_UIOBJECT_HANDLE hAppIcon,int index)
{
    appArray[index] = hAppIcon;
    NGOS_AddChild(hList,hAppIcon);
    return ;
}

void UpdateAppList(NGOS_UIOBJECT_HANDLE hList)
{
    int i = 0;
    int count = ICON_W_NUM*ICON_H_NUM*APP_PAGE_MAX;
    RECT pos;
    POINT point;
    for(i=0;i<count;++i)
    {
        if(appArray[i])
        {
            point = GetAppIconPos(i);
            pos.left = point.X;
            pos.top = point.Y;
            pos.right = pos.left + ICON_WIDTH;
            pos.bottom = pos.top + ICON_WIDTH + 46;
            NGOS_SetUIObjectRect(appArray[i],&pos);
            //NGOS_AddChild(hPage,pageArray[i]);
        }
    }
}
//void AppListAddAppIcon(NGOS_UIOBJECT_HANDLE hList,NGOS_UIOBJECT_HANDLE hAppIcon)
//{
//    
//}

//void AppListRemoveAppIcon(NGOS_UIOBJECT_HANDLE hList,const char* appID)
//{
//
//}





