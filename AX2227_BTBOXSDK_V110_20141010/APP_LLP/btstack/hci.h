#ifndef __HCI_H__
#define __HCI_H__

#include "global.h"

#define controlerToHostFlowCtrlEnable 1

#define BT_On_Start     0
#define BT_On_Fail      0xff
#define BT_On_Finish    0xfe

// HCI.h
//
//////////////////////////////////////////////////////////////////////
//================HCI Command opCode ==============================//
/* Group: Link control commands */
#define HCC_INQUIRY                      0x0401
#define HCC_INQUIRY_CANCEL               0x0402
#define HCC_START_PERIODIC_INQ_MODE      0x0403
#define HCC_EXIT_PERIODIC_INQ_MODE       0x0404
#define HCC_CREATE_CONNECTION            0x0405
#define HCC_DISCONNECT                   0x0406
#define HCC_ADD_SCO_CONNECTION           0x0407 /* Not in 1.2 */
#define HCC_CREATE_CONNECTION_CANCEL     0x0408 /* 1.2 */
#define HCC_ACCEPT_CON_REQ               0x0409
#define HCC_REJECT_CON_REQ               0x040A
#define HCC_LINK_KEY_REQ_REPL            0x040B
#define HCC_LINK_KEY_REQ_NEG_REPL        0x040C
#define HCC_PIN_CODE_REQ_REPL            0x040D
#define HCC_PIN_CODE_REQ_NEG_REPL        0x040E
#define HCC_CHNG_CONN_PACKET_TYPE        0x040F
#define HCC_AUTH_REQ                     0x0411
#define HCC_SET_CONN_ENCRYPT             0x0413
#define HCC_CHNG_CONN_LINK_KEY           0x0415
#define HCC_MASTER_LINK_KEY              0x0417
#define HCC_REM_NAME_REQ                 0x0419
#define HCC_REM_NAME_REQ_CANCEL          0x041A /* 1.2 */
#define HCC_READ_REMOTE_FEATURES         0x041B
#define HCC_READ_REMOTE_EXT_FEATURES     0x041C /* 1.2 */
#define HCC_READ_REMOTE_VERSION          0x041D
#define HCC_READ_CLOCK_OFFSET            0x041F
#define HCC_READ_LMP_HANDLE              0x0420 /* 1.2 */
#define HCC_EXCHANGE_FIXED_INFO          0x0421 /* 1.2, not in 2.1 */
#define HCC_EXCHANGE_ALIAS_INFO          0x0422 /* 1.2, not in 2.1 */
#define HCC_PRIVATE_PAIRING_REQ_REPL     0x0423 /* 1.2, not in 2.1 */
#define HCC_PRIVATE_PAIRING_REQ_NEG_REPL 0x0424 /* 1.2, not in 2.1 */
#define HCC_GENERATED_ALIAS              0x0425 /* 1.2, not in 2.1 */
#define HCC_ALIAS_ADDRESS_REQ_REPL       0x0426 /* 1.2, not in 2.1 */
#define HCC_ALIAS_ADDRESS_REQ_NEG_REPL   0x0427 /* 1.2, not in 2.1 */
#define HCC_SETUP_SYNC_CONNECTION        0x0428 /* 1.2 */
#define HCC_ACCEPT_SYNC_CON_REQ          0x0429 /* 1.2 */
#define HCC_REJECT_SYNC_CON_REQ          0x042A /* 1.2 */
#define HCC_IO_CAPABILITY_RESPONSE       0x042B /* 2.1 */
#define HCC_USER_CONFIRM_REQ_REPL        0x042C /* 2.1 */
#define HCC_USER_CONFIRM_REQ_NEG_REPL    0x042D /* 2.1 */
#define HCC_USER_PASSKEY_REQ_REPL        0x042E /* 2.1 */
#define HCC_USER_PASSKEY_REQ_NEG_REPL    0x042F /* 2.1 */
#define HCC_REM_OOB_DATA_REQ_REPL        0x0430 /* 2.1 */
#define HCC_REM_OOB_DATA_REQ_NEG_REPL    0x0433 /* 2.1 */
#define HCC_IO_CAPABILITY_NEG_REPL       0x0434 /* 2.1 */

