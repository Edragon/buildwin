/*****************************************************************************
 * Module    : User
 * File      : user_fat.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �û�¼������
 *****************************************************************************/
#include "include.h"
#include "user_spi.h"
#include "BtApi.h"

s8 agc_buf_cnt;
type_volume_ctl volume_ctl;             //����������
type_mp3en_ctrl mp3en_ctrl;             //¼��������

struct {
    u8 mode;
} agc_ctl;

extern IAR_BIT_A bool scoflag;  //for nokia 5000 use sco play music
extern u8 bt_debug_sco;

extern __near_func void aux_eq(void);
extern __near_func void loud_speaker(void);
extern __near_func u8 agchook_linein(void *buf);

//s16 *agc_far_wpos;
__near_func u8 agc_isr_hook(void *buf)
{
#if !BT_CALL_PRIVATE
    if(flag_btmode && scoflag)
    {
        //agc_far_wpos = sco_farpcm_wptr;
        //agc_far_wpos -= ((576)/24);// + 48;  // offset;
        //if(agc_far_wpos < sco_farpcm)
        //    agc_far_wpos += SCO_FARPCM_SIZE;

        //u32 e = voice_power(buf, 576);
        //printf("%ld\n", e);

        agc_buf_cnt += (576) / 48;  //BT_MIC�޸�Ϊ16K������
        if (agc_buf_cnt > 2) {
            bt_debug_sco += 3;                  //ADC�����ж࣬�ٶ෢
        }
        //KICK_SOFT_INT();
    }
#endif
    return 0;
}

//���AGCBuffer�Ľӿ�
#pragma location="MP3_EN_COM"
__near_func u8 mp3en_agcbuf_hook(void *buf)
{
    return 0;
}

#pragma location="LOUD_SPEAKER_COM"
__near_func void loud_speaker_out(void)
{
#if TASK_LOUDSPEAKER_EN
    loud_speaker();
#endif
}

#if (TASK_LOUDSPEAKER_EN && MIX_MODE_DECAY_NOISE)
__no_init u8 fade_cnt @ "MIX_XDATA";
__no_init u8 fade_delay @ "MIX_XDATA";
#pragma data_alignment=2
__no_init u16 low_pre @ "MIX_XDATA";
#pragma data_alignment=2
__no_init u16 out_pre @ "MIX_XDATA";
#pragma data_alignment=2
__no_init u16 threshold @ "MIX_XDATA";
#endif
void adc_decay_init(void)
{
#if (TASK_LOUDSPEAKER_EN && MIX_MODE_DECAY_NOISE)
    fade_delay = 0;
    low_pre = 0;
    out_pre = 0;
    fade_cnt = 0;
    threshold = 512;
#endif
}

//������MIC����
#pragma location="LOUD_SPEAKER_COM"
__near_func void mp3en_mic_noise_reduction(void *buf)
{
#if (TASK_LOUDSPEAKER_EN && MIX_MODE_DECAY_NOISE)
    extern u8 fade_delay;
    extern u8 fade_cnt;
    AUCON10 &= ~(BIT(6) | BIT(7));
    iir_hp(buf, 576, 1);
    u16 mean_val = abs_mean(buf, 576, 1);
    if (mean_val < 0x900)
    {
        if (fade_delay)
        {
            fade_delay--;
        }
        else
        {
            adc_fade(buf, 576, 1, fade_cnt);
            if(fade_cnt < 6)
                fade_cnt ++;
        }
    }
    else
    {
        fade_cnt = 0;
        fade_delay = 8;
    }
#endif
}

//ѡ��¼������
bool mp3en_set_record_mode(u8 mode_record)
{
    switch (mode_record) {
    case ITEM_RECORD_LOW:
        mp3en_ctrl.sample = SAMPLE_22050;
        mp3en_ctrl.bitrate = 64;                        //����
        break;
    case ITEM_RECORD_MID:
        mp3en_ctrl.sample = SAMPLE_44100;
        mp3en_ctrl.bitrate = 96;
        break;
    case ITEM_RECORD_HIGH:
        mp3en_ctrl.sample = SAMPLE_44100;
        mp3en_ctrl.bitrate = 128;
        break;
    default:
        return false;
    }
    return true;
}

