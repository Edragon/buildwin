/*****************************************************************************
 * Module    : User
 * File      : user_record.c
 * Author    : Hanny
 * Function  : 用户录音定义
 *****************************************************************************/
#include "include.h"
#include "user_spi.h"
#include "task_speaker.h"
#include "task_usbdev.h"
#include "task_record.h"
#include "task_aux_eq.h"
#include "task_tomcat.h"
#include "task_fm.h"

//mic模拟增益的取值范围：（-1 ~ 10）x 6db = -6db ~ 60db
#pragma constseg="SDADC_SEG_CONST"
IAR_CONST type_sdadc_ctl tbl_sdadc_set[] = {
    //采样率,        样点数,         mp3压缩码率, mic模拟增益,  数字增益,     adc通道,         处理函数
    {SDADC_SPR_48K,  32,                0,         5,            0x60,        MIC_CHANNEL_SEL,  0},                   //MIC测试用，请勿修改
     //蓝牙mic的模拟增益和数字增益使用MPTool或者config_setting.h中的配置，此处请勿修改
    {SDADC_SPR_BT,   BT_VIOCE_SAMPLES,  0,         0,            0x00,        MIC_CHANNEL_SEL,  bt_mic_process},      //BT MIC
    {SDADC_SPR_8K,   SPEAKER_SAMPLES,   0,         5,            0x60,        MIC_CHANNEL_SEL,  task_speaker_process},//SPEAKER
    {SDADC_SPR_48K,  USBMIC_SAMPLES,    0,         5,            0x60,        MIC_CHANNEL_SEL,  task_usbmic_process}, //USB MIC
    {SDADC_SPR_MREC, MICREC_SAMPLES,  MREC_BITR,   5,            0x60,        MIC_CHANNEL_SEL,  task_record_process}, //RECORD MIC
    {SDADC_SPR_LREC, AUXREC_SAMPLES,  LREC_BITR,   0,            0x60,        AUX_CHANNEL_SEL,  task_record_process}, //RECORD AUX
    {SDADC_SPR_48K,  AUX_EQ_SAMPLES,    0,         0,            0x60,        AUX_CHANNEL_SEL,  task_aux_eq_process}, //AUX EQ
    {SDADC_SPR_16K,  TOMCAT_SAMPLES,    0,         5,            0x60,        MIC_CHANNEL_SEL,  task_tomcat_process}, //TOMCAT
    {SDADC_SPR_FREC, FMREC_SAMPLES,   FREC_BITR,   0,            0x60,        FM_CHANNEL_SEL,   task_fmrec_process},  //RECORD FM
};
#pragma constseg=default




