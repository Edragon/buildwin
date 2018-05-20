/*****************************************************************************
 * Module    : Task
 * File      : task_tomcat.h
 * Author    : Hanny
 * Function  : ��ķè�����õ��Ľṹ���
 *****************************************************************************/
#ifndef TASK_TOMCAT_H
#define TASK_TOMCAT_H

#define TOMCAT_SAMPLES      0x80
#define TOM_BUF_SIZE        0x1800
#define TOM_BLOCK_SIZE      128L    //BLK�Ĵ�С

enum {
    TOM_NULL,
    TOM_REC,
    TOM_PLAY,
};

typedef struct {
    u8  pos;                        //¼��buf��־
    u8  volatile block_cnt;         //¼����BLK��
    u8 *block_adr;                  //¼��buf�ĵ�ǰ��ַ
    u8  block_lmt;                  //¼��buf��BLK��
    u8  sta;                        //��ķè״̬
    u16 rec_addr;                   //¼����ʼ��ַ
    u16 rec_len;                    //¼������
    u8  auto_play;                  //�Զ����ű�־
#if IS_LCD_DISPLAY
    u8 disp_flag;                   //��ʾ���±�־
    u8 picture_disp_flag;           //��ͼƬ��ʾ����
    u8 last_sta;                    //��һ��״̬
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
