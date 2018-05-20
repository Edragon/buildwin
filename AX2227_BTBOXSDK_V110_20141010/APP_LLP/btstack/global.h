#ifndef __GLOBAL_H__
#define __GLOBAL_H__


#include "include.h"


/*Modify by brent for Merge HSF&A2DP @20120528*/
#define _COMPILE_HSF_
#define _COMPILE_A2DP_

#ifdef _COMPILE_A2DP_
	   #define A2DP_DISCON_COMMON // put a2dp disconnect func. at common bank
       #define _AP_A2DP_
	   #define _SBC_DECODE_      // always defined!!!!
	   #define AVRCP_EN
	   #define AVCTPVOL // enable volume up/down func. in avctp volume cmd
	   #define USE_AVRCP_PAUSE // use avrcp pause cmd to pause a2dp when redial pressed, else use avdtp suspend cmd
	   #define BYPASS_SBCBUFFER  // always defined!!!!
#endif // _COMPILE_A2DP_


#ifdef _COMPILE_HSF_
    #define _AP_HEADSET_HANDSFREE_
    #define _RFCOM_LAYER_
    #define HSF_RINGTONE
    //#define _AUTH_UNDER_RFOCMM_
    //#define AUTO_CONNECT_HF
#endif
/*Modify by brent for Merge HSF&A2DP @20120528*/

//enum
//{
//    _RUN_AP_INIT_  = 0x00,
//    _RUN_AP_HSF_   = 0x01,
//    _RUN_AP_A2DP_  = 0x02,
//    _RUN_AP_SDP_   = 0x03,
//
//    _RUN_AP_IDLE_  = 0xFF,
//};
//
//#define MASTER  0
//#define SLAVE   1
//
//#define FALSE 0
//#define TRUE 1

/*
typedef int BOOL;
typedef int bool;
typedef unsigned char  u8  ;
typedef unsigned short   u16 ;
typedef unsigned long  u32 ;
typedef signed char    s8  ;
typedef signed short     s16 ;
typedef signed long    s32 ;
*/
union u16_tag {
	u16 word ;
	struct  {
#ifdef __IAR_SYSTEMS_ICC__
	u8  lo ;	// [ 7~0]
	u8  hi ;	// [15~8]
#else
	u8  hi ;	// [15~8]
	u8  lo ;	// [ 7~0]
#endif
	} byte ;
};

union s16_tag {
	s16 word ;
	struct {
#ifdef __IAR_SYSTEMS_ICC__
	u8  lo ;	// [ 7~0]
	u8  hi ;	// [15~8]
#else
	u8  hi ;	// [15~8]
	u8  lo ;	// [ 7~0]
#endif
	} byte ;
};

union u32_tag {
	u32 lung ;
	struct {
#ifdef __IAR_SYSTEMS_ICC__
          u8  lo ;	// [ 7~ 0]
          u8  f8 ;	// [15~ 8]
          u8  mm ;	// [23~16]
          u8  hi ;	// [31~24]
#else
          u8  hi ;	// [31~24]
          u8  mm ;	// [23~16]
          u8  f8 ;	// [15~ 8]
          u8  lo ;	// [ 7~ 0]
#endif
	} byte ;
};

union s32_tag {
	s32 lung ;
	struct {
#ifdef __IAR_SYSTEMS_ICC__
          u8  lo ;	// [ 7~ 0]
          u8  f8 ;	// [15~ 8]
          u8  mm ;	// [23~16]
          u8  hi ;	// [31~24]
#else
          u8  hi ;	// [31~24]
          u8  mm ;	// [23~16]
          u8  f8 ;	// [15~ 8]
          u8  lo ;	// [ 7~ 0]
#endif
	} byte ;
};


//extern int printf(char * fmt, ...);

#define BT_DEBUG 1 //remove_debug
#if BT_DEBUG == 1
#define BT_Debug   printf
#define BT_Debug_Buf  uart_putbuf
#else
#define BT_Debug(...)
#define BT_Debug_Buf(...)
#endif

