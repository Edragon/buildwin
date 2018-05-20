/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : USB设备的相关定义
 *****************************************************************************/
#ifndef TASK_ALARM_H
#define TASK_ALARM_H

#define USB_AUDIO_VOLMAX      15  //音量级数

typedef struct {
    u8 disp_opt;                            //显示的读写状态
    u8 opt_status;                          //当前的读写状态
    u8 disp_update;                         //控制屏显更新。主要限制更新频率
    u8 disp_volume;                         //显示的音量值
    u8 first_key;                           //发送第一个按键
    u8 vol_flag;                            //调节声音标志
} type_usbdev_task;

extern type_usbdev_task t_usbdev;


void task_usbdev_display(void);

#endif
