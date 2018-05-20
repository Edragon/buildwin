/*****************************************************************************
* Module    : Task
* File      : task_alarm.c
* Author    : Yinglang_Li
* Email     :
* Function  : ����ģ��
*****************************************************************************/
#include "include.h"
#if ALARM_EN
#include "task.h"
#include "task_alarm.h"
#include "task_clock.h"

type_alarm_task t_alarm;
u32 next_alarm;          //��һ����ʱ��
u32 cur_time_sec;
void task_alarm_calc_and_set(u8 Mode);
bool task_alarm_msgbox(void);
extern u16 crc_calculate(void *buf, u16 len);
#include SET_USER_PATH(CFG_UI_CLOCK_DIR, /ui/ui_alarm.c)

//�ж���������������
#pragma location="TASK_ALARM_SEG"
void task_alm_music_get(void)
{
    t_alarm.alm_music_type = 0;
    t_alarm.alm_music_type = irtcc_read_ram8(ALM_MUSIC_TYPE);
    if(!t_alarm.alm_music_type)
    {
        return;
    }
    t_alarm.alm_music_type = 0;
    if (!device_is_actived(fs_cur_dev()) || !fs_is_mounted()) {
        draw_wait();
        device_change();
        if (fs_need_mounted()) {
            set_sys_clk(SYS_48M);                   //�����Ƶ������
            fs_init();
            set_sys_clk(SYS_24M);
        }
    }
    if (!device_is_actived(fs_cur_dev())) {
        return;
    }
    if (!fs_is_mounted()) {
        return;
    }
    f_scan_set(SCAN_SUB_FOLDER, &file_filter_music, &dir_filter_music);
    if(!music_total())
    {
        return;
    }
    t_alarm.alm_music_num = irtcc_read_ram16(ALM_FILE_NUM);
    if (!f_open_opt(t_alarm.alm_music_num)) {
        return;
    }
    f_get_opt_filename(filename);
    u16 name_crc = irtcc_read_ram16(ALM_NAME_CRC);
    if(name_crc == crc_calculate(filename,8))
    {
      if (TYPE_NONE == music_init()) {
        return;
      }
      t_alarm.alm_music_type = 1;
      music_eq(sys_ctl.eq_num);
      music_set_speed(0);
      music_play();
    }
}
//�����ʼ��
#pragma location="TASK_ALARM_SEG"
void task_alarm_enter(void)
{
    set_sys_clk(SYS_24M);
    //dac_dynamic(DYNAMIC_NO);
    task_ctl.work_sta = TASK_ALARM;
    set_lcd_buf(LCD_FULL_MODE);
    user_change_volume(WARNING_VOLUME);//  //
    dac_enable();
    task_alm_music_get();
    sys_ctl.voice_play_flag = 1;
    if(!t_alarm.alm_music_type)
    {
        mp3_res_play(RES_MP3_ALARM_BELL);
    }
    t_alarm.disp_index = 0xFF;
    t_alarm.index = 1;                          //Ĭ��ѡ���ǣ�����
    t_alarm.cur_state = 0;                      //��������
    task_alarm_check();
}

//�����˳�
#pragma location="TASK_ALARM_SEG"
void task_alarm_exit(void)
{

    music_stop();                           //����Music����
    dac_disable();
#if IS_LCD_DISPLAY
    task_ctl.work_sta = task_ctl.back_up_work_sta;      // ����֮ǰ��״̬
    task_ctl.back_up_work_sta = TASK_MENU;      // �������ϴε�״̬ΪTASK_MENU
#endif
}


//����������
#pragma location="TASK_ALARM_SEG"
void task_alarm_event(void)
{
    WATCHDOG_CLR();
    if (music_get_status() <= STATUS_PLAY_STOPPING) {
        if(!t_alarm.alm_music_type)
        {
            mp3_res_play(RES_MP3_ALARM_BELL);
        }
    }
    if(t_alarm.alm_music_type)
    {
        if (!device_is_actived(fs_cur_dev())) {
            music_stop();                           //ֹͣ��ǰ����
            task_ctl.dev_change = 1;                //��ǰ�豸��Ч���豸�����䶯
        }
    }
    comm_event();
    music_event();

    if (cur_time_sec - next_alarm > SECS_ALARM_BELL) {
        t_alarm.index = 2;
        t_alarm.cur_state = 1;                      //����ʱ�䵽��Ĭ������
    }
    if(t_alarm.cur_state){
        if (((t_alarm.index == 2) && (t_alarm.ring_times) < (ALARM_REBELL_TIMES-1)) || (t_alarm.index == 1)) {
            next_alarm += SECS_ALARM_REBELL;        //��������
            if (next_alarm > 3155759999UL) {
                sys_ctl.alarm_switch = 0;
            }
            t_alarm.ring_times++;
        } else {
            task_alarm_set();                               //�رգ���������
#if !IS_LCD_DISPLAY
            if(sys_ctl.alarm_mode == 0)
            {
                sys_ctl.alarm_switch = 0;
                sys_param_write8(PARAM_ALARM_SWITCH, sys_ctl.alarm_switch);
            }
#endif
        }
        task_ctl.work_sta = TASK_CLOCK;     //���������ʾʱ��
    }
}


