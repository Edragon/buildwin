/*****************************************************************************
 * Module    : Theme
 * File      : ledseg_default_menu.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD断码屏的默认主题 - 音乐播放
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_BT == LEDSEG_THEME_DEFAULT)
#include "task.h"
#include "user_bt.h"
#include "btapi.h"

//显示设备信息
#pragma location="BT_API2"
void task_bt_display_device(u8 show_time)
{

}

//蓝牙任务显示处理
#pragma location="TASK_BT_SEG"
void task_bt_display(void)
{
    if (!ledseg_ocx_event()) {      //没有更新显示时不用一直刷新
        return;
    }

    switch (ledseg_ocx.disp_status) {
    case LEDSEG_OCX_NULL:
#if BT_DISP_NUM_TIME
        if((BT_STATE == OutGoingCall || BT_STATE == InComingCall) && bt_phonenum_flag){ //有些手机不支持号码显示时则显示bt
            ledseg_disp(MENU_PHONENUMBER);
        } else if(BT_STATE == OnThePhone){
            ledseg_disp(MENU_ON_THE_PHONE_TIME);
        } else
#endif
        {
            ledseg_disp(MENU_BT);
        }
        break;

    default:
        ledseg_ocx_display();
        break;
    }
}
#endif
