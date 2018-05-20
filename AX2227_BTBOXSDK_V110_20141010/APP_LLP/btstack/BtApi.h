#ifndef _BTAPI_HEADER
#define _BTAPI_HEADER
#include "include.h"


#define HEADSET_ENABLE                  1   //手机音频连接使能
#define A2DP_ENABLE                     1   //媒体音频连接使能
#define BT_HID_EN                       0   //是否打开蓝牙HID服务
#define BT_SPP_EN                       0   //是否打开蓝牙串口SPP服务

#define BT_SIMPLE_PAIR_EN               1   //是否使用简易配对
#define BT_CHANGE_PINCODE               0   //是否支持修改pincode
#define BT_DEF_PINCODE                  "0000"  //默认pincode

#define BT_EV3_EN                       0   //1:打开EV3，通话有重传机制，但差的蓝牙可能丢包通话断续；0:关闭EV3，虽然没有重传但语言比较连续
#define SCO_SUPERVISION_CHECK           0   //1:通话时检测是否通信变差有导致断线的可能并修复；

#define BT_SLEEP_CNT                    8   //BT模块超过8秒没收到数据，则认为进入睡眠
#define PowerOn_Connect_count           3   //上电尝试回连次数
#define SuperTimeout_Connect_count      24  //远距离断线，尝试回连次数，每次约5.6s

#define BT_FT_APPLE_DEVICE_EN           0x80    //是否支持苹果电量显示：0x80=支持，0x00=不支持
#define BT_FT_APPLE_VOL_CTRL_EN         0x00    //是否支持苹果音量控制：0x40=支持，0x00=不支持






#include "hci.h"
#include "l2cap.h"
#include "a2dp.h"
#include "hs_hf.h"
#include "hid.h"

#define  SHOW_BT_INFO                   0

#define bt_scan_enable                  3

#define _RESERVED_CID_003D_FOR_MODULE_TEST_
#define _RESERVED_CID_003E_FOR_SBC_DATA_

#if BT_SIMPLE_PAIR_EN
    #undef BT_CHANGE_PINCODE
    #define BT_CHANGE_PINCODE           0       //简易配对不需要动态修改密码
#endif

#define KICK_SOFT_INT() SPMODE |= BIT(7);

//BT_Status
extern u8 taskState;
#define BT_On_Start     0
#define BT_On_Fail      0xff
#define BT_On_Finish    0xfe
bool BT_On();

enum {
    StateNone = 0,
    StateIdle,
    StateSleep,
    StateConnected,
    StatePlay,
    OnThePhone,
    InComingCall,
    OutGoingCall,
    StateLineIn,
    StateShutDown,
    StateMp3Play,
    StateMp3Pause,
};


extern u8 bt_user_feature;

#define BT_FT_APPLE_DEVICE         0x01
#define BT_FT_APPLE_VOL_CTRL       0x02
#define IS_APPLE_VOL_CTRL       (BT_FT_APPLE_VOL_CTRL_EN && (bt_user_feature & BT_FT_APPLE_VOL_CTRL))
#define IS_APPLE_BT_DEVICE      (BT_FT_APPLE_DEVICE_EN && (bt_user_feature & BT_FT_APPLE_DEVICE))


#define StateMask              0x0F
//#define StopModeState          BIT(8)


#define tmr1msGetCurrentTicks()       tmr1ms_current_ticks
#define tmr1msIsExpired(offset_ticks, Texpire)    ((u16)((tmr1ms_current_ticks)+(u16)(0x00010000-(offset_ticks))) >= (Texpire))
#define tmr100msGetCurrentTicks()       tmr100ms_current_ticks
#define tmr100msIsExpired(offset_ticks, Texpire)    ((u16)((tmr100ms_current_ticks)+(u16)(0x00010000-(offset_ticks))) >= (Texpire))


extern u8 bt_connect_flag;
extern u8 bt_sleep_cnt;             //BT模块计数进入休眠模式
extern u8 send_ppkey_countdown;
extern bool need_send_ppkey;
extern IAR_BIT_A bool flag_bt_first;

extern u8 bt_dev_bat;

extern IAR_XDATA_A u8 phone_call_number[24];

#if BT_NAME_DYMANIC_CHANGE
extern IAR_CONST u8 bt_name[32];
#endif

bool sbc_load_code(u16 addr, u8 segnum);
void bt_os_init();
u8 bt_power_up();
void hal_uart_init(void);


void BT_Connect(void);
void BT_Connect_A2DP(void);
void BT_Disconnect(void);
void BT_Disconnect_A2DP(void);
void BT_Reconnect(void);
__near_func void process_BTVoice(void);
void BT_TimeOutReconnect();


