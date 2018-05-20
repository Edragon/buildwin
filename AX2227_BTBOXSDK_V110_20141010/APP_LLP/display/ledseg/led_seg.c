/*****************************************************************************
 * Module    : Display
 * File      : led_seg.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LED�������ʾ����
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"
#include "task_fm.h"
#include "task_clock.h"
#include "task_record.h"
#include "btapi.h"

#if THEME_SELECT == LEDSEG_THEME_DEFAULT
#include "led_seg.h"
__no_init u8 disp_buf[5];

u16 disp_sel_num;
IAR_IDATA u8 flicker_500ms_cnt;



extern u8 HSFCallActive;//0: no call  1:call active  2:outing call alerting
extern u8 HSFRingActive;

typedef void(*H)(void);

#pragma constseg="DISP_LEDSEG_TBL"
IAR_CONST u8 tbl_ledseg_num[10] =
{
    LEDSEG_0, LEDSEG_1, LEDSEG_2, LEDSEG_3, LEDSEG_4,
    LEDSEG_5, LEDSEG_6, LEDSEG_7, LEDSEG_8, LEDSEG_9,
};

#pragma constseg="DISP_LEDSEG_TBL"
IAR_CONST u8 eq_table[7][3] =
{
    {LEDSEG_N,LEDSEG_O,LEDSEG_R},//NORMAL
    {LEDSEG_P,LEDSEG_O,LEDSEG_P},//POP
    {LEDSEG_R,LEDSEG_O,LEDSEG_C},//ROCK
    {LEDSEG_J,LEDSEG_A,LEDSEG_Z},//JAZZ
    {LEDSEG_C,LEDSEG_L,LEDSEG_A},//CLASSIC
    {LEDSEG_C,LEDSEG_U,LEDSEG_T},//COUNTRY
    {LEDSEG_B,LEDSEG_A,LEDSEG_S},//BASS
};

#pragma constseg="DISP_LEDSEG_TBL"
IAR_CONST u8 play_mode_table[6][3] =
{
    {LEDSEG_A,LEDSEG_L,LEDSEG_L},//ȫ��ѭ�h
    {LEDSEG_O,LEDSEG_N,LEDSEG_E},//����ѭ�h
    {LEDSEG_F,LEDSEG_O,LEDSEG_D},//�ļ��Aѭ�h
    {LEDSEG_R,LEDSEG_D,LEDSEG_O},//�S��
    {LEDSEG_N,LEDSEG_O,LEDSEG_R},//˳�򲥷�
    {LEDSEG_B,LEDSEG_R,LEDSEG_O},//���
};

#pragma constseg=default

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ�����豸
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp_device(void)
{
    if(task_ctl.work_sta == TASK_MUSIC || task_ctl.work_sta == TASK_VOICE) {
        if(fs_cur_dev() == DEVICE_SDMMC) {
            disp_buf[4] |= ICON_SD;
        } else if (fs_cur_dev() == DEVICE_UDISK) {
            disp_buf[4] |= ICON_USB;
        }
    }
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp_num(u16 num)
{
    disp_buf[0] = tbl_ledseg_num[num / 1000];
    disp_buf[1] = tbl_ledseg_num[(num % 1000)/100];
    disp_buf[2] = tbl_ledseg_num[(num % 100)/10];
    disp_buf[3] = tbl_ledseg_num[num % 10];
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ�������ղ���
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_dummy_ledseg(void)
{
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ����������ʱ��ʾ��HI��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG_1"
void menu_welcome_ledseg(void)
{
    disp_buf[1] = LEDSEG_H;
    disp_buf[2] = LEDSEG_I;
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ��������������ʱ��ʾ�Ĳ˵�
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_vol_ledseg(void)
{
    disp_buf[1] = LEDSEG_U;

    //��ʱ����д����Ȼ��װ�ܲ��ã�������������
#if BT_CALL_PRIVATE
    disp_buf[2] = tbl_ledseg_num[sys_ctl.volume / 10];
    disp_buf[3] = tbl_ledseg_num[sys_ctl.volume % 10];
#else
    if((BT_STATE != StateNone) && (HSFCallActive || HSFRingActive || scoflag)){
        //HSFCallActive��HSFRingActive�Ǹ��õı���������ģʽ����ʹ��
        disp_buf[2] = tbl_ledseg_num[sys_ctl.hshf_vol / 10];
        disp_buf[3] = tbl_ledseg_num[sys_ctl.hshf_vol % 10];
    } else {
        disp_buf[2] = tbl_ledseg_num[sys_ctl.volume / 10];
        disp_buf[3] = tbl_ledseg_num[sys_ctl.volume % 10];
    }
#endif

    if(task_ctl.work_sta == TASK_MUSIC) {
        if(u_msc.pause) {
            disp_buf[4] |= ICON_PAUSE;
        } else {
            disp_buf[4] |= (ICON_PLAY | ICON_MP3);
        }
    }
    ledseg_disp_device();
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������FMģʽ����ʾ���ڲ��ŵ�Ƶ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_freq_ledseg(void)
{
    ledseg_disp_num(t_fm.freq / 10);
    if(disp_buf[0] == LEDSEG_0)
        disp_buf[0] = LEDSEG_OFF;
    disp_buf[4] |= ICON_FM;
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������FMģʽ����ʾ���ڲ��ŵ�Ƶ����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_freq_chan_ledseg(void)
{
    ledseg_disp_num(t_fm.ch_sel);
    disp_buf[0] = LEDSEG_OFF;
    disp_buf[1] = LEDSEG_P;
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ��������ʾ���������
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_numsel_ledseg(void)
{
    ledseg_disp_num(disp_sel_num);
    ledseg_disp_device();
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������MP3����ģʽ�²��Ÿ���ʱ����ʾ�˵�
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_play_ledseg(void)
{
    disp_buf[0] = tbl_ledseg_num[u_msc.cur_time.min / 10];
    disp_buf[1] = tbl_ledseg_num[u_msc.cur_time.min % 10];
    disp_buf[2] = tbl_ledseg_num[u_msc.cur_time.sec / 10];
    disp_buf[3] = tbl_ledseg_num[u_msc.cur_time.sec % 10];
    disp_buf[4] |= ICON_PLAY | ICON_MP3 | ICON_DOT2;
    ledseg_disp_device();
}

/*----------------------------------------------------------------------------*/
/**@brief MP3����ģʽ����ͣʱ����ʾ�˵�
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_pause_ledseg(void)
{
    disp_buf[0] = LEDSEG_P;
    disp_buf[1] = LEDSEG_A;
    disp_buf[2] = LEDSEG_U;
    disp_buf[3] = LEDSEG_S;
    disp_buf[4] |= ICON_PAUSE | ICON_MP3;
    ledseg_disp_device();
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������MP3����ģʽ����ʾ���ڲ��ŵĸ�����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_music_num_ledseg(void)
{
    u16 num = (t_msc.music_num > 9999)? 9999 : t_msc.music_num;
    ledseg_disp_num(num);
    ledseg_disp_device();
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������LINEINģʽ��ʾ����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_linein_ledseg(void)
{
    disp_buf[0] = LEDSEG_L;
    disp_buf[1] = LEDSEG_I;
    disp_buf[2] = LEDSEG_N;
    disp_buf[3] = LEDSEG_E;
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������USBDEVģʽ��ʾ����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_pc_ledseg(void)
{
    disp_buf[1] = LEDSEG_P;
    disp_buf[2] = LEDSEG_C;
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������BTģʽ��ʾ����
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_bt_ledseg(void)
{
    disp_buf[1] = LEDSEG_B;
    disp_buf[2] = LEDSEG_T;
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������RTCģʽ��ʾʱ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#if TASK_RTC_EN
#pragma location="DISP_LEDSEG_SEG_1"
void menu_rtc_time_blick_ledseg(void)
{
    if(t_clock.sub_sta == STA_SHOW_CLOCK) {
        disp_buf[4] &= ~ICON_DOT2;
    }
    else if((t_clock.sub_sta == STA_SET_CLOCK) || (t_clock.sub_sta == STA_SET_ALARM)) {
        if(t_clock.set_sta == STA_SET_TIME_HOUR) {
            disp_buf[0] = LEDSEG_OFF;
            disp_buf[1] = LEDSEG_OFF;
        }
        else if(t_clock.set_sta == STA_SET_TIME_MIN) {
            disp_buf[2] = LEDSEG_OFF;
            disp_buf[3] = LEDSEG_OFF;
        }
    }
}

#pragma location="DISP_LEDSEG_SEG"
void menu_rtc_time_ledseg(void)
{
    disp_buf[0] = tbl_ledseg_num[t_time.hour / 10];
    disp_buf[1] = tbl_ledseg_num[t_time.hour % 10];
    disp_buf[2] = tbl_ledseg_num[t_time.minute / 10];
    disp_buf[3] = tbl_ledseg_num[t_time.minute % 10];
    disp_buf[4] |= ICON_DOT2;

    if(task_ctl.work_sta == TASK_SHOW_TIME) {
        if(flicker_500ms_cnt % 2) {
            disp_buf[4] &= ~ICON_DOT2;
        }
    }
    else
    {
        if(flicker_500ms_cnt % 2) {
            menu_rtc_time_blick_ledseg();
        }
    }
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������ALARM��ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#if ALARM_EN
#pragma location="DISP_LEDSEG_SEG"
void menu_alarm_ledseg(void)
{
#if 1
    t_clock.sub_sta = STA_SHOW_CLOCK;
    menu_rtc_time_ledseg();
#else
    disp_buf[0] = tbl_ledseg_num[sys_ctl.alarm_hour / 10];
    disp_buf[1] = tbl_ledseg_num[sys_ctl.alarm_hour % 10];
    disp_buf[2] = tbl_ledseg_num[sys_ctl.alarm_min / 10];
    disp_buf[3] = tbl_ledseg_num[sys_ctl.alarm_min % 10];
    disp_buf[4] |= ICON_DOT2;
#endif
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������ALARM����״̬
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_alarm_status_ledseg(void)
{
    if(sys_ctl.alarm_switch == 0) {
        disp_buf[1] = LEDSEG_O;
        disp_buf[2] = LEDSEG_F;
        disp_buf[3] = LEDSEG_F;
    }
    else if(sys_ctl.alarm_switch == 1) {
        disp_buf[1] = LEDSEG_O;
        disp_buf[2] = LEDSEG_N;
    }
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������EQ��ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_eq_ledseg(void)
{
#if !EQ_DISPLAY_NUM
    disp_buf[1] = eq_table[sys_ctl.eq_num][0];
    disp_buf[2] = eq_table[sys_ctl.eq_num][1];
    disp_buf[3] = eq_table[sys_ctl.eq_num][2];
#else
    disp_buf[1] = LEDSEG_E;
    disp_buf[2] = LEDSEG_Q;
    disp_buf[3] = tbl_ledseg_num[sys_ctl.eq_num];
#endif
    ledseg_disp_device();
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������PLAY MODE��ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_mode_play_ledseg(void)
{
    disp_buf[1] = play_mode_table[sys_ctl.mode_play][0];
    disp_buf[2] = play_mode_table[sys_ctl.mode_play][1];
    disp_buf[3] = play_mode_table[sys_ctl.mode_play][2];
    ledseg_disp_device();
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������MIC��ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#if TASK_MIC_EN
#pragma location="DISP_LEDSEG_SEG"
void menu_mic_ledseg(void)
{
    disp_buf[1] = LEDSEG_R;
    disp_buf[2] = LEDSEG_E;
    disp_buf[3] = LEDSEG_C;
    ledseg_disp_device();
}
#endif

#if LOWER_POWER_DISPLAY_OFF
/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������POWEROFF��ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG_1"
void menu_poweroff_ledseg(void)
{
    disp_buf[1] = LEDSEG_O;
    disp_buf[2] = LEDSEG_F;
    disp_buf[3] = LEDSEG_F;
}
#endif

#if BT_DISP_NUM_TIME
/*----------------------------------------------------------------------------*/
/**@brief �������ʾ����������ģʽ����ʾ���ڲ��������ĺ���
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_phonenumber_ledseg(void)
{
    if(phone_call_number[disp_phone_num_cnt]){
        disp_buf[3] = tbl_ledseg_num[phone_call_number[disp_phone_num_cnt] - '0'];
    }
    if(disp_phone_num_cnt > 0 && phone_call_number[disp_phone_num_cnt - 1]){
        disp_buf[2] = tbl_ledseg_num[phone_call_number[disp_phone_num_cnt - 1] - '0'];
    }
    if(disp_phone_num_cnt > 1 && phone_call_number[disp_phone_num_cnt - 2]){
        disp_buf[1] = tbl_ledseg_num[phone_call_number[disp_phone_num_cnt - 2] - '0'];
    }
    if(disp_phone_num_cnt > 2 && phone_call_number[disp_phone_num_cnt - 3]){
        disp_buf[0] = tbl_ledseg_num[phone_call_number[disp_phone_num_cnt - 3] - '0'];
    }
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ����������ģʽ����ʾͨ��ʱ��
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_on_the_phone_time_ledseg(void)
{
    disp_buf[0] = tbl_ledseg_num[on_the_phone_time / 60 % 100 / 10];
    disp_buf[1] = tbl_ledseg_num[on_the_phone_time / 60 % 10];
    disp_buf[2] = tbl_ledseg_num[on_the_phone_time % 60 / 10];
    disp_buf[3] = tbl_ledseg_num[on_the_phone_time % 60 % 10];
    disp_buf[4] |= ICON_DOT2;
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ��������������ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#if TASK_LOUDSPEAKER_EN
#pragma location="DISP_LEDSEG_SEG"
void menu_mix_ledseg(void)
{
    disp_buf[0] = LEDSEG_O;
    disp_buf[1] = LEDSEG__;
    disp_buf[2] = LEDSEG__;
    disp_buf[3] = LEDSEG_O;
}

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ��������������ʾ
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void menu_mix_step_ledseg(void)
{
    ledseg_disp_num(t_spk.step);
    disp_buf[0] = LEDSEG_O;
    disp_buf[1] = LEDSEG__;
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief �������ʾ������MP3����ģʽ�²��Ÿ���ʱ����ʾ�˵�
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#if RECORD_EN
#pragma location="DISP_LEDSEG_SEG"
void menu_recording_ledseg(void)
{
    u8 min, sec;
    if(t_rec.rec_time < 3600) {
        min = t_rec.rec_time/60;
        sec = t_rec.rec_time%60;
        disp_buf[4] = ICON_DOT2;
    } else {
        min = (t_rec.rec_time/60)/60;
        sec = (t_rec.rec_time/60)%60;
    }

    disp_buf[0] = tbl_ledseg_num[min / 10];
    disp_buf[1] = tbl_ledseg_num[min % 10];
    disp_buf[2] = tbl_ledseg_num[sec / 10];
    disp_buf[3] = tbl_ledseg_num[sec % 10];

    if(flicker_500ms_cnt % 2) {
        if(fs_cur_dev() == DEVICE_SDMMC) {
            disp_buf[4] |= ICON_SD | ICON_DOT2;
        } else if (fs_cur_dev() == DEVICE_UDISK) {
            disp_buf[4] |= ICON_USB | ICON_DOT2;
        }
    }
}

#pragma location="DISP_LEDSEG_SEG"
void menu_no_device_ledseg(void)
{
    disp_buf[1] = LEDSEG_N;
    disp_buf[2] = LEDSEG_O;
}

#pragma location="DISP_LEDSEG_SEG"
void menu_del_ledseg(void)
{
    disp_buf[1] = LEDSEG_D;
    disp_buf[2] = LEDSEG_E;
    disp_buf[3] = LEDSEG_L;
}

#pragma location="DISP_LEDSEG_SEG_1"
void menu_full_ledseg(void)
{
    disp_buf[0] = LEDSEG_F;
    disp_buf[1] = LEDSEG_U;
    disp_buf[2] = LEDSEG_L;
    disp_buf[3] = LEDSEG_L;
}

#pragma location="DISP_LEDSEG_SEG"
void menu_err_ledseg(void)
{
    disp_buf[1] = LEDSEG_E;
    disp_buf[2] = LEDSEG_R;
    disp_buf[3] = LEDSEG_R;
}
#endif

/*----------------------------------------------------------------------------*/
/**@brief �������buff
   @param ��
   @return ��
   @note
*/
/*----------------------------------------------------------------------------*/
#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp_buf_clr(void)
{
    my_memset((u8*)disp_buf,0x00,5);
}

