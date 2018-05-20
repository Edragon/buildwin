/*****************************************************************************
 * Module    : API
 * File      : api_dac.h
 * Author    : Hanny
 * Function  : DAC��ADC����ؽӿ�
 *****************************************************************************/
#ifndef API_DAC_H
#define API_DAC_H

//AMUXͨ��ѡ��
#define CHANNEL_00              0x10            //��������ʹ��P00 L
#define CHANNEL_01              0x01            //��������ʹ��P01 R
#define CHANNEL_02              0x20            //��������ʹ��P02 L
#define CHANNEL_03              0x02            //��������ʹ��P03 R
#define CHANNEL_20              0x30            //��������ʹ��P20 L
#define CHANNEL_21              0x03            //��������ʹ��P21 R
#define CHANNEL_22              0x40            //��������ʹ��P22 L
#define CHANNEL_23              0x04            //��������ʹ��P23 R
#define CHANNER_VOUTR           0x50            //��������ʹ��VOUTL L
#define CHANNER_VOUTL           0x05            //��������ʹ��VOUTL R
#define CHANNER_MIC0            0x60            //��������ʹ��MIC0 L
#define CHANNER_MIC1            0x06            //��������ʹ��MIC1 R

//MICͨ��ѡ��
#define MIC_CHANNEL             0x08
#define MCHANNEL_0             (MIC_CHANNEL | CHANNER_MIC0)     //��������ʹ��MIC0 L
#define MCHANNEL_1             (MIC_CHANNEL | CHANNER_MIC1)     //��������ʹ��MIC1 R

//AMUX������˫���
#define CHANNEL_DL              0x70
#define CHANNEL_DR              0x07
#define CHANNEL_D00            (CHANNEL_00 | CHANNEL_DR)        //������˫�����ʹ��P00 L
#define CHANNEL_D01            (CHANNEL_DL | CHANNEL_01)        //������˫�����ʹ��P01 R
#define CHANNEL_D02            (CHANNEL_02 | CHANNEL_DR)        //������˫�����ʹ��P02 L
#define CHANNEL_D03            (CHANNEL_DL | CHANNEL_03)        //������˫�����ʹ��P03 R
#define CHANNEL_D20            (CHANNEL_20 | CHANNEL_DR)        //������˫�����ʹ��P20 L
#define CHANNEL_D21            (CHANNEL_DL | CHANNEL_21)        //������˫�����ʹ��P21 R
#define CHANNEL_D22            (CHANNEL_22 | CHANNEL_DR)        //������˫�����ʹ��P22 L
#define CHANNEL_D23            (CHANNEL_DL | CHANNEL_23)        //������˫�����ʹ��P23 R

//˫������AMUX����һ��һ���������
#define TWO_CHANNEL             0x80
#define CHANNEL_00_01          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_01)     //˫������ʹ��P00 P01
#define CHANNEL_00_03          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_03)     //˫������ʹ��P00 P03
#define CHANNEL_00_21          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_21)     //˫������ʹ��P00 P21
#define CHANNEL_00_23          (TWO_CHANNEL | CHANNEL_00 | CHANNEL_23)     //˫������ʹ��P00 P23
#define CHANNEL_02_01          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_01)     //˫������ʹ��P02 P01
#define CHANNEL_02_03          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_03)     //˫������ʹ��P02 P03
#define CHANNEL_02_21          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_21)     //˫������ʹ��P02 P21
#define CHANNEL_02_23          (TWO_CHANNEL | CHANNEL_02 | CHANNEL_23)     //˫������ʹ��P02 P23
#define CHANNEL_20_01          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_01)     //˫������ʹ��P20 P01
#define CHANNEL_20_03          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_03)     //˫������ʹ��P20 P03
#define CHANNEL_20_21          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_21)     //˫������ʹ��P20 P21
#define CHANNEL_20_23          (TWO_CHANNEL | CHANNEL_20 | CHANNEL_23)     //˫������ʹ��P20 P23
#define CHANNEL_22_01          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_01)     //˫������ʹ��P22 P01
#define CHANNEL_22_03          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_03)     //˫������ʹ��P22 P03
#define CHANNEL_22_21          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_21)     //˫������ʹ��P22 P21
#define CHANNEL_22_23          (TWO_CHANNEL | CHANNEL_22 | CHANNEL_23)     //˫������ʹ��P22 P23

#define CLEFT_MASK              0x70
#define CRIGHT_MASK             0x07

enum {
    //�̶�Ƶ��
    SPR_48000,
    SPR_44100,
    SPR_32000,
    SPR_24000,
    SPR_22050,
    SPR_16000,
    SPR_12000,
    SPR_11025,
    SPR_8000,

    //ͬ��Ƶ��
    S_SPR_48000,
    S_SPR_44100,
    S_SPR_32000,
    S_SPR_16000,
    S_SPR_8000,
};


void dac_init(u8 mode);                             //��ʼ��DAC��mode: 0���ٳ�ʼ����1��ž����ʼ��
void dac_off(u8 mode);                              //�ر�DAC�����������ػ�ž��
void dac_left_right_mix_out(void);      			//�����������������������ڵ��������ӣ�6685C��6685D��
u8 dac_dynamic(u8 dynamic_mode);                    //�����Ƿ�ʹ�ö�̬���룬�����޸�֮ǰ��ֵ
void dac_set_mono(void);                            //����DAC���������(R+L)/2
void dac_enable(void);                              //����DAC
void dac_disable(void);                             //�ر�DAC
void dac_vcmbuf_en(void);                           //ʹ��VCMBUF

void sys_mute(void);                                //MUTEס����ϵͳ������DAC�빦�ŵȣ�
void sys_unmute(void);                              //��MUTEϵͳ��ϵͳ���Գ���

__near_func void at_write(u8 adr, u8 dat);
__near_func u8 at_read(u8 adr);
__near_func void dac_direct_out(u16 adr, u16 samples, u8 ch);

void pcm_init(u8 spr);                              //pcm����ֱ����dac�ĳ�ʼ������
void pcm_play(void);                                //��ʼ����PCM����
void pcm_stop(void);                                //ֹͣ����PCM����
void mic_direct_out_enter(u8 channel,u8 vol);
void mic_direct_out_exit(void);
bool amux_init(u8 channel);                         //��ʼ��AMUXͨ��
void amux_exit(void);                               //�˳�AMUXͨ��
void amux_play(void);                               //AMUXͨ����ʼ����
void amux_stop(void);                               //AMUXͨ��ֹͣ����

bool adc_init(u8 adc_type);                         //��ʼ��ADC
void adc_start(void);                               //��ʼADC����
void adc_stop(void);                                //ֹͣADC����
void adc_exit(void);                            	//�ر�ADCģ��

#endif
