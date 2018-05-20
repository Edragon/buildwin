/*****************************************************************************
 * Module    : User
 * File      : user_config_cola.h
 * Author    : jingfa
 * Email     :
 * Function  : 用户 USER_COLA 自定义参数设置
 *****************************************************************************/
#ifndef USER_CONFIG_COLA_H
#define USER_CONFIG_COLA_H


/*****************************************************************************
 * Module    : 系统配置
 *****************************************************************************/
#define SYS_CRY                         CRY_12M  //系统时钟源选择


/*****************************************************************************
 * Module    : 主菜单及功能相关配置
 *****************************************************************************/
#define TASK_MUSIC_EN                   1   //是否打开MUSIC功能
#define TASK_AUX_EN                     1   //是否打开LINEIN功能
#define TASK_FM_EN                      1   //是否打开FM功能
#define TASK_USBDEV_EN                  1   //是否打开USB设备功能
#define TASK_POWEROFF_EN                1   //是否打开关机功能
#define TASK_BT_EN                      1   //是否打开蓝牙功能


/*****************************************************************************
 * Module    : 主题选择
 *****************************************************************************/
//整体主题，必须要有全套的主题才能选。否则请分别定制局部主题
#define THEME_SELECT                    NO_THEME


/*****************************************************************************
 * Module    : Music模块相关配置
 *****************************************************************************/
#define AUTO_CHANGE_DEVICE              1   //双设备时自动切换设备
#define K_MODE_CHANGE_DEVICE            1   //双设备时使用模式键切换设备
#define MUSIC_MODE_AUTO                 1   //插入设备，自动切换到音乐播放
#define BREAK_POINT_EN                  1   //断点记忆


/*****************************************************************************
 * Module    : FM模块相关配置
 *****************************************************************************/
#define FM_TYPE                         FM_QN8065   //FM类型选择
#define KU_PLAY_TO_SEEKCHANNEL          1           //是否使用短按播放键来搜台( 包括PALY 和 PLAY_S10)


/*****************************************************************************
 * Module    : USB设备模块相关配置
 *****************************************************************************/
//USB设备枚举类型
#define USBDEV_ENUM_TYPE                (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)


/*****************************************************************************
 * Module    : 蓝牙相关配置
 *****************************************************************************/
#define BT_TYPE                         BT6639
#define BT_CALL_PRIVATE                 0   //强制私密接听
#define BT_RING_NUMBER                  1   //是否需要来电报号
#define KU_PLAY_ANSWER_AND_HANGUP       1   //是否使用短按播放键来接听和挂断电话(包括PALY 和 PLAY_S10)
#define KL_PLAY_HANDUP                  1   //是否使用长按PLAY键挂断电话
#define BT_NAME_WITH_ADDR               1   //蓝牙名称是否显示3位地址(调试用，例如：BW-BTBOX-***)


/*****************************************************************************
 * Module    : 系统细节配置
 *****************************************************************************/
#define USE_P35_USBPOW_CTL              0   //P35脚的使用选择。用做MUTE脚时为0，用做控制USB供电时选1，其他用处时为2
#define IIC_REUSE_SD                    1   //IIC接口是否需要与SD复用P20 P27
#define EAR_DETECT_EN                   1   //是否使用耳机检测
#define LED_BLUE_BLINK                  1   //是否使用蓝灯闪烁
#define LED_GREEN_BLINK                 0   //是否使用绿灯闪烁


/*****************************************************************************
 * Module    : 按键及外设IO配置
 *****************************************************************************/
//ADKey的使用，0为使用IOKey，1为1组ADKey，2为2组ADKey
#define USE_ADKEY                       0

//是否使用另外两个IO做单独的MODE和PLAY键
#define USE_MODE_PLAY_KEY               1

//sd_port值:0: P20 P21 P27,    1: P20 P21 P27, clk复用检测,    2: P30 P31 P32,    3: P30 P31 P32, clk复用检测
#define SD_PORT_SEL                     0

//是否使用P01作串口打印调试
#define DEBUG_UART_SEL                  1


/*****************************************************************************
 * Module    :IO按键定义
 *****************************************************************************/
#define K_PLAY_INIT()                   P0DIR |= BIT(6); P0PU0 |= BIT(6)
#define IS_K_PLAY()                     ((P0 & BIT(6)) == 0)
#define K_NEXT_INIT()                   {P3DIR |= (BIT(0)|BIT(2));\
                                        P3PU0 |= (BIT(0)|BIT(4));\
                                        P3PU0 &= ~(BIT(1)|BIT(5));}
#define IS_K_NEXT()                     ((P3 & BIT(0)) == 0)
#define K_PREV_INIT()
#define IS_K_PREV()                     ((P3 & BIT(2)) == 0)

