/*****************************************************************************
 * Module    : Config
 * File      : config_define.h
 * Author    : Hanny
 * Function  : 配置文件的配置定义选择列表，请勿随意修改
 *****************************************************************************/
#ifndef CONFIG_DEFINE_H
#define CONFIG_DEFINE_H

#define CFG_SDK_VERSION                 0x00010200UL    //SDK 版本号：v0.1.2

/*****************************************************************************
 * Module    : 系统宏配置选择列表
 *****************************************************************************/
//系统时钟源选项
#define CRY_32K                         1               //外部32K晶振
#define CRY_12M                         2               //外部12M晶振
#define CRY_2M                          3               //共用蓝牙26MHz(无晶振方案)

//ADKEY PORT口选项
#define ADKEY_P33                       0
#define ADKEY_P21                       1
#define ADKEY_P14                       2
#define ADKEY_P22                       3
#define ADKEY_P30                       4
#define ADKEY_P13                       5

//DAC 动态降噪模式选择
#define DYNAMIC_NO                      0               //不需要动态降噪
#define DYNAMIC_DAC_ONLY                1               //只对DAC进行动态降噪，动态降噪时不MUTE功放
#define DYNAMIC_DAC_SPK                 2               //动态降噪时，同时MUTE功放

//FM模块选项
#define FM_RDA5807                      1
#define FM_BK1080                       2
#define FM_QN8035                       3
#define FM_QN8065                       4
#define FM_AX5111                       5               //使用AX5111 FM模块时，目前只支持使用12M晶振供给AX5111
#define FM_RTC6207                      6
#define FM_RTC6218                      7

//FM_CLK_SEL,FM时钟选项
#define P33_OUT_NULL                    1               //共晶振时,不需要P33输出时钟给FM OSC.
#define P33_OUT_32K                     2               //P33 输出32K方波给FM OSC.
#define P33_OUT_12M                     3               //P33 输出12M方波给FM OSC.
#define FMOSC_REUSE_12MSDCLK            4               //使用12M的SDCLK作为FM时钟源
#define FMOSC_REUSE_32KSDCLK            5               //使用32K的SDCLK作为FM时钟源
#define SHARE_CRYSTAL_32K               P33_OUT_NULL    //共晶振时,不需要P33输出时钟给FM OSC.

//录音格式选项
#define REC_WAV_PCM                     0
#define REC_WAV_ADPCM                   1
#define REC_MP3                         2

/*****************************************************************************
 * Module    : 显示相关配置选择列表
 *****************************************************************************/
//显示驱动屏选择
#define DISPLAY_NO                      0                                       //无显示模块
#define DISPLAY_LCD                     0x100                                   //选用LCD点阵屏做为显示驱动
#define DISPLAY_LCDSEG                  0x200                                   //选用断码屏做为显示驱动
#define DISPLAY_LEDSEG                  0x300                                   //选用数码管做为赤示驱动

//整合显示屏与主题选择。直接选择主题，自动对应相应显示驱动
#define THEME_NO                        DISPLAY_NO                              //无主题，无显示
#define THEME_LCD_DEFAULT               (DISPLAY_LCD | 0x00)                    //LCD点阵屏默认主题
#define THEME_LCDSEG_DEFAULT            (DISPLAY_LCDSEG | 0x00)                 //断码屏默认主题
#define THEME_LEDSEG_5C7S               (DISPLAY_LEDSEG | 0x00)                 //5C7S 数码管
#define THEME_LEDSEG_7PIN               (DISPLAY_LEDSEG | 0x81)                 //7PIN 数码管(FM模式下显示FM)
#define THEME_LEDSEG_7PIN_MHZ           (DISPLAY_LEDSEG | 0x82)                 //7PIN 数码管(FM模式下显示MHz)

//字体选项
#define SMALL_FONT                      1              //10号字体
#define BIG_FONT                        2              //12号字体

//录音文件名选择
#define SHORT_NAME                      1
#define LONG_NAME                       2

/*****************************************************************************
 * Module    : 以下宏定义请勿随意修改，详细用户配置请修改user_config.h
 *****************************************************************************/
//生成用户配置跟径
#define CONCAT(x, y)        x ## y
#define SET_STR1(x)         #x
#define SET_STR(x)          SET_STR1(x)
#define SET_USER_PATH(x, y) SET_STR(CONCAT(x, y))

//包含用户配置路径
#include SET_USER_PATH(CFG_DIR, /config_setting.h)
#include "config_extend.h"

#endif
