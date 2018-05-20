/*****************************************************************************
 * Module    : User
 * File      : user_setting.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ϵͳ��������
 *****************************************************************************/
#ifndef USER_SETTING_H
#define USER_SETTING_H

//������Ϣƫ�Ƶ�ַ
#define PARAM_LANG_ID           0       //����          1byte
#define PARAM_CONTRAST          1       //�Աȶ�        1byte
#define PARAM_MODE_PLAY         2       //����ģʽ      1byte
#define PARAM_MODE_RECORD       3       //¼��ģʽ      1byte
#define PARAM_VOLUME            4       //ϵͳ����      1byte
#define PARAM_BACKLIGHT         5       //������ʱ      1byte
#define PARAM_POWEROFF          6       //�Զ��ػ�      1byte
#define PARAM_ALARM_HOUR        7       //����ʱ��      2byte
#define PARAM_ALARM_MIN         8
#define PARAM_ALARM_MODE        9       //����ģʽ      1byte
#define PARAM_ALARM_SWITCH      10      //���ӿ���      1byte
#define PARAM_MUSIC_NUM_SD      11      //��ǰ��Ŀ      2byte
#define PARAM_EQ_NUM            13      //EQѡ��        1byte
#define PARAM_EBOOK_NUM         14      //��ǰ������    2byte
#define PARAM_EBOOK_MODE        16      //�Ķ�ģʽ      1byte
#define PARAM_EBOOK_SPEED       17      //�Ķ��ٶ�      1byte
#define PARAM_VOICE_NUM         18      //VIOCE��Ŀ     2byte
#define PARAM_MUSIC_NUM_USB     20      //U����Ŀ       2byte

#define PARAM_FM_CHSEL          22      //��ǰ̨��  1byte
#define PARAM_FM_FREQ           23      //��ǰ̨��Ƶ�� 2byte
#define PARAM_MIX_STEP          25      //��ǰ�Ļ��켶�� 1byte
#define PARAM_FM_TIME           26      //��ʱfm��־
#define PARAM_FM_HOUR           27
#define PARAM_FM_MIN            28
#define PARAM_FM_TIME_FREQ      29      //2byte
#define PARAM_EBOOK_FPTR        31      //������ϵ��ַ���� 4byte

#define PARAM_HSHF_VOLUME       35      //ͨ������      1byte
#define PARAM_BT_AUTO_PLAY      36      //��������״̬����ػ��󿪻��Զ������ɹ����Ƿ���Ҫ�Զ�����
#define PARAM_REMOTE_DEV_ADDR   37      //�����豸��������ַ    6byte

#define PARAM_BT_NAME           43      //����̬�޸ĺ����������  32byte
#define PARAM_BT_PINCODE_SUM    75      //�����������У����  1byte
#define PARAM_BT_PINCODE        76      //�����������  4byte

//#define PARAM_BT_SAVE_NUM       61      //��������豸����  1byte
//#define PARAM_BT_PAIRED_INDEX   62      //�����Ϣ����  2byte���ܹ���MAX_PAIRED_DEVICE_NUM*2 byte

#define PARAM_FM_CH             80      //��һ����Ƶ������2byte���������Ƶ������160byte

#define VOLUME_DEFAULT      10          //����Ĭ��ֵ
#define HSHFVOL_DEFAULT     12          //ͨ������Ĭ��ֵ
#define VOLUME_MIN          5           //���������С����

//һЩ���������ֵ
#define CONTRAST_MAX        15          //�Աȶȵ����ֵ
#define MUSIC_EQ_NUM        7           //EQ�ĸ���

#ifndef MODE_PLAY_NUM
#define MODE_PLAY_NUM       4           //����ģʽ������
#endif

