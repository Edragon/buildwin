/*****************************************************************************
 * Module    : User
 * File      : user_bt.c
 * Author    : Hanny
 * Function  : �����û�API�ӿ�
 *****************************************************************************/
#include "include.h"
#include "task.h"

#define DBG_BT_PARAM(...)           printf(__VA_ARGS__)

#pragma constseg="BT_API_CONST"
//RF ������
IAR_CONST u8 tbl_rfparam[64] = {
    //����RF��Ϣ
    0x0F,0x40,0x8F,0x41,0x04,0x06,0x04,0x04,0x00,0x80,0x00,0x00,0x80,0x01,0x20,0xD6,
    0x93,0x00,0xF7,0x3F,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x06,0x70,
    0xFF,0x1E,0x2C,0x1F,0x06,0x42,0x14,0x7E,0x00,0x04,0x07,0x0E,0x1E,0xF8,0x00,0x01,
    0x00,0x00,0xF4,0xF1,0x0C,0xC3,0x00,0x16,0x44,0x00,0x29,0x1C,0x1C,0x24,0xFF,0xFE
};

//RF ������
IAR_CONST u8 tbl_rfparam_2[64] = {
    //����RF��Ϣ //�Ż�GFSK��IQ����
    0x0F,0x40,0xB2,0x41,0x04,0x06,0xA0,0x40,0x00,0x81,0x00,0x00,0x80,0x01,0x20,0xD6,
    0x93,0x00,0xF7,0x3F,0x00,0x00,0xBD,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,0x80,0xA3,
    0x01,0x11,0xAC,0x18,0x93,0x03,0x64,0x7C,0x00,0x04,0x07,0xD8,0x14,0x28,0x00,0x01,
    0x00,0x80,0x04,0x7A,0x0C,0xC3,0x00,0x76,0x01,0x00,0x29,0x1C,0x1C,0x24,0x01,0x01
};

//����Ĭ������
IAR_CONST char tbl_bt_name[] = "BW-BTBOX";
IAR_CONST char tbl_bt_hid_name[] = "BT-Shutter";

#pragma constseg="BT_INIT_CONST"
IAR_CONST char str_bt_init_addr[] = "BD:<%02x:%02x:%02x:%02x:%02x:%02x> - ";
IAR_CONST char str_debug_rfinfo[] = "BTDev<%02x:%02x:%02x:%02x:%02x:%02x>: %s\n";

#pragma constseg="BT_LINK_KEY_CONST"
IAR_CONST char str_get_addr[] = "get addr\n";
IAR_CONST char str_get_linkaddr[] = "latest: %02x:%02x:%02x:%02x:%02x:%02x\n";
IAR_CONST char str_delete_link[] = "delete: %02x:%02x:%02x:%02x:%02x:%02x\n";
IAR_CONST char str_get_link_key[] = "get: %02x:%02x:%02x:%02x:%02x:%02x\n";
IAR_CONST char str_save_link_key[] = "save: %02x:%02x:%02x:%02x:%02x:%02x\n";

#pragma constseg=default
#pragma constseg="BT_RFC_TX_CONST"
IAR_CONST char at_cmd0_ptr[] = "AT+IPHONEACCEV=1,1,4\r";
#pragma constseg=default

#define BT_FT_APPLE_DEVICE          0x01
#define BT_FT_APPLE_VOL_CTRL        0x02
#define BT_FT_HSF_3WCALL            0x04
#define BT_FT_APPLE_DEVICE_EN       0x00    //�Ƿ�֧��ƻ��������ʾ��0x80=֧�֣�0x00=��֧��
#define BT_FT_APPLE_VOL_CTRL_EN     0x00    //�Ƿ�֧��ƻ���������ƣ�0x40=֧�֣�0x00=��֧��
#define BT_FT_HSF_3WCALL_EN         0x20    //�Ƿ�֧��3��ͨ�����ƣ�0x20=֧�֣�0x00=��֧��
#define IS_APPLE_BT_DEVICE          (BT_FT_APPLE_DEVICE_EN && (bt_user_feature & BT_FT_APPLE_DEVICE))
#define IS_APPLE_VOL_CTRL           (BT_FT_APPLE_VOL_CTRL_EN && (bt_user_feature & BT_FT_APPLE_VOL_CTRL))

