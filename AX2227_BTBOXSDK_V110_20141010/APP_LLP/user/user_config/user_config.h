/*****************************************************************************
 * Module    : User
 * File      : user_config.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 默认用户参数设置，用户自定义功能请分别在相应的功能下进行修改
 *****************************************************************************/
#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "user_cfgdef.h"

/*****************************************************************************
 * Module    : 用户选择
 *****************************************************************************/
#define USER_2227                       0x00        //NO.13-138,2227DEMO板
#define USER_S10                        0x01        //NO.13-146
#define USER_LEPA                       0x02        //NO.13-153
#define USER_COLA                       0x03        //NO.13-157,可乐罐子
#define USER_2226_S10                   0x04        //NO.13-161,兼容1006
#define USER_2226_DEMO                  0x05        //NO.14-xxx,带U盘,FM独立晶振
#define USER_2226_DEMO2                 0x06        //NO.14-xxx,带U盘,省FM晶振
#define USER_2226_S10_SIMPLE            0x07        //NO.14-115,兼容1006,精简版S10
#define USER_6681E_DEMO                 0x08        //NO.14-135,带U盘，焊接6681E芯片
#define USER_6680E_S10                  0x09        //NO.14-135,不带U盘，焊接6680E芯片
#define USER_6680E_S10_SIMPLE           0x0A        //NO.14-147,不带U盘，焊接6680E芯片 6680E_S10_SIMPLE
#define USER_6680M                      0x0B        //NO.14-136,6680M
#define USER_6682M                      0x0C        //NO.14-136,6682M
#define USER_6682E                      0x0D        //NO.14-152B,6682E
#define USER_2227_LCD                   0x0E        //NO.14-xxx,点阵LCD显示,AX2227 CW6639 BT Boombox LCD DISPLAY V1.0.pdf


#define USER_SEL                        USER_2227

//加载用户自定义配置
#if (USER_SEL == USER_2227)
    #include "user_config_2227.h"
#elif (USER_SEL == USER_S10)
    #include "user_config_S10.h"
#elif (USER_SEL == USER_LEPA)
    #include "user_config_lepa.h"
#elif (USER_SEL == USER_COLA)
    #include "user_config_cola.h"
#elif (USER_SEL == USER_2226_S10)
    #include "user_config_2226_S10.h"
#elif (USER_SEL == USER_2226_DEMO)
    #include "user_config_2226_demo.h"
#elif (USER_SEL == USER_2226_DEMO2)
    #include "user_config_2226_demo2.h"
#elif (USER_SEL == USER_2226_S10_SIMPLE)
    #include "user_config_2226_S10_SIMPLE.h"
#elif (USER_SEL == USER_6681E_DEMO)
    #include "user_config_6681E_DEMO.h"
#elif (USER_SEL == USER_6680E_S10)
    #include "user_config_6680E_S10.h"
#elif (USER_SEL == USER_6680E_S10_SIMPLE)
    #include "user_config_6680E_S10_SIMPLE.h"
#elif (USER_SEL == USER_6680M)
    #include "user_config_6680M.h"
#elif (USER_SEL == USER_6682M)
    #include "user_config_6682M.h"
#elif (USER_SEL == USER_6682E)
    #include "user_config_6682E.h"
#elif (USER_SEL == USER_2227_LCD)
    #include "user_config_2227_lcd.h"
#endif


/*****************************************************************************
 * Module    : 系统配置
 *****************************************************************************/
//主控封装选择
#ifndef SYS_PKG
#define SYS_PKG                         PKG_LQFP48
#endif

//系统时钟源选择
#ifndef SYS_CRY
#define SYS_CRY                         CRY_32K
#endif

//0：升级时使用P06，1：升级时使用P13，2：使用P30，其他：暂不支持
#ifndef MAKECODE_ID
#define MAKECODE_ID                     0   //默认用P06，一般不改动
#endif

//0: 不需要SD卡升级  1: 需要SD卡升级(P2口)  2: 需要SD卡升级(P3口)
#ifndef SD_UPDATE_ID
#define SD_UPDATE_ID                    1
#endif

//0：无语音资源，1：中文语音资源，2：英文语音资源，0x80以上用户自定义资源
#ifndef MP3RES_ID
#define MP3RES_ID                       0x02    //默认用英文语音资源
#endif

//对齐COD文件，0：不对齐，由BT_NAME或烧写器指定蓝牙名字，1：对齐(单位1M Byte)，由bt_data.bin指定蓝牙地址和名字
#ifndef ALIGN_COD
#define ALIGN_COD                       0
#endif


/*****************************************************************************
 * Module    : 主菜单及功能相关配置
 *****************************************************************************/
//是否打开MUSIC功能
#ifndef TASK_MUSIC_EN
#define TASK_MUSIC_EN                   0
#endif

//是否打开FM功能
#ifndef TASK_FM_EN
#define TASK_FM_EN                      0
#endif

//是否打开LINEIN功能
#ifndef TASK_AUX_EN
#define TASK_AUX_EN                     0
#endif

//是否打开蓝牙功能
#ifndef TASK_BT_EN
#define TASK_BT_EN                      0
#endif

//USB设备任务
#ifndef TASK_USBDEV_EN
#define TASK_USBDEV_EN                  0
#endif

//是否打开关机功能
#ifndef TASK_POWEROFF_EN
#define TASK_POWEROFF_EN                0
#endif

//是否打开MIC录音模块
#ifndef TASK_MIC_EN
#define TASK_MIC_EN                     0
#endif

//是否打开录音播放模块
#ifndef TASK_VOICE_EN
#define TASK_VOICE_EN                   0
#endif

//是否打开扩音器模块
#ifndef TASK_LOUDSPEAKER_EN
#define TASK_LOUDSPEAKER_EN             0
#endif

//是否打开RTC功能
#ifndef TASK_RTC_EN
#define TASK_RTC_EN                     0
#endif

//是否打开卡拉ok模块
#ifndef TASK_KARAOKE_EN
#define TASK_KARAOKE_EN                 0
#endif

//IDLE
#ifndef TASK_IDLE_EN
#define TASK_IDLE_EN                    0
#endif


/*****************************************************************************
 * Module    : 主题选择
 *****************************************************************************/
//整体主题，必须要求有下列的全套才能选。否则请用下面的局部主题来定制
#ifndef THEME_SELECT
#define THEME_SELECT                    LEDSEG_THEME_DEFAULT
#endif

