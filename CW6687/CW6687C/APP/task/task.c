/*****************************************************************************
* Module    : Task
* File      : task.c
* Author    : Hanny
* Function  : ���������
*****************************************************************************/
#include "include.h"
#include "task.h"

type_task_ctl task_ctl;
void BtMainLoop(void);
type_move_ctl move_ctl;
u8 work_sta_interrupt = TASK_NULL;          //�绰�жϱ�־
extern void bt_pincode_init(void);
extern void task_init(void);

//�����л�˳���
#pragma constseg="TASK_CONST"
IAR_CONST u8 tbl_task_order[] = {
#if TASK_MUSIC_EN
    TASK_MUSIC,
#endif
#if TASK_VOICE_EN
    TASK_VOICE,
#endif
#if TASK_MIC_EN
    TASK_MIC,
#endif
#if TASK_FM_EN
    TASK_FM,
#endif
#if TASK_AUX_EN
    TASK_AUX,
#endif
#if TASK_AUX_EQ_EN
    TASK_AUX_EQ,
#endif
#if TASK_SPEAKER_EN
    TASK_SPEAKER,
#endif
#if TASK_TOMCAT_EN
    TASK_TOMCAT,
#endif
#if TASK_RTC_EN
    TASK_CLOCK,
#endif
#if TASK_USBDEV_EN
    TASK_USBDEV,
#endif
#if TASK_BT_EN && (!MP3BT_DECT_EN)
    TASK_BT,
#endif
#if TASK_BTHID_EN && (!MP3BT_DECT_EN)
    TASK_BT_HID,
#endif
};
#pragma constseg=default
#define TASK_TOTAL  sizeof(tbl_task_order)

#include SET_USER_PATH(CFG_UI_COMM_DIR, /ui/ui_comm.c)         //UI����

//�������ԭĬ�ϵ�����
void task_clear(void)
{
    WATCHDOG_CLR();
    set_lcd_buf(LCD_MINI_MODE);         //�����Դ棬ʹ��256ByteС����
    user_set_volex(3);                  //Ĭ����Ҫ3DB������������
    ocx_init(NULL, NULL);               //���Ĭ�ϵĿؼ��¼�����Ϣ
    sys_ctl.sleep_cnt = 0xffff;
    sys_ctl.disp_bat_val = 0;
#if WARNING_VOL_MAXMIN
    sys_ctl.vol_maxmin_music_flag = 0;
    sys_ctl.vol_maxmin_music_play = 0;
#endif
}

void task_init(void)
{
    task_ctl.last_work = TASK_NULL;     //��һ��������
    task_ctl.work_sta = TASK_IDLE;
#if MP3BT_DECT_EN
    MP3BT_DECT_INIT();
    delay(1);
    if(MP3BT_DECT_IS_BT()) {
        task_ctl.work_sta = TASK_BT;
    }else{
        task_ctl.work_sta = TASK_MUSIC;
    }
    MP3BT_DECT_END();
#endif

#if POWERON_TO_FM
    if(task_ctl.work_sta == TASK_IDLE && fm_init()) {
        task_ctl.work_sta = TASK_FM;
    }
#endif
#if TASK_BT_EN && !MP3BT_DECT_EN
    if(task_ctl.work_sta == TASK_IDLE) {
        task_ctl.work_sta = TASK_BT;
    }
#endif

    if(task_ctl.work_sta == TASK_BT) {
        led_bt_none();
    } else {
        led_idle();
    }
    if (!sys_ctl.mute_flag) {
        sys_unmute();
    }
#if TASK_SHOWTIME_EN
    task_ctl.work_sta = TASK_SHOW_TIME;
#endif
}

