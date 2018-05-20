/*****************************************************************************
 * Module    : Task
 * File      : task_poweroff.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 关机任务
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "rtc.h"


extern void poweroff_display(void);
/*****************************************************************************
 * holdmode关机注意事项：
 * 1、因为关机模式会关SPI，无法切bank，所以
 *    syspll_resume、io_sleep3、io_wakeup这3个函数必须加"#pragma inline=forced"声明。
 * 2、若有功放MUTE，可以打开POWER_OFF_DAC，完全关闭DAC，可减小功耗。
 * 3、若关机时关闭蓝牙电源，需要将RST/RX/TX要设为低电平或输入下拉，避免IO漏电
*****************************************************************************/


#define POWER_OFF_DAC       0       //是否需要彻底关闭DAC，有啪声需要MUTE功放


#pragma inline=forced
void syspll_resume(void)
{
#if SYS_CRY == CRY_RC
    PLLCON |= 0x0f;     //pll source is rc, pll enable
    PLLINTH = 0x00;
    PLLINTL = ((*(u8 __code *)0x7ff4)>>5) + 3; //存放值+3;
    PLLFRACH = *(u8 __code *)0x7ff6;
    PLLFRACL = *(u8 __code *)0x7ff5;
    delay(0x12);        //需要延时1ms
    PLLCON |= 0x0d;     //pll source is rc, pll analog enable
    delay(0x12);        //需要延时1ms
    PLLCON |= 0x02;     //pll digital enable
    CLKCON1 &= 0xf0;    //PLL output 48M,系统跑pll
#elif SYS_CRY == CRY_12M
    PLLCON = 0x00;    //0x08: pll digital & analog disable, pll clock source sel x32k
    PLLFRACH = 0x00;
    PLLFRACL = 0x00;
    PLLINTH = 0x00;//(5>>8) & 0xFF;    //1831
    PLLINTL = 0x05;//5 & 0xFF;         //1831

    PLLCON |= 0x02; // pll digital enable
    PLLCON |= 0x01; // pll analog enable
    delay(0x12);    //需要延时1ms
    PLLCON |= 0x02; // pll digital enable(重复设置一次)
    delay(0xC0);    //需要延时10ms
#elif SYS_CRY == CRY_32K
    PLLCON = 0x0b;
#endif
    delay(20);
}

