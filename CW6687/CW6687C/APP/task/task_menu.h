/*****************************************************************************
 * Module    : Task
 * File      : task_menu.h
 * Author    : Hanny
 * Function  : ���˵������õ��Ľṹ���
 *****************************************************************************/
#ifndef TASK_MENU_H
#define TASK_MENU_H

/*****************************************************************************
 * LCD����Ĭ���������
 *****************************************************************************/
#ifndef LCD_THEME_DEFAULT_H
#define LCD_THEME_DEFAULT_H

#define RES_MAIN_MUSIC                  RES_MAIN_MUSIC0_BMP
#define RES_MAIN_TAPE                   RES_MAIN_TAPE0_BMP
#define RES_MAIN_AUX                    RES_MAIN_AUX0_BMP
#define RES_MAIN_MIC                    RES_MAIN_MIC0_BMP
#define RES_MAIN_VOICE                  RES_MAIN_VOICE0_BMP
#define RES_MAIN_RADIO                  RES_MAIN_RADIO0_BMP
#define RES_MAIN_RTC                    RES_MAIN_RTC0_BMP
#define RES_MAIN_USB                    RES_MAIN_USB0_BMP
#define RES_MAIN_EBOOK                  RES_MAIN_EBOOK0_BMP
#define RES_MAIN_SYSTEM                 RES_MAIN_SYSTEM0_BMP
#define RES_MAIN_SPEARKER               RES_MAIN_SPEARKER0_BMP
#define RES_MAIN_BT                     RES_MAIN_BT0_BMP
#define RES_MAIN_TOMCAT                 RES_MAIN_TOMCAT0_BMP
#endif

#define MENU_ROLL           1    //1Ϊʹ�ù����˵���0Ϊʹ�÷�ҳ�˵�

#define NUM_PAGE            4    //ÿҳ��ʾ4���˵�
#define MENU_NUM ((sizeof(menu_item) / sizeof(menu_item[0])))
typedef struct {
    u8 sta;                             //�˵�����
    u8 r_index;                         //��Դ����
    u16 name;                   //�˵���
#if MENU_MUSIC
    u8 mp3res;                    //��������
#endif
} type_menu_item;

//task menu controller
typedef struct {
    u8 index;                           //��ǰ�˵�
    u8 page_index;                      //��ǰ�˵�����ҳ�ĵ�һ��
    u8 total;                           //�˵�������
    u8 icon_index;                      //�˵������л�
    u8 icon_cnt;                        //�л�Ƶ��
#if TASK_SHOWTIME_EN
    u8 back_sec;                        //�޲�������show time
#endif
    u8 disp_index;                      //������ʾ�Ĳ˵���
    u8 disp_flag;                       //ˢ����ʾ
} type_task_menu_ctl;

extern type_task_menu_ctl t_menu;
extern IAR_CONST type_menu_item menu_item[];

void task_menu_display(void);           //���������Ҫ�ڲ�ͬ�����ж���

#endif
