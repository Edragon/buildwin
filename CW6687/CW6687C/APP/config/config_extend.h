/*****************************************************************************
 * Module    : Config
 * File      : config_extend.h
 * Author    : Hanny
 * Function  : �����ļ�����չ���ã������ڰ汾�������Ƶȣ����������޸�
 *****************************************************************************/
#ifndef CONFIG_EXTEND_H
#define CONFIG_EXTEND_H

/*****************************************************************************
 * Module    : �汾��������
 *****************************************************************************/
#if (CFG_SDK_VERSION == 0x00010100UL)

#ifndef BT_PHONE_BOOK_EN
#define BT_PHONE_BOOK_EN                        0       //�Ƿ�֧�ֵ绰������
#endif

#endif

/*****************************************************************************
 * Module    : ϵͳ�궨����չ
 *****************************************************************************/

#if (SYS_CRY == CRY_2M)
#   undef TASK_RTC_EN
#   define TASK_RTC_EN                          0       //ʹ�ú�������������֧��RTCʱ�ӹ���
#endif

#if (SYS_CRY != CRY_32K)

#if (FM_CLK_SEL == P33_OUT_32K)
#error "��32K���񷽰���P33��֧�����32Kʱ��"
#endif

#if (FM_CLK_SEL == SHARE_CRYSTAL_32K)
#error "��32K���񷽰�����֧��FM������"
#endif

#endif

#if !TASK_BT_EN
#undef  BT_ALL_TASK
#define BT_ALL_TASK                             0       //δ����������ʱ������ȫ��ͬ����Ч
#endif

#if (!TASK_BT_EN) || (!BT_A2DP_EN)
#undef BT_ID3_EN
#define BT_ID3_EN                               0       //δ������ý����Ƶʱ��ID3������Ч
#undef BT_PLAY_TIME_EN
#define BT_PLAY_TIME_EN                         0       //δ������ý����Ƶʱ����ʾʱ�书����Ч
#endif

#if BT_ALL_TASK && TASK_BTHID_EN
#error "������̨���ܴ򿪶���HID������ģʽ"
#endif

#if TASK_BTHID_EN

#if BT_HID_MANU
#error "����HID������ģʽ���ܴ��ֶ�����HID����"
#endif

#if !BT_HID_EN
#undef BT_HID_EN
#define BT_HID_EN                               1
#endif

#endif

#if !BT_ALL_TASK || !BT_SPP_EN
#undef  BT_SPP_MSC_EN
#define BT_SPP_MSC_EN							0
#undef  BT_SPP_EQ_EN
#define BT_SPP_EQ_EN                            0
#endif

#if BT_ALL_TASK
#undef  SPI_4K_SECTOR_SUPPORT
#define SPI_4K_SECTOR_SUPPORT                   0
#undef  TASK_TOMCAT_EN
#define TASK_TOMCAT_EN                          0
#endif

#if !FM_VOUT_THROUGH_MCU && FM_RECORD_EN
#error "FM���������ز��ܴ�FM¼������"
#endif

#if TASK_MIC_EN || TASK_VOICE_EN || TASK_UPDATE_FW_EN || (TASK_AUX_EN && AUX_RECORD_EN) || (TASK_FM_EN && FM_RECORD_EN)
#undef  TASK_MUSIC_EN
#define TASK_MUSIC_EN							1
#endif

#if !MUSIC_ID3_EN
#undef  MP3_ID3_EN
#define MP3_ID3_EN                              0
#undef  WAV_ID3_EN
#define WAV_ID3_EN                              0
#undef  WMA_ID3_EN
#define WMA_ID3_EN                              0
#endif

#if VCMBUF_EN
#undef  DEBUG_LOADER_DIS
#define DEBUG_LOADER_DIS                        1
#undef  DEBUG_UART_SEL
#define DEBUG_UART_SEL                          0
#endif

/*****************************************************************************
 * Module    : ��ʾ���궨����չ
 *****************************************************************************/