//音乐播放
#if TASK_MUSIC_EN
#ifndef THEME_SELECT_MUSIC
#define THEME_SELECT_MUSIC              THEME_SELECT
#endif
#endif

//收音机
#if TASK_FM_EN
#ifndef THEME_SELECT_FM
#define THEME_SELECT_FM                 THEME_SELECT
#endif
#endif

//蓝牙
#if TASK_BT_EN
#ifndef THEME_SELECT_BT
#define THEME_SELECT_BT                 THEME_SELECT
#endif
#endif

//AUX & RECORD
#if (TASK_AUX_EN || TASK_MIC_EN)
#ifndef THEME_SELECT_RECORD
#define THEME_SELECT_RECORD             THEME_SELECT
#endif
#endif

//USB设备
#if TASK_USBDEV_EN
#ifndef THEME_SELECT_USBDEV
#define THEME_SELECT_USBDEV             THEME_SELECT
#endif
#endif

//时钟
#if TASK_RTC_EN
#ifndef THEME_SELECT_CLOCK
#define THEME_SELECT_CLOCK              THEME_SELECT
#endif
#endif

//闹钟
#if ALARM_EN
#ifndef THEME_SELECT_ALARM
#define THEME_SELECT_ALARM              THEME_SELECT
#endif
#endif

//扩音器
#if TASK_LOUDSPEAKER_EN
#ifndef THEME_SELECT_SPEAKER
#define THEME_SELECT_SPEAKER            THEME_SELECT
#endif
#endif

//升级程序
#ifndef THEME_SELECT_UPDATEFW
#define THEME_SELECT_UPDATEFW           THEME_SELECT
#endif

//关机
#ifndef THEME_SELECT_POWEROFF
#define THEME_SELECT_POWEROFF           THEME_SELECT
#endif

//ledseg屏类型选择
#ifndef LEDSEG_TYPE_SELECT
#define LEDSEG_TYPE_SELECT              LEDSEG_5C7S_MHZ
#endif


/*****************************************************************************
 * Module    : 主题相关参数
 *****************************************************************************/
#define LCD_THEME                       ((THEME_SELECT >= 1) && (THEME_SELECT < 0x80))
#define LCDSEG_THEME                    ((THEME_SELECT >= 0x80) && (THEME_SELECT < 0xC0))
#define LEDSEG_THEME                    (THEME_SELECT >= 0xC0)


/*****************************************************************************
 * Module    : 系统提示音相关配置
 *****************************************************************************/
//是否需要使用固定提示音音量(默认使用，若不使用则提示音音量跟随系统音量)
#ifndef CONSTANT_WARNING_VOLUME_EN
#define CONSTANT_WARNING_VOLUME_EN      1
#endif

//提示音音量(CONSTANT_WARNING_VOLUME_EN 打开时此宏才有效)
#ifndef WARNING_VOLUME
#define WARNING_VOLUME                  10
#endif

//是否需要开机提示音
#ifndef POWER_ON_VOICE
#define POWER_ON_VOICE                  0
#endif

//是否需要音乐模式提示音
#ifndef MUSIC_MODE_WARNING_MUSIC
#define MUSIC_MODE_WARNING_MUSIC        0
#endif

//是否需要U盘或SD卡提示音
#ifndef USB_SD_WARNING_MUSIC
#define USB_SD_WARNING_MUSIC            1
#endif

//是否需要FM模式提示音
#ifndef FM_WARNING_MUSIC
#define FM_WARNING_MUSIC                1
#endif

//是否需要AUX模式提示音
#ifndef AUX_WARNING_MUSIC
#define AUX_WARNING_MUSIC               1
#endif

//是否需要录音播放模式提示音
#ifndef VOICE_MODE_WARNING_MUSIC
#define VOICE_MODE_WARNING_MUSIC        0
#endif

//是否需要录音模式提示音
#ifndef MIC_RECORD_WARNING_MUSIC
#define MIC_RECORD_WARNING_MUSIC        0
#endif

//是否需要扩音器模式提示音
#ifndef LOUDSPEAKER_WARNING_MUSIC
#define LOUDSPEAKER_WARNING_MUSIC       0
#endif

//是否需要时钟模式提示音
#ifndef CLOCK_MODE_WARNING_MUSIC
#define CLOCK_MODE_WARNING_MUSIC        0
#endif

//是否需要USB设备模式提示音
#ifndef USB_DEVICE_WARNING_MUSIC
#define USB_DEVICE_WARNING_MUSIC        0
#endif

//是否需要来电报号提示音
#ifndef BT_RING_NUMBER
#define BT_RING_NUMBER                  0
#endif

//是否需要蓝牙模式提示音
#ifndef BT_MODE_MUSIC
#define BT_MODE_MUSIC                   1
#endif

//是否需要蓝牙连接提示音
#ifndef BT_CONNECT_MUSIC
#define BT_CONNECT_MUSIC                1
#endif

//是否需要蓝牙断开连接提示音
#ifndef BT_DISCONNECT_MUSIC
#define BT_DISCONNECT_MUSIC             1
#endif

//是否需要蓝牙模式退出提示音
#ifndef BT_MODE_EXIT_MUSIC
#define BT_MODE_EXIT_MUSIC              0
#endif

//是否需要低电提示音
#ifndef LOWPOWER_MUSIC
#define LOWPOWER_MUSIC                  1
#endif

//是否需要音量调节到最大或最小时提示音
#ifndef VOL_MAXMIN_MUSIC
#define VOL_MAXMIN_MUSIC                0
#endif

//音量调节提示音只响一次或一直响直到按键松开(默认一直响)(VOL_MAXMIN_MUSIC打开时此宏才有效)
#ifndef VOL_MUSIC_ONCE
#define VOL_MUSIC_ONCE                  0
#endif

//是否需要按键音
#ifndef KEY_VOICE_EN
#define KEY_VOICE_EN                    0
#endif


/*****************************************************************************
 * Module    : MUSIC模块相关配置
 *****************************************************************************/
//是否需要自动切换设备(只有1个播放设备时不需要此功能)
#ifndef AUTO_CHANGE_DEVICE
#define AUTO_CHANGE_DEVICE              0
#endif

//是否使用模式键切换设备(只有1个播放设备时不需要此功能)
#ifndef K_MODE_CHANGE_DEVICE
#define K_MODE_CHANGE_DEVICE            0
#endif

//插入设备，自动切换到音乐播放
#ifndef MUSIC_MODE_AUTO
#define MUSIC_MODE_AUTO                 0
#endif

