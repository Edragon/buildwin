#include "include.h"
#include "hci.h"
#include "btapi.h"
#include "hs_hf.h"
#include "hosapi.h"
#include "task.h"

#define DBG_BT_SPP_EN           0



union {
    struct {
        u8 bt_status_flag:                       1;     //����״̬����ʾ���±�־
        u8 bt_state_flag:                        1;     //����ͨ��״̬��ʾ���±�־
        u8 bt_atcmd_clcc_flag:                   1;     //������ȡȥ����������־
        u8 bt_phonenum_flag:                     1;     //����ͨ��������ʾ���±�־
    };
    u8 bt_disp_flag;
};


u8 BT_ONLINE = TASK_BT_EN;
u8 BT_STATE = 0;
u8 BT_STATE_pre = 0xFF;

u8 bt_connect_flag;
u8 bt_sleep_cnt;        //BTģ�������������ģʽ

bool need_send_ppkey;
u8  send_ppkey_countdown;

u8 bt_dev_bat;          //������ʾ����

u8 far_vol;
s16 far_noise_pow_cnt;
u8 far_noise_pow;

u16 mic_on_time;	// ����MIC˵����mic gain���Ʊ���һ��ʱ��
u16 spk_on_time;	//Զ��˵��ʱ��ϳ�ʱ����һ�¼�Ȿ��MIC�Ƿ���˵��

IAR_XDATA_A s16 sco_farpcm[SCO_FARPCM_SIZE] @ "BT_SCOFAR_XDATA";   // scoԶ�˵�PCM����,���ڼ����ƻ���, BT_SCOFAR_XDATA��BT_AVDTP_XDATA����

#pragma data_alignment=2
IAR_XDATA_A s16 temp_sample @ "SCO_VAD_BUF";

IAR_XDATA_A volatile u8 *bt_voice_pcmrd;
IAR_XDATA_A volatile u8 *bt_voice_pcmwr;

s16 *sco_farpcm_wptr;
s16 *sco_farpcm_rptr;
u16  sco_farpcm_len;

#define SCO_DELAY (sizeof(sco_spk_vad))
IAR_XDATA_A u8 sco_spk_vad[1] @ "SCO_VAD_BUF";
u8 sco_spk_vad_last, sco_spk_vad_index;
u8 sco_spk_vad_mute;                    //Զ��ֹͣ�󣬽���MIC��MUTEһС��ʱ��
u8 sco_far_vad, sco_far_vad_last;
u8 sco_near_mic_mute;                       //Զ��ֹͣ�󣬽������Ե����ȼ�ģʽMUTEһС��ʱ��
//u8 sco_mic_vad;
u8 sco_spk_stable;

u8 sco_mic_pow, sco_mic_stable;

s16 sco_mic_gain;
u8 sco_cal_index;
s8 sco_cal_max, sco_cal_min;
u8 __xdata *sco_buf_ptr;

u8 try_dis_sco;     //�����л�Ϊ˽�ܽ���
u8 bt_hshf_flag;    //����ͨ����־
u8 g_InComingCall = 0;

#define MIC_GAIN_STEP                  (MIC_GAIN_MAX/16)
#define SCO_SPK_MUTE                   20   //����MUTEס20 * 3 ms
u8 vad_threshold;
u8 vad_threshold_mute;//mic mute����ֵ
u8 sco_spk_mute;


#define SPP_MSG_NUM                     6

typedef struct {
    u8 msg;
    char str[8];
} type_spp_msg;

#pragma constseg="SPP_RX_CONST"
IAR_CONST char tbl_spp_sig[] = "BW";
IAR_CONST type_spp_msg tbl_spp_msg[SPP_MSG_NUM] = {
    {KU_PREV, "AT#ME"},
    {KU_NEXT, "AT#MD"},
    {K_VOL_UP, "AT#VU"},
    {K_VOL_DOWN, "AT#VD"},
    {KU_PLAY, "AT#MA"},
    {KU_MODE, "AT#MO"},
};
IAR_CONST char tbl_spp_settime[] = "AT#RT";
#pragma constseg=default


#define DBNum     4

typedef struct{
    u8  Link_key[16];
    u8  bdAddr[6];
    u8  bdName[32];
    u8  record_valid;
}   DB_DEVICE_RECORD;

typedef struct {
	DB_DEVICE_RECORD dev_list[DBNum];
	u8 current_num; //current pairing device
}  DB_RECORD;

IAR_XDATA_A DB_RECORD g_db @ "BTAPI_XDATA";


// �����������
u8 EnableSimplePairMode;    //0x00:disable, 0x01:enable simple pairing
u8 btpincode[4];            // PinCode: "0000"


#pragma constseg="BT_API_CONST"
IAR_CONST u8 BT_RFINFO_TABLE[128] = {//spi_bt_rfinfo_read
    // ��Ϣͷ
    'C',  'O', 'N', 'W', 'I', 'S', 'E', '-', '6', '6', '3', '9', 'v', '1', '.', '0',

    // ��������
    BT_NAME,

    // ������ַ
    0x56,0x34,0x12,0x99,0x22,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

    // ����RF��Ϣ,eeprom address: [0x80~0xBF]
    0x0F,0x40,0x8F,0x41,0x04,0x06,0x04,0x04,0x00,0x80,0x00,0x00,0x80,0x01,0x20,0xD6,
    0x93,0x00,0xF7,0x3F,0x00,0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00,0x00,0x06,0x70,
    0xFF,0x1E,0x2C,0x1F,0x06,0x42,0x14,0x7E,0x00,0x04,0x07,0x0E,0x1E,0xF8,0x00,0x01,
    0x00,0x00,0xF4,0xF1,0x0C,0xC3,0x00,0x16,0x44,0x00,0x29,0x1C,0x1C,0x24,0xFF,0xFE
};

IAR_CONST u8 BT_PAIRED_INFO_HEAD[16] = {'B', 'T', 'P', 'A', 'I', 'R', 'E', 'D', 'I', 'N', 'F', 'O', 'H', 'E', 'A', 'D'};

__root IAR_CONST u8 BT_INFO_BUF[64] = {0};
__root IAR_CONST u8 BT_INFO_TAG[] = "BTINF";
#pragma constseg=default

bool spi_load_btinfo(u8 *buf);
bool spi_bt_config_read(void *buf,u8 addr,u16 len); //��ȡ����������Ϣ
void spi_bt_config_write(void *buf,u8 addr,u16 len);//д������������Ϣ

extern IAR_XDATA_A u8 spi_param_data[256];
//===============Data Base =====================//
static bool DB_LoadFromNV(void)
{
	//my_memset(Local_BDAddr,0xff,6);
#if ALIGN_COD
    if(spi_bt_paired_read_2(spi_param_data, 0, 256) == true)  //  SPI������������Ϣ��Ч
#else
    if(spi_bt_config_read(spi_param_data, 0, 256) == true)
#endif
    {
        if((spi_param_data[0]=='B') &&(spi_param_data[1]=='T')&&(spi_param_data[2]=='P')&&(spi_param_data[3]=='A'))
    	{
            //printf("DB_LoadFromNV\n");
            my_memcpy(&g_db, spi_param_data+16, sizeof(g_db));   //ǰ��16 BYTE Ϊ BT_PAIRED_INFO_HEAD ��Ϣͷ
			return true;
    	}
    }
 	my_memset((u8*)&g_db, 0x00, sizeof(g_db));
   	return false;
}
bool DB_SaveToNV(void)
{
	//printf("DB_SaveToNV\n");
#if ALIGN_COD
    spi_bt_paired_write_2(&g_db, 16, sizeof(g_db));
#else
    spi_bt_config_write(&g_db, 16, sizeof(g_db));	//ǰ��16 BYTE Ϊ BT_PAIRED_INFO_HEAD ��Ϣͷ
#endif
	return DB_LoadFromNV();
}

void DB_saveVol(void)
{
#ifdef SAVEVOL	

#endif
}

//SECTION(".SYSTEM_INIT")
bool DB_Init(void)
{
	my_memset((u8*)&g_db,0,sizeof(g_db));
	my_memset(Link_BdAddr,0,6);
	//read flash data
    sbc_init();
    return DB_LoadFromNV();
}
////commented by Jocelyn
//bool DB_Deinit(void)
//{
//  //save new flash data
//    return DB_SaveToNV();
//}

bool DB_ClearDBData(void)
{
    my_memset((u8*)&g_db,0,sizeof(g_db));
    my_memset(Link_BdAddr,0,6);
    return DB_SaveToNV();
}

//SECTION(".CONNECT_BANK_TEXT")
u8 DB_SearchDevice(const u8*addr, u8 *linkey)
{
	u8 i=0;
	for(i=0;i<DBNum;i++){
		if(!(os_memcmp(g_db.dev_list[i].bdAddr,addr,6)) && (g_db.dev_list[i].record_valid)){
			g_db.current_num = i;//printf("g_db.current_num: %d\n",g_db.current_num);
			if(linkey!=0){
				os_memcpy(linkey,g_db.dev_list[i].Link_key,16);
                //printf("sear_lnk%d: ", i);
                //printf_array(g_db.dev_list[i].Link_key,16);
            }
			//printf("DB%d\n",i);
			return i;
		}
	}
	return 0xff;
}

//SECTION(".CONNECT_BANK_TEXT")
bool DB_AddRecord(const u8*addr, const u8 *linkey)
{
	u8 i=0;
	for(i=0;i<DBNum;i++){
		if(!g_db.dev_list[i].record_valid){
			g_db.current_num=i;
			if(linkey!=0){
				os_memcpy(g_db.dev_list[i].Link_key,linkey,16);
			}
			else{
				my_memset(g_db.dev_list[i].Link_key,0xff,16);//clear linkey

			}
			os_memcpy(g_db.dev_list[i].bdAddr,addr,6);

            hcmd_remote_name_request((u8 *)addr);

			g_db.dev_list[i].record_valid=TRUE;
            //printf("ad=%d\n",g_db.current_num);
			return DB_SaveToNV();
		}
	}
//	printf("DB_AddRecord=%d\n",g_db.current_num);
	g_db.current_num++;
	g_db.current_num=(g_db.current_num%DBNum);
	if(linkey!=0){
		os_memcpy(g_db.dev_list[g_db.current_num].Link_key,linkey,16);
	}
	else{
		my_memset(g_db.dev_list[i].Link_key,0xff,16);//clear linkey
		
	}
	os_memcpy(g_db.dev_list[g_db.current_num].bdAddr,addr,6);
//	printf("DB_AddRecord=%d\n",g_db.current_num);
//	uart_putbuf((u8*)(&g_db),sizeof(g_db));
//	printf("\n");

	return DB_SaveToNV();
	
}