//��������
#pragma location="TASK_ALARM_SEG"
void task_alarm_set(void)
{
    u8 next_day = 0;
    u8 num = 0;
    u8 i;
    u32 cur_time;
    t_alarm.ring_times = 0;
    next_alarm = 0;
    cur_time = irtcc_read_rtc(READ_RTC);
    make_time(irtcc_read_rtc(READ_RTC));  //������ж� ��Ҫ�õ�ת�����t_time
#if ALM_TYPE
    //����ʱ������ѹ���
    if (sys_ctl.alarm_mode == ALARM_CYC_7D)
    {
        if (t_time.hour > sys_ctl.alarm_hour || (t_time.hour == sys_ctl.alarm_hour && t_time.minute >= sys_ctl.alarm_min))
        {   //ÿ������
            next_day = 1;
        }
    }
    else if (sys_ctl.alarm_mode == ALARM_CYC_5D)
        //����������
    {
        switch (t_time.wday)
        {
        case FRIDAY:
            if (t_time.hour > sys_ctl.alarm_hour || (t_time.hour == sys_ctl.alarm_hour && t_time.minute >= sys_ctl.alarm_min))
            {   //ÿ������
                next_day = 3;
            }
            else
            {
                next_day = 0;
            }
            break;
        case SATURDAY:
            next_day =  2;
            break;
        case SUNDAY:
            next_day = 1;
            break;
        default:
            if (t_time.hour > sys_ctl.alarm_hour || (t_time.hour == sys_ctl.alarm_hour && t_time.minute >= sys_ctl.alarm_min))
            {
                next_day = 1;
            }
            else
            {
                next_day = 0;
            }
            break;
        }
    }
#else
    //printf("setam a mode = %d\n",sys_ctl.alarm_mode);
    //printf("setam t_time.hour = %d\n",t_time.hour);
    //printf("setam  t_time.minute = %d\n",t_time.minute);
    //printf("setam sys_ctl.alarm_hour = %d\n",sys_ctl.alarm_hour);
    //printf("setam  sys_ctl.alarm_min = %d\n",sys_ctl.alarm_min);
    if(sys_ctl.alarm_mode != 0)
    {
        num = 7;
        for(i = 0; i < 7; i++)
        {
            if(t_time.wday == i && (t_time.hour > sys_ctl.alarm_hour || (t_time.hour == sys_ctl.alarm_hour && t_time.minute >= sys_ctl.alarm_min)))
            {
                if(num >= 7)
                    num = 7;
            }
            else if(sys_ctl.alarm_mode & (1<<i))
            {
                if(t_time.wday <= i)
                {
                    if(num >= i - t_time.wday)
                        num = i - t_time.wday;
                }
                else
                {
                    if(num >= i + 7 - t_time.wday)
                        num = i + 7 - t_time.wday;
                }
            }
        }
    }
    else if (t_time.hour > sys_ctl.alarm_hour || (t_time.hour == sys_ctl.alarm_hour && t_time.minute >= sys_ctl.alarm_min)){ //�������Ӳ���ʱ���ѹ���
        num = 1;
    }
    next_day = num;
#endif
    //printf("setam nxd=%d\n",next_day);
    next_alarm = ((cur_time / SECS_PER_DAY + next_day) * SECS_PER_DAY) + sys_ctl.alarm_hour*SECS_PER_HOUR \
                    + sys_ctl.alarm_min*SECS_PER_MINUTE;
    //printf("setam a=0x%lX\n",next_alarm);
    irtcc_write_rtc(WRITE_ALM,next_alarm);
    irtcc_write_ram32(NEXT_ALARM, next_alarm);
    sys_param_write8(PARAM_ALARM_SWITCH, sys_ctl.alarm_switch);
    if (next_alarm > 3155759999UL) {
        sys_ctl.alarm_switch = 0;
    }
}


//�������
#pragma location="TASK_ALARM_SEG"
void task_alarm_clear(void)
{
    t_alarm.ring_times = 0;
}

__near_func bool task_alarm_check(void)
{
    //����
    cur_time_sec = irtcc_read_rtc(READ_RTC);
    //deg_u32("cur0",cur_time_sec);
    //deg_u32("nx0",next_alarm);
    //deg_u8("sw",sys_ctl.alarm_switch);
    if(sys_ctl.alarm_switch) {
        if(cur_time_sec - next_alarm <= SECS_DIFFER_OF_ALARM) {
#if IS_LCD_DISPLAY
            if(task_ctl.work_sta != TASK_ALARM) {
                task_ctl.back_up_work_sta = task_ctl.work_sta;      // ����֮ǰ��״̬
            }
#endif
            task_ctl.work_sta = TASK_ALARM;
            return true;
        }
    }
    return false;
}

#pragma constseg="TASK_ALARM_SEG_CONST"
IAR_CONST char str_show_taskalarm[] = "task_alarm\n";
#pragma constseg=default
//����������
#pragma location="TASK_ALARM_SEG"
void task_alarm(void)
{
    printf(str_show_taskalarm);
    task_alarm_enter();
    while (task_ctl.work_sta == TASK_ALARM) {
        task_alarm_deal_msg();
        task_alarm_event();
        task_alarm_display();  //��ʾʱ��
    }
    task_alarm_exit();
}

#endif