//ʹ��bt_paramд����ʱ��ֻ֧��writepage
IAR_XDATA_A TYPE_BT_PARAM_PAIR bt_param @ "BT_PAIR_PARAM";
IAR_XDATA_A u8 bt_param_sum[4] @ "BT_PAIR_PARAM";
IAR_BIT_A bool task_hid_flag;

extern u8 bt_power_up();
bool btdec_load_code(u16 addr, u8 segnum);
extern u8 bt_user_feature;
bool hcmd_write_xdata(u16 addr, u8 val);   // for RF optimization
IAR_XDATA_A u8  bt_mic_param[10] @ "BT_MIC_PARAM";
IAR_XDATA_A u16 noise_value      @ "BT_MIC_PARAM";
IAR_XDATA_A u16 far_delay        @ "BT_MIC_PARAM";
IAR_XDATA_A u16 echo_atten       @ "BT_MIC_PARAM";
IAR_XDATA_A u8  bt_digital_gain  @ "BT_MIC_PARAM";
IAR_XDATA_A u8  bt_analog_gain   @ "BT_MIC_PARAM";

// �����������
u8 EnableSimplePairMode = 1;   //0x00:disable, 0x01:enable simple pairing
u8 btpincode[4]={'0','0','0','0'};  // PinCode: "0000"

#if 1   // for two rf power
void hcmd_write_rfreg(u8 addr, u16 value);
void switch_rf_power(u8 mode)
{
    if(get_chip_type() == 2) {
        if(mode){
            hcmd_write_rfreg(16, 0xA380);   // �����ֵ�ʱ��
        } else{
            hcmd_write_rfreg(16, 0x8380);   // ͨ�����豸������ʱ��
        }
    }
}
#endif

#if 0
#pragma location="BT_PATCH_CODE"    // must placed @ BT_PATCH_CODE
void bt_patch_rxvga(u8 *tune)
{
// 0: u8 m_tune_enable = 0x01;   // default(0x01)
// 1: u8 rssi_up_threshold;      // default(0x1E)
// 2: u8 rssi_down_threshold;    // default(0x22)
// 3: u8 tune_vag_0              // default(0x14)
// 4: u8 tune_vag_1              // default(0x1A)
    tune[0] = 0x01;    // enble
    tune[1] = 0x1E;
    tune[2] = 0x22;
    tune[3] = 0x14;
    tune[4] = 0x1A;
}
#endif

#pragma location="BT_API_CMD"
u8 char_to_hex(u8 c)
{
    if(c >= 'A' && c <= 'F') {
        c -= 'A' + 10;
    } else if(c >= 'a' && c <= 'f') {
        c -= 'a' + 10;
    } else if(c >= '0' && c <= '9') {
        c -= '0';
    } else {
        c = 0xff;
    }
    return c;
}

//0~15ת���ַ��������spi_bt_rfinfo_read��ͬ��bank
#pragma location="BT_API_CMD"
char hex_to_char(u8 num)
{
    if (num >= 10) {
        return ('A' + num - 10);
    } else {
        return ('0' + num);
    }
}

//���ñ��豸��������ַ
#pragma location="BT_API_CMD"
void btinfo_get_addr(u8 *ptr)
{
    sysinfo_load(ptr, BT_RFINFO_SET, 1);                //��ȡ����
    if (ptr[0] & BIT(BT_RFINFO_SET_ADDR)) {
        if (ptr[0] & BIT(BT_RFINFO_SET_DYADDR)) {
            //ʹ�ö�̬��ַ
            U32_ACCESS(ptr) = sys_param_read32(PARAM_RANDOM_KEY);
            ptr[4] = 0x23;
            ptr[5] = 0x30;
        } else {
            //ʹ�����úõĵ�ַ��Ϣ
            sysinfo_load(ptr, BT_RFINFO_ADDR, 6);       //���ص�ַ
        }
    } else {
        //ʹ�ö�̬��ַ
        U32_ACCESS(ptr) = sys_param_read32(PARAM_RANDOM_KEY);
        ptr[4] = 0x21;
        ptr[5] = 0x30;
    }
#if TASK_BTHID_EN
    if(task_hid_flag) {
        ptr[5] ^= BIT(0);
    }
#endif
    //printf("MAC: ");
    //printf_array(ptr, 6);
}

