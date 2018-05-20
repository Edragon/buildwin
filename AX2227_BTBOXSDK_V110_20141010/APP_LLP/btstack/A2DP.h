
#include "HS_HF.h"

void InitA2DPSetting(void);
void Process_A2DP_API(void);
void A2DPCreateMediaChannel(void);
//void APICreateAVCTP(void);

#define APICreateAVCTP()   do{\
    if(A2DPAPI.AVCTPConList[0].state==AP_STATE_DISCONNECTED){\
		A2DPAPI.CreateConnTimes = 2;\
		A2DPAPI.AVCTPConList[0].state=AP_STATE_CONNECTREQ;\
	}\
}while(0);

#define SearchAVCTPbyCID(cid)   ((A2DPAPI.AVCTPConList[0].l2cap_cid == cid) ? 0 : 0xFF)


bool APICreateAVDTP(void);
bool AVDTPConnectInd(u16 cid,u16 psm,u16 handle,u8 BD[]);
bool AVCTPConnectInd(u16 cid,u16 psm,u16 handle,u8 BD[]);
bool AVDTPConnectCfm(u16 cid,u16 psm,u16 result,u16 handle,u8 BD[]);
bool AVCTPConnectCfm(u16 cid,u16 psm,u16 result,u16 handle,u8 BD[]);
bool A2DPConnected(L2capCon  *con);	// (u16 cid);
bool AVCTPConfigReq(u16 hciHdl);
bool AVDTPDisConnectInd(u16 cid);
bool AVCTPDisConnectInd(u16 cid);
void AVDTPDisconnectCfm(u16 cid);
void AVCTPDisconnectCfm(u16 cid);
//void AutoTurnOffby80msTimer(u8 decrease);
void InitA2DPConnection(void);
__near_func u8 SearchAVDTPbyCID(u16 l2cap_cid);
//u8 SearchAVCTPbyCID(u16 l2cap_cid);
//void ClearPowerSavingTMR80ms(void);

//==============eeprom_AP_FunctionSetting===============================================================
//#define AUTOTURNOFFTIME					7500		//7500*80ms=600 second =10 minute
//#define BATLOWAUTOTURNOFFTIME			250//250	//250*80ms=20 second
#define INQURYSCANINTERVAL				0x0400
#define PAGESCANINTERVAL				0x0400
#define PAGETIMEOUT_625us				0x2000		//0x02000:5.12sec

 //#define _VOICE_VIA_HCI_TO_HOST_		// lm_sco_cvsd.c

//#define WATCH_DOG_RESET_ENABLE				(eeprom_AP_FunctionSetting&0x01)
//#define BATTERY_LOW_DETECT_ENABLE			(eeprom_AP_FunctionSetting&0x02)
//#define ALLOW_REGISER_IN_IDLE_MODE			(eeprom_AP_FunctionSetting&0x04)
//#define CREATE_CONNECTION_AFTER_POWER_ON	(eeprom_AP_FunctionSetting&0x08)
//#define LED_ELECTRODE						(eeprom_AP_FunctionSetting&0x10)	//LED on by GPIO pull high
//#define GPIO_POWER_CONTROL					(eeprom_AP_FunctionSetting&0x20)	//use button volume up/donw gpio for wakeup host and BT power state indicator.
//#define SPI_EN								(eeprom_AP_FunctionSetting&0x40)	//Enable SPI and Disable SSP
//#define CREATE_CONNECTION_BY_OTHER_KEY		(eeprom_AP_FunctionSetting&0x80)

#ifdef __THIS_IS_AP_VAR__
//	u8 ReadyInSniffMode;
//	u8 FirstIntoSniff;
//	u8 acl_detach_flag=FALSE;
	//u8 eeprom_AP_FunctionSetting; //remarked by arthur
	//u8 eeprom_A2DP_Volume;

#else
	extern u8 ReadyInSniffMode;
	extern u8 FirstIntoSniff;
	extern u8 acl_detach_flag;
	extern u8 eeprom_AP_FunctionSetting;
//	extern u8 eeprom_A2DP_Volume;
#endif



