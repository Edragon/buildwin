/*****************************************************************************
 * Module    : Task
 * File      : task_tomcat.h
 * Author    : Hanny
 * Function  : 汤姆猫任务用到的结构体等
 *****************************************************************************/
#ifndef TASK_TOMCAT_H
#define TASK_TOMCAT_H

#define TOMCAT_SAMPLES      0x80
#define TOM_BUF_SIZE        0x1800
#define TOM_BLOCK_SIZE      128L    //BLK的大小

enum {
    TOM_NULL,
    TOM_REC,
    TOM_PLAY,
};

typedef struct {
    u8  pos;                        //录音buf标志
    u8  volatile block_cnt;         //录音的BLK数
    u8 *block_adr;                  //录音buf的当前地址
    u8  block_lmt;                  //录音buf的BLK数
    u8  sta;                        //汤姆猫状态
    u16 rec_addr;                   //录音起始地址
    u16 rec_len;                    //录音长度
    u8  auto_play;                  //自动播放标志
#if IS_LCD_DISPLAY
    u8 disp_flag;                   //显示更新标志
    u8 picture_disp_flag;           //大图片显示更新
    u8 last_sta;                    //上一个状态
#endif
} type_task_tomcat;

extern type_task_tomcat t_tom;
extern volatile u16 cr_speed;

bool tomcat_load_code(u16 addr);
void tomcat_record_init(void);
__near_func void task_tomcat_process(void);

void tempo_init(void);
__near_func void tempo_calc(u8 *dst_buf, u8 *src_buf);

#endif