//SECTION(".CONNECT_BANK_TEXT")
bool DB_DeleteRecord(const u8*addr)
{
    u8 i=0,j=0;
    for(i=0;i<DBNum;i++){
        if(!(os_memcmp(g_db.dev_list[i].bdAddr,addr,6)) && (g_db.dev_list[i].record_valid)){
            g_db.dev_list[i].record_valid=FALSE;
            my_memset(g_db.dev_list[i].Link_key,0xff,16);
            my_memset(g_db.dev_list[i].bdName,0xff,32);
            if(g_db.current_num==i){//current_num
               for(j=DBNum;j>0;j--){
                    if(g_db.dev_list[j-1].record_valid){
                        g_db.current_num=j-1;
                        //printf("db=%d\n",g_db.current_num);
                        return TRUE;
                    }
               }
               g_db.current_num=0;//no pairing device
              // printf("db=%d\n",g_db.current_num);
            }
            return TRUE;
        }
    }
    return FALSE;
}

//SECTION(".CONNECT_BANK_TEXT")
bool DB_ReplaceRecord(const u8 index, const u8 *linkey)
{
    g_db.current_num=index;
    os_memcpy(g_db.dev_list[index].Link_key,linkey,16);

    return DB_SaveToNV();
}

//SECTION(".BT_COMMON")
void DB_CheckRecord(void)
{
    if(DB_SearchDevice(Link_BdAddr, 0)!=0xff){      //change current number
        hcmd_remote_name_request(Link_BdAddr);
        DB_SaveToNV();
    }
    else{
        DB_AddRecord(Link_BdAddr, 0);
    }
}

//SECTION(".CONNECT_BANK_TEXT")
bool DB_GetCurrentDevice(u8* addr)
{
    if(g_db.dev_list[g_db.current_num].record_valid){
        os_memcpy(addr,g_db.dev_list[g_db.current_num].bdAddr,6);
        return TRUE;
    }
    else return FALSE;
}

//SECTION(".CONNECT_BANK_TEXT")
u8* GetCurrentRemoteDev(void)
{
    u8 aa[6];
    memset(aa, 0, 6);
    if(os_memcmp(Link_BdAddr,aa,6)){
        return Link_BdAddr;
    }
    else{
        if(g_db.current_num < DBNum && os_memcmp(g_db.dev_list[g_db.current_num].bdAddr, aa, 6)) //no pairing device
            return g_db.dev_list[g_db.current_num].bdAddr;
        else
            return 0;
    }
}

/*----------------------------------------------------------------------------*/
/**@brief: ��ӡ����Թ����豸��Ϣ�������ã�
//@param:  ��
//@return: ��
//@note:   ��
*/
/*----------------------------------------------------------------------------*/

#pragma location="BT_API_1"
void printf_remote_dev_bt_info()
{
    for(u8 i=0; i<DBNum; i++){
        if(g_db.dev_list[i].record_valid){
            printf("\n^^^^^^ device %d ^^^^^^^\n",i);
            printf("bdAddr: ");
            for(u8 j=5; j>0; j--){
                printf("%02x : ",g_db.dev_list[i].bdAddr[j]);
            }
            printf("%02x\n",g_db.dev_list[i].bdAddr[0]);

            printf("bdName: ");
            for(u8 k=0; k<32; k++){
                if(g_db.dev_list[i].bdName[k]){
                    printf("%c",g_db.dev_list[i].bdName[k]);
                }
                else{
                    printf("\n");
                    break;
                }
            }
        }
    }
}

/*----------------------------------------------------------------------------*/
/**@brief: ѡ����Թ����豸����
//@param:  device_numΪ��Ҫ���ӵ��豸��ţ�ȡֵ��ΧΪ0��3
//@return: ��
//@note:   ���ô˺���ǰ�����Ѿ������������豸������Զ��Ͽ������豸�����ӣ�Ȼ���Զ�������ѡ����豸��
           ���Ѿ���������ѡ����豸�����޷�Ӧ����δ�������κ��豸������Զ�������ѡ����豸��
*/
/*----------------------------------------------------------------------------*/
void bt_connect_appointed_dev(u8 device_num)
{
    if(g_db.dev_list[device_num].record_valid && ((BT_STATE >= StateConnected && g_db.current_num != device_num) || BT_STATE < StateConnected)){
        //printf("appointed_dev_num: %d\n",device_num);
        BT_Disconnect();
        while (bt_connect_flag || bt_voice_state != BT_VMS_IDLE) {
            WATCHDOG_CLR();
            BtMainLoop();
        }

        DB_SearchDevice(g_db.dev_list[device_num].bdAddr, 0);
        //BT_Connect();
        BT_TimeOutReconnect();
        //bt_voice_put(BT_VOICE_NUM0 + device_num);
    }
}

/*----------------------------------------------------------------------------*/
/*@brief: ��ȡ��Թ���������������FLASH��
//@param: nameΪ����ȡ����ǰ�����ӵ��豸���ƣ������浽g_db.dev_list[i].bdName��
//@return ��
//@note:  ���ú���hcmd_remote_name_request(u8 *bd_addr)����ڿ������Զ����ô�
//        �������ⲿ�����ڴ˺�������ȥ��������ȡ���豸�����籣�浽FLASH����ȵȲ���
*/
/*----------------------------------------------------------------------------*/
#pragma location="BT_API_1"
void hevt_remote_name_complete(u8 *name)
{
    //printf("NAME: %s\n", name);
    //memset((void *)&name[25], 0 , 7);
    //memset((void *)&g_db.dev_list[g_db.current_num].bdName[25], 0 , 7);
    /*for(u8 i=0; i<32; i++){
        printf("name[%d]: %c , bdName[%d]: %c\n",i,name[i],i,g_db.dev_list[g_db.current_num].bdName[i]);
    }
    printf("size: %d\n",sizeof(g_db.dev_list[g_db.current_num].bdName));*/

    //u8 size_name = (sizeof(name) > sizeof(g_db.dev_list[g_db.current_num].bdName))? sizeof(name): sizeof(g_db.dev_list[g_db.current_num].bdName);

    /*for(u8 i=0; i<32; i++){
        if(name[i] != g_db.dev_list[g_db.current_num].bdName[i]){
            //printf("name[%d]: %c , bdName[%d]: %c\n",i,name[i],i,g_db.dev_list[g_db.current_num].bdName[i]);
            os_memcpy(g_db.dev_list[g_db.current_num].bdName, name, 32);
            DB_SaveToNV();
            break;
        }
    }*/

    if(memcmp(g_db.dev_list[g_db.current_num].bdName, name, 32)){
        //printf("update_remote_dev_name_to_flash\n");
        os_memcpy(g_db.dev_list[g_db.current_num].bdName, name, 32);
        DB_SaveToNV();
    }
}

IAR_XDATA_A u8 phone_call_number[24] @ "BT_XDATA";
/*----------------------------------------------------------------------------*/
/*@brief: �����˻�ȡ�ֻ��˲����ĺ���ӿ�
//@param: cmdΪָ���ֻ����������ĺ�������ָ�����
//@return:��
//@note:  ���ֻ��˲�����������hsf_send_atcmd(u8 *str, u16 len)���������
          ����str�ַ���Ϊ"AT+CLCC"��lenΪstr�ַ����ĳ��ȣ��綨��
          u8 get_phone_call_number[]="AT+CLCC";
          �����hsf_send_atcmd(get_phone_call_number, sizeof(get_phone_call_number));��
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
        if(*ptr == '\"' && *(ptr+1) >= '0' && *(ptr+1) <= '9')  //�ж�һ�º����Ƿ���Ч
        {
            do{
                ptr++;
                size_of_call_number ++;
            }while(*ptr && *ptr != '\"');
            *ptr = '\0';
            //printf("NUM:%s\n", &cmd[18]);   // get number

            if(size_of_call_number == 1){
                ((u8 *)phone_call_number)[0] = '#';
                ((u8 *)phone_call_number)[1] = '\0';
            }
            else{
	            size_of_call_number --;
	            //printf("size_of_call_number: %d\n",size_of_call_number);
	            memset((u8 *)&phone_call_number, 0, sizeof(phone_call_number));
	            memcpy((u8 *)&phone_call_number, (u8 *)&cmd[18], size_of_call_number);

                bt_phonenum_flag = 1;
			}
            //printf("NUM:%s\n", phone_call_number);
        }
    }
}

/*----------------------------------------------------------------------------*/
/*@brief: ��������������뺯���ӿڣ�
//@param: pincodeΪ��Ҫ���õ�������룬���볤��Ϊ4λ��ֻ�йرռ������ʱ�������Ч;
//@return:��
//@note:  ���ô˺������ú���һ�ν�������ģʽʱ���÷�����Ч
*/
/*----------------------------------------------------------------------------*/
#pragma location="BT_API_1"
void set_pincode(char __code* pincode)
{
    u8 sum=0;
    for(u8 i=0; i<4; i++) {
        sum += pincode[i];
    }
    param_write((u8 *)pincode, PARAM_BT_PINCODE, 4);
    param_write8(PARAM_BT_PINCODE_SUM, sum);
}

//ÿ�ν�������ģʽʱ��ȡ�������������������
#pragma location="BT_API_1"
void read_pincode(void)
{
#if BT_SIMPLE_PAIR_EN
    EnableSimplePairMode = 1;           //�������
    memcpy(btpincode, (u8 *)BT_DEF_PINCODE, 4);
#else
    EnableSimplePairMode = 0;           //�Ǽ������
#if !BT_CHANGE_PINCODE
    memcpy(btpincode, (u8 *)BT_DEF_PINCODE, 4);
#else                                   //��̬�����������
    u8 sum = param_read8(PARAM_BT_PINCODE_SUM);
    param_read(btpincode, PARAM_BT_PINCODE, 4);

    for(u8 i=0; i<4; i++) {
        if(btpincode[i] < 32 || btpincode[i] > 126) {  //����Ƿ�ascii��ӡ�ַ�
            sum = 1;
            break;
        }
        sum -= btpincode[i];
    }
    if(sum != 0) {
        set_pincode(BT_DEF_PINCODE);
        memcpy(btpincode, (u8 *)BT_DEF_PINCODE, 4);
    }

#endif
#endif
    //printf("PinCode%d: ", EnableSimplePairMode);
    //printf("%c ", btpincode[0]);
    //printf("%c ", btpincode[1]);
    //printf("%c ", btpincode[2]);
    //printf("%c \n", btpincode[3]);
}

