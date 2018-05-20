/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_bt_lost.c
 * Author    :
 * Function  : 蓝牙任务 - 防丢器任务
 *****************************************************************************/
#ifdef TASK_SUB
void task_bt_lost_deal_msg(u8 msg);

#pragma location="TASK_BT_LOST_SEG"
void tasksub_bt_lost_enter(void)
{
    task_ctl.work_sub = TASK_BT_LOST;
    led_bt_idle();
}

#pragma location="TASK_BT_LOST_SEG"
void tasksub_bt_lost_exit(void)
{
    task_bt_sta_update();
}

#pragma location="TASK_BT_LOST_SEG"
void tasksub_bt_lost_event(void)
{
    WATCHDOG_CLR();
    comm_event();

    if (sys_ctl.bt_sta >= BT_STA_CONNECTED) {
        task_ctl.work_sub = TASK_BT_NULL;  //退出出BT_LOST
    }
}

#pragma location="TASK_BT_LOST_SEG"
void tasksub_bt_lost_loop(void)
{
    u8 msg;
    u8 work_sta = task_ctl.work_sta;    //TASK_BT / TASK_BT_HID

    while (task_ctl.work_sta == work_sta && task_ctl.work_sub == TASK_BT_LOST) {
        tasksub_bt_lost_event();
        while (msg = get_msg()) {
            task_bt_lost_deal_msg(msg);
        }
        task_bt_display();
    }
}

#pragma location="TASK_BT_LOST_SEG"
void tasksub_bt_lost(void)
{
    //printf("tasksub_ bt_lost\n");
    tasksub_bt_lost_enter();
    tasksub_bt_lost_loop();
    tasksub_bt_lost_exit();
    //printf("tasksub_bt_ lost exit\n");
}
#endif
