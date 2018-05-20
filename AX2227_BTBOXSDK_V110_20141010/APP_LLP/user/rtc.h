/*****************************************************************************
 * Module    : user
 * File      : user.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户文件
 *****************************************************************************/
#ifndef RTC_H
#define RTC_H


void irtcc_init(void);
__near_func void irtcc_read_rtc(u8 *rtc_buf);
__near_func u32 irtcc_read_sec(void);
void irtcc_write_alm(u8 hour,u8 minute,u8 second);

//u8 irtcc_read_cfg(void);
//void irtcc_write_cfg(u8 irtcc_cfg);
void irtcc_write_cfg(u8 cmd,u8 irtcc_cfg);
//void rtccmd_write_nbyte(u8 cmd, u8 *tx_buf , u8 len);
u8 irtcc_read_cfg(u8 cmd);
u32 irtcc_read_alm(u8 *rtc_buf);
//__root void irtcc_write(unsigned char x);
//__root unsigned char irtcc_read(void);
//void irtcc_write_rtc(u16 day,u8 hour,u8 minute,u8 second);

void irtcc_write_ram(void *buf, u8 addr, u8 len);
void irtcc_read_ram(void *buf, u8 addr, u8 len);
void irtcc_write_ram8(u8 addr, u8 val);
void irtcc_write_ram16(u8 addr, u16 val);
void irtcc_write_ram32(u8 addr, u32 val);
u8 irtcc_read_ram8(u8 addr);
u16 irtcc_read_ram16(u8 addr);
u32 irtcc_read_ram32(u8 addr);

#define NEXT_ALARM          0       //下一次闹钟时间，4byte
#define ALARM_SWITCH        4       //闹钟开关，1byte
#define LCD_CONTRAST        5       //屏的对比度，1byte
#define SLEEP_TIME          6       //休眠时间，4byte


//断点记忆
#define USB_BREAK_FRAME     10        //4BYTE
#define USB_BREAK_POS       14       //4BYTE
#define USB_NAME_CRC        18       //2BYTE

#define SD_BREAK_FRAME      20       //4BYTE
#define SD_BREAK_POS        24       //4BYTE
#define SD_NAME_CRC         28       //2BYTE

//闹铃音乐信息
#define ALM_MUSIC_TYPE      30       //1byte
#define ALM_FILE_NUM        31       //2byte
#define ALM_NAME_CRC        33       //2BYTE

#endif