//是否为接听键，用于中止铃声
#define IS_HSF_KEY(k)                   (k == KU_PLAY_S10)


/*****************************************************************************
 * Module    :AUX通道配置
 *****************************************************************************/
//定制MIC的通道
#define MIC_CHANNEL_SEL                 MCHANNEL_3

//定制FM的通道
#define FM_CHANNEL_SEL                  CHANNEL_00

//定制AUX的通道
#define AUX_CHANNEL_SEL                 CHANNEL_22


/*****************************************************************************
 * Module    : IIC的相关配置
 *****************************************************************************/
#define IIC_IO_SET
#define IIC_DATA                        7       //P27
#define IIC_CLK                         0       //P20
#define IIC_DATA_CLK_OUT()              P2PU1 &= ~BIT(6); P2PU0 &= ~BIT(0);  P2DIR &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); delay(10)
#define IIC_DATA_IN()                   P2DIR |= BIT(IIC_DATA); P2PU1 |= BIT(6)
#define IIC_DATA_OUT()                  P2PU1 |= BIT(6); P2DIR &= ~BIT(IIC_DATA)
#define IIC_DATA_CLK_HIGH()             P2 |= BIT(IIC_DATA) | BIT(IIC_CLK)
#define IIC_DATA_HIGH()                 P2 |= BIT(IIC_DATA)
#define IIC_DATA_LOW()                  P2 &= ~BIT(IIC_DATA)
#define IIC_CLK_HIGH()                  P2 |= BIT(IIC_CLK)
#define IIC_CLK_LOW()                   P2 &= ~BIT(IIC_CLK)
#define IIC_DATA_IS_HIGH()              (P2 & BIT(IIC_DATA))


/*****************************************************************************
 * Module    :LED IO配置
 *****************************************************************************/
//蓝色LED
#define LED_BLUE                        5
#define LED_BLUE_INIT()                 P2DIR &= ~BIT(LED_BLUE); P2 &= ~BIT(LED_BLUE)
#define LED_BLUE_ON()                   P2 |= BIT(LED_BLUE)
#define LED_BLUE_OFF()                  P2 &= ~BIT(LED_BLUE)
#define LED_BLUE_TOG()                  P2 ^= BIT(LED_BLUE)
#define LED_BLUE_IS_ON()                (P2 & BIT(LED_BLUE))


/*****************************************************************************
 * Module    : 蓝牙相关IO配置
 *****************************************************************************/
//蓝牙电源控制
#define BTLDO_POWIO_SET
#define BTLDO_DIR_INIT()                PWRCON1 &= ~BIT(4); P3DIR &= ~BIT(6); P3 &= ~BIT(6)
#define BTLDO_EN()                      P3 |= BIT(6)
#define BTLDO_DIS()                     P3 &= ~BIT(6)


/*****************************************************************************
 * Module    : 其它杂项IO相关配置
 *****************************************************************************/
//耳机检测
#define EAR_DETECT_INIT()               {P2DIR |= BIT(4);P2PU1 |= BIT(0);P2PU1 &= ~BIT(1);}
#define IS_EAR_IN()                     (P2 & BIT(4))

//MUTE功放控制
#define MUSIC_MUTE()                    P3DIR &= ~BIT(5); P3 |= BIT(5)
#define MUSIC_UNMUTE()                  P3DIR &= ~BIT(5); P3 &= ~BIT(5)

//AB/D类功放控制
#define OPA_CTL_INIT()                  {P2DIR &= ~BIT(3);P2 |= BIT(3);}
#define OPA_CTL_HIGH()                  P2 |= BIT(3)
#define OPA_CTL_LOW()                   P2 &= ~BIT(3)

//SD检测脚配置(复用 ADKEY 或 SD_CLK 检测时不需要定义SD_DETECT_INIT)
#define SD_DETECT_INIT()                P2DIR |= BIT(2);P2PU0 |= BIT(4);P2PU0 &= ~BIT(5);
#define IS_SD_IN()                      !(P2 & BIT(2))


/*****************************************************************************
 * Module    : 休眠及唤醒IO相关配置
 *****************************************************************************/
#define IO_SLEEP()                      {P0 = 0x00;P1 = 0xb0;P2 = 0x00;\
                                        P0DIR = 0xfd;P1DIR = 0xbf;P2DIR = 0xdf;P3DIR = 0xbf;}
#define IO_WAKEUP()                     {P0 = 0x7e;P1 = 0xd7;P2 = 0xcb;P3 = 0xef;\
                                        P0DIR = 0xd1;P1DIR = 0xbf;P2DIR = 0x5e;P3DIR = 0x97;}
#define IS_KEY_WAKEUP()                 !(P3 & BIT(2)) || !(P3 & BIT(0)) || !(P0 & BIT(6)) || !(P1 & BIT(2)) || !(P1 & BIT(3))


#endif
