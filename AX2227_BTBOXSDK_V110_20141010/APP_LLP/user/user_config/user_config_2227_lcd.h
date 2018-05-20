/*****************************************************************************
 * Module    : User
 * File      : user_config_2227_lcd.h
 * Author    : caiyue
 * Email     : NC
 * Function  : 用户 USER_2227 LCD 自定义参数设置
 *****************************************************************************/
#ifndef USER_CONFIG_2227_LCD_H
#define USER_CONFIG_2227_LCD_H


/*****************************************************************************
 * Module    : 主菜单及功能相关配置
 *****************************************************************************/
#define TASK_MUSIC_EN                   1   //是否打开MUSIC功能
#define TASK_AUX_EN                     1   //是否打开LINEIN功能
#define TASK_FM_EN                      1   //是否打开FM功能
#define TASK_USBDEV_EN                  1   //是否打开USB设备功能
#define TASK_POWEROFF_EN                1   //是否打开关机功能
#define TASK_BT_EN                      1   //是否打开蓝牙功能
#define TASK_LOUDSPEAKER_EN             1   //是否打开扩音器模块
#define TASK_MIC_EN                     1   //是否打开MIC录音模块
#define TASK_VOICE_EN                   1   //是否打开录音播放模块
#define TASK_RTC_EN                     1   //是否打开RTC功能
#define TASK_KARAOKE_EN                 0   //是否打开卡拉ok功能


/*****************************************************************************
 * Module    : 主题选择
 *****************************************************************************/
//整体主题，必须要有全套的主题才能选。否则请分别定制局部主题
#define THEME_SELECT                    LCD_THEME_DEFAULT


/*****************************************************************************
 * Module    : 系统提示音相关配置
 *****************************************************************************/
#define POWER_ON_VOICE                  1   //是否需要开机提示音
#define VOICE_MODE_WARNING_MUSIC        1   //是否需要录音播放模式提示音
#define MIC_RECORD_WARNING_MUSIC        1   //是否需要录音模式提示音
#define LOUDSPEAKER_WARNING_MUSIC       1   //是否需要扩音器模式提示音
#define CLOCK_MODE_WARNING_MUSIC        1   //是否需要时钟模式提示音


/*****************************************************************************
 * Module    : Music模块相关配置
 *****************************************************************************/
#define AUTO_CHANGE_DEVICE              1   //双设备时自动切换设备
#define K_MODE_CHANGE_DEVICE            1   //双设备时使用模式键切换设备
#define MUSIC_MODE_AUTO                 1   //插入设备，自动切换到音乐播放
#define BREAK_POINT_EN                  1   //断点记忆
#define MUSIC_LRC_EN                    0   //歌词显示，不能同时打开ID3显示
#define MUSIC_ID3_EN                    1   //ID3显示，不能同时打开歌词显示


/*****************************************************************************
 * Module    : FM模块相关配置
 *****************************************************************************/
#define FM_TYPE                         FM_QN8035   //FM类型选择
#define SEEK_DISPLAY_CHANNEL_NUM        1           //搜台过程中显示搜到的频道
#define FM_RECORD_EN                    1           //是否需要FM录音


/*****************************************************************************
 * Module    : 扩音器功能相关配置
 *****************************************************************************/
#define MIX_MODE_DECAY_NOISE            0   //扩音器降噪


/*****************************************************************************
 * Module    : AUX录音功能相关配置
 *****************************************************************************/
#define AUX_RECORD_EN                   1   //AUX是否需要录音
#define KU_PLAY_LINEIN_MUTE_EN          0   //LINEIN模式下是否使用短按播放键MUTE功放(包括PALY 和 PLAY_S10)


/*****************************************************************************
 * Module    : USB设备模块相关配置
 *****************************************************************************/
//USB设备枚举类型
#define USBDEV_ENUM_TYPE                (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)


/*****************************************************************************
 * Module    : RTC相关配置
 *****************************************************************************/
#define ALARM_EN                        1


/*****************************************************************************
 * Module    : 蓝牙相关配置
 *****************************************************************************/