//L2acp debug
#define L2cap_Debug 0
#if L2cap_Debug==1
#define L2cap_Report         printf
#define L2cap_Report_Buf  uart_putbuf
#else
#define L2cap_Report(...)
#define L2cap_Report_Buf(...)
#endif

#define HCIFLW_Debug 0
#if HCIFLW_Debug==1
#define HCIFLW_Report printf
#else
#define HCIFLW_Report(...)
#endif

// rfcomm debug

#define Rfcomm_Debug 0
#if Rfcomm_Debug==1
#define Rfcomm_Report printf
#define Rfcomm_Report_Buf uart_putbuf
#else
#define Rfcomm_Report(...)
#define Rfcomm_Report_Buf(...)
#endif
//other

//bool EnterRegisterFinished;	//YS:Added

#define LA_DEBUG 0
#if LA_DEBUG == 1

#define LA_DEBUG_IO_Mode_OUT				REG32(P7DIR)&= 0xFFFFFF00;
#ifdef _AP_HEADSET_HANDSFREE_
#define LA2_DEBUG_IO_Mode_OUT				REG32(P8DIR)&= 0xFFFF0000; REG32(P6DIR)&= 0xFFFFFF02;
#else
#define LA2_DEBUG_IO_Mode_OUT				REG32(P8DIR)&= 0xFFFF0000; REG32(P6DIR)&= 0xFFFFFF00;
#endif

#define LA_PORT(byteout)   ({u8 tmp_save; tmp_save=(u8)REG32(P7); REG32(P7)=byteout; REG32(P7)=tmp_save;})
#define LA(byteout)   ({ REG32(P7)=byteout;})
#define LA2(byteout)   ({u16 tmp_save; tmp_save=(u16)REG32(P8);tmp_save&=0xFF00; REG32(P8)=byteout|tmp_save;})
#define LA3(byteout)   ({u16 tmp_save; tmp_save=(u16)REG32(P8);tmp_save&=0x00FF; REG32(P8)=(byteout<<8)|tmp_save;})
#define LA23(byteout) ({ REG32(P8)=byteout;})

void la_23(u32 byteout);




//-------------------------------
#define LA_L2CAP2AVDTP_TG  ({u8 tmp_save; tmp_save=(u8)REG32(P7)&BIT(0); if(tmp_save) REG32(P7)&=~BIT(0); else REG32(P7)|=BIT(0);})
#define LA_RQ2_TG  ({u8 tmp_save; tmp_save=(u8)REG32(P7)&BIT(1); if(tmp_save) REG32(P7)&=~BIT(1); else REG32(P7)|=BIT(1);})
#define LA_SBC_DECODER_TG  ({u8 tmp_save; tmp_save=(u8)REG32(P7)&BIT(2); if(tmp_save) REG32(P7)&=~BIT(2); else REG32(P7)|=BIT(2);})


//-------------------------------
// TOGGLE OUTPUT
#define LA_HCI2SBC							REG32(P7) ^=BIT(0)
#define LA_SBC2DECODER						REG32(P7) ^=BIT(1)
//#define LA_SBC_DECODER_TG					REG32(P7) ^=BIT(2)
#define LA_SBC2PCM							REG32(P7) ^=BIT(3)
#define LA_PCM_DAC							REG32(P7) ^=BIT(4)
#define LA_IEE_TOG							REG32(P6) ^=BIT(4)