//����ĳЩ����ʱ������ʾ���������dummy����ʡ�ռ�
#if !TASK_RTC_EN
    #define menu_rtc_time_ledseg        menu_dummy_ledseg
#endif

#if !ALARM_EN
    #define menu_alarm_ledseg           menu_dummy_ledseg
    #define menu_alarm_status_ledseg    menu_dummy_ledseg
#endif

#if !TASK_MIC_EN
    #define menu_mic_ledseg             menu_dummy_ledseg
#endif

#if !TASK_LOUDSPEAKER_EN
    #define menu_mix_ledseg             menu_dummy_ledseg
    #define menu_mix_step_ledseg        menu_dummy_ledseg
#endif

#if !RECORD_EN
    #define menu_recording_ledseg       menu_dummy_ledseg
    #define menu_no_device_ledseg       menu_dummy_ledseg
    #define menu_del_ledseg             menu_dummy_ledseg
    #define menu_full_ledseg            menu_dummy_ledseg
    #define menu_err_ledseg             menu_dummy_ledseg
#endif

#if !BT_DISP_NUM_TIME
    #define menu_phonenumber_ledseg         menu_dummy_ledseg
    #define menu_on_the_phone_time_ledseg   menu_dummy_ledseg
#endif

#if !LOWER_POWER_DISPLAY_OFF
    #define menu_poweroff_ledseg        menu_dummy_ledseg