//���ñ��豸����������
#pragma location="BT_API_CMD"
void btinfo_get_name(u8 *ptr)
{
#if TASK_BTHID_EN
    if(task_hid_flag) {
        memcpy(ptr, (char *)tbl_bt_hid_name, sizeof(tbl_bt_hid_name));
    } else
#endif
    {
        sysinfo_load(ptr, BT_RFINFO_SET, 1);                //��ȡ����
        if (ptr[0] & BIT(BT_RFINFO_SET_NAME)) {
            sysinfo_load(ptr, BT_RFINFO_NAME, 32);
        } else {
            memcpy(ptr, (char *)tbl_bt_name, sizeof(tbl_bt_name));
        }
    }
#if BT_NAME_WITH_ADDR
    u8 i;
    u8 temp_addr[6];
    btinfo_get_addr(temp_addr);
    for(i=0; i<32-5; i++) {
        if (ptr[i] == 0) {
            *((u8 *)&ptr[i]) = '-';
            *((u8 *)&ptr[i+1]) = hex_to_char(temp_addr[1]&0xf);
            *((u8 *)&ptr[i+2]) = hex_to_char((temp_addr[0]>>4)&0xf);
            *((u8 *)&ptr[i+3]) = hex_to_char(temp_addr[0]&0xf);
            *((u8 *)&ptr[i+4]) = 0;
            break;
        }
    }
#endif
}

//���ñ��豸��������ַ
#pragma location="BT_API_CMD"
void btinfo_get_rfparam(u8 *ptr)
{
    sysinfo_load(ptr, BT_RFINFO_ADDR, 6);
    memcpy(ptr, (u8 *)tbl_rfparam, 64);
}

//���ñ��豸��������ַ2
#pragma location="BT_API_CMD"
void btinfo_get_rfparam_2(u8 *ptr)
{
    sysinfo_load(ptr, BT_RFINFO_ADDR, 6);
    memcpy(ptr, (u8 *)tbl_rfparam_2, 64);
}

//���ñ��豸��mic����
#pragma location="BT_API_CMD"
void btinfo_get_mic(u8 *ptr)
{
    sysinfo_load(ptr, BT_RFINFO_SET, 1);                //��ȡ����
    if (ptr[0] & BIT(BT_RFINFO_SET_MIC)) {
        sysinfo_load(ptr, BT_RFINFO_MIC, 2);
        bt_analog_gain = ptr[0];
        bt_digital_gain = ptr[1];
    } else {
        bt_analog_gain = BT_ANA_GAIN;
        bt_digital_gain = BT_DIGIT_GAIN;
    }
}

//���ñ��豸��ͨ������
#pragma location="BT_API_CMD"
void btinfo_get_voice(u8 *ptr)
{
    sysinfo_load(ptr, BT_RFINFO_SET, 1);                //��ȡ����
    if (ptr[0] & BIT(BT_RFINFO_SET_VOICE)) {
        sysinfo_load(ptr, BT_RFINFO_VOICE, 8);
        noise_value = (ptr[1] << 8) | ptr[0];
        far_delay = (ptr[5] << 8) | ptr[4];
        echo_atten = ((ptr[7] << 8) | ptr[6]);
    } else {
        noise_value = BT_NOISE_VALUE;
        far_delay = BT_FAR_DELAY;
        echo_atten = BT_ECHO_ATTEN;
    }
}