#if !WARNING_MUSIC
//�ر�������ʾ��
#   undef  WARNING_POWER_ON
#   define WARNING_POWER_ON                     0
#   undef  WARNING_TASK_BT
#   define WARNING_TASK_BT                      0
#   undef  WARNING_TASK_CLOCK
#   define WARNING_TASK_CLOCK                   0
#   undef  WARNING_TASK_AUX
#   define WARNING_TASK_AUX                     0
#   undef  WARNING_TASK_FM
#   define WARNING_TASK_FM                      0
#   undef  WARNING_TASK_VOICE
#   define WARNING_TASK_VOICE                   0
#   undef  WARNING_TASK_SPEAKER
#   define WARNING_TASK_SPEAKER                 0
#   undef  WARNING_TASK_MIC
#   define WARNING_TASK_MIC                     0
#   undef  WARNING_TASK_USBDEV
#   define WARNING_TASK_USBDEV                  0
#   undef  WARNING_TASK_TOMCAT
#   define WARNING_TASK_TOMCAT                  0
#   undef  WARNING_BT_CONNECT
#   define WARNING_BT_CONNECT                   0
#   undef  WARNING_BT_DISCONNECT
#   define WARNING_BT_DISCONNECT                0
#   undef  WARNING_LOWPOWER
#   define WARNING_LOWPOWER                     0
#   undef  WARNING_POWER_OFF
#   define WARNING_POWER_OFF                    0
#   undef  WARNING_VOL_MAXMIN
#   define WARNING_VOL_MAXMIN                   0
#   undef  WARNING_USB_SD
#   define WARNING_USB_SD                       0
#endif

#if !WARNING_VOL_MAXMIN
#   undef  WARNING_VOL_MAXMIN_ONCE
#   define WARNING_VOL_MAXMIN_ONCE              0
#endif

/*****************************************************************************
 * Module    : �͹��ĺ궨����չ
 *****************************************************************************/

#if !TASK_HOLD_EN
#   undef  MUSIC_AUTO_STANDBY_EN
#   define MUSIC_AUTO_STANDBY_EN                0
#   undef  FM_AUTO_STANDBY_EN
#   define FM_AUTO_STANDBY_EN                   0
#   undef  BT_AUTO_STANDBY_EN
#   define BT_AUTO_STANDBY_EN                   0
#   undef  AUX_AUTO_STANDBY_EN
#   define AUX_AUTO_STANDBY_EN                  0
#endif

/*****************************************************************************
 * Module    : ��ʾ�궨����չ
 *****************************************************************************/

#define DISPLAY_SEL         (THEME_SELECT & 0xFF00)     //��ʾ����ѡ��ֱ�Ӵ�����ѡ������ȡ��ʾ������Ϣ
#if (DISPLAY_SEL == DISPLAY_LCD)
#   define IS_LCD_DISPLAY                       1
#else
#   define IS_LCD_DISPLAY                       0
#endif

#if (DISPLAY_SEL == DISPLAY_LCDSEG)
#   define IS_LCDSEG_DISPLAY                    1
#else
#   define IS_LCDSEG_DISPLAY                    0
#endif

#if (DISPLAY_SEL == DISPLAY_LEDSEG)
#   define IS_LEDSEG_DISPLAY                    1
#   if (THEME_SELECT & 0x80)
#       define IS_LEDSEG_7PIN_DISPLAY           1
#       define IS_LEDSEG_5C7S_DISPLAY           0
#   else
#       define IS_LEDSEG_7PIN_DISPLAY           0
#       define IS_LEDSEG_5C7S_DISPLAY           1
#   endif
#else
#   define IS_LEDSEG_DISPLAY                    0
#   define IS_LEDSEG_5C7S_DISPLAY               0
#   define IS_LEDSEG_7PIN_DISPLAY               0
#endif

#if !IS_LCD_DISPLAY
//��LCD���������⣬��֧�ֵĹ���
#   undef  MUSIC_LRC_EN
#   undef  FILE_NAV_EN
#   define MUSIC_LRC_EN                         0   //��֧�ָ����ʾ
#   define FILE_NAV_EN                          0   //��֧���ļ�����
#endif

