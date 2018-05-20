/*****************************************************************************
 * Module    : User
 * File      : user_setting.h
 * Author    : Hanny
 * Function  : ϵͳ��������
 *****************************************************************************/
#ifndef USER_SETTING_H
#define USER_SETTING_H

/*****************************************************************************
 * Module    : RTC�����Ϣƫ�Ƶ�ַ
 *****************************************************************************/
#define NEXT_ALARM          0       //��һ������ʱ�䣬4byte

//�ϵ����
#define USB_BREAK_FRAME     10        //4BYTE
#define USB_BREAK_POS       14       //4BYTE
#define USB_NAME_CRC        18       //2BYTE

#define SD_BREAK_FRAME      20       //4BYTE
#define SD_BREAK_POS        24       //4BYTE
#define SD_NAME_CRC         28       //2BYTE

//����������Ϣ
#define ALM_MUSIC_TYPE      30       //1byte
#define ALM_FILE_NUM        31       //2byte
#define ALM_NAME_CRC        33       //2BYTE

#define FIRST_POWERON_FLAG  35       //1BYTE
/*****************************************************************************
 * Module    : Flashϵͳ��Ϣƫ�Ƶ�ַ
 *****************************************************************************/
#define PARAM_RANDOM_KEY        0       //����ִ���������������ַ  4byte

#define PARAM_LANG_ID           4       //����          1byte
#define PARAM_CONTRAST          5       //�Աȶ�        1byte
#define PARAM_MODE_PLAY         6       //����ģʽ      1byte
#define PARAM_MODE_RECORD       7       //¼��ģʽ      1byte
#define PARAM_VOLUME            8       //ϵͳ����      1byte
#define PARAM_BACKLIGHT         9       //������ʱ      1byte
#define PARAM_POWEROFF          10      //�Զ��ػ�      1byte
#define PARAM_ALARM_HOUR        11      //����ʱ��      2byte
#define PARAM_ALARM_MIN         12
#define PARAM_ALARM_MODE        13      //����ģʽ      1byte
#define PARAM_ALARM_SWITCH      14      //���ӿ���      1byte
#define PARAM_MUSIC_NUM_SD      15      //��ǰ��Ŀ      2byte
#define PARAM_EQ_NUM            17      //EQѡ��        1byte
#define PARAM_EBOOK_NUM         18      //��ǰ������    2byte
#define PARAM_EBOOK_MODE        20      //�Ķ�ģʽ      1byte
#define PARAM_EBOOK_SPEED       21      //�Ķ��ٶ�      1byte
#define PARAM_VOICE_NUM         22      //VIOCE��Ŀ     2byte
#define PARAM_MUSIC_NUM_USB     24      //U����Ŀ       2byte

#define PARAM_FM_CHSEL          26      //��ǰ̨��  1byte
#define PARAM_FM_FREQ           27      //��ǰ̨��Ƶ�� 2byte
#define PARAM_MIX_STEP          29      //��ǰ�Ļ��켶�� 1byte
#define PARAM_FM_TIME           30      //��ʱfm��־
#define PARAM_FM_HOUR           31
#define PARAM_FM_MIN            32
#define PARAM_FM_TIME_FREQ      33      //2byte
#define PARAM_EBOOK_FPTR        35      //������ϵ��ַ���� 4byte


#define PARAM_HSHF_VOLUME       39      //ͨ������          1byte
#define PARAM_PLAY_DEVICE       40      //�����豸����      1byte
#define PARAM_PHONEBOOK_NUM     41      //�绰����ϵ������  2byte
#define PARAM_PHONEBOOK_BT_ADDR 43      //��ǰ�ѻ�ȡ�绰����Ϣ��Զ���豸������ַ 6byte

//#define PARAM_BT_AUTO_PLAY            //��������״̬����ػ��󿪻��Զ������ɹ����Ƿ���Ҫ�Զ�����
//#define PARAM_REMOTE_DEV_ADDR         //�����豸��������ַ    6byte

//#define PARAM_BT_NAME           43      //����̬�޸ĺ����������  32byte
//#define PARAM_BT_PINCODE_EN     75      //�Ƿ�ʹ�������������  1byte
//#define PARAM_BT_PINCODE        76      //�����������  4byte

//#define PARAM_BT_SAVE_NUM       61      //��������豸����  1byte
//#define PARAM_BT_PAIRED_INDEX   62      //�����Ϣ����  2byte���ܹ���MAX_PAIRED_DEVICE_NUM*2 byte

#define PARAM_FM_CH             80      //��һ����Ƶ������2byte���������Ƶ������160byte

#define VOLUME_DEFAULT      10          //����Ĭ��ֵ
#define HSHFVOL_DEFAULT     12          //ͨ������Ĭ��ֵ
#define VOLUME_MIN          5           //���������С����

//һЩ���������ֵ
#define CONTRAST_MAX        15          //�Աȶȵ����ֵ
#if MUSIC_DRC_EN
    #define MUSIC_EQ_NUM    7           //EQ�ĸ���
#else
    #define MUSIC_EQ_NUM    6           //EQ�ĸ���
#endif

#ifndef MODE_PLAY_NUM
#define MODE_PLAY_NUM       6           //����ģʽ������
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

typedef struct {
    u8 lang_id;                 //����ѡ��
    u8 lang_num;                //��������
    u8 contrast;                //�Աȶ�
    u8 eq_num;                  //EQѡ��
    u8 mode_play;               //����ģʽ
    u8 mode_record;             //¼��ģʽ
    u8 volume;                  //����
    u8 hshf_vol;                //ͨ������
    s8 vol_ex;                  //��������
    u8 backlight;               //����ʱ��ѡ��
    u8 poweroff;                //�Զ��ػ�ʱ��

    u8 backlightcnt;            //����ʱ��
    u8 backlight_sta;           //��ǰ״̬
    u16 poweroffcnt;            //��ʱ�ػ�ʱ�������

    u8 alarm_hour;              //����: ʱ
    u8 alarm_min;               //����: ��
    u8 alarm_mode;              //����ģʽ
    u8 alarm_switch;            //���ӿ���
    u8 bat_val;                 //ʵ�ʵ�ص���
    u8 disp_bat_val;            //��ǰ��ʾ�ĵ�ص���
    u16 sleep_cnt;              //��ͣ�����߼���
    u8 lowpower_cnt;            //�͵���ʱ

    u8 bt_sta;                  //����״̬
    u8 connected;               //�����Ƿ�������

    u8 voice_play_flag;         //����������־
    u8 mute_flag;               //ϵͳmute��־
    u8 spk_mute;                //�����Ƿ�MUTEס��־
    u8 dig_vol;                 //��������
    u8 mic_volume;              //��˷������
    u8 time_fm_on;              //��ʱfm���ر�־
    u8 fm_min;
    u8 fm_hour;
    u8 bt_online_flag;          //�������߱�־
    u8 bt_online_change;        //��������״̬�ı�
    u32 sclust;                 //¼���ļ���ʼ��
    u16 file_num;
    u8 vol_maxmin_music_flag;   //�������������Сֵʱ������ʾ�����ɿ��������ڰ�����־
    u8 vol_maxmin_music_play;   //�������������Сֵʱ������ʾ����־
    bool sleep_flag;            //�ػ���־( ����hold_mode)
    bool lowpower_flag;         //�͵��־
    u8 lowpower_warning_flag;   //�͵����ѱ�־
    u8 lowpower_warning_cycle;  //�͵�����ѭ���ӳ٣�����͵�ʱ
    bool led_status_blink_flag;
    bool soft_poweroff_flag;    //��ػ���־ 0: Ĭ��״̬��1��������ػ�״̬
#if BT_AUTO_PLAY
    bool bt_auto_play_flag;     //��������״̬����ػ��󿪻��Զ������ɹ����Ƿ���Ҫ�Զ�����
#endif
    bool ir_power_flag;         //����POWER��Ӳ�ػ���־
    bool alarm_wakeup_flag;
} type_sys_ctl;

#if ALARM_EN
extern u32 next_alarm;          //��һ����ʱ��
#endif

extern IAR_BIT_A bool mute_flag;


extern type_sys_ctl sys_ctl;
extern IAR_CONST type_progress_param tbl_pg_volume;
extern IAR_CONST type_progress_param tbl_digital_volume;
extern IAR_CONST type_progress_param tbl_mic_volume;
extern IAR_XDATA_A u8 bteq_buf[123];

void digital_vol_set(u16 dig_vol);
void agc_anl_set(u8 vol);

void user_set_lang(u8 lang_id);
void user_set_contrast(u8 contrast);
void user_def_eq(void);
bool user_set_eq(u8 eq_num);
void user_set_mode_play(u8 mode_play);
void user_set_mode_record(u8 mode_record);
bool user_set_volume(u8 volume);
bool user_set_hshfvol(u8 volume);
bool user_change_volume(u8 volume);
bool change_digital_volume(u8 volume);
bool change_mic_volume(u8 volume);
u8 user_volume_inc(u8 volume);
u8 user_volume_dec(u8 volume);
u8 user_mode_play_adjust(u8 mode_play);


void user_set_volex(s8 volex);
void user_set_backlight(u8 backlight);
void user_set_poweroff(u8 poweroff);
void user_set_alarm_time(u8 hour, u8 min);
void user_set_alram_mode(u8 alarm_mode);
void user_set_alarm_switch(u8 alarm_switch);
__near_func void set_backlight(void);
__near_func void reset_backlight(void);
__near_func void set_poweroff(void);
#endif