//ִ������
void run_task(void)
{
    task_init();
    comm_event();
    while (1) {
        task_clear();
        switch (task_ctl.work_sta) {
#if TASK_MENU_EN
        case TASK_MENU:
            task_menu();
            break;
#endif

#if TASK_SHOWTIME_EN
        case TASK_SHOW_TIME:
            task_show_time();
            break;
#endif

#if TASK_MUSIC_EN
        case TASK_MUSIC:
            task_ctl.last_work = TASK_MUSIC;
            task_music();
            break;
#endif

#if TASK_MIC_EN
        case TASK_MIC:
            task_ctl.last_work = TASK_MIC;
            task_record();
            break;
#endif

#if TASK_AUX_EN
        case TASK_AUX:
            task_ctl.last_work = TASK_AUX;
            task_record();
            break;
#endif

#if TASK_VOICE_EN
        case TASK_VOICE:
            task_ctl.last_work = TASK_VOICE;
            task_voice();
            break;
#endif

#if TASK_FM_EN
        case TASK_FM:
            task_ctl.last_work = TASK_FM;
            task_fm();
            break;
#endif

#if TASK_AUX_EQ_EN
        case TASK_AUX_EQ:
            task_ctl.last_work = TASK_AUX_EQ;
            task_aux_eq();
            break;
#endif

#if TASK_SPEAKER_EN
        case TASK_SPEAKER:
            task_ctl.last_work = TASK_SPEAKER;
            task_speaker();
            break;
#endif

#if TASK_USBDEV_EN
        case TASK_USBDEV:
            task_ctl.last_work = TASK_USBDEV;
            task_usbdev();
            break;
#endif

#if TASK_BT_EN
        case TASK_BT:
            task_ctl.last_work = TASK_BT;		
            task_bt();
            break;
#endif

#if TASK_BTHID_EN
        case TASK_BT_HID:
            task_ctl.last_work = TASK_BT_HID;		
            task_bt_hid();
            break;
#endif

#if TASK_TOMCAT_EN
        case TASK_TOMCAT:
            task_ctl.last_work = TASK_TOMCAT;		
            task_tomcat();
            break;
#endif

#if TASK_RTC_EN
        case TASK_CLOCK:
            task_ctl.last_work = TASK_CLOCK;
            task_clock();
            break;
#endif

#if ALARM_EN
        case TASK_ALARM:
            task_ctl.last_work = TASK_ALARM;
            task_alarm();
            break;
#endif

#if TASK_POWEROFF_EN
        case TASK_POWEROFF:
            task_poweroff();
            break;
#endif

#if TASK_IDLE_EN
        case TASK_IDLE:
            task_idle();
            break;
#endif

#if TASK_UPDATE_FW_EN
        case TASK_UPDATE_FW:
            task_update_fw();
            break;
#endif

#if TASK_SETTING_EN
        case TASK_SETTING:
            task_ctl.last_work = TASK_SETTING;
            task_setting();
            break;
#endif

        case TASK_EXIT:
            //task_ctl.work_sta = TASK_IDLE;
            task_exit();
            break;

        default:
            task_exit();            //�˳���Ч�Ĳ˵�
            break;
        }
    }
}

//��飬�������豸
__near_func bool device_change(void)
{
    if (!task_ctl.dev_change) {
        return DEVICE_IDLE;                //�豸����
    }
    task_ctl.dev_change = 0;
    return device_change_check();
}

//��ʱ��������
#pragma inline=forced
__near_func void task_sleep_check(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    if (sys_ctl.sleep_cnt && sys_ctl.sleep_cnt != 0xffff) {
        sys_ctl.sleep_cnt--;
        //printf("slpcnt %d\n", sys_ctl.sleep_cnt);
    }
    IE_EA = ie_ea;
}

#pragma inline=forced
__near_func void task_poweroff_check(void)
{
    if (sys_ctl.lowpower_flag) {
        task_ctl.work_sta = TASK_POWEROFF;
    }
}

#if IS_LCD_DISPLAY
//����ʱ�䴦����
__near_func void task_backlight_check(void)
{
    bool ie_ea = IE_EA;
    IE_EA = 0;
    if(sys_ctl.backlightcnt != 0 && sys_ctl.backlightcnt != 0xff){
        sys_ctl.backlightcnt--;
    }
    set_backlight();
    IE_EA = ie_ea;
}
#endif

//�˳�����
#pragma location="TASK_SEG"
void task_exit(void)
{
#if TASK_MENU_EN
    //��������˵����򷵻����˵�
    task_ctl.work_sta = TASK_MENU;
#else
    //���û�����˵������л�����һ������
    u8 task_num;
    for (task_num=0; task_num!=TASK_TOTAL; task_num++) {
        if (task_ctl.last_work == tbl_task_order[task_num]) {
            break;
        }
    }
    task_num++;                                     //�л�����һ������
    if (task_num >= TASK_TOTAL) {
        task_num = 0;                               //���һ��ѭ���ص�һ��
    }
    task_ctl.work_sta = tbl_task_order[task_num];   //�µ�����
#endif
}

