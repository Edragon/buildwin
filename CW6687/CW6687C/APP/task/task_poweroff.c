/*****************************************************************************
 * Module    : Task
 * File      : task_poweroff.c
 * Author    : Hanny
 * Function  : 关机任务
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "typedef.h"

#pragma constseg="RTC_SEG_CONST"
IAR_CONST char str_power_down_enter[] = "power_down_enter\n";
#pragma constseg=default

#pragma location="RTC_SEG"
void power_down_enter(void)
{
    u8 wko_cfg = 0;
#if WKO_WKUP_POWERDOWN  //使用上升沿唤醒
    wko_cfg = 0x1c;  //wko pin output en, 60k pull down, pin en & input en
#endif
#if ALARM_WKUP_POWERDOWN     //BIT(5)|BIT(1) ALARM WAKE UP
     wko_cfg |=  BIT(5)|BIT(1);
#endif
    irtcc_write_cfg(WRITE_WKO, 0x00);
    asm("nop");asm("nop");asm("nop");

    WDTCON = 0;
    IE_EA = 0;
    P0DIR = 0xFF;
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P3DIR = 0xFF;
    P0PU = 0;
    P1PU = 0;
    P2PU = 0;
    P3PU = 0;
    u8 vcl = irtcc_read_cfg(READ_VCL);
    //接大于4.2v电压(电池电压)测试时要屏蔽BIT(1)，否则关机后仍有电且无法软开机
    irtcc_write_cfg(WRITE_VCL,vcl|BIT(2)/*| BIT(1)*/ | BIT(0));  //加上BIT(0),解决低电关机后,电压上升到3.4V以上都不能开机的问题
    delay_ms(1);

    irtcc_write_cfg(WRITE_CFG,irtcc_read_cfg(READ_CFG)|BIT(5));
    delay_ms(1);

    irtcc_write_cfg(WRITE_WKO, wko_cfg);
    asm("nop");asm("nop");asm("nop");

    PWRCON1 = 0x00;
    PWRCON2 = 0x00;
    PWRCON3 = 0x00;
    PWRCON4 = 0x00;
    PWRCON5 = 0x00;
#if SOFT_POWER_OFF_20UA
    PWRCON3 &= ~BIT(4);
    while(1){
        irtcc_write_cfg(WRITE_PWR, 0x02);
        delay_ms(1);
        WDTCON = 0x13;      //使用看门狗，每128ms唤醒一次
        irtcc_write_cfg(WRITE_CFG,irtcc_read_cfg(READ_CFG)&(~BIT(5)));
        irtcc_write_cfg(WRITE_PWR, 0x0F);  //RVDD disable
#if SOFT_POWER_ON_OFF //解决硬开关机时低电关机重启的问题
        PWRCON3 |= BIT(4);
#endif
        delay_ms(100);
    }
#elif  SOFT_POWER_OFF_3UA
    //irtcc_write_cfg(WRITE_PWR, 0x02);    //VDDIO disable
    irtcc_write_cfg(WRITE_PWR, 0x0F);    //修正3uA软关机电流过大
    delay_ms(200);
    while(1);
#endif
}

//有DCIN则假关机, 无则进入软关机
#pragma location="RTC_SEG"
void power_down_dcin_check(void)
{
    u16 wko_high_cnt = 0;
    WDTCON = 0;
    IE_EA = 0;
    P0DIR = 0xFF;
    P1DIR = 0xFF;
    P2DIR = 0xFF;
    P3DIR = 0xFF;
    //P0DIR &= ~BIT(1); //开打印
    while(irtcc_read_cfg(READ_STA) & BIT(5)){
        delay(0xFF);
    }
    while(1){
        delay_ms(1);
        if(irtcc_read_cfg(READ_STA) & BIT(5)){
            if (wko_high_cnt++ >800){
                WDTCON = 0x18;  //2ms复位
                while(1);   //WKO(PWR/PP)长按开机
            }
        }else{
             wko_high_cnt = 0;
             DCIN_INIT();
             if(IS_DCIN()){  //假关机,功耗大一点  //接5V电源,没有按PWR时.
                //printf("-");
                continue;    //继续检测WKO
             }else{          //低功耗软关机
                //printf("go down\n");
                power_down_enter();
             }
        }
    }
}

#if TASK_POWEROFF_EN
//任务主流程
#pragma location="TASK_POWER"
void task_poweroff(void)
{
#if WARNING_POWER_OFF
    dac_enable();
    sys_unmute();
    mp3_res_play_wait(RES_MP3_POWER_OFF);
    dac_disable();
#endif
    led_mode_off();
#if INTERNAL_CHARGE_EN
	dac_off(0);  //修正6687B关机拔出usb电源时有啪声
#endif
#if LOWER_POWER_DISPLAY_OFF
#if IS_LEDSEG_DISPLAY
    ledseg_disp(MENU_POWEROFF);
    u8 temp = 1;
    while(temp--){
        WATCHDOG_CLR();
        delay_5ms(200);
    }
#endif
#endif

#if 0//SOFT_POWER_ON_OFF
    if(sys_ctl.lowpower_flag || sys_ctl.soft_poweroff_flag){
        sys_ctl.sleep_flag = 1;
        #if LED_GREEN_BLINK
            LED_GREEN_OFF();
        #endif
        #if LED_BLUE_BLINK
            LED_BLUE_OFF();
        #endif
        #if LED_STATUS_BLINK
            LED_STATUS_OFF();
        #endif

        dac_disable();
        speaker_mute();
        IE_EA = 0;
        WDTCON = 0;
        SOFT_POWER_CTL_OFF();
        while(1);
    }
#endif
#if BT_ALL_TASK
    bt_close();
#endif

#if IS_LCD_DISPLAY
    lcd_off();
#endif

    power_down_dcin_check();
   // power_down_enter();
   // sleep_mode_enter();
}

#endif