//���ñ��豸��PINCODE
#pragma location="BT_API_CMD"
void btinfo_get_pincode(u8 *ptr)
{
    smemcpy(ptr, btpincode, 4);
}

void BT_PairingResult(u8 re)
{
	//re=0 pairing success
	//re=other  pairing fail
	//printf("pa=%d\n",re);
}

#pragma location="BT_RFC_TX"
bool send_bat_to_iphone(u8 batval)
{
    ((u8*)at_cmd0_ptr)[19] = batval+'0';
    return bt_send_atcmd((char *)at_cmd0_ptr);
}

#pragma location="BT_RFC_TX"
void send_bat_to_iphone_hook(void)
{
   // printf("%02x ",bt_user_feature);
    if(sys_ctl.connected && IS_APPLE_BT_DEVICE) {                //ƻ���豸��ʾ����
        if(sys_ctl.bat_val != t_bt.disp_dev_bat) {
          //  printf("bat:%d\n",sys_ctl.bat_val);
            if(send_bat_to_iphone(sys_ctl.bat_val)) {
              //  printf("d ");
                t_bt.disp_dev_bat = sys_ctl.bat_val;
            }
        }
    }
}

#pragma location="BT_INIT_SEG"
void user_bt_init(void)
{
    //�򿪵�ģ��
#if TASK_BTHID_EN
    if(task_hid_flag) {
        bt_cfg.profile = (0xC0 * BT_HID_EN);
    } else {
        bt_cfg.profile = (0x20 * BT_SPP_EN) | (0x18 * BT_HSHF_EN) | (0x7 * BT_A2DP_EN);
    }
#else
    bt_cfg.profile = (0xC0 * BT_HID_EN) | (0x20 * BT_SPP_EN) | (0x18 * BT_HSHF_EN) | (0x7 * BT_A2DP_EN);
#endif

    //��������
    bt_cfg.connect_times = BT_POWER_ON_CONN_TIMES;

    //Զ����Ͽ���������
    bt_cfg.timeout_connect_times = BT_TIME_OUT_CONN_TIMES;
#if BT_3WCALL_EN
    bt_user_feature = BT_FT_APPLE_DEVICE_EN | BT_FT_HSF_3WCALL_EN;
#else
    bt_user_feature = BT_FT_APPLE_DEVICE_EN;
#endif
    bt_user_feature |= BT_FT_APPLE_VOL_CTRL_EN;

    //��ʼ������ģ���õ��ı���
    Test_Mode_Flag =0;
    bt_init();
    bt_set_scan(1, 1);                      //�ɱ����֣��ɱ�����

    //���¿�ʼ��λ����ģ��
    BTLDO_EN();

    btdec_load_code(0x10, 3);
    bt_uart_init();
    //P3DIR |= BIT(3);  // CTS
    memset((u8 *)0x8900, 0, 0x1600);        //���BT������
    btmode_enter();
    bt_reset();

    hcmd_write_xdata(0x0207, 0x00);
    hcmd_write_xdata(0x0208, 0xFB);
    hcmd_write_xdata(0x0209, 0x00);
    hcmd_write_xdata(0x020A, 0x40);

    bt_play_status = 0;
    btinfo_get_mic(bt_mic_param);
    btinfo_get_voice(bt_mic_param+2);

#if 1
    extern u8 bt_temp_buf[0x100];
    btinfo_get_addr(bt_temp_buf);
    btinfo_get_name(bt_temp_buf+6);
    printf(str_debug_rfinfo, bt_temp_buf[5],bt_temp_buf[4],bt_temp_buf[3],bt_temp_buf[2],bt_temp_buf[1],bt_temp_buf[0], bt_temp_buf+6);
#endif
    btmode_exit();

#if BT_AUTO_PLAY
    sys_ctl.bt_auto_play_flag = 0;
    sys_param_write8(PARAM_BT_AUTO_PLAY, sys_ctl.bt_auto_play_flag);
#endif
}

void user_bt_off(void)
{
    bt_off();
    sys_ctl.connected = 0;
}

