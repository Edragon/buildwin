#ifndef USER_BT_H
#define USER_BT_H

#define MAX_PAIRED_DEVICE_NUM    6
#define BT_PAIRED_LEN       54
#define MAX_PAIRED_INFO_NUM     70
#define MAX_RECON_CNT           5
#define RECON_INTERVAL          600     //3s间隔
#define MAX_DISCON_CNT          6
#define DISCON_INTERVAL         70      //350ms间隔

#define SBC_CTS_DIR    P3DIR
#define SBC_CTS_PORT   P3
#define SBC_CTS_BIT    3
#define SBC_CTS_DIR_OUT()   SBC_CTS_DIR&=~BIT(SBC_CTS_BIT)
#define SBC_CTS_H()         SBC_CTS_PORT|=BIT(SBC_CTS_BIT)
#define SBC_CTS_L()         SBC_CTS_PORT&=~BIT(SBC_CTS_BIT)

#define BT_PARAM_ADDRESS         0x3A
#define BT_PARAM_NAME            0xc0
#define BT_PARAM_PDL_LENGTH      0x100//1 Byte
#define BT_PARAM_PDL             0x101//8 Byte
#define BT_PARAM_REMOTE_DEVICE0  0x109//6 Byte + 16 Byte = 22 Byte

#define BT_PARAM_READ(addr,buf,len)
#define BT_PARAM_WRITE(addr,buf,len)



#define LED_BT_PLAY()
#define LED_BT_PAUSE()
#define LED_BT_NC()

