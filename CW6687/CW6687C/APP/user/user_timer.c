/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Function  : �û���IO����
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"
#include "task_fm.h"
#include "user_fft_led.h"

#define DEMO_PWN_TEST                   0       //���Ժ����Ƴ���

extern u8 flicker_500ms_cnt;                //��������һЩ���Ƶ���ʾʱ��
bool paly_twice_flag = 0;
u16 twice_cnt = 0;
u16 tmr100ms_ticks;

u8 timer5ms_cnt, timer100ms_cnt, timer1s_cnt;
u8 ocx_show_cnt;      //��������һЩ���Ƶ���ʾʱ��

#if DEMO_PWN_TEST

#define PWN_CYCLE                       128             //PWN����
#define PWN_LEVEL                        24             //һ����������

IAR_CONST u8 tbl_pwn_duty[PWN_LEVEL] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    11, 13, 16, 19, 23, 27, 32, 38, 46, 55,
    66, 79, 95, 128,
};

//��PWNʵ�ֺ�����
struct __TYPE_PWN {
    u8 cnt;
    u8 duty1;
    u8 dir1;
    u8 duty2;
    u8 dir2;
    u8 duty3;
    u8 dir3;
} t_pwn;
#endif

u16 tmr100ms_get_ticks(void)
{
    return tmr100ms_ticks;
}

#if DEMO_PWN_TEST
__near_func void pwn_duty_change(void)
{
    if (t_pwn.dir1) {
        t_pwn.duty1++;
        if (t_pwn.duty1 >= PWN_LEVEL-1) {
            t_pwn.dir1 = 0;
        }
    } else {
        t_pwn.duty1--;
        if (t_pwn.duty1 <= 0) {
            t_pwn.dir1 = 1;
        }
    }

    if (t_pwn.dir2) {
        t_pwn.duty2++;
        if (t_pwn.duty2 >= PWN_LEVEL-1) {
            t_pwn.dir2 = 0;
        }
    } else {
        t_pwn.duty2--;
        if (t_pwn.duty2 <= 0) {
            t_pwn.dir2 = 1;
        }
    }

    if (t_pwn.dir3) {
        t_pwn.duty3++;
        if (t_pwn.duty3 >= PWN_LEVEL-1) {
            t_pwn.dir3 = 0;
        }
    } else {
        t_pwn.duty3--;
        if (t_pwn.duty3 <= 0) {
            t_pwn.dir3 = 1;
        }
    }
}
#endif

//1s ��ʱ�ж�
__near_func void timer1s_hook(void)
{
    timer1s_cnt++;
    if (bt_sleep_cnt) {
        bt_sleep_cnt--;
    }
    put_msg(QSYSTEM_1S);
#if INTERNAL_CHARGE_EN
    charge_detect();
#endif
    if (timer1s_cnt >= 10) {
        timer1s_cnt = 0;
        put_msg(QSYSTEM_10S);
    }
}

//100ms ��ʱ�ж�
__near_func void timer100ms_hook(void)
{
	tmr100ms_ticks++;
    //��ʱ���ε͵���
    task_battery_check();

    if (device_set_testready(UDISK_TEST_READY_TIME)) {
        task_ctl.dev_change = 1;
    }

    timer100ms_cnt++;
#if IS_LEDSEG_DISPLAY
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

    move_ctl.ms_flag = 1;   //100ms��־

#if DEMO_PWN_TEST
    pwn_duty_change();
#endif
}

//50ms ��ʱ�ж�
__near_func void timer50ms_hook(void)
{
    if(!sys_ctl.sleep_flag){
        led_scan();
    }
}

#if DEMO_PWN_TEST
//125us ��ʱ�ж�
__near_func void timer125us_hook(void)
{
    t_pwn.cnt++;
    if (t_pwn.cnt >= PWN_CYCLE) {
        t_pwn.cnt = 0;
    }
    if (t_pwn.cnt < tbl_pwn_duty[t_pwn.duty1]) {
        P2 |= BIT(7);
    } else {
        P2 &= ~BIT(7);
    }
    if (t_pwn.cnt < tbl_pwn_duty[t_pwn.duty2]) {
        P2 |= BIT(6);
    } else {
        P2 &= ~BIT(6);
    }
    if (t_pwn.cnt < tbl_pwn_duty[t_pwn.duty3]) {
        P2 |= BIT(5);
    } else {
        P2 &= ~BIT(5);
    }
}
#endif

#if IS_LEDSEG_7PIN_DISPLAY
//500us ��ʱ�ж�
__near_func void timer500us_hook(void)
{
    ledseg_scan();
}
#endif

//1ms ��ʱ�ж�
__near_func void timer1ms_hook(void)
{
#if LED_REUSE_ADKEY || SDCLK_REUSE_ADKEY
    adc_chk();
#endif

#if IS_LEDSEG_5C7S_DISPLAY
    if(!sys_ctl.sleep_flag) {
        ledseg_scan();
    }
#endif

#if FFT_LED_EN
     fft_led_scan();
#endif

#if WKO_OUTPUT_EN
    wko_state_set();
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
    key_scan();             //����ɨ��
    usb_detect();
    sd_detect();
#if (LINEIN_DETECT_EN && !LINEIN_SEG_REUSE)
     linein_detect();
#endif

#if (EAR_DETECT_EN && !EAR_SEG_REUSE)
     earphone_detect();
#endif

#if FFT_LED_EN
     fft_energy_get();
#endif
}

void timer_init(void)
{
#if DEMO_PWN_TEST
    memset(&t_pwn, 0, sizeof(t_pwn));
    P2DIR &= ~(BIT(7) | BIT(6) | BIT(5));
    P2 &= ~(BIT(7) | BIT(6) | BIT(5));
    t_pwn.duty1 = 1;
    t_pwn.duty2 = 16;
    t_pwn.duty3 = 32;
#endif
    timer1_init();          //��ʱ���������ܳ�ʼ�����Ա�������ʼ�����õ��Ķ�ʱ����ʱ������ʱ��֧����չ
}

