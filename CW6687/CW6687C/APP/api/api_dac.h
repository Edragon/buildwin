/*****************************************************************************
 * Module    : API
 * File      : api_dac.h
 * Author    : Hanny
 * Function  : DAC与ADC的相关接口
 *****************************************************************************/
#ifndef API_DAC_H
#define API_DAC_H

//AMUX通道选择
#define CHANNEL_00              0x10            //单声道，使用P00 L
#define CHANNEL_01              0x01            //单声道，使用P01 R
#define CHANNEL_02              0x20            //单声道，使用P02 L
#define CHANNEL_03              0x02            //单声道，使用P03 R
#define CHANNEL_20              0x30            //单声道，使用P20 L
#define CHANNEL_21              0x03            //单声道，使用P21 R
#define CHANNEL_22              0x40            //单声道，使用P22 L
#define CHANNEL_23              0x04            //单声道，使用P23 R
#define CHANNER_VOUTR           0x50            //单声道，使用VOUTL L
#define CHANNER_VOUTL           0x05            //单声道，使用VOUTL R
#define CHANNER_MIC0            0x60            //单声道，使用MIC0 L
#define CHANNER_MIC1            0x06            //单声道，使用MIC1 R

//MIC通道选择
#define MIC_CHANNEL             0x08
#define MCHANNEL_0             (MIC_CHANNEL | CHANNER_MIC0)     //单声道，使用MIC0 L
#define MCHANNEL_1             (MIC_CHANNEL | CHANNER_MIC1)     //单声道，使用MIC1 R

//AMUX单声道双输出
#define CHANNEL_DL              0x70
#define CHANNEL_DR              0x07
#define CHANNEL_D00            (CHANNEL_00 | CHANNEL_DR)        //单声道双输出，使用P00 L
#define CHANNEL_D01            (CHANNEL_DL | CHANNEL_01)        //单声道双输出，使用P01 R
#define CHANNEL_D02            (CHANNEL_02 | CHANNEL_DR)        //单声道双输出，使用P02 L
#define CHANNEL_D03            (CHANNEL_DL | CHANNEL_03)        //单声道双输出，使用P03 R
#define CHANNEL_D20            (CHANNEL_20 | CHANNEL_DR)        //单声道双输出，使用P20 L
#define CHANNEL_D21            (CHANNEL_DL | CHANNEL_21)        //单声道双输出，使用P21 R
#define CHANNEL_D22            (CHANNEL_22 | CHANNEL_DR)        //单声道双输出，使用P22 L
#define CHANNEL_D23            (CHANNEL_DL | CHANNEL_23)        //单声道双输出，使用P23 R

//双声道，AMUX可以一左一右自由组合
#define TWO_CHANNEL             0x80
#define CHANNEL_00_01          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_01)     //双声道，使用P00 P01
#define CHANNEL_00_03          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_03)     //双声道，使用P00 P03
#define CHANNEL_00_21          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_21)     //双声道，使用P00 P21
#define CHANNEL_00_23          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_23)     //双声道，使用P00 P23
#define CHANNEL_02_01          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_01)     //双声道，使用P02 P01
#define CHANNEL_02_03          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_03)     //双声道，使用P02 P03
#define CHANNEL_02_21          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_21)     //双声道，使用P02 P21
#define CHANNEL_02_23          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_23)     //双声道，使用P02 P23
#define CHANNEL_20_01          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_01)     //双声道，使用P20 P01
#define CHANNEL_20_03          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_03)     //双声道，使用P20 P03
#define CHANNEL_20_21          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_21)     //双声道，使用P20 P21
#define CHANNEL_20_23          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_23)     //双声道，使用P20 P23
#define CHANNEL_22_01          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_01)     //双声道，使用P22 P01
#define CHANNEL_22_03          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_03)     //双声道，使用P22 P03
#define CHANNEL_22_21          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_21)     //双声道，使用P22 P21
#define CHANNEL_22_23          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_23)     //双声道，使用P22 P23

#define CLEFT_MASK              0x70
#define CRIGHT_MASK             0x07

enum {
    //固定频率
    SPR_48000,
    SPR_44100,
    SPR_32000,
    SPR_24000,
    SPR_22050,
    SPR_16000,
    SPR_12000,
    SPR_11025,
    SPR_8000,

    //同步频率
    S_SPR_48000,
    S_SPR_44100,
    S_SPR_32000,
    S_SPR_16000,
    S_SPR_8000,
};


void dac_init(u8 mode);                             //初始化DAC。mode: 0快速初始化，1无啪声初始化
void dac_off(u8 mode);                              //关闭DAC，用于消除关机啪声
void dac_left_right_mix_out(void);      			//左声道混合右声道输出，用于单声道案子（6685C和6685D）
u8 dac_dynamic(u8 dynamic_mode);                    //设置是否使用动态降噪，返回修改之前的值
void dac_set_mono(void);                            //设置DAC单声道输出(R+L)/2
void dac_enable(void);                              //开启DAC
void dac_disable(void);                             //关闭DAC
void dac_vcmbuf_en(void);                           //使用VCMBUF

void sys_mute(void);                                //MUTE住整个系统，包括DAC与功放等，
void sys_unmute(void);                              //解MUTE系统，系统可以出声

__near_func void at_write(u8 adr, u8 dat);
__near_func u8 at_read(u8 adr);
__near_func void dac_direct_out(u16 adr, u16 samples, u8 ch);

void pcm_init(u8 spr);                              //pcm数据直接推dac的初始化函数
void pcm_play(void);                                //开始播放PCM数据
void pcm_stop(void);                                //停止播放PCM数据
void mic_direct_out_enter(u8 channel,u8 vol);
void mic_direct_out_exit(void);
bool amux_init(u8 channel);                         //初始化AMUX通道
void amux_exit(void);                               //退出AMUX通道
void amux_play(void);                               //AMUX通道开始播放
void amux_stop(void);                               //AMUX通道停止播放

bool adc_init(u8 adc_type);                         //初始化ADC
void adc_start(void);                               //开始ADC采样
void adc_stop(void);                                //停止ADC采样
void adc_exit(void);                            	//关闭ADC模块

#endif
