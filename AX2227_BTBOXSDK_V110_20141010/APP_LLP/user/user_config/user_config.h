/*****************************************************************************
 * Module    : User
 * File      : user_config.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : Ĭ���û��������ã��û��Զ��幦����ֱ�����Ӧ�Ĺ����½����޸�
 *****************************************************************************/
#ifndef USER_CONFIG_H
#define USER_CONFIG_H

#include "user_cfgdef.h"

/*****************************************************************************
 * Module    : �û�ѡ��
 *****************************************************************************/
#define USER_2227                       0x00        //NO.13-138,2227DEMO��
#define USER_S10                        0x01        //NO.13-146
#define USER_LEPA                       0x02        //NO.13-153
#define USER_COLA                       0x03        //NO.13-157,���ֹ���
#define USER_2226_S10                   0x04        //NO.13-161,����1006
#define USER_2226_DEMO                  0x05        //NO.14-xxx,��U��,FM��������
#define USER_2226_DEMO2                 0x06        //NO.14-xxx,��U��,ʡFM����
#define USER_2226_S10_SIMPLE            0x07        //NO.14-115,����1006,�����S10
#define USER_6681E_DEMO                 0x08        //NO.14-135,��U�̣�����6681EоƬ
#define USER_6680E_S10                  0x09        //NO.14-135,����U�̣�����6680EоƬ
#define USER_6680E_S10_SIMPLE           0x0A        //NO.14-147,����U�̣�����6680EоƬ 6680E_S10_SIMPLE
#define USER_6680M                      0x0B        //NO.14-136,6680M
#define USER_6682M                      0x0C        //NO.14-136,6682M
#define USER_6682E                      0x0D        //NO.14-152B,6682E
#define USER_2227_LCD                   0x0E        //NO.14-xxx,����LCD��ʾ,AX2227 CW6639 BT Boombox LCD DISPLAY V1.0.pdf


#define USER_SEL                        USER_2227

//�����û��Զ�������
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
 * Module    : ϵͳ����
 *****************************************************************************/
//���ط�װѡ��
#ifndef SYS_PKG
#define SYS_PKG                         PKG_LQFP48
#endif

//ϵͳʱ��Դѡ��
#ifndef SYS_CRY
#define SYS_CRY                         CRY_32K
#endif

//0������ʱʹ��P06��1������ʱʹ��P13��2��ʹ��P30���������ݲ�֧��
#ifndef MAKECODE_ID
#define MAKECODE_ID                     0   //Ĭ����P06��һ�㲻�Ķ�
#endif

//0: ����ҪSD������  1: ��ҪSD������(P2��)  2: ��ҪSD������(P3��)
#ifndef SD_UPDATE_ID
#define SD_UPDATE_ID                    1
#endif

//0����������Դ��1������������Դ��2��Ӣ��������Դ��0x80�����û��Զ�����Դ
#ifndef MP3RES_ID
#define MP3RES_ID                       0x02    //Ĭ����Ӣ��������Դ
#endif

//����COD�ļ���0�������룬��BT_NAME����д��ָ���������֣�1������(��λ1M Byte)����bt_data.binָ��������ַ������
#ifndef ALIGN_COD
#define ALIGN_COD                       0
#endif


/*****************************************************************************
 * Module    : ���˵��������������
 *****************************************************************************/
//�Ƿ��MUSIC����
#ifndef TASK_MUSIC_EN
#define TASK_MUSIC_EN                   0
#endif

//�Ƿ��FM����
#ifndef TASK_FM_EN
#define TASK_FM_EN                      0
#endif

//�Ƿ��LINEIN����
#ifndef TASK_AUX_EN
#define TASK_AUX_EN                     0
#endif

//�Ƿ����������
#ifndef TASK_BT_EN
#define TASK_BT_EN                      0
#endif

//USB�豸����
#ifndef TASK_USBDEV_EN
#define TASK_USBDEV_EN                  0
#endif

//�Ƿ�򿪹ػ�����
#ifndef TASK_POWEROFF_EN
#define TASK_POWEROFF_EN                0
#endif

//�Ƿ��MIC¼��ģ��
#ifndef TASK_MIC_EN
#define TASK_MIC_EN                     0
#endif

//�Ƿ��¼������ģ��
#ifndef TASK_VOICE_EN
#define TASK_VOICE_EN                   0
#endif

//�Ƿ��������ģ��
#ifndef TASK_LOUDSPEAKER_EN
#define TASK_LOUDSPEAKER_EN             0
#endif

//�Ƿ��RTC����
#ifndef TASK_RTC_EN
#define TASK_RTC_EN                     0
#endif

//�Ƿ�򿪿���okģ��
#ifndef TASK_KARAOKE_EN
#define TASK_KARAOKE_EN                 0
#endif

//IDLE
#ifndef TASK_IDLE_EN
#define TASK_IDLE_EN                    0
#endif


/*****************************************************************************
 * Module    : ����ѡ��
 *****************************************************************************/
//�������⣬����Ҫ�������е�ȫ�ײ���ѡ��������������ľֲ�����������
#ifndef THEME_SELECT
#define THEME_SELECT                    LEDSEG_THEME_DEFAULT
#endif

//���ֲ���
#if TASK_MUSIC_EN
#ifndef THEME_SELECT_MUSIC
#define THEME_SELECT_MUSIC              THEME_SELECT
#endif
#endif

//������
#if TASK_FM_EN
#ifndef THEME_SELECT_FM
#define THEME_SELECT_FM                 THEME_SELECT
#endif
#endif

//����
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

//USB�豸
#if TASK_USBDEV_EN
#ifndef THEME_SELECT_USBDEV
#define THEME_SELECT_USBDEV             THEME_SELECT
#endif
#endif

//ʱ��
#if TASK_RTC_EN
#ifndef THEME_SELECT_CLOCK
#define THEME_SELECT_CLOCK              THEME_SELECT
#endif
#endif

//����
#if ALARM_EN
#ifndef THEME_SELECT_ALARM
#define THEME_SELECT_ALARM              THEME_SELECT
#endif
#endif