//断点记忆
#ifndef BREAK_POINT_EN
#define BREAK_POINT_EN                  0
#endif

//歌词显示，不能同时打开ID3显示
#ifndef MUSIC_LRC_EN
#define MUSIC_LRC_EN                    0
#endif

//是否需要优先播放U盘
#ifndef FIRST_PLAY_UDISK
#define FIRST_PLAY_UDISK                0
#endif

//是否需要SD卡播放功能
#ifndef SD_CARD_EN
#define SD_CARD_EN                      1
#endif

//AB复读(未调试)
#ifndef AB_REPEAT_EN
#define AB_REPEAT_EN                    0
#endif

//音乐模式下是否需要频谱显示(需要LCD显示)
#ifndef MUSIC_FREQ_SHOW
#define MUSIC_FREQ_SHOW                 0
#endif

//是否需要歌曲ID3显示(需要LCD显示，不能同时打开歌词显示)
#ifndef MUSIC_ID3_EN
#define MUSIC_ID3_EN                    0
#endif

//是否需要MP3歌曲ID3显示(需要LCD显示)
#ifndef MP3_ID3_EN
#define MP3_ID3_EN                      1
#endif

//是否需要WMA歌曲ID3显示(需要LCD显示)
#ifndef WMA_ID3_EN
#define WMA_ID3_EN                      1
#endif

//是否需WAV要歌曲ID3显示(需要LCD显示)
#ifndef WAV_ID3_EN
#define WAV_ID3_EN                      1
#endif


/*****************************************************************************
 * Module    : FM任务相关配置
 *****************************************************************************/
//FM类型选择
#ifndef FM_TYPE
#define FM_TYPE                         FM_RDA5807
#endif

//是否需要FM录音
#ifndef FM_RECORD_EN
#define FM_RECORD_EN                    0
#endif

//FM模式下是否使用短按播放键MUTE功放(包括PALY 和 PLAY_S10)
#ifndef KU_PLAY_FM_MUTE_EN
#define KU_PLAY_FM_MUTE_EN              0
#endif

//是否使用短按播放键来搜台(包括PALY 和 PLAY_S10，关闭此宏则会默认使用长按播放键来搜台)
#ifndef KU_PLAY_TO_SEEKCHANNEL
#define KU_PLAY_TO_SEEKCHANNEL          0
#endif

//FM模式下调节音量时是否需要切BANK(主要用于解决调节音量时SPI干扰声)
#ifndef FM_SET_VOL_CHANGE_BANK
#define FM_SET_VOL_CHANGE_BANK          1
#endif

//搜台过程中是否显示搜到的频道(默认显示频点)
#ifndef SEEK_DISPLAY_CHANNEL_NUM
#define SEEK_DISPLAY_CHANNEL_NUM        0
#endif

//FM_VOUT是否经过主控
#ifndef FM_VOUT_THROUGH_MCU
#define FM_VOUT_THROUGH_MCU             1
#endif

//FM模式下是否需要自动进入休眠待机(KU_PLAY_FM_MUTE_EN打开时此宏才有效)
#ifndef FM_AUTO_STANDBY_EN
#define FM_AUTO_STANDBY_EN              0
#endif

//FM模式下是否需要声音能量检测
#ifndef FM_SOUND_DETECT_EN
#define FM_SOUND_DETECT_EN              0
#endif

//FM模式下是否需要频谱显示(需要LCD显示)
#ifndef FM_FREQ_SHOW
#define FM_FREQ_SHOW                    0
#endif


/*****************************************************************************
 * Module    : AUX录音功能相关配置
 *****************************************************************************/
//LINEIN模式下是否使用短按播放键MUTE功放(包括PALY 和 PLAY_S10)
#ifndef KU_PLAY_LINEIN_MUTE_EN
#define KU_PLAY_LINEIN_MUTE_EN          1
#endif

//AUX是否需要录音
#ifndef AUX_RECORD_EN
#define AUX_RECORD_EN                   0
#endif

//AUX模式下是否需要自动进入休眠待机(KU_PLAY_LINEIN_MUTE_EN打开时此宏才有效)
#ifndef AUX_AUTO_STANDBY_EN
#define AUX_AUTO_STANDBY_EN             0
#endif

//AUX模式下是否使用短按上下曲按键调节音量
#ifndef AUX_KU_NEXT_PREV_VOL
#define AUX_KU_NEXT_PREV_VOL            0
#endif

//AUX通道是否使用AGC放大(如果觉得AUX音量不够大时可以打开此宏)
#ifndef AUX_AGC_AMPLIFY
#define AUX_AGC_AMPLIFY                 0
#endif

//AUX模式下是否需要声音能量检测
#ifndef AUX_SOUND_DETECT_EN
#define AUX_SOUND_DETECT_EN             0
#endif

//AUX模式下是否需要频谱显示(需要LCD显示)
#ifndef AUX_FREQ_SHOW
#define AUX_FREQ_SHOW                   0
#endif


/*****************************************************************************
 * Module    : LOUDSPEAKER功能相关配置
 *****************************************************************************/
//扩音器降噪
#ifndef MIX_MODE_DECAY_NOISE
#define MIX_MODE_DECAY_NOISE            0
#endif


/*****************************************************************************
 * Module    : USB设备功能相关配置
 *****************************************************************************/
//USB设备枚举类型
#ifndef USBDEV_ENUM_TYPE
#define USBDEV_ENUM_TYPE                (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)
#endif

//USB Audio调音量模式。1为直接使用音量键调整，0为先按音量键，再用Prev与Next键进行调整
#ifndef USBAUDIO_VOLUME_MODE
#define USBAUDIO_VOLUME_MODE            1
#endif

//USB设备模式下是否需要频谱显示(需要LCD显示)
#ifndef USBAUDIO_FREQ_SHOW
#define USBAUDIO_FREQ_SHOW              0
#endif


/*****************************************************************************
 * Module    : 蓝牙相关配置
 *****************************************************************************/
//是否只有BT功能
#ifndef TASK_BT_ONLY
#define TASK_BT_ONLY                    0
#endif

//蓝牙型号
#ifndef BT_TYPE
#define BT_TYPE                         BT6639
#endif

//蓝牙串口IO选择，0=P16 P17，1=P36 P37
#ifndef BT_UART_IO
#define BT_UART_IO                      0
#endif

