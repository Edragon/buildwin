/*****************************************************************************
 * Module    : API
 * File      : api_device.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �豸�������Ӧ�ӿ�
 *****************************************************************************/
#ifndef API_DEVICE_H
#define API_DEVICE_H

/*****************************************************************************
 * Module    : �豸�������Ӧ�ӿ�
 *****************************************************************************/

enum {
    DEVICE_SDMMC,               //SD/MMC��
    DEVICE_UDISK,               //U��
    DEVICE_LINEIN,              //LINE IN
    DEVICE_EAR,                 //���Ӷ���
    DEVICE_PC,                  //����PC
    DEVICE_TAPE,                //���ӴŴ�
    DEVICE_RESERVED,            //����
};

//sd_portֵ:
//0: P20 P21 P27
//1: P20 P21 P27, clk���ü��
//2: P30 P31 P32
//3: P30 P31 P32, clk���ü��
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
 * Module    : USB����ؽӿ�
 *****************************************************************************/
//usb_connected�����ķ���ֵ
enum {
    USB_CONNECTED_NONE,     //δ����
    USB_CONNECTED_PC,       //����PC����Device
    USB_CONNECTED_UDISK,    //���U�̣���Host
};

#define USB_DEV_MASS        0x01
#define USB_DEV_SPEAKER     0x02
#define USB_DEV_MIC         0x04
#define USB_DEV_HID         0x08

bool usb_dev_init(u8 dev_type);      //TYPEΪö������
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