#define BT_TYPE                         BT6639
#define BT_CALL_PRIVATE                 0   //强制私密接听
#define BT_RING_NUMBER                  1   //是否需要来电报号
#define KL_PLAY_DISCONNECT              1   //是否使用长按 PLAY 键断开连接(不包括 PLAY_S10 键)
#define BT_NAME_WITH_ADDR               1   //蓝牙名称是否显示3位地址(调试用，例如：BW-BTBOX-***)
#define NUMBER_KEY_TO_CALL              1   //是否使用数字按键拨号功能


/*****************************************************************************
 * Module    :频谱显示配置
 *****************************************************************************/
#define MUSIC_FREQ_SHOW                 1   //音乐模式下是否需要频谱显示(需要LCD显示)
#define FM_FREQ_SHOW                    1   //FM模式下是否需要频谱显示(需要LCD显示)
#define AUX_FREQ_SHOW                   1   //AUX模式下是否需要频谱显示(需要LCD显示)
#define USBAUDIO_FREQ_SHOW              1   //USB设备模式下是否需要频谱显示(需要LCD显示)
#define BT_FREQ_SHOW                    0   //蓝牙模式下是否需要频谱显示(需要LCD显示)


/*****************************************************************************
 * Module    : LCD的相关配置
 *****************************************************************************/
#define LCD_X_FIX                       0   //是否需要发送地址修正
#define LCD_SOFT_RESET                  1   //是否需要软件复位LCD屏
#define LCD_INVERT_DISPLAY              0   //是否需要设置LCD屏invert寄存器
#define LCD_CONTRAST_DEFAULT            10  //默认的LCD屏对比度(取值范围: 0~15)

//LCD IO 配置
#define LCD_IO_SET
#define LCD_DATA                        5   //P25
#define LCD_CLK                         6   //P26
#define LCD_A0                          0   //P30
#define LCD_RES                         1   //p31
#define LCD_CS                          2   //P32
#define LCD_BACKLIGHT                   5   //p15

//LCD端口设置
#define LCD_PORT_SET
#define LCD_RESET()                     P3DIR &= ~BIT(LCD_RES); P3 &= ~BIT(LCD_RES)
#define LCD_PORT_INIT()                 /*LCD_BACKLIGHT_ON();*/\
                                        PMUXCON0 |= BIT(4); \
                                        P2 |= BIT(LCD_CLK);\
                                        P3 |= BIT(LCD_CS);\
                                        P3 |= BIT(LCD_A0);\
                                        P2DIR &= ~(BIT(LCD_CLK) | BIT(LCD_DATA));\
                                        P3DIR &= ~BIT(LCD_CS);\
                                        P3DIR &= ~BIT(LCD_A0);\
                                        P3DIR &= ~BIT(LCD_RES)

#define SET_LCD_A0                      P3 |= BIT(LCD_A0);
#define CLR_LCD_A0                      P3 &= ~BIT(LCD_A0);
#define SET_LCD_CS                      P3 |= BIT(LCD_CS);
#define CLR_LCD_CS                      P3 &= ~BIT(LCD_CS);
#define SET_LCD_RES                     P3 |= BIT(LCD_RES);
#define CLR_LCD_RES                     P3 &= ~BIT(LCD_RES);

//LCD背光IO相关配置
#define LCD_BACKLIGHT_SET
#define LCD_BACKLIGHT_ON()              if (!sys_ctl.backlight_sta) {sys_ctl.backlight_sta = 1; P1DRV1 |= BIT(7); P1DIR |= BIT(LCD_BACKLIGHT);}
#define LCD_BACKLIGHT_OFF()             if (sys_ctl.backlight_sta)  {sys_ctl.backlight_sta = 0; P1DRV1 &= ~BIT(7);}


/*****************************************************************************
 * Module    : 控件界面的停留时间控制
 *****************************************************************************/
//进度条progressbox的停留时间
#define PROGRESS_TIME                   30  //3秒

//消息框停留时间
#define MSGBOX_TIME                     5   //1.5秒

//在只有提示的情况下，是否使用简单的消息框
#define MSGBOX_SIMPLE                   1


/*****************************************************************************
 * Module    : 系统细节配置
 *****************************************************************************/