//蓝牙唤醒IO选择，0=P34，1=P07
#ifndef BT_PWK_SEL
#define BT_PWK_SEL                      0
#endif

//通话时MIC音量
#ifndef MIC_GAIN_MAX
#define MIC_GAIN_MAX                    200 //参考值，160~255
#endif

//远端声音能量值大于该值时MUTE本地MIC(此值过大时回音严重，过小时通话会断断续续)
#ifndef VAD_THRESHOLD
#define VAD_THRESHOLD                   7   //参考值，6~8
#endif

//远端声音能量值小于该值时打开本地MIC(此值过大时回音严重，过小时通话会断断续续)
#ifndef VAD_THRESHOLD_MUTE
#define VAD_THRESHOLD_MUTE              7   //参考值，6~8
#endif

//是否强制使用私密接听(手机端接听)
#ifndef BT_CALL_PRIVATE
#define BT_CALL_PRIVATE                 0
#endif

//通话时是否需要切换手机端与蓝牙端接听
#ifndef BT_PHONE_SWITCH
#define BT_PHONE_SWITCH		            1
#endif

//蓝牙模式下是否要默认音量最大
#ifndef BT_VOLUME_MAX
#define BT_VOLUME_MAX                   0
#endif

//是否使用短按播放键来接听和挂断电话(包括PALY 和 PLAY_S10)
#ifndef KU_PLAY_ANSWER_AND_HANGUP
#define KU_PLAY_ANSWER_AND_HANGUP       0
#endif

//是否使用双击 PLAY_S10 键来拒接电话(默认使用长按 PLAY_S10 键拒接电话)(不包括 PLAY 键)
#ifndef KU_PLAY_S10_TWICE_REJECT
#define KU_PLAY_S10_TWICE_REJECT        0
#endif

//是否使用双击 PLAY_S10 键来回拨电话(默认使用长按 PLAY_S10 键回拨电话)(不包括 PLAY 键)
#ifndef KU_PLAY_S10_TWICE_CALLBACK
#define KU_PLAY_S10_TWICE_CALLBACK      0
#endif

//是否在全部任务下支持蓝牙(此SDK版本为非蓝牙后台版本，不支持全程蓝牙，如有需要请使用蓝牙后台版本SDK)
#ifndef BT_ALL_TASK
#define BT_ALL_TASK                     0
#endif

//是否使用长按 PLAY 键断开连接(不包括 PLAY_S10 键)
#ifndef KL_PLAY_DISCONNECT
#define KL_PLAY_DISCONNECT              0
#endif

//是否使用长按 PLAY 键挂断电话(不包括 PLAY_S10 键)
#ifndef KL_PLAY_HANDUP
#define KL_PLAY_HANDUP                  0
#endif

//蓝牙播放状态下软关机后开机自动回连成功后是否需要自动播放
#ifndef BT_SOFT_POWEROFF_AUTO_PLAY
#define BT_SOFT_POWEROFF_AUTO_PLAY      0
#endif

//蓝牙播放状态下切换模式后回到蓝牙模式自动回连成功后是否需要自行播放
#ifndef BT_CHANGE_MODE_AUTO_PLAY
#define BT_CHANGE_MODE_AUTO_PLAY        0
#endif

//是否使用长按 PLAY_S10 键断开蓝牙连接(不包括 PLAY 键)
#ifndef KL_PLAY_S10_DISCONNECT
#define KL_PLAY_S10_DISCONNECT          0
#endif

//蓝牙名称是否显示3位地址(调试用，例如：BW-BTBOX-***)
#ifndef BT_NAME_WITH_ADDR
#define BT_NAME_WITH_ADDR               0
#endif

//蓝牙设备名称
#ifndef BT_NAME
#define BT_NAME                         'B',  'W', '-', 'B', 'T', 'B', 'O', 'X',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif

//蓝牙音乐播放时是否使用动态降噪
#ifndef BT_DYMANIC_SUPPRESSION
#define BT_DYMANIC_SUPPRESSION          1
#endif

//是否使用数字按键拨号功能
#ifndef NUMBER_KEY_TO_CALL
#define NUMBER_KEY_TO_CALL              0
#endif

//是否需要动态配置蓝牙名称
#ifndef BT_NAME_DYMANIC_CHANGE
#define BT_NAME_DYMANIC_CHANGE          0
#endif

//蓝牙通话时是否需要显示来去电号码和时间(只有选择 LEDSEG_THEME_DEFAULT 或 LCD_THEME 时此宏配置才有效)
#ifndef BT_DISP_NUM_TIME
#define BT_DISP_NUM_TIME                0
#endif

//蓝牙模式下是否需要频谱显示(需要LCD显示)
#ifndef BT_FREQ_SHOW
#define BT_FREQ_SHOW                    0
#endif

//是否复用SD_CMD作蓝牙复位脚
#ifndef BTRST_REUSE_SDCMD
#define BTRST_REUSE_SDCMD               0
#endif


/*****************************************************************************
 * Module    : RTC功能相关配置
 *****************************************************************************/
//是否显示年月日(未调试)
#ifndef DATE_EN
#define DATE_EN                         0
#endif

//闹钟
#ifndef ALARM_EN
#define ALARM_EN                        0
#endif

//IRRTC WAKE_UP 脚使用(未调试)
#ifndef IRTCC_WAKEUP_EN
#define IRTCC_WAKEUP_EN                 0
#endif


/*****************************************************************************
 * Module    : 系统细节配置
 *****************************************************************************/
//功放解MUTE延时cnt*5ms，某些功放解MUTE延时较长，如8002解MUTE后要延时0.5s左右才有声音输出
//可根据实际情况调整，此宏只在播放提示音时使用
#ifndef OPA_UNMUTE_DELAY
#define OPA_UNMUTE_DELAY                10  //10*5ms
#endif

//蓝牙音量或系统音量为0时是否MUTE功放（若同时打开BT_DYMANIC_SUPPRESSION，可实现蓝牙播静音文件时MUTE功放）
#ifndef VOL_CTRL_MUTE
#define VOL_CTRL_MUTE                   0   //注意：若功放反应慢或者开关MUTE时有啪声，不建议打开
#endif

//是否节省DAC左右声道隔直电容，一般推耳机时使用
#ifndef DAC_VCMBUF_EN
#define DAC_VCMBUF_EN                   0
#endif

//是否使用拨动开关开机检测MP3和BT的功能
#ifndef MP3BT_DECT_EN
#define MP3BT_DECT_EN                   0
#endif

