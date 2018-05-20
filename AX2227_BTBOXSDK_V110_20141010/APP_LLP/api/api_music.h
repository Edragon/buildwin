/*****************************************************************************
 * Module    : API
 * File      : api_music.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
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

/*****************************************************************************
 * Module    : ���ֲ������
 *****************************************************************************/

#define REPEAT_LOOP 0xff        //����ѭ������
#define REPEAT_END  0xfe        //����һ�κ�ֹͣ�����ڸ���

__near_func u32 voice_power(void *buf, u16 len);        //��ȡһ��������ƽ������ֵ
__near_func u16 voice_maxpow(void *buf, u16 len);       //��ȡһ���������������

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
u8 music_get_repeat(void);                               //��ȡʣ�ิ������
bool music_decode_ID3(void);                             //��ȡ������ID3��Ϣ

//Ƶ���߳�ʼ��ѡ��
enum {
    FREQ_DAC,                   //����DAC��Ƶ����
    FREQ_ADC,                   //����ADC��Ƶ����
};
void music_freq_init(u8 freq_type);                     //��ʼ��Ƶ����, freq_type�ο�Ƶ���߳�ʼ��ѡ��ö��
bool music_freq_get(void);                              //��ȡƵ����
void music_freq_kick(void);                             //Kick��һ��
extern u16 freq_buf[64];
__near_func u16 voice_maxpow_step(void *ptr, u16 len, u8 step);

//EQѡ��
enum{
    EQ_NORMAL,                  //��������EQ
    EQ_POP,                     //����
    EQ_ROCK,                    //ҡ��
    EQ_JAZZ,                    //��ʿ
    EQ_CLASSIC,                 //�ŵ�
    EQ_COUNTRY,                 //���
    EQ_BASS,                    //�ص���
};
void music_eq(u8 eqnum);                                //����EQ
void aux_fm_eq(u8 eqnum);                               //����Linein��FM EQ