extern volatile u8 uartTxWorking;
extern U32 axPwrsaveCountDown9sec;	//15sec/80ms=187.5

extern u8 NoSoundEnable;
extern u8 BT_ONLINE;
extern u8 BT_STATE;
extern u8 BT_STATE_pre;
extern u8 activateDiscState;
extern u8 Test_Mode_Flag;
__near_func void process_BTStateChange(void);
__near_func u8 GetA2DPChannelConnectionState(void);
__near_func u8 GetRfCommChannelConnectionState(void);
u8 GetSPPChannelConnectionState(void);
u8 GetSPPChannelDlci(void);

extern u8 SBCDataBankTail;
extern u8 SBCDataBankHead;
extern u32 uartRxQ2_skip;
extern IAR_BIT_A bool scoflag;  //for nokia 5000 use sco play music
extern IAR_BIT_A bool flag_btmode;            //为1时启用完整的蓝牙功能


void TurnOnRingTone(void);
void TurnOffRingTone(void);

bool  DB_SaveToNV(void);
u8 DB_SearchDevice(const u8*addr, u8 *linkey);
bool DB_AddRecord(const u8*addr, const u8 *linkey);
bool DB_DeleteRecord(const u8*addr);
bool DB_ReplaceRecord(const u8 index, const u8 *linkey);
void DB_CheckRecord(void);

void hcmd_change_local_name(u8 *parm, u8 len);
void hcmd_change_bt_addr(u8 *parm, u8 len);
void hcmd_remote_name_request(u8 *bd_addr);     //hcmd_remote_name_request(Link_BdAddr); callback function hevt_remote_name_complete
void hevt_remote_name_complete(u8 *name);       // utf bluetooth name
u8 hsf_send_atcmd(u8 *str, u16 len);            // u8 call_number_key[]="ATD13800138000;";  hsf_send_atcmd(call_number_key, sizeof(call_number_key));
void hcmd_w_scan_enable(u8 scan_enable);
bool hcmd_write_rf_reg(u8 addr, u16 value);

u8 RfcommSendATCommand(u16 cid, u8 dlci, u8 type, u16 len, const u8 __code *Data);
void AVRCPChangeVolume(void);


u8 send_hshfvol_hook(void);
void hshfvol_change_hook(u8 vol);
void a2dpvol_change_hook(u8 mode);


extern u16 tmr1ms_current_ticks;
extern u16 tmr100ms_current_ticks;
extern u8 bt_timeout_reconnect;
extern u16 tmr1s_ticks;


/********************************************************
 * A2DP相关
********************************************************/
enum {
    RQ2_FSM_NULL=0,
    RQ2_FSM_KICK0,
    RQ2_FSM_READ0_WAIT,
    RQ2_FSM_KICK1,
    RQ2_FSM_READ1,

    RQ2_FSM_HWCLK_START,
    RQ2_FSM_HWCLK_READ,  // send command
    RQ2_FSM_HWCLK_WAIT, //  wait act
    RQ2_FSM_HWCLK_CHECK,  // check

    RQ2_FSM_RESET_LC,  // reset lc
    RQ2_FSM_STOP,
};
extern u8 uartRxQ2_check_fsm;
extern u8 state_a2dp_player;
extern u16 avdtp_sbc_len;
extern u8 avdtp_sbc_rindex;
extern u8 avdtp_sbc_windex;
extern u8 *avdtp_sbc_rptr;
extern u8 *avdtp_sbc_wptr;
extern u16 avdtp_used_len;
extern u8  avdtp_sbc_active;    //
extern bool avdtp_play_status;

extern IAR_XDATA_A u16 SbcBufTotalLen;
extern IAR_XDATA_A u8* SbcBuf[2][2];

extern A2DPCon	A2DPAPI;

extern volatile u8 sbc_sample_rate;

void sbc_init(void);
__near_func void avdtp_sbc_reset();
__near_func bool player_set_dac(u8 sample);


/********************************************************
 * 通话相关
********************************************************/
#define SCO_FARPCM_SIZE         (0x800/2)      // unit 16bits

extern IAR_DATA_A u8 resample_delta_high;
extern IAR_DATA_A u8 resample_delta;
extern IAR_DATA_A u8 resample_delta_frac;

extern IAR_DATA_A u8 resample_step_high;
extern IAR_DATA_A u8 resample_step;
extern IAR_DATA_A u8 resample_step_frac;

extern IAR_DATA_A u8 resample_rate;      //同步DAC速度，0为正常速度，1为变慢，2为变快
extern u16 resample_rate_fast;

extern s16 l_sample1, l_sample2;


