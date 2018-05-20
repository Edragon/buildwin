#ifndef __L2CAP_H__
#define __L2CAP_H__

#include "global.h"

#define _L2CAP_DATAFAIL 	0x0002
#define MTU_DEF 			(672+4)//672	//567	//328
#define L2_HD_OFFSET	4

//=============================================================================

typedef struct {
	u8 mode;			
	u8 tx_window_size;
	u8 max_transmit;
	u16 retx_timeout;
	u16	monitor_timeout;
	u16 max_pdu_size;
} ReTXFlowControl;


typedef struct{
	u8 flag;		//default=0
	u8 service;		//default=0x01
	u32 token_rate;
	u32 bucket_size;	//u8
	u32 peak_bw;		//u8s/sec
	u32 latency;		//ms
	u32 delay_var;		//ms
} Qos;

typedef struct {
	u8 sig_id_rcv;
	u8 sig_id_sen;
	u8 State;
	u8 link;	//bit0,1: used for configure request receive & tranmit
			//bit4,5: used for configure response receive & tranmit
	u8 d_bd[6];
	u16 HciHdl;	//hci connection handle
	u16 l_cid;	//local channel ID
	u16 r_cid;	//remote channel ID
	u16 psm;
	u16 incomingMTU;	//local  MTU, incoming from air
	u16 outgoingMTU;	//remote MTU, outgoing to air
	u16 flushto;	//remote device flush timeout
	u16 flag;
	u32 ertx;
	u32 rtx;
	struct {
		u8 init;
		u8 nbr_resends;
		u32 timer;
	} time;
	Qos q;

	ReTXFlowControl flow;

	u8 GOTMTUFlag;
} L2capCon;


typedef struct {
	u16 mtu;
	u16 flush_timeout;
	u16 allow_conless;	//determine that connectionless data
	Qos q;
} L2capLayer;

typedef struct {
	u8 valid;
	u16 cid;
	u16 len;
	#ifndef _YS_MODIFY_L2CAP_BUFFER_
	u8 payload[MTU_DEF];
	#endif
} L2capRcv;
extern L2capRcv  l2caprcv;

#define MAX_L2CAP_NUM 7

typedef u8 bd_addr[6];
typedef struct {
	u8 init;
	u8 count;
	u16 cid;
	u16 psm;
	bd_addr bd[MAX_L2CAP_NUM];
}  L2capConless;

extern u8  L2CAP_resent;

//=============================================================================
//State Machine
void InitL2cap(void);
void process_l2cap(void);
//void tmr0_l2cap_625us(void);
bool L2capReceiveData(const u16 connHdl,const u16 Len, const u8  *payload);

u16 L2caWriteDataReq(u16 cid, u16 len, u8 OutBuffer[]);
//Upper Layer to L2cap
u16 L2caConnectReq(u16 psm, u8 BDAddr[]);

bool L2caDisconnectReq(u16 cid);
bool L2caDisconnectRsp(L2capCon  *con);
bool L2caGetInfo(u8 BDAddr[], u16 InfoType);
bool L2caPing(u8 BDAddr[], u8 EchoData[], u16 Len);
//L2cap Connection less
u16 L2caEnableCLT(u16 psm);
u16 L2caDisableCLT(u16 psm);
u16 L2caGroupCreate(u16 psm);
u16 L2caGroupClose(u16 cid);
u16 L2caGroupAddMember(u16 cid, u8 bd[]);
u16 L2caGroupRemoveMember(u16 cid, u8 bd[]);
bool L2caConfigReq(L2capCon  *con, u16 link_to, u8 sen);


extern bool IsAllL2capFree(void);


//Help funciton
void DeleteL2capCon(L2capCon  *con);
void FreeIndicateL2capCon(u16 connHdl);
void FreeAllL2capCon(void);
bool isExistingL2capChannel(u16 lcid);


L2capCon  * CreateL2capCon(u16 hciHdl, u16 r_cid, u16 psm);

#ifdef _AP_HEADSET_HANDSFREE_
L2capCon  * GetL2capHdl_PSM(u16 hciHdl, u16 psm);
#endif

//__near_func
L2capCon  * GetL2capHdl(u16 hciHdl);
__near_func L2capCon  * GetL2capLcid(u16 cid);

L2capCon  * GetL2capBDAddr(u8 BDAddr[], u8 State);
L2capCon  * L2caGroupMemberShip(u16 cid);
L2capCon  * GetL2capGroup(u16 cid);

#endif
