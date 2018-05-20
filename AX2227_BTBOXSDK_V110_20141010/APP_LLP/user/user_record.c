/*****************************************************************************
 * Module    : User
 * File      : user_fat.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户录音定义
 *****************************************************************************/
#include "include.h"
#include "user_spi.h"
#include "BtApi.h"

s8 agc_buf_cnt;
type_volume_ctl volume_ctl;             //音量控制器
type_mp3en_ctrl mp3en_ctrl;             //录音控制器

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

        agc_buf_cnt += (576) / 48;  //BT_MIC修改为16K采样率
        if (agc_buf_cnt > 2) {
            bt_debug_sco += 3;                  //ADC收数有多，再多发
        }
        //KICK_SOFT_INT();
    }
#endif
    return 0;
}

//检查AGCBuffer的接口
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

//扩音器MIC降噪
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

//选择录音质量
bool mp3en_set_record_mode(u8 mode_record)
{
    switch (mode_record) {
    case ITEM_RECORD_LOW:
        mp3en_ctrl.sample = SAMPLE_22050;
        mp3en_ctrl.bitrate = 64;                        //码率
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

//选择录音源，sys_cfg表示是否采用系统的配置，还是固定的配置
bool mp3en_source_init(u8 record_source)
{
    mp3en_ctrl.mp3_en = 1;
    mp3en_ctrl.dac_out = 0;
    switch (record_source) {
    case RECORD_LINEIN:             //LINE IN 录音
    //case LINE_BT:
        //选择LineIN录音                                //是否DAC播放AMUX的声音，在此暂且不打开DAC淡入播放，
        mp3en_ctrl.vol_out = 0;                         //在外部先延时一段时间再打开以消除AUX通道配置带来的噪声
        mp3en_ctrl.channel = AUX_CHANNEL_SEL;           //LineIN 使用 AMUX通道
        mp3en_ctrl.agc_en = 0;                          //非录MIC
        mp3en_ctrl.rec_volume = 3;                      //录音增益]
        mp3en_ctrl.dac_out |= TWO_CHANAL;
        break;
   case LINE_BT:
        mp3en_ctrl.mp3_en = 0;
        mp3en_ctrl.vol_out = 1;                         //是否DAC播放AMUX的声音
        mp3en_ctrl.channel = AUX_CHANNEL_SEL;           //LineIN 使用 AMUX通道
        mp3en_ctrl.agc_en = 0;                          //非录MIC
        mp3en_ctrl.rec_volume = 3;                      //录音增益
        break;
    case AUX_EQ:
    case FM_EQ:
        if (AUX_EQ == record_source )
        {
            mp3en_ctrl.channel = AUX_CHANNEL_SEL;       //LineIN 使用 AMUX通道
        }
        else
        {
            mp3en_ctrl.channel = FM_CHANNEL_SEL;       //LineIN 使用 AMUX通道
        }
        mp3en_ctrl.vol_out = 0;                         //是否DAC播放AMUX的声音
        mp3en_ctrl.agc_en = 1;                          //开启AGC
        mp3en_ctrl.rec_volume = 3;                      //录音增益
        mp3en_ctrl.mp3_en = 0;                          //USB Audio时，不需要开启MP3压缩
        mp3en_ctrl.dac_out |= (AUX_EQ_EN | TWO_CHANAL);
        break;
    case RECORD_MIC:                //MIC 录音
    case RECORD_MIC_SPI:            //MIC 跟读
    case AUDIO_MIC:                 //USB Audio录音
    case MIC_LOUD_SPEAKER:
        //选择MIC录音

        mp3en_ctrl.vol_out = 0;                         //是否DAC播放AMUX的声音

        mp3en_ctrl.channel = MIC_CHANNEL_SEL;               //MIC 使用 AMUX通道 P02
        mp3en_ctrl.agc_en = 1;                          //开启AGC
        mp3en_ctrl.rec_volume = 10;                     //录音增益
        if (RECORD_MIC_SPI == record_source) {
            mp3en_ctrl.agc_en = 0;                      //录SPI时，关闭AGC以加快跟读速度
        } else if (AUDIO_MIC == record_source )
        {
             mp3en_ctrl.mp3_en = 0;
        }
          else if (MIC_LOUD_SPEAKER == record_source) {
            mp3en_ctrl.mp3_en = 0;                      //SPEAKER时，不需要开启MP3压缩
            mp3en_ctrl.dac_out |= SPEAKER_EN;
            mp3en_ctrl.rec_volume = 7;                     //录音增益
        }
        break;

    case BT_MIC:
        //蓝牙通话MIC配置
        mp3en_ctrl.vol_out = 0;                         //是否DAC播放AMUX的声音
        mp3en_ctrl.channel = MIC_CHANNEL_SEL;           //MIC 使用 AMUX通道 P02
        mp3en_ctrl.agc_en = 0;                          //开启AGC
        mp3en_ctrl.rec_volume = 10;                     //录音增益
        mp3en_ctrl.mp3_en = 0;
        break;

    case RECORD_FM:                 //FM 录音
    case RECORD_TAPE_SPI:           //磁带复读录音
        mp3en_ctrl.vol_out = 1;                         //是否DAC播放AMUX的声音
        mp3en_ctrl.channel = FM_CHANNEL_SEL;            //TAPE 和 FM共用通道
        mp3en_ctrl.agc_en = 0;                          //非录MIC
        mp3en_ctrl.rec_volume = 3;                      //录音增益
        break;
    case RECORD_KARA:
        mp3en_ctrl.vol_out = 0;                         //是否DAC播放AMUX的声音
        mp3en_ctrl.mp3_en = 0;                          //混响模式需要MP3压缩模块
        mp3en_ctrl.channel = MIC_CHANNEL_SEL;               //MIC 使用 AMUX通道 P02
        mp3en_ctrl.agc_en = 0;                          //不开启AGC
        mp3en_ctrl.rec_volume = 9;
        break;
    default:
        return false;
    }

    mp3en_ctrl.rec_channel = mp3en_ctrl.channel;        //录音通道与AMUX通道相同
    if (BT_MIC == record_source) {
        mp3en_ctrl.sample = SAMPLE_BT_MIC;                //设置BT_MIC的采样率，//BT_MIC修改为16K采样率
    } else if (AUDIO_MIC == record_source) {
        mp3en_ctrl.sample = SAMPLE_48000;               //设置USB Audio的采样率
    } else if (record_source & RECORD_SPI) {
        mp3en_ctrl.sample = SAMPLE_44100;               //录SPI的配置
        mp3en_ctrl.bitrate = SPI_BITRATE;
    } else if (record_source == MIC_LOUD_SPEAKER) {
        mp3en_ctrl.sample = SAMPLE_16000;               //设置SPEAKER MIC的采样率
        mp3en_ctrl.dac_sample = SAMPLE_16000;               //设置SPEAKER MIC DAC的采样率
    }
    else if (record_source == AUX_EQ || record_source == FM_EQ) {
        mp3en_ctrl.sample = SAMPLE_48000;               //设置SPEAKER MIC的采样率
        mp3en_ctrl.dac_sample = SAMPLE_48000;               //设置SPEAKER MIC DAC的采样率
        mp3en_ctrl.bitrate = 128;
    }
    else if (record_source == RECORD_KARA)
    {
        mp3en_ctrl.sample = SAMPLE_16000;               //采样率
      //  mp3en_ctrl.sample = encode_adc_sample;
        mp3en_ctrl.bitrate = 64;                        //位速
    }
    else {                //使用系统配置
        if (!mp3en_set_record_mode(sys_ctl.mode_record)) {
            return false;
        }
    }

    //初始化encoder模块
    mp3en_init(&mp3en_ctrl);
    return true;
}
