/*****************************************************************************
 * Module    : user
 * File      : user_fm.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : FM驱动的相关函数定义
 *****************************************************************************/
#ifndef USER_FM_H
#define USER_FM_H


typedef struct {
    u8 w_reg[10];                       //FM写寄存器
    u8 r_reg[10];                       //FM读寄存器
} type_fm_ctl;

extern type_fm_ctl fm_ctl;
extern u8 fm_online_flag;

#if ICC_REUSE_EN
extern volatile u8 iic_busy_flag;
#endif

#if FMOSC_REUSE_SDCLK && FM_TYPE==FM_RTC6218
    #define FM_SEEK_50K_STEP    1       //免晶振时，50Khz步进搜台
#else
    #define FM_SEEK_50K_STEP    0       //共晶振时，100Khz步进搜台
#endif

//FM的接口
void fm_io_init(void);                  //IO初始化
bool fm_init(void);                     //初始化
void fm_set_freq(u16 freq);             //调台
u8 fm_check_freq(u16 freq);           //
void fm_off(void);                      //关闭
void fm_unmute(void);
void fm_set_vol(u8 volume);

__near_func void task_fm_dispmem_ch_num(u8 num, u8 x, u8 line);
__near_func void task_fm_display_ch(void);
__near_func void task_fm_display_freq(void);
void task_fm_display_val(void);
__near_func void task_fm_deal_msg(u8 msg);
__near_func void task_fm_display(void);
//以下是FM模块的内部接口

//IIC接口
__near_func void iic_start(void);
__near_func void iic_stop(void);
__near_func void iic_sendbyte(u8 val);
__near_func u8 iic_getbyte(void);
__near_func void iic_sendack(void);
__near_func void iic_sendnack(void);
__near_func bool iic_getack(void);

//RDA 5807 接口
bool fm_rda5807_init(void);             //初始化
void fm_rda5807_off(void);              //关闭
bool fm_rda5807_set_freq(u16 freq);     //调台,搜到台则返回ture
void fmrda5807_set_vol(u8 vol);
void rda5807_set_freq(u16 freq);

//BK 1080 接口
bool fm_bk1080_init(void);             //初始化
void fm_bk1080_off(void);              //关闭
bool fm_bk1080_set_freq(u16 freq);     //调台
void bk1080_set_freq(u16 freq);        //设置某个频率的台
void bk1080_unmute(void);

//QN 8035 接口
bool fm_qn8035_init(void);             //初始化
void fm_qn8035_off(void);              //关闭
bool fm_qn8035_set_freq(u16 freq);     //调台
void qn8035_set_freq(u16 freq);        //设置某个频率的台
void qn8035_set_vol(u8 volume);
void qn8035_unmute(void);

//QN 8065 接口
u8 qn8065_online(void);                 //初始化
void qn8065_off(void);                  //关闭
u8 qn8065_seek(u16 freq);               //调台,搜到台则返回ture
void QN8065_TuneToCH(UINT16 ch);        //设置某个频率的台
void qn8065_set_vol(u8 volume);
void QN8065_SetMute(UINT8 On);

//AX 5111 接口
u8 ax5111_online();                     //初始化
void ax5111_off();                      //关闭
u8 ax5111_seek(u16 freq);               //调台,搜到台则返回ture
void ax5111_set_freq(u16 freq);         //设置某个频率的台
void ax5111_set_vol(u8 vol);

//RTC 6218 接口
u8 RTC6218_online(void);                //初始化
void RTC6218_off(void);                 //关闭
__near_func u8 RTC6218_seek(u16 channel_freq);      //调台,搜到台则返回ture
void RTC6218_set_freq(u16 channel_freq);//设置某个频率的台
void RTC6218_set_vol(u8 FM_Volumn);

//RTC 6207 接口
u8 RTC6207_online(void);                //初始化
void RTC6207_off(void);                 //关闭
u8 RTC6207_seek(u16 channel_freq);      //调台,搜到台则返回ture
void RTC6207_set_freq(u16 channel_freq);//设置某个频率的台
void RTC6207_set_vol(u8 FM_Volumn);

#endif
