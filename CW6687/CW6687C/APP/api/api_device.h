/*****************************************************************************
 * Module    : API
 * File      : api_device.h
 * Author    : Hanny
 * Function  : �豸�������Ӧ�ӿ�
 *****************************************************************************/
#ifndef API_DEVICE_H
#define API_DEVICE_H

/*****************************************************************************
 * Module    : �豸�������Ӧ�ӿ�
 *****************************************************************************/

#define UDISK_TEST_READY_TIME   15//1.5S

enum {
    DEVICE_SDMMC,               //SD/MMC��
    DEVICE_UDISK,               //U��
    DEVICE_LINEIN,              //LINE IN
    DEVICE_EAR,                 //���Ӷ���
    DEVICE_PC,                  //����PC
    DEVICE_TAPE,                //���ӴŴ�
    DEVICE_RESERVED,            //����
};

enum {
    DEVICE_IDLE,                //�豸�ޱ仯
    DEVICE_CHANGE,              //�豸�����仯
};

enum {
    DEV_UNACTIVE,               //�豸δ��ʼ��
    DEV_ACTIVED,                //�豸��Ч
    DEV_INVALID,                //�豸��Ч
    DEV_NO_MEDIUM,              //�豸�����ߣ���Զ���������
    DEV_ACTIVED_READY,          //�豸��Ч�������ٴβ���
    DEV_TEST_READY,             //�����豸�Ƿ����ߣ���Զ���������
};

typedef struct {
    u8 delay;                   //������ʱ
    u8 cnt;                     //�ȶ�����
    u8 status;                  //˲ʱ״̬
    u8 stable;                  //�ȶ�״̬
    u8 actived;                 //�������
} type_dev;

typedef struct {
    u8 dev_num;                 //��ǰ������豸��ţ�U����SD����
    type_dev sd;                //SD/MMC��
    type_dev udisk;             //����U��
    type_dev line;              //LINE
    type_dev ear;               //EAR PHONE
    type_dev pc;                //����PC
    type_dev tape;              //���ӴŴ�
    type_dev rsvd[2];           //����
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
__near_func u8 usb_connected(void);    //DP/DM��ʪ������ΪPC���������������
__near_func u8 usb_connected_1(void);  //�������ǳ����DP/DM�̽���Ϊ1V���ң���������ΪPC������
void usb_audio_set_volume(u8 volume);

bool usb_hid_set(u8 hid_key);

__near_func bool device_set_testready(u8 test_ready_time);

void sd_port_init_func(void (__near_func *sd_port_init_f)(u8 type));      //SD����
__near_func bool sd_stop(bool type);
void sd_reactive(void);
void usb_host_reactive(void);
__near_func void usb_host_suspend(void);
__near_func void usb_host_resume(void);
#endif