/* Group: Link policy commands */
#define HCC_HOLD_MODE                    0x0801
#define HCC_SNIFF_MODE                   0x0803
#define HCC_EXIT_SNIFF_MODE              0x0804
#define HCC_PARK_MODE                    0x0805
#define HCC_EXIT_PARK_MODE               0x0806
#define HCC_QOS_SETUP                    0x0807
#define HCC_ROLE_DISCOVERY               0x0809
#define HCC_SWITCH_ROLE                  0x080B
#define HCC_READ_LINK_POLICY             0x080C
#define HCC_WRITE_LINK_POLICY            0x080D
#define HCC_READ_DEFAULT_LINK_POLICY     0x080E /* 1.2 */
#define HCC_WRITE_DEFAULT_LINK_POLICY    0x080F /* 1.2 */
#define HCC_FLOW_SPECIFICATION           0x0810 /* 1.2 */
#define HCC_SNIFF_SUBRATING              0x0811 /* 2.1 */

/* Group: Host controller and baseband commands */
#define HCC_SET_EVENT_MASK               0x0C01
#define HCC_RESET                        0x0C03
#define HCC_EVENT_FILTER                 0x0C05
#define HCC_FLUSH                        0x0C08
#define HCC_READ_PIN_TYPE                0x0C09
#define HCC_WRITE_PIN_TYPE               0x0C0A
#define HCC_CREATE_NEW_UNIT_KEY          0x0C0B
#define HCC_READ_STORED_LINK_KEY         0x0C0D
#define HCC_WRITE_STORED_LINK_KEY        0x0C11
#define HCC_DEL_STORED_LINK_KEY          0x0C12
#define HCC_CHNG_LOCAL_NAME              0x0C13
#define HCC_READ_LOCAL_NAME              0x0C14
#define HCC_READ_CONN_ACCEPT_TIMEOUT     0x0C15
#define HCC_WRITE_CONN_ACCEPT_TIMEOUT    0x0C16
#define HCC_READ_PAGE_TIMEOUT            0x0C17
#define HCC_WRITE_PAGE_TIMEOUT           0x0C18
#define HCC_READ_SCAN_ENABLE             0x0C19
#define HCC_WRITE_SCAN_ENABLE            0x0C1A
#define HCC_READ_PAGE_SCAN_ACTIVITY      0x0C1B
#define HCC_WRITE_PAGE_SCAN_ACTIVITY     0x0C1C
#define HCC_READ_INQ_SCAN_ACTIVITY       0x0C1D
#define HCC_WRITE_INQ_SCAN_ACTIVITY      0x0C1E
#define HCC_READ_AUTH_ENABLE             0x0C1F
#define HCC_WRITE_AUTH_ENABLE            0x0C20
#define HCC_READ_ENCRYPT_MODE            0x0C21 /* Not in 2.1 */
#define HCC_WRITE_ENCRYPT_MODE           0x0C22 /* Not in 2.1 */
#define HCC_READ_CLASS_OF_DEVICE         0x0C23
#define HCC_WRITE_CLASS_OF_DEVICE        0x0C24
#define HCC_READ_VOICE_SETTING           0x0C25
#define HCC_WRITE_VOICE_SETTING          0x0C26
#define HCC_READ_AUTO_FLUSH_TIMEOUT      0x0C27
#define HCC_WRITE_AUTO_FLUSH_TIMEOUT     0x0C28
#define HCC_READ_NUM_BC_RETRANSMIT       0x0C29
#define HCC_WRITE_NUM_BC_RETRANSMIT      0x0C2A
#define HCC_READ_HOLD_MODE_ACTIVITY      0x0C2B
#define HCC_WRITE_HOLD_MODE_ACTIVITY     0x0C2C
#define HCC_READ_XMIT_POWER_LEVEL        0x0C2D
#define HCC_READ_SCO_FC_ENABLE           0x0C2E
#define HCC_WRITE_SCO_FC_ENABLE          0x0C2F
#define HCC_SET_CTRLR_TO_HOST_FLOW_CTRL  0x0C31
#define HCC_HOST_BUFFER_SIZE             0x0C33
#define HCC_HOST_NUM_COMPLETED_PACKETS   0x0C35
#define HCC_READ_LINK_SUPERV_TIMEOUT     0x0C36
#define HCC_WRITE_LINK_SUPERV_TIMEOUT    0x0C37
#define HCC_READ_NUM_IAC                 0x0C38
#define HCC_READ_CURRENT_IAC_LAP         0x0C39
#define HCC_WRITE_CURRENT_IAC_LAP        0x0C3A
#define HCC_READ_PAGE_SCAN_PERIOD_MODE   0x0C3B /* Not in 2.1 */
#define HCC_WRITE_PAGE_SCAN_PERIOD_MODE  0x0C3C /* Not in 2.1 */
#define HCC_READ_PAGE_SCAN_MODE          0x0C3D /* Not in 1.2 */
#define HCC_WRITE_PAGE_SCAN_MODE         0x0C3E /* Not in 1.2 */
#define HCC_SET_AFH_HOST_CHAN_CLASS      0x0C3F /* 1.2 */
#define HCC_READ_INQ_SCAN_TYPE           0x0C42 /* 1.2 */
#define HCC_WRITE_INQ_SCAN_TYPE          0x0C43 /* 1.2 */
#define HCC_READ_INQ_MODE                0x0C44 /* 1.2 */
#define HCC_WRITE_INQ_MODE               0x0C45 /* 1.2 */
#define HCC_READ_PAGE_SCAN_TYPE          0x0C46 /* 1.2 */
#define HCC_WRITE_PAGE_SCAN_TYPE         0x0C47 /* 1.2 */
#define HCC_READ_AFH_CHAN_ASSESS_MODE    0x0C48 /* 1.2 */
#define HCC_WRITE_AFH_CHAN_ASSESS_MODE   0x0C49 /* 1.2 */
#define HCC_READ_ANONYMITY_MODE          0x0C4A /* 1.2, not in 2.1 */
#define HCC_WRITE_ANONYMITY_MODE         0x0C4B /* 1.2, not in 2.1 */
#define HCC_READ_ALIAS_AUTH_ENABLE       0x0C4C /* 1.2, not in 2.1 */
#define HCC_WRITE_ALIAS_AUTH_ENABLE      0x0C4D /* 1.2, not in 2.1 */
#define HCC_READ_ANON_ADDR_CHNG_PARM     0x0C4E /* 1.2, not in 2.1 */
#define HCC_WRITE_ANON_ADDR_CHNG_PARM    0x0C4F /* 1.2, not in 2.1 */
#define HCC_RESET_FIXED_ADDR_COUNTER     0x0C50 /* 1.2, not in 2.1 */
#define HCC_READ_EXTENDED_INQ_RESP       0x0C51 /* 2.1 */
#define HCC_WRITE_EXTENDED_INQ_RESP      0x0C52 /* 2.1 */
#define HCC_REFRESH_ENCRYPT_KEY          0x0C53 /* 2.1 */
#define HCC_READ_SIMPLE_PAIRING_MODE     0x0C55 /* 2.1 */
#define HCC_WRITE_SIMPLE_PAIRING_MODE    0x0C56 /* 2.1 */
#define HCC_READ_LOCAL_OOB_DATA          0x0C57 /* 2.1 */
#define HCC_READ_INQ_RESP_TRANS_PWR_LVL  0x0C58 /* 2.1 */
#define HCC_WRITE_INQ_TRANS_PWR_LVL      0x0C59 /* 2.1 */
#define HCC_READ_DEF_ERR_DATA_REPORTING  0x0C5A /* 2.1 */
#define HCC_WRITE_DEF_ERR_DATA_REPORTING 0x0C5B /* 2.1 */
#define HCC_ENHANCED_FLUSH               0x0C5F /* 2.1 */
#define HCC_SEND_KEYPRESS_NOTIFY         0x0C60 /* 2.1 */