//������
#if TASK_LOUDSPEAKER_EN
#ifndef THEME_SELECT_SPEAKER
#define THEME_SELECT_SPEAKER            THEME_SELECT
#endif
#endif

//��������
#ifndef THEME_SELECT_UPDATEFW
#define THEME_SELECT_UPDATEFW           THEME_SELECT
#endif

//�ػ�
#ifndef THEME_SELECT_POWEROFF
#define THEME_SELECT_POWEROFF           THEME_SELECT
#endif

//ledseg������ѡ��
#ifndef LEDSEG_TYPE_SELECT
#define LEDSEG_TYPE_SELECT              LEDSEG_5C7S_MHZ
#endif


/*****************************************************************************
 * Module    : ������ز���
 *****************************************************************************/
#define LCD_THEME                       ((THEME_SELECT >= 1) && (THEME_SELECT < 0x80))
#define LCDSEG_THEME                    ((THEME_SELECT >= 0x80) && (THEME_SELECT < 0xC0))
#define LEDSEG_THEME                    (THEME_SELECT >= 0xC0)


/*****************************************************************************
 * Module    : ϵͳ��ʾ���������
 *****************************************************************************/
//�Ƿ���Ҫʹ�ù̶���ʾ������(Ĭ��ʹ�ã�����ʹ������ʾ����������ϵͳ����)
#ifndef CONSTANT_WARNING_VOLUME_EN
#define CONSTANT_WARNING_VOLUME_EN      1
#endif

//��ʾ������(CONSTANT_WARNING_VOLUME_EN ��ʱ�˺����Ч)
#ifndef WARNING_VOLUME
#define WARNING_VOLUME                  10
#endif

//�Ƿ���Ҫ������ʾ��
#ifndef POWER_ON_VOICE
#define POWER_ON_VOICE                  0
#endif

//�Ƿ���Ҫ����ģʽ��ʾ��
#ifndef MUSIC_MODE_WARNING_MUSIC
#define MUSIC_MODE_WARNING_MUSIC        0
#endif

//�Ƿ���ҪU�̻�SD����ʾ��
#ifndef USB_SD_WARNING_MUSIC
#define USB_SD_WARNING_MUSIC            1
#endif

//�Ƿ���ҪFMģʽ��ʾ��
#ifndef FM_WARNING_MUSIC
#define FM_WARNING_MUSIC                1
#endif

//�Ƿ���ҪAUXģʽ��ʾ��
#ifndef AUX_WARNING_MUSIC
#define AUX_WARNING_MUSIC               1
#endif

//�Ƿ���Ҫ¼������ģʽ��ʾ��
#ifndef VOICE_MODE_WARNING_MUSIC
#define VOICE_MODE_WARNING_MUSIC        0
#endif

//�Ƿ���Ҫ¼��ģʽ��ʾ��
#ifndef MIC_RECORD_WARNING_MUSIC
#define MIC_RECORD_WARNING_MUSIC        0
#endif

//�Ƿ���Ҫ������ģʽ��ʾ��
#ifndef LOUDSPEAKER_WARNING_MUSIC
#define LOUDSPEAKER_WARNING_MUSIC       0
#endif

//�Ƿ���Ҫʱ��ģʽ��ʾ��
#ifndef CLOCK_MODE_WARNING_MUSIC
#define CLOCK_MODE_WARNING_MUSIC        0
#endif

//�Ƿ���ҪUSB�豸ģʽ��ʾ��
#ifndef USB_DEVICE_WARNING_MUSIC
#define USB_DEVICE_WARNING_MUSIC        0
#endif

//�Ƿ���Ҫ���籨����ʾ��
#ifndef BT_RING_NUMBER
#define BT_RING_NUMBER                  0
#endif

//�Ƿ���Ҫ����ģʽ��ʾ��
#ifndef BT_MODE_MUSIC
#define BT_MODE_MUSIC                   1
#endif

//�Ƿ���Ҫ����������ʾ��
#ifndef BT_CONNECT_MUSIC
#define BT_CONNECT_MUSIC                1
#endif

//�Ƿ���Ҫ�����Ͽ�������ʾ��
#ifndef BT_DISCONNECT_MUSIC
#define BT_DISCONNECT_MUSIC             1
#endif

//�Ƿ���Ҫ����ģʽ�˳���ʾ��
#ifndef BT_MODE_EXIT_MUSIC
#define BT_MODE_EXIT_MUSIC              0
#endif

//�Ƿ���Ҫ�͵���ʾ��
#ifndef LOWPOWER_MUSIC
#define LOWPOWER_MUSIC                  1
#endif

//�Ƿ���Ҫ�������ڵ�������Сʱ��ʾ��
#ifndef VOL_MAXMIN_MUSIC
#define VOL_MAXMIN_MUSIC                0
#endif

//����������ʾ��ֻ��һ�λ�һֱ��ֱ�������ɿ�(Ĭ��һֱ��)(VOL_MAXMIN_MUSIC��ʱ�˺����Ч)
#ifndef VOL_MUSIC_ONCE
#define VOL_MUSIC_ONCE                  0
#endif

//�Ƿ���Ҫ������
#ifndef KEY_VOICE_EN
#define KEY_VOICE_EN                    0
#endif


/*****************************************************************************
 * Module    : MUSICģ���������
 *****************************************************************************/
//�Ƿ���Ҫ�Զ��л��豸(ֻ��1�������豸ʱ����Ҫ�˹���)
#ifndef AUTO_CHANGE_DEVICE
#define AUTO_CHANGE_DEVICE              0
#endif

//�Ƿ�ʹ��ģʽ���л��豸(ֻ��1�������豸ʱ����Ҫ�˹���)
#ifndef K_MODE_CHANGE_DEVICE
#define K_MODE_CHANGE_DEVICE            0
#endif

//�����豸���Զ��л������ֲ���
#ifndef MUSIC_MODE_AUTO
#define MUSIC_MODE_AUTO                 0
#endif

//�ϵ����
#ifndef BREAK_POINT_EN
#define BREAK_POINT_EN                  0
#endif

