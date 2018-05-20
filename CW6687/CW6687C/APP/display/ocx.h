/*****************************************************************************
* Module    : Display
* File      : ocx.c
* Author    : Hanny
* Function  : 控件的相关定义
*****************************************************************************/
#ifndef OCX_H
#define OCX_H

typedef struct {
    void (*event_func)(void);            //控件的事件
    bool (*msg_func)(u8 msg);            //控件需要响应的消息
} type_ocx_ctl;

//进度条参数，这个为值回调模式
typedef struct {
    string title;
    bool (*on_change)(u8 value);
    bool (*on_click)(u8 value);
} type_progress_param;

extern type_ocx_ctl ocx_ctl;

#define LB_NO_TITLE     0x01    //是否需要菜单标题栏
#define LB_RES          0x02    //文字模式还是资源模式
#define LB_NO_ICON      0x04    //是否不显示图标
#define LB_PAGE         0x08    //使用翻页菜单
#define LB_EQ           0x10    //EQ列表

#define MB_TITLE        0x01    //是否需要菜单标题栏
#define MB_RES          0x02    //文字模式还是资源模式
#define MB_YESNO        0x80    //两个按键的模式
#define MB_OK           0x40    //单OK按键的形式

#define PG_VOLKEY       0x10    //Volume Key模式，改响应Volume Key

#define FL_CDUP         0x08    //是否需要返回上一级的菜单

//文件名最后1byte为标志位

#define FB_UNICODE      0x00
#define FB_GBK          0x80

#define FB_ICON_MASK    0x0f
#define FB_DEFAULT      0x00
#define FB_FOLDERUP     0x01
#define FB_FOLDER       0x02
#define FB_FILE         0x03
#define FB_MUSIC        0x04

#define TX_CENTER       0x7FFF      //文字居中显示
#define TX_HALF         0x01        //是否显示半格的字

void ocx_init(void (*event_func)(void), bool (*msg_func)(u8 msg));
void ocx_event(void);
void ocx_msg(u8 msg);

u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time);
u32 checkbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time, u32 lparam);
u16 listboxa(u16 index, u16 total, u8 mode, u8 itemlen, u32 param);
bool msgbox(u16 param, u8 mode, u8 show_time);

u8 progressbox(u8 value, u8 min, u8 max, u32 param, u8 mode, u8 show_time);
u16 filelist(u16 index, u8 mode);
u16 password(void);
u16 numbox(u8 value, u8 mode);
void msg_direct_display(u16 param,u8 mode);

#endif