void user_bt_event(void)
{
    bt_event();
    sys_ctl.bt_sta = bt_status_get();
    if (sys_ctl.connected) {
        if (sys_ctl.bt_sta <= BT_STA_IDLE) {
            sys_ctl.connected = 0;
            put_msg(Q_BT_DISCONN_DONE);
        }
    } else {
        if (sys_ctl.bt_sta >= BT_STA_CONNECTED) {
            sys_ctl.connected = 1;
            put_msg(Q_BT_CONN_DONE);
        }
    }
}

#pragma constseg="PHONE_CALL_NUM"
IAR_CONST u8 phone_call_number[32] = {0};
#pragma constseg=default
// u8 call[]="AT+CLCC";
// hsf_send_atcmd(call, sizeof(call));
/*----------------------------------------------------------------------------*/
/*@brief: �����˻�ȡ�ֻ��˲����ĺ���ӿ�
//@param: cmdΪָ���ֻ����������ĺ�������ָ�����
//@return:��
//@note:  ���ֻ��˲�����������hsf_send_atcmd(u8 *str, u16 len)���������
          ����str�ַ���Ϊ"AT+CLCC"��lenΪstr�ַ����ĳ��ȣ��綨��
          u8 call_phone[]="AT+CLCC";�����hsf_send_atcmd(call_phone, sizeof(call_phone));��
          �ڿ�������Զ�����hshf_rx_atcmd(u8 *cmd)���ڴ˺�������������õ��ֻ�
          �˲����ĺ��뱣�浽phone_call_number[32]�����������.
*/
/*----------------------------------------------------------------------------*/
#pragma location="BT_API_1"
void hshf_rx_atcmd(u8 *cmd)
{
    if(cmd[1] == 'C' && cmd[2] == 'L' && cmd[3] == 'C' && cmd[4] == 'C')
    {   // +CLCC: 1,0,0,0,0,"10086"
        u8 *ptr = &cmd[17];
        u8 size_of_call_number = 0;
        if(*ptr == '\"')
        {
            do{
                ptr++;
                size_of_call_number ++;
            }while(*ptr && *ptr != '\"');
            *ptr = '\0';
            //printf("NUM:%s\n", &cmd[18]);   // get number

            size_of_call_number --;
            //printf("size_of_call_number: %d\n",size_of_call_number);
            memset((u8 *)&phone_call_number, 0, sizeof(phone_call_number));
            memcpy((u8 *)&phone_call_number, (u8 *)&cmd[18], size_of_call_number);
            //printf("NUM:%s\n", phone_call_number);
        }
    }
}

#if BT_ID3_EN
void bt_rx_id3_hook(u8 *ptr)
{
    u16 idx;
    u16 total = ((u16)ptr[0]<<8) + ptr[1];     //ptr[0:1]=total_len

    if(total > 1+4) {
        u8 num = ptr[2];    //ptr[2]=num_of_attr
        total -= 1;
        ptr += 2+1;
        idx = 3;
        while(num && idx<total) {
            //u16 char_set = ((u16)ptr[idx+1]<<8) + ptr[idx+2];  //0x6a = UTF-8
            u16 attr_len = ((u16)ptr[idx+3]<<8) + ptr[idx+4];

            switch(ptr[idx]) {
            case 0x01://title
                //printf("title:%s\n", ptr+idx+5);
                //printf_array(ptr+idx+5, attr_len);
                break;
            case 0x02://artist
                //printf("artist:%s\n", ptr+idx+5);
                //printf_array(ptr+idx+5, attr_len);
                break;
            case 0x03://album
                //printf("album:%s\n", ptr+idx+5);
                //printf_array(ptr+idx+5, attr_len);
                break;
            }
            idx += 4+2+2+attr_len;
            num--;
        }
    }
}
#endif

#if BT_PLAY_TIME_EN
#pragma location="BT_AVCTP_RX"
void bt_play_time_hook(u8 *ptr)
{
    u8 tmp = *ptr;
    *ptr = *(ptr+3);
    *(ptr+3) = tmp;

    tmp = *(ptr+1);
    *(ptr+1) = *(ptr+2);
    *(ptr+2) = tmp;

    //printf("%d:%d\n", (u8)(*(u32 *)ptr/1000/60), (u8)(*(u32 *)ptr/1000%60));
}
#endif