//�����ʾ������ͬʱ��ID3��ʾ
#ifndef MUSIC_LRC_EN
#define MUSIC_LRC_EN                    0
#endif

//�Ƿ���Ҫ���Ȳ���U��
#ifndef FIRST_PLAY_UDISK
#define FIRST_PLAY_UDISK                0
#endif

//�Ƿ���ҪSD�����Ź���
#ifndef SD_CARD_EN
#define SD_CARD_EN                      1
#endif

//AB����(δ����)
#ifndef AB_REPEAT_EN
#define AB_REPEAT_EN                    0
#endif

//����ģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#ifndef MUSIC_FREQ_SHOW
#define MUSIC_FREQ_SHOW                 0
#endif

//�Ƿ���Ҫ����ID3��ʾ(��ҪLCD��ʾ������ͬʱ�򿪸����ʾ)
#ifndef MUSIC_ID3_EN
#define MUSIC_ID3_EN                    0
#endif

//�Ƿ���ҪMP3����ID3��ʾ(��ҪLCD��ʾ)
#ifndef MP3_ID3_EN
#define MP3_ID3_EN                      1
#endif

//�Ƿ���ҪWMA����ID3��ʾ(��ҪLCD��ʾ)
#ifndef WMA_ID3_EN
#define WMA_ID3_EN                      1
#endif

//�Ƿ���WAVҪ����ID3��ʾ(��ҪLCD��ʾ)
#ifndef WAV_ID3_EN
#define WAV_ID3_EN                      1
#endif


/*****************************************************************************
 * Module    : FM�����������
 *****************************************************************************/
//FM����ѡ��
#ifndef FM_TYPE
#define FM_TYPE                         FM_RDA5807
#endif

//�Ƿ���ҪFM¼��
#ifndef FM_RECORD_EN
#define FM_RECORD_EN                    0
#endif

//FMģʽ���Ƿ�ʹ�ö̰����ż�MUTE����(����PALY �� PLAY_S10)
#ifndef KU_PLAY_FM_MUTE_EN
#define KU_PLAY_FM_MUTE_EN              0
#endif

//�Ƿ�ʹ�ö̰����ż�����̨(����PALY �� PLAY_S10���رմ˺����Ĭ��ʹ�ó������ż�����̨)
#ifndef KU_PLAY_TO_SEEKCHANNEL
#define KU_PLAY_TO_SEEKCHANNEL          0
#endif

//FMģʽ�µ�������ʱ�Ƿ���Ҫ��BANK(��Ҫ���ڽ����������ʱSPI������)
#ifndef FM_SET_VOL_CHANGE_BANK
#define FM_SET_VOL_CHANGE_BANK          1
#endif

//��̨�������Ƿ���ʾ�ѵ���Ƶ��(Ĭ����ʾƵ��)
#ifndef SEEK_DISPLAY_CHANNEL_NUM
#define SEEK_DISPLAY_CHANNEL_NUM        0
#endif

//FM_VOUT�Ƿ񾭹�����
#ifndef FM_VOUT_THROUGH_MCU
#define FM_VOUT_THROUGH_MCU             1
#endif

//FMģʽ���Ƿ���Ҫ�Զ��������ߴ���(KU_PLAY_FM_MUTE_EN��ʱ�˺����Ч)
#ifndef FM_AUTO_STANDBY_EN
#define FM_AUTO_STANDBY_EN              0
#endif

//FMģʽ���Ƿ���Ҫ�����������
#ifndef FM_SOUND_DETECT_EN
#define FM_SOUND_DETECT_EN              0
#endif

//FMģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#ifndef FM_FREQ_SHOW
#define FM_FREQ_SHOW                    0
#endif


/*****************************************************************************
 * Module    : AUX¼�������������
 *****************************************************************************/
//LINEINģʽ���Ƿ�ʹ�ö̰����ż�MUTE����(����PALY �� PLAY_S10)
#ifndef KU_PLAY_LINEIN_MUTE_EN
#define KU_PLAY_LINEIN_MUTE_EN          1
#endif

//AUX�Ƿ���Ҫ¼��
#ifndef AUX_RECORD_EN
#define AUX_RECORD_EN                   0
#endif

//AUXģʽ���Ƿ���Ҫ�Զ��������ߴ���(KU_PLAY_LINEIN_MUTE_EN��ʱ�˺����Ч)
#ifndef AUX_AUTO_STANDBY_EN
#define AUX_AUTO_STANDBY_EN             0
#endif

//AUXģʽ���Ƿ�ʹ�ö̰�������������������
#ifndef AUX_KU_NEXT_PREV_VOL
#define AUX_KU_NEXT_PREV_VOL            0
#endif

//AUXͨ���Ƿ�ʹ��AGC�Ŵ�(�������AUX����������ʱ���Դ򿪴˺�)
#ifndef AUX_AGC_AMPLIFY
#define AUX_AGC_AMPLIFY                 0
#endif

//AUXģʽ���Ƿ���Ҫ�����������
#ifndef AUX_SOUND_DETECT_EN
#define AUX_SOUND_DETECT_EN             0
#endif

//AUXģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#ifndef AUX_FREQ_SHOW
#define AUX_FREQ_SHOW                   0
#endif


/*****************************************************************************
 * Module    : LOUDSPEAKER�����������
 *****************************************************************************/
//����������
#ifndef MIX_MODE_DECAY_NOISE
#define MIX_MODE_DECAY_NOISE            0
#endif


/*****************************************************************************
 * Module    : USB�豸�����������
 *****************************************************************************/
//USB�豸ö������
#ifndef USBDEV_ENUM_TYPE
#define USBDEV_ENUM_TYPE                (USB_DEV_MASS | USB_DEV_SPEAKER | USB_DEV_MIC | USB_DEV_HID)
#endif

//USB Audio������ģʽ��1Ϊֱ��ʹ��������������0Ϊ�Ȱ�������������Prev��Next�����е���
#ifndef USBAUDIO_VOLUME_MODE
#define USBAUDIO_VOLUME_MODE            1
#endif

//USB�豸ģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#ifndef USBAUDIO_FREQ_SHOW
#define USBAUDIO_FREQ_SHOW              0
#endif