#endif


#pragma constseg=DISP_LEDSEG_CONST
const H menu_ledseg[] =
{
    menu_welcome_ledseg,
    menu_vol_ledseg,
    menu_freq_ledseg,
    menu_freq_chan_ledseg,
    menu_numsel_ledseg,
    menu_play_ledseg,
    menu_pause_ledseg,
    menu_music_num_ledseg,
    menu_linein_ledseg,
    menu_pc_ledseg,
    menu_bt_ledseg,
    menu_rtc_time_ledseg,
    menu_alarm_ledseg,
    menu_alarm_status_ledseg,
    menu_eq_ledseg,
    menu_mode_play_ledseg,
    menu_mic_ledseg,
    menu_poweroff_ledseg,
    menu_phonenumber_ledseg,
    menu_on_the_phone_time_ledseg,
    menu_mix_ledseg,
    menu_mix_step_ledseg,
    menu_recording_ledseg,
    menu_no_device_ledseg,
    menu_del_ledseg,
    menu_full_ledseg,
    menu_err_ledseg,
};
#pragma constseg=default

#pragma location="DISP_LEDSEG_SEG"
void ledseg_disp (u8 menu_number)
{
    void (*p)(void);
    ledseg_disp_buf_clr();
    p = menu_ledseg[menu_number];
    (*p)();
    ledseg_value_set();
}

#endif
