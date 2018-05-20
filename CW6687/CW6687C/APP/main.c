/*****************************************************************************
* Module    : Main
* File      : main.c
* Author    : Hanny
* Function  : 主函数
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_spi.h"
#include "user_fm.h"
#include "user_fft_led.h"

#define MSG_POOL_DEPTH      0x20
u8 msg_pool[MSG_POOL_DEPTH];

void flush_dispensable_msg(void)
{
    u8 msg;
    for(u8 i=0; i<MSG_POOL_DEPTH; i++) {
        msg = get_msg();
        if(msg == Q_NOMSG) {
            break;
        } else if(msg >= 0x60 && msg < 0x7e) {    //重要的系统消息留下
            put_msg(msg);
        }
    }
}

#pragma location="RTC_SEG"
void rtc_poweron_set(void)
{
#if SYS_CRY == CRY_32K
    if(irtcc_read_ram8(FIRST_POWERON_FLAG) == 0xAA) //第一次上电
    {
        irtcc_write_ram8(FIRST_POWERON_FLAG, 0x00);
        //printf("p_set\n");
        memset(&t_time, 0, sizeof(t_time));
#if 1
        t_time.year = 15;//15;
        t_time.hour = 8;//8;
        t_time.minute = 0;//8;
        t_time.second = 0;
        set_rtc_time(); //初始化为2015年1月1日8:00   //根据t_time,写RTC时间counter
        irtcc_write_rtc(WRITE_ALM,0);  //闹钟时间清零
        irtcc_write_ram32(NEXT_ALARM, 0);
        user_set_alarm_time(0,0);
#else  //ALARM TEST, 打开这里，15S会会进入闹钟模式
        t_time.year = 0;//15;
        t_time.hour = 0;//8;
        t_time.minute = 1;
        t_time.second = 45;
        set_rtc_time(); //初始化为2015年1月1日8:00   //根据t_time,写RTC时间counter
        irtcc_write_rtc(WRITE_ALM,2*60);  //闹钟时间清零
        irtcc_write_ram32(NEXT_ALARM,2*60);
        user_set_alarm_time(0,2);
  #if ALARM_EN
        user_set_alarm_switch(1);  //打开闹钟开关
  #endif
#endif

    }
#endif
}

#define FIRST_POWER  2 //WRITE_CFG时注意这一位，用于判断是否是第一次上电
#pragma location="RTC_SEG"
void irtcc_init(void)
{
    PCON2 &= ~BIT(3);
    u8 cfg =irtcc_read_cfg(READ_CFG);
    u8 wko = irtcc_read_cfg(READ_WKO);
    if((cfg & BIT(FIRST_POWER)) ){   //用于第一次上电的判断.(PWRCON的BIT7 PD_FLAG读不出来)
        irtcc_write_ram8(FIRST_POWERON_FLAG, 0xAA);
        irtcc_write_rtc(WRITE_ALM,0);  //闹钟时间清零
        irtcc_write_rtc(WRITE_RTC,0);  //时间清零
        //printf("FON\n");
    }else{
        //printf("NF\n");
    }
    //printf("cfg=0x%X\n\n",cfg);
    //printf("wko=0x%X\n\n",wko);
    u8 wksta = irtcc_read_cfg(READ_STA);
    if(wksta & BIT(0)){  //当前是闹钟唤醒
        sys_ctl.alarm_wakeup_flag = true;
        //printf("WK\n");
    }else{
        sys_ctl.alarm_wakeup_flag = false;
        //printf("NWK\n");
    }
#if IRTCC_WKUP_EN
    u8 irtcc_cfg = irtcc_read_cfg(WK_STA_RCMD);
    irtcc_write_cfg(WRITE_WKO,0x41);     //BIT(6) wko pin output enable  BIT(1) WKO pin wakeup enable
#if SYS_CRY == CRY_32K
    irtcc_write_cfg(WRITE_CFG,0x98);
#elif SYS_CRY == CRY_12M
    irtcc_write_cfg(WRITE_CFG,0x58); //32K:0x98  12M:0x58
#endif
#else

#if SYS_CRY == CRY_32K
    irtcc_write_cfg(WRITE_CFG,0x88);  //here  //0x88
#elif SYS_CRY == CRY_12M
    irtcc_write_cfg(WRITE_CFG,0x48); //32K:0x88  12M:0x48
    //irtcc_write_cfg(OSC_DRV_WCMD,0x51);   //12M
#endif
#endif

#if IRTCC_WKUP_EN
    RTC_ALARM_CLR_PEND();	//clear alarm pending
    //RTC_ALARM_INT_EN();	//alarm int enable
#endif
}

//lib中main函数会调用的打印函数
#pragma location="INIT_SEG"
void uart_init_hook(void)
{
#if DEBUG_UART_SEL
    uart_init();
    my_printf_init(uart_putchar);
#else
    my_printf_init((void (__near_func *)(char))dummy_void);
#endif
}

//系统变量初始化函数
void sys_var_init(void)
{
    memset(&sys_ctl, 0, sizeof(sys_ctl));  //在最前面清零
    task_hid_flag = 0;
    bt_low_scan_flag = 0;
}

//系统初始化函数
void sys_init(void)
{
    PCON1 &= ~(BIT(7) | BIT(6) | BIT(2) | BIT(1) | BIT(0));
    PCON2 &= ~(BIT(6));
    PCON3 &= ~BIT(5);
    PCON1 &= ~BIT(7);
    sys_var_init();
#if SYS_CRY == CRY_32K  //使用外接的32k晶振
    sys_pll_x32k();
#elif SYS_CRY == CRY_2M
    sys_pll_x26m();
#endif

    irtcc_init();
#if IS_LCD_DISPLAY
    LCD_RESET();                //LCD res 防止出现雪花
#endif
    user_io_init();

#if INTERNAL_CHARGE_EN
    charge_init();      //充电初始化
#endif
    scan_init();                            //初始化扫描控制
#if BUCK_MODE_EN
    buck_mode_select();
#endif

#if !DEBUG_UART_SEL
    uart_disable();
#endif
#if IS_LEDSEG_DISPLAY
    ledseg_init();
    ledseg_disp(MENU_POWERON);
#endif
    led_mode_on();
    timer_init();
    spctl_init();
    param_init();                           //上电初始化记忆设备
#if IS_LCD_DISPLAY
    set_lcd_buf(LCD_FULL_MODE); //设置初始化显存
    disp_language_numget();
    lcd_init(); //lcd显示初始化
    disp_clear();
    disp_draw(RES_POWERON_WELCOME_BMP, 5, 2);
    disp_update();
    LCD_BACKLIGHT_ON();
#endif
    rtc_poweron_set();
#if FFT_LED_EN
    fft_led_init();
#endif
    msg_init(msg_pool, MSG_POOL_DEPTH);

#if !INTERNAL_CHARGE_EN
    device_init();
#endif

#if SD_CMD_REUSE_SD_DETECT
    P3PUS0 &= ~BIT(1);
#endif
#if EAR_ADKEY_REUSE
    device_set_delay(DEVICE_EAR,4);  //device_set_delay可以更改设备检测的延时时间
#endif

#if HARD_IR_EN
    ir_hardware_init();
#elif SOFT_IR_EN
    ir_timer2_init();
#endif
    sys_ctl.spk_mute = 1;
    timer5ms_cnt = timer100ms_cnt = timer1s_cnt = 0;
    timer1_hook_enable();                   //初始化完毕，开启定时器扩展，以便各项检测等。
    dac_init(DAC_INIT_MODE);
#if DAC_MIX_OUT_EN
    dac_left_right_mix_out();
#endif
#if VCMBUF_EN
    uart_disable();                         //关闭串口的使用
    dac_vcmbuf_en();                        //使用VCMBUF
#endif
    flush_msg();
    set_sys_clk(SYS_48M);                   //初始化完毕，设置时钟

#if WARNING_POWER_ON
    if (!sys_ctl.mute_flag) {
        sys_unmute();
    }
    dac_enable();
    mp3_res_play_wait(RES_MP3_START);       //播放开机提示音.中调用的EVENT有可能改变work_sta值
    dac_disable();
#endif

#if TASK_BT_EN
#if BT_ALL_TASK
    user_bt_init();
    //开始回连
    bt_connect();
#else
    user_bt_init();
    bt_close();
#endif
#endif

#if ALARM_EN
    if (task_alarm_check()) {
        task_alarm_clear();
    } else {
        if(!sys_ctl.alarm_wakeup_flag){
            task_alarm_set();               //设置闹钟
        }
    }
    next_alarm = irtcc_read_rtc(READ_ALM);
#endif

#if IS_LCD_DISPLAY
    sys_ctl.poweroffcnt = 0xffff;   // 初始化时禁止启动定时关机功能
    reset_backlight();              // 背光时间要重新设置，不然就初值为0
#endif
}

//软开关开机键检测
void soft_power_check(void)
{
    u16 cnt;
    for (cnt = 500; cnt!=0; cnt--) {
        if (!SOFT_POWER_CHK_IS_ON()) {
            //printf("power_off\n");
            WDTCON = 0;
            SOFT_POWER_CTL_OFF();
            while (1);
        }
        delay(2800);
    }
    //printf("power_on\n");
    SOFT_POWER_CTL_ON();
}

#pragma location="INIT_SEG"
void main(void)
{
    IE0 = 0;                //关闭所有中断
    IE1 = 0;
    DPCON = 0;
    IE_EA = 1;
    sys_init();
    run_task();
    while (1);
}

