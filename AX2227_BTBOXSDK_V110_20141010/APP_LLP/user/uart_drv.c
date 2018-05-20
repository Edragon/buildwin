/*****************************************************************************
* Module    : BTSTACK
* File      : uart_drv.c
* Author    : Hanny
* Email     : coldney@163.com
* Function  : Uart发送与接收控制
*****************************************************************************/
#include "include.h"
#include "uart_drv.h"
#include "btapi.h"
#include "hal_uart.h"

__near_func void bt_uart_txset(void *buf, u8 len);
__near_func void bt_uart_transfering(void);
__near_func void bt_uart_receiving(void);
__near_func u8 bt_uart_rxget(void);

IAR_DATA_A volatile u8 bt_uart_txptr @ "UART_DATA";     //发送指针
IAR_DATA_A volatile u8 bt_uart_txlen @ "UART_DATA";     //待发送长度
IAR_DATA_A volatile u8 bt_uart_rxptr @ "UART_DATA";     //发送指针
IAR_DATA_A volatile u8 bt_uart_rxlen @ "UART_DATA";
#pragma data_alignment=256
IAR_XDATA_A u8 bt_uart_txbuf[256] @ "BT_UART_TXBUF";
IAR_XDATA_A u8 bt_uart_rxbuf[256] @ "BT_UART_RXBUF";

#pragma data_alignment=2
IAR_XDATA_A s16 hp_coef[5];
IAR_XDATA_A s16 hp_temp[4];


__near_func void btrx_isr(unsigned char rdata);
__near_func void bt_uart_rxskip(u8 len);
__near_func void bt_uart_getbytes(void *buf, u8 len);

extern u8  uartRxState;
extern u8 __xdata* urIsrRxPtr;	//pointer
extern u16  urIsrRxLen1;
extern u16  urIsrRxLen2;	//in case (urIsrRxLen1 > RQ2_LEN_ACL_PLD)
extern u8   urIsrRxEvtLen1;

#pragma constseg="HP_INIT_CONST"
/*
#pragma data_alignment=2
IAR_CONST u8 test_data[256] = {
	0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x02, 0x00, 0x01, 0x00, 0xFF, 0xFF, 0x01, 0x00, 0x02, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x03, 0x00, 0x04, 0x00, 0x03, 0x00, 0x01, 0x00, 0x05, 0x00,
	0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x04, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08, 0x00, 0x04, 0x00,
	0x03, 0x00, 0x08, 0x00, 0x06, 0x00, 0x07, 0x00, 0x01, 0x00, 0xFD, 0xFF, 0x00, 0x00, 0xFE, 0xFF,
	0x00, 0x00, 0xFC, 0xFF, 0xF9, 0xFF, 0xF8, 0xFF, 0xF9, 0xFF, 0xF9, 0xFF, 0xF8, 0xFF, 0xF8, 0xFF,
	0xF1, 0xFF, 0xF7, 0xFF, 0xF8, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xF8, 0xFF, 0xFE, 0xFF, 0x02, 0x00,
	0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFB, 0xFF, 0xF9, 0xFF, 0x00, 0x00, 0x01, 0x00, 0xFA, 0xFF,
	0xFF, 0xFF, 0x00, 0x00, 0x02, 0x00, 0xFD, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0x03, 0x00, 0xFF, 0xFF,
	0xFB, 0xFF, 0xFC, 0xFF, 0xFE, 0xFF, 0x07, 0x00, 0x03, 0x00, 0x03, 0x00, 0xFF, 0xFF, 0x01, 0x00,
	0x01, 0x00, 0xFD, 0xFF, 0xFF, 0xFF, 0xFC, 0xFF, 0xFD, 0xFF, 0xFD, 0xFF, 0xF7, 0xFF, 0x00, 0x00,
	0xFD, 0xFF, 0x00, 0x00, 0xFE, 0xFF, 0xF4, 0xFF, 0xFC, 0xFF, 0xF9, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF,
	0xF6, 0xFF, 0xF7, 0xFF, 0xF2, 0xFF, 0xF1, 0xFF, 0xF3, 0xFF, 0xF1, 0xFF, 0xF2, 0xFF, 0xF6, 0xFF,
	0xF4, 0xFF, 0xF5, 0xFF, 0xF2, 0xFF, 0xEF, 0xFF, 0xF5, 0xFF, 0xEF, 0xFF, 0xEF, 0xFF, 0xE8, 0xFF,
	0xEA, 0xFF, 0xE8, 0xFF, 0xE8, 0xFF, 0xE7, 0xFF, 0xEA, 0xFF, 0xEE, 0xFF, 0xE6, 0xFF, 0xEA, 0xFF,
	0xEE, 0xFF, 0xEE, 0xFF, 0xEC, 0xFF, 0xEE, 0xFF, 0xEC, 0xFF, 0xEF, 0xFF, 0xF0, 0xFF, 0xED, 0xFF,
	0xF7, 0xFF, 0xF5, 0xFF, 0xF6, 0xFF, 0xF7, 0xFF, 0xFD, 0xFF, 0x04, 0x00, 0x06, 0x00, 0x05, 0x00
};
*/
IAR_CONST s16 tbl_hp_coef[5] = {13488, -26975, 13488, 26459, -11107};
#pragma constseg=default

