#ifndef __HS_HF_H__
#define __HS_HF_H__

#include "global.h"

#define AP_STATE_DISCONNECTED			0x00
#define AP_STATE_CONNECTING				0x01
#define AP_STATE_CONNECTIND				0x02
#define AP_STATE_CONNECTCFM				0x03
#define AP_STATE_CONNECTED				0x04

#define AP_STATE_CONNECTREQ				0x10    //‘› ±”√CONNECTREQ
#define AP_STATE_W4ACL					0x11
#define AP_STATE_CONNECT                0x12
#define AP_STATE_W4CONNECTED			0x13
#define AP_STATE_CONFIGREQ				0x14
#define AP_STATE_W4CINFIGCFM			0x15
#define AP_STATE_DISCONNECTREQ			0x16
#define AP_STATE_W4DISCONNECTED			0x17
#define AP_STATE_W4_ROLE_SWITCH			0x18
#define AP_STATE_W4_ENCRYPTED			0x19	//#ifdef _CODE_SIMPLE_PAIRING_MODE_

#define AP_STATE_W4_RFCOMMDISC			0x21

#define AP_STATE_TX_SDP_SERVICE_SEARCH_ATT_REQ	0x31
#define AP_STATE_W4_SDP_SERVICE_SEARCH_ATT_RSP	0x32
#define AP_STATE_RX_SDP_SERVICE_SEARCH_ATT_RSP	0x33
#define AP_STATE_SDP_FINISHED					0x34

#define AP_STATE_TX_RFCOMM_SABM0				0x41
#define AP_STATE_W4_RFCOMM_SABM0_UA				0x42
#define AP_STATE_RFCOMM_FINISHED				0x4F


#define HID_PSM_CONTROL					0x0011
#define HID_PSM_INTERRUPT				0x0013
#define A2DP_PSM_AVCTP					0x0017
#define A2DP_PSM_AVDTP					0x0019


#define SDP_NOT_ASK					    0x00
#define SDP_ASK_PROFILE				    0x01


#define PAGETIMEOUT_100ms				52		// 5.2s


#define HF_STATE_W4                     0x80
enum {
    HF_STATE_IDLE = 0,
    HF_STATE_CONNECT_FINISHED,
    HF_STATE_BRSF_REQ,
    HF_STATE_INDI_SUPPORTED,
    HF_STATE_INDI_STATUS,
    HF_STATE_REQINDI_STATUS,
    HF_STATE_CLIP_ENABLE,
    HF_STATE_GAIN_LEVEL,
    HF_STATE_APPLE_XAPL,

    HF_STATE_W4_BRSF_REQ_OK     = HF_STATE_W4 | HF_STATE_BRSF_REQ,
    HF_STATE_W4_INDI_SUPPORTED_OK = HF_STATE_W4 | HF_STATE_INDI_SUPPORTED,
    HF_STATE_W4_INDI_STATUS_OK  = HF_STATE_W4 | HF_STATE_INDI_STATUS,
    HF_STATE_W4_REQINDI_STATUS_OK = HF_STATE_W4 | HF_STATE_REQINDI_STATUS,
    HF_STATE_W4_CLIP_ENABLE_OK  = HF_STATE_W4 | HF_STATE_CLIP_ENABLE,
    HF_STATE_W4_GAIN_LEVEL_OK   = HF_STATE_W4 | HF_STATE_GAIN_LEVEL,
    HF_STATE_W4_APPLE_XAPL_OK   = HF_STATE_W4 | HF_STATE_APPLE_XAPL,
};


//extern u8 HSFCallConnect;
extern u8 HSFCallActive;//0: no call  1:call active  2:outing call alerting
extern u8 HSFRingActive;
//extern u8 InBandRing_flag;
//extern u8 HSFCallConnect;
extern u8 HSFPhoneService;
extern HSHFCon  HSHFAPI;

//void APICreateACL(void);
 //void GetServiceSearchAtt(u8 RfCommChannelNum);

//bool HSF_SDPConnectInd(u16 cid,u16 handle,u8 BD[]);
//bool HSF_SDPConnectCfm(u16 cid,u16 result,u16 handle,u8 BD[]);
//bool HSF_SDPConnected(u16 cid);
//bool HSF_SDPDisConnectInd(u16 cid);
//void HSF_SDPDisconnectCfm(u16 cid);
bool HSF_RFCOMMConnectInd(u16 cid,u16 handle,u8 BD[]);
bool HSF_RFCOMMConnectCfm(u16 cid,u16 result,u16 handle,u8 BD[]);
bool HSF_RFCOMMConnected(u16 cid);
bool HSF_RFCOMMDisConnectInd(u16 cid);
void HSF_RFCOMMDisconnectCfm(u16 cid);

void CreateHandFreeConnection(void);
void CreateHeadSetConnection(void);
//bool TurnOffReminderBuzzer(void);
//void TimeoutCreateConnection(void);
//void PowerOnCreateConnection(void);
void InitHSFAplication(void);
void Process_HSF_API(void);
bool NeedNegotiationDisconnectHSF(void);

void InitHSF(void);
void InitRFComm(void);

#endif