// PinCode  �������
void HciSetBtPinCode(u8 *pincode) @ "BT_HCI_EVENT_LINK"
{
    //pincode[0] = '0';
    //pincode[1] = '0';
    //pincode[2] = '0';
    //pincode[3] = '0';
    //printf("check_pincode\n");
    memcpy(pincode, btpincode, 4);
}

extern u32 serial_id;

//д������RF�����Ϣ����Զ���� 256
#pragma location="BT_API_CMD"
void spi_bt_rfinfo_write(void *buf)
{
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

//��ȡ����RF�����Ϣ
#pragma location="BT_API_CMD"
void *spi_bt_rfinfo_read(u8 len)
{
    u8 *ptr = os_malloc(256);

#if ALIGN_COD

    spi_bt_rfinfo_read_2(ptr);

#else

    u8 i;
    bool cfg_addr=0;

	spi_bt_config_read(ptr, 0, 16);                                 //BT_TAG
    if(memcmp(ptr, (u8 *)BT_PAIRED_INFO_HEAD, 16)) {
        memcpy(ptr, (void*)BT_PAIRED_INFO_HEAD, 16);
        memcpy(ptr+16, &g_db, sizeof(g_db));
        spi_bt_config_write(ptr, 0, 256);
    }

	memcpy(ptr+16, (void*)&BT_RFINFO_TABLE[16], 128-16);            //BT_NAME, BT_ADDR, RF_INFO

    //�޸�BT_INFO_BUF��ֱ�����棬��������bank
    spi_load_btinfo((u8 *)BT_INFO_BUF);
    if(memcmp((u8 *)BT_INFO_TAG, (u8 *)BT_INFO_BUF, 5) == 0) {
        u16 sum=0;
        for(i=0; i<62; i++) {
            sum += BT_INFO_BUF[i];
        }
        //printf_array((u8 *)BT_INFO_BUF, 64);
        //printf("bt_sum: %04x\n", sum);
        if(sum == *((u16 *)&BT_INFO_BUF[62])) {
            if(BT_INFO_BUF[6] & 0x01) {                             //���ʹ����д�����õ�����
                //printf("cfg_name\n");
                memcpy((void*)&ptr[BT_RFINFO_NAME], (void*)&BT_INFO_BUF[0x0e], 32);
            }
            if(BT_INFO_BUF[6] & 0x02) {                             //����Ƿ�ʹ����д�����õĵ�ַ
                //printf("cfg_addr\n");
                cfg_addr = 1;
                memcpy((void*)&ptr[BT_RFINFO_ADDR], (void*)&BT_INFO_BUF[0x2e], 6);
            }
            if(BT_INFO_BUF[6] & 0x04)
            {
                //printf("cfg_param\n");
                vad_threshold = BT_INFO_BUF[7];//7;//mic unmte����ֵ
                vad_threshold_mute = BT_INFO_BUF[8];//mic mute����ֵ
                sco_spk_mute = BT_INFO_BUF[9];//20; //mute����
            }
        }
    }
    //printf("un:%d,mu:%d,ti:%d\n",vad_threshold,vad_threshold_mute,sco_spk_mute);

    if(!cfg_addr) {
        U32_ACCESS(&ptr[BT_RFINFO_ADDR]) = serial_id;   //��д��δ���õ�ַʱ��ʹ��2227���к�
    }
#if BT_NAME_WITH_ADDR
    for(i=0; i<32-5; i++) {
        if(ptr[16+i] == 0) {                            // update name
            *((u8 *)&ptr[BT_RFINFO_NAME+0+i]) = '-';
            *((u8 *)&ptr[BT_RFINFO_NAME+1+i]) = hex_to_char(ptr[BT_RFINFO_ADDR+1]&0xf);
            *((u8 *)&ptr[BT_RFINFO_NAME+2+i]) = hex_to_char((ptr[BT_RFINFO_ADDR]>>4)&0xf);
            *((u8 *)&ptr[BT_RFINFO_NAME+3+i]) = hex_to_char(ptr[BT_RFINFO_ADDR]&0xf);
            *((u8 *)&ptr[BT_RFINFO_NAME+4+i]) = 0;
            break;
        }
    }
#endif
#if BT_NAME_DYMANIC_CHANGE
    if(bt_name[0] != 0){                                            //��̬������������
        //printf("check_bt_name[0]= 0x%02x\n",bt_name[0]);
        for(i=0; i<32; i++){
            *((u8 *)&ptr[BT_RFINFO_NAME+i]) = bt_name[i];
        }
    }
#endif
#endif

    memcpy(spi_param_data, ptr, 128);

    os_free(ptr);
    return spi_param_data;
}

void SetConnTimes(u8 times)
{
#if HEADSET_ENABLE
	HSHFAPI.CreateConnTimes = times;
#elif A2DP_ENABLE
	A2DPAPI.CreateConnTimes = times;
#elif BT_HID_EN
    HIDAPI.CreateConnStep = 0;
    HIDAPI.CreateConnTimes = times;
#endif
    //printf("Sc:%x,%x\n", A2DPAPI.AVDTPConList[0].state, times);
}

//SECTION(".CONNECT_BANK_TEXT")
void BT_Connect(void) @ "BT_A2DP_MMI"
{
    if(GetA2DPChannelConnectionState() != 0x07 &&  A2DPAPI.RemoteCreateConnect == 0){//Add by chchang 0115 for ipad1 play/pause problem when ipad not support HF
        if(HSHFAPI.RFCOMMConList[0].state != AP_STATE_RFCOMM_FINISHED)
            SetConnTimes(2);
        else
            A2DPAPI.CreateConnTimes = 2;

        //need_send_ppkey = 1;	// �����������Ƿ��Զ��������֣������������ͷ���һ��PLAY��
    }
}
//SECTION(".CONNECT_BANK_TEXT")
void BT_Connect_A2DP(void)
{
	if(GetA2DPChannelConnectionState() != 0x07 &&  A2DPAPI.RemoteCreateConnect == 0){//Add by chchang 0115 for ipad1 play/pause problem when ipad not support HF
	    	A2DPAPI.CreateConnTimes=2;
	}
}
//SECTION(".CONNECT_BANK_TEXT")
void BT_Disconnect(void)
{
    if(activateDiscState == 0)
	{
        activateDiscState=0x01;
    }
}
//SECTION(".CONNECT_BANK_TEXT")
void BT_Disconnect_A2DP(void)
{
    if(activateDiscState == 0)
	{
        activateDiscState=0x01;
    }
}

//SECTION(".HSF_BANK_TEXT")
void BT_IncomingCallNumber(u8* pnum, u8 len)
{
    if(g_InComingCall)
    {
#if (!BT_CALL_PRIVATE && BT_RING_NUMBER)
        memset((u8 *)&phone_call_number, 0, sizeof(phone_call_number));
        if(*pnum>='0' && *pnum<='9') {                      //�ж�һ�º����Ƿ���Ч
            u8 i = 0;
            memcpy((u8 *)&phone_call_number , pnum , len);  //��ȡ�������
            for(i = 0; i < len; i++){
                bt_voice_put(BT_VOICE_NUM0 + pnum[i]-0x30);
            }
            bt_phonenum_flag = 1;
        }
        bt_state_flag = 1;
        bt_voice_put(BT_VOICE_RING);
#endif
        g_InComingCall = 0;
    }
}

//SECTION(".HSF_BANK_TEXT")
void TurnOnRingTone(void)
{
//printf("ON :%d%d\n", HSFRingActive, HSFCallActive);
#ifdef MP3_PLAY
    if(mWorkingMode != WORK_MODE_BT)
    {
        switch_mode_save = WORK_MODE_MP3;
        switch_to_btmode();
    }
#endif
    if((GetRfCommChannelConnectionState()&0x10) == 0) {  //���ӹ����в���������
        return;
    }

	if(HSFRingActive == 0){
		HSFRingActive  = 1;
        g_InComingCall = 1;//����
#if !BT_CALL_PRIVATE
        bt_voice_put(BT_VOICE_RING);
#endif
	}
}

//SECTION(".HSF_BANK_TEXT")
void TurnOffRingTone(void)
{
	//printf("OFF:%d%d\n", HSFRingActive, HSFCallActive);
	HSFRingActive = 0;
    bt_voice_abort = 0;
    bt_voice_outptr = bt_voice_inptr;   //�屨�ż�����

	if(HSFCallActive == 0)
	{
  #ifdef MP3_PLAY
        if(switch_mode_save == WORK_MODE_MP3)
        {
			WARNING_MSG("re\n");
            PutMsg(MODE_MSG_MP3);
        }
   #endif
    }
}

void BT_PairingResult(u8 re)
{
	//re=0 pairing success
	//re=other  pairing fail
	//printf("pa=%d\n",re);
}

void process_BTVoiceConnect()
{
    DB_CheckRecord();
    sbc_init();

#if BT_CONNECT_MUSIC
    bt_voice_put(BT_VOICE_CONNECT);
#endif
}

#define A2DP_IS_CONN()          (GetA2DPChannelConnectionState()==0x07)
#define RFCOMM_IS_CONN()        (GetRfCommChannelConnectionState()!=0)
#define HID_IS_CONN()           (GetHidChannelConnectionState()==0xc0)
/*
 * ��Ϊandriod2.3���ֻ�������֧��HID�����ֻ����HID�������ֶϿ��������
 * ���ԣ�֧������profileʱ������HID�����ж������Ƿ����ϣ�
 * ֻ��HIDʱ������HID�����ж������Ƿ����ϡ�
 */
#if HEADSET_ENABLE||A2DP_ENABLE
    #define BT_IS_CONN()        (A2DP_IS_CONN() || RFCOMM_IS_CONN())
#elif BT_HID_EN
    #define BT_IS_CONN()        HID_IS_CONN()
#else
    #define BT_IS_CONN()        0   //����֧��ʱ����Ϊ�Ͽ�����
#endif

#if HEADSET_ENABLE || A2DP_ENABLE || BT_HID_EN
    #define BT_IS_DISCONN()     ((GetRfCommChannelConnectionState() | GetA2DPChannelConnectionState() | GetHidChannelConnectionState()) == 0x00)
#else
    #define BT_IS_DISCONN()     1   //����֧��ʱ����Ϊ�Ͽ�����
#endif

void bt_conn_check(void)
{
    if(!bt_connect_flag) {
        if(BT_IS_CONN()){
            bt_connect_flag=1;
            put_msg(BT_CONN_REQ);
        }
    } else {
        if(BT_IS_DISCONN()) {
            bt_connect_flag=0;
        }
    }

#if !BT_CALL_PRIVATE
    if (HSFRingActive || HSFCallActive || scoflag) {
        if (task_ctl.work_sta != TASK_BT) {
            work_sta_interrupt = task_ctl.work_sta;
            task_ctl.work_sta = TASK_BT;
        }
    }
#endif
}

#pragma location="BT_VOICE_SEG"
__near_func void process_BTVoice(void)
{
    process_bt_voice_do();


    if(!bt_connect_flag) {
        if(BT_IS_CONN()){
            bt_connect_flag=1;
            process_BTVoiceConnect();
        }
    } else {
        if(BT_IS_DISCONN())
        {
            bt_connect_flag=0;
            put_msg(BT_DIS_CONN);
#if BT_DISCONNECT_MUSIC
        #if !BT_MODE_EXIT_MUSIC
            bt_voice_put(BT_VOICE_DISCONNECT);
        #else
            if(task_ctl.work_sta == TASK_BT){
                bt_voice_put(BT_VOICE_DISCONNECT);
            }
        #endif
#endif
        }
    }
}

#pragma location="BT_STATE_CHANGE"
__near_func void process_BTStateChange(void)
{
    if(need_send_ppkey){
        if(GetA2DPChannelConnectionState() == 0x07 && BT_STATE != StatePlay){
            //printf("timeout-kick\n");
            send_ppkey_countdown = 30;  // @ 100ms unit, �쳣�Ͽ���������3�����ֻ�δ���ų����ͷ���һ�����ż�
            need_send_ppkey = 0;
        }
        if(BT_STATE == StateIdle && HSHFAPI.CreateConnTimes == 0 && A2DPAPI.CreateConnTimes == 0){
            //printf("timeout-clear\n");
            need_send_ppkey = 0;
        }
    }

    if(send_ppkey_countdown){
        if(BT_STATE != StateConnected){
            //printf("countdown-clear:%x\n", BT_STATE);
            send_ppkey_countdown = 0;
        }
        else if(send_ppkey_countdown < 5){    // send_ppkey_countdown �ݼ���Ϊ0�ˣ�����һ��PLAY��
            //printf("send play key: %d\n", state_a2dp_player);
            btMMI_PlayPause();
            send_ppkey_countdown = 0;   // ���ĿǰΪ����״̬��������,��෢��һ��PP��
        }
    }

    if(BT_IS_DISCONN())
    {
        BT_STATE=StateIdle;
    }
    else
    {
        if(HSFRingActive){
            BT_STATE = InComingCall;
        }
        else if(HSFCallActive == 2){
            if(avdtp_play_status){
                HSFCallActive = 0;    // avdtp_play_status��ʾSBC�ڲ����У�lenovo MA308���ز����ڶ��κ����ָñ�־����0
            }
            BT_STATE = OutGoingCall;
        }
        else if(HSFCallActive == 1){
            BT_STATE = OnThePhone;
        }
        else if(avdtp_play_status || scoflag){ // state_a2dp_player
            BT_STATE = StatePlay;
        }
        else{
            BT_STATE = StateConnected;
        }
    }
}

#pragma constseg="BT_RESET8202_CONST"
IAR_CONST char str_HWReset8202[] = "ZZZZZZZZZZZZZ: %d:%d:%d\n";
#pragma constseg=default


#pragma location="BT_RESET8202"
bool BT_ChipReset(u8 reConnCnt)
{
    bool ret;
    //reset 8202 add by chchang
    BT_RST_INIT();      // RESET_DIR(); // Set reset pin as output
    BT_RST_LOW();       // RESET_LOW();
    delay_5ms(2);

    BT_RST_HIGH();      // RESET_HIGH();
    bt_sleep_cnt = BT_SLEEP_CNT;	// CW6639��λ��ֻ����HCI RESET����������Ӧ��
    delay_5ms(30);

    hal_uart_init();
    bt_os_init();

    ret = BT_On();

    SetConnTimes(reConnCnt);

    state_a2dp_player = 0;
    return ret;
}

#pragma location="BT_RESET8202"
void BT_TimeOutReconnect()
{
    printf(str_HWReset8202, tmr1s_ticks/3600, tmr1s_ticks/60, tmr1s_ticks%60);

    if(/*BT_STATE == StateConnected && */state_a2dp_player == 1)
    {
        //printf("need PP\n");
        need_send_ppkey = 1;    // �쳣�Ͽ�ǰ�������ڲ���״̬���������������û���ڲ����У��ͷ���һ����������
    }

    BT_ChipReset(SuperTimeout_Connect_count);

    bt_timeout_reconnect = 0;
}

#pragma constseg="BT_INIT_CONST"
IAR_CONST char str_bt_init_addr[] = "BD:<%02x:%02x:%02x:%02x:%02x:%02x> - ";
IAR_CONST char str_debug_rfinfo[] = "BTDev<%02x:%02x:%02x:%02x:%02x:%02x>: %s\n";
#pragma constseg=default

extern u8 SDP_Record_en;
void avdtp_sbc_init();

#pragma location="BT_INIT_SEG"
void bt_init(void)
{
    vad_threshold = VAD_THRESHOLD;      //mic unmte����ֵ
    vad_threshold_mute = VAD_THRESHOLD_MUTE;//mic mute����ֵ
    sco_spk_mute = SCO_SPK_MUTE;        //mute����
    flag_bt_first = 1;
    memset((u8 *)0x4000,  0, 0x3000);   //��� DEC RAM

    need_send_ppkey = 0;
    send_ppkey_countdown = 0;
    bt_timeout_reconnect = 0;

    scoflag = 0;
    try_dis_sco = 0;

    SDP_Record_en = (SDP_HEADSET*HEADSET_ENABLE) | (SDP_A2DP*A2DP_ENABLE) | (SDP_SPP*BT_SPP_EN) | (SDP_HID*BT_HID_EN);

    bt_connect_flag = 0;

    avdtp_sbc_init();
    avdtp_sbc_reset();

#if !BT_SIMPLE_PAIR_EN && BT_CHANGE_PINCODE
    set_pincode(BT_DEF_PINCODE);    //ÿ���ϵ�ָ�Ĭ��pincode
#endif
}

#pragma location="BT_INIT_SEG"
u8 bt_power_up()
{
    bt_hshf_flag = 0;
    bt_dev_bat = 0xff;  //���µ�����ʾ
    tmr1s_ticks = 0;
    tmr100ms_current_ticks = 0;

    bt_voice_abort = 0;
    bt_voice_inptr = 0;
    bt_voice_outptr = 0;
    bt_voice_msg = 0;    //
    bt_voice_state = BT_VMS_IDLE;

    bt_user_feature = BT_FT_APPLE_DEVICE_EN | BT_FT_APPLE_VOL_CTRL_EN;

    read_pincode();

    DB_Init();

	//reset 8202 add by chchang
    //g_db.current_num = 1;
    BT_ONLINE = BT_ChipReset(PowerOn_Connect_count);

#if 0   // SHOW_BT_INFO
    for(u8 i = 0; i < DBNum; i++)
    {
      DB_DEVICE_RECORD *dblist = &g_db.dev_list[i];
      if(dblist->record_valid)
      {
          u8 j;
          printf(str_bt_init_addr, dblist->bdAddr[0],dblist->bdAddr[1],dblist->bdAddr[2],dblist->bdAddr[3],dblist->bdAddr[4],dblist->bdAddr[5]);
          for(j = 0; j < 16; j++)
          {
              //printf("%02x,", dblist->Link_key[j]);
              printf("%02x", dblist->Link_key[15-j]);
          }
          printf("\n");
      }
    }
#endif
    vad_threshold = VAD_THRESHOLD;      //mic unmte����ֵ
    vad_threshold_mute = VAD_THRESHOLD_MUTE;//mic mute����ֵ
    sco_spk_mute = SCO_SPK_MUTE;        //mute����
    u8 *ptr = spi_bt_rfinfo_read(128);  // show bt name & address
    u8 *paddr = &ptr[BT_RFINFO_ADDR];
    printf(str_debug_rfinfo, paddr[5],paddr[4],paddr[3],paddr[2],paddr[1],paddr[0], &ptr[BT_RFINFO_NAME]);

    return 1;
}

void ProssSppSendATCommand(u8 * Data,u16 len)
{
	u8 st;
	st = GetSPPChannelConnectionState();
	if(st) 	//  SPP��������
	{	
		RfcommSendATCommand(HSHFAPI.RFCOMMConList[0].l2cap_cid, GetSPPChannelDlci(), 4, len, (u8 __code *)Data);
	}
}

#pragma location="SPP_RX_SEG"
void spp_tx(u8 *ptr, u8 len)
{
#if BT_SPP_EN
    ProssSppSendATCommand(ptr,len);
#endif
}

#pragma location="SPP_RX_SEG"
u8 char_to_num(u8 ch)
{
    if (ch >= '0' && ch <= '9') {
        return (ch - '0');
    }
    return 0;
}

#pragma location="SPP_RX_SEG"
void spp_rx_msg(u8 *ptr, u8 len)
{
    if (0 == memcmp(&ptr[len-3], (char *)tbl_spp_sig, 2)) {
#if TASK_RTC_EN
        if (0 == memcmp(ptr, (char *)tbl_spp_settime, 5)) {
            if (len == 22) {
                u8 *p = ptr + 5;
                u16 year = char_to_num(*p++) * 1000;
                memset(&t_time, 0, sizeof(t_time));
                year += char_to_num(*p++) * 100;
                year += char_to_num(*p++) * 10;
                year += char_to_num(*p++);
                if (year >= 2000 & year <= 2099) {
                    t_time.year = year - 2000;
                }
                t_time.month = char_to_num(*p++) * 10;
                t_time.month += char_to_num(*p++);
                t_time.month--;

                t_time.day += char_to_num(*p++) * 10;
                t_time.day += char_to_num(*p++);
                t_time.day--;

                t_time.hour += char_to_num(*p++) * 10;
                t_time.hour += char_to_num(*p++);

                t_time.minute += char_to_num(*p++) * 10;
                t_time.minute += char_to_num(*p++);

                t_time.second += char_to_num(*p++) * 10;
                t_time.second += char_to_num(*p++);
                set_rtc_time();
            }
        }
        else
#endif
        {
            u8 i;
            for (i=0; i!=SPP_MSG_NUM; i++) {
                if (0 == memcmp(ptr, (char *)tbl_spp_msg[i].str, 5)) {
                    put_msg(tbl_spp_msg[i].msg);
                    return;
                }
            }
        }
    }
}


// bluestack would call this function....
#pragma location="SPP_RX_SEG"
void spp_rx(u8 *ptr, u8 len)
{
#if DBG_BT_SPP_EN
   printf("SPP(%x): %s\n", len, ptr);

   u32 i;
   for(i = 0; i < len; i++)
   {
	    if(ptr[i] >= '0' && ptr[i] <= '9')
	    {
	        bt_voice_put((0x80|BT_VOICE_NUM0)+(ptr[i] - '0'));
	    }
   }
#endif
   if (len >= 8) {
        spp_rx_msg(ptr, len);
   }

}


IAR_XDATA_A u8 bt_voice_inptr;
IAR_XDATA_A u8 bt_voice_outptr;
IAR_XDATA_A u8 bt_voice_msg;    //
u8 bt_voice_state;
volatile u8 bt_voice_abort;   //�����绰ʱ��ֹ�������ű�־
volatile u8 bt_voice_delay_cnt;

#define BT_VOICE_MSGLEN     32
IAR_XDATA_A u8 bt_voice_msg_pool[BT_VOICE_MSGLEN] @ "BT_MSG_POOL";

#pragma data_alignment=2
IAR_XDATA_A u8 bt_voice_ibuf[0x200] @ "BT_VOICE_IBUF";

void bt_voice_put(u8 msg)
{
    bt_voice_msg_pool[bt_voice_inptr] = msg;
    bt_voice_inptr++;
    bt_voice_inptr &= (BT_VOICE_MSGLEN - 1);
}

u8 bt_voice_show(void)
{
    u8 msg;

    if (bt_voice_outptr == bt_voice_inptr){
        return 0;
    }
    msg = bt_voice_msg_pool[bt_voice_outptr];

    return msg;
}

u8 bt_voice_get(void)
{
    u8 msg = bt_voice_show();
    if(msg)
    {
        bt_voice_msg_pool[bt_voice_outptr] = 0;
        bt_voice_outptr++;
        bt_voice_outptr &= (BT_VOICE_MSGLEN - 1);
    }
    return msg;
}

#pragma location="BT_VOICE_SEG"
__near_func void bt_voice_play(void)
{
    u8 fill_cnt, aucon5 = AUCON5;
    u8 *ptr = bt_voice_ibuf;

    if(bt_voice_state != BT_VMS_PLAYING) {
        return;
    }

    if(aucon5 == 0) {
        if(mp3res.bits == 8) {
            fill_cnt = 1;   //512/4=0x80, ������1��
        } else {
            fill_cnt = 2;   //512/2/4=0x40, ������2��
        }
    } else if(aucon5 >= 0x20) {
        return;
    } else {
        fill_cnt = 1;
    }

    while(fill_cnt--) {
        if(bt_voice_abort || spi_mp3res_read(ptr) == 0) {
            bt_voice_state = BT_VMS_PLAYEND;
            return;
        }

        //2014-1-1, ֧��8bit/16bit��WAV������Դ
        if(mp3res.bits == 8) {
            for(u16 i=0; i != 512; i++){
                while(!(AUCON7 & BIT(7)));
                AUCON5 = ptr[i];
                AUCON5 = 0x00;
                while(!(AUCON7 & BIT(7)));
                AUCON5 = ptr[i];
                AUCON5 = 0x00;
            }
        } else {
            for(u16 i=0; i != 512; i+=2){
                while(!(AUCON7 & BIT(7)));
                AUCON5 = ptr[i+1];
                AUCON5 = ptr[i];
                while(!(AUCON7 & BIT(7)));
                AUCON5 = ptr[i+1];
                AUCON5 = ptr[i];
            }
        }
    }
}

void bank_process_bt_voice(void)
{
    switch(bt_voice_state)
    {
    case BT_VMS_IDLE:
    {
        if (bt_voice_outptr != bt_voice_inptr)
        {
            u8 msg = bt_voice_get();
            bt_voice_msg = msg;
            if (msg) {
                bool temp_sint = IE_SINT;
                IE_SINT = 0;
                if (!scoflag) {
                    sbc_load_code(DCODE_BT_SCO, 1);
                }
                bt_voice_state = BT_VMS_DELAY;
                sys_flag.voice_play_flag = 1;
                IE_SINT = temp_sint;

#if CONSTANT_WARNING_VOLUME_EN
                user_change_volume(WARNING_VOLUME);
#endif
#if BT_DYMANIC_SUPPRESSION
                dac_fade_in(1);
#endif
#if 0//VOL_MAXMIN_MUSIC
                if(bt_voice_msg == RES_WAV_VOL_MAXMIN){
                    bt_voice_delay_cnt = 100;
                } else
#endif
                {
                    bt_voice_delay_cnt = OPA_UNMUTE_DELAY;     //���Ž�MUTE��ʱ���ɸ���ʵ���������
                }
            }
        }
        break;
    }
    case BT_VMS_DELAY:
        if(bt_voice_delay_cnt){//printf("BT_VMS_DELAY\n");
            return;
        } else {//printf("BT_VMS_INIT\n");
#if 0//VOL_MAXMIN_MUSIC
            if(bt_voice_msg == RES_WAV_VOL_MAXMIN){
                if(!sys_ctl.vol_maxmin_music_flag || sys_ctl.vol_maxmin_music_flag == 1){
                    bt_voice_outptr = bt_voice_inptr;
                    bt_voice_state = BT_VMS_PLAYEND;
                    break;
                }
            }
#endif
            bt_voice_state = BT_VMS_INIT;
        }
        break;
    case BT_VMS_INIT:
    {
        usb_audio_sound_play(bt_voice_msg & 0x7F);   // &0x7F, ��Ϊ��_BT_DEBUG_SPP��ʱ���ֻ�SPP�����������汨��
        sbc_pcm_len = 0;
        AUCON0 = 0x06;  //RST
        AUCON9 = 0x04;
        player_set_dac(mp3res.spr);             //2014-1-1, ֧�ֶ�����ʵ�WAV������Դ
        spi_critical = 1;
        bt_voice_state = BT_VMS_PLAYING;
        break;
    }

    case BT_VMS_PLAYING:
	{
#if (!BT_CALL_PRIVATE && BT_RING_NUMBER)    //�������ͱ���
        if((bt_voice_msg == BT_VOICE_RING)  || (bt_voice_msg >= BT_VOICE_NUM0 && bt_voice_msg <= BT_VOICE_NUM9))
#elif (!BT_CALL_PRIVATE && !BT_RING_NUMBER) //ֻ������
        if((bt_voice_msg == BT_VOICE_RING))
#else
        if(0)                               //˽�ܽ���ʱ������������
#endif
        {
            if(HSFRingActive){
            }
            else{
                bt_voice_state = BT_VMS_PLAYEND;  // exit
                break;
            }
        }
        break;
	}
    case BT_VMS_PLAYEND:
        spi_critical = 0;
        if(bt_voice_abort) {
            bt_voice_outptr = bt_voice_inptr;
        }
        {
            u8 msg = bt_voice_get();
            if(msg){
                bt_voice_msg = msg;
                bt_voice_state = BT_VMS_INIT;
#if VOL_MAXMIN_MUSIC
                if(msg == RES_WAV_VOL_MAXMIN){
                    if(sys_ctl.vol_maxmin_music_flag == 2){
                        bt_voice_delay_cnt = 120;   //����ͨ����ֵ�ı�����ģʽ������������ʾ���Ĳ����ٶȣ�ԽСԽ��
                        bt_voice_state = BT_VMS_DELAY;
                    } else {
                        bt_voice_outptr = bt_voice_inptr;
                        bt_voice_state = BT_VMS_PLAYEND;
                    }
                }
#endif
            }
            else
            {
#if !BT_CALL_PRIVATE    //��˽�ܽ������ظ�������
                if(HSFRingActive && bt_voice_msg == BT_VOICE_RING)
                {
                    bt_voice_msg = BT_VOICE_RING;
                    bt_voice_state = BT_VMS_INIT;
                } else
#endif
                {
                    if (AUCON5 == 0)
                    {
                        IE_SINT = 0;
                        clr_dac_buf();
                        sys_flag.voice_play_flag = 0;
#if !BT_CALL_PRIVATE
                        if((HSFCallActive)||(HSFRingActive)) {
                            user_set_hshfvol(sys_ctl.hshf_vol);
                        } else
#endif
                        {
                            user_change_volume(sys_ctl.volume);
                        }

                        sbc_pcm_len = 0;
                        bt_voice_state = BT_VMS_IDLE;
                        if (!scoflag) {
                            sbc_load_code(DCODE_BT_SBC, 1);
                            sbc_init();                 //���������ж�
                        } else {
                            player_set_dac(8);
                        }
                        bt_voice_msg = 0;    //
                        IE_SINT = 1;
                    }
                }
            }

            break;
        }
    } // switch(bt_voice_state)

}

#pragma location="BT_VOICE_SEG"
__near_func void process_bt_voice()
{
}

#pragma location="BT_VOICE_SEG"
__near_func void process_bt_voice_do()
{
    //printf("%d: %04x %04x\n", bt_voice_state, bt_voice_inptr, bt_voice_outptr);
    if(AUCON5 < 0x60 && (bt_voice_state != BT_VMS_IDLE || bt_voice_outptr != bt_voice_inptr))
    {
        bank_process_bt_voice();
    }
    if(bt_hshf_flag == 1 && bt_voice_state == BT_VMS_IDLE) {
        bt_enter_hshf_do();
    }
}

//#pragma location="BT_SCO_SEG"
#pragma optimize=speed
#pragma inline=forced
__near_func void calc_spk_power(u8 power)
{
    //u8 vad_1, vad_2;
    if (power < (vad_threshold * 4)) {
        if (power > far_noise_pow) {
            far_noise_pow_cnt++;
            if (far_noise_pow_cnt >= 1000) {
                far_noise_pow_cnt = 0;
                far_noise_pow++;
            }
        } else if (power < far_noise_pow) {
            far_noise_pow_cnt--;
            if (far_noise_pow_cnt <= -1000) {
                far_noise_pow_cnt = 0;
                far_noise_pow--;
            }
        }
        //far_noise_pow = (u16)power * 1 + (((u32)far_noise_pow * 4095 + 2048) >> 12);
        //fpow = (u8)(far_noise_pow >> 12);
        //printf("%02x ", fpow);
    }
    //vad_1 = (far_noise_pow > vad_threshold / 2) ? (far_noise_pow * 2) : vad_threshold;
    //vad_2 = (far_noise_pow > vad_threshold_mute / 2) ? (far_noise_pow * 2 + vad_threshold_mute - vad_threshold) : vad_threshold_mute;
    if (far_noise_pow > 3) {
        power = power * 3 / far_noise_pow;
    }
	
    u8 var = 0;//spk_on_time / 128;
    if(var > 15){
        var = 15;
	}
    if(sco_spk_vad_last == 0) {
        if (power >= vad_threshold + var) {
            if(sco_spk_stable < 3) {    // 3 * 3 ms
                sco_spk_stable++;
            } else {
                sco_spk_vad_last = 0xff;
                sco_spk_stable = 0;
            }
        } else {
            sco_spk_stable = 0;
        }
    } else{
        /*******************************************************************************************
        ����⵽sco_spk_stable��(ÿ�μ��ʱ���Լ3ms)Զ����������ֵС��(vad_threshold_mute + var)
        ����ΪԶ����˵���������ټ���MUTEס sco_spk_vad_mute * 3ms���˺���򿪱���MIC���������Ƚ�
        ���ؿ��԰ѡ�sco_spk_stable < 5���� sco_spk_vad_mute ����һ��(0~255)��
        *******************************************************************************************/
        if(power <= vad_threshold_mute + var){
            if(sco_spk_stable < 5) {    //5 * 3 ms
                sco_spk_stable++;
            } else {
                sco_spk_vad_mute = 0;//sco_spk_mute;
                sco_spk_vad_last = 0;
                sco_spk_stable = 0;
            }
        } else{
            sco_spk_stable = 0;
        }
    }
    if (sco_spk_vad_last) {
        sco_spk_vad_last = (3 * 255) / far_noise_pow;
    }
    //printf("%02d %02x, ", power, sco_spk_vad_last);


    /*u8 mic_index = sco_spk_vad_index + ((0x90 - AUCON5) / 6); //ÿ��24���㣬һ��AUCON5��6����
    //printf("%02x ", AUCON5);  //dac_run_flag
    if (mic_index >= SCO_DELAY) {
        mic_index -= SCO_DELAY;
    }*/

    //printf("%d", sco_spk_vad_last);
    u8 mic_index = sco_spk_vad_index;
    sco_far_vad = sco_spk_vad[mic_index];
    if (sco_spk_vad_mute) {
        sco_spk_vad_mute--;
        sco_spk_vad[sco_spk_vad_index] = 1;                         //����MUTEס����һ��ʱ��
    } else {
        sco_spk_vad[sco_spk_vad_index] = sco_spk_vad_last;
    }
    sco_spk_vad_index++;
    if (sco_spk_vad_index >= SCO_DELAY) {
        sco_spk_vad_index = 0;
    }
}

#pragma optimize=speed
#pragma location="BT_SCO_SEG"
__near_func void sco_echo_process(s8 cur)
{
    if(sco_cal_index < 24){
        //u8 high = rdata;
        if (cur > sco_cal_max) {
            sco_cal_max = cur;
        }
        if (cur < sco_cal_min) {
            sco_cal_min = cur;
        }
        sco_cal_index++;
        if(sco_cal_index == 24)
        {
            u8 sco_cal_power = sco_cal_max - sco_cal_min;
            //printf(">%02x ", sco_cal_power);
            calc_spk_power(sco_cal_power);

            sco_cal_index = 0;
            sco_cal_max = -128;
            sco_cal_min = 127;

            if (far_vol < sco_spk_vad_last) {
                //far_vol++;
                far_vol += 10;
                if(far_vol > sco_spk_vad_last){
                    far_vol = sco_spk_vad_last;
                }
            } else if (far_vol > sco_spk_vad_last) {
                static u8 far_vol_decrease_cnt = 2;
                if(far_vol_decrease_cnt){
                    far_vol_decrease_cnt --;
                } else {
                    far_vol--;
                    far_vol_decrease_cnt = 2;
                }
            }
        }
    }
}

#pragma location="BT_SCO_SEG"
__near_func void Process_Agc_Data(void)
{
    if(bt_debug_sco >= 6)
    {
        bt_debug_sco -= 3;
#if HEADSET_ENABLE
        //while(agc_buf_cnt >= ((576*3) / 24))
        //    agc_buf_cnt -= (576*3) / 24;

    #if 0
        if(agc_buf_cnt && bt_uart_txlen == 0)
        {
            //HciSendSco(sco_buf_ptr);
			//uartTxQ3[0]=0x03;//packet type
			//uartTxQ3[1]=0x5C;
			//uartTxQ3[2]=0x00;
			//uartTxQ3[3]=0x30;//48 bytes, 24 PCM samples

            //u16 power = *((u16*)agc_far_wpos);
            //agc_far_wpos++;
            //printf("%3d,%3d\n", power,sco_mic_gain);
            //printf("{%2d,%d}", sco_spk_stable, sco_spk_vad);//power);
            //uart_putchar('0'+sco_spk_vad);

//            calc_spk_power(power);

        #ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_
            if(Test_Mode_Flag == 3)
                sco_mic_gain = MIC_GAIN_MAX;   // ��MIC����ʱ��������������
        #endif
            //if(*power > 100)
            //    my_memset(uartTxQ3 + 4, 0, 24*2);     // freq_666
            //else
            //    my_memcpy(uartTxQ3 + 4, sco_buf_ptr, 24*2);     // freq_666
            s16 *dst = (void*)(uartTxQ3 + 4);
            s16 *src = (void*)sco_buf_ptr;
            for(u8 k = 0; k < 24; k++) {
                *dst = highpass(src);
                src++;
                dst++;
            }
            dst = (void*)(uartTxQ3 + 4);
            u16 mic_pow = voice_maxpow_step(dst, 24, 1);

            if (sco_mic_pow) {
                if (mic_pow <= 0x400) { //0x800
                    sco_mic_stable++;
                    if (sco_mic_stable >= 15) { //10
                        sco_mic_pow = 0;
                        sco_mic_stable = 0;
                    }
                } else {
                    sco_mic_stable = 0;
                }
            } else {
                if (mic_pow >= 0x500) { //0x1000
                    sco_mic_stable++;
                    if (sco_mic_stable >= 2) {  //3
                        sco_mic_pow = 1;
                        sco_mic_stable = 0;
                    }
                } else {
                    sco_mic_stable = 0;
                }
            }

            if (sco_mic_vad == 0 || !sco_mic_pow) {    // far is talking..... or mic is mute
                //printf("0");
                if(sco_mic_gain > MIC_GAIN_STEP)
                    sco_mic_gain -= MIC_GAIN_STEP;
                else
                    sco_mic_gain = 0;//1;
            } else {
                //printf("1");
                if(sco_mic_gain < MIC_GAIN_MAX - MIC_GAIN_STEP) {
                    sco_mic_gain += MIC_GAIN_STEP;
                } else {
                    sco_mic_gain = MIC_GAIN_MAX;
                }
            }
            //printf("%03d ", sco_mic_gain);

            for(u8 k = 0; k < 24; k++) {
                sco_mul_s16_s16(dst, sco_mic_gain);
                dst++;
            }
            bt_uart_tx(uartTxQ3, 4 + 24*2);	// 24 samples

            //bt_uart_tx((void*)sco_header, 4);
            //bt_uart_tx(sco_buf_ptr, 24*2);	// 24 samples

            sco_buf_ptr += 24*2;
            if (sco_buf_ptr >= &mp3_adc_buf[0xd80])
            {
                sco_buf_ptr = mp3_adc_buf;
            }
            u8 temp_ea = IE_EA;
            IE_EA = 0;
            agc_buf_cnt--;
            IE_EA = temp_ea;
        }

    #else   //�°���Զ��˵��ʱ����Ϊ��������������(ѹ����)����������������ʱMIC����Ҳ������0��
            //Զ�˼����ض���˵��ʱ����MIC���滺������(������С)��
            //��ʹuart���ڸ�����IC6639����uart���ݣ�Ҳ����sco_buf_ptr��agc_buf_cnt��Щ����

        if(agc_buf_cnt > -10) //��ʹuart���ڸ�����IC6639����uart���ݣ�Ҳ����sco_buf_ptr��agc_buf_cnt��Щ����
        {
            //HciSendSco(sco_buf_ptr);
			//uartTxQ3[0]=0x03;//packet type
			//uartTxQ3[1]=0x5C;
			//uartTxQ3[2]=0x00;
			//uartTxQ3[3]=0x30;//48 bytes, 24 PCM samples

            //u16 power = *((u16*)agc_far_wpos);
            //agc_far_wpos++;
            //printf("%3d,%3d\n", power,sco_mic_gain);
            //printf("{%2d,%d}", sco_spk_stable, sco_spk_vad);//power);
            //uart_putchar('0'+sco_spk_vad);

//            calc_spk_power(power);

        #ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_
            if(Test_Mode_Flag == 3)
                sco_mic_gain = MIC_GAIN_MAX;   // ��MIC����ʱ��������������
        #endif
            //if(*power > 100)
            //    my_memset(uartTxQ3 + 4, 0, 24*2);     // freq_666
            //else
            //    my_memcpy(uartTxQ3 + 4, sco_buf_ptr, 24*2);     // freq_666
            s16 *dst = (void*)(uartTxQ3 + 4);
            s16 *src = (void*)sco_buf_ptr;

			{	//BT_MIC�޸�Ϊ16K������
                u8 k;
                s16 sig;
                extern __near_func s16 fir_lp_proc(s16 sample);
                for (k = 0; k < 48; k++) {
                    sig = fir_lp_proc(src[k]);
                    if (k & 0x01) {
                        src[k/2] = sig;
                    }
                }
            }

            for(u8 k = 0; k < 24; k++) {
                //*dst = highpass(src);	//�رո�ͨ�˲�
                *dst = *src;
                src++;
                dst++;
            }
            dst = (void*)(uartTxQ3 + 4);
            u16 mic_pow = voice_maxpow_step(dst, 24, 1);
            //printf("%04x\n", mic_pow);

            /***********************************************************************************************
            ����⵽Զ����˵���󣬻��ڴ˴��ټ���MUTEס����MIC sco_near_mic_mute * 3ms,�˺�Ż�򿪱���MIC��
            �������Ƚ����ؿ��԰� sco_near_mic_mute ��ֵ����һ��(0~255).
            ***********************************************************************************************/
            u8 temp_sco_far_vad = sco_far_vad;
            if (sco_far_vad_last && sco_far_vad == 0) {
                sco_near_mic_mute = 0;//100;                             //������ģʽ����MUTE100 * 3ms
            }
            sco_far_vad_last = sco_far_vad;
            if (temp_sco_far_vad) {
                    mic_pow = 0;
            } else if (sco_near_mic_mute) {
                sco_near_mic_mute--;
                if (mic_pow <= 0x1000) {
                    mic_pow = 0;
                    temp_sco_far_vad = 1;
                } else {
                    //printf("X");
                    sco_near_mic_mute = 0;
                }
            }
//printf("%x,", mic_pow >> 8);
            if (sco_mic_pow) {
                if (mic_pow <= 0x800) {
                    sco_mic_stable++;
                    if (sco_mic_stable >= 10) {
                        sco_mic_pow = 0;
                        sco_mic_stable = 0;
                    }
                } else {
                    sco_mic_stable = 0;
                }
            } else {
                if (mic_pow >= 0x1000) {
                    sco_mic_stable++;
                    if (sco_mic_stable >= 10) {
                        sco_mic_pow = 1;
                        sco_mic_stable = 0;
                    }
                } else {
                    sco_mic_stable = 0;
                }
            }

            if(mic_on_time){
                mic_on_time--;
            }
//printf("(%x,%x) ", sco_mic_gain, spk_on_time);
//printf("%x ",spk_on_time);
            if (temp_sco_far_vad /*|| !sco_mic_pow*/) {    // far is talking..... or mic is mute
                //printf("0");
                if(sco_mic_gain > MIC_GAIN_STEP){
                    sco_mic_gain -= MIC_GAIN_STEP;
                }
                else{
                    sco_mic_gain = 2;//2
                }

                /**************************************************************************************
                ������MIC��˵��ʱ����⵽Զ����˵�����򱾵�MIC���治���̽���0��������mic_on_time * 3ms
                ʱ��������MIC_GAIN_STEP������С������С��С����sco_mic_gain = 20�����ڴ�ʱ�����Żή��
                ��sco_mic_gain = 2;��.�˴������һ���Ļ������ʰѡ�sco_mic_gain = 20����ֵ��Сһ��Ի���
                ��һ�����ơ�
                **************************************************************************************/
                if(mic_on_time){
					if(sco_mic_gain > 30){
						sco_mic_gain -= MIC_GAIN_STEP;
					}
                    else{
                        sco_mic_gain = 20;
                    }
                }

                //if(sco_mic_gain <= 20 && spk_on_time < 65535){
	            	spk_on_time++;
                //}
/*
                //printf("%x ", sco_mic_gain);

                if(sco_mic_gain > MIC_GAIN_STEP)
                    sco_mic_gain -= MIC_GAIN_STEP;
                else
                    sco_mic_gain = 10;
*/
            }
        #if 0
            else if(!sco_mic_pow){  //��������������ʱMIC����Ҳ������0��
                    // MIC���봦��
            #define   MIC_NOISE_GAIN_MAX    (MIC_GAIN_MAX/4)
            #define   MIC_GAIN_INC_STEP       4
                if (sco_mic_gain > MIC_NOISE_GAIN_MAX) {
                    if (sco_mic_gain > MIC_NOISE_GAIN_MAX + MIC_GAIN_STEP) {
                        sco_mic_gain -= MIC_GAIN_STEP;
                    } else {
                        sco_mic_gain = MIC_NOISE_GAIN_MAX;
                    }
                } else {
                    if (sco_mic_gain < MIC_NOISE_GAIN_MAX - MIC_GAIN_INC_STEP) {
                        sco_mic_gain += MIC_GAIN_INC_STEP;
                    } else {
                        sco_mic_gain = MIC_NOISE_GAIN_MAX;
                    }
                }
            }
        #endif
            else {  //Զ����˵��������˵��ʱ������MIC���滺������(������С)��
				spk_on_time = 0;
                /*if(spk_on_time){
                    spk_on_time--;
                }*/

                if(sco_mic_pow && sco_mic_gain == MIC_GAIN_MAX){
                	mic_on_time = 400;	// 400 * 3 ms
                }
                else{
                    mic_on_time = 0;
                }

                //printf("1");
                #ifndef MIC_GAIN_INC_STEP
                #define MIC_GAIN_INC_STEP       4
				#endif
                if(sco_mic_gain < MIC_GAIN_MAX - MIC_GAIN_INC_STEP) {
                    sco_mic_gain += MIC_GAIN_INC_STEP;
                } else {
                    sco_mic_gain = MIC_GAIN_MAX;
                }
            }
            //printf("%03d,", sco_mic_gain);

            for(u8 k = 0; k < 24; k++) {
                sco_mul_s16_s16(dst, sco_mic_gain);
                dst++;
            }
            if(bt_uart_txlen == 0)
            {
			#if !SCO_SUPERVISION_CHECK
                bt_uart_tx(uartTxQ3, 4 + 24*2);	// 24 samples
			#else
				// ����SCO_SUPERVISION_CHECK��ͨ��ʱ����Ƿ�ͨ�ű���е��¶��ߵĿ��ܲ��޸���
                extern u8 sco_supervision_cnt;
                if(sco_supervision_cnt < 2){
                    bt_uart_tx(uartTxQ3, 4 + 24*2);	// 24 samples
                }

                // get
                extern u16 scoconn_addr;    //keil c pointer to xdata , supervision.tmroffset; 0x27
                extern u16 aclconn_addr;
                static u16 read_xdata_timer = 0;
                read_xdata_timer++;
                if(read_xdata_timer >= 500/3 && sco_supervision_cnt < 0x10){
                    read_xdata_timer = 0;

                    //u16 addr = scoconn_addr + 0x27;
                    u16 addr = aclconn_addr+0x18;

                    //printf("xdata %04x\n", addr);
                    uartTxQ3[4+0] = 0x01;
                    uartTxQ3[4+1] = 0x07;
                    uartTxQ3[4+2] = 0xFC;
                    uartTxQ3[4+3] = 0x02;
                    uartTxQ3[4+4] = addr;
                    uartTxQ3[4+5] = addr>>8;
                    bt_uart_tx(uartTxQ3+4, 6);	// 24 samples
                }
			#endif
            }

            //bt_uart_tx((void*)sco_header, 4);
            //bt_uart_tx(sco_buf_ptr, 24*2);	// 24 samples

            sco_buf_ptr += 48*2;	//BT_MIC�޸�Ϊ16K������
            if (sco_buf_ptr >= &mp3_adc_buf[0xd80])
            {
                sco_buf_ptr = mp3_adc_buf;
            }
            u8 temp_ea = IE_EA;
            IE_EA = 0;
            agc_buf_cnt--;
            IE_EA = temp_ea;
        }
    #endif
#endif
	}
}

#pragma location="BT_SCO_SEG"
__near_func u8 bt_voice_softisr_scodo(void)
{
        Process_Agc_Data();

        while (sco_farpcm_len && AUCON5 < 0x88) {
            temp_sample = *sco_farpcm_rptr++;
            WATCHDOG_CLR();
            if(sco_farpcm_rptr >= &sco_farpcm[SCO_FARPCM_SIZE])
                sco_farpcm_rptr = sco_farpcm;

            bool temp_ea = IE_EA;
            IE_EA = 0;//temp_ea;
            sco_farpcm_len--;
            IE_EA = temp_ea;
/*
			if(spk_on_time > 400){	//Զ��˵��ʱ��ϳ�ʱ����һ�£���0���Լ�Ȿ���Ƿ���˵��
				if(spk_on_time < 450)
					temp_sample = 0;//,printf("m ");
				else
					spk_on_time = 0;
			}
*/
            sco_echo_process(temp_sample>>8);

            //sample = sample >> 1;
            if (AUCON5 <= 0x18) {
                //���� -0.3%
                resample_step_frac = 59;
                resample_step = 255;
                resample_step_high = 0;
            } else if (AUCON5 >= 0x30) {
                //���� +1%
                resample_step_frac = 143;
                resample_step = 2;
                resample_step_high = 1;
            } else if (AUCON5 >= 0x20) {
                //�����ٶ�
                resample_step_frac = 0;
                resample_step = 0;
                resample_step_high = 1;
            }

            if (sco_spk_vad_last == 0) {
                //temp_sample = 0;
            }
            sco_pcm_out(temp_sample);

/*            if (far_vol != 255) {
                sco_mul_s16_s16(&temp_sample, far_vol);
            }
            sco_pcm_out(temp_sample);*/
/*
            while(!(AUCON7 & BIT(7)));
            AUCON5 = sample >> 8;
            AUCON5 = sample;
            while(!(AUCON7 & BIT(7)));
            AUCON5 = sample >> 8;
            AUCON5 = sample;
*/
        }
        return 0;
}

__near_func u8 bt_voice_softisr(void)
{
    if (!flag_btmode) {
#if AUX_SOUND_DETECT_EN || FM_SOUND_DETECT_EN
        sound_detect_process();
#endif
        return 0;
    }
    else if(bt_voice_state != BT_VMS_IDLE)
    {
        bt_voice_play();
        return 1;
    }
#if !BT_CALL_PRIVATE
    //ǿ��˽�ܽ���������Ҫ��һ��
    else if (scoflag)
    {
        bt_voice_softisr_scodo();
        return 1;
    }
#endif
#if BT_DYMANIC_SUPPRESSION
    else if (!avdtp_play_status) {  //�ǲ���״̬
        sound_detect_process();
    }
#endif
    return 0;
}

#pragma location="BT_API_SEG"
void bt_enter_hshf()
{
#if BT_DYMANIC_SUPPRESSION
    sound_detect_disable();     //����ǰ�Ƚ�ֹ�������
#endif
    avdtp_sbc_reset();
    if(bt_hshf_flag == 0) {
        bt_hshf_flag = 1;
    }
    //printf("en_hshf, %d\n", bt_hshf_flag);
}

#pragma location="BT_API_SEG"
void bt_enter_hshf_do()
{
    bt_hshf_flag = 2;

    //avdtp_sbc_reset(); // avdtp_sbc_len = 0;
    //while(sbc_dec_busy){}   // wait sbc decode complete
#if !BT_CALL_PRIVATE
    dac_fade_out(1);            //�ȵ���DAC��������DACž��
#endif

    AUCON0 = 0x06;  //RST
    AUCON9 = 0x04;
    //printf("en_hshf_do\n");
    avdtp_sbc_reset(); // avdtp_sbc_len = 0;

    bool temp_sint = IE_SINT;
    IE_SINT = 0;

    bt_voice_pcmrd = (void*)sco_farpcm;
    bt_voice_pcmwr = (void*)sco_farpcm;

    sco_mic_gain = MIC_GAIN_MAX;
    memset(sco_spk_vad, 0, sizeof(sco_spk_vad));
    sco_spk_vad_index = 0;
    sco_spk_vad_mute = 0;

    //sco_mic_vad = 1;
    sco_far_vad = 0;
    sco_far_vad_last = 0;
    sco_spk_stable = 0;

    sco_mic_pow = 1;
    sco_mic_stable = 0;

    mic_on_time = 0;
    spk_on_time = 0;

    sco_cal_index = 0;
    sco_cal_max = -128;
    sco_cal_min = 127;

    sco_farpcm_len = 0;
    sco_farpcm_wptr = sco_farpcm;
    sco_farpcm_rptr = sco_farpcm;
    memset(sco_farpcm, 0, sizeof(sco_farpcm));

    agc_buf_cnt = -12;
    sco_buf_ptr = mp3_adc_buf;

    sbc_pcm_len = 0;

    resample_delta_high = 1;
    resample_delta = 0;
    resample_delta_frac = 0;

    resample_step_frac = 0;
    resample_step = 0;
    resample_step_high = 1;

    resample_rate = 0;
    resample_rate_fast = 0;

    l_sample1 = l_sample2 = 0;

    far_vol = 255;
    far_noise_pow = 0;
    far_noise_pow_cnt = 0;

    uartTxQ3[0]=0x03;//packet type
    uartTxQ3[1]=0x5C;
    uartTxQ3[2]=0x00;
    uartTxQ3[3]=0x30;//48 bytes, 24 PCM samples

    hp_init();
    fir_lp_init();

    //bool temp_sint = IE_SINT;
    //IE_SINT = 0;
    sbc_load_code(DCODE_BT_SCO, 1);

#if !BT_CALL_PRIVATE
    dac_anl_fade_wait();
    dac_disable();
    mp3en_source_init(BT_MIC);
    dac_enable();
    dac_fade_in(1);
    mp3en_start(false);
    IE_SINT = 0;
#endif

    player_set_dac(8);
    agc_buf_cnt = 0;
    bt_debug_sco = 0;
    scoflag=1;
    try_dis_sco = 0;
    //bt_voice_msg = 0;
    //bt_voice_outptr = 0;
    //bt_voice_inptr = 0;
    //bt_voice_state = BT_VMS_IDLE;
    IE_SINT = temp_sint;

#if !BT_CALL_PRIVATE
    user_change_volume(sys_ctl.hshf_vol);
#endif
}

#pragma location="BT_API_SEG"
void bt_exit_hshf(void)
{
    //printf("exit_hf, %d\n", bt_hshf_flag);
    if(bt_hshf_flag == 2) {
#if !BT_CALL_PRIVATE
        dac_fade_out(1);            //�ȵ���DAC��������DACž��
        dac_anl_fade_wait();
#endif
        sco_farpcm_len = 0;
#if !BT_CALL_PRIVATE
        mp3en_stop();
        mp3en_exit();               //2014-01-25, ���������绰������ģʽ��EQ��������
#endif
        clr_dac_buf();
        music_unmute();
        dac_enable();
        dac_fade_in(1);

        IE_SINT = 0;
        scoflag=0;
        try_dis_sco = 0;
        sco_farpcm_len = 0;
        sco_farpcm_wptr = sco_farpcm;
        sco_farpcm_rptr = sco_farpcm;
        delay_5ms(25);                  //�ȴ�DAC�������
        sbc_load_code(DCODE_BT_SBC, 1);
        sbc_init();                     //���������ж�
#if !BT_CALL_PRIVATE
        user_change_volume(sys_ctl.volume);
#endif
    }
    bt_hshf_flag = 0;
#if BT_DYMANIC_SUPPRESSION
    sound_detect_enable(1);
#endif
}

//������ĺ궨��
#pragma location="TASK_BT_SEG"
void sbc_hw_init_hook(void)
{
    AUCON0 = 0x06;    // sync with dac enable
}

#pragma location="BT_DEC_INT_SEG"
__near_func void sbc_subbands_hook(void)
{
}

extern IAR_DATA_A u8 dac_chk_cnt;
extern IAR_BIT_A bool dac_chk_flag;

__near_func bool mp3_isr_sbchook(void)
{
    if (dac_chk_cnt) {

    }
    if (AUCON5 >= dac_chk_cnt) {
        dac_chk_flag = 1;
        return 0;
    }
    return 1;
}

void avdtp_play_start()
{
    //delay_5ms(10);  // delay 50ms to start sbc

    while (AUCON5 < 0x88)
    {
        while(!(AUCON7 & BIT(7)));
        AUCON5 = 0x00;
        AUCON5 = 0x00;
        while(!(AUCON7 & BIT(7)));
        AUCON5 = 0x00;
        AUCON5 = 0x00;
    }

    avdtp_play_status = 1;
    //KICK_SOFT_INT();
}

u8 send_bat_to_iphone(u8 batval) @ "BT_RFC_TX"
{
    extern IAR_CONST u8 at_iphoneaccev_str[];   //  = "AT+IPHONEACCEV=1,1,8";
    ((u8*)at_iphoneaccev_str)[19] = batval+'0';
    return hsf_send_atcmd((u8*)at_iphoneaccev_str, 20);
}

//����ͨ�������ӿ�
extern u8 send_at_vgs(u8 vol);
u8 send_hshfvol_hook(void) @ "BT_RFC_TX"
{
    return send_at_vgs(sys_ctl.hshf_vol);
}

//�ֻ��˸ı�ͨ�������ӿ�
void hshfvol_change_hook(u8 vol) @ "BT_RXAT_CMD"
{
#if !BT_CALL_PRIVATE
    user_set_hshfvol(vol);
    put_msg(BT_PHONE_CHANGE_HSHFVOL);
#endif
}

//�ֻ��˸ı�ý�������ӿڣ�1ΪUP, 0ΪDOWN
#pragma location="BT_AVCTP_RX"
void a2dpvol_change_hook(u8 mode)
{
    if (mode == 0) {
        put_msg(BT_PHONE_VOL_DOWN);
    } else {
        put_msg(BT_PHONE_VOL_UP);
    }
}

#if BT_FT_APPLE_VOL_CTRL_EN
#pragma location="BT_AVCTP_RX"
void a2dpvol_set_hook(u8 vol)
{
    vol = (vol+2)/8;				//iphone�˵��豸������ ����iphone������Χ��0~0x7f��Ҫת��Ϊ�豸������VOLUME_MAX����ܱ���������16��32
    if(vol != sys_ctl.volume) {
        user_set_volume(vol);
        show_volume();
    }

    //vol = (vol+(0x80/VOLUME_MAX-1))/(0x80/VOLUME_MAX);  //������ʱ���ô˹�ʽ����
    //if(vol != sys_ctl.volume) {
    //    user_set_volume(vol);
    //    show_volume();
    //}
}

#pragma location="BT_AVCTP_RX"
u8 a2dpvol_get_hook(void)
{
    u8 vol = sys_ctl.volume*8+2;	//iphone��ȡ�豸������Ҫת��Ϊiphone������VOLUME_MAX����ܱ�����
    if(vol < 7) {
        return 0;
    } else if(vol > 0x77) {
        return 0x7f;
    } else {
	    return vol;
    }

    //vol = sys_ctl.volume*(0x80/VOLUME_MAX);  //������ʱ���ô˹�ʽ����
    //if(vol > 0x7f) {
    //    vol = 0x7f;
    //}
    //return vol;
}
#endif

//ֻ��һ�����ռ�ʱ���������ŷ�������ֵ
#pragma location="BT_HID"
void hid_key_camera(void)
{
#if BT_HID_EN
    SendHidButton(0x28);//enter, android4.0����
    for(u8 i=0; i<10; i++) {
        BtMainLoop();   //���ӵ���ʱ��������Щ�ֻ���Ӧ������
    }
    SendHidConsumer(0xe9);//volume up, ios
#endif
}

void bt_test_key(void)
{
    if (Test_Mode_Flag == 2) {
        bt_voice_put(BT_VOICE_KEY);
    }
}

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

#if !BT_EV3_EN
bool bt_is_open_ev3(void)
{
    return false;
}
#endif

__near_func void set_clk32k_offset(u8 ofs);
__near_func void set_rc_offset(u8 ofs);
#pragma location="BT_DEC_SEG"
__near_func void set_clk_offset(u8 ofs)
{
#if SYS_CRY == CRY_32K
    set_clk32k_offset(ofs);
#elif SYS_CRY == CRY_RC
    set_rc_offset(ofs);
#endif
}