//�������µ�һ����ַ���
u8 bt_set_latest_num(u8 latest)
{
    return latest;
}

//��ȡ���µ�һ����ַ���
u8 bt_get_latest_num(void)
{
    u8 num = bt_param_read8(BT_PARAM_LATEST);
    if (num > 4) {
        num = 0;
    }
    return num;
}

//�������µ�����
void bt_set_order(u8 order[], u8 latest)
{
    u8 i;
    u8 n = latest - 1;
    u8 old = order[n];
    if (old > 4) {
        old = 0;
    }
    for (i=0; i!=4; i++) {
        if (order[i] && order[i] <= old) {
            order[i]++;
        }
    }
    order[n] = 1;
}

//���ݵ�ַ����ȡ��š��Ҳ�����Ӧ��ַʱ������0
u8 bt_get_addr_num(u8 *addr)
{
    u8 i;
    TYPE_BT_PAIR *pair;
    bt_param_read(&bt_param, 0, sizeof(bt_param));      //������Page���ݣ�����SPI_PARAM_DATA�ռ�
    pair = &bt_param.pair[0];
    for (i=1; i!=5; i++,pair++) {
        if (pair->valid && (0 == memcmp(pair->addr, addr, 6))) {
            return i;
        }
    }
    return 0;
}

//����һ���µ�ַ
u8 bt_alloc_num(u8 order[])
{
    u8 m = 0;
    u8 j = 0;
    u8 i;
    for (i=0; i!=4; i++) {
        if (order[i] == 0) {
            return i+1;
        }
        if (order[i] > m) {
            m = order[i];
            j = i;
        }
    }
    return j+1;
}