//-------------------------------
// STEADY OUTPUT
#define LA_L2CAP2AVDTP_HGIH						REG32(P7) |=BIT(0)
#define LA_L2CAP2AVDTP_LOW						REG32(P7) &=~BIT(0)
#if 0
#define LA_SBC2DECODER_HGIH					REG32(P7) |=BIT(1)
#define LA_SBC2DECODER_LOW					REG32(P7) &=~BIT(1)
#define LA_RQ2_HGIH
#define LA_RQ2_LOW
#else
#define LA_SBC2DECODER_HGIH
#define LA_SBC2DECODER_LOW
#define LA_RQ2_HGIH							REG32(P7) |=BIT(1)
#define LA_RQ2_LOW							REG32(P7) &=~BIT(1)
#endif
#define LA_SBC_DECODER_HGIH					REG32(P7) |=BIT(2)
#define LA_SBC_DECODER_LOW					REG32(P7) &=~BIT(2)
#if 1
#define LA_UART_RX_HGIH						REG32(P7) |=BIT(3)
#define LA_UART_RX_LOW						REG32(P7) &=~BIT(3)
#define LA_DEC2PCM_HGIH
#define LA_DEC2PCM_LOW
#else
#define LA_UART_RX_HGIH
#define LA_UART_RX_LOW
#define LA_DEC2PCM_HGIH						REG32(P7) |=BIT(3)
#define LA_DEC2PCM_LOW						REG32(P7) &=~BIT(3)
#endif
#define LA_PCM_DAC_HGIH						REG32(P7) |=BIT(4)
#define LA_PCM_DAC_LOW						REG32(P7) &=~BIT(4)
#if 0
//#define LA_UART_RX_HGIH						REG32(P7) |=BIT(5)
//#define LA_UART_RX_LOW						REG32(P7) &=~BIT(5)
#define LA_ACL_RX_HGIH
#define LA_ACL_RX_LOW
#else
//#define LA_UART_RX_HGIH
//#define LA_UART_RX_LOW
#define LA_ACL_RX_HGIH						REG32(P7) |=BIT(5)
#define LA_ACL_RX_LOW						REG32(P7) &=~BIT(5)
#endif

#define LA_UART_TX_HGIH						REG32(P6) |=BIT(6)
#define LA_UART_TX_LOW						REG32(P6) &=~BIT(6)
#if 0
#define LA_TEST_HIGH						REG32(P6) |=BIT(2)
#define LA_TEST_LOW							REG32(P6) &=~BIT(2)
#define LA_VALID_HIGH
#define LA_VALID_LOW
#else
#define LA_TEST_HIGH
#define LA_TEST_LOW
#define LA_VALID_HIGH						REG32(P6) |=BIT(2)
#define LA_VALID_LOW						REG32(P6) &=~BIT(2)
#endif


#if 1
#define LA_UART_ISR_HGIH					REG32(P6) |=BIT(0)
#define LA_UART_ISR_LOW						REG32(P6) &=~BIT(0)
#define LA_IEE_HIGH							REG32(P6) |=BIT(4)
#define LA_IEE_LOW							REG32(P6) &=~BIT(4)

#else
#define LA_UART_ISR_HGIH
#define LA_UART_ISR_LOW
#define LA_IEE_HIGH							REG32(P6) |=BIT(0)
#define LA_IEE_LOW							REG32(P6) &=~BIT(0)
#endif

#if 1
#define LA_TX1ST_HIGH							
#define LA_TX1ST_LOW
#define LA_FLSRD_HIGH
#define LA_FLSRD_LOW
#define LA_KEY_HIGH							REG32(P6) |=BIT(3)
#define LA_KEY_LOW							REG32(P6) &=~BIT(3)
#define LA_NUM_HIGH
#define LA_NUM_LOW
#else
#define LA_TX1ST_HIGH							REG32(P6) |=BIT(3)
#define LA_TX1ST_LOW							REG32(P6) &=~BIT(3)
#define LA_FLSRD_HIGH
#define LA_FLSRD_LOW
#define LA_KEY_HIGH
#define LA_KEY_LOW
#define LA_NUM_HIGH							
#define LA_NUM_LOW							
#endif

