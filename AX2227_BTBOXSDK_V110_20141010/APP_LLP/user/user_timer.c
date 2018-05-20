/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �û���IO����
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "btApi.h"
#include "task_music.h"
#include "task_fm.h"

#if THEME_SELECT == LEDSEG_THEME_DEFAULT
extern IAR_IDATA u8 flicker_500ms_cnt;      //��������һЩ���Ƶ���ʾʱ��
#endif
extern __near_func void task_battery_check(void);

IAR_IDATA u8 timer5ms_cnt, timer100ms_cnt;


#if THEME_SELECT == LEDSEG_THEME_DEFAULT && (LEDSEG_TYPE_SELECT == LEDSEG_7P7S_FM || LEDSEG_TYPE_SELECT == LEDSEG_7P7S_MHZ)
IAR_DATA u8 tm2_cnt;     //timer2 ����
__near_func void timer2_process(void);

//��ʼ��timer2Ϊ500us�жϣ�Ҫ��timer2_init֮�����
void timer2_500us_init(void)
{
    TMR2PRL = 0xBB; //0x0BBΪ500us
    TMR2PRH = 0x00;
    TMR2CNTL = 0;   //Clear CNT
    TMR2CNTH = 0;
    tm2_cnt = 0;
}

//500us�ж�
__near_func void timer500us_hook(void)
{
#if LEDSEG_THEME
    ledseg_scan();
#endif
}

//timer2�жϴ�����
__near_func void timer2_isr_do(void)
{
    tm2_cnt++;
    timer500us_hook();      //500us�жϣ����Դ����������飬����������
    if(tm2_cnt >= 2) {      //2*500us = 1ms
        tm2_cnt = 0;
        timer2_process();   //ԭtimer2������������1ms����һ��
    }
}

#endif

//1s ��ʱ�ж�
__near_func void timer1s_hook(void)
{
    tmr1s_ticks++;
    if (bt_sleep_cnt) {
        bt_sleep_cnt--;
    }
#if (THEME_SELECT == LEDSEG_THEME_DEFAULT || THEME_SELECT == LCD_THEME) && BT_DISP_NUM_TIME
    if(BT_STATE == OnThePhone){
        on_the_phone_time ++;
        show_update();
#if LCD_THEME
        bt_status_flag = 1;
#endif
    }
#endif
    put_msg(QSYSTEM_1S);
}

//100ms ��ʱ�ж�
__near_func void timer100ms_hook(void)
{
	tmr100ms_current_ticks++;
    task_battery_check();
    if (device_set_testready()) {
        task_ctl.dev_change = 1;
    }

    if(send_ppkey_countdown){
        send_ppkey_countdown--;
    }

    timer100ms_cnt++;
#if THEME_SELECT == LEDSEG_THEME_DEFAULT
    if (timer100ms_cnt % 5 == 0) {
        flicker_500ms_cnt++;
    }
#endif
    if (timer100ms_cnt >= 10) {
        timer100ms_cnt = 0;
        timer1s_hook();
    }
    //100MS���¼�
    if (ocx_show_cnt != 255) {
        ocx_show_cnt++;
    }
#if LCD_THEME
    move_ctl.ms_flag = 1;   //100ms��־
    freq_show_update = 1;   //����Ƶ��
#endif
}

//50ms ��ʱ�ж�
__near_func void timer50ms_hook(void)
{
    led_scan();
}
u8 bt_debug_sco;


//1ms ��ʱ�ж�
__near_func void timer1ms_hook(void)
{
    tmr1ms_current_ticks++;

#if LED_REUSE_ADKEY || SDCLK_REUSE_ADKEY
    adc_chk();
#endif
    if(bt_debug_sco < 6) {
        bt_debug_sco++;
    }

    //if( flag_btmode )// && (bt_voice_state == BT_VMS_PLAYING || bt_uart_rxlen | scoflag) )
    if(!(AGCDMACON & BIT(2)))
    {
        KICK_SOFT_INT();
    }
#if LEDSEG_THEME && LEDSEG_TYPE_SELECT == LEDSEG_5C7S_MHZ
    ledseg_scan();
#endif
}

//5ms ��ʱ�ж�
__near_func void timer5ms_hook(void)
{
    timer5ms_cnt++;
    if (timer5ms_cnt == 10) {
        timer50ms_hook();
    } else if (timer5ms_cnt >= 20) {
        timer50ms_hook();
        timer5ms_cnt = 0;
        timer100ms_hook();
    }
    if (flag_btmode) {
        if(avdtp_sbc_active){
            avdtp_sbc_active--;
            if(avdtp_sbc_active == 0){
                if(avdtp_play_status && state_a2dp_player == 1){   // still play...
                    //printf("fsm(%x)\n", uartRxQ2_check_fsm);
                    if(!uartRxQ2_check_fsm){
                        uartRxQ2_check_fsm = RQ2_FSM_KICK0; // start
                    }
                }
                avdtp_sbc_reset();
            }
        }
        if(bt_voice_delay_cnt){
            bt_voice_delay_cnt--;
        }
    }

#if SD_DECT_REUSE_LED
    SD_DETECT_INIT();
#endif

    //5MS���¼�
    key_scan();             //����ɨ��

#if !USB_GPIO_EN
    usb_detect();
#endif
#if SD_CARD_EN
    sd_detect();
#endif

#if (LINEIN_DETECT_EN && (!LINEIN_SEG_REUSE))
    linein_detect();
#endif

#if EAR_DETECT_EN
    earphone_detect();
#else
    mute_ctl_auto();
#endif

}

