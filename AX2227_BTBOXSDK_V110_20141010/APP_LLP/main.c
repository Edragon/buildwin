/*****************************************************************************
* Module    : Main
* File      : main.c
* Author    : Hanny
* Email     : coldney@163.com
* Function  : 主函数
*****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_spi.h"
#include "btapi.h"
#include "user_fm.h"


/*****************************************************************************
 * 中断优先级说明：
 * 最高：UART1中断，接收蓝牙TX和RX数据(不能被阻塞超过8us，使能位为IE1的bit6)
 * 次高：Timer2 1ms中断，处理UART1接收的数据(不能被阻塞超过1ms)
 * 最低：软中断和解码中断
 *
 * 开关中断注意事项：
 * 1、处理短时间临界条件时，可以关总中断IE_EA，处理完后恢复。
 *    具体步骤可参考led_5c7s.c中的led_7p7s_value_set()
 * 2、处理较长时间的临界条件时，可以关掉除UART1中断以外的中断，然后恢复。
 *    具体步骤可参考led_7p7s.c中的led_7p7s_value_set()函数。
 * 3、一定不能只关IE_TM2(Timer2中断)，否则会出现优先级反转，导致低优先级
 *    的软中断和解码中断阻塞Timer2中断。
*****************************************************************************/


#define MSG_POOL_DEPTH      0x20
u8 msg_pool[MSG_POOL_DEPTH];

extern u32 serial_id;
extern IAR_IDATA u8 timer5ms_cnt, timer100ms_cnt;
extern u8 __data banknum;
IAR_BIT_A bool freq_show_update;
IAR_DATA u8 ocx_show_cnt;      //用来控制一些控制的显示时间
u8 dac_vdd_hp_flag;

void bt_init(void);
void bt_uart_var_init(void);
void bt_param_init(void);
void bt_shutdown(void);
bool sbc_load_code(u16 addr, u8 segnum);
void sd_port_reuse(void);



//初始化变量。先按48M来初始化各项时钟
#pragma location="INIT_SEG"
void sys_clk_init(void)
{
#if SYS_CRY == CRY_RC
    CLKCON2 &= 0x3f;    //关闭12M和32K晶振
    sys_pll_rc();
#elif SYS_CRY == CRY_32K
    CLKCON2 &= 0xbf;    //关闭12M晶振
    irtcc_write_cfg(OSC_DRV_WCMD,0xD7); //晶振引脚内部电容配置为21pf，并增强32.768K晶振驱动能力，不需要外挂电容
    sys_pll_x32k();
#elif SYS_CRY == CRY_12M
    CLKCON2 &= 0x7f;    //关闭32K晶振
    irtcc_write_cfg(OSC_DRV_WCMD,0xD7); //晶振引脚内部电容配置为21pf，不需要外挂电容
    sys_pll_x12m();
#else
    #error "请选择一种时钟源"
#endif
}

