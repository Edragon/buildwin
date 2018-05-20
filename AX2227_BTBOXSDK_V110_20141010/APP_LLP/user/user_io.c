/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的IO设置
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_fm.h"
#include "user_bt.h"
#include "btapi.h"


//用户IO初始化
void user_io_init(void)
{
#if USB_GPIO_EN
    USB_DPDM_GPIO_EN();
    USBCON1 &= ~(BIT(7)|BIT(5)|BIT(4));        //关掉DP/DM上下拉，DM的100k上拉可不关
#endif

#if LED_GREEN_BLINK
    LED_GREEN_INIT();
#endif

#if LED_BLUE_BLINK
    LED_BLUE_INIT();
#endif

#if SOFT_POWER_ON_OFF
    SOFT_POWER_CTL_INIT();
    SOFT_POWER_CHK_INIT();
#endif

    MUSIC_MUTE();
    OPA_CTL_INIT();

#if(USE_P35_USBPOW_CTL == 1)
    PIE1 |= BIT(3);                 //USB模块供电
    P3DIR |= BIT(5);
    irtcc_write_cfg(LCD_MAP_WCMD, BIT(6));
#elif(!USE_P35_USBPOW_CTL)
    PIE1 &= ~BIT(3);                //MUTE脚,设为数字输出
#endif

#if TASK_FM_EN
    fm_io_init();                   //FM IO初始化
#endif

#if MP3BT_DECT_EN && !MP3BT_DECT_REUSE_EN
    MP3BT_DECT_INIT();              //开机MP3-BT检测
#endif

#if LINEIN_DETECT_EN
    LINEIN_DETECT_INIT();           //LINEIN检测
#endif

#if EAR_DETECT_EN
    EAR_DETECT_INIT();              //耳机检测
#endif

#if SD_CARD_EN
  //SD CLK复用检测脚初始化
  #if(SD_PORT_SEL == 0)
    PMUXCON0 |= BIT(0);
  #elif(SD_PORT_SEL == 1)
    PMUXCON0 |= BIT(0);
    P2DIR |= BIT(1) | BIT(0);       //CLK用于SD卡检测
    P2PU0 |= BIT(2) | BIT(0);       //打开上拉
    P2PU0 &= ~BIT(3);
  #elif (SD_PORT_SEL == 2)
    PMUXCON0 &= ~BIT(0);
  #elif (SD_PORT_SEL == 3)
    PMUXCON0 &= ~BIT(0);
    P3DIR |= BIT(0);                //CLK用于SD卡检测
    P3PU0 |= BIT(0);                //打开上拉
    P3PU0 &= ~BIT(1);
  #endif

  //SD 独立检测脚初始化
    SD_DETECT_INIT();
  #if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET
    SD_CMD_DIR_IN();
    SD_CMD_PU_10K();
  #endif

  #if SDCLK_REUSE_ADKEY
    SD_CLK_DIR_IN();
  #endif
#endif


    DCIN_INIT();
    BTLDO_DIR_INIT();
    USBPOW_INIT();
}

//USB检测函蕐
__near_func void usb_detect(void)
{
#if IICCLK_REUSE_USB
    if((USBDPDM & BIT(4)) != 0) {   //dpdm as gpio
        return;
    }
#endif

    u8 usb_conn = usb_connected();

#if TASK_USBDEV_EN
    if (USB_CONNECTED_PC == usb_conn) {
        if (device_insert(DEVICE_PC)) {
            //printf("PC IN\n");
        }
    } else {
        if (device_remove(DEVICE_PC)) {
            //printf("PC OUT\n");
        }
    }
#endif

    if (USB_CONNECTED_UDISK == usb_conn) {
        if (device_insert(DEVICE_UDISK)) {
            task_ctl.dev_change = 1;
            //printf("UDISK IN\n");
        }
    } else {
        if (usb_remove_check()) {
            //通信错误才检测拔出
            if (device_remove(DEVICE_UDISK)) {
                task_ctl.dev_change = 1;
                //printf("UDISK OUT\n");
            }
        }
    }
}

//检测SD卡是否在线
__near_func void sd_detect(void)
{
#if (SD_PORT_SEL == 1) || (SD_PORT_SEL == 3)
    if(IS_SD_CHK_BUSY()) {  //复用检测
        return;             //上拉未打开，处于CLK输出模式
    }
#endif

    if (IS_SD_IN()) {
        SD_DETECT_END();
        if (device_insert(DEVICE_SDMMC)) {
            task_ctl.dev_change = 1;
            //printf("SD IN\n");
        }
    }
    else {
        SD_DETECT_END();
        if (device_remove(DEVICE_SDMMC)) {
            task_ctl.dev_change = 1;
            //printf("SD OUT\n");
        }
    }
}

