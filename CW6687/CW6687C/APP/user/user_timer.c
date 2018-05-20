/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Function  : 用户的IO设置
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "task_music.h"
#include "task_fm.h"
#include "user_fft_led.h"

#define DEMO_PWN_TEST                   0       //测试呼吸灯程序

extern u8 flicker_500ms_cnt;                //用来控制一些控制的显示时间
bool paly_twice_flag = 0;
u16 twice_cnt = 0;
u16 tmr100ms_ticks;

u8 timer5ms_cnt, timer100ms_cnt, timer1s_cnt;
u8 ocx_show_cnt;      //用来控制一些控制的显示时间

#if DEMO_PWN_TEST

#define PWN_CYCLE                       128             //PWN周期
#define PWN_LEVEL                        24             //一共几极亮度

IAR_CONST u8 tbl_pwn_duty[PWN_LEVEL] = {
    0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
    11, 13, 16, 19, 23, 27, 32, 38, 46, 55,
    66, 79, 95, 128,
};

//软PWN实现呼吸灯
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

//1s 定时中断
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

//100ms 定时中断
__near_func void timer100ms_hook(void)
{
	tmr100ms_ticks++;
    //暂时屏蔽低电检测
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

    //100MS的事件
    if (ocx_show_cnt != 255) {
        ocx_show_cnt++;
    }

    move_ctl.ms_flag = 1;   //100ms标志

#if DEMO_PWN_TEST
    pwn_duty_change();
#endif
}

//50ms 定时中断
__near_func void timer50ms_hook(void)
{
    if(!sys_ctl.sleep_flag){
        led_scan();
    }
}

#if DEMO_PWN_TEST
//125us 定时中断
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
//500us 定时中断
__near_func void timer500us_hook(void)
{
    ledseg_scan();
}
#endif

//1ms 定时中断
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

//5ms 定时中断
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
    key_scan();             //按键扫描
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
    timer1_init();          //定时器基本功能初始化，以便其它初始化中用到的定时器延时。但此时不支持扩展
}

