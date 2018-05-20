/*****************************************************************************
 * Module    : API
 * File      : api_device.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 设备管理的相应接口
 *****************************************************************************/
#ifndef API_DEVICE_H
#define API_DEVICE_H

/*****************************************************************************
 * Module    : 设备管理的相应接口
 *****************************************************************************/

enum {
    DEVICE_SDMMC,               //SD/MMC卡
    DEVICE_UDISK,               //U盘
    DEVICE_LINEIN,              //LINE IN
    DEVICE_EAR,                 //连接耳机
    DEVICE_PC,                  //连接PC
    DEVICE_TAPE,                //连接磁带
    DEVICE_RESERVED,            //保留
};

//sd_port值:
//0: P20 P21 P27
//1: P20 P21 P27, clk复用检测
//2: P30 P31 P32
//3: P30 P31 P32, clk复用检测
void sd_init(u8 sd_port);
void sd_init_none(void);
void usb_init(void);
void device_var_init(void);
void device_init(u8 sd_port);
void device_set_delay(u8 dev_num, u8 delay);
bool device_change_check(void);
__near_func bool device_is_online(u8 dev_num);

__near_func bool device_insert(u8 dev_num);
__near_func bool device_remove(u8 dev_num);
__near_func bool device_need_activate(u8 dev_num);
__near_func bool device_is_actived(u8 dev_num);
__near_func bool device_activate(u8 dev_num);
__near_func u8 device_get_actived(u8 dev_num);
__near_func bool device_set_testready(void);

/*****************************************************************************
 * Module    : USB的相关接口
 *****************************************************************************/
//usb_connected函数的返回值
enum {
    USB_CONNECTED_NONE,     //未连接
    USB_CONNECTED_PC,       //连接PC，当Device
    USB_CONNECTED_UDISK,    //外接U盘，当Host
};

#define USB_DEV_MASS        0x01
#define USB_DEV_SPEAKER     0x02
#define USB_DEV_MIC         0x04
#define USB_DEV_HID         0x08

bool usb_dev_init(u8 dev_type);      //TYPE为枚举类型
void usb_dev_exit(void);
void usb_dev_event(void);
u8 usb_get_opt(void);
__near_func u8 usb_connected(void);
__near_func u8 usb_remove_check(void);
void usb_audio_set_volume(u8 volume);

bool usb_hid_set(u8 hid_key);

void usb_host_reactive(void);
void sd_reactive(void);
__near_func bool sd_stop(bool type);
__near_func void  sd_set_rate0(u8 rate);

#endif
