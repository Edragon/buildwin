/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Function  : 用户的IO设置
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_fm.h"

//用户IO初始化
void user_io_init(void)
{
#if LED_GREEN_BLINK
    LED_GREEN_INIT();
#endif

#if LED_BLUE_BLINK
    LED_BLUE_INIT();
#endif

#if (LED_POWER_BLINK && INTERNAL_CHARGE_EN)
    LED_POWER_INIT();
#endif
/*#if(USE_P35_USBPOW_CTL == 1)
    PIE1 |= BIT(6);                 //USB模块供电
    P3DIR |= BIT(5);
    irtcc_write_cfg(LCD_MAP_WCMD, BIT(6));
#elif(!USE_P35_USBPOW_CTL)   */
    //PIE1 &= ~BIT(6);                //MUTE脚,设为数字输出
//#endif

/*#if USB_GPIO_EN
    USB_DPDM_GPIO_EN();
    USBCON1 &= ~(BIT(7)|BIT(5)|BIT(4));        //关掉DP/DM上下拉，DM的100k上拉可不关
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

#if TASK_FM_EN                      //FM IO初始化
    fm_io_init();
#endif

#if MP3BT_DECT_EN && !MP3BT_DECT_REUSE_EN
    MP3BT_DECT_INIT();              //开机MP3-BT检测
#endif*/

#if LINEIN_DETECT_EN
    LINEIN_DETECT_INIT();           //LINEIN检测
#endif

#if EAR_DETECT_EN
    EAR_DETECT_INIT();              //耳机检测
#endif

#if(SD_PORT_SEL == 0)   //单独IO检测SD
    SD_DETECT_INIT();
    PMUXCON0 |= BIT(0);
#elif(SD_PORT_SEL == 1)         //SD CLK复用检测脚初始化
    PMUXCON0 |= BIT(0);
    P2DIR |= BIT(1) | BIT(0);
    P2PU |= BIT(1) | BIT(0);
#elif (SD_PORT_SEL == 2) //单独IO检测SD
     SD_DETECT_INIT();
     PMUXCON0 &= ~BIT(0);
#elif (SD_PORT_SEL == 3)        //SD CLK复用检测脚初始化
    PMUXCON0 &= ~BIT(0);
    P3DIR |= BIT(0);
    P3PU |= BIT(0);
#endif
//SD 独立检测脚初始化
#if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET
    SD_CMD_DIR_IN();
    SD_CMD_PU_10K();
#endif

#if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET || SDCLK_REUSE_ADKEY ||SD_CMD_REUSE_SD_DETECT
    sd_port_reuse();
#endif

#if DYNAMIC_PAUSE_MUTE_EN
    dynamic_pause_mute(1);
#endif

     DCIN_INIT();

#if SDCLK_REUSE_ADKEY
    SD_CLK_DIR_IN();
#endif

   /* BTLDO_DIR_INIT();
    USBPOW_INIT();*/

    USER_OTHER_INIT();
}

//USB检测函蕐
//usb_connected(void);    //DP/DM因潮湿而误检测为PC，用这个函数更好.(DP/DM都开了120K上拉，需要两个同时受潮接地才会误判为PC)
//usb_connected_1(void);  //修正三星充电器DP/DM短接且为1V左右，引起误检测为PC的问题。(仅DM受潮接地即可误判为PC)
__near_func void usb_detect(void)
{
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
            //通信错误才检测拔出..
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
#if SD_CMD_REUSE_SD_DETECT
    if(IS_SD_DETECT_BUSY()) {
        return;
    }
#elif (SD_PORT_SEL == 1) || (SD_PORT_SEL == 3)  //默认复用SDCLK检测SD卡插入。
    if(IS_SD_CHK_BUSY()) {  //复用检测
        return;             //上拉未打开，处于CLK输出模式
    }
#endif
    if (IS_SD_IN()) {
        if (device_insert(DEVICE_SDMMC)) {
            task_ctl.dev_change = 1;
            //printf("SD IN\n");
        }
    }
    else {
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
        }
    } else {
        LINEIN_DETECT_END();
        if (device_remove(DEVICE_LINEIN)) {
        }
    }
}
#endif

#if EAR_DETECT_EN
//检测耳机是否插入
__near_func void earphone_detect(void)
{
#if SDCMD_REUSE_EARDET
    if (IS_EAR_DETECT_BUSY()) {
        return;
    }
#endif

    if(EAR_IS_IN()) {
        if(device_insert(DEVICE_EAR)) {
        }
    } else {
        if(device_remove(DEVICE_EAR)) {
        }
    }
    if (device_is_online(DEVICE_EAR)) {
        SPEAKER_MUTE();
    } else {
        if(!sys_ctl.spk_mute)
        {
            SPEAKER_UNMUTE();
        }
    }
}
#endif

__near_func sd_port_reuse_hook(u8 type)
{
#if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET || SD_CMD_REUSE_SD_DETECT
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



#if WKO_OUTPUT_EN
bool volatile wko_set_kick = false;
bool volatile wko_set_sta = false;
__near_func u8 irtcc_read_cfg_near(u8 cmd)
{
    u8 irtcc_cfg;
    IRTCON |= 0x01;                  //irtcc communicate en
    irtcc_write(cmd);
    irtcc_cfg = irtcc_read();
    IRTCON &= ~(0x01);
    return irtcc_cfg;
}

__near_func void irtcc_write_cfg_near(u8 cmd,u8 irtcc_cfg)
{
    IRTCON |= 0x01;
    irtcc_write(cmd);
    irtcc_write(irtcc_cfg);
    IRTCON &= ~(0x01);
}

__near_func wko_state_set(void)   //1ms中断中调用
{
    if(wko_set_kick &&(!(IRTCON&0x01))){
         wko_set_kick = false;
         u8 wko_sfr = irtcc_read_cfg_near(READ_WKO);
         wko_sfr |= BIT(2)|BIT(3);  ////in&out en
         if(wko_set_sta){
            irtcc_write_cfg_near(WRITE_WKO, wko_sfr|0x80);  //WKO设置为高
         }else{
            irtcc_write_cfg_near(WRITE_WKO, wko_sfr&0x7F);  //WKO设置为低
         }
    }
}
#endif