/*****************************************************************************
 * Module    : �����ڸ�user ���г�ʼ��
*****************************************************************************/

#ifndef USER_OTHER_INIT
#define USER_OTHER_INIT()
#endif


/*****************************************************************************
 * Module    : ��ԴĿ¼������չ
 *****************************************************************************/
#ifndef CFG_DEFAULT_DIR
#define CFG_DEFAULT_DIR                 CFG_DIR
#endif

#ifndef CFG_IO_KEY_DIR
#define CFG_IO_KEY_DIR                  CFG_DEFAULT_DIR
#endif

#ifndef CFG_IO_IR_DIR
#define CFG_IO_IR_DIR                   CFG_DEFAULT_DIR
#endif

#ifndef CFG_IO_LED_DIR
#define CFG_IO_LED_DIR                  CFG_DEFAULT_DIR
#endif

#ifndef CFG_IO_HOLD_DIR
#define CFG_IO_HOLD_DIR                 CFG_DEFAULT_DIR
#endif

#ifndef CFG_IO_LED_SEG_DIR
#define CFG_IO_LED_SEG_DIR              CFG_DEFAULT_DIR
#endif


//����UI����
#ifndef CFG_TASK_UI
#define CFG_TASK_UI                     CFG_DEFAULT_DIR
#endif

#ifndef CFG_UI_COMM_DIR
#define CFG_UI_COMM_DIR                 CFG_TASK_UI
#endif

#ifndef CFG_UI_MUSIC_DIR
#define CFG_UI_MUSIC_DIR                CFG_TASK_UI
#endif

#ifndef CFG_UI_RECORD_DIR
#define CFG_UI_RECORD_DIR               CFG_TASK_UI
#endif

#ifndef CFG_UI_FM_DIR
#define CFG_UI_FM_DIR                   CFG_TASK_UI
#endif

#ifndef CFG_UI_CLOCK_DIR
#define CFG_UI_CLOCK_DIR                CFG_TASK_UI
#endif

#ifndef CFG_UI_BT_DIR
#define CFG_UI_BT_DIR                   CFG_TASK_UI
#endif

#ifndef CFG_UI_SPEAKER_DIR
#define CFG_UI_SPEAKER_DIR              CFG_TASK_UI
#endif

#ifndef CFG_UI_TOMCAT_DIR
#define CFG_UI_TOMCAT_DIR               CFG_TASK_UI
#endif

#ifndef CFG_UI_AUX_EQ_DIR
#define CFG_UI_AUX_EQ_DIR               CFG_TASK_UI
#endif

#ifndef CFG_UI_MENU_DIR
#define CFG_UI_MENU_DIR                 CFG_TASK_UI
#endif

#ifndef CFG_UI_USBDEV_DIR
#define CFG_UI_USBDEV_DIR               CFG_TASK_UI
#endif

#ifndef CFG_UI_HOLD_DIR
#define CFG_UI_HOLD_DIR                 CFG_TASK_UI
#endif

#ifndef CFG_UI_UPDATEFW_DIR
#define CFG_UI_UPDATEFW_DIR            	CFG_TASK_UI
#endif

/*****************************************************************************
 * Module    : ��Ӧ��Դͷ�ļ�
 *****************************************************************************/
#include SET_USER_PATH(CFG_DIR, /mp3res/mp3res.h)           //ѡ��MP3�����ļ�

/*****************************************************************************
 * Module    : makecfg.c��makecode�滻
 *****************************************************************************/
#ifndef CHARACTER_CONVERT
#define CHARACTER_CONVERT                0x00               // ��ʹ���ֿ�ת����
#endif

#ifndef CHARACTER
#define CHARACTER                        0x00               // ��ʹ���ֿ�
#endif

#ifndef PICTURE
#define PICTURE                          0x00               // ��ʹ��ͼƬ��Դ
#endif

#ifndef MENU_PICTURE
#define MENU_PICTURE                     0x00               // ��ʹ�ò˵�ͼƬ��Դ
#endif

#endif
