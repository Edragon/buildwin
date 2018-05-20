/*****************************************************************************
 * Module    : Display
 * File      : msgbox.c
 * Author    : Hanny
 * Function  : 对话框控件，用于处于UI中用到的对话框功能
 *****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY
#include "task.h"
//list box controller
IAR_XDATA_A struct {
    u8 disp_index;                      //当前显示的选择项，用于判断是否需要刷新
    u8 index;                           //当前的选择项
    u8 m_icon;                          //是否显示对话框的图标
    u8 m_res;                           //文字模式或资源
    u8 m_title;                         //是否需要显示列表框标题
    u8 m_yesno;                         //是否需要显示两个按键
    u8 m_ok;                            //是否需要OK键
    u8 ok_flag;                         //是否按下确定键
    u8 show_time;                       //显示时间
    string msg;                         //消息内容
} t_msg @ "OCX_XDATA";

#pragma location="MSG_BOX_SEG"
void msgbox_event(void)
{
    if (!comm_event()) {
        t_msg.ok_flag = 0xff;
    }
    if (t_msg.show_time && ocx_show_cnt >= t_msg.show_time) {
        t_msg.ok_flag = 0xff;
    }
}

//列表控件处理消息
#pragma location="MSG_BOX_SEG"
void msgbox_deal_msg(void)
{
    u8 msg = get_msg();
    if(msg >= 0x6d && msg < 0x7e) { //重要的系统消息保留下来等待退出后处理
        put_msg(msg);
        return;
    }
    switch(msg) {
        //确定
    case KU_OK:
        t_msg.ok_flag = 1;
        break;

#if THEME_SELECT==THEME_LCD_DEFAULT
    case KL_PLAY_POWER:
        task_ctl.work_sta = TASK_EXIT;
        break;

        //取消
    case KU_PLAY_POWER:
#else
    case KU_BACK:
#endif
        t_msg.ok_flag = 0xff;
        break;

        //上一项，下一项
    case KU_PREV:
        if (t_msg.m_yesno) {
            ocx_show_cnt = 0;
            t_msg.index = 1;
        }
        break;
    case KU_NEXT:
        if (t_msg.m_yesno) {
            ocx_show_cnt = 0;
            t_msg.index = 0;
        }
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}

//列表显示接口
#pragma location="MSG_BOX_SEG"
void msgbox_display(void)
{
   // char *msg_ptr;
    if (t_msg.index == t_msg.disp_index) {
        return;
    }
#if MSGBOX_SIMPLE
    if (!t_msg.m_yesno && !t_msg.m_ok) {
        //快速消息框
        line_update_clear(0);
        disp_menu(t_msg.msg, 0, 0, 0, ME_LINES|ME_MSG);//
        line_update_clear(4);
        line_update(6);
        return;
    }
#endif
    t_msg.disp_index = t_msg.index;
    line_clear();
    if (t_msg.m_title) {
        //标题栏模式
    } else {
        //无标题栏模式
        disp_menu(STR_MSG_TITLE, 0, 0, 0, ME_CENTER);
    }
    line_update_clear(0);
   /* disp_menu(t_msg.msg, 0, 0, 0);
     line_update_clear(2);
    if(t_msg.msg==STR_FACTORY_CONFIRM)
    {
      disp_menu(t_msg.msg+1, 2, 0, 0);
    }
    line_update_clear(4);*/
    disp_menu(t_msg.msg, 0, 0, 0, ME_LINES|ME_MSG);//
    line_update_clear(4);
    if (t_msg.m_yesno) {
        disp_menu(STR_MSG_OK, 25, 0, 0, 0);
        disp_menu(STR_MSG_CANCEL, 79, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
        disp_invert((t_msg.index == 1) ? 22 : 76,        //反色选取
            0,
            (sys_ctl.lang_id == 0 || t_msg.index == 1) ? 30 : 42,
            2
        );
#else
        disp_invert((t_msg.index == 1) ? 22 : 76,        //反色选取
            0,
            (sys_ctl.lang_id == 0 || t_msg.index == 1) ? 37 : 49,
            2
        );
#endif
    } else {
        disp_menu(STR_MSG_OK, 52, 0, 0, 0);
#if(FONT_SEL == SMALL_FONT)
        disp_invert(49, 0, 30, 2);
#else
        disp_invert(49, 0, 35, 2);
#endif
    }
    line_update(6);
}
//mode模式选择;  param: 内容参数
//返回确定或取消
#pragma location="MSG_BOX_SEG"
bool msgbox(u16 param, u8 mode, u8 show_time)
{
    t_msg.disp_index = 0xFF;
    t_msg.index = 0;                                //默认选择否
    t_msg.m_title = (mode & MB_TITLE) ? 1 : 0;
    t_msg.m_res = (mode & MB_RES) ? 1 : 0;
    t_msg.m_yesno = (mode & MB_YESNO) ? 1 : 0;
    t_msg.m_ok = (mode & MB_OK) ? 1 : 0;
    t_msg.ok_flag = 0;
    t_msg.show_time = show_time;

#pragma diag_remark=Pe1053
    //暂时忽略Pe1053这个警告，指针类型大转小
    t_msg.msg = (string)param;
#pragma diag_warning=Pe1053
    if (!t_msg.m_yesno) {
        t_msg.index = 1;                //只有确认可选
    }

    ocx_show_cnt = 0;
    while (!t_msg.ok_flag) {
        msgbox_event();
        msgbox_deal_msg();
        msgbox_display();
#if ALARM_EN
        if (task_ctl.work_sta == TASK_USBDEV || task_ctl.work_sta == TASK_ALARM) {
#else
        if (task_ctl.work_sta == TASK_USBDEV) {
#endif
            t_msg.ok_flag = 0xfe;
        }
    }

    if (t_msg.ok_flag == 1 && t_msg.index == 1) {
        return true;                    //按下确定键，并且选择了确定按钮
    }
    return false;
}
#endif