//ѡ��¼��Դ��sys_cfg��ʾ�Ƿ����ϵͳ�����ã����ǹ̶�������
bool mp3en_source_init(u8 record_source)
{
    mp3en_ctrl.mp3_en = 1;
    mp3en_ctrl.dac_out = 0;
    switch (record_source) {
    case RECORD_LINEIN:             //LINE IN ¼��
    //case LINE_BT:
        //ѡ��LineIN¼��                                //�Ƿ�DAC����AMUX���������ڴ����Ҳ���DAC���벥�ţ�
        mp3en_ctrl.vol_out = 0;                         //���ⲿ����ʱһ��ʱ���ٴ�������AUXͨ�����ô���������
        mp3en_ctrl.channel = AUX_CHANNEL_SEL;           //LineIN ʹ�� AMUXͨ��
        mp3en_ctrl.agc_en = 0;                          //��¼MIC
        mp3en_ctrl.rec_volume = 3;                      //¼������]
        mp3en_ctrl.dac_out |= TWO_CHANAL;
        break;
   case LINE_BT:
        mp3en_ctrl.mp3_en = 0;
        mp3en_ctrl.vol_out = 1;                         //�Ƿ�DAC����AMUX������
        mp3en_ctrl.channel = AUX_CHANNEL_SEL;           //LineIN ʹ�� AMUXͨ��
        mp3en_ctrl.agc_en = 0;                          //��¼MIC
        mp3en_ctrl.rec_volume = 3;                      //¼������
        break;
    case AUX_EQ:
    case FM_EQ:
        if (AUX_EQ == record_source )
        {
            mp3en_ctrl.channel = AUX_CHANNEL_SEL;       //LineIN ʹ�� AMUXͨ��
        }
        else
        {
            mp3en_ctrl.channel = FM_CHANNEL_SEL;       //LineIN ʹ�� AMUXͨ��
        }
        mp3en_ctrl.vol_out = 0;                         //�Ƿ�DAC����AMUX������
        mp3en_ctrl.agc_en = 1;                          //����AGC
        mp3en_ctrl.rec_volume = 3;                      //¼������
        mp3en_ctrl.mp3_en = 0;                          //USB Audioʱ������Ҫ����MP3ѹ��
        mp3en_ctrl.dac_out |= (AUX_EQ_EN | TWO_CHANAL);
        break;
    case RECORD_MIC:                //MIC ¼��
    case RECORD_MIC_SPI:            //MIC ����
    case AUDIO_MIC:                 //USB Audio¼��
    case MIC_LOUD_SPEAKER:
        //ѡ��MIC¼��

        mp3en_ctrl.vol_out = 0;                         //�Ƿ�DAC����AMUX������

        mp3en_ctrl.channel = MIC_CHANNEL_SEL;               //MIC ʹ�� AMUXͨ�� P02
        mp3en_ctrl.agc_en = 1;                          //����AGC
        mp3en_ctrl.rec_volume = 10;                     //¼������
        if (RECORD_MIC_SPI == record_source) {
            mp3en_ctrl.agc_en = 0;                      //¼SPIʱ���ر�AGC�Լӿ�����ٶ�
        } else if (AUDIO_MIC == record_source )
        {
             mp3en_ctrl.mp3_en = 0;
        }
          else if (MIC_LOUD_SPEAKER == record_source) {
            mp3en_ctrl.mp3_en = 0;                      //SPEAKERʱ������Ҫ����MP3ѹ��
            mp3en_ctrl.dac_out |= SPEAKER_EN;
            mp3en_ctrl.rec_volume = 7;                     //¼������
        }
        break;

    case BT_MIC:
        //����ͨ��MIC����
        mp3en_ctrl.vol_out = 0;                         //�Ƿ�DAC����AMUX������
        mp3en_ctrl.channel = MIC_CHANNEL_SEL;           //MIC ʹ�� AMUXͨ�� P02
        mp3en_ctrl.agc_en = 0;                          //����AGC
        mp3en_ctrl.rec_volume = 10;                     //¼������
        mp3en_ctrl.mp3_en = 0;
        break;

    case RECORD_FM:                 //FM ¼��
    case RECORD_TAPE_SPI:           //�Ŵ�����¼��
        mp3en_ctrl.vol_out = 1;                         //�Ƿ�DAC����AMUX������
        mp3en_ctrl.channel = FM_CHANNEL_SEL;            //TAPE �� FM����ͨ��
        mp3en_ctrl.agc_en = 0;                          //��¼MIC
        mp3en_ctrl.rec_volume = 3;                      //¼������
        break;
    case RECORD_KARA:
        mp3en_ctrl.vol_out = 0;                         //�Ƿ�DAC����AMUX������
        mp3en_ctrl.mp3_en = 0;                          //����ģʽ��ҪMP3ѹ��ģ��
        mp3en_ctrl.channel = MIC_CHANNEL_SEL;               //MIC ʹ�� AMUXͨ�� P02
        mp3en_ctrl.agc_en = 0;                          //������AGC
        mp3en_ctrl.rec_volume = 9;
        break;
    default:
        return false;
    }

    mp3en_ctrl.rec_channel = mp3en_ctrl.channel;        //¼��ͨ����AMUXͨ����ͬ
    if (BT_MIC == record_source) {
        mp3en_ctrl.sample = SAMPLE_BT_MIC;                //����BT_MIC�Ĳ����ʣ�//BT_MIC�޸�Ϊ16K������
    } else if (AUDIO_MIC == record_source) {
        mp3en_ctrl.sample = SAMPLE_48000;               //����USB Audio�Ĳ�����
    } else if (record_source & RECORD_SPI) {
        mp3en_ctrl.sample = SAMPLE_44100;               //¼SPI������
        mp3en_ctrl.bitrate = SPI_BITRATE;
    } else if (record_source == MIC_LOUD_SPEAKER) {
        mp3en_ctrl.sample = SAMPLE_16000;               //����SPEAKER MIC�Ĳ�����
        mp3en_ctrl.dac_sample = SAMPLE_16000;               //����SPEAKER MIC DAC�Ĳ�����
    }
    else if (record_source == AUX_EQ || record_source == FM_EQ) {
        mp3en_ctrl.sample = SAMPLE_48000;               //����SPEAKER MIC�Ĳ�����
        mp3en_ctrl.dac_sample = SAMPLE_48000;               //����SPEAKER MIC DAC�Ĳ�����
        mp3en_ctrl.bitrate = 128;
    }
    else if (record_source == RECORD_KARA)
    {
        mp3en_ctrl.sample = SAMPLE_16000;               //������
      //  mp3en_ctrl.sample = encode_adc_sample;
        mp3en_ctrl.bitrate = 64;                        //λ��
    }
    else {                //ʹ��ϵͳ����
        if (!mp3en_set_record_mode(sys_ctl.mode_record)) {
            return false;
        }
    }

    //��ʼ��encoderģ��
    mp3en_init(&mp3en_ctrl);
    return true;
}
