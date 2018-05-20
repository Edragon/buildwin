/*****************************************************************************
 * Module    : Task
 * File      : task_fm.h
 * Author    : Hanny
 * Function  : �����������õ��Ľṹ���
 *****************************************************************************/
#ifndef TASK_FM_H
#define TASK_FM_H

#if (REC_TYPE == REC_MP3)
    #define FMREC_SAMPLES       384
#elif (REC_TYPE == REC_WAV_ADPCM)
    #if ((FM_CHANNEL_SEL&0x80) == 0x80)
        #define FMREC_SAMPLES   121
    #else
        #define FMREC_SAMPLES   249
    #endif
#elif (REC_TYPE == REC_WAV_PCM)
    #define FMREC_SAMPLES       128
#endif

#if (REC_TYPE == REC_MP3)
    #define FREC_BITR           160
#else
    #define FREC_BITR           0
#endif

typedef struct _TYPE_TASK_FM {
    u8 mute;
    u8 fm_sta;                          //��״̬
    u8 disp_flag;                       //״̬��ʾ���¿���
    u8 disp_ch_flag;                    //Ƶ����ʾ����
    u8 disp_ch_cnt;                     //��ǰ��ʾƵ������
    u8 disp_ch_sel;                     //��ǰ��ʾƵ��ѡ��
    u16 disp_freq;                      //Ƶ����ʾ���¿���

    u8 eq_num;                          //EQ
    u16 freq;                           //��ǰƵ��
    u8 ch_cnt;                          //Ƶ������
    u8 ch_sel;                          //��ǰƵ��
    u8 auto_delay;                      //�Զ���̨ʱ���м䲥��ʱ��

    char *num_buf;                      //����¼��ʱ��ʾ¼��ʱ��
    u8 num_len;
    char *colon;
    char *rec_icon;
    char *bignum_buf;
    u8 bignum_len;
    char *bat_buf;
    u8 bat_len;
    char *point_buf;
    char seek_sta;
    u8 clk_set;
    u8 sta;
} TYPE_TASK_FM;

typedef struct {
    u8  dir_open;                       //�ļ����Ѵ�����־
    u8  file_open;                      //�ļ��򿪱�־
    u16 spl;                            //ÿ��BLK��������
    u8  volatile pos;                   //¼��buf��־
    u16 adc_sample_cnt;
    u8 	play_flag;                      //�Ƿ񲥷Ÿ�¼�µ��ļ�
    u32 rec_time;                       //¼��ʱ��
    u8  sync_cnt;                      	//ͬ������

    u8  volatile block_cnt;             //¼����BLK��
    u8  slice;                          //ÿ��BLK��SLC��
    u32 file_size;                      //¼���ļ���С
    u8 *block_adr;                      //¼��buf�ĵ�ǰ��ַ
    u32 file_ssect;                     //¼���ļ���һ������
    u16 spr;                            //¼���Ĳ�����
#if IS_LCD_DISPLAY
    u8  updata_flag;                    //���±�־
    u8  disp_sta;                       //������˸״̬
#endif
} type_task_fm_rec;

enum {
    FM_PLAY,                            //��������
    FM_AUTO_SEEK,                       //�Զ���̨ģʽ
    FM_AUTO_PLAY,                       //�Զ���̨ģʽ���̵Ķ�ʱ����
    FM_RECORD,                          //¼��ģʽ
    FM_REC_PAUSE,                       //¼����ͣ
    FM_REC_PLAY,                        //¼������
};

extern TYPE_TASK_FM t_fm;
extern type_task_fm_rec t_fm_rec;

void task_fm_disp_init(void);
void task_fm_list(void);
void task_fm_volume_set(void);
void task_fm_load(void);

void tasksub_fm_record(void);
void task_fm_record_stop(void);
bool fm_sync_file(void);
void tasksub_fm_recplay(void);
void task_fm_reuse_exit(void);
void task_fm_record_pause(void);
void task_fm_record_sync(void);

#endif