//开机检测MP3和BT的IO是否复用其他IO(主要用于2226复用P05作MP3BT开机检测)
#ifndef MP3BT_DECT_REUSE_EN
#define MP3BT_DECT_REUSE_EN             0
#endif

//是否需要检测LINE IN线
#ifndef LINEIN_DETECT_EN
#define LINEIN_DETECT_EN                0
#endif

//linein检测脚是否需要复用led seg
#ifndef LINEIN_SEG_REUSE
#define LINEIN_SEG_REUSE                0
#endif

//是否使用耳机检测
#ifndef EAR_DETECT_EN
#define EAR_DETECT_EN                   0
#endif

//是否使用蓝灯闪烁(双LED时，蓝灯主要用于蓝牙模式)
#ifndef LED_BLUE_BLINK
#define LED_BLUE_BLINK                  0
#endif

//是否使用绿灯闪烁(双LED时，绿灯主要用于非蓝牙模式)
#ifndef LED_GREEN_BLINK
#define LED_GREEN_BLINK                 0
#endif

//是否需要模式切换时蓝灯闪烁一下
#ifndef MODE_CHANGE_BLUE_ON
#define MODE_CHANGE_BLUE_ON             1
#endif

//是否使用长按 PLAY_S10 键切换模式(不包括 PLAY 键)
#ifndef KL_PLAY_S10_CHANGE_MODE
#define KL_PLAY_S10_CHANGE_MODE         1
#endif

//是否使用 HSF键切换模式
#ifndef K_HSF_CHANGE_MODE
#define K_HSF_CHANGE_MODE               0
#endif

//是否使用软开关机
#ifndef SOFT_POWER_ON_OFF
#define SOFT_POWER_ON_OFF               0
#endif

//是否需要开机检测进入FM
#ifndef POWERON_TO_FM
#define POWERON_TO_FM                   0
#endif

//录音功能总开关。为0时关闭所有的录音功能
#ifndef RECORD_EN
#define RECORD_EN                       0
#endif

//录音文件名长度选择
#ifndef RECORD_FILE_NAME
#define RECORD_FILE_NAME                SHORT_NAME
#endif

//是否需要录音完马上播放
#ifndef REC_PLAY_EN
#define REC_PLAY_EN                     1
#endif

//SD检测延时
#ifndef SD_DETECT_DELAY
#define SD_DETECT_DELAY                 50
#endif

//P35脚的使用选择。用做MUTE脚时为0，用做控制USB供电时选1，其他用处时为2
#ifndef USE_P35_USBPOW_CTL
#define USE_P35_USBPOW_CTL              0
#endif

//IIC接口是否需要与SD复用P20 P27 或者 P31 P32
#ifndef IIC_REUSE_SD
#define IIC_REUSE_SD                    0
#endif

//是否复用SDCLK作为FM时钟源(主要用于无晶振)
#ifndef FMOSC_REUSE_SDCLK
#define FMOSC_REUSE_SDCLK               0
#endif

//复用SDCLK作为FM时钟源时FM时钟频率选择(只有宏 FMOSC_REUSE_SDCLK 打开时，此宏配置才有效)
#ifndef FMOSC_FREQ_SEL
#define FMOSC_FREQ_SEL                  FMOSC_12M
#endif

//IICCLK是否与USB复用(主要用于无晶振，SDCLK复用推12M时钟时)
#ifndef IICCLK_REUSE_USB
#define IICCLK_REUSE_USB                0
#endif

//IICDAT是否与SDDAT复用(主要用于无晶振，SDCLK复用推12M时钟时)
#ifndef IICDAT_REUSE_SDDAT
#define IICDAT_REUSE_SDDAT              0
#endif

//是否复用ADKEY检测SD卡
#ifndef SD_DECT_REUSE_ADKEY
#define SD_DECT_REUSE_ADKEY             0
#endif

//是否复用LED检测SD卡(主要用于2226 IO 口不够用时)
#ifndef SD_DECT_REUSE_LED
#define SD_DECT_REUSE_LED               0
#endif

//是否复用ADKEY检测LINE IN插入
#ifndef AUX_DECT_REUSE_ADKEY
#define AUX_DECT_REUSE_ADKEY            0
#endif

//是否复用SD_CMD作耳机检测
#ifndef SDCMD_REUSE_EARDET
#define SDCMD_REUSE_EARDET              0
#endif

//是否复用SD_CLK作ADKEY检测
#ifndef SDCLK_REUSE_ADKEY
#define SDCLK_REUSE_ADKEY               0
#endif

//是否复用IICCLK检测耳机插入
#ifndef EAR_DETECT_REUSE_IICCLK
#define EAR_DETECT_REUSE_IICCLK         0
#endif

//是否复用IICCLK检测 LINE IN 插入
#ifndef LINEIN_DETECT_REUSE_IICCLK
#define LINEIN_DETECT_REUSE_IICCLK      0
#endif

//是否复用SD_CMD作LINE IN检测
#ifndef SDCMD_REUSE_LINEIN_DET
#define SDCMD_REUSE_LINEIN_DET          0
#endif

//是否复用ADKEY推LED灯(主要用于2226 IO 口不够用时)
#ifndef LED_REUSE_ADKEY
#define LED_REUSE_ADKEY                 0
#endif

//是否使用USBDP/USBDM推LED灯(主要用于2226 IO 口不够用时)(打开后不能使用U盘和USBDEV)
#ifndef LED_USE_USBIO
#define LED_USE_USBIO                   0
#endif

//是否使用USBDP/USBDM作MUTE控制(主要用于2226 IO 口不够用时)(打开后不能使用U盘和USBDEV)
#ifndef MUTE_USE_USBIO
#define MUTE_USE_USBIO                  0
#endif

//长时间无操作(时间由宏 STANDBY_TIME 控制)是否需要自动进入休眠待机
#ifndef AUTO_STANDBY_EN
#define AUTO_STANDBY_EN                 1
#endif

//进入休眠待机时间选择(默认10分钟)
#ifndef STANDBY_TIME
#define STANDBY_TIME                    600
#endif

//进入休眠待机后是否需要唤醒功能
#ifndef STANDBY_WAKEUP_EN
#define STANDBY_WAKEUP_EN               1
#endif

//是否仅有BT和AUX功能(用于在蓝牙模式下决定是否要直接切换到AUX)
#ifndef BT_AUX_MODE_ONLY
#define BT_AUX_MODE_ONLY                0
#endif

