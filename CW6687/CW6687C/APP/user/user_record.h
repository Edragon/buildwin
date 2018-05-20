/*****************************************************************************
 * Module    : User
 * File      : user_record.c
 * Author    : Hanny
 * Function  : 用户录音控制的定义
 *****************************************************************************/
#ifndef USER_RECORD_H
#define USER_RECORD_H

u32 get_sclust_addr(void);
__near_func void dummy_void(void);

#if TASK_BT_EN
    __near_func void bt_mic_process(void);
    __near_func void bt_mic_8k_process(void);
    __near_func void bt_mic_16k_process(void);
    #if BT_SAMPLERATE_16K
        #define bt_mic_process      bt_mic_16k_process
    #else
        #define bt_mic_process      bt_mic_8k_process
    #endif
#else
    #define bt_mic_process          dummy_void
#endif

#if TASK_SPEAKER_EN
    __near_func void task_speaker_process(void);
#else
    #define task_speaker_process    dummy_void
#endif

#if TASK_USBDEV_EN
    __near_func void task_usbmic_process(void);
#else
    #define task_usbmic_process     dummy_void
#endif

#if (TASK_MIC_EN || TASK_AUX_EN)
    __near_func void task_record_process(void);
#else
    #define task_record_process     dummy_void
#endif

#if TASK_AUX_EQ_EN
    __near_func void task_aux_eq_process(void);
#else
    #define task_aux_eq_process     dummy_void
#endif

#if TASK_TOMCAT_EN
    __near_func void task_tomcat_process(void);
#else
    #define task_tomcat_process     dummy_void
#endif

#if (TASK_FM_EN && FM_RECORD_EN)
    __near_func void task_fmrec_process(void);
#else
    #define task_fmrec_process      dummy_void
#endif

//必须与 tbl_sdadc_set[] 表顺序一致
enum {
    SEL_TESTMODE = 0,       //0保留为测试模式，请勿修改
    SEL_BT_MIC,
    SEL_SPEAKER,
    SEL_USBMIC,
    SEL_RECMIC,
    SEL_RECAUX,
    SEL_AUX_EQ,
	SEL_TOMCAT,
    SEL_RECFM,
};

//sdadc output sample rate
enum {
    SDADC_SPR_48K = 0,      //pass band 18k~24k
    SDADC_SPR_32K,          //pass band 12k~16k
    SDADC_SPR_24K,          //pass band 9k~12k
    SDADC_SPR_16K,          //pass band 6k~8k
    SDADC_SPR_12K,          //pass band 4.5k~6k
    SDADC_SPR_8K,           //pass band 3k~4k
    SDADC_SPR_16K_2,        //pass band 3k~4k
};

#if BT_SAMPLERATE_16K
    #define SDADC_SPR_BT    SDADC_SPR_16K_2
#else
    #define SDADC_SPR_BT    SDADC_SPR_8K
#endif

#define SDADC_SPR_MREC      SDADC_SPR_16K
#define SDADC_SPR_LREC      SDADC_SPR_16K
#define SDADC_SPR_FREC      SDADC_SPR_16K

typedef struct {
    u8  sample_rate;    //采样率
    u16 samples;        //采样点数
    u8  bit_rate;       //mp3压缩码率
    s8  mic_ana_gain;   //mic的模拟增益
    u8  digit_gain;     //数字增益(mic和aux复用)
    u8  adc_ch;         //adc通道
    void (__near_func *agc_process)(void);
} type_sdadc_ctl;

extern u8 adc_buf[0x600];
extern u8 adc_ch;

#endif
