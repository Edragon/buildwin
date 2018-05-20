/*****************************************************************************
 * Module    : Task
 * File      : task_seaker.h
 * Author    : Hanny
 * Function  : 扩音器任务相关定义
*****************************************************************************/
#ifndef TASK_SPEAKER_H
#define TASK_SPEAKER_H

typedef struct {
    u8  mix_step;            //混响级数
    u8  pos;
#if IS_LCD_DISPLAY
    u8  update_flag;         //显示更新
    u8  picture_update_flag; //大图片更新
#endif
} type_task_speaker;

extern type_task_speaker t_speaker;

#define SPEAKER_SAMPLES         0x80

#define ECHO_MAX                14                      //最大混响级数
#define ECHO_STEP               0x200                   //每级混响的间隔
#define ECHO_BUF_LEN            0x1C00
#define ECHO_SAMPLES            SPEAKER_SAMPLES/2       //每次计算的样点数
#define ECHO_BUF_SATRT          (u16)echo_buf
#define ECHO_BUF_END            ((u16)echo_buf+ECHO_BUF_LEN)

void mix_step_set(u8 step);
bool speaker_load_code(u16 addr);
void task_speaker_display(void);
void speaker_play_vol_music(void);
void speaker_continue(void);
__near_func void mix_proc(u16 samples);

#endif
