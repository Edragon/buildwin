// SDP.h
//
//////////////////////////////////////////////////////////////////////
#ifndef __SDP_H__
#define __SDP_H__

#include "global.h"

/* UUID type */
#define UUID16	0x19
#define UUID32	0x1A
#define UUID128	0x1C

#define HEADSET_UUID16		0x1108
#define HEADSET_AG_UUID16	0x1112
#define HANDFREE_AG_UUID16	0x111F
#define SERIAL_UUID16		0x1101
#define Generic_AUDIO_UUID16	0x1203
#define L2CAP_UUID16		0x0100
#define RFCOMM_UUID16		0x0003

#define HANDFREE_RFCOMM_SEVERCHN	0x01		//SDP report
#define HEADSET_RFCOMM_SEVERCHN 	0x02		//SDP report


#define PROTOCOL_DESCRIPTOR_ATTR	0x0004

//=============================================================================
/* SDP message buffer number */

/* PDU ID */
#define ERROR_RSP 			0x01
#define SERVICESEARCH_REQ 	0x02
#define SERVICESEARCH_RSP 	0x03
#define SERVICEATTR_REQ 	0x04
#define SERVICEATTR_RSP 	0x05
#define SERVICESEARCHATTR_REQ 0x06
#define SERVICESEARCHATTR_RSP 0x07

/* Type */
#define NULL_TYPE	0
#define UNSIGN_INT	1
#define TWO_COMPLE	2
#define UUID_TYPE	3
#define STR_TEXT	4
#define BOOL_TYPE	5
#define SEQU_DATA	6
#define SEQU_ALTER	7
#define URL_TYPE	8
#define ERROR_TYPE	9
//=============================================================================



#ifdef _AP_HEADSET_HANDSFREE_
#define MAX_BYTE_COUNT	420
#else
#define MAX_BYTE_COUNT	180	//or 64 //56x2<120
#endif
#define MAX_REC_NUM		6

typedef struct sdpCon_tag {
	u8  state;
	u16 l2cap_cid;
	u16 cont_state;
}  SdpCon;

struct sdp_cont_tag{
	u16 total;
	u16 cont_len;
	u8 cont[MAX_BYTE_COUNT+20];
};

extern u8 SDP_Record_en;

void SdpInit(void);

void SdpReceiveData(u16 cid, u16 len, u8  *Data);
#ifdef _AP_HEADSET_HANDSFREE_
//u8 search_Rfcomm_serverchannel(u8 param[], u16 count);

bool SdpConnectReq(u8 BD[], u16 hciHdl);
#endif

bool SdpConnectInd(u16 cid,u16 handle,u8 BD[]);
bool SdpConnectCfm(u16 cid,u16 result,u16 handle,u8 BD[]);
bool SdpDisconnectReq(u16 cid);
bool SdpDisconnectInd(u16 cid);
void SdpDisconnectCfm(u16 cid);
bool SdpConnected(u16 cid);

u16 SdpServiceSearchAttrReq(u16 cid, u16 ClassId[], u8 IdNum, u16 RangeL, u16 RangeH, u16 attr);

//============= SDP Descript ===========================//



#endif