void task_init(void)
{
    task_ctl.last_work = TASK_NULL;
    task_ctl.work_sta = TASK_IDLE;
#if MP3BT_DECT_EN
#if MP3BT_DECT_REUSE_EN
    MP3BT_DECT_INIT();
#endif
    if(MP3BT_DECT_IS_BT()) {
        task_ctl.work_sta = TASK_BT;
    }
    MP3BT_DECT_END();
#endif

#if POWERON_TO_FM
    if(task_ctl.work_sta == TASK_IDLE && fm_init()) {
        fm_off();   //若FM_VOUT不经过主控且插着SD卡或U盘开机时会导致FM没关掉，从而在音乐模式下FM的声音一直在
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
}

void test_init(void)
{
    PCON1 &= ~(BIT(7) | BIT(6) | BIT(2) | BIT(1) | BIT(0));
    PCON2 &= ~(BIT(6));
    PCON3 &= ~BIT(5);
    PCON1 &= ~BIT(7);
#if LCD_THEME
    LCD_RESET();                //LCD res 防止出现雪花
#endif
    irtcc_init();

    memset(&sys_ctl, 0, sizeof(sys_ctl));
    sys_flag.all = 0;
    flag_btmode = 0;
    sys_flag.mute_flag = 1;
    sys_ctl.lowpower_cnt = 50;
#if LOWPOWER_WARNING
    sys_ctl.lowpower_warning_flag = 0;
#endif
#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E || SYS_PKG == PKG_QFN48_6680M)
    dac_vdd_hp_flag = 1;        //DACVDD和DACHP短接
#else
    dac_vdd_hp_flag = 0;        //DACVDD和DACHP独立引脚
#endif
    work_sta_interrupt = 0;
    timer5ms_cnt = 0;
    timer100ms_cnt = 0;
    sound_detect_disable();

#if LEDSEG_THEME
    ledseg_init();              //数码管初始化，放在timer2_init之前防止鬼影
    ledseg_disp(MENU_POWERON);
#endif
    led_init();

    bt_uart_var_init();
    timer2_init();              //定时器基本功能初始化，以便其它初始化中用到的定时器延时。但此时不支持扩展
#if THEME_SELECT == LEDSEG_THEME_DEFAULT && (LEDSEG_TYPE_SELECT == LEDSEG_7P7S_FM || LEDSEG_TYPE_SELECT == LEDSEG_7P7S_MHZ)
    void timer2_500us_init(void);
    timer2_500us_init();
#endif
#if SOFT_IR_EN
    ir_timer1_init();
#endif
#if LCD_THEME
    set_lcd_buf(LCD_FULL_MODE); //设置初始化显存
    disp_language_numget();
#endif
    spi_param_init();           //上电初始化记忆设备
    msg_init(msg_pool, MSG_POOL_DEPTH);
#if LCD_THEME
    lcd_init(); //lcd显示初始化
    disp_clear();
    disp_draw(RES_POWERON_WELCOME_BMP, 5, 2);
    disp_update();
    LCD_BACKLIGHT_ON();
#endif

    device_var_init();      //设备控制变量初始化
#if !USB_GPIO_EN
    usb_init();             //USB初始化
#endif
#if SD_CARD_EN
    sd_init(SD_PORT_SEL);   //SD初始化
#endif

    task_init();

#if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET || SDCLK_REUSE_ADKEY
    sd_port_reuse();
#endif

#if HARD_IR_EN
    ir_hardware_init();
#endif
    device_set_delay(DEVICE_SDMMC, SD_DETECT_DELAY);

    spi_music_init();
    scan_init();                        //初始化扫描控制
    timer2_hook_enable();               //初始化完毕，开启定时器扩展，以便各项检测等。

#if DAC_VCMBUF_EN
    dac_init_vcmbuf();
#else
    dac_init();
#endif

#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E)
    dac_set_mono();                     //设置DAC为单声道
#endif
    flush_msg();
    set_sys_clk(SYS_48M);               //初始化完毕，设置时钟

#if ALARM_EN
    sys_ctl.next_alarm = irtcc_read_ram32(NEXT_ALARM);
    sys_ctl.alarm_switch = irtcc_read_ram8(ALARM_SWITCH);
    if (sys_ctl.alarm_switch > 1) {
        sys_ctl.alarm_switch = 0;
    }
    if (task_alarm_check()) {
        task_alarm_clear();
    } else {
        task_alarm_set();               //设置闹钟
    }
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

void test_bt(void)
{
    bt_uart_init();
    //P3DIR |= BIT(3);  // CTS
    memset((u8 *)0x8900, 0, 0x1700);        //清空BT数据区
    bt_power_up();
    BT_STATE = StateIdle;
}

//V099改进：SD升级后会自动复位，而且可在show_sdupd显示升级结果
//因为通常SD升级过程功放被MUTE住，无法确认是否升级成功
void show_sdupd(void)
{
    u8 tmp, tmp1, tmp2;
    tmp1 = *(u8 *)0x3efc;
    *(u8 *)0x3efc = 0x00;
    tmp1 ^= *(u8 *)0x3efd;
    tmp2 = *(u8 *)0x3efe;
    tmp2 ^= *(u8 *)0x3eff;
    tmp = tmp1 & tmp2;
    tmp1 = *(u8 *)0x3efd;
    tmp2 = *(u8 *)0x3efe;
    if(tmp == 0xff && (tmp1 == 0xaa || tmp1 == 0x99) && tmp2 < 10) {
        //printf("sdupd: %02x\n", tmp2);
        switch(tmp2) {
        case 0x00://升级成功
        case 0x03://升级文件与板上程序相同
        case 0x05://强制升级文件与板上程序相同
        case 0x07://强制升级成功
            //printf("sdupd ok\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_OK);
            break;
        case 0x01://升级文件大小错误
        case 0x02://升级文件一致性错误
        case 0x04://升级失败
        case 0x06://强制升级失败
            //printf("sdupd fail\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_FAIL);
            break;
        case 0x08://升级文件不存在蓝牙信息
            //printf("no btinf\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_FAIL);
            break;
        case 0x09://存在两个升级文件
            //printf("two files\n");
            //dac_enable();
            //mp3_res_play_wait(RES_SDUPD_FAIL);
            break;
        }
    }
}

#pragma constseg="INIT_CONST"
IAR_CONST char str_showsid[] = "SID:%lu\n";
IAR_CONST char str_start[] = "\nStart\n";
#pragma constseg=default

#pragma location="INIT_SEG"
void main(void)
{
#if !SD_CARD_EN
    sd_init_none();         //SD空初始化，此后SD引脚可当GPIO用
#endif
    user_io_init();         //IO上电初始化

    printf_io_select(1);    //打印口选择P01
#if !DEBUG_UART_SEL
    uart_disable();
#endif
    printf(str_start);
    printf(str_showsid, serial_id);

#if SOFT_POWER_ON_OFF
    soft_power_check();     //软开机键检测
#endif

    IE0 = 0;                //关闭所有中断
    IE1 = 0;
    DPCON = 0;
    IE_EA = 1;

    bt_init();              // 蓝牙一些需要在timer前初始化的全局变量
    test_init();

    sbc_load_code(DCODE_BT_SBC, 3);
    ENTER_DCODE();

#if POWER_ON_VOICE
    dac_enable();
    mp3_res_play_wait(RES_MP3_START);
#endif

    show_sdupd();           //显示SD升级结果

    run_task();
    while (1) {
    }
}
