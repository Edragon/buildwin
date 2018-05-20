/*****************************************************************************
 * Module    : user
 * File      : user_fm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : FM��������غ�������
 *****************************************************************************/
#ifndef USER_FM_H
#define USER_FM_H


typedef struct {
    u8 w_reg[10];                       //FMд�Ĵ���
    u8 r_reg[10];                       //FM���Ĵ���
} type_fm_ctl;

extern type_fm_ctl fm_ctl;
extern u8 fm_online_flag;

#if ICC_REUSE_EN
extern volatile u8 iic_busy_flag;
#endif

#if FMOSC_REUSE_SDCLK && FM_TYPE==FM_RTC6218
    #define FM_SEEK_50K_STEP    1       //�⾧��ʱ��50Khz������̨
#else
    #define FM_SEEK_50K_STEP    0       //������ʱ��100Khz������̨
#endif

//FM�Ľӿ�
void fm_io_init(void);                  //IO��ʼ��
bool fm_init(void);                     //��ʼ��
void fm_set_freq(u16 freq);             //��̨
u8 fm_check_freq(u16 freq);           //
void fm_off(void);                      //�ر�
void fm_unmute(void);
void fm_set_vol(u8 volume);

__near_func void task_fm_dispmem_ch_num(u8 num, u8 x, u8 line);
__near_func void task_fm_display_ch(void);
__near_func void task_fm_display_freq(void);
void task_fm_display_val(void);
__near_func void task_fm_deal_msg(u8 msg);
__near_func void task_fm_display(void);
//������FMģ����ڲ��ӿ�

//IIC�ӿ�
__near_func void iic_start(void);
__near_func void iic_stop(void);
__near_func void iic_sendbyte(u8 val);
__near_func u8 iic_getbyte(void);
__near_func void iic_sendack(void);
__near_func void iic_sendnack(void);
__near_func bool iic_getack(void);

//RDA 5807 �ӿ�
bool fm_rda5807_init(void);             //��ʼ��
void fm_rda5807_off(void);              //�ر�
bool fm_rda5807_set_freq(u16 freq);     //��̨,�ѵ�̨�򷵻�ture
void fmrda5807_set_vol(u8 vol);
void rda5807_set_freq(u16 freq);

//BK 1080 �ӿ�
bool fm_bk1080_init(void);             //��ʼ��
void fm_bk1080_off(void);              //�ر�
bool fm_bk1080_set_freq(u16 freq);     //��̨
void bk1080_set_freq(u16 freq);        //����ĳ��Ƶ�ʵ�̨
void bk1080_unmute(void);

//QN 8035 �ӿ�
bool fm_qn8035_init(void);             //��ʼ��
void fm_qn8035_off(void);              //�ر�
bool fm_qn8035_set_freq(u16 freq);     //��̨
void qn8035_set_freq(u16 freq);        //����ĳ��Ƶ�ʵ�̨
void qn8035_set_vol(u8 volume);
void qn8035_unmute(void);

//QN 8065 �ӿ�
u8 qn8065_online(void);                 //��ʼ��
void qn8065_off(void);                  //�ر�
u8 qn8065_seek(u16 freq);               //��̨,�ѵ�̨�򷵻�ture
void QN8065_TuneToCH(UINT16 ch);        //����ĳ��Ƶ�ʵ�̨
void qn8065_set_vol(u8 volume);
void QN8065_SetMute(UINT8 On);

//AX 5111 �ӿ�
u8 ax5111_online();                     //��ʼ��
void ax5111_off();                      //�ر�
u8 ax5111_seek(u16 freq);               //��̨,�ѵ�̨�򷵻�ture
void ax5111_set_freq(u16 freq);         //����ĳ��Ƶ�ʵ�̨
void ax5111_set_vol(u8 vol);

//RTC 6218 �ӿ�
u8 RTC6218_online(void);                //��ʼ��
void RTC6218_off(void);                 //�ر�
__near_func u8 RTC6218_seek(u16 channel_freq);      //��̨,�ѵ�̨�򷵻�ture
void RTC6218_set_freq(u16 channel_freq);//����ĳ��Ƶ�ʵ�̨
void RTC6218_set_vol(u8 FM_Volumn);

//RTC 6207 �ӿ�
u8 RTC6207_online(void);                //��ʼ��
void RTC6207_off(void);                 //�ر�
u8 RTC6207_seek(u16 channel_freq);      //��̨,�ѵ�̨�򷵻�ture
void RTC6207_set_freq(u16 channel_freq);//����ĳ��Ƶ�ʵ�̨
void RTC6207_set_vol(u8 FM_Volumn);

#endif
