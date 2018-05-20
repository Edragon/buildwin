#ifndef __RFCOMM_H__
#define __RFCOMM_H__

#include "global.h"

extern IAR_CONST u8 at_chup_str[];
extern IAR_CONST u8 at_ckpd_str[];
extern IAR_CONST u8 at_ata_str[];
extern IAR_CONST u8 at_bldn_str[];
extern IAR_CONST u8 at_bvra_0_str[];
extern IAR_CONST u8 at_bvra_1_str[];
extern IAR_CONST u8 at_clip_str[];
extern IAR_CONST u8 at_cmer_str[];
extern IAR_CONST u8 at_cind_e_str[];
extern IAR_CONST u8 at_cind_q_str[];
extern IAR_CONST u8 at_brsf_str[];


#define MAX_NBR_RFCOMM	1
#define MAX_NBR_DLCI	3
#define RFCOMM_DEF_MTU	127
#define MIN_REMOTE_CREDIT			5
#define SUGGEST_REMOTE_CREDIT		24

//=============================================================================
#define CONTROL_DLCI_LINE		0
#define HANDSFREE_DLCI_LINE		1
#define HEADSET_DLCI_LINE		2
typedef struct{	// rfcomm connection
	u8 state;
	u16 l2cap_cid;
	u8 init;	//  1:Command/Response:Initiator Started C/R Sequence  ,	0:Command/Response: Responder Started C/R Sequence ;	record this structure is initiator or not
	u8 credit_flow;
	struct {
		u8 serverchannel;
		u8 link;	//bit2 Receive RSP //bit1 Send CMD /bit0 receive CMD used for msc message
		u8 state;
		u8 prior;
		u8 local_credit;
		u8 remote_credit;
		u16 rfcomm_mtu;
	} dlci[MAX_NBR_DLCI+1];	//dlci[0] used for control message
	struct {
		u8 start;
		u32 timer;	//default 96000
	} timer;
}   RfcommCon ;
// RfcommCon State
#define RFCOMM_DISCONNECTED		0
#define RFCOMM_CONNECTING		1
#define RFCOMM_NEGOTIATING		2
#define RFCOMM_CONNECTED		3
#define RFCOMM_DISCONNECTING	4
#define RFCOMM_FLOW_STOPPED		5


//=============================================================================
u8 RfcommReceiveData(u16 cid, u16 len, u8  *Data);

void RfcommInit(void);
void process_rfcomm(void);
bool RfcommConnected(L2capCon  *con);	// u16 cid);
bool RfcommConnectReq(u8 BD[], u16 hciHdl);
bool RfcommConnectInd(u16 cid,u16 handle,u8 BD[]);
bool RfcommConnectCfm(u16 cid,u16 result,u16 handle,u8 BD[]);
bool RfcommDisconnectReq(u16 cid);
bool RfcommDisconnectInd(u16 cid);
void RfcommDisconnectCfm(u16 cid);
u8   RfcommSendData(u8 forcesyn_cmd, u16 cid, u8 dlci, u8 mapdlci, u16 len, u8 pkt[]);
void tmr0_rfcomm_625us(void);
//void proc_rfcomm(void);

u8 RfcommSendSabm(u16 cid, u8 dlci, u8 mapdlci);
bool RfcommSendDisc(u16 cid, u8 dlci);
//u8 RfcommSendPnMessage(u16 cid, u8 dlci, u8 mapdlci, u8 prior, bool cr, u16 frame_size, u8 credit);
u8 RfcommSendPnMessage(u16 cid, u8 dlci, u8 prior, bool cr, u16 frame_size, u8 credit);
u8 RxSetAsysnchornousBalanceMode(u16 cid, u16 len, u8 Data[]);
u8 RxUnnumberedAcknowledgement(u16 cid, u16 len, u8 Data[]);
u8 RxDisconnectMode(u16 cid, u16 len, u8 Data[]);
u8 RxDISC(u16 cid, u16 len, u8 Data[]);
u8 RxUnnumberedInfowithHeaderCheck(u16 cid, u16 len, u8 Data[]);
//u8 proc_recUIH(u8 fcs, u16 info_len, u16 cid, u8 aa, u8 index, u8 tmpdlci, u8 mapdlci, u8 Data[], bool pf) ;

#endif
