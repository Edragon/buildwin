/*****************************************************************************
 * Module    : Task
 * File      : task_menu.h
 * Author    : Hanny
 * Function  : 主菜单任务用到的结构体等
 *****************************************************************************/
#ifndef TASK_MENU_H
#define TASK_MENU_H

/*****************************************************************************
 * LCD屏的默认主题参数
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

#define MENU_ROLL           1    //1为使用滚动菜单，0为使用翻页菜单

#define NUM_PAGE            4    //每页显示4个菜单
#define MENU_NUM ((sizeof(menu_item) / sizeof(menu_item[0])))
typedef struct {
    u8 sta;                             //菜单任务
    u8 r_index;                         //资源索引
    u16 name;                   //菜单名
#if MENU_MUSIC
    u8 mp3res;                    //语音播报
#endif
} type_menu_item;

//task menu controller
typedef struct {
    u8 index;                           //当前菜单
    u8 page_index;                      //当前菜单所在页的第一项
    u8 total;                           //菜单项总数
    u8 icon_index;                      //菜单动画切换
    u8 icon_cnt;                        //切换频率
#if TASK_SHOWTIME_EN
    u8 back_sec;                        //无操作返回show time
#endif
    u8 disp_index;                      //正在显示的菜单项
    u8 disp_flag;                       //刷新显示
} type_task_menu_ctl;

extern type_task_menu_ctl t_menu;
extern IAR_CONST type_menu_item menu_item[];

void task_menu_display(void);           //这个函数需要在不同主题中定义

#endif