/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/
//�Ƿ�ֻ��BT����
#ifndef TASK_BT_ONLY
#define TASK_BT_ONLY                    0
#endif

//�����ͺ�
#ifndef BT_TYPE
#define BT_TYPE                         BT6639
#endif

//��������IOѡ��0=P16 P17��1=P36 P37
#ifndef BT_UART_IO
#define BT_UART_IO                      0
#endif

//��������IOѡ��0=P34��1=P07
#ifndef BT_PWK_SEL
#define BT_PWK_SEL                      0
#endif

//ͨ��ʱMIC����
#ifndef MIC_GAIN_MAX
#define MIC_GAIN_MAX                    200 //�ο�ֵ��160~255
#endif

//Զ����������ֵ���ڸ�ֵʱMUTE����MIC(��ֵ����ʱ�������أ���Сʱͨ����϶�����)
#ifndef VAD_THRESHOLD
#define VAD_THRESHOLD                   7   //�ο�ֵ��6~8
#endif

//Զ����������ֵС�ڸ�ֵʱ�򿪱���MIC(��ֵ����ʱ�������أ���Сʱͨ����϶�����)
#ifndef VAD_THRESHOLD_MUTE
#define VAD_THRESHOLD_MUTE              7   //�ο�ֵ��6~8
#endif

//�Ƿ�ǿ��ʹ��˽�ܽ���(�ֻ��˽���)
#ifndef BT_CALL_PRIVATE
#define BT_CALL_PRIVATE                 0
#endif

//ͨ��ʱ�Ƿ���Ҫ�л��ֻ����������˽���
#ifndef BT_PHONE_SWITCH
#define BT_PHONE_SWITCH		            1
#endif

//����ģʽ���Ƿ�ҪĬ���������
#ifndef BT_VOLUME_MAX
#define BT_VOLUME_MAX                   0
#endif

//�Ƿ�ʹ�ö̰����ż��������͹Ҷϵ绰(����PALY �� PLAY_S10)
#ifndef KU_PLAY_ANSWER_AND_HANGUP
#define KU_PLAY_ANSWER_AND_HANGUP       0
#endif

//�Ƿ�ʹ��˫�� PLAY_S10 �����ܽӵ绰(Ĭ��ʹ�ó��� PLAY_S10 ���ܽӵ绰)(������ PLAY ��)
#ifndef KU_PLAY_S10_TWICE_REJECT
#define KU_PLAY_S10_TWICE_REJECT        0
#endif

//�Ƿ�ʹ��˫�� PLAY_S10 �����ز��绰(Ĭ��ʹ�ó��� PLAY_S10 ���ز��绰)(������ PLAY ��)
#ifndef KU_PLAY_S10_TWICE_CALLBACK
#define KU_PLAY_S10_TWICE_CALLBACK      0
#endif

//�Ƿ���ȫ��������֧������(��SDK�汾Ϊ��������̨�汾����֧��ȫ��������������Ҫ��ʹ��������̨�汾SDK)
#ifndef BT_ALL_TASK
#define BT_ALL_TASK                     0
#endif

//�Ƿ�ʹ�ó��� PLAY ���Ͽ�����(������ PLAY_S10 ��)
#ifndef KL_PLAY_DISCONNECT
#define KL_PLAY_DISCONNECT              0
#endif

//�Ƿ�ʹ�ó��� PLAY ���Ҷϵ绰(������ PLAY_S10 ��)
#ifndef KL_PLAY_HANDUP
#define KL_PLAY_HANDUP                  0
#endif

//��������״̬����ػ��󿪻��Զ������ɹ����Ƿ���Ҫ�Զ�����
#ifndef BT_SOFT_POWEROFF_AUTO_PLAY
#define BT_SOFT_POWEROFF_AUTO_PLAY      0
#endif

//��������״̬���л�ģʽ��ص�����ģʽ�Զ������ɹ����Ƿ���Ҫ���в���
#ifndef BT_CHANGE_MODE_AUTO_PLAY
#define BT_CHANGE_MODE_AUTO_PLAY        0
#endif

//�Ƿ�ʹ�ó��� PLAY_S10 ���Ͽ���������(������ PLAY ��)
#ifndef KL_PLAY_S10_DISCONNECT
#define KL_PLAY_S10_DISCONNECT          0
#endif

//���������Ƿ���ʾ3λ��ַ(�����ã����磺BW-BTBOX-***)
#ifndef BT_NAME_WITH_ADDR
#define BT_NAME_WITH_ADDR               0
#endif

//�����豸����
#ifndef BT_NAME
#define BT_NAME                         'B',  'W', '-', 'B', 'T', 'B', 'O', 'X',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,\
                                        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
#endif

//�������ֲ���ʱ�Ƿ�ʹ�ö�̬����
#ifndef BT_DYMANIC_SUPPRESSION
#define BT_DYMANIC_SUPPRESSION          1
#endif

//�Ƿ�ʹ�����ְ������Ź���
#ifndef NUMBER_KEY_TO_CALL
#define NUMBER_KEY_TO_CALL              0
#endif

//�Ƿ���Ҫ��̬������������
#ifndef BT_NAME_DYMANIC_CHANGE
#define BT_NAME_DYMANIC_CHANGE          0
#endif

//����ͨ��ʱ�Ƿ���Ҫ��ʾ��ȥ������ʱ��(ֻ��ѡ�� LEDSEG_THEME_DEFAULT �� LCD_THEME ʱ�˺����ò���Ч)
#ifndef BT_DISP_NUM_TIME
#define BT_DISP_NUM_TIME                0
#endif

//����ģʽ���Ƿ���ҪƵ����ʾ(��ҪLCD��ʾ)
#ifndef BT_FREQ_SHOW
#define BT_FREQ_SHOW                    0
#endif

//�Ƿ���SD_CMD��������λ��
#ifndef BTRST_REUSE_SDCMD
#define BTRST_REUSE_SDCMD               0
#endif


/*****************************************************************************
 * Module    : RTC�����������
 *****************************************************************************/
//�Ƿ���ʾ������(δ����)
#ifndef DATE_EN
#define DATE_EN                         0
#endif