#pragma location="TASK_POWER"
void hold_mode_enter(void)
{
    u8 usbcon0, usbcon1, usbdpdm;
    u8 clkcon, sdcon0, adccon;
#if POWER_OFF_DAC
    u8 atcon0, atcon1, atcon2;
#endif

    IE_EA = 0;                  //关闭总中断
    MUSIC_MUTE();               //MUTE功放
    adccon = ADCCON;
    usbcon0 = USBCON0;
    usbcon1 = USBCON1;
    usbdpdm = USBDPDM;
    clkcon = CLKCON;
    sdcon0 = SDCON0;
    dac_disable();              //关闭DAC
    ADCCON = 0;                 //关闭ADC
    at_write(AUADCON1, 0x00);
    USBCON0 = 0x40;             //关闭USB
    USBCON1 = 0x00;
    USBDPDM = 0x00;
    SDCON0 = 0;                 //关闭SD
    SPI0CON = 0;                //关闭SPI
    SPI1CON = 0;
    FMAMCON0 = 0;

#if POWER_OFF_DAC
    atcon0 = ATCON0;            //彻底关闭DAC，需要MUTE功放
    atcon1 = ATCON1;
    atcon2 = ATCON2;
    ATCON0 = 0;
    ATCON1 = 0;
    ATCON2 = 0X05;
#endif

    IO_SLEEP();                 //设置IO为使整机保持在低功耗的状态

#if POWER_OFF_VPG33
    PWRCON1 &= ~BIT(4);         //关闭VPG33
    P3DIR &= ~BIT(6);           //P36和VPG33是同个引脚，所以要设为低电平
    P3 &= ~BIT(6);
    PWKEN = 0x0f;               //关闭IO唤醒
#else
    if(!sys_flag.lowpower_flag && STANDBY_WAKEUP_EN && task_ctl.last_work == TASK_BT){
        PWK_INIT();             //自动休眠时，设置P34下降沿唤醒
    }
#endif

    CLKCON &= ~0x03;            //关PLL之前, 切到内部rc
    asm("nop");asm("nop");asm("nop");
    PLLCON = 0;                 //关闭PLL
    asm("nop");asm("nop");asm("nop");

    if(!sys_flag.lowpower_flag && STANDBY_WAKEUP_EN) {
        WDTCON = 0x13;          //自动休眠时，看门狗128ms中断唤醒
    } else {
        WDTCON = 0;             //低电或者软关机时关闭看门狗
    }

    while(1) {

        PCON0 |= BIT(1);        //进入hold_mode
        asm("nop");asm("nop");asm("nop");

        WATCHDOG_CLR();

#if IR_HARD_POWERDOWN_EN
        if(IRCON0 & BIT(2)){
            IRCON0 &= ~BIT(2);
            if(IRDAT2 == IR_PWR_DAT){
                break;
            }
        }
#endif

        if((IS_KEY_WAKEUP() || PWK_IS_PEND()) && STANDBY_WAKEUP_EN){
            break;
        }
    }

#if POWER_OFF_VPG33
    PWRCON1 |= BIT(4);          //恢复VPG33
#endif
#if POWER_OFF_DAC
    ATCON0 = atcon0;
    ATCON1 = atcon1;
    ATCON2 = atcon2;
#endif

    syspll_resume();            //恢复系统时钟
    CLKCON = clkcon;

    IO_WAKEUP();                //恢复IO方向及上下拉

    PCON0 = 0x00;
    PWKEN = 0x0f;               //关闭IO唤醒
    SPI0CON = 0x80;
    SPI1CON = 0xb1;
    ADCCON = adccon;
    USBCON0 = usbcon0;
    USBCON1 = usbcon1;
    USBDPDM = usbdpdm;
    SDCON0 = sdcon0;

    WATCHDOG_INIT();
    user_io_init();
    led_init();
#if DEBUG_UART_SEL
    printf_io_select(1);        //打印口选择P01
#endif
#if LEDSEG_THEME
    ledseg_init();
#endif
    if(task_ctl.last_work == TASK_BT) {
#if LED_BLUE_BLINK              //蓝牙模式蓝灯优先
        LED_BLUE_ON();
#elif LED_GREEN_BLINK
        LED_GREEN_ON();
#endif
    } else {
#if LED_GREEN_BLINK             //其他模式绿灯优先
        LED_GREEN_ON();
#elif LED_BLUE_BLINK
        LED_BLUE_ON();
#endif
    }

    dac_enable();
    user_change_volume(sys_ctl.volume);
    task_ctl.work_sta = task_ctl.last_work;
    sys_flag.need_mute = 0;     //唤醒后要再动态检测是否需要MUTE
    sys_flag.ir_power_flag = 0;
    sys_ctl.sleep_cnt = 0;

#if IR_HARD_POWERDOWN_EN || STANDBY_WAKEUP_EN
    sys_ctl.wakeup_delay = KEY_LONG_TIMES+2;
#endif

    IE_EA = 1;                  //开总中断
#if !USB_GPIO_EN
    usb_host_reactive();        //如果U盘在线，则需要重新枚举才能正常使用
#endif
#if SD_CARD_EN
    sd_reactive();
#endif
    flush_msg();                //清空消息池
}