/* Group: Informational parameters */
#define HCC_READ_LOCAL_VERSION           0x1001
#define HCC_READ_LOCAL_COMMANDS          0x1002 /* 1.2 */
#define HCC_READ_LOCAL_FEATURES          0x1003
#define HCC_READ_LOCAL_EXT_FEATURES      0x1004 /* 1.2 */
#define HCC_READ_BUFFER_SIZE             0x1005
#define HCC_READ_COUNTRY_CODE            0x1007 /* Not in 1.2 */
#define HCC_READ_BD_ADDR                 0x1009

/* Group: Status parameters */
#define HCC_READ_FAILED_CONTACT_COUNT    0x1401
#define HCC_RESET_FAILED_CONTACT_COUNT   0x1402
#define HCC_GET_LINK_QUALITY             0x1403
#define HCC_READ_RSSI                    0x1405
#define HCC_READ_AFH_CHANNEL_MAP         0x1406 /* 1.2 */
#define HCC_READ_CLOCK                   0x1407 /* 1.2 */

/* Group: Testing commands */
#define HCC_READ_LOOPBACK_MODE           0x1801
#define HCC_WRITE_LOOPBACK_MODE          0x1802
#define HCC_ENABLE_DUT                   0x1803
#define HCC_WRITE_SIMPLE_PAIR_DBG_MODE   0x1804 /* 2.1 */