#define USE_P35_USBPOW_CTL              0   //P35脚的使用选择。用做MUTE脚时为0，用做控制USB供电时选1，其他用处时为2
#define LED_GREEN_BLINK                 0   //是否使用绿灯闪烁
#define HARD_IR_EN                      1   //红外遥控(NEC硬解)
#define IR_HARD_POWERDOWN_EN            1   //红外POWER键硬关机使能
#define K_HSF_CHANGE_MODE               1   //是否使用 HSF 键切换模式
#define RECORD_EN                       1   //录音功能总开关。为0时关闭所有的录音功能
#define SOFT_POWER_ON_OFF               0   //是否使用软开关机


/*****************************************************************************
 * Module    :AUX通道配置
 *****************************************************************************/
//定制MIC的通道
#define MIC_CHANNEL_SEL                 MCHANNEL_3

//定制FM的通道
#define FM_CHANNEL_SEL                  CHANNEL_00

//选择LINEIN的通道
#define AUX_CHANNEL_SEL                 CHANNEL_72


/*****************************************************************************
 * Module    : 按键及外设IO配置
 *****************************************************************************/
//ADKey的使用，0为使用IOKey，1为1组ADKey，2为2组ADKey
#define USE_ADKEY                       1

//sd_port值:0: P20 P21 P27,    1: P20 P21 P27, clk复用检测,    2: P30 P31 P32,    3: P30 P31 P32, clk复用检测
#define SD_PORT_SEL                     1

//是否使用P01作串口打印调试
#define DEBUG_UART_SEL                  1

//ADKEY键值表
#define KEY1_NUM                        5
#define KEY1_ADC                        0x3F, 0x8C, 0xAC, 0xCA, 0xEA, 0xFF
#define KEY1_VAL                        T_KEY_HSF, T_KEY_PREV, T_KEY_NEXT, T_KEY_VOL_DOWN, T_KEY_VOL_UP, NO_KEY

//是否为接听键，用于中止铃声
#define IS_HSF_KEY(k)                   (k == KU_HSF)

//复用功能键
#define T_KEY_TIME_DOWN                 T_KEY_NEXT
#define T_KEY_TIME_UP                   T_KEY_PREV


/*****************************************************************************
 * Module    : IIC的相关配置
 *****************************************************************************/
#define IIC_IO_SET
#define IIC_DATA                        3   //P13
#define IIC_CLK                         2   //P12

#define IIC_DATA_CLK_OUT()              P1PU0 &= ~(BIT(IIC_DATA) | BIT(IIC_CLK));  P1DIR &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); delay(10)
#define IIC_DATA_IN()                   P1DIR |= BIT(IIC_DATA); P1PU0 |= BIT(IIC_DATA)
#define IIC_DATA_OUT()                  P1PU0 |= BIT(IIC_DATA); P1DIR &= ~BIT(IIC_DATA)
#define IIC_DATA_CLK_HIGH()             P1 |= BIT(IIC_DATA) | BIT(IIC_CLK)
#define IIC_DATA_HIGH()                 P1 |= BIT(IIC_DATA)
#define IIC_DATA_LOW()                  P1 &= ~BIT(IIC_DATA)
#define IIC_CLK_HIGH()                  P1 |= BIT(IIC_CLK)
#define IIC_CLK_LOW()                   P1 &= ~BIT(IIC_CLK)
#define IIC_DATA_IS_HIGH()              (P1 & BIT(IIC_DATA))


/*****************************************************************************
 * Module    :LED IO配置
 *****************************************************************************/
//绿色LED
#define LED_GREEN                       4
#define LED_GREEN_INIT()                P2DIR &= ~BIT(LED_GREEN); P2 &= ~BIT(LED_GREEN)
#define LED_GREEN_ON()                  P2 |= BIT(LED_GREEN)
#define LED_GREEN_OFF()                 P2 &= ~BIT(LED_GREEN)
#define LED_GREEN_TOG()                 P2 ^= BIT(LED_GREEN)
#define LED_GREEN_IS_ON()               (P2 & BIT(LED_GREEN))


/*****************************************************************************
 * Module    : 其它杂项IO相关配置
 *****************************************************************************/