#define MODE_RECORD_MAX     2           //¼��ģʽ�����ֵ
#define VOLUME_MAX          15          //�������
#define BACKLIGHT_NUM       6           //������������
#define POWEROFF_NUM        5           //�Զ��ػ���������
#define MAX_ALARM_HOUR      23          //����ʱ�����ֵ
#define MAX_ALARM_MIN       59
#if ALM_TYPE
#define MAX_ALARM_MODE      1          //����ģʽ���ֵ
#else
#define MAX_ALARM_MODE      0x7f       //����ģʽ���ֵ
#endif

#define MIX_STEP_MAX        30          //���켶�����ֵ

typedef struct {
#if LCD_THEME
    u8 lang_id;                 //����ѡ��
    u8 lang_num;                //��������
    u8 contrast;                //�Աȶ�
    u8 backlight_sta;           //��ǰ״̬
#endif
    u8 eq_num;                  //EQѡ��
    u8 mode_play;               //����ģʽ
    u8 mode_record;             //¼��ģʽ
    u8 volume;                  //����
    u8 hshf_vol;                //ͨ������
    s8 vol_ex;                  //��������

#if ALARM_EN
    u8 alarm_hour;              //����: ʱ
    u8 alarm_min;               //����: ��
    u8 alarm_switch;            //���ӿ���
    u8 ring_times;              //�������
    u32 next_alarm;             //��һ����ʱ��
#endif

    u8 bat_val;                 //ʵ�ʵ�ص���
    u8 disp_bat_val;            //��ǰ��ʾ�ĵ�ص���
    u16 sleep_cnt;              //������ʱ
    u8 lowpower_cnt;            //�͵���ʱ

    u8 dig_vol;                 //��������
    u8 mic_volume;              //��˷������

#if IR_HARD_POWERDOWN_EN || STANDBY_WAKEUP_EN
    u8 wakeup_delay;            //���Ѻ���ʱ
#endif
#if VOL_MAXMIN_MUSIC
    u8 vol_maxmin_music_flag;   //�������������Сֵʱ������ʾ�����ɿ��������ڰ�����־
#endif
#if LOWPOWER_WARNING
    u8 lowpower_warning_flag;   //�͵����ѱ�־��������͵�������ʾ����ָ���������
#endif
} type_sys_ctl;

typedef union
{
    struct {
        u8 mute_flag :              1;      //ϵͳmute��־
        u8 need_mute :              1;      //��Ҫ���ڶ�̬����Ƿ���ҪMUTE���ţ���ǲ���״̬
        u8 voice_play_flag :        1;      //����������־
        u8 ir_power_flag :          1;      //����POWER���ػ���־(����hold_mode)
        u8 lowpower_flag :          1;      //�͵��־(����task_poweroff)
#if BT_SOFT_POWEROFF_AUTO_PLAY || BT_CHANGE_MODE_AUTO_PLAY
        u8 bt_auto_play_flag :      1;      //�����Զ����ſ��Ʊ�־
#endif
    };
    u8 all;                                 //���8��bit
} type_sys_flag;

extern IAR_BDATA type_sys_flag sys_flag;
extern IAR_XDATA type_sys_ctl sys_ctl;

void digital_vol_set(u16 dig_vol);
void agc_anl_set(u8 vol);

bool user_set_eq(u8 eq_num);
void user_set_mode_play(u8 mode_play);
bool user_set_volume(u8 volume);
bool user_set_hshfvol(u8 volume);
bool user_change_volume(u8 volume);
bool change_digital_volume(u8 volume);
bool change_mic_volume(u8 volume);
u8 user_volume_inc(u8 volume);
u8 user_volume_dec(u8 volume);
u8 user_mixstep_inc(u8 step);
u8 user_mixstep_dec(u8 step);
u8 user_eq_adjust(u8 eq_num);
u8 user_mode_play_adjust(u8 mode_play);


void user_set_volex(s8 volex);
void user_set_backlight(u8 backlight);
void user_set_alarm_time(u8 hour, u8 min);
void user_set_alarm_switch(u8 alarm_switch);
__near_func void set_backlight(void);
__near_func void reset_backlight(void);

#endif
