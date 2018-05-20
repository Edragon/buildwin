/*****************************************************************************
 * Module    : TaskSub
 * File      : tasksub_bt_test.c
 * Author    : Hanny
 * Function  : 蓝牙任务 - 测试盒任务
 *****************************************************************************/
#ifdef TASK_SUB

#pragma location="TASK_BT_TEST_SEG"
void tasksub_bt_test_enter(void)
{
    task_ctl.work_sub = TASK_BT_TEST;

    dac_dynamic(DYNAMIC_NO);                    //测试盒任务，不需要动态降噪

    btdec_play();                               //开始播放
}

#pragma location="TASK_BT_TEST_SEG"
void tasksub_bt_test_exit(void)
{
    dac_dynamic(BT_DYNAMIC_NOISE);              //结束响铃，开启动态降噪
    task_bt_sta_update();
}

#pragma location="TASK_BT_TEST_SEG"
void tasksub_bt_test_event(void)
{
    WATCHDOG_CLR();
    comm_event();

    if (!bt_testmode()) {
        task_ctl.work_sub = TASK_BT_NULL;
    }
}

#pragma location="TASK_BT_TEST_SEG"
void tasksub_bt_test_loop(void)
{
    u8 msg;
    u8 work_sta = task_ctl.work_sta;    //TASK_BT / TASK_BT_HID

    while (task_ctl.work_sta == work_sta && task_ctl.work_sub == TASK_BT_TEST) {
        tasksub_bt_test_event();
        while (msg = get_msg()) {
            task_bt_test_deal_msg(msg);
        }
        task_bt_display();
    }
}

#pragma location="TASK_BT_TEST_SEG"
void tasksb_bt_test_play(void)
{
    bt_test_vendor(0x44);
}

#pragma location="TASK_BT_TEST_SEG"
void tasksb_bt_test_prev(void)
{
    bt_test_vendor(0x4c);
}

#pragma location="TASK_BT_TEST_SEG"
void tasksb_bt_test_next(void)
{
    bt_test_vendor(0x4b);
}

#pragma location="TASK_BT_TEST_SEG"
void tasksb_bt_test_voldown(void)
{
    bt_test_vendor(0x42);
}

#pragma location="TASK_BT_TEST_SEG"
void tasksb_bt_test_volup(void)
{
    bt_test_vendor(0x41);
}

#pragma location="TASK_BT_TEST_SEG"
void tasksb_bt_test_hsf(void)
{
    bt_test_vendor(0x88);
}

#pragma location="TASK_BT_TEST_SEG"
void tasksub_bt_test(void)
{
    //printf("tasksub_bt_test\n");
    tasksub_bt_test_enter();
    tasksub_bt_test_loop();
    tasksub_bt_test_exit();
    //printf("tasksub_bt_test exit\n");
}

#endif