//耳机检测
#define EAR_DETECT_INIT()               P1DIR |= BIT(1);P1PU0 |= BIT(1)
#define IS_EAR_IN()                     (P1 & BIT(1))

//MUTE功放控制
#define MUSIC_MUTE()                    P3DIR &= ~BIT(5); P3 |= BIT(5)
#define MUSIC_UNMUTE()                  P3DIR &= ~BIT(5); P3 &= ~BIT(5)

//软开关机IO检测配置
#define SOFT_POWER_CHK_INIT()           P2DIR |= BIT(2)
#define SOFT_POWER_CHK_IS_ON()          (P2 & BIT(2))

//软开关机IO控制配置
#define SOFT_POWER_CTL_INIT()
#define SOFT_POWER_CTL_ON()             irtcc_write_cfg(RTC_CFG_WCMD,irtcc_read_cfg(RTC_CFG_RCMD) | BIT(4));    //IRTWKO output high
#define SOFT_POWER_CTL_OFF()            irtcc_write_cfg(RTC_CFG_WCMD,irtcc_read_cfg(RTC_CFG_RCMD) & ~BIT(4));   //IRTWKO output controlled by IRTCC alarm function (low)


/*****************************************************************************
 * Module    : 休眠及唤醒IO相关配置
 *****************************************************************************/
/*****************************************************************************
 * IO_SLEEP()和IO_WAKEUP()两个宏是关机时处理引脚状态
 * 1、休眠时，所有引脚设为输入下拉，以下除外：
 *    P35为MUTE脚，高MUTE，保持输出高；
 *    P34为唤醒脚，保持为输入；
 *    P06为ADKEY是外部上拉，保持为输入；
 *    P07为红外引脚，保持输入上拉，才能红外唤醒；
 *    P14/P16/P17为蓝牙RST/RX/TX，不关蓝牙电源时保持原状态，否则设为输入下拉
 * 2、唤醒时，理论上要将所有IO恢复为关机前状态，这里只是先打开SPI；
 *    P33为SPI的CS，恢复为输出；
 *    P04/P05为SPI的DAT/CLK，恢复为输出；
 *    其余IO主要由user_io_init恢复；
*****************************************************************************/
#if POWER_OFF_VPG33
#define IO_SLEEP_P1()                   {P1DIR = 0xff;              /* 关闭蓝牙电源时，RST/RX/TX设为输入下拉，避免IO漏电 */\
                                        P1PD0 = 0xff;}
#else
#define IO_SLEEP_P1()                   {P1DIR |= 0x2f;             /* 除了P14/P16/P17 */\
                                        P1PD0 = 0x2f;}
#endif
#define IO_SLEEP_P023()                 {PIE0 = 0xff;               /* 复用IO设为数字输入 */\
                                        PIE1 = 0xff;\
                                        P0DIR = 0xff;\
                                        P2DIR = 0xff;\
                                        P3DIR = ~BIT(5);            /* 除了P35 */\
                                        P1DRV1 = 0;                 /* 关闭P15上拉 */\
                                        P0PU0 = 0x80;               /* 除了P07 */\
                                        P1PU0 = 0;\
                                        P2PU0 = 0;\
                                        P2PU1 = 0;\
                                        P3PU0 = 0;\
                                        P0PD0 = 0x3f;               /* 除了P06/P07 */\
                                        P2PD0 = 0x55;\
                                        P2PD1 = 0x55;\
                                        P3PD0 = 0xc5;               /* 除了P35、P34 */\
                                        P3PUD1 = 0x07;}

#define IO_SLEEP()                      IO_SLEEP_P023();IO_SLEEP_P1()

#define IO_WAKEUP()                     {P3DIR &= ~BIT(3);          /* 恢复SPI的CS */\
                                        P0DIR &= ~(BIT(4)|BIT(5));  /* 恢复SPI的DAT/CLK */\
                                        P0PD0 = 0x00;               /* 关掉其它下拉 */\
                                        P1PD0 = 0x00;\
                                        P2PD0 = 0x00;\
                                        P2PD1 = 0x00;\
                                        P3PD0 = 0x00;\
                                        P3PUD1 = 0x00;}

#define IS_KEY_WAKEUP()                 !(P0 & BIT(6))


#endif