//����
#ifndef ALARM_EN
#define ALARM_EN                        0
#endif

//IRRTC WAKE_UP ��ʹ��(δ����)
#ifndef IRTCC_WAKEUP_EN
#define IRTCC_WAKEUP_EN                 0
#endif


/*****************************************************************************
 * Module    : ϵͳϸ������
 *****************************************************************************/
//���Ž�MUTE��ʱcnt*5ms��ĳЩ���Ž�MUTE��ʱ�ϳ�����8002��MUTE��Ҫ��ʱ0.5s���Ҳ����������
//�ɸ���ʵ������������˺�ֻ�ڲ�����ʾ��ʱʹ��
#ifndef OPA_UNMUTE_DELAY
#define OPA_UNMUTE_DELAY                10  //10*5ms
#endif

//����������ϵͳ����Ϊ0ʱ�Ƿ�MUTE���ţ���ͬʱ��BT_DYMANIC_SUPPRESSION����ʵ�������������ļ�ʱMUTE���ţ�
#ifndef VOL_CTRL_MUTE
#define VOL_CTRL_MUTE                   0   //ע�⣺�����ŷ�Ӧ�����߿���MUTEʱ��ž�����������
#endif

//�Ƿ��ʡDAC����������ֱ���ݣ�һ���ƶ���ʱʹ��
#ifndef DAC_VCMBUF_EN
#define DAC_VCMBUF_EN                   0
#endif

//�Ƿ�ʹ�ò������ؿ������MP3��BT�Ĺ���
#ifndef MP3BT_DECT_EN
#define MP3BT_DECT_EN                   0
#endif

//�������MP3��BT��IO�Ƿ�������IO(��Ҫ����2226����P05��MP3BT�������)
#ifndef MP3BT_DECT_REUSE_EN
#define MP3BT_DECT_REUSE_EN             0
#endif

//�Ƿ���Ҫ���LINE IN��
#ifndef LINEIN_DETECT_EN
#define LINEIN_DETECT_EN                0
#endif

//linein�����Ƿ���Ҫ����led seg
#ifndef LINEIN_SEG_REUSE
#define LINEIN_SEG_REUSE                0
#endif

//�Ƿ�ʹ�ö������
#ifndef EAR_DETECT_EN
#define EAR_DETECT_EN                   0
#endif

//�Ƿ�ʹ��������˸(˫LEDʱ��������Ҫ��������ģʽ)
#ifndef LED_BLUE_BLINK
#define LED_BLUE_BLINK                  0
#endif

//�Ƿ�ʹ���̵���˸(˫LEDʱ���̵���Ҫ���ڷ�����ģʽ)
#ifndef LED_GREEN_BLINK
#define LED_GREEN_BLINK                 0
#endif

//�Ƿ���Ҫģʽ�л�ʱ������˸һ��
#ifndef MODE_CHANGE_BLUE_ON
#define MODE_CHANGE_BLUE_ON             1
#endif

//�Ƿ�ʹ�ó��� PLAY_S10 ���л�ģʽ(������ PLAY ��)
#ifndef KL_PLAY_S10_CHANGE_MODE
#define KL_PLAY_S10_CHANGE_MODE         1
#endif

//�Ƿ�ʹ�� HSF���л�ģʽ
#ifndef K_HSF_CHANGE_MODE
#define K_HSF_CHANGE_MODE               0
#endif

//�Ƿ�ʹ�����ػ�
#ifndef SOFT_POWER_ON_OFF
#define SOFT_POWER_ON_OFF               0
#endif

//�Ƿ���Ҫ����������FM
#ifndef POWERON_TO_FM
#define POWERON_TO_FM                   0
#endif

//¼�������ܿ��ء�Ϊ0ʱ�ر����е�¼������
#ifndef RECORD_EN
#define RECORD_EN                       0
#endif

//¼���ļ�������ѡ��
#ifndef RECORD_FILE_NAME
#define RECORD_FILE_NAME                SHORT_NAME
#endif

//�Ƿ���Ҫ¼�������ϲ���
#ifndef REC_PLAY_EN
#define REC_PLAY_EN                     1
#endif

//SD�����ʱ
#ifndef SD_DETECT_DELAY
#define SD_DETECT_DELAY                 50
#endif

//P35�ŵ�ʹ��ѡ������MUTE��ʱΪ0����������USB����ʱѡ1�������ô�ʱΪ2
#ifndef USE_P35_USBPOW_CTL
#define USE_P35_USBPOW_CTL              0
#endif

//IIC�ӿ��Ƿ���Ҫ��SD����P20 P27 ���� P31 P32
#ifndef IIC_REUSE_SD
#define IIC_REUSE_SD                    0
#endif

//�Ƿ���SDCLK��ΪFMʱ��Դ(��Ҫ�����޾���)
#ifndef FMOSC_REUSE_SDCLK
#define FMOSC_REUSE_SDCLK               0
#endif

//����SDCLK��ΪFMʱ��ԴʱFMʱ��Ƶ��ѡ��(ֻ�к� FMOSC_REUSE_SDCLK ��ʱ���˺����ò���Ч)
#ifndef FMOSC_FREQ_SEL
#define FMOSC_FREQ_SEL                  FMOSC_12M
#endif

//IICCLK�Ƿ���USB����(��Ҫ�����޾���SDCLK������12Mʱ��ʱ)
#ifndef IICCLK_REUSE_USB
#define IICCLK_REUSE_USB                0
#endif

//IICDAT�Ƿ���SDDAT����(��Ҫ�����޾���SDCLK������12Mʱ��ʱ)
#ifndef IICDAT_REUSE_SDDAT
#define IICDAT_REUSE_SDDAT              0
#endif

//�Ƿ���ADKEY���SD��
#ifndef SD_DECT_REUSE_ADKEY
#define SD_DECT_REUSE_ADKEY             0
#endif

//�Ƿ���LED���SD��(��Ҫ����2226 IO �ڲ�����ʱ)
#ifndef SD_DECT_REUSE_LED
#define SD_DECT_REUSE_LED               0
#endif