#if TASK_POWEROFF_EN
//任务主流程
#pragma location="TASK_POWER"
void task_poweroff(void)
{
#if LCD_THEME
    set_lcd_buf(LCD_FULL_MODE);
    LCD_BACKLIGHT_OFF();
#endif
#if LOWER_POWER_DISPLAY_OFF
    poweroff_display();
    #if !LOWPOWER_MUSIC         //没有低电提示音时，加延时显示OFF
        u8 temp = 3;
        while(temp--){
            WATCHDOG_CLR();
            delay_5ms(200);
        }
    #endif
#endif

#if LOWPOWER_MUSIC
    if(sys_flag.lowpower_flag){
        dac_enable();
        mp3_res_play_wait(RES_MP3_PLEASE_CHARGE);
        dac_disable();
    }
#endif

#if SOFT_POWER_ON_OFF
    IE_EA = 0;
    dac_disable();
    MUSIC_MUTE();
    draw_clear();
    WDTCON = 0;
    SOFT_POWER_CTL_OFF();       //有软开关时，主控断电
    while(1);
#else
    hold_mode_enter();          //无软开关时，进入holdmode
#endif
}

#endif


/*****************************************************************************
 * powerdown关机注意事项：
 * 1、因为关机模式会关SPI，无法切bank，所以
 *    irtcc_write_cfg_temp函数必须加"#pragma inline=forced"声明。
 * 2、关机时若关闭VDDIO，此时LDOIN必须不超过4.2V，否则存在烧IC的风险。
*****************************************************************************/
//必须用inline声明，防止切bank
#pragma inline=forced
void irtcc_write_cfg_temp(u8 cmd,u8 irtcc_cfg)
{
    RTCON |= 0x01;          //enable IRTCC communications
    RTCDAT = cmd;           //RTCC communication command
    while (RTCON & 0x02);   //等待命令传输完毕
    RTCDAT = irtcc_cfg;     //RTCC communication data
    while (RTCON & 0x02);   //等待数据传输完毕
    RTCON &= ~(0x01);       //disable IRTCC communications
}

//关闭内部LDO，进入低功耗不需要唤醒
#pragma location="TASK_POWER"
void power_down_enter(void)
{
    IE_EA = 0;                  //关闭总中断
    MUSIC_MUTE();               //MUTE功放
    dac_disable();              //关闭DAC
    ADCCON = 0;                 //关闭ADC
    at_write(AUADCON1, 0x00);
    USBCON0 = 0x40;             //关闭USB
    USBCON1 = 0x00;
    USBDPDM = 0x00;
    SDCON0 = 0;                 //关闭SD
    SPI0CON = 0;                //关闭SPI
    SPI1CON = 0;
    FMAMCON0 = 0;

    ATCON0 = 0;
    ATCON1 = 0;
    ATCON2 = 0X05;

    IO_SLEEP();                 //设置IO为使整机保持在低功耗的状态

    PWRCON1 &= ~BIT(4);         //关闭VPG33
    P3DIR &= ~BIT(6);           //P36和VPG33是同个引脚，所以要设为低电平
    P3 &= ~BIT(6);
    PWKEN = 0x0f;               //关闭IO唤醒

    //配置IRTWKO引脚唤醒(下降沿)，进入睡眠时IRTWKO引脚会开内部下拉维持低电平，唤醒时外部给个高电平后松开即可产生下降沿
    //irtcc_write_cfg_temp(RTC_CFG_WCMD,0x88);//X32K En //Normal RTC //bit4=0, Wake up Pin out put0, 相当打开下拉了.
    //irtcc_write_cfg_temp(WK_EN_WCMD,0x02);  //bit1=1: WKO_WK_EN(Hight Wake_Up)

    CLKCON &= ~0x03;            //关PLL之前, 切到内部rc
    asm("nop");asm("nop");asm("nop");
    PLLCON = 0;                 //关闭PLL
    asm("nop");asm("nop");asm("nop");
    WDTCON = 0;                 //关闭看门狗

    while(1){
        irtcc_write_cfg_temp(PWR_CTL_WCMD,0xBF);    //关机(0xBF:关VDDIO,此时LDOIN必须不超过4.2V，否则存在烧IC的风险)
        //irtcc_write_cfg_temp(WK_EN_WCMD,0x00);      //Clear wakeup pending
        //irtcc_write_cfg_temp(WK_EN_WCMD,0x02);      //bit1=1: WKO_WK_EN(Hight Wake_Up)
    }
}