//红外POWER键硬关机使能
#ifndef IR_HARD_POWERDOWN_EN
#define IR_HARD_POWERDOWN_EN            0
#endif

//是否需要低电提醒
#ifndef LOWPOWER_WARNING
#define LOWPOWER_WARNING                0
#endif

//低电提醒电压控制(默认0x80时约为3.3-3.4V)(只有宏 LOWPOWER_WARNING 打开时此宏才有效)
#ifndef LOWPOWER_WARNING_VOLTAGE
#define LOWPOWER_WARNING_VOLTAGE        0x80
#endif

//低电关机电压控制(默认0x7d时约为3.2-3.3V)
#ifndef LOWPOWER_POWEROFF_VOLTAGE
#define LOWPOWER_POWEROFF_VOLTAGE       0x7d
#endif

//是否使用独立IO口POWER键
#ifndef POWER_IOKEY_EN
#define POWER_IOKEY_EN                  0
#endif

//是否使用独立IO口MODE键
#ifndef MODE_IOKEY_EN
#define MODE_IOKEY_EN                   0
#endif

//是否使用独立IO口音量加减键
#ifndef VOL_IOKEY_EN
#define VOL_IOKEY_EN                    0
#endif

//是否使用EQ显示Eq0-Eqn(默认显示EQ模式名称的前三个字母)
#ifndef EQ_DISPLAY_NUM
#define EQ_DISPLAY_NUM                  0
#endif

//低电时是否需要显示一下OFF并且在3秒后关机
#ifndef LOWER_POWER_DISPLAY_OFF
#define LOWER_POWER_DISPLAY_OFF         0
#endif

//关机时是否需要关闭VPG33
#ifndef POWER_OFF_VPG33
#define POWER_OFF_VPG33                 0
#endif


/*****************************************************************************
 * Module    :AUX通道配置
 *****************************************************************************/
//选择MIC的通道
#ifndef MIC_CHANNEL_SEL
#define MIC_CHANNEL_SEL                 MCHANNEL_3
#endif

//选择FM的通道
#ifndef FM_CHANNEL_SEL
#define FM_CHANNEL_SEL                  CHANNEL_72
#endif

//选择LINEIN的通道
#ifndef AUX_CHANNEL_SEL
#define AUX_CHANNEL_SEL                 CHANNEL_01
#endif


/*****************************************************************************
 * Module    : 按键及外设IO配置
 *****************************************************************************/
//ADKey的使用，0为使用IOKey，1为1组ADKey，2为2组ADKey
#ifndef USE_ADKEY
#define USE_ADKEY                       0
#endif

//选择第1组ADKEY的IO
#ifndef ADKEY_PORT
#define ADKEY_PORT                      ADKEY_P06
#endif

//第1组ADKEY是否使用内部10K上拉(只有3个按键时可以使用)
#ifndef ADKEY_INT_PU10K
#define ADKEY_INT_PU10K                 0
#endif

//选择第2组ADKEY的IO
#ifndef ADKEY2_PORT
#define ADKEY2_PORT                     ADKEY_P33
#endif

//第2组ADKEY是否使用内部10K上拉(只有3个按键时可以使用)
#ifndef ADKEY2_INT_PU10K
#define ADKEY2_INT_PU10K                0
#endif

//红外遥控(NEC硬解)
#ifndef HARD_IR_EN
#define HARD_IR_EN                      0
#endif

//红外遥控(飞利浦红外软解，注意：此宏只针对飞利浦的一款红外遥控，若不是使用此款遥控请不要打开此宏)
#ifndef SOFT_IR_EN
#define SOFT_IR_EN                      0
#endif

//是否使用另外两个IO做单独的MODE和PLAY键
#ifndef USE_MODE_PLAY_KEY
#define USE_MODE_PLAY_KEY               0
#endif

//sd_port值:0: P20 P21 P27,    1: P20 P21 P27, clk复用检测,    2: P30 P31 P32,    3: P30 P31 P32, clk复用检测
//选择0或2时还需要定义SD检测引脚，详见SD_DETECT_INIT和IS_SD_IN
#ifndef SD_PORT_SEL
#define SD_PORT_SEL                     0
#endif

//是否使用P01作串口打印调试
#ifndef DEBUG_UART_SEL
#define DEBUG_UART_SEL                  0
#endif

//是否为接听键，用于中止铃声
#ifndef IS_HSF_KEY
#define IS_HSF_KEY(k)                   0
#endif


/*****************************************************************************
 * Module    :IO按键定义
 *****************************************************************************/
#ifndef K_PLAY_INIT
#define K_PLAY_INIT()
#endif
#ifndef IS_K_PLAY
#define IS_K_PLAY()                     (0)
#endif
#ifndef K_NEXT_INIT
#define K_NEXT_INIT()
#endif
#ifndef IS_K_NEXT
#define IS_K_NEXT()                     (0)
#endif
#ifndef K_PREV_INIT
#define K_PREV_INIT()
#endif
#ifndef IS_K_PREV
#define IS_K_PREV()                     (0)
#endif


/*****************************************************************************
 * Module    :IIC IO配置
 *****************************************************************************/
#ifndef IIC_IO_SET
    #define IIC_IO_SET
    #define IIC_DATA                    1       //P31
    #define IIC_CLK                     0       //P30
    #define IIC_DATA_CLK_OUT()          P3PU0 &= ~(BIT(0) | BIT(2));  P3DIR &= ~(BIT(IIC_DATA) | BIT(IIC_CLK)); delay(10)
    #define IIC_DATA_IN()               P3DIR |= BIT(IIC_DATA); P3PU0 |= BIT(2)
    #define IIC_DATA_OUT()              P3PU0 &= ~BIT(2); P3DIR &= ~BIT(IIC_DATA)
    #define IIC_DATA_CLK_HIGH()         P3 |= BIT(IIC_DATA) | BIT(IIC_CLK)
    #define IIC_DATA_HIGH()             P3 |= BIT(IIC_DATA)
    #define IIC_DATA_LOW()              P3 &= ~BIT(IIC_DATA)
    #define IIC_CLK_HIGH()              P3 |= BIT(IIC_CLK)
    #define IIC_CLK_LOW()               P3 &= ~BIT(IIC_CLK)
    #define IIC_DATA_IS_HIGH()          (P3 & BIT(IIC_DATA))
#endif


/*****************************************************************************
 * Module    :LED IO配置
 *****************************************************************************/
