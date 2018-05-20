
/*
 *  hal_uart.h
 *
 *  Hardware abstraction layer that provides
 *  - blockwise IRQ-driven read/write
 *  - blockwise write by polling
 *
 */


#include "typedef.h"


int  hal_uart_set_baud(u32 baud);
#ifdef TXQ9EN // 0=disable txq9
bool uart_write(const u8 *p_buf, u16 cnt);
#endif // #ifndef TXQ9EN // 0=disable txq9
void hal_uart_init(void);

__near_func u8 * uartRxQ2_getHciL2cap(void);	//kidd,
						//octet[0]: HCI ACL header, Handle[7:0]
						//octet[1]: HCI ACL header, BC flag,PB flag,Handle[11:8]
						//octet[2]: HCI ACL header, Data total length
						//octet[3]: HCI ACL header, Data total length
						//octet[4~]:HCI ACL payload
u16 * uartRxQ3_getPcm(void);		//kidd
__near_func void HciEventProcess();
__near_func u8 * uartRxQ4_getHciEvent(void);	//kidd
						//octet[0]: HCI Event op
						//octet[1]: HCI Event length of parameters
						//octet[2~]:HCI Event parameters
//__near_func void uartRxQ2_incrementReadIndex(void);	//kidd, after already processed
void uartRxQ3_incrementReadIndex(void);	//kidd, after already processed
U16  uartRxQ3_count(void);		//kidd

extern u8 uartRxQ4_w_idx;
extern u8 uartRxQ4_r_idx;

extern u8 uartRxQ2_w_idx;
extern u8 uartRxQ2_r_idx;

//void uartRxQ3_TxQ3_init(void);		//kidd
//void uartRxQ4_incrementReadIndex(void);	//kidd, after already processed
void uartTxQ1_triggerSend_ifUartTxIdle(void);
void uartTxQ2_triggerSend_ifUartTxIdle(void);
//void uartTxQ3_triggerSend_ifUartTxIdle(void);
__near_func bool HciSendCommand(u16 opCode,u8 parmLen, u8 Data[]);
void HciSendSco();	// 24 PCM samples
void uartTxQ2_tokenHcNumAclDataPkts_increment(u8 increment); //kidd
extern volatile u8 uartTxQ2_tokenHcNumAclDataPkt70; //kidd