#if LINEIN_DETECT_EN
//检测LineIn
__near_func void linein_detect(void)
{
#if SDCMD_REUSE_LINEIN_DET || LINEIN_DETECT_REUSE_IICCLK
    if (IS_LINEIN_DETECT_BUSY()) {
        return;
    }
#endif

#if LINEIN_DETECT_REUSE_IICCLK
    LINEIN_DETECT_INIT();
#endif

    if (IS_LINEIN_IN()) {
        LINEIN_DETECT_END();
        if (device_insert(DEVICE_LINEIN)) {
            //printf("LINE IN\n");
        }
    } else {
        LINEIN_DETECT_END();
        if (device_remove(DEVICE_LINEIN)) {
            //printf("LINE OUT\n");
        }
    }
}
#endif

#if EAR_DETECT_EN
//检测耳机是否插入
__near_func void earphone_detect(void)
{
#if SDCMD_REUSE_EARDET || EAR_DETECT_REUSE_IICCLK
    if (IS_EAR_DETECT_BUSY()) {
        return;
    }
#endif

#if EAR_DETECT_REUSE_IICCLK
    EAR_DETECT_INIT();
#endif

    if(IS_EAR_IN()) {
        EAR_DETECT_END();
        if(device_insert(DEVICE_EAR)) {
            //printf("EAR IN\n");
        }
    } else {
        EAR_DETECT_END();
        if(device_remove(DEVICE_EAR)) {
            //printf("EAR OUT\n");
        }
    }
    if (device_is_online(DEVICE_EAR)) {
        MUSIC_MUTE();
    } else {
        mute_ctl_auto();
    }
}
#endif

//判断是否需要MUTE功放，在中断调用
__near_func bool is_sys_mute(void)
{
    return (sys_flag.mute_flag)? true : false;
}

//判断是否需要MUTE功放，在中断调用
__near_func bool is_need_mute(void)
{
    //系统MUTE、音量为0或者非播放状态时，可以MUTE功放
    return (sys_flag.mute_flag || sys_flag.need_mute || sys_ctl.volume == 0 || DAC_VOL_IS_ZERO())? true : false;
}

__near_func void mute_ctl_auto(void)
{
    if((BT_STATE != StateNone) && (HSFCallActive | HSFRingActive | scoflag)) {
        if(HSHF_VOL_IS_ZERO()) {
            MUSIC_MUTE();
        } else {
            MUSIC_UNMUTE();     //通话过程不MUTE
        }
    } else {
        if((!sys_flag.voice_play_flag) && is_need_mute()) {
            MUSIC_MUTE();
        } else {
            MUSIC_UNMUTE();
        }
    }
}

//打印串口选择，1：P01，0：P16
void printf_io_select(u8 iotmp)
{
    if(iotmp)
    {
#if (SYS_PKG == PKG_SSOP24)
        P1DIR |= BIT(6)|BIT(7);
        P1PU0 &= ~(BIT(6)|BIT(7));
        P1PD0 &= ~(BIT(6)|BIT(7));
#endif
        UARTSTA = 0x01;
        P0DIR &= ~BIT(1);
    }
    else
    {
        UARTSTA = 0x00;
        P0DIR |= BIT(1);
        P1DIR &= ~BIT(6);
    }
}

//静音
void music_mute(void)
{
    sys_flag.mute_flag = 1;
}

//非静音，正常播放，因为调用了delay_5ms，必须开中断后才能调用
void music_unmute(void)
{
    if(sys_flag.mute_flag || sys_ctl.volume==0) {//避免重复调用时延时久, 2014-1-2, 系统音量为0时也需要延时
        sys_flag.mute_flag = 0;
        delay_5ms(2);                           //2014-5-9：此处延时过久会导致蓝牙模式下出现一些异常，功放解MUTE需要长延时的，调整到定时中断延时，
    }
}

__near_func void adc_key_kick(void);
__near_func sd_port_reuse_hook(u8 type)
{
#if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET
    if (type == 0 || type == 1) {
        SD_CMD_PU_500R();           //SDCMD复用检测时，开500R上拉通信
    } else {
        SD_CMD_PU_10K();            //空闲时，开10K上拉用于检测
    }
#endif

#if SDCLK_REUSE_ADKEY
    bool ie_ea = IE_EA;
    IE_EA = 0;
    if (type == 0 || type == 1) {
        SD_CLK_DIR_OUT();           //CLK用于时钟输出
    } else {
        SD_CLK_DIR_IN();            //CLK用于ADKEY检测
        SD_CLK_PU_DIS();            //关上拉
        adc_key_kick();
    }
    IE_EA = ie_ea;
#endif
}
