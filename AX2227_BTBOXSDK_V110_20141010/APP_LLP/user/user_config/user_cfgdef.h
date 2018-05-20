/*****************************************************************************
 * Module    : User
 * File      : user_cfgdef.h
 * Author    : jingfa
 * Email     :
 * Function  : 用户参数列表
 *****************************************************************************/
#ifndef USER_CFGDEF_H
#define USER_CFGDEF_H


//主控封装选项
#define PKG_LQFP48                      1       //LQFP48封装    2227
#define PKG_SSOP24                      2       //SSOP24封装    2226、1006
#define PKG_LQFP48_6680E                3       //LQFP48封装    6680E、6681E
#define PKG_QFN48_6680M                 4       //QFN48封装     6680M、6681M、6682M、6682E


//系统时钟源选项
#define CRY_32K                         1       //32K晶振
#define CRY_12M                         2       //12M晶振
#define CRY_RC                          3       //内部RC(无晶振方案)


//FM时钟频率选项
#define FMOSC_32K                       1       //32K
#define FMOSC_12M                       2       //12M


//ADKEY PORT口选项
#define ADKEY_P33   0
#define ADKEY_P06   1
#define ADKEY_P04   2
#define ADKEY_P05   3
#define ADKEY_P30   4
#define ADKEY_P13   5


//FM模块选项
#define FM_RDA5807                      1
#define FM_BK1080                       2
#define FM_QN8035                       3
#define FM_QN8065                       4
#define FM_AX5111                       5       //使用AX5111 FM模块时，目前只支持使用12M晶振供给AX5111
#define FM_RTC6218                      6
#define FM_RTC6207                      7


//蓝牙模块选项
#define BT6635                          0       //蓝牙6635(未调试)
#define BT6638                          1       //蓝牙6638(未调试)
#define BT6639                          2       //蓝牙6639


//ledseg屏类型选项
#define LEDSEG_5C7S_MHZ                 0       //5c7s屏，FM模式下显示MHZ
#define LEDSEG_7P7S_FM                  1       //7脚屏，FM模式下显示FM
#define LEDSEG_7P7S_MHZ                 2       //7脚屏，FM模式下显示MHZ


//无显示
#define NO_THEME                        0       //无显示方案

//选用LCD屏的不同模板
#define LCD_THEME_DEFAULT               0x01    //LCD默认主题
#define LCD_THEME_ACT                   0x02    //LCD ACT主题(此主题目前只调试了音乐模式，其他模式未调试)

//选用LCD断码屏的不同模板
#define LCDSEG_THEME_DEFAULT            0x80    //断码屏默认主题(未调试)

//选用LED屏的不同模板
#define LEDSEG_THEME_DEFAULT            0xC0    //LED屏默认主题


//录音文件名长度选择
#define SHORT_NAME                      1       //短文件名
#define LONG_NAME                       2       //长文件名（未调试）


#endif