/* Group: Vendor Command*/
#define HCC_Vendor_05          0xFC05	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_
#define HCC_Vendor_06          0xFC06	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_
#define HCC_Vendor_0E          0xFC0E
#define HCC_Vendor_18          0xFC18	//YS module IO test
#define HCC_Vendor_19          0xFC19
#define HCC_Vendor_1E          0xFC1E	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_
#define HCC_Vendor_1F          0xFC1F	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_
#define HCC_Vendor_20          0xFC20	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_
#define HCC_Vendor_21          0xFC21	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_	trigger_ini
#define HCC_Vendor_22          0xFC22	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_	module gpio
#define HCC_Vendor_23          0xFC23	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_	product gpio/key
#define HCC_Vendor_24          0xFC24	//#ifdef _RESERVED_CID_003D_FOR_MODULE_TEST_	mic
#define HCC_Vendor_BC          0xFCBC
#define HCC_Vendor_CC          0xFCCC
#define HCC_Vendor_CD          0xFCCD


//================HCI Event code ==============================//
#define HCE_INQUIRY_COMPLETE               0x01
#define HCE_INQUIRY_RESULT                 0x02
#define HCE_CONNECT_COMPLETE               0x03
#define HCE_CONNECT_REQUEST                0x04
#define HCE_DISCONNECT_COMPLETE            0x05
#define HCE_AUTH_COMPLETE                  0x06
#define HCE_REMOTE_NAME_REQ_COMPLETE       0x07
#define HCE_ENCRYPT_CHNG                   0x08
#define HCE_CHNG_CONN_LINK_KEY_COMPLETE    0x09
#define HCE_MASTER_LINK_KEY_COMPLETE       0x0A
#define HCE_READ_REMOTE_FEATURES_COMPLETE  0x0B
#define HCE_READ_REMOTE_VERSION_COMPLETE   0x0C
#define HCE_QOS_SETUP_COMPLETE             0x0D
#define HCE_COMMAND_COMPLETE               0x0E
#define HCE_COMMAND_STATUS                 0x0F
#define HCE_HARDWARE_ERROR                 0x10
#define HCE_FLUSH_OCCURRED                 0x11
#define HCE_ROLE_CHANGE                    0x12
#define HCE_NUM_COMPLETED_PACKETS          0x13
#define HCE_MODE_CHNG                      0x14
#define HCE_RETURN_LINK_KEYS               0x15
#define HCE_PIN_CODE_REQ                   0x16
#define HCE_LINK_KEY_REQ                   0x17
#define HCE_LINK_KEY_NOTIFY                0x18
#define HCE_LOOPBACK_COMMAND               0x19
#define HCE_DATA_BUFFER_OVERFLOW           0x1A
#define HCE_MAX_SLOTS_CHNG                 0x1B
#define HCE_READ_CLOCK_OFFSET_COMPLETE     0x1C
#define HCE_CONN_PACKET_TYPE_CHNG          0x1D
#define HCE_QOS_VIOLATION                  0x1E
#define HCE_PAGE_SCAN_MODE_CHANGE          0x1F /* Not in 1.2 */
#define HCE_PAGE_SCAN_REPETITION_MODE      0x20
#define HCE_FLOW_SPECIFICATION_COMPLETE    0x21 /* 1.2 */
#define HCE_INQUIRY_RESULT_WITH_RSSI       0x22 /* 1.2 */
#define HCE_READ_REMOTE_EXT_FEAT_COMPLETE  0x23 /* 1.2 */
#define HCE_FIXED_ADDRESS                  0x24 /* 1.2 */
#define HCE_ALIAS_ADDRESS                  0x25 /* 1.2 */
#define HCE_GENERATE_ALIAS_REQ             0x26 /* 1.2 */
#define HCE_ACTIVE_ADDRESS                 0x27 /* 1.2 */
#define HCE_ALLOW_PRIVATE_PAIRING          0x28 /* 1.2 */
#define HCE_ALIAS_ADDRESS_REQ              0x29 /* 1.2 */
#define HCE_ALIAS_NOT_RECOGNIZED           0x2A /* 1.2 */
#define HCE_FIXED_ADDRESS_ATTEMPT          0x2B /* 1.2 */
#define HCE_SYNC_CONNECT_COMPLETE          0x2C /* 1.2 */
#define HCE_SYNC_CONN_CHANGED              0x2D /* 1.2 */
#define HCE_HCI_SNIFF_SUBRATING            0x2E /* 2.1 */
#define HCE_EXTENDED_INQUIRY_RESULT        0x2F /* 2.1 */
#define HCE_ENCRYPT_KEY_REFRESH_COMPLETE   0x30 /* 2.1 */
#define HCE_IO_CAPABILITY_REQUEST          0x31 /* 2.1 */
#define HCE_IO_CAPABILITY_RESPONSE         0x32 /* 2.1 */
#define HCE_USER_CONFIRMATION_REQUEST      0x33 /* 2.1 */
#define HCE_USER_PASSKEY_REQUEST           0x34 /* 2.1 */
#define HCE_REMOTE_OOB_DATA_REQUEST        0x35 /* 2.1 */
#define HCE_SIMPLE_PAIRING_COMPLETE        0x36 /* 2.1 */
#define HCE_LINK_SUPERV_TIMEOUT_CHANGED    0x38 /* 2.1 */
#define HCE_ENHANCED_FLUSH_COMPLETE        0x39 /* 2.1 */
#define HCE_USER_PASSKEY_NOTIFICATION      0x3B /* 2.1 */
#define HCE_KEYPRESS_NOTIFICATION          0x3C /* 2.1 */
#define HCE_REMOTE_HOST_SUPPORTED_FEATURES 0x3D /* 2.1 */
#define HCE_BLUETOOTH_LOGO                 0xFE
#define HCE_VENDOR_SPECIFIC                0xFF

