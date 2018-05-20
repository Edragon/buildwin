/*****************************************************************************
 * Module    : User
 * File      : user_bt.h
 * Author    : Hanny
 * Function  : ��������û�API�ӿ�
 *****************************************************************************/
#ifndef USER_BT_H
#define USER_BT_H

#include "menu.h"

#define BT_RFINFO_SET           0x06
#define BT_RFINFO_MIC           0x0a
#define BT_RFINFO_NAME          0x0e
#define BT_RFINFO_ADDR          0x2e
#define BT_RFINFO_VOICE         0x40

#if BT_SAMPLERATE_16K
    #define BT_VIOCE_SAMPLES    256
#else
    #define BT_VIOCE_SAMPLES    128
#endif

//BT_RFINFO_SET BIT
#define BT_RFINFO_SET_NAME      0
#define BT_RFINFO_SET_ADDR      1
#define BT_RFINFO_SET_VOICE     2
#define BT_RFINFO_SET_MIC       3
#define BT_RFINFO_SET_DYADDR    6

/*****************************************************************************
 * Module    : ���������Ϣƫ�Ƶ�ַ��Ϊ�˷�����䣬���0��Ч����1��ʼ
 *****************************************************************************/
#define BT_PARAM_LATEST             0       //���һ����Ч���������        1byte
#define BT_PARAM_ORDER              4       //�����Ϣ����                  ����16byte
#define BT_PARAM_PAIRS              8       //�����Ϣ                      58byte x 4 = 232byte
//������ÿ�������Ϣ��ƫ�Ƶ�ַ
#define BT_PAIR_NUM                 4       //���֧��4�������Ϣ
#define BT_PAIR_SIZE                sizeof(TYPE_BT_PAIR)                                //�����Ϣ
#define BT_PAIR_VALID(n)            ((BT_PAIR_SIZE * (n)) + BT_PARAM_PAIRS)             //�����Ϣ�Ƿ���Ч      1byte
#define BT_PAIR_ADDR(n)             ((BT_PAIR_SIZE * (n)) + BT_PARAM_PAIRS + 1)         //������ַ              6byte
#define BT_PAIR_LINK_KEY(n)         ((BT_PAIR_SIZE * (n)) + BT_PARAM_PAIRS + 1+6)       //����link key          16byte
#define BT_PAIR_NAME(n)             ((BT_PAIR_SIZE * (n)) + BT_PARAM_PAIRS + 1+6+16)    //��������              26byte

#if BT_PHONE_BOOK_EN
#define BT_MENU_ITEM_NUM   8
#else
#define BT_MENU_ITEM_NUM   7
#endif

#if IS_LCD_DISPLAY
enum {
    ITEM_BT_CON,
#if BT_PHONE_BOOK_EN
    ITEM_BT_PHONEBOOK,
#endif
    //ITEM_BT_LIST,
    ITEM_BT_EQ,
    ITEM_BT_DEL_PAIRED,
    ITEM_BT_INFO,
    ITEM_BTMENU_BACK,
    ITEM_BT_EXIT,
};

extern string IAR_CONST tbl_bt_menu_const[];
extern __no_init __root string tbl_bt_menu[];
extern u8 *bt_tmp;
#endif

typedef struct __TYPE_BT_PAIR {
    u8 valid;                               //���һ����Ч���������
    //u8 reserved[1];
    u8 addr[6];
    u8 link_key[16];
    u8 name[32];
} TYPE_BT_PAIR;

typedef struct {
    u8 valid;                               //���һ����Ч���������
    //u8 reserved[1];
    u8 addr[6];
    u8 link_key[16];
    //u8 name[32];
} TYPE_BT_PAIR2;

typedef struct __TYPE_BT_PARAM_PAIR {
    u8 latest;                              //���һ����Ч���������
    u8 reserved[3];
    u8 order[4];
    TYPE_BT_PAIR pair[4];
#if TASK_BTHID_EN
    TYPE_BT_PAIR2 pair2[1];                 //HID����ģʽ������Ϣ
#endif
} TYPE_BT_PARAM_PAIR;

typedef struct {
    u8 disp_bt_sta;
    u8 disp_dev_bat;                //ƻ���豸������ʾֵ
    u8 add_sco_flag;                //�ж��Ƿ��Ѿ�����add_sco����
    u8 hid_enable;                  //�Ƿ�����HID
#if IS_LCD_DISPLAY
    u8 updata_flag;
    u8 disp_status_flag;
    u8 hostname_change;
    u8 phone_num_disp;      //  �绰������ʾ��־
    u8 incoming_call_disp;  //  Incoming Call��ʾ��־
    u8 calling_disp;        //  ͨ������ʾ��־
    u32 play_time;          //  ������ʾ�Ĳ���ʱ��
    u8 bt_paired_update;    //  �������Ӻ������Ϣ�Ƿ���±��
#endif
} type_bt;

extern type_bt t_bt;
extern bool bt_play_status;
extern IAR_BIT_A bool task_hid_flag;

void user_bt_init(void);
void user_bt_off(void);
void user_bt_event(void);
u8 bt_status_get(void);             //���·�װ����״̬��ȡ
void bt_pause_play(void);           //�������ֲ���/��ͣ
void send_vol_to_iphone_hook(void);

bool bt_hid_photo(void);

void btinfo_get_name(u8 *ptr);
void btinfo_get_addr(u8 *ptr);
void btinfo_get_rfparam(u8 *ptr);
void btinfo_get_mic(u8 *ptr);
void btinfo_get_voice(u8 *ptr);
void spp_tx(char *ptr, u8 len);               //SPP���ͽӿ�
char hex_to_char(u8 num);
u8 char_to_hex(u8 c);
void spp_msc_get_lfn(u16 total_num);
void spp_tx_msc_num(u16 num);
void spp_tx_msc_total(u16 total);

extern u16 spp_msc_lfn_num;
extern volatile u8 bt_sleep_cnt;             //BTģ�������������ģʽ
extern IAR_BIT_A bool bt_low_scan_flag;
void send_bat_to_iphone_hook(void);
char hex_to_char(u8 num);
u8 get_num_len(void);
u8 *get_num_buf(void);
void set_num_len(u8 num);

void task_bt_enter(void);
void task_bt_event(void);
void task_bt_display(void);
void task_bt_hid_deal_msg(u8 msg);
void task_bt_exit(void);
u8 get_chip_type(void);

#endif