#ifndef LED_GREEN_INIT
#define LED_GREEN_INIT()
#endif
#ifndef LED_GREEN_ON
#define LED_GREEN_ON()
#endif
#ifndef LED_GREEN_OFF
#define LED_GREEN_OFF()
#endif
#ifndef LED_GREEN_TOG
#define LED_GREEN_TOG()
#endif
#ifndef LED_GREEN_IS_ON
#define LED_GREEN_IS_ON()       0
#endif

#ifndef LED_BLUE_INIT
#define LED_BLUE_INIT()
#endif
#ifndef LED_BLUE_ON
#define LED_BLUE_ON()
#endif
#ifndef LED_BLUE_OFF
#define LED_BLUE_OFF()
#endif
#ifndef LED_BLUE_TOG
#define LED_BLUE_TOG()
#endif
#ifndef LED_BLUE_IS_ON
#define LED_BLUE_IS_ON()       0
#endif


/*****************************************************************************
 * Module    : 蓝牙相关IO配置
 *****************************************************************************/
//蓝牙模块WAKEUP IO
#ifndef BT_SLEEP_IO_SET     //默认使用P34，若省wakeup脚，要定义BT_IS_SLEEP()为0
    #define BT_SLEEP_IO_SET
    #define BT_IS_SLEEP()               (P3 & BIT(4))
#endif

//蓝牙模块WAKEUP IO没有复用其他功能时，定义成空
#ifndef BT_IS_SLEEP_INIT
#define BT_IS_SLEEP_INIT()
#endif
#ifndef BT_IS_SLEEP_END
#define BT_IS_SLEEP_END()
#endif

//蓝牙模块复位 IO
#ifndef BT_RESET_IO_SET
  #if BTRST_REUSE_SDCMD
    #define BT_RESET_IO_SET
    #define BT_RST_INIT()               SD_CMD_DIR_OUT();
    #define BT_RST_LOW()                SD_CMD_CLR();
    #define BT_RST_HIGH()               SD_CMD_SET();
  #else
    #define BT_RESET_IO_SET
    #define BT_RST_INIT()               P1DIR &= ~BIT(4);
    #define BT_RST_LOW()                P1 &= ~BIT(4);
    #define BT_RST_HIGH()               P1 |= BIT(4);
  #endif
#endif

//蓝牙电源供电IO设置
#ifndef BTLDO_POWIO_SET
    #define BTLDO_POWCTRL_SET
    #define BTLDO_DIR_INIT()
    #define BTLDO_EN()
    #define BTLDO_DIS()
#endif


/*****************************************************************************
 * Module    : LCD显示相关配置
 *****************************************************************************/
//是否需要发送地址修正
#ifndef LCD_X_FIX
#define LCD_X_FIX                       0
#endif

//是否需要软件复位LCD屏
#ifndef LCD_SOFT_RESET
#define LCD_SOFT_RESET                  1
#endif

//是否需要设置LCD屏invert寄存器
#ifndef LCD_INVERT_DISPLAY
#define LCD_INVERT_DISPLAY              0
#endif

//默认的LCD屏对比度(取值范围: 0~15)
#ifndef LCD_CONTRAST_DEFAULT
#define LCD_CONTRAST_DEFAULT            10
#endif

//LCD IO 配置
#ifndef LCD_IO_SET
#define LCD_IO_SET
#define LCD_DATA                        5   //P25
#define LCD_CLK                         6   //P26
#define LCD_A0                          0   //P30
#define LCD_RES                         1   //p31
#define LCD_CS                          2   //P32
#define LCD_BACKLIGHT                   5   //p15
#endif

//LCD端口设置
#ifndef LCD_PORT_SET
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
#endif

//LCD背光IO相关配置
#ifndef LCD_BACKLIGHT_SET
#define LCD_BACKLIGHT_SET
#define LCD_BACKLIGHT_ON()              if (!sys_ctl.backlight_sta) {sys_ctl.backlight_sta = 1; P1DRV1 |= BIT(7); P1DIR |= BIT(LCD_BACKLIGHT);}
#define LCD_BACKLIGHT_OFF()             if (sys_ctl.backlight_sta)  {sys_ctl.backlight_sta = 0; P1DRV1 &= ~BIT(7);}
#endif


/*****************************************************************************
 * Module    : 控件界面的停留时间控制
 *****************************************************************************/
//菜单列表listbox的停留时间
#ifndef LIST_TIME
#define LIST_TIME                       80  //8秒
#endif

//进度条progressbox的停留时间
#ifndef PROGRESS_TIME
#define PROGRESS_TIME                   30  //3秒
#endif

//消息框停留时间
#ifndef MSGBOX_TIME
#define MSGBOX_TIME                     5   //1.5秒
#endif

//在只有提示的情况下，是否使用简单的消息框
#ifndef MSGBOX_SIMPLE
#define MSGBOX_SIMPLE                   1
#endif


/*****************************************************************************
 * Module    : 其它杂项IO相关配置
 *****************************************************************************/
//电源插入检测
#ifndef DCIN_INIT
#define DCIN_INIT()
#endif
#ifndef IS_DCIN
#define IS_DCIN()                       (0)
#endif

//上电时BT/MP3检测
#ifndef MP3BT_DECT_INIT
#define MP3BT_DECT_INIT()
#endif
#ifndef MP3BT_DECT_IS_BT
#define MP3BT_DECT_IS_BT()              0
#endif

//复用时要自定义
#ifndef MP3BT_DECT_END
#define MP3BT_DECT_END()
#endif

//耳机检测
#ifndef EAR_DETECT_INIT
#define EAR_DETECT_INIT()
#endif

//复用SDCMD检测时不需要定义IS_EAR_IN
#if !SDCMD_REUSE_EARDET
#ifndef IS_EAR_IN
#define IS_EAR_IN()                     0
#endif
#endif

//不复用IICCLK检测耳机时定义成空
#if !EAR_DETECT_REUSE_IICCLK
#ifndef EAR_DETECT_END
#define EAR_DETECT_END()
#endif
#endif

//LINE IN检测，复用ADKEY检测LINE IN 插入时不需要定义LINEIN_DETECT_INIT
#if !SDCMD_REUSE_LINEIN_DET && !AUX_DECT_REUSE_ADKEY
#ifndef LINEIN_DETECT_INIT
#define LINEIN_DETECT_INIT()
#endif
#endif

