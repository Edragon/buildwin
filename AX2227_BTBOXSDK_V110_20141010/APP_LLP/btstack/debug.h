
//------------------------------------
// btmgr.c
//------------------------------------

#define uart_putbuf printf_array

#define HCI_EVENT_DEBUG 0
#if HCI_EVENT_DEBUG == 1
#define HCI_EVENT_Debug   printf
#define HCI_EVENT_BUF_Debug   uart_putbuf
#else
#define HCI_EVENT_Debug(...)
#define HCI_EVENT_BUF_Debug(...)
#endif


#define DB_DEBUG 0
#if DB_DEBUG == 1
#define DB_Debug   printf
#define DB_BUF_Debug   uart_putbuf
#else
#define DB_Debug(...)
#define DB_BUF_Debug(...)
#endif

//------------------------------------
//hci.c
//------------------------------------



#define HCI_DEBUG 0
#if HCI_DEBUG == 1
#define HCI_Debug   printf
#define HCI_RBUF_Debug   uart_putbuf
#else
#define HCI_Debug(...)
#define HCI_RBUF_Debug(...)
#endif

#define ACL_DEBUG 0
#if ACL_DEBUG == 1
#define ACL_Debug   printf
//#define ACL_RBUF_Debug   uart_putbuf
#define ACL_RBUF_Debug(...)
#else
#define ACL_Debug(...)
#define ACL_RBUF_Debug(...)
#endif

#define HCI_CMD_DEBUG 0
#if HCI_CMD_DEBUG == 1
#define HCI_CMD_Debug   printf
#else
#define HCI_CMD_Debug(...)
#endif

//------------------------------------
//hal_uart.c
//------------------------------------

#define HAL_UART_DEBUG 0
#if HAL_UART_DEBUG == 1
#define HAL_UART_Debug   printf
#define HAL_UART_HEX_Debug   uart_putasic
#else
#define HAL_UART_Debug(...)
#define HAL_UART_HEX_Debug(...)
#endif

//------------------------------------
//A2DP.c
//------------------------------------


//#define A2DP_DEBUG 1
#if A2DP_DEBUG == 1
#define A2DP_Debug   printf
#define A2DP_RBUF_Debug   uart_putbuf
#else
#define A2DP_Debug(...)
#define A2DP_RBUF_Debug(...)
#endif


//#define AVDTP_DEBUG 1
#if AVDTP_DEBUG == 1
#define AVDTP_Debug   printf
#define AVDTP_RBUF_Debug   uart_putbuf
#else
#define AVDTP_Debug(...)
#define AVDTP_RBUF_Debug(...)
#endif

#define AVDTP_MEDIA_DEBUG 0
#if AVDTP_MEDIA_DEBUG == 1
#define AVDTP_MEDIA_Debug   printf
#else
#define AVDTP_MEDIA_Debug(...)
#endif


#define AVDTP_ERR_DEBUG 0
#if AVDTP_ERR_DEBUG == 1
#define AVDTP_ERR_Debug   printf
#else
#define AVDTP_ERR_Debug(...)
#endif


#define SBC_DEBUG 0
#if SBC_DEBUG == 1
#define SBC_Debug   printf
#else
#define SBC_Debug(...)
#endif


//#define DISC_TO_DEBUG
#ifdef DISC_TO_DEBUG
#define DISC_TO_Debug   printf
#else
#define DISC_TO_Debug(...)
#endif

//#define A2DP_RECON_DEBUG
#ifdef A2DP_RECON_DEBUG
#define A2DP_RECON_Debug   printf
#else
#define A2DP_RECON_Debug(...)
#endif

//#define AVCTP_DEBUG
#ifdef AVCTP_DEBUG
#define AVCTP_Debug   printf
#else
#define AVCTP_Debug(...)
#endif

//#define AP_RUN_DEBUG
#ifdef AP_RUN_DEBUG
#define AP_RUN_Debug   printf
//#define AP_RUN_Debug_hold delay_ms(10)

#else
#define AP_RUN_Debug(...)
#define AP_RUN_Debug_hold

#endif

//#define DAC_DEBUG
#ifdef DAC_DEBUG
#define DAC_Debug   printf
#else
#define DAC_Debug(...)
#endif


//#define SDP_DEBUG
#ifdef SDP_DEBUG
#define SDP_Debug   printf
#else
#define SDP_Debug(...)
#endif


//#define MMI_DEBUG
#ifdef MMI_DEBUG
#define MMI_Debug   printf
#else
#define MMI_Debug(...)
#endif


//#define A2DPBNK_DEBUG
#ifdef A2DPBNK_DEBUG
#define A2DPBNK_Debug   printf
#else
#define A2DPBNK_Debug(...)
#endif

//#define AVRCP_DEBUG
#ifdef AVRCP_DEBUG
#define AVRCP_Debug   printf
#else
#define AVRCP_Debug(...)
#endif
	
//#define PAUSE_DEBUG
#ifdef PAUSE_DEBUG
#define PAUSE_Debug   printf
#else
#define PAUSE_Debug(...)
#endif

