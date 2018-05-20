/*****************************************************************************
 * Module    : Display
 * File      : listbox.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 列表控件的相关定义
 *****************************************************************************/
#ifndef LISTBOX_H
#define LISTBOX_H

typedef struct {
    u16 disp_index;                     //当前显示的菜单项，用于判断是否需要刷新
    u16 index;                          //当前菜单
    u16 total;                          //菜单总数
    u16 page_index;                     //当前菜单所在页的第一项
    u8 n_page;                          //每页显示条数
    u8 m_icon;                          //是否显示菜单的图标
    u8 m_title;                         //是否需要显示列表框标题
    u8 m_roll;                          //滚动菜单还是翻页菜单
    u8 m_eq;                            //是否在显示eq
    u8 ok_flag;                         //是否按下确定键
    u8 show_time;                       //显示时间
    union {
        //For ListBox
        struct {
            u8 m_res;                           //文字模式或资源
            string *llp_item;               //复读机字符串列表
        };
        //For ListBoxA
        struct {
            u8 itemlen;                         //菜单项的字符串长度
            char *item;                         //列表内容
            char *(*get_list)(u16 index, u8 cnt);  //列表内容获取钩子
        };
    };
} type_list_ctl;

#if SLIDER_EN
typedef struct {
   u8 line;  //滑块的起始行，大行，1大行有16小行
   u8 angle_line_nums; // 滑块大小，即多少小行
   u8 line_remainder; // 滑块大小， 滚动条中剩余的空白小行
   u8 line_start;  // 滑块的中大行里的第几小行，最大值为16；
   u8 pre_num;
} type_slider_ctl;

extern type_slider_ctl t_slider;
extern type_slider_ctl t_slider;
extern void listbox_display_slider_init(u8 num);
extern void listbox_display_slider(u8 line,u8 num);
#endif
extern type_list_ctl t_list;
extern void menu_move_control(u8 wide);
#endif
