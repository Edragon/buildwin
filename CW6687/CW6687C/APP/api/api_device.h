/*****************************************************************************
 * Module    : API
 * File      : api_device.h
 * Author    : Hanny
 * Function  : 设备管理的相应接口
 *****************************************************************************/
#ifndef API_DEVICE_H
#define API_DEVICE_H

/*****************************************************************************
 * Module    : 设备管理的相应接口
 *****************************************************************************/

#define UDISK_TEST_READY_TIME   15//1.5S

enum {
    DEVICE_SDMMC,               //SD/MMC卡
    DEVICE_UDISK,               //U盘
    DEVICE_LINEIN,              //LINE IN
    DEVICE_EAR,                 //连接耳机
    DEVICE_PC,                  //连接PC
    DEVICE_TAPE,                //连接磁带
    DEVICE_RESERVED,            //保留
};

enum {
    DEVICE_IDLE,                //设备无变化
    DEVICE_CHANGE,              //设备插入或变化
};

enum {
    DEV_UNACTIVE,               //设备未初始化
    DEV_ACTIVED,                //设备有效
    DEV_INVALID,                //设备无效
    DEV_NO_MEDIUM,              //设备不在线，针对读卡器而言
    DEV_ACTIVED_READY,          //设备有效，但需再次测试
    DEV_TEST_READY,             //测试设备是否在线，针对读卡器而言
};

typedef struct {
    u8 delay;                   //处理延时
    u8 cnt;                     //稳定计数
    u8 status;                  //瞬时状态
    u8 stable;                  //稳定状态
    u8 actived;                 //软件激活
} type_dev;

typedef struct {
    u8 dev_num;                 //当前激活的设备编号（U盘与SD卡）
    type_dev sd;                //SD/MMC卡
    type_dev udisk;             //连接U盘
    type_dev line;              //LINE
    type_dev ear;               //EAR PHONE
    type_dev pc;                //连接PC
    type_dev tape;              //连接磁带
    type_dev rsvd[2];           //保留
} type_dev_ctl;

void device_init(void);
void device_set_delay(u8 dev_num, u8 delay);
bool device_change_check(void);
__near_func bool device_is_online(u8 dev_num);

__near_func bool device_insert(u8 dev_num);
__near_func bool device_remove(u8 dev_num);
__near_func bool device_need_activate(u8 dev_num);
__near_func bool device_is_actived(u8 dev_num);
__near_func bool device_activate(u8 dev_num);
__near_func u8 device_get_actived(u8 dev_num);

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
__near_func u8 usb_connected(void);    //DP/DM因潮湿而误检测为PC，用这个函数更好
__near_func u8 usb_connected_1(void);  //修正三星充电器DP/DM短接且为1V左右，引起误检测为PC的问题
void usb_audio_set_volume(u8 volume);

bool usb_hid_set(u8 hid_key);

__near_func bool device_set_testready(u8 test_ready_time);

void sd_port_init_func(void (__near_func *sd_port_init_f)(u8 type));      //SD引脚
__near_func bool sd_stop(bool type);
void sd_reactive(void);
void usb_host_reactive(void);
__near_func void usb_host_suspend(void);
__near_func void usb_host_resume(void);
#endif