//复用SDCMD或者ADKEY检测时不需要定义IS_LINEIN_IN
#if !SDCMD_REUSE_LINEIN_DET && !AUX_DECT_REUSE_ADKEY
#ifndef IS_LINEIN_IN
#define IS_LINEIN_IN()                  0
#endif
#endif

#if !LINEIN_DETECT_REUSE_IICCLK
#ifndef LINEIN_DETECT_END
#define LINEIN_DETECT_END()
#endif
#endif

//SD检测脚配置(复用 ADKEY 或 SD_CLK 检测时不需要定义SD_DETECT_INIT)
#if !SD_DECT_REUSE_ADKEY
#ifndef SD_DETECT_INIT
#define SD_DETECT_INIT()
#endif
#endif

//复用clk检测时不需要定义IS_SD_IN
#if (SD_PORT_SEL != 1) && (SD_PORT_SEL != 3) && !SD_DECT_REUSE_ADKEY
#ifndef IS_SD_IN
#define IS_SD_IN()                      0
#endif
#endif

//SD检测不复用ADKEY时定义成空
#ifndef SD_DETECT_END
#define SD_DETECT_END()
#endif

//MUTE功放控制
#ifndef MUSIC_MUTE
#define MUSIC_MUTE()
#endif
#ifndef MUSIC_UNMUTE
#define MUSIC_UNMUTE()
#endif

//AB/D类功放控制
#ifndef OPA_CTL_INIT
#define OPA_CTL_INIT()
#endif
#ifndef OPA_CTL_HIGH
#define OPA_CTL_HIGH()
#endif
#ifndef OPA_CTL_LOW
#define OPA_CTL_LOW()
#endif

//软开关机IO控制配置
#ifndef SOFT_POWER_CTL_INIT
#define SOFT_POWER_CTL_INIT()
#endif
#ifndef SOFT_POWER_CTL_ON
#define SOFT_POWER_CTL_ON()
#endif
#ifndef SOFT_POWER_CTL_OFF
#define SOFT_POWER_CTL_OFF()
#endif

//软开关机IO检测配置
#ifndef SOFT_POWER_CHK_INIT
#define SOFT_POWER_CHK_INIT()
#endif
#ifndef SOFT_POWER_CHK_IS_ON
#define SOFT_POWER_CHK_IS_ON()          1
#endif

//USB电源控制
#ifndef USBPOW_INIT
#define USBPOW_INIT()
#endif
#ifndef USBPOW_EN
#define USBPOW_EN()
#endif
#ifndef USBPOW_DIS
#define USBPOW_DIS()
#endif


/*****************************************************************************
 * Module    : 休眠及唤醒IO相关配置
 *****************************************************************************/
#ifndef IO_SLEEP
#define IO_SLEEP()
#endif

#ifndef IO_WAKEUP
#define IO_WAKEUP()
#endif

#ifndef IS_KEY_WAKEUP
#define IS_KEY_WAKEUP()                 0
#endif


/*
 * 以下是自动选择的宏
 */
/*****************************************************************************
 * Module    : 系统相关配置
 *****************************************************************************/
#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E || SYS_PKG == PKG_QFN48_6680M)
    #if (SYS_CRY != CRY_RC)
    #error "SSOP24只能选择无晶振方案"
    #endif
#endif


/*****************************************************************************
 * Module    : MUSIC相关配置
 *****************************************************************************/
//关闭所有格式歌曲的ID3显示
#if !MUSIC_ID3_EN
#undef MP3_ID3_EN
#undef WMA_ID3_EN
#undef WAV_ID3_EN
#define MP3_ID3_EN                      0
#define WMA_ID3_EN                      0
#define WAV_ID3_EN                      0
#endif


/*****************************************************************************
 * Module    : 红外及其唤醒相关配置
 *****************************************************************************/
#if !AUTO_STANDBY_EN && !IR_HARD_POWERDOWN_EN
    #undef STANDBY_WAKEUP_EN
    #define STANDBY_WAKEUP_EN           0   //不需要自动关机或者红外关机时，不需要唤醒
#endif


#if !HARD_IR_EN && !SOFT_IR_EN
#undef  IR_HARD_POWERDOWN_EN
#define IR_HARD_POWERDOWN_EN            0
#endif

#if IR_HARD_POWERDOWN_EN
#ifndef IR_PWR_DAT
#define IR_PWR_DAT                      0x45   //低功耗，红外遥控唤醒按键值
#endif
#endif


/*****************************************************************************
 * Module    : 录音相关配置
 *****************************************************************************/
#if !RECORD_EN
//关闭所有的录音宏定义
#undef FM_RECORD_EN
#undef TASK_MIC_EN
#undef AUX_RECORD_EN
#undef TASK_VOICE_EN
#undef REC_PLAY_EN
#define FM_RECORD_EN                    0
#define TASK_MIC_EN                     0
#define AUX_RECORD_EN                   0
#define TASK_VOICE_EN                   0  //由于不需要录音功能，同时关闭录音播放任务
#define REC_PLAY_EN                     0
#endif

#if !FM_VOUT_THROUGH_MCU
#undef FM_RECORD_EN
#define FM_RECORD_EN                    0   //FM不经过主控时，不支持FM录音
#endif


/*****************************************************************************
 * Module    : AUX相关配置
 *****************************************************************************/
#if !TASK_AUX_EN
#undef LINEIN_DETECT_EN
#undef LINEIN_DETECT_INIT
#undef IS_LINEIN_IN
#define LINEIN_DETECT_EN                0
#define LINEIN_DETECT_INIT()
#define IS_LINEIN_IN()                  0
#endif


/*****************************************************************************
 * Module    : RTC相关配置
 *****************************************************************************/
#if !TASK_RTC_EN
#undef ALARM_EN
#undef DATE_EN
#define ALARM_EN                        0
#define ALARM_EN                        0
#endif


/*****************************************************************************
 * Module    : USB_GPIO相关配置
 *****************************************************************************/
#if (LED_USE_USBIO || MUTE_USE_USBIO)
    #define USB_GPIO_EN                 1
#else
    #define USB_GPIO_EN                 0
#endif


/*****************************************************************************
 * Module    : ICC复用相关配置
 *****************************************************************************/
#if IIC_REUSE_SD || IICCLK_REUSE_USB || IICDAT_REUSE_SDDAT || EAR_DETECT_REUSE_IICCLK || LINEIN_DETECT_REUSE_IICCLK
    #define ICC_REUSE_EN                1
#else
    #define ICC_REUSE_EN                0
#endif


#endif