#if FM_TIME_EN
//��ʱfmʱ����
__near_func void task_fm_check(void)
{
    u8 rtc_buf[5];
    if(sys_ctl.time_fm_on) {
        irtcc_read_rtc(rtc_buf);
        if(sys_ctl.fm_hour == rtc_buf[2])
        {
            if((sys_ctl.fm_min == rtc_buf[3]))
            {
                if(rtc_buf[4]<3)
                {
                    task_ctl.work_sta = TASK_FM;
                    sys_ctl.time_fm_on |= BIT(2);
                }
            }
        }
    }
}
#endif

//��������Ĺ����¼�����
bool comm_event(void)
{
    bool ret = true;
    WATCHDOG_CLR();
#if TASK_BT_EN
    if(task_ctl.work_sta == TASK_NULL)
        return false;
    if(BT_ALL_TASK || TASK_BT == task_ctl.work_sta || TASK_BT_HID == task_ctl.work_sta) {
        user_bt_event();
#if PLAY_DETECT_TO_BT//������ģʽ�¼�⵽�ֻ������������֣����л�������
        if (sys_ctl.bt_sta >= BT_STA_PLAYING) {
#else
        if (sys_ctl.bt_sta >= BT_STA_INCOMING) {
#endif
            if (TASK_BT == task_ctl.work_sta) {
                return true;                                    //ͨ�����̲����κ��л�
            }else {
                work_sta_interrupt = task_ctl.work_sta;
                task_ctl.work_sta = TASK_BT;
                return false;
            }
        }
    }
#endif
#if LINEIN_DETECT_EN
    if (device_need_activate(DEVICE_LINEIN)) {
        if (device_activate(DEVICE_LINEIN)) {
    #if TASK_AUX_EN
            task_ctl.work_sta = TASK_AUX;                      //����״̬�£�����LINEIN
    #elif TASK_AUX_EQ_EN
            task_ctl.work_sta = TASK_AUX_EQ;
    #endif
            ret = false;
        }
    }
#endif

#if MUSIC_MODE_AUTO
#if MP3BT_DECT_EN
    if(task_ctl.work_sta != TASK_BT)  //MP3BT_DECT_ENʱ,����ģʽ�忨���е�MUSIC.
#else
    if((task_ctl.work_sta != TASK_USBDEV) && (task_ctl.work_sta != TASK_MIC)
#if FM_RECORD_EN
        && (task_ctl.work_sta != TASK_FM)
#endif
#if AUX_RECORD_EN
        && (task_ctl.work_sta != TASK_AUX)
#endif
     )  //USBDEV��¼��ģʽ�忨���е�MUSIC.
#endif
    {
        if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
            task_ctl.work_sta = TASK_MUSIC;                         //����״̬�£������豸
            ret = false;
        }
    }
#endif

#if TASK_USBDEV_EN
    if (device_need_activate(DEVICE_PC)) {
        if (device_activate(DEVICE_PC)) {
            task_ctl.work_sta = TASK_USBDEV;
            ret = false;
        }
    }
#endif

    /*if (1) {
        //�м���ģʽ����Ҫ���������ɨ��
        u8 f_change = device_change();
        if (DEVICE_CHANGE == f_change) {
            task_ctl.work_sta = TASK_MUSIC;                         //����״̬�£������豸���������ֲ���ģʽ
            ret = false;
        }
    } */

    if(sys_ctl.alarm_wakeup_flag)
    {
        sys_ctl.alarm_wakeup_flag = false;
        task_ctl.work_sta = TASK_ALARM;
        ret = false;
    }
    return ret;
}

//�ж�utf8�ж��ٸ��ֽ�
#pragma location="LRC_SEG"
u8 utf8_char_size(char code)
{
    if (code <= 0x7f)//˵�����λΪ'0'������ζ��utf8����ֻ��1���ֽڣ�
    {
        return 1;
    }
    else if ((code & 0xe0)== 0xc0)//ֻ���������λ���������λ�ǲ���110�����������ζ��utf8������2���ֽڣ�
    {
        return 2;
    }
    else if ((code & (0xf0))== 0xe0)//ֻ���������λ���������λ�ǲ���1110�����������ζ��utf8������3���ֽڣ�
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

