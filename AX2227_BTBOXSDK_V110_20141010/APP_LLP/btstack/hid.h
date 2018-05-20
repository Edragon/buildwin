#ifndef _HID_H_
#define _HID_H_

//HID
#define HID_PROCESS_Idle			    0x00
#define	HID_PROCESS_Read_Motion		    0x01
#define	HID_PROCESS_SEND_DATA			0x02
#define HID_PROCESS_W4_POWERDOWN	    0xF1//when Idle in reg mode->go to POWERDOWN mode
#define HID_PROCESS_POWERDOWN		    0xF2
#define HID_PROCESS_POWERUP			    0xF3



#define _KEY_MAX_			13
#define _SDP_KEY_MAX_		6//SDP report key num	



#define MAX_NBR_HID			2
#define HID_PSM_CONTROL		0x0011
#define HID_PSM_INTERRUPT	0x0013
#define _HID_KEYBOARD_PROTOCOL_COUNT	5	

#define _HID_KEYBOARD_		1
#define _HID_MOUSE_		2	



typedef struct{
	u8 psm;	//0x11:HID control(Host to Device)		0x13:Interrupt
	u8 state;
	u16 l2cap_cid;	//set 0x0000 in freeHIDConnection()
} HIDCon;


typedef struct hidapi_tag{
	HIDCon HIDConList[MAX_NBR_HID];//0:control	1:interrupt
	u16 HciHdl;
	u8 bd[6];
    u8 CreateConnStep;
	u8 CreateConnTimes;
}HIDAPICON;

struct HIDVal_tag{
	u8 keyboardval; // 1:keyboard val   0:consurm val
	u8 press;// 1:press 0:release
	u16 val;
};

extern u8 HID_buf_in;
extern u8 HID_buf_out;
extern struct HIDVal_tag HIDVal[5];
extern u8 HID_protocl;//rx or timeout
extern  HIDAPICON HIDAPI;



#define PAGETIMEOUT_625us				0x2000		//0x02000:5.12sec
#define PAGETIMEOUT_80ms				0x4F		//0x40=64=5.12sec (+15 > 5.12sec)

//#define PAGETIMEOUT_625us				0x800		//0x800:1.28sec
//#define PAGETIMEOUT_80ms				0x14 		//0x10=16=1.28sec (+4 > 1.28sec)
//#define PAGETIMEOUT_625us_2				0x1000		//0x01000:2.56sec
//#define PAGETIMEOUT_80ms_2				0x28		//0x20=34=2.56sec (+8 > 2.56sec)



bool HIDConnected(u16 cid);
bool HIDDisConnectInd(u16 cid);
void HIDDisconnectCfm(u16 cid);
bool HIDConnectInd(u16 cid,u8 psm,u8 *bd);
bool HIDConnectCfm(u16 cid,u8 psm,u16 result,u8 *bd);
u8 HIDReceiveData(u16 cid, u16 len, u8 *Data);
void SendKeyBoardVal(u8 val,bool press);
void SendConsumerVal(u16 val,bool press);
void HIDInit(void);
void process_DisconnectACL(void);
__near_func u8 GetHidChannelConnectionState(void);
void process_HID_Connection(void);
void Send10Val(void);
bool HID_CreateConn(HIDCon *hid);
//bool IsAllL2capFree(void);
void Process_ReSendData(void);
bool NeedNegotiationDisconnectHID(void);

bool SendHidButton(u8 button);
bool SendHidConsumer(u8 button);


#endif