#pragma location="BT_UART_ROUTE"
__near_func void bt_uart_route(void)
{
    u8 try_cnt = 255;
    while (bt_uart_rxlen && try_cnt) {
        if (bt_uart_rxlen == 255) {
            //printf("[ov]\n");               //over flow
        }
        bt_sleep_cnt = BT_SLEEP_CNT;
        if(uartRxState == 0x33) //wait for hci SCO payload
        {
            if(bt_uart_rxlen >= 2){
                u8 low_byte = bt_uart_rxget();//uartRxQ3.pcm[uartRxQ3_w_idx & RQ3_INDEX_MASK].byte.lo;
                u8 rdata = bt_uart_rxget();
                //try_cnt -= 2;
                if (scoflag && bt_voice_msg == 0)         //等Buffer不满
                {
                    if(sco_farpcm_len < SCO_FARPCM_SIZE){
                        *sco_farpcm_wptr++ = (rdata<<8) | low_byte;
                        if(sco_farpcm_wptr >= &sco_farpcm[SCO_FARPCM_SIZE])
                            sco_farpcm_wptr = sco_farpcm;
                        sco_farpcm_len++;
                    }
                }
                if(urIsrRxEvtLen1 <= 2) {//finish receive a hci SCO packet
                    uartRxState=0x00;//back to original
                }
				urIsrRxEvtLen1 -= 2;
            }
        }
        else if (uartRxState == 0x24) {
            u8 rxlen = try_cnt;
            if (rxlen > bt_uart_rxlen) {
                rxlen = bt_uart_rxlen;
            }
            if (urIsrRxLen2) {
                if (rxlen > urIsrRxLen2) {
                    rxlen = urIsrRxLen2;
                }
                bt_uart_getbytes(urIsrRxPtr, rxlen);
                urIsrRxPtr += rxlen;
                urIsrRxLen2 -= rxlen;
            } else {
                if (rxlen > urIsrRxLen1) {
                    rxlen = urIsrRxLen1;
                }
                bt_uart_rxskip(rxlen);
            }
            try_cnt -= rxlen;
            urIsrRxLen1 -= rxlen;
            if (urIsrRxLen1==0) {  //finish receive a hci ACL packet
//__near_func void show_bt_info();
//show_bt_info();
                uartRxQ2_w_idx++;  //increment write_index @uart_isr, after roger completed
                uartRxState=0x00;  //back to original
            }
        } else {
            try_cnt--;
            btrx_isr(bt_uart_rxget());
        }
    }
}

__near_func void bt_uart_route_hook(void)
{
    if (BT_ALL_TASK || flag_btmode) {
        bt_uart_route();
    }
}

#pragma location="INIT_SEG"
void bt_uart_var_init(void)
{
    bt_uart_txptr = 0;
    bt_uart_txlen = 0;
    bt_uart_rxptr = 0;
    bt_uart_rxlen = 0;
}

//蓝牙驱动的串口发送接口
#pragma location="BT_UART_TX_SEG"
__near_func void bt_uart_tx(void *buf, u8 len)
{
    //if (len == 0) {
    //    return;
    //}
    bool temp_sint = IE_SINT;
    IE_SINT = 0;
    WATCHDOG_CLR();

    //u8 tmp = 255 - len;
    //while (tmp < bt_uart_txlen);

    if (bt_sleep_cnt == 0) {
        u8 timeout = 5;    // 5 * 20ms == 100ms
        //printf("WK\n");
        while(timeout--){
            UART1DATA = 0xff;
            delay_5ms(5);   // delay 20ms
            WATCHDOG_CLR();
            if(uartRxQ4_w_idx != uartRxQ4_r_idx)
                break;
        }
    }
    bt_uart_txset(buf, len);
    //printf_array(bt_uart_buf, 32);

    bool temp_ea = IE_EA;
    IE_EA = 0;
    if (UART1STA & BIT(2)) {
        bt_uart_transfering();
    }
    IE_EA = temp_ea;
    IE_SINT = temp_sint;

    while (bt_uart_txlen);

}

#pragma location="HP_INIT_SEG"
void hp_init(void)
{
    memset(hp_temp, 0, sizeof(hp_temp));
    memcpy(hp_coef, (u8 *)tbl_hp_coef, sizeof(tbl_hp_coef));
/*
    s16 *test_ptr = (s16 *)test_data;
    u8 i;
    for (i=0; i!=128; i++) {
        printf("%04x\n", highpass(test_ptr++));
        printf("%04x %04x %04x %04x\n", hp_temp[0], hp_temp[1], hp_temp[2], hp_temp[3]);
    }

    WDTCON = 0;
    while (1);
    */
}
