#ifndef _AX5111_H_
#define _AX5111_H_

#include "include.h"

#define REFCLK              12    //4,12,24 MHz
#define RSSI_AVERAGE        64
#define AGC_RSSI_THRESHOLD  (-19)*RSSI_AVERAGE
#define ISSTATION_DRSSI_TH  (2)*RSSI_AVERAGE
#define ISSTATION_DRSSI_4M  (19)*RSSI_AVERAGE
#define NOISE_FLOOR         (-65)*RSSI_AVERAGE

#define FM_START_FREQ       875
#define FM_STOP_FREQ        1080

extern void Write_FMREG(u8 Addr,u8 Data);
extern void LO_Calibration(u16 Freq);
extern int GetDRSSI(u16 freq);
extern void ax5111_init();
extern void ax5111_set_freq(u16 freq);
extern u8 ax5111_seek(u16 freq);
extern void ax5111_on();
extern void ax5111_off();
extern void ax5111_set_vol(u8 vol);
extern u8 ax5111_online();

//----------------------------------
//DO NOT MODIFY THE FOLLOWING !!!
//----------------------------------
#define 	AX5111_CLKCON0        0x00
#define 	AX5111_CLKCON1        0x01
#define 	AX5111_LVDCON         0x02
#define 	AX5111_P0             0x03
#define 	AX5111_P1             0x04
#define 	AX5111_P0DIR          0x05
#define 	AX5111_P1DIR          0x06
#define 	AX5111_PUP0           0x07
#define 	AX5111_PUP1           0x08
#define 	AX5111_PDN0           0x09
#define 	AX5111_PDN1           0x0a
#define 	AX5111_PTE            0x0b
#define 	AX5111_DCN            0x0c
#define 	AX5111_ID_H           0x0d
#define 	AX5111_ID_L           0x0e
#define 	AX5111_DACCON0        0x0f
#define 	AX5111_DACCON1        0x10
#define 	AX5111_DACCON2        0x11
#define 	AX5111_DACCON3        0x12
#define 	AX5111_DACCON5        0x13
#define 	AX5111_FMRX_CON       0x14
#define 	AX5111_FMRXACON0      0x15
#define 	AX5111_FMRXACON1      0x16
#define 	AX5111_FMRXACON2      0x17
#define 	AX5111_FMRXACON3      0x18
#define 	AX5111_FMRXACON4      0x19
#define 	AX5111_FMRXACON5      0x1a
#define 	AX5111_FMRXACON6      0x1b
#define 	AX5111_FMRXACON7      0x1c
#define 	AX5111_FMRXACON8      0x1d
#define 	AX5111_FMRX_TS0       0x1e
#define 	AX5111_LPLL_FRAC_N_H  0x1f
#define 	AX5111_LPLL_FRAC_N_L  0x20
#define 	AX5111_LPLL_INT_N     0x21
#define 	AX5111_FMRX_CTL       0x22
#define 	AX5111_LPLL_CTL0      0x23
#define 	AX5111_LPLL_CTL1      0x24
#define 	AX5111_LPLL_CTL2      0x25
#define 	AX5111_IFADC_CTL0     0x26
#define 	AX5111_IFADC_CTL1     0x27
#define 	AX5111_FMFE_CTL0      0x28
#define 	AX5111_FMFE_CTL1      0x29
#define 	AX5111_FMFE_CTL2      0x2a
#define 	AX5111_FMFE_CTL3      0x2b
#define 	AX5111_FMFE_CTL4      0x2c
#define 	AX5111_FMFE_CTL5      0x2d
#define 	AX5111_FM_BLEND       0x2e
#define 	AX5111_VOL_TAB_H      0x2f
#define 	AX5111_VOL_TAB_L      0x30
#define 	AX5111_VOL_TRM_L_H    0x31
#define 	AX5111_VOL_TRM_L_L    0x32
#define 	AX5111_VOL_TRM_R_H    0x33
#define 	AX5111_VOL_TRM_R_L    0x34
#define 	AX5111_FM_TEST_CTL    0x35
#define 	AX5111_BANDCNT_CTL    0x36
#define 	AX5111_I_GAIN_H       0x37
#define 	AX5111_I_GAIN_L       0x38
#define 	AX5111_Q_GAIN_H       0x39
#define 	AX5111_Q_GAIN_L       0x3a
#define 	AX5111_BANDCNT_H      0x3b
#define 	AX5111_BANDCNT_L      0x3c
#define 	AX5111_PILOT_POWER_H  0x3d
#define 	AX5111_PILOT_POWER_L  0x3e
#define 	AX5111_RSSI           0x3f


#endif