//��ȡ���µĻ�����ַ
#pragma location="BT_LINK_KEY_SEG"
bool bt_get_linkaddr(u8 *addr)
{
    DBG_BT_PARAM(str_get_addr);
    bt_param_read(&bt_param, 0, sizeof(bt_param));
    //printf_array(&bt_param, sizeof(bt_param));
#if TASK_BTHID_EN
    if(task_hid_flag) {
        TYPE_BT_PAIR2 *pair2;
        bt_param_read(&bt_param, 0, sizeof(bt_param));      //������Page���ݣ�����SPI_PARAM_DATA�ռ�
        pair2 = &bt_param.pair2[0];
        if (!pair2->valid) {
            return false;
        }
        memcpy(addr, pair2->addr, 6);
    } else
#endif
    {
        u8 l_num = bt_get_latest_num();
        u8 n;
        if (0 == l_num) {                               //������ַ�����Ч
            return false;
        }
        n = l_num - 1;
        if (0 == bt_param_read8(BT_PAIR_VALID(n))) {    //������Ϣ��Ч
            return false;
        }
        bt_param_read(addr, BT_PAIR_ADDR(n), 6);
    }
    DBG_BT_PARAM(str_get_linkaddr, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    return true;
}

//ɾ��������Ϣ
#pragma location="BT_LINK_KEY_SEG"
void bt_delete_link(u8 *addr)
{
    DBG_BT_PARAM(str_delete_link, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
#if TASK_BTHID_EN
    if(task_hid_flag) {
        TYPE_BT_PAIR2 *pair2;
        bt_param_read(&bt_param, 0, sizeof(bt_param));      //������Page���ݣ�����SPI_PARAM_DATA�ռ�
        pair2 = &bt_param.pair2[0];
        if (!pair2->valid || (0 != memcmp(pair2->addr, addr, 6))) {
            return;
        }
        memset(pair2, 0, sizeof(TYPE_BT_PAIR2));
    } else
#endif
    {
        TYPE_BT_PAIR *pair;
        u8 n = bt_get_addr_num(addr);                   //�Ȳ����Ƿ��иõ�ַ
        if (n == 0) {
            return;
        }
        u8 l_num = bt_get_latest_num();
        bt_param_read(&bt_param, 0, sizeof(bt_param));  //������Page���ݣ�����SPI_PARAM_DATA�ռ�
        pair = &bt_param.pair[n-1];
        if (l_num == n) {
            bt_param.latest = bt_set_latest_num(0);
        }
        bt_param.order[n-1] = 0;
        memset(pair, 0, sizeof(TYPE_BT_PAIR));
    }
    bt_param_writepage(&bt_param);                  //ʹ��bt_paramʱ��ֻ֧��writepage
}

//���ݵ�ַ����ȡ������KEYֵ
#pragma location="BT_LINK_KEY_SEG"
bool bt_get_link_key(u8 *addr, u8 *link_key)
{
    DBG_BT_PARAM(str_get_link_key, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
#if TASK_BTHID_EN
    if(task_hid_flag) {
        TYPE_BT_PAIR2 *pair2;
        bt_param_read(&bt_param, 0, sizeof(bt_param));      //������Page���ݣ�����SPI_PARAM_DATA�ռ�
        pair2 = &bt_param.pair2[0];
        if (!pair2->valid || (0 != memcmp(pair2->addr, addr, 6))) {
            return false;
        }
        memcpy(link_key, pair2->link_key, 16);
    } else
#endif
    {
        u8 n = bt_get_addr_num(addr);                   //�Ȳ����Ƿ��иõ�ַ
        if (n == 0) {
            return false;
        }
        bt_param_read(link_key, BT_PAIR_LINK_KEY(n-1), 16);
        u8 l_num = bt_get_latest_num();
        if (l_num != n) {
            bt_param_read(&bt_param, 0, sizeof(bt_param));  //������Page���ݣ�����SPI_PARAM_DATA�ռ�
            bt_param.latest = bt_set_latest_num(n);
            bt_set_order(bt_param.order, n);
            bt_param_writepage(&bt_param);                  //ʹ��bt_paramʱ��ֻ֧��writepage
        }
    }
    //printf_array(link_key, 16);
    return true;
}

//�����ַ��LINK KEY
#pragma location="BT_LINK_KEY_SEG"
void bt_save_link_key(u8 *addr, u8 *link_key)
{
    DBG_BT_PARAM(str_save_link_key, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    //printf_array(link_key, 16);

#if TASK_BTHID_EN
    if(task_hid_flag) {
        TYPE_BT_PAIR2 *pair2;
        bt_param_read(&bt_param, 0, sizeof(bt_param));      //������Page���ݣ�����SPI_PARAM_DATA�ռ�
        pair2 = &bt_param.pair2[0];
        pair2->valid = 1;
        memcpy(pair2->addr, addr, 6);
        memcpy(pair2->link_key, link_key, 16);
	} else
#endif
    {
        TYPE_BT_PAIR *pair;
        u8 n = bt_get_addr_num(addr);                   //�Ȳ����Ƿ��иõ�ַ
        bt_param_read(&bt_param, 0, sizeof(bt_param));  //������Page���ݣ�����SPI_PARAM_DATA�ռ�
        if (n == 0) {
            n = bt_alloc_num(bt_param.order);
        }
        pair = &bt_param.pair[n-1];
        bt_set_order(bt_param.order, n);
        bt_param.latest = bt_set_latest_num(n);
        pair->valid = 1;
        memcpy(pair->addr, addr, 6);
        memcpy(pair->link_key, link_key, 16);
    }
    bt_param_writepage(&bt_param);                  //ʹ��bt_paramʱ��ֻ֧��writepage


    //printf_array(&bt_param, sizeof(bt_param));
}

//���·�װ����״̬��ȡ
u8 bt_status_get(void)
{
    u8 sta = bt_get_status();
    if (sta == BT_STA_PLAYING || sta == BT_STA_CONNECTED) {
        if(TASK_BT == task_ctl.work_sta){
            if (bt_play_status) {
                sta = BT_STA_PLAYING;
            } else {
                sta = BT_STA_CONNECTED;
            }
        }
    }
    return sta;
}

//�������ֲ���/��ͣ
void bt_pause_play(void)
{
    if (bt_play_status != 1) {
        bt_play();
    } else {
        bt_pause();
    }
}

//����״̬����
void task_bt_sta_update(void)
{
    sys_ctl.bt_sta = bt_status_get();
   // printf("updsta: %d\n", sys_ctl.bt_sta );
    if (sys_ctl.bt_sta == BT_STA_PLAYING) {
        btdec_play();                           //�ָ��������ֲ���
    } else if (sys_ctl.bt_sta == BT_STA_INCOMING) {
        put_msg(Q_BT_INCOMING);
    } else if ((sys_ctl.bt_sta == BT_STA_OUTGOING || sys_ctl.bt_sta == BT_STA_PHONE)
      && (task_ctl.work_sta != TASK_POWEROFF))  //����ͨ��ʱ���ܵ͵�ػ�
    {
        put_msg(Q_BT_CALL);
    }
}

//�����¼�֪ͨ
void bt_notice(u8 evt)
{
    switch (evt) {
    case BT_EVT_DISCONNECT:
        //�����Ͽ��¼�
        break;

    case BT_EVT_LOSTCONNECT:
        //�������Ӷ�ʧ��Զ����Ͽ���
        put_msg(Q_BT_CONN_LOST);
        break;

    case BT_EVT_CONNECT:
        //���������¼�
        break;

    case BT_EVT_CONNECTED:
        //�������ӳɹ��¼�
        break;

    case BT_EVT_PLAY:
        //������ʼ�����¼�
        put_msg(Q_BT_PLAY);
        break;

    case BT_EVT_STOP:
        //����ֹͣ�����¼�
        put_msg(Q_BT_STOP);
        break;

    case BT_EVT_INCOMING:
        //�����¼�
       // printf("IN\n");
        put_msg(Q_BT_INCOMING);
        break;

    case BT_EVT_OUTGOING:
    case BT_EVT_CALL:
        //ȥ���¼�
        put_msg(Q_BT_CALL);
        break;

    default:
        break;
    }
}

//���ռ������ŷ�������ֵ
#pragma location="BT_HID"
bool bt_hid_photo(void)
{
#if BT_HID_EN
    bt_hid_connect();   //HIDδ����ʱ����HID
    delay_5ms(2);
    if (!bt_hid_wait_connect()) {
        return 0;
    }
    bt_hid_key(HID_KEY_ENTER);                  //enter, android4.0����
    for(u8 i=0; i<50; i++) {
        bt_event();                             //���ӵ���ʱ��������Щ�ֻ���Ӧ������
    }
    bt_hid_consumer(HID_KEY_VOL_UP);            //volume up, ios
#endif
    return 1;
}

//����ʱ��ӿ�
void hcc_vendor_settime(u8 *buf)
{
    t_time.year = buf[0];
    t_time.month = buf[1];
    t_time.day = buf[2];
    t_time.hour = buf[3];
    t_time.minute = buf[4];
    t_time.second = buf[5];
    set_rtc_time();
}

//Ƶƫ����
void bt_test_freq(void)
{
    //ѡ��2M����ϵͳʱ��ѡ48MHz
    sys_pll_x26m();

    P3DIR &= ~BIT(0);           //CLK����ʱ�����
    P3 |= BIT(0);               //CLK����ʱ�����
    SDCON0 |= BIT(0);           //Enable SDC
    SDBAUD = 11;                //���2MHzʱ��
    SDCON0 |= BIT(1);           //Keep Clk Outing
}

u8 bt_use_ev3(void)
{
    return 1;
}

#if BT_HID_MANU
bool bt_hid_enable(void)
{
    return t_bt.hid_enable;
}
#endif

//ƻ���豸��������
void send_vol_to_iphone_hook(void)
{
    if(sys_ctl.connected && IS_APPLE_VOL_CTRL) {
        AVRCPChangeVolume();
    }
}