//�Ƿ���ADKEY���LINE IN����
#ifndef AUX_DECT_REUSE_ADKEY
#define AUX_DECT_REUSE_ADKEY            0
#endif

//�Ƿ���SD_CMD���������
#ifndef SDCMD_REUSE_EARDET
#define SDCMD_REUSE_EARDET              0
#endif

//�Ƿ���SD_CLK��ADKEY���
#ifndef SDCLK_REUSE_ADKEY
#define SDCLK_REUSE_ADKEY               0
#endif

//�Ƿ���IICCLK����������
#ifndef EAR_DETECT_REUSE_IICCLK
#define EAR_DETECT_REUSE_IICCLK         0
#endif

//�Ƿ���IICCLK��� LINE IN ����
#ifndef LINEIN_DETECT_REUSE_IICCLK
#define LINEIN_DETECT_REUSE_IICCLK      0
#endif

//�Ƿ���SD_CMD��LINE IN���
#ifndef SDCMD_REUSE_LINEIN_DET
#define SDCMD_REUSE_LINEIN_DET          0
#endif

//�Ƿ���ADKEY��LED��(��Ҫ����2226 IO �ڲ�����ʱ)
#ifndef LED_REUSE_ADKEY
#define LED_REUSE_ADKEY                 0
#endif

//�Ƿ�ʹ��USBDP/USBDM��LED��(��Ҫ����2226 IO �ڲ�����ʱ)(�򿪺���ʹ��U�̺�USBDEV)
#ifndef LED_USE_USBIO
#define LED_USE_USBIO                   0
#endif

//�Ƿ�ʹ��USBDP/USBDM��MUTE����(��Ҫ����2226 IO �ڲ�����ʱ)(�򿪺���ʹ��U�̺�USBDEV)
#ifndef MUTE_USE_USBIO
#define MUTE_USE_USBIO                  0
#endif

//��ʱ���޲���(ʱ���ɺ� STANDBY_TIME ����)�Ƿ���Ҫ�Զ��������ߴ���
#ifndef AUTO_STANDBY_EN
#define AUTO_STANDBY_EN                 1
#endif

//�������ߴ���ʱ��ѡ��(Ĭ��10����)
#ifndef STANDBY_TIME
#define STANDBY_TIME                    600
#endif

//�������ߴ������Ƿ���Ҫ���ѹ���
#ifndef STANDBY_WAKEUP_EN
#define STANDBY_WAKEUP_EN               1
#endif

//�Ƿ����BT��AUX����(����������ģʽ�¾����Ƿ�Ҫֱ���л���AUX)
#ifndef BT_AUX_MODE_ONLY
#define BT_AUX_MODE_ONLY                0
#endif

//����POWER��Ӳ�ػ�ʹ��
#ifndef IR_HARD_POWERDOWN_EN
#define IR_HARD_POWERDOWN_EN            0
#endif

//�Ƿ���Ҫ�͵�����
#ifndef LOWPOWER_WARNING
#define LOWPOWER_WARNING                0
#endif

//�͵����ѵ�ѹ����(Ĭ��0x80ʱԼΪ3.3-3.4V)(ֻ�к� LOWPOWER_WARNING ��ʱ�˺����Ч)
#ifndef LOWPOWER_WARNING_VOLTAGE
#define LOWPOWER_WARNING_VOLTAGE        0x80
#endif

//�͵�ػ���ѹ����(Ĭ��0x7dʱԼΪ3.2-3.3V)
#ifndef LOWPOWER_POWEROFF_VOLTAGE
#define LOWPOWER_POWEROFF_VOLTAGE       0x7d
#endif

//�Ƿ�ʹ�ö���IO��POWER��
#ifndef POWER_IOKEY_EN
#define POWER_IOKEY_EN                  0
#endif

//�Ƿ�ʹ�ö���IO��MODE��
#ifndef MODE_IOKEY_EN
#define MODE_IOKEY_EN                   0
#endif

//�Ƿ�ʹ�ö���IO�������Ӽ���
#ifndef VOL_IOKEY_EN
#define VOL_IOKEY_EN                    0
#endif

//�Ƿ�ʹ��EQ��ʾEq0-Eqn(Ĭ����ʾEQģʽ���Ƶ�ǰ������ĸ)
#ifndef EQ_DISPLAY_NUM
#define EQ_DISPLAY_NUM                  0
#endif

//�͵�ʱ�Ƿ���Ҫ��ʾһ��OFF������3���ػ�
#ifndef LOWER_POWER_DISPLAY_OFF
#define LOWER_POWER_DISPLAY_OFF         0
#endif

//�ػ�ʱ�Ƿ���Ҫ�ر�VPG33
#ifndef POWER_OFF_VPG33
#define POWER_OFF_VPG33                 0
#endif


/*****************************************************************************
 * Module    :AUXͨ������
 *****************************************************************************/
//ѡ��MIC��ͨ��
#ifndef MIC_CHANNEL_SEL
#define MIC_CHANNEL_SEL                 MCHANNEL_3
#endif

//ѡ��FM��ͨ��
#ifndef FM_CHANNEL_SEL
#define FM_CHANNEL_SEL                  CHANNEL_72
#endif

//ѡ��LINEIN��ͨ��
#ifndef AUX_CHANNEL_SEL
#define AUX_CHANNEL_SEL                 CHANNEL_01
#endif


/*****************************************************************************
 * Module    : ����������IO����
 *****************************************************************************/
//ADKey��ʹ�ã�0Ϊʹ��IOKey��1Ϊ1��ADKey��2Ϊ2��ADKey
#ifndef USE_ADKEY
#define USE_ADKEY                       0
#endif

//ѡ���1��ADKEY��IO
#ifndef ADKEY_PORT
#define ADKEY_PORT                      ADKEY_P06
#endif

//��1��ADKEY�Ƿ�ʹ���ڲ�10K����(ֻ��3������ʱ����ʹ��)
#ifndef ADKEY_INT_PU10K
#define ADKEY_INT_PU10K                 0
#endif

