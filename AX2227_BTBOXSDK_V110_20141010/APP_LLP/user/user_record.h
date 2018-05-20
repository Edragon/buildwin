/*****************************************************************************
 * Module    : User
 * File      : user_record.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户录音控制的定义
 *****************************************************************************/
#ifndef USER_RECORD_H
#define USER_RECORD_H

enum {
    ITEM_RECORD_LOW,
    ITEM_RECORD_MID,
    ITEM_RECORD_HIGH,
};

#define RECORD_SPI 0x80

enum {
    RECORD_LINEIN,
    RECORD_MIC,
    LINE_BT,
    RECORD_FM,
    AUDIO_MIC,
    BT_MIC,
    MIC_LOUD_SPEAKER,
    AUX_EQ,
    FM_EQ,
    RECORD_KARA,
    RECORD_TAPE_SPI = RECORD_SPI,
    RECORD_MIC_SPI,
};

#define RECORD_SYNC_CNT     5       //每5秒同步一次

#define MIC_FOLLOW_SIZE         0x800            //跟读的最大大小，不能超过总大小

bool mp3en_set_record_mode(u8 mode_record);
bool mp3en_source_init(u8 record_source);

#endif
