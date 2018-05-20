/*****************************************************************************
* Module    : Display
* File      : ocx.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : 控件管理中心
*****************************************************************************/
#include "include.h"
#include "task.h"

type_ocx_ctl ocx_ctl;

void ocx_init(void (*event_func)(void), bool (*msg_func)(u8 msg))
{
    ocx_ctl.event_func = event_func;
    ocx_ctl.msg_func = msg_func;
}

void ocx_event(void)
{
    if (NULL != ocx_ctl.event_func) {
        (*ocx_ctl.event_func)();
    }
}

void ocx_msg(u8 msg)
{
    bool res = false;
    if (NULL != ocx_ctl.msg_func) {
        res = (*ocx_ctl.msg_func)(msg);
    }
    if (!res) {
        deal_msg(msg);
    }
}
