/*****************************************************************************
 * Module    : Task
 * File      : task_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : ���ֲ�����������
 *****************************************************************************/
#ifndef TASK_MUSIC_H
#define TASK_MUSIC_H

enum {
    ITEM_LOOP_ALL,
    ITEM_LOOP_SINGLE,
    ITEM_LOOP_FOLDER,
    ITEM_LOOP_RAMDOM,
    //ITEM_LOOP_NORMAL,
    //ITEM_LOOP_BROWSE,
};


#define LRC_FILENAME_TIME                   (3 + 1)           //�и��ʱ�������ȱ���3�����

typedef struct {
    u8 pause;                               //��ͣ����Ҫ����״̬��ʾ
    u8 music_voice_flag;                    //�����ǲ���voice���ǲ���music, 1=����voice, 0=����music
    type_play_time cur_time;                //��ǰ����ʱ��
} type_user_music_ctl;

typedef struct {
    u8 play_sta;                            //������״̬����
    u8 file_change;                         //�ļ��ı䣬��Ҫ�����ļ�
    u8 file_prev_next;                      //�ļ�����0Ϊnext��1Ϊprev
    u16 music_num;                          //������Ŀ���
    u16 music_total;                        //��Ŀ����
    u16 dir_last;                           //Ŀ¼���һ���ļ����
    u8 music_type;                          //��Ƶ��ʽ
    u8 speed;                               //�ٶ�ѡ��
#if AB_REPEAT_EN
    u8 disp_status_flag;                    //�Ƿ���Ҫ����״̬��ʾ
    type_music_point_ab point;              //��¼AB��
    u16 disp_rtime;                         //��ʾ����ʱ��
    u16 rtime;                              //��ǰ����ʱ��
     u8 repeat_flag;                        //������־
     u8 repeat_show;                        //���Ƹ�����־��˸
    u8 auto_repeat;                         //�Զ�������־
    u8 auto_repeat_start;                   //��ʼ�Զ�����
    u32 next_sentence;                      //�����������һ�俪ʼ��ʱ��
#endif
    union {
        struct {
            u8 disp_flag:                  1;  //��ʾȫ�����±�־
            u8 lrc_change:                 1;  //�Ƿ���Ҫ���¸��
            u8 filename_change:            1;  //������ʾ�ļ���
            u8 disp_status_flag:           1;  //�Ƿ���Ҫ����״̬��ʾ
        };
    };
#if TASK_KARAOKE_EN
    u8 karaoke_flag;                        //����okģʽ��־
#endif
#if K_MODE_CHANGE_DEVICE
     u8 mode_flag;                          //ģʽ���л��豸
#endif
#if BREAK_POINT_EN
     u8 break_dev;
     u8 break_flag;                         //�ϵ����
    u16 filename_crc;                       //������crc
    type_music_point break_point;           //��¼�ϵ���Ϣ
#endif
#if MUSIC_LRC_EN
    u8 lrc_flag;                            //�Ƿ��и��
    u8 lrc_type;                               //lrc�ļ���������
#endif
#if LCD_THEME
    u32 bit_rate;
    type_play_time disp_time;               //������ʾ��ʱ��
    type_play_time disp_music_time;         //������ʾ�ĸ�����ʱ��
#endif
} type_task_music_ctl;

extern type_user_music_ctl u_msc;           //���ܸ���
extern type_task_music_ctl t_msc;           //������ģʽ����


void task_music_display(void);              //��������������ж���


#endif