#if 1
#define LA_ACL_RX_TST_HGIH
#define LA_ACL_RX_TST_LOW
#define LA_DISC_REQ_HGIH					REG32(P7) |=BIT(6)
#define LA_DISC_REQ_LOW					REG32(P7) &=~BIT(6)
#else
#define LA_ACL_RX_TST_HGIH					REG32(P7) |=BIT(6)
#define LA_ACL_RX_TST_LOW					REG32(P7) &=~BIT(6)
#define LA_DISC_REQ_HGIH
#define LA_DISC_REQ_LOW
#endif


#define LA_SBC_DBUF_FULL_HGIH   					REG32(P7) |=BIT(7)
#define LA_SBC_DBUF_FULL_LOW	   					REG32(P7) &=~BIT(7)

#ifdef _AP_HEADSET_HANDSFREE_
#define LA_SBCFRMBIG_HIGH
#define LA_SBCFRMBIG_LOW
#else
#define LA_SBCFRMBIG_HIGH					REG32(P6) |=BIT(1)
#define LA_SBCFRMBIG_LOW					REG32(P6) &=~BIT(1)
#endif // _AP_HEADSET_HANDSFREE_

#else

#define LA_DEBUG_IO_Mode_OUT
#define LA2_DEBUG_IO_Mode_OUT

#define LA_PORT(...)
#define LA_(...)

//-------------------------------
// TOGGLE OUTPUT
#define LA_HCI2SBC
#define LA_SBC2DECODER
#define LA_SBC_DECODER
#define LA_SBC2PCM
#define LA_PCM_DAC
//-------------------------------
// STEADY OUTPUT
#define LA_L2CAP2AVDTP_HGIH
#define LA_L2CAP2AVDTP_LOW
#define LA_SBC2DECODER_HGIH
#define LA_SBC2DECODER_LOW
#define LA_SBC_DECODER_HGIH
#define LA_SBC_DECODER_LOW
#define LA_DEC2PCM_HGIH
#define LA_DEC2PCM_LOW
#define LA_PCM_DAC_HGIH
#define LA_PCM_DAC_LOW
#define LA_UART_RX_HGIH
#define LA_UART_RX_LOW
#define LA_UART_TX_HGIH
#define LA_UART_TX_LOW
#define LA_SBC_DBUF_FULL_HGIH
#define LA_SBC_DBUF_FULL_LOW
#define LA_ACL_RX_HGIH
#define LA_ACL_RX_LOW
#define LA_RQ2_HGIH
#define LA_RQ2_LOW
#define LA_ACL_RX_TST_HGIH
#define LA_ACL_RX_TST_LOW

#define LA_UART_ISR_HGIH
#define LA_UART_ISR_LOW
#define LA_IEE_HIGH
#define LA_IEE_LOW

#define LA_TX1ST_HIGH
#define LA_TX1ST_LOW
#define LA_SBCFRMBIG_HIGH
#define LA_SBCFRMBIG_LOW


#define LA_DEBUG_IO_Mode_OUT
#define LA2_DEBUG_IO_Mode_OUT
#define LA(byteout)
#define LA2(byteout)

#define LA_L2CAP2AVDTP_TG
#define LA_RQ2_TG
#define LA_SBC_DECODER_TG


#define LA_NUM_HIGH							
#define LA_NUM_LOW	
#define LA_TEST_HIGH
#define LA_TEST_LOW	
#define LA_VALID_HIGH
#define LA_VALID_LOW					

#define LA_DISC_REQ_HGIH
#define LA_DISC_REQ_LOW
#define LA_KEY_HIGH
#define LA_KEY_LOW
#define LA_FLSRD_HIGH
#define LA_FLSRD_LOW


#endif



#if LA_DEBUG == 1
//#define log23   la_23
#define log23(...)
#define log_1st(...)

#define log_dac la_23
#define log_dbank(...)
#define log_bank(...)
#define log_rq2(...)
#define log_id(...)

#else
#define log23(...)
#define log_1st(...)

#define log_dac(...)
#define log_dbank(...)
#define log_bank(...)
#define log_rq2(...)
#define log_id(...)

#endif

//#define BB32KTIME	150

#endif
/// EOF ///