extern s16 *sco_farpcm_wptr;
extern s16 *sco_farpcm_rptr;
extern u16  sco_farpcm_len;
extern s16 sco_farpcm[SCO_FARPCM_SIZE];

extern s16 sco_mic_gain;
extern u8 sco_spk_stable;

extern u8 sco_cal_index;

#define UA_PCM_SIZE         (0x300+0xC0)    // 必须是 96 的倍数
extern s16 ua_pcm_buf[UA_PCM_SIZE];
extern __data s16 *sbc_pcm_wptr;
extern s16 *sbc_pcm_rptr;
extern __data volatile u16 sbc_pcm_len;
extern s16 sco_sample1;

extern volatile u8 *bt_voice_pcmrd;
extern volatile u8 *bt_voice_pcmwr;

extern s8 agc_buf_cnt;
extern u8 uartTxQ3[64];
extern u8 bt_debug_sco;
extern IAR_DATA_A volatile u8 bt_uart_txlen;
extern IAR_DATA_A volatile u8 bt_uart_rxlen;

extern volatile u8 sbc_dec_busy;

void bt_enter_hshf();
void bt_enter_hshf_do();
void bt_exit_hshf(void);
__near_func void sco_pcm_out(u16 pcm);
void hp_init(void);
void fir_lp_init(void);
__near_func s16 highpass(void *inptr);
__near_func void sco_mul_s16_s16(void *inptr, s16 gain);
__near_func void sco_echo_process(s8 high);
__near_func void Process_Agc_Data(void);


/********************************************************
 * 消息接口函数
********************************************************/
void btMMI_Forward();
void btMMI_Backward();
void btMMI_PlayPause();
void btMMI_Phone();
void btMMI_HandupDuringRing();
void btMMI_CallTransfer(void);


/********************************************************
 * 提示音相关
********************************************************/
// voice_state
enum {
    BT_VMS_IDLE = 0,
    BT_VMS_DELAY,
    //BT_VMS_DACEN,
    BT_VMS_INIT,
    BT_VMS_PLAYING,
    //BT_VMS_MUTING,
    //BT_VMS_DATAEND,
    BT_VMS_PLAYEND,
};

#define       BT_VOICE_CONNECT              RES_WAV_BT_CONNECT
#define       BT_VOICE_DISCONNECT           RES_WAV_BT_DISCONNECT
#define       BT_VOICE_NUM0                 RES_WAV_NUM0
#define       BT_VOICE_NUM1                 RES_WAV_NUM1
#define       BT_VOICE_NUM2                 RES_WAV_NUM2
#define       BT_VOICE_NUM3                 RES_WAV_NUM3
#define       BT_VOICE_NUM4                 RES_WAV_NUM4
#define       BT_VOICE_NUM5                 RES_WAV_NUM5
#define       BT_VOICE_NUM6                 RES_WAV_NUM6
#define       BT_VOICE_NUM7                 RES_WAV_NUM7
#define       BT_VOICE_NUM8                 RES_WAV_NUM8
#define       BT_VOICE_NUM9                 RES_WAV_NUM9
#define       BT_VOICE_RING                 RES_WAV_RING
#define       BT_VOICE_KEY                  RES_WAV_KEY
//#define       BT_VOICE_START                RES_WAV_START


extern u8 bt_voice_inptr;
extern u8 bt_voice_outptr;
extern u8 bt_voice_msg;    //
extern u8 bt_voice_state;
extern volatile u8 bt_voice_abort;
extern volatile u8 bt_voice_delay_cnt;
void bt_voice_put(u8 msg);
u8 bt_voice_get(void);
__near_func void bt_voice_play(void);
__near_func void process_bt_voice_do();


/********************************************************
 * 蓝牙信息相关
********************************************************/
#define BT_RFINFO_NAME      (16*1)
#define BT_RFINFO_ADDR      (16*3)
#define BT_RFINFO_PARM      (16*4)

void spi_bt_config_erase(); // 擦除蓝牙名字和配置信息等
void *spi_bt_rfinfo_read(u8 len);     // 读取蓝牙RFINFO(RF参数，名字，地址),返回参数的BUFFER
void spi_bt_rfinfo_write(void *buf);    // 写入蓝牙RFINFO(RF参数，名字，地址)


extern union {
    struct {
        u8 bt_status_flag:                       1;     //蓝牙状态栏显示更新标志
        u8 bt_state_flag:                        1;     //蓝牙通话状态显示更新标志
        u8 bt_atcmd_clcc_flag:                   1;     //蓝牙获取去电号码命令标志
        u8 bt_phonenum_flag:                     1;     //蓝牙通话号码显示更新标志
    };
    u8 bt_disp_flag;
};
#endif
