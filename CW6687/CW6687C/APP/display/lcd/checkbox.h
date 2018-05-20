/*****************************************************************************
 * Module    : Display
 * File      : checkbox.h
 * Author    : Hanny
 * Function  : 多选框控件的相关定义
 *****************************************************************************/
#ifndef CHECKBOX_H
#define CHECKBOX_H

typedef struct {
    u32 menu_value;                     //每个菜单项对应该变量的1bit，从低位开始算起
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
    u8 disp_flag;                       //按确定后，需要刷新显示
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
} type_checkbox_ctl;

extern type_checkbox_ctl t_check;

#endif
