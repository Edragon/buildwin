/*****************************************************************************
 * Module    : API
 * File      : api_bt.h
 * Author    : Hanny
 * Function  : �������API�ӿ�
 *****************************************************************************/
#ifndef API_BT_H
#define API_BT_H

#define BT_SLEEP_CNT                    8           //BTģ�鳬��8��û�յ����ݣ�����Ϊ����˯��

//����״̬����bt_get_status��������ȡ
enum {
    BT_STA_OFF,                                 //����ģ���ѹر�
    BT_STA_IDLE,                                //δ����
    BT_STA_SLEEP,                               //�͹���

    BT_STA_DISCONNECTING,                       //�Ͽ���
    BT_STA_CONNECTING,                          //������

    BT_STA_CONNECTED,                           //������
    BT_STA_PLAYING,                             //����
    BT_STA_INCOMING,                            //��������
    BT_STA_OUTGOING,                            //���ں���
    BT_STA_PHONE,                               //ͨ����
};

enum {
    BT_EVT_DISCONNECT,                          //�����Ͽ��¼�
    BT_EVT_LOSTCONNECT,                         //����Զ�������Ӷ�ʧ�¼�
    BT_EVT_CONNECT,                             //���������¼�
    BT_EVT_CONNECTED,                           //�������ӳɹ��¼�
    BT_EVT_PLAY,                                //������ʼ�����¼�
    BT_EVT_STOP,                                //����ֹͣ�����¼�
    BT_EVT_INCOMING,                            //�����¼�
    BT_EVT_OUTGOING,                            //ȥ���¼�
    BT_EVT_CALL,                                //����ͨ��
};

//��׼HID��
#define HID_KEY_ENTER       0x28

//�Զ���HID��
#define HID_KEY_VOL_DOWN    0xE8
#define HID_KEY_VOL_UP      0xE9



typedef struct __TYPE_BT_CFG {
    u8 profile;                                 //Profile���ƣ��򿪵�SDP����

    u8 connect_times;                           //�ϵ��������
    u8 timeout_connect_times;                   //Զ����Ͽ��رߴ���

} TYPE_BT_CFG;

extern IAR_BIT_A bool flag_btmode;              //Ϊ1ʱ������������������
extern TYPE_BT_CFG bt_cfg;
extern u8 Test_Mode_Flag;

void btmode_enter(void);                        //���뵽����ģʽ��
void btmode_exit(void);                         //�˳�����ģʽ

void sbc_init(void);

void bt_init(void);                             //��ʼ����������
void bt_reset(void);                            //����ģ�鸴λ
void bt_off(void);                              //�ر�����ģ��
void bt_event(void);                            //������������Ҫ���¼�
void bt_notice(u8 evt);                         //�����¼�֪ͨ���¼���� BT_EVT ö��
void bt_close(void);

bool btdec_play(void);                          //������������ʼ���빤�������ֻ�����������״̬�޹ء�����ʧ���򷵻�false
void btdec_stop(void);                          //ֹͣ���������������ֻ�����������״̬�޹�

u8 bt_get_status(void);                         //��ȡ�����ĵ�ǰ״̬
u8 bt_get_call_status(void);
__near_func u8 bt_testmode(void);               //�жϵ�ǰ�����Ƿ��ڲ���ģʽ
void bt_set_scan(bool scan, bool link);         //���÷���������״̬��inquiry �ɱ����֣�page �ɱ�����
void bt_play(void);                             //�������ֲ���
void bt_pause(void);                            //����������ͣ
void bt_play_pause(void);                       //�������ֲ���/��ͣ
void bt_stop(void);                             //��������ֹͣ
void bt_prev(void);                             //��һ��
void bt_next(void);                             //��һ��
void bt_redial(void);                           //�绰�ز�
void bt_pickup(void);                           //�����绰
void bt_hangup(void);                           //�Ҷϵ绰
void bt_callswap(void);
void bt_test_vendor(u8 key_val);                //��������
bool bt_hid_key(u8 val);                        //����һ����׼HID��, ��Enter
bool bt_hid_consumer(u8 val);                   //����һ���Զ���HID��, ��VOL+ VOL-
void bt_rssi_update(void);                      //�����������RSSIֵ����Ҫһ��ʱ����ܸ��£�����1S����һ�Σ�
u8   bt_rssi_get(void);                         //��ȡ��ǰ��RSSIֵ

void bt_connect(void);                          //�����豸����
void bt_disconnect(void);                       //�����豸�Ͽ�
void bt_a2dp_connect(void);                     //����A2DP����
void bt_a2dp_disconnect(void);                  //����A2DP�Ͽ�
void bt_hid_connect(void);                      //����HID����
bool bt_hid_wait_connect(void);
void bt_hid_disconnect(void);                   //����HID�Ͽ�
bool bt_hid_wait_disconnect(void);

bool bt_sco_isconnected(void);                  //SCO�Ƿ�������
bool bt_sco_connect(void);                      //����SCOͨ�����л������������
bool bt_sco_disconnect(void);                   //�Ͽ�SCOͨ�����л���˽�ܽ�����
void bt_sco_switch(void);                       //˽�ܽ������ܣ�SPK���ֻ���Ͳ�л�
void bt_enter_call(void);                       //����ͨ��
void bt_exit_call(void);                        //�˳�ͨ��
__near_func bool bt_in_call(void);              //�ж��Ƿ��ѽ���ͨ��

bool bt_send_atcmd(const char * cmd);           //����AT����
bool bt_send_spp(const char * cmd);             //����SPP����
bool bt_send_spp_ex(const char * cmd, u8 len);  //�����ȵķ���SPP����

void PhonebookStop();                           //��ֹ��ǰ�绰����Ϣ��ȡ
bool IsPBAPInIdleMode(void);                    //�ж�PBAP�绰����ȡ�����Ƿ��ڿ���״̬
bool phonebook_get_start(u8 phone_sim_index);   //��ʼ��ȡ�绰����Ϣ

/*****************************************************************************
 * CallBack  : ������ص������ӿ�
 *****************************************************************************/
u8 bt_use_ev3(void);                            //�Ƿ�ʹ��EV3������0ΪHV3��1ΪEV3��2Ϊ2-EV3
void AVRCPChangeVolume(void);                   //AVRCP�ı��ֻ�������
void a2dpvol_change_hook(u8 mode);              //A2DP�ֻ��˸ı�ý�������ӿڣ�һ�����ڹ��ܻ�
void a2dpvol_set_hook(u8 vol);                  //A2DP���������ӿڣ�һ�����ڹ��ܻ�
u8 a2dpvol_get_hook(void);                      //A2DP��ȡ���������ӿ�
void bt_test_freq(void);                        //ִ��Ƶƫ���Դ�������ʵ���������IO�ź����
void hcc_vendor_settime(u8 *buf);               //ͨ�����Ժ�����ʱ��
void bt_notice(u8 evt);                         //�����¼�֪ͨ
void hshf_rx_atcmd(u8 *cmd);                    //�����˻�ȡ�ֻ��˲����ĺ���ӿ�

void bt_save_link_key(u8 *addr, u8 *link_key);  //�����ַ��LINK KEY
bool bt_get_link_key(u8 *addr, u8 *link_key);   //���ݵ�ַ����ȡ������KEYֵ
void bt_delete_link(u8 *addr);                  //ɾ��������Ϣ
bool bt_get_linkaddr(u8 *addr);                 //��ȡ���µĻ�����ַ


#endif