//Command
enum{
  COMMAND_WAKEUP_READY                = 0x00,
  COMMAND_INITIAL_CW6633              = 0x01,
  COMMAND_GET_AX2220_CODE             = 0x02,
  COMMAND_SET_LOCAL_BT_ADDRESS_NAME   = 0x03,
  COMMAND_VERSION_CONTROL             = 0x04,
  COMMAND_POWER_ON                    = 0x21,
  COMMAND_POWER_OFF                   = 0x22,
  COMMAND_ENTER_PAIRING_MODE          = 0x23,
  COMMAND_EXIT_PAIRING_MODE           = 0x24,
  COMMAND_CREATE_CONNECTION           = 0x25,
  COMMAND_DISCONNECT                  = 0x26,
  COMMAND_SEND_PRESS_BUTTON           = 0x27,
  COMMAND_RESPONSE_VOLUME_CONTROL     = 0x28,
  COMMAND_RESPONSE_LINK_KEY           = 0x29,
  COMMAND_AVDTP_STATUS                = 0x2A,
  COMMAND_REQUEST_PARING_INFO         = 0x2F,
  COMMAND_SYNC                        = 0x30,
  COMMAND_POWERSAVING                 = 0x31,//COMMAND_DEVICE_SYSTEM_STATE
  COMMAND_SOFTWARE_RESET              = 0x32,
  COMMAND_DEVICE_REQUEST_PARAMETERS   = 0x33,
  COMMAND_TEST_MODE                   = 0x34,
  COMMAND_LOW_DATA                    = 0x36
};
//Event
enum{
  EVENT_WAKEUP_READY                = 0x00,
  EVENT_INITIAL_CW6633              = 0x01,
  EVENT_GET_AX2220_CODE             = 0x02,
  EVENT_SET_LOCAL_BT_ADDRESS_NAME   = 0x03,
  EVENT_VERSION_CONTROL             = 0x04,
  EVENT_POWER_ON                    = 0x21,
  EVENT_POWER_OFF                   = 0x22,
  EVENT_ENTER_PAIRING_MODE          = 0x23,
  EVENT_EXIT_PAIRING_MODE           = 0x24,
  EVENT_CREATE_CONNECTION           = 0x25,
  EVENT_DISCONNECT                  = 0x26,
  EVENT_SEND_PRESS_BUTTON           = 0x27,
  EVENT_RESPONSE_VOLUME_CONTROL     = 0x28,
  EVENT_RESPONSE_LINK_KEY           = 0x29,
  EVENT_AVDTP_STATUS                = 0x2A,
  EVENT_DEVICE_CONNECT                = 0x2B,
  EVENT_DEVICE_DISCONNECT             = 0x2C,
  EVENT_REMOTE_CONNECT                = 0x2D,
  EVENT_REMOTE_DISCONNECT             = 0x2E,
  EVENT_WRITE_PAIRING_INFO            = 0x2F,
  EVENT_SYNC                          = 0x30,
  EVENT_POWERSAVING                   = 0x31,//EVENT_DEVICE_SYSTEM_STATE
  EVENT_SOFTWARE_RESET                = 0x32,
  EVENT_DEVICE_REQUEST_PARAMETERS     = 0x33,
  EVENT_TEST_MODE                     = 0x34,
  EVENT_LOW_DATA                      = 0x36
};
//STATUS -- ERROR CODE
enum{
  SUCCESS                    = 0x00,
  CONNECTING                 = 0x01,
  DISCONNECTING              = 0x02,
  ALREADY_DISCONNECT         = 0x03,
  ALREADY_CONNECT            = 0x04,
  PAGE_TIMEOUT               = 0x05,
  SUPERVISION_TIMEOUT        = 0x06,
  REMOTE_NOT_ALLOW_CONNECT   = 0x07,
  LINK_KEY_NOT_EXIST         = 0x08,
  AUTH_FAIL                  = 0x09
};
//KEY VALUE
enum{
  KEY_VOLUME_UP          = 0x41,
  KEY_VOLUME_DOWN        = 0x42,
  KEY_PLAY_PAUSE         = 0x44,
  KEY_STOP               = 0x45,
  KEY_FORWARD            = 0x4B,
  KEY_BACKWARD           = 0x4C
};
//AVDTP Status
enum{
  AVDTP_DISCOVER         = 0x01,
  AVDTP_GET_CAPABILITIES = 0x02,
  AVDTP_SET_CONFIGRATION = 0x03,
  AVDTP_GET_CONFIGRATION = 0x04,
  AVDTP_RECONFIGURE      = 0x05,
  AVDTP_OPEN             = 0x06,
  AVDTP_START            = 0x07,
  AVDTP_CLOSE            = 0x08,
  AVDTP_SUSPEND          = 0x09,
  AVDTP_ABORT            = 0x0A
};
//BT Status
enum{
  BT_STATUS_INITIAL             = 0x00,
  BT_STATUS_PAIRING             = 0x01,
  BT_STATUS_CONNECTING          = 0x02,
  BT_STATUS_CONNECTED           = 0x03,
  BT_STATUS_PLAYING             = 0x04
};
//  1、开机上电BT_INIT_WAKEUP - BT_INIT_END
//  2、切模式BT_INIT_WAKEUP - BT_INIT_PWRON-1，BT_INIT_REQSTAT2 - BT_INIT_END
//  3、唤醒BT_INIT_REQSTAT3 - BT_INIT_END
enum{
    BT_INIT_NOT         = 0,
    BT_INIT_WAKEUP      = 1,
    BT_INIT_LOWDATA     = 6,
 //   BT_INIT_REQSTAT1    = 10,
    BT_INIT_PWRON       = 10,
//    BT_INIT_SETPAR      = 30,
    BT_INIT_REQSTAT2    = 30,
    BT_INIT_CRTCON      = 40,
    BT_INIT_REQSTAT3    = 41,   //  用于中途唤醒蓝牙
    BT_INIT_STAT_CHECK  = 45,
    BT_INIT_END         = 46
};

enum{
    BT_SINGLE_NOTCONNECT    = 0x00,
    BT_SINGLE_CONNECTED     = 0xfe,
    BT_SINGLE_PLAYING       = 0xfd
};

extern void sbc_uart_io_init(void);
extern __near_func void set_sbc_cts(void);
extern __near_func void clr_sbc_cts(void);
extern void bt_info_read(void);
extern void bt_shutdown(void);
extern void bt_power_off(void);
extern void uart_bt_putc(u8 c);
extern void sbc_command_normal(u8 group, u8 request,u8 length, u8 *pdata);

#endif  //USER_BT_H