//ѡ���2��ADKEY��IO
#ifndef ADKEY2_PORT
#define ADKEY2_PORT                     ADKEY_P33
#endif

//��2��ADKEY�Ƿ�ʹ���ڲ�10K����(ֻ��3������ʱ����ʹ��)
#ifndef ADKEY2_INT_PU10K
#define ADKEY2_INT_PU10K                0
#endif

//����ң��(NECӲ��)
#ifndef HARD_IR_EN
#define HARD_IR_EN                      0
#endif

//����ң��(�����ֺ�����⣬ע�⣺�˺�ֻ��Է����ֵ�һ�����ң�أ�������ʹ�ô˿�ң���벻Ҫ�򿪴˺�)
#ifndef SOFT_IR_EN
#define SOFT_IR_EN                      0
#endif

//�Ƿ�ʹ����������IO��������MODE��PLAY��
#ifndef USE_MODE_PLAY_KEY
#define USE_MODE_PLAY_KEY               0
#endif

//sd_portֵ:0: P20 P21 P27,    1: P20 P21 P27, clk���ü��,    2: P30 P31 P32,    3: P30 P31 P32, clk���ü��
//ѡ��0��2ʱ����Ҫ����SD������ţ����SD_DETECT_INIT��IS_SD_IN
#ifndef SD_PORT_SEL
#define SD_PORT_SEL                     0
#endif

//�Ƿ�ʹ��P01�����ڴ�ӡ����
#ifndef DEBUG_UART_SEL
#define DEBUG_UART_SEL                  0
#endif

//�Ƿ�Ϊ��������������ֹ����
#ifndef IS_HSF_KEY
#define IS_HSF_KEY(k)                   0
#endif


/*****************************************************************************
 * Module    :IO��������
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
 * Module    :IIC IO����
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
 * Module    :LED IO����
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
 * Module    : �������IO����
 *****************************************************************************/
//����ģ��WAKEUP IO
#ifndef BT_SLEEP_IO_SET     //Ĭ��ʹ��P34����ʡwakeup�ţ�Ҫ����BT_IS_SLEEP()Ϊ0
    #define BT_SLEEP_IO_SET
    #define BT_IS_SLEEP()               (P3 & BIT(4))
#endif

//����ģ��WAKEUP IOû�и�����������ʱ������ɿ�
#ifndef BT_IS_SLEEP_INIT
#define BT_IS_SLEEP_INIT()
#endif
#ifndef BT_IS_SLEEP_END
#define BT_IS_SLEEP_END()
#endif

//����ģ�鸴λ IO
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

//������Դ����IO����
#ifndef BTLDO_POWIO_SET
    #define BTLDO_POWCTRL_SET
    #define BTLDO_DIR_INIT()
    #define BTLDO_EN()
    #define BTLDO_DIS()
#endif


/*****************************************************************************
 * Module    : LCD��ʾ�������
 *****************************************************************************/
//�Ƿ���Ҫ���͵�ַ����
#ifndef LCD_X_FIX
#define LCD_X_FIX                       0
#endif

//�Ƿ���Ҫ�����λLCD��
#ifndef LCD_SOFT_RESET
#define LCD_SOFT_RESET                  1
#endif

//�Ƿ���Ҫ����LCD��invert�Ĵ���
#ifndef LCD_INVERT_DISPLAY
#define LCD_INVERT_DISPLAY              0
#endif

//Ĭ�ϵ�LCD���Աȶ�(ȡֵ��Χ: 0~15)
#ifndef LCD_CONTRAST_DEFAULT
#define LCD_CONTRAST_DEFAULT            10
#endif

//LCD IO ����
#ifndef LCD_IO_SET
#define LCD_IO_SET
#define LCD_DATA                        5   //P25
#define LCD_CLK                         6   //P26
#define LCD_A0                          0   //P30
#define LCD_RES                         1   //p31
#define LCD_CS                          2   //P32
#define LCD_BACKLIGHT                   5   //p15
#endif

//LCD�˿�����
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

//LCD����IO�������
#ifndef LCD_BACKLIGHT_SET
#define LCD_BACKLIGHT_SET
#define LCD_BACKLIGHT_ON()              if (!sys_ctl.backlight_sta) {sys_ctl.backlight_sta = 1; P1DRV1 |= BIT(7); P1DIR |= BIT(LCD_BACKLIGHT);}
#define LCD_BACKLIGHT_OFF()             if (sys_ctl.backlight_sta)  {sys_ctl.backlight_sta = 0; P1DRV1 &= ~BIT(7);}
#endif


/*****************************************************************************
 * Module    : �ؼ������ͣ��ʱ�����
 *****************************************************************************/
//�˵��б�listbox��ͣ��ʱ��
#ifndef LIST_TIME
#define LIST_TIME                       80  //8��
#endif

//������progressbox��ͣ��ʱ��
#ifndef PROGRESS_TIME
#define PROGRESS_TIME                   30  //3��
#endif

//��Ϣ��ͣ��ʱ��
#ifndef MSGBOX_TIME
#define MSGBOX_TIME                     5   //1.5��
#endif

//��ֻ����ʾ������£��Ƿ�ʹ�ü򵥵���Ϣ��
#ifndef MSGBOX_SIMPLE
#define MSGBOX_SIMPLE                   1
#endif


/*****************************************************************************
 * Module    : ��������IO�������
 *****************************************************************************/
//��Դ������
#ifndef DCIN_INIT
#define DCIN_INIT()
#endif
#ifndef IS_DCIN
#define IS_DCIN()                       (0)
#endif

//�ϵ�ʱBT/MP3���
#ifndef MP3BT_DECT_INIT
#define MP3BT_DECT_INIT()
#endif
#ifndef MP3BT_DECT_IS_BT
#define MP3BT_DECT_IS_BT()              0
#endif

//����ʱҪ�Զ���
#ifndef MP3BT_DECT_END
#define MP3BT_DECT_END()
#endif

//�������
#ifndef EAR_DETECT_INIT
#define EAR_DETECT_INIT()
#endif

//����SDCMD���ʱ����Ҫ����IS_EAR_IN
#if !SDCMD_REUSE_EARDET
#ifndef IS_EAR_IN
#define IS_EAR_IN()                     0
#endif
#endif

