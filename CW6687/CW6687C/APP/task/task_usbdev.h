/*****************************************************************************
 * Module    : Task
 * File      : task_usbdev.h
 * Author    : Hanny
 * Function  : USB设备的相关定义
 *****************************************************************************/
#ifndef TASK_ALARM_H
#define TASK_ALARM_H

#define USB_AUDIO_VOLMAX        15  //音量级数

#define USBMIC_SAMPLES          0x200

//USB Audio音量调节
#define USB_AUDIO_STOP          0x80
#define USB_AUDIO_PREFILE       0x20
#define USB_AUDIO_NEXTFILE      0x10
#define USB_AUDIO_PLAYPAUSE     0x08
#define USB_AUDIO_MUTE          0x04
#define USB_AUDIO_VOLDOWN       0x02
#define USB_AUDIO_VOLUP         0x01
#define USB_AUDIO_NONE          0x00

enum {
    OPT_NONE,
    OPT_READ,
    OPT_WRITE,
};

typedef struct {
    u8 disp_opt;                            //显示的读写状态
    u8 opt_status;                          //当前的读写状态
    u8 disp_update;                         //控制屏显更新。主要限制更新频率
    u8 disp_volume;                         //显示的音量值
    u8 first_key;                           //发送第一个按键
    u8 vol_flag;                            //调节声音标志
} type_task_usbdev;

extern type_task_usbdev t_usbdev;
extern IAR_BIT_A bool usb_dev_flag;

__near_func void task_usbmic_process(void);
void task_usbdev_display(void);

#endif
