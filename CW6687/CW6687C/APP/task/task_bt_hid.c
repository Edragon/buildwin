/*****************************************************************************
 * Module    : Task
 * File      : task_bt_hid.c
 * Author    : Hanny
 * Function  : 蓝牙任务流程
 *****************************************************************************/
#include "include.h"

#if TASK_BTHID_EN
#include "task.h"

#pragma constseg="TASK_BT_HID_CONST"
IAR_CONST char str_show_task_bthid[] = "task_bthid\n";
IAR_CONST char str_show_task_bthid_exit[] = "task_bthid_exit\n";
#pragma constseg=default

#pragma location="TASK_BT_HID_SEG"
void task_bt_hid_enter(void)
{
    task_hid_flag = 1;
    task_bt_enter();
}

#pragma location="TASK_BT_HID_SEG"
void task_bt_hid_loop(void)
{
    u8 msg;
    while ((task_ctl.work_sta == TASK_BT_HID || sys_ctl.bt_sta >= BT_STA_INCOMING)
           && (task_ctl.work_sta != TASK_POWEROFF))
    {
        task_bt_event();
        while (msg = get_msg()) {
            task_bt_hid_deal_msg(msg);
#if WARNING_LOWPOWER    // 低电时跳出手机响铃
            if(task_ctl.work_sta == TASK_POWEROFF) {
                break;
            }
#endif
        }
        task_bt_display();
    }
}

#pragma location="TASK_BT_HID_SEG"
void task_bt_hid_exit(void)
{
    task_bt_exit();
    task_hid_flag = 0;
}

//蓝牙任务
#pragma location="TASK_BT_HID_SEG"
void task_bt_hid(void)
{
    printf(str_show_task_bthid);
    task_bt_hid_enter();
    task_bt_hid_loop();
    task_bt_hid_exit();
    printf(str_show_task_bthid_exit);
}
#endif
