/*****************************************************************************
 * Module    : User
 * File      : user_bt.c
 * Author    : Hanny
 * Function  : ����SPP�����ӿ�
 *****************************************************************************/
#include "include.h"

#define DBG_BT_SPP_EN           0
#define SPP_MSG_NUM             9

typedef struct {
    u8 msg;
    char str[8];
} TYPE_SPP_MSG;

//���͸�ʽΪAT#**BW ��BW��һ��Ҫ��һ����ո�ȵ������ַ�
#pragma constseg="SPP_RX_CONST"
IAR_CONST char tbl_spp_sig[] = "BW";
IAR_CONST TYPE_SPP_MSG tbl_spp_msg[SPP_MSG_NUM] = {
    {KU_PREV, "AT#ME"},
    {KU_NEXT, "AT#MD"},
    {K_VOL_UP, "AT#VU"},
    {K_VOL_DOWN, "AT#VD"},
    {KU_PLAY, "AT#MA"},
    {KU_MODE, "AT#MO"},
    {Q_SPP_MSC_GET_TOTAL, "AT#GT"}, //AT#GTBW :��ȡ����Ŀ
    {Q_SPP_MSC_GET_LFN, "AT#GF"},   //AT#GF0001BW :��ȡ��һ�׸������ļ���
    {Q_SPP_MSC_GET_NUM, "AT#GN"},   //AT#GNBW :��ȡ��ǰ��Ŀ
};
IAR_CONST char tbl_spp_settime[] = "AT#RT";
IAR_CONST char tbl_spp_seteq[] = "AT#EQ";//����EQ

IAR_XDATA_A u8 bteq_buf[123] @ "BTEQ_BUF";

bool eqflag = FALSE;
#pragma constseg=default

//CHAR ת������ֵ
#pragma location="SPP_RX_SEG"
u8 char_to_num(u8 ch)
{
    if (ch >= '0' && ch <= '9') {
        return (ch - '0');
    }
    return 0;
}

//SPP���մ���
#pragma location="SPP_RX_SEG"
void spp_rx_msg(u8 *ptr, u8 len)
{
    if (0 == memcmp(&ptr[len-3], (char *)tbl_spp_sig, 2)) {
        //SPPͬ��RTCʱ��
        if (0 == memcmp(ptr, (char *)tbl_spp_settime, 5)) {
#if BT_SPP_EQ_EN==0
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
#endif
        }
        else if (0 == memcmp(ptr, (char *)tbl_spp_seteq, 5)) { //����eq����
            eqflag = true;
        }
        else {
            //SPP��Ϣת������Ϣ
            u8 i;
            for (i=0; i!=SPP_MSG_NUM; i++) {
                //printf("str:%s\n", tbl_spp_msg[i].str);
                if (0 == memcmp(ptr, (char *)tbl_spp_msg[i].str, 5)) {
#if BT_SPP_MSC_EN
                    //printf("msg:%02x\n", tbl_spp_msg[i].msg);
                    if(tbl_spp_msg[i].msg == Q_SPP_MSC_GET_LFN) {
                        u8 tmp;
                        u16 num = 0;
                        for(u8 j=5; j<10; j++) {
                            if(ptr[j] == '\0') {
                                break;
                            }
                            tmp = char_to_hex(ptr[j]);
                            if(tmp == 0xff) {
                                return;
                            }
                            num *= 10;
                            num += tmp;
                        }
                        //printf("num: %d\n", num);
                        spp_msc_lfn_num = num;
                    }
#endif
                    put_msg(tbl_spp_msg[i].msg);
                    return;
                 }
             }
        }
    }
    else{
#if BT_SPP_EQ_EN
        if(eqflag == true)
        {
            if(123 == len) //123��eq����
            {
                eqflag = FALSE;
                for(u8 i=0;i<len;i++)
                    bteq_buf[i] = ptr[i];
               }

        }
#endif
    }
}

//SPP���յ�������
#pragma location="SPP_RX_SEG"
void spp_rx(u8 *ptr, u8 len)
{
#if DBG_BT_SPP_EN
   printf("SPP_RX(%x): %s\n", len, ptr);
#endif
   if (len >= 8) {
       spp_rx_msg(ptr, len);
   }
}

//SPP���͵�����
#pragma location="SPP_TX_SEG"
void spp_tx(char *ptr, u8 len)
{
#if DBG_BT_SPP_EN
    printf("SPP_TX(%x): %s\n", len, ptr);
#endif
    bt_send_spp_ex(ptr, len);
}

#if BT_SPP_MSC_EN

IAR_XDATA_A u8 spp_buf[100+3] @ "DIR2_LFN";   //��BUFֻ���ڷ�����ģʽ�·���SPP��Ϣ
IAR_XDATA_A u16 spp_msc_lfn_num;

//SPP��ȡ��������Ŀ
#pragma location="SPP_TX_SEG"
void spp_tx_msc_total(u16 total)
{
    spp_buf[0] = 'M';   //music
    spp_buf[1] = 'T';   //total
    for(u8 i=6; i>1; i--) {
        spp_buf[i] = hex_to_char(total%10);
        total /= 10;
    }
    spp_buf[7] = '\0';

    spp_tx((char *)spp_buf, 8);
}

//SPP��ȡ��ǰ��Ŀ
#pragma location="SPP_TX_SEG"
void spp_tx_msc_num(u16 num)
{
    spp_buf[0] = 'M';   //music
    spp_buf[1] = 'N';   //num
    for(u8 i=6; i>1; i--) {
        spp_buf[i] = hex_to_char(num%10);
        num /= 10;
    }
    spp_buf[7] = '\0';

    spp_tx((char *)spp_buf, 8);
}

//SPP��ȡָ����Ŀ���ļ���
#pragma location="SPP_TX_SEG"
void spp_tx_msc_lfn(u16 lfn_len)
{
    spp_buf[0] = 'M';   //mucic
    spp_buf[1] = 'F';   //file name
    spp_buf[lfn_len+2] = '\0';

    spp_tx((char *)spp_buf, lfn_len+3);
}

//SPP��ȡָ����Ŀ���ļ���
#pragma location="SPP_TX_SEG"
void spp_msc_get_lfn(u16 total_num)
{
    u16 lfn_len = 1;
    if((spp_msc_lfn_num <= total_num) && f_search_fname(spp_msc_lfn_num, (char *)&spp_buf[3], 98)) {
        spp_buf[2] = (spp_buf[3+97]==0)? 0x01 : 0x02;      //spp_buf[2]: 0x01=UNICODE, 0x02=GBK
        lfn_len += str_len2(&spp_buf[3], spp_buf[3+97]);   //spp_buf[3~3+97]: filename
        //printf_array(&spp_buf[3], 98);
    } else {
        spp_buf[2] = 0x00;                              //0x00=ERROR
        //printf("error\n");
    }

    spp_tx_msc_lfn(lfn_len);
}
#endif
