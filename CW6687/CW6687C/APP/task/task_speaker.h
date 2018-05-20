/*****************************************************************************
 * Module    : Task
 * File      : task_seaker.h
 * Author    : Hanny
 * Function  : ������������ض���
*****************************************************************************/
#ifndef TASK_SPEAKER_H
#define TASK_SPEAKER_H

typedef struct {
    u8  mix_step;            //���켶��
    u8  pos;
#if IS_LCD_DISPLAY
    u8  update_flag;         //��ʾ����
    u8  picture_update_flag; //��ͼƬ����
#endif
} type_task_speaker;

extern type_task_speaker t_speaker;

#define SPEAKER_SAMPLES         0x80

#define ECHO_MAX                14                      //�����켶��
#define ECHO_STEP               0x200                   //ÿ������ļ��
#define ECHO_BUF_LEN            0x1C00
#define ECHO_SAMPLES            SPEAKER_SAMPLES/2       //ÿ�μ����������
#define ECHO_BUF_SATRT          (u16)echo_buf
#define ECHO_BUF_END            ((u16)echo_buf+ECHO_BUF_LEN)

void mix_step_set(u8 step);
bool speaker_load_code(u16 addr);
void task_speaker_display(void);
void speaker_play_vol_music(void);
void speaker_continue(void);
__near_func void mix_proc(u16 samples);

#endif