//����״̬
enum {
    STATUS_PLAY_STOP,                       //��ȫֹͣ
    STATUS_PLAY_STOPPING,                   //����ֹͣ
    STATUS_PLAY_PLAYING,                    //���ڲ���
    STATUS_PLAY_PAUSE,                      //��ͣ��
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
u32 music_get_mtime(void);
u32 music_point_a_mtime(void);

void spi_music_wait(void);                             //�������ŵȴ�����
void mp3_res_play(u8 music_name);                             //ѡ�����Բ˵�����
void mp3_res_play_wait(u8 music_name);                       //ѡ�����Բ˵�����,���ȴ��������
void wav_res_play(u8 music_name);                             //ѡ�����Բ˵�����

void usb_audio_sound_play(u8 music_name);
__near_func bool spi_mp3res_read(void *buf);

/*****************************************************************************
 * Module    : ¼�����
 *****************************************************************************/
//AMUX
#define CHANNEL_0           0x10            //��������ʹ��P00 L
#define CHANNEL_1           0x01            //��������ʹ��P01 R
#define CHANNEL_2           0x02            //��������ʹ��P02 R
#define CHANNEL_3           0x20            //��������ʹ��P03 L
#define CHANNEL_6           0x03            //��������ʹ��P36 R
#define CHANNEL_7           0x30            //��������ʹ��P37 L

//MIC ֻ֧�� P02 �� P03
#define MCHANNEL            0x08
#define MCHANNEL_2          (MCHANNEL | CHANNEL_2)       //��������ʹ��P02 R
#define MCHANNEL_3          (MCHANNEL | CHANNEL_3)       //��������ʹ��P03 L

//AMUX������˫���
#define CHANNEL_DL          0x70
#define CHANNEL_DR          0x07
#define CHANNEL_00          (CHANNEL_0 | CHANNEL_DR)        //������˫�����ʹ��P00 L
#define CHANNEL_11          (CHANNEL_DL | CHANNEL_1)        //������˫�����ʹ��P01 R
#define CHANNEL_22          (CHANNEL_DL | CHANNEL_2)        //������˫�����ʹ��P02 R
#define CHANNEL_33          (CHANNEL_3 | CHANNEL_DR)        //������˫�����ʹ��P03 L
#define CHANNEL_66          (CHANNEL_DL | CHANNEL_6)        //������˫�����ʹ��P36 R
#define CHANNEL_77          (CHANNEL_7 | CHANNEL_DR)        //������˫�����ʹ��P37 L

//˫������AMUX����һ��һ���������
#define TWO_CHANNEL         0x80
#define CHANNEL_01          (TWO_CHANNEL | CHANNEL_0 | CHANNEL_1)     //˫������ʹ��P00 P01
#define CHANNEL_02          (TWO_CHANNEL | CHANNEL_0 | CHANNEL_2)     //˫������ʹ��P00 P02
#define CHANNEL_06          (TWO_CHANNEL | CHANNEL_0 | CHANNEL_6)     //˫������ʹ��P00 P36
#define CHANNEL_31          (TWO_CHANNEL | CHANNEL_3 | CHANNEL_1)     //˫������ʹ��P03 P01
#define CHANNEL_32          (TWO_CHANNEL | CHANNEL_3 | CHANNEL_2)     //˫������ʹ��P03 P02
#define CHANNEL_36          (TWO_CHANNEL | CHANNEL_3 | CHANNEL_6)     //˫������ʹ��P03 P36
#define CHANNEL_71          (TWO_CHANNEL | CHANNEL_7 | CHANNEL_1)     //˫������ʹ��P37 P01
#define CHANNEL_72          (TWO_CHANNEL | CHANNEL_7 | CHANNEL_2)     //˫������ʹ��P37 P02
#define CHANNEL_76          (TWO_CHANNEL | CHANNEL_7 | CHANNEL_6)     //˫������ʹ��P37 P36
#define CHANNEL_10          CHANNEL_01
#define CHANNEL_20          CHANNEL_02
#define CHANNEL_60          CHANNEL_06
#define CHANNEL_13          CHANNEL_31
#define CHANNEL_23          CHANNEL_32
#define CHANNEL_63          CHANNEL_36
#define CHANNEL_17          CHANNEL_71
#define CHANNEL_27          CHANNEL_72
#define CHANNEL_67          CHANNEL_76

enum {
    SAMPLE_48000,
    SAMPLE_44100,
    SAMPLE_32000,
    SAMPLE_24000,
    SAMPLE_22050,
    SAMPLE_16000,
    SAMPLE_12000,
    SAMPLE_11025,
    SAMPLE_8000,
    SAMPLE_BT_MIC,
};
#define SPEAKER_EN          0x01
#define AUX_EQ_EN           0x02
#define TWO_CHANAL          0x04

typedef struct {
    u8 dac_volume;
} type_volume_ctl;

typedef struct {
    //AMUX������
    u8 mp3_en;                          //�Ƿ���ҪMP3ѹ��ģ��
    u8 vol_out;                         //DAC�Ƿ񲥳�����
    u8 channel;                         //AMUX��ͨ��
    //ADC��¼��������
    u8 agc_en;                          //�Ƿ���AGC
    u8 rec_volume;                      //¼������AGCCON2,-12~72,2ʱΪ0
    u8 rec_channel;                     //ѡ�õ�¼��ͨ��
    u8 sample;                          //0Ϊ�Ͳ����ʣ�1Ϊ�߲�����
    u16 bitrate;                        //����
    u8 dac_out;                        //ֱ�ӰѲɵ��������Ƴ�DAC����
    u8 dac_sample;                     //dac �����ʣ�Ҫ��adc��������һ��
} type_mp3en_ctrl;

extern type_volume_ctl volume_ctl;      //����������
extern type_mp3en_ctrl mp3en_ctrl;      //¼��������

void mp3en_init(type_mp3en_ctrl *mp3en_ctrl);   //��λ��ʼ������ģ��
void mp3en_start(bool mode);                    //��ʼ¼����mode: true�Զ�ģʽ��false�ֶ���չģʽ
void mp3en_stop(void);                          //ֹͣ¼��
void mp3en_exit(void);                          //�˳�¼�����ܣ��ر�����¼��ģ��
u8 mp3en_write(void);                         //��Buffer������д�룬������ѭ���е���
bool mp3en_write_spi(void);
__near_func void imdct_neg(void);            //��������
__near_func bool wma_init_sdk(u8 __xdata *const __code *buffer_tbl, u8 total_num);
__near_func bool m_wma_init(u8 __xdata *const __code *buffer_tbl, u8 total_num);
bool mp3_get_ID3(void);
bool wav_get_info(void);


__near_func void at_write(u8 adr, u8 dat);
__near_func u8 at_read(u8 adr);
void adc_enable(void);


extern u8 mp3_adc_buf[0xd80];
extern __root __no_init char filename[100];

u8 lrc_init(void);
u8 music_get_lrc(u32 time_display);
u32 music_lrc_first_time(void);                         //��ȡ��һ��LRC��ʱ��
u32 music_lrc_cur_time(void);                           //��ȡ����LRC��ʱ��
u32 music_lrc_next_time(void);                          //��ȡ��һ��LRC��ʱ��
#endif