//=============================================================================
/* HCI definitions */
#define BB_ACL_BUFFER_SIZE			128	//250
#define HCI_NUM_ACL_BUFFERS			2	//4 // ref to RQ2_ELEMENT_NUM
#define RXAclBufferSize				678	//339 //define can receive ACL data buffer length
//#define EventBufferSize				255 //max event length
#define HCI_NUM_SCO_BUFFERS          1
#define RXScoBufferSize 			    48 //define can receive SCO data buffer length

//define SDP_Record_en
enum
{
    SDP_A2DP	=      0X07,
    SDP_HEADSET =      0X18,
    SDP_SPP		=      0x20,
    SDP_HID		= 	   0xC0,
};

extern u8 Link_BdAddr[6];

#define MAX_NBR_AVDTP		2
typedef struct{
	u16 psm;	//0x0011:HID control(Host to Device),0x0013:Interrupt ; 0x0017:AVCTP,0x0019:AVDTP
	u8 state;
	u16 l2cap_cid;	//set 0x0000 in freeA2DPConnection()
	u8 channeltype;
}  UpperL2CAPCon;

typedef struct a2dpapi_tag{
	UpperL2CAPCon AVDTPConList[MAX_NBR_AVDTP];//0:Signal/1:Media/2:Reporting/3:Recovery channel
	UpperL2CAPCon AVCTPConList[1];//Control channel

	u16 HciHdl;
	u8 bd[6];
	u8 ActiveA2DPConnect;	//Device Active Connect to Host
	u8 RemoteCreateConnect;	
	u8 CreateConnTimes;	//power on connection/Supervision timeout or connection fail to re-connection
	u8 DisconnectIndex;	//[0]:AVDTP Signal channel,[1]:AVDTP Media channel	[2]:AVCTP

}  A2DPCon;