//������IICCLK������ʱ����ɿ�
#if !EAR_DETECT_REUSE_IICCLK
#ifndef EAR_DETECT_END
#define EAR_DETECT_END()
#endif
#endif

//LINE IN��⣬����ADKEY���LINE IN ����ʱ����Ҫ����LINEIN_DETECT_INIT
#if !SDCMD_REUSE_LINEIN_DET && !AUX_DECT_REUSE_ADKEY
#ifndef LINEIN_DETECT_INIT
#define LINEIN_DETECT_INIT()
#endif
#endif

//����SDCMD����ADKEY���ʱ����Ҫ����IS_LINEIN_IN
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

//SD��������(���� ADKEY �� SD_CLK ���ʱ����Ҫ����SD_DETECT_INIT)
#if !SD_DECT_REUSE_ADKEY
#ifndef SD_DETECT_INIT
#define SD_DETECT_INIT()
#endif
#endif

//����clk���ʱ����Ҫ����IS_SD_IN
#if (SD_PORT_SEL != 1) && (SD_PORT_SEL != 3) && !SD_DECT_REUSE_ADKEY
#ifndef IS_SD_IN
#define IS_SD_IN()                      0
#endif
#endif

//SD��ⲻ����ADKEYʱ����ɿ�
#ifndef SD_DETECT_END
#define SD_DETECT_END()
#endif

//MUTE���ſ���
#ifndef MUSIC_MUTE
#define MUSIC_MUTE()
#endif
#ifndef MUSIC_UNMUTE
#define MUSIC_UNMUTE()
#endif

//AB/D�๦�ſ���
#ifndef OPA_CTL_INIT
#define OPA_CTL_INIT()
#endif
#ifndef OPA_CTL_HIGH
#define OPA_CTL_HIGH()
#endif
#ifndef OPA_CTL_LOW
#define OPA_CTL_LOW()
#endif

//���ػ�IO��������
#ifndef SOFT_POWER_CTL_INIT
#define SOFT_POWER_CTL_INIT()
#endif
#ifndef SOFT_POWER_CTL_ON
#define SOFT_POWER_CTL_ON()
#endif
#ifndef SOFT_POWER_CTL_OFF
#define SOFT_POWER_CTL_OFF()
#endif

//���ػ�IO�������
#ifndef SOFT_POWER_CHK_INIT
#define SOFT_POWER_CHK_INIT()
#endif
#ifndef SOFT_POWER_CHK_IS_ON
#define SOFT_POWER_CHK_IS_ON()          1
#endif

//USB��Դ����
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
 * Module    : ���߼�����IO�������
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
 * �������Զ�ѡ��ĺ�
 */
/*****************************************************************************
 * Module    : ϵͳ�������
 *****************************************************************************/
#if (SYS_PKG == PKG_SSOP24 || SYS_PKG == PKG_LQFP48_6680E || SYS_PKG == PKG_QFN48_6680M)
    #if (SYS_CRY != CRY_RC)
    #error "SSOP24ֻ��ѡ���޾��񷽰�"
    #endif
#endif


/*****************************************************************************
 * Module    : MUSIC�������
 *****************************************************************************/
//�ر����и�ʽ������ID3��ʾ
#if !MUSIC_ID3_EN
#undef MP3_ID3_EN
#undef WMA_ID3_EN
#undef WAV_ID3_EN
#define MP3_ID3_EN                      0
#define WMA_ID3_EN                      0
#define WAV_ID3_EN                      0
#endif


/*****************************************************************************
 * Module    : ���⼰�份���������
 *****************************************************************************/
#if !AUTO_STANDBY_EN && !IR_HARD_POWERDOWN_EN
    #undef STANDBY_WAKEUP_EN
    #define STANDBY_WAKEUP_EN           0   //����Ҫ�Զ��ػ����ߺ���ػ�ʱ������Ҫ����
#endif


#if !HARD_IR_EN && !SOFT_IR_EN
#undef  IR_HARD_POWERDOWN_EN
#define IR_HARD_POWERDOWN_EN            0
#endif

#if IR_HARD_POWERDOWN_EN
#ifndef IR_PWR_DAT
#define IR_PWR_DAT                      0x45   //�͹��ģ�����ң�ػ��Ѱ���ֵ
#endif
#endif


/*****************************************************************************
 * Module    : ¼���������
 *****************************************************************************/
#if !RECORD_EN
//�ر����е�¼���궨��
#undef FM_RECORD_EN
#undef TASK_MIC_EN
#undef AUX_RECORD_EN
#undef TASK_VOICE_EN
#undef REC_PLAY_EN
#define FM_RECORD_EN                    0
#define TASK_MIC_EN                     0
#define AUX_RECORD_EN                   0
#define TASK_VOICE_EN                   0  //���ڲ���Ҫ¼�����ܣ�ͬʱ�ر�¼����������
#define REC_PLAY_EN                     0
#endif

#if !FM_VOUT_THROUGH_MCU
#undef FM_RECORD_EN
#define FM_RECORD_EN                    0   //FM����������ʱ����֧��FM¼��
#endif


/*****************************************************************************
 * Module    : AUX�������
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
 * Module    : RTC�������
 *****************************************************************************/
#if !TASK_RTC_EN
#undef ALARM_EN
#undef DATE_EN
#define ALARM_EN                        0
#define ALARM_EN                        0
#endif


/*****************************************************************************
 * Module    : USB_GPIO�������
 *****************************************************************************/
#if (LED_USE_USBIO || MUTE_USE_USBIO)
    #define USB_GPIO_EN                 1
#else
    #define USB_GPIO_EN                 0
#endif


/*****************************************************************************
 * Module    : ICC�����������
 *****************************************************************************/
#if IIC_REUSE_SD || IICCLK_REUSE_USB || IICDAT_REUSE_SDDAT || EAR_DETECT_REUSE_IICCLK || LINEIN_DETECT_REUSE_IICCLK
    #define ICC_REUSE_EN                1
#else
    #define ICC_REUSE_EN                0
#endif


#endif
