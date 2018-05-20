#include "include.h"
#include "user_fm.h"
#include "AX5111.h"

#if (FM_TYPE == FM_AX5111)

#pragma constseg="FM_INIT_CONST_AX5111"
IAR_CONST u8 AX5111_INIT_TABLE[]=
{
  0xE0, //CLKCON0      54
  0xF0, //CLKCON1      48
  0x24, //LVDCON       68
  0x00, //P0
  0x00, //P1
  0x00, //P0DIR
  0x00, //P1DIR
  0x00, //PUP0
  0x00, //PUP1
  0x00, //PDN0
  0x00, //PDN1
  0x40, //PTE
  0x00, //DCN    RO
  0x00, //ID_H   RO
  0x15, //ID_L   RO
  0x00, //DACCON0
  0x40, //DACCON1      5B
  0x80, //DACCON2      3F
  0x02, //DACCON3      0D
  0x18, //DACCON5      15
  0x01, //FMRX_CON     01
  0x37, //FMRXACON0
  0xA8, //FMRXACON1    AA
  0xAA, //FMRXACON2
  0x00, //FMRXACON3    7F
  0x2A, //FMRXACON4
#if (REFCLK==4)
  0x23,//0x23, //FMRXACON5    4M:0x23,12M/24M:0x21
#else
  0x21,//0x21,
#endif
  0x30, //FMRXACON6    32
  0xAA, //FMRXACON7
  0xD0, //FMRXACON8
  0x04, //FMRX_TS0
  0x5D, //LPLL_FRAC_N_H
  0x2F, //LPLL_FRAC_N_L
  0x17, //LPLL_INT_N
  0xD3, //FMRX_CTL    17
  0x00, //LPLL_CTL0    81
#if (REFCLK==4)
  0x16, //LPLL_CTL1    4M:0x16,12M/24M:0x1E
#else
  0x1E,
#endif
  0x20, //LPLL_CTL2    20
  0x05, //IFADC_CTL0   BD
  0xA9, //IFADC_CTL1   A9
#if (REFCLK==24)
  0x04,
#else
  0x00, //FMFE_CTL0    4M/12M:0x00,24M:0x04
#endif
  0x01, //FMFE_CTL1    AA
  0x00, //FMFE_CTL2    EA
  0x07, //FMFE_CTL3
  0x07, //FMFE_CTL4    87
  0x20, //FMFE_CTL5    20
  0x7F, //FM_BLEND     FF
  0x7F, //VOL_TAB_H
  0xFF, //VOL_TAB_L
  0x00, //VOL_TRM_L_H
  0x00, //VOL_TRM_L_L
  0x00, //VOL_TRM_R_H
  0x00, //VOL_TRM_R_L
  0x00, //FM_TEST_CTL
#if (REFCLK==4)
  0x18, //BANDCNT_CTL  4M:0x18,12M/24M:0x1C
#else
  0x1C,
#endif
  0x7F, //I_GAIN_H
  0xFF, //I_GAIN_L
  0x7F, //Q_GAIN_H
  0xFF  //Q_GAIN_L
};
#pragma constseg=default

#pragma constseg="FM_CONST_AX5111"
u8 VOL_TABLE[16][2]=
{
	0x00,0x00,
	0x00,0x10,
	0x00,0x20,
	0x00,0x40,
	0x00,0x80,
	0x01,0x00,
	0x02,0x00,
	0x04,0x00,
	0x08,0x00,
	0x10,0x00,
	0x16,0x9F,
	0x20,0x00,
	0x2D,0x3F,
	0x40,0x00,
	0x5A,0x7E,
	0x7F,0xFF
};

#pragma location="FM_SEG_AX5111"
u8 ax5111_seek(u16 freq)
{
    int DRSSI;
    Write_FMREG(AX5111_FM_TEST_CTL,0x00); //disable complex filter
    Write_FMREG(AX5111_FMRX_CTL,0x07);    //disable demodulator
    LO_Calibration(freq);
    DRSSI = GetDRSSI(freq);

    if(DRSSI>=ISSTATION_DRSSI_TH)
    {
        if(((freq%40)==0)||(freq==948)||(freq==949)||(freq==1021)||(freq==1022))
        {
            if(DRSSI<ISSTATION_DRSSI_4M){
                return 0;
            }
        }
        Write_FMREG(AX5111_FMRX_CTL,0x0F);	//set CS Filter to 128K
        DRSSI = GetDRSSI(freq-1);
        Write_FMREG(AX5111_FMRX_CTL,0x07);	//set CS Filter to 228K
        if(DRSSI>=ISSTATION_DRSSI_TH){
            ax5111_set_freq(freq);          //判断为真台后设置一下频率播放一小段时间
            return 1;
        }
    }
    return 0;
}

#pragma location="FM_SEG_AX5111_1"
void ax5111_set_vol(u8 vol)
{
    if(vol>15)
        vol=15;

    Write_FMREG(AX5111_VOL_TAB_L,VOL_TABLE[vol][1]);
    Write_FMREG(AX5111_VOL_TAB_H,VOL_TABLE[vol][0]);
}

#endif