#define MAX_SDP_SEARCH	3
#define MAX_NBR_DLCI	3

typedef struct {
	UpperL2CAPCon SDPConList[1];        //PSM=0x0001
	u8 SDPRequestServer;
	u8 SDPRequestServerStatus;
	u8 SDPSearchList[MAX_SDP_SEARCH+1];
} SDPCon;

typedef struct {
	UpperL2CAPCon RFCOMMConList[1];		//PSM=0x0003,support 1 rfocmm "MAX_NBR_RFCOMM"
    u16 HciHdl;
	u8 bd[6];
	u8 CreateConnTimes;	        //power on connection/Supervision timeout or connection fail to re-connection
	u8 LocalCreate;	            //Create Headset/Handsfree by local
    u8 HandfreeState;
	//u8 TimeoutReConnect;	    //After supervision timeout,reminder to try to connecte to cell phone.If still fail ==>buzzer on	
	//u8 PowerOnConnect;
	u8 HandsfreeChannelNumber;
	u8 HeadsetChannelNumber;
	u8 PhoneDlci;
} HSHFCon;

//LinkType
#define HCI_SCO   0
#define HCI_ACL   1
#define HCI_eSCO  2

typedef struct{
	u16 ConnHdl;
	u8 BDADDR[6];
	u8 used:1,	//connection being used
	   role:1,	//0:master, 1:slave
	   LinkType:1,	//0:SCO, 1:ACL,
	   pincode_req_event:1,		//#ifdef _CODE_SIMPLE_PAIRING_MODE_
	   set_encrypt_finish:2,	//#ifdef _CODE_SIMPLE_PAIRING_MODE_
	   encryption_enabled:1,	//#ifdef _CODE_SIMPLE_PAIRING_MODE_
	   hcmd_create_connect:1;	//#ifdef _CODE_SIMPLE_PAIRING_MODE_
	u8 auth_fail_ocuured:1,		//#ifdef _CODE_SIMPLE_PAIRING_MODE_
	   todoReBonding:1,		//#ifdef _CODE_SIMPLE_PAIRING_MODE_
	   flag_roleswitching:1,
	   zzzzz:5;
	u8 countAuthRequested;		//#ifdef _CODE_SIMPLE_PAIRING_MODE_

	//link key stored in eeprom
}  HciCon ;


#define HCI_NUM		2
extern HciCon  HciConList[HCI_NUM];


typedef u8 BtInquiryMode;

#define INQ_MODE_NORMAL    0 /* Normal Inquiry Response format           */
#define INQ_MODE_RSSI      1 /* RSSI Inquiry Response format             */
#define INQ_MODE_EXTENDED  2 /* Extended or RSSI Inquiry Response format */


typedef struct{
	u16           TXAclBufferSize;
    	u16           radioPageTimeout;
    	u8             bdaddr[6];
 //     u8              btVersion;
   //   u8              btFeatures[8];
  //    BtInquiryMode   inqMode;           /* Current inquiry mode */
	u8	hcTotalNumAclDataPkts;	//kidd, assume myLocalHC.hcTotalNumAclDataPkts[15:8]==0
}  LocalHC;

extern LocalHC myLocalHC ;

extern A2DPCon A2DPAPI;
//=============================================================================
void HCI_Init(void);

HciCon * AllocateHciCon(u8 bd_addr[], u8 linktype);
HciCon * SearchHciHdl(u16 connHdl, u8 linktype);
HciCon * SearchHciBDAddrLinkType(u8 BDAddr[], u8 linktype);
void freeHciConn(HciCon *hci);
void process_hci(void);
bool HciSendACLData(u16 connHdl, u16 Len, u8 pData[]);

__near_func void process_receive(void);
	
void HciEventDiscconectComplete(u16 connHdl, u8 Reason);


void SetConnTimes(u8 times);
void process_AP(void);
void BtMainLoop(void);

//bool os_memcmp_code (const u8 *buffer1,  const u8 __code *buffer2, u16 len);
//__near_func bool os_memcmp (const u8 *buffer1,  const u8 *buffer2, u16 len);
#define os_memcmp   memcmp
#define os_memcpy   my_memcpy


#endif
