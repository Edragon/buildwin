/*****************************************************************************
 * Module    : API
 * File      : api_bt.h
 * Author    : Hanny
 * Function  : 蓝牙相关API接口
 *****************************************************************************/
#ifndef API_BT_H
#define API_BT_H

#define BT_SLEEP_CNT                    8           //BT模块超过8秒没收到数据，则认为进入睡眠

//蓝牙状态，用bt_get_status函数来获取
enum {
    BT_STA_OFF,                                 //蓝牙模块已关闭
    BT_STA_IDLE,                                //未连接
    BT_STA_SLEEP,                               //低功耗

    BT_STA_DISCONNECTING,                       //断开中
    BT_STA_CONNECTING,                          //连接中

    BT_STA_CONNECTED,                           //已连接
    BT_STA_PLAYING,                             //播放
    BT_STA_INCOMING,                            //来电响铃
    BT_STA_OUTGOING,                            //正在呼出
    BT_STA_PHONE,                               //通话中
};

enum {
    BT_EVT_DISCONNECT,                          //蓝牙断开事件
    BT_EVT_LOSTCONNECT,                         //蓝牙远距离连接丢失事件
    BT_EVT_CONNECT,                             //蓝牙连接事件
    BT_EVT_CONNECTED,                           //蓝牙连接成功事件
    BT_EVT_PLAY,                                //蓝牙开始播放事件
    BT_EVT_STOP,                                //蓝牙停止播放事件
    BT_EVT_INCOMING,                            //来电事件
    BT_EVT_OUTGOING,                            //去电事件
    BT_EVT_CALL,                                //建立通话
};

//标准HID键
#define HID_KEY_ENTER       0x28

//自定义HID键
#define HID_KEY_VOL_DOWN    0xE8
#define HID_KEY_VOL_UP      0xE9



typedef struct __TYPE_BT_CFG {
    u8 profile;                                 //Profile控制，打开的SDP服务

    u8 connect_times;                           //上电回连次数
    u8 timeout_connect_times;                   //远距离断开回边次数

} TYPE_BT_CFG;

extern IAR_BIT_A bool flag_btmode;              //为1时启用完整的蓝牙功能
extern TYPE_BT_CFG bt_cfg;
extern u8 Test_Mode_Flag;

void btmode_enter(void);                        //进入到蓝牙模式下
void btmode_exit(void);                         //退出蓝牙模式

void sbc_init(void);

void bt_init(void);                             //初始化蓝牙变量
void bt_reset(void);                            //蓝牙模块复位
void bt_off(void);                              //关闭蓝牙模块
void bt_event(void);                            //蓝牙任务所必要的事件
void bt_notice(u8 evt);                         //蓝牙事件通知，事件详见 BT_EVT 枚举
void bt_close(void);

bool btdec_play(void);                          //蓝牙解码器开始解码工作。与手机的蓝牙播放状态无关。启动失败则返回false
void btdec_stop(void);                          //停止蓝牙解码器。与手机的蓝牙播放状态无关

u8 bt_get_status(void);                         //获取蓝牙的当前状态
u8 bt_get_call_status(void);
__near_func u8 bt_testmode(void);               //判断当前蓝牙是否处于测试模式
void bt_set_scan(bool scan, bool link);         //设置发现与连接状态。inquiry 可被发现；page 可被连接
void bt_play(void);                             //蓝牙音乐播放
void bt_pause(void);                            //蓝牙音乐暂停
void bt_play_pause(void);                       //蓝牙音乐播放/暂停
void bt_stop(void);                             //蓝牙音乐停止
void bt_prev(void);                             //上一曲
void bt_next(void);                             //下一曲
void bt_redial(void);                           //电话重拨
void bt_pickup(void);                           //接听电话
void bt_hangup(void);                           //挂断电话
void bt_callswap(void);
void bt_test_vendor(u8 key_val);                //按键测试
bool bt_hid_key(u8 val);                        //发送一个标准HID键, 如Enter
bool bt_hid_consumer(u8 val);                   //发送一个自定义HID键, 如VOL+ VOL-
void bt_rssi_update(void);                      //发送命令更新RSSI值（需要一定时间才能更新，建议1S发送一次）
u8   bt_rssi_get(void);                         //获取当前的RSSI值

void bt_connect(void);                          //蓝牙设备回连
void bt_disconnect(void);                       //蓝牙设备断开
void bt_a2dp_connect(void);                     //蓝牙A2DP连接
void bt_a2dp_disconnect(void);                  //蓝牙A2DP断开
void bt_hid_connect(void);                      //蓝牙HID连接
bool bt_hid_wait_connect(void);
void bt_hid_disconnect(void);                   //蓝牙HID断开
bool bt_hid_wait_disconnect(void);

bool bt_sco_isconnected(void);                  //SCO是否已连接
bool bt_sco_connect(void);                      //连接SCO通道（切换到音箱接听）
bool bt_sco_disconnect(void);                   //断开SCO通道（切换到私密接听）
void bt_sco_switch(void);                       //私密接听功能，SPK、手机听筒切换
void bt_enter_call(void);                       //进入通话
void bt_exit_call(void);                        //退出通话
__near_func bool bt_in_call(void);              //判断是否已进入通话

bool bt_send_atcmd(const char * cmd);           //发送AT命令
bool bt_send_spp(const char * cmd);             //发送SPP命令
bool bt_send_spp_ex(const char * cmd, u8 len);  //带长度的发送SPP命令

void PhonebookStop();                           //中止当前电话本信息获取
bool IsPBAPInIdleMode(void);                    //判断PBAP电话本获取进程是否处于空闲状态
bool phonebook_get_start(u8 phone_sim_index);   //开始获取电话本信息

/*****************************************************************************
 * CallBack  : 蓝牙库回调函数接口
 *****************************************************************************/
u8 bt_use_ev3(void);                            //是否使用EV3，返回0为HV3，1为EV3，2为2-EV3
void AVRCPChangeVolume(void);                   //AVRCP改变手机端音量
void a2dpvol_change_hook(u8 mode);              //A2DP手机端改变媒体音量接口，一般用于功能机
void a2dpvol_set_hook(u8 vol);                  //A2DP设置音量接口，一般用于功能机
u8 a2dpvol_get_hook(void);                      //A2DP获取当音音量接口
void bt_test_freq(void);                        //执行频偏测试处理，根据实际情况进行IO信号输出
void hcc_vendor_settime(u8 *buf);               //通过测试盒设置时间
void bt_notice(u8 evt);                         //蓝牙事件通知
void hshf_rx_atcmd(u8 *cmd);                    //蓝牙端获取手机端拨出的号码接口

void bt_save_link_key(u8 *addr, u8 *link_key);  //保存地址及LINK KEY
bool bt_get_link_key(u8 *addr, u8 *link_key);   //根据地址，获取回连的KEY值
void bt_delete_link(u8 *addr);                  //删除回连信息
bool bt_get_linkaddr(u8 *addr);                 //获取最新的回连地址


#endif
