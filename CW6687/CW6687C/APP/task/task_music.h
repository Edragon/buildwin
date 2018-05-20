/*****************************************************************************
 * Module    : Task
 * File      : task_music.h
 * Author    : Hanny
 * Function  : ���ֲ�����������
 *****************************************************************************/
#ifndef TASK_MUSIC_H
#define TASK_MUSIC_H

#define PLAYMODE_ITEMS_NUM          (MODE_PLAY_NUM + 1)
#define MUSIC_EQ_ITEM_NUM           (MUSIC_EQ_NUM + 1)
#define DEVICE_ITEM_NUM             3

//��ʾ���±�־
void task_music_display(void);              //��������������ж���

enum {
    ITEM_LOOP_ALL,
    ITEM_LOOP_SINGLE,
    ITEM_LOOP_FOLDER,
    ITEM_LOOP_NORMAL,
    ITEM_LOOP_RAMDOM,
    ITEM_LOOP_BROWSE,
};

#define LRC_FILENAME_TIME                   (3 + 1)           //�и��ʱ�������ȱ���3�����

typedef struct {
    u8 play_sta;                            //������״̬����
    u16 music_num;                          //������Ŀ���
    u16 music_total;                        //��Ŀ����
    u16 dir_last;                           //Ŀ¼���һ���ļ����
    u8 music_type;                          //��Ƶ��ʽ
    u8 eq_num;                              //EQѡ��
    u8 speed;                               //�ٶ�ѡ��
    union {
        struct {
            u8 pause:                       1;     //��ͣ������״̬��ʾ
            u8 file_change:                 1;     //�ļ��ı䣬��Ҫ�����ļ�
            u8 file_prev_next:              1;     //�ļ�����0Ϊnext��1Ϊprev
            u8 no_file_flag:                1;     //�豸���������ļ� �������ļ�ʱ���豸
            u8 mode_flag:                   1;     //�豸���������ļ� �������ļ�ʱ���豸
            u8 music_voice_flag:            1;     //ģʽ���л��豸
            u8 break_flag:                  1;     //�ϵ����
        };
        u8 music_ctl;
    };
    type_play_time cur_time;                //��ǰ����ʱ��
    type_play_time disp_time;               //������ʾ��ʱ��
    type_play_time disp_music_time;         //������ʾ�ĸ���ʱ��
    u32 bit_rate;                           //����
    u8 disp_flag:                  1;  //��ʾȫ�����±�־
    u8 lrc_change:                 1;  //�Ƿ���Ҫ���¸��
    u8 filename_change:            1;  //������ʾ�ļ���
    u8 disp_status_flag:           1;  //�Ƿ���Ҫ����״̬��ʾ
#if AB_REPEAT_EN
    type_music_point_ab point;              //��¼AB��
    u16 disp_rtime;                         //��ʾ����ʱ��
    u16 rtime;                              //��ǰ����ʱ��
     u8 repeat_flag;                        //������־
     u8 repeat_show;                        //���Ƹ�����־��˸
    u8 auto_repeat;                         //�Զ�������־
    u8 auto_repeat_start;                   //��ʼ�Զ�����
    u32 next_sentence;                      //�����������һ�俪ʼ��ʱ��
#endif

#if MUSIC_LRC_EN
    u8 lrc_flag;                            //�Ƿ��и��
    u8 lrc_type;                           //lrc�ļ���������
#endif
#if BREAK_POINT_EN
     u8 break_dev;
    u16 filename_crc;                       //������crc
    type_music_point break_point;           //��¼�ϵ���Ϣ
#endif
    u16 dir_total;  //�ļ�������
    u16 dir_num;//��ǰ�ļ��б��
    //bool pause_before_menu_music;           //������ʾ��ǰ�����Ƿ�����ͣ״̬
} type_task_music_ctl;

extern type_task_music_ctl t_msc;


#endif
