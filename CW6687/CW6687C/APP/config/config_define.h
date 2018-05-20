/*****************************************************************************
 * Module    : Config
 * File      : config_define.h
 * Author    : Hanny
 * Function  : �����ļ������ö���ѡ���б����������޸�
 *****************************************************************************/
#ifndef CONFIG_DEFINE_H
#define CONFIG_DEFINE_H

#define CFG_SDK_VERSION                 0x00010200UL    //SDK �汾�ţ�v0.1.2

/*****************************************************************************
 * Module    : ϵͳ������ѡ���б�
 *****************************************************************************/
//ϵͳʱ��Դѡ��
#define CRY_32K                         1               //�ⲿ32K����
#define CRY_12M                         2               //�ⲿ12M����
#define CRY_2M                          3               //��������26MHz(�޾��񷽰�)

//ADKEY PORT��ѡ��
#define ADKEY_P33                       0
#define ADKEY_P21                       1
#define ADKEY_P14                       2
#define ADKEY_P22                       3
#define ADKEY_P30                       4
#define ADKEY_P13                       5

//DAC ��̬����ģʽѡ��
#define DYNAMIC_NO                      0               //����Ҫ��̬����
#define DYNAMIC_DAC_ONLY                1               //ֻ��DAC���ж�̬���룬��̬����ʱ��MUTE����
#define DYNAMIC_DAC_SPK                 2               //��̬����ʱ��ͬʱMUTE����

//FMģ��ѡ��
#define FM_RDA5807                      1
#define FM_BK1080                       2
#define FM_QN8035                       3
#define FM_QN8065                       4
#define FM_AX5111                       5               //ʹ��AX5111 FMģ��ʱ��Ŀǰֻ֧��ʹ��12M���񹩸�AX5111
#define FM_RTC6207                      6
#define FM_RTC6218                      7

//FM_CLK_SEL,FMʱ��ѡ��
#define P33_OUT_NULL                    1               //������ʱ,����ҪP33���ʱ�Ӹ�FM OSC.
#define P33_OUT_32K                     2               //P33 ���32K������FM OSC.
#define P33_OUT_12M                     3               //P33 ���12M������FM OSC.
#define FMOSC_REUSE_12MSDCLK            4               //ʹ��12M��SDCLK��ΪFMʱ��Դ
#define FMOSC_REUSE_32KSDCLK            5               //ʹ��32K��SDCLK��ΪFMʱ��Դ
#define SHARE_CRYSTAL_32K               P33_OUT_NULL    //������ʱ,����ҪP33���ʱ�Ӹ�FM OSC.

//¼����ʽѡ��
#define REC_WAV_PCM                     0
#define REC_WAV_ADPCM                   1
#define REC_MP3                         2

/*****************************************************************************
 * Module    : ��ʾ�������ѡ���б�
 *****************************************************************************/
//��ʾ������ѡ��
#define DISPLAY_NO                      0                                       //����ʾģ��
#define DISPLAY_LCD                     0x100                                   //ѡ��LCD��������Ϊ��ʾ����
#define DISPLAY_LCDSEG                  0x200                                   //ѡ�ö�������Ϊ��ʾ����
#define DISPLAY_LEDSEG                  0x300                                   //ѡ���������Ϊ��ʾ����

//������ʾ��������ѡ��ֱ��ѡ�����⣬�Զ���Ӧ��Ӧ��ʾ����
#define THEME_NO                        DISPLAY_NO                              //�����⣬����ʾ
#define THEME_LCD_DEFAULT               (DISPLAY_LCD | 0x00)                    //LCD������Ĭ������
#define THEME_LCDSEG_DEFAULT            (DISPLAY_LCDSEG | 0x00)                 //������Ĭ������
#define THEME_LEDSEG_5C7S               (DISPLAY_LEDSEG | 0x00)                 //5C7S �����
#define THEME_LEDSEG_7PIN               (DISPLAY_LEDSEG | 0x81)                 //7PIN �����(FMģʽ����ʾFM)
#define THEME_LEDSEG_7PIN_MHZ           (DISPLAY_LEDSEG | 0x82)                 //7PIN �����(FMģʽ����ʾMHz)

//����ѡ��
#define SMALL_FONT                      1              //10������
#define BIG_FONT                        2              //12������

//¼���ļ���ѡ��
#define SHORT_NAME                      1
#define LONG_NAME                       2

/*****************************************************************************
 * Module    : ���º궨�����������޸ģ���ϸ�û��������޸�user_config.h
 *****************************************************************************/
//�����û����ø���
#define CONCAT(x, y)        x ## y
#define SET_STR1(x)         #x
#define SET_STR(x)          SET_STR1(x)
#define SET_USER_PATH(x, y) SET_STR(CONCAT(x, y))

//�����û�����·��
#include SET_USER_PATH(CFG_DIR, /config_setting.h)
#include "config_extend.h"

#endif
