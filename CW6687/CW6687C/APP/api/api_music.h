/*****************************************************************************
 * Module    : API
 * File      : api_music.h
 * Author    : Hanny
 * Function  : ���ֲ���API�ӿ�
 *****************************************************************************/
#ifndef API_MUSIC_H
#define API_MUSIC_H

typedef struct {
    u32 frame;
    u32 pos;
} type_music_point;

typedef struct {
    type_music_point pa;
    type_music_point pb;
} type_music_point_ab;

typedef struct {
    u8 min;
    u8 sec;
} type_play_time;

enum {
    MUSIC_FAST_FORWORD,
    MUSIC_FAST_BACKWORD,
};

enum {
    TYPE_NONE,             //��֧�ֵĸ�ʽ
    TYPE_MP3,
    TYPE_WMA,
    TYPE_WAV,
};

typedef struct {
    u8 type;                                //���ָ�ʽ
    u8 status;
    u32 cur_mtime;                          //��ǰ����ʱ�䣬��λΪ0.1s
    u8 repeat;                              //�ظ�����
    u8 data_start_set;                      //��ʼλ������
    bool (__near_func *read)(void*);        //�����Ķ��ӿ�
    type_music_point_ab point_ab;           //AB��
    u16 all_time;

    u8 err_cnt;                             //�����������
} type_msc_ctl;

extern type_msc_ctl msc_ctl;

/*****************************************************************************
 * Module    : ���ֲ������
 *****************************************************************************/

#define REPEAT_LOOP 0xff        //����ѭ������
#define REPEAT_END  0xfe        //����һ�κ�ֹͣ�����ڸ���

__near_func bool decoder_read(void *buf);               //�������Ķ��ӿ�
__near_func bool decoder_seek(u8 mode, u32 offset);     //��������SEEK�ӿ�

#define f_read_opt decoder_read
#define f_seek_opt decoder_seek

u8 music_init(void);                                    //��ʼ�����ֽ���

bool mp3_init(void);                                    //��ʼ��MP3����
bool mp3_init_spi(void);                                //��ʼ��MP3�����SPIFlash�����ư�
//bool mp3_init_alarm(void);                              //��ʼ������

void music_dac_exit(void);

u32 music_get_bitrate(void);                          //��ȡ��������

void music_clear(void);                                 //��������RAM
u16 music_total(void);                                  //��ȡ�ܹ��ĸ�����Ŀ
bool music_init_tmpfile(void);                          //��ʼ��������tempfile
void music_stop(void);                                  //ֹͣ���ֲ���
void music_event(void);                                 //������Ҫ���¼�
u16 music_get_alltime(void);                            //��ȡ��ʱ��
void music_play(void);                                  //��ʼ����
void music_pause(void);                                 //��ͣ
bool music_start_pause(void);                           //����/��ͣ
void music_fast(u8 direct, u8 sec);                     //���/����
void music_fast_end(void);                              //���/���˽������ָ�����
void music_get_point(type_music_point *point);          //��ȡ��ǰ����λ��
void music_set_a_point(type_music_point *point);        //����A�㣬�Ա㸴��ʱ�����
//AB������repeatΪ������������repeat = REPEAT_LOOP�����޸�����ΪREPEAT_ENDʱ������һ��ֹͣ
void music_repeat(u8 repeat, type_music_point_ab *pab);
void music_repeat_clear(void);                          //�����ǰ�ĸ���״̬��������������
void music_jump(type_music_point *point);               //����ĳ����в���
bool music_set_speed(u8 speed);                         //���ò����ٶ�
__near_func void dac_set_volume(u8 volume);             //���ò�������
u8 music_get_repeat(void);                              //��ȡʣ�ิ������
u32 music_lrc_first_time(void);                         //��ȡ��һ��LRC��ʱ��
u32 music_lrc_cur_time(void);                           //��ȡ����LRC��ʱ��
u32 music_lrc_next_time(void);                          //��ȡ��һ��LRC��ʱ��
__near_func bool music_repeat_hook2(void);              //�����ӿڣ����ڽ��������

__near_func u16 voice_maxpow(void *buf, u16 len);       //��ȡһ���������������������ֵ��
__near_func u16 voice_maxpow_step(void *ptr, u16 len, u8 step);

//EQѡ��
enum{
    EQ_NORMAL,                  //��������EQ
    EQ_POP,                     //����
    EQ_ROCK,                    //ҡ��
    EQ_JAZZ,                    //��ʿ
    EQ_CLASSIC,                 //�ŵ�
    EQ_COUNTRY,                 //���
};
void music_eq(u8 eqnum);        //MP3����EQ�Ľӿ�

void eq_disable(void);          //�ر�EQ

//����״̬
enum {
    STATUS_PLAY_STOP,                       //��ȫֹͣ
    STATUS_PLAY_STOPPING,                   //����ֹͣ
    STATUS_PLAY_PAUSE,                      //��ͣ��
    STATUS_PLAY_PLAYING,                    //���ڲ���
    STATUS_PLAY_FAST,                       //���������
    STATUS_PLAY_CONTINUE,                   //�ȴ��ٴ�KICK��������
};

extern struct
{
    u32 addr;//��Դ�׵�ַ
    u16 len;//��Դ��С
    u16 pos;
    u8 spr;//������
    u8 bits;//��������
}mp3res;

__near_func music_get_status(void);     //��ȡ���ֵĲ���״̬
__near_func u32 music_calc_mtime(u32 frame_count);
__near_func u32 music_get_mtime(void);
__near_func u32 music_point_a_mtime(void);
void mp3_res_play(u8 music_name);                             //ѡ�����Բ˵�����
void mp3_res_play_wait(u8 music_name);                       //ѡ�����Բ˵�����,���ȴ��������
void mp3_res_stop(void);                      //��������ֹͣ����
void mp3_res_loop_set(bool loop_flag);//Ӧ�ڵ�����mp3_res_play���ٵ��ã�����ѭ�����ţ�1:����ѭ�����ţ�ֱ������mp3_res_stop(); 0:����������һ��ֹͣ
void wav_res_play(u8 music_name);                             //ѡ�����Բ˵�����
void usb_audio_sound_play(u8 music_name);
__near_func bool spi_mp3res_read_256(void *buf);
__near_func bool spi_mp3res_read(void *buf);

__near_func u8 sound_detect(u16 v_pow);
__near_func void sound_ctl_init(u8 is_cnt, u16 is_pow, u8 silence_cnt, u16 silence_pow);
__near_func u8 sound_flag(void);
__near_func void sound_set_flag(u8 flag, u8 cnt);

__near_func bool wma_init_sdk(u8 __xdata *const __code *buffer_tbl, u8 total_num);
__near_func bool m_wma_init(u8 __xdata *const __code *buffer_tbl, u8 total_num);
bool wav_get_info(void);
bool mp3_get_ID3(void);

/*****************************************************************************
 * Module    : �����ʾ���
 *****************************************************************************/
extern __root __no_init char filename[100];
u8 lrc_init(void);
u8 music_get_lrc(u32 time_display);
#endif
