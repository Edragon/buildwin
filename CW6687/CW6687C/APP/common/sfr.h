/*****************************************************************************
 * Module    : COMMON
 * File      : sfr.h
 * Author    : Hanny
 * Function  : SFR寄存器定义
 *****************************************************************************/
#ifndef SFR_H
#define SFR_H

#ifdef __C51__
#ifndef __CX51__
#define __CX51__
#endif
#endif

#ifdef __A51__
#ifndef __AX51__
#define __AX51__
#endif
#endif

#ifdef __CX51__
#define SFR_DEF(reg, addr) sfr reg = addr;
#define SFR_DEFC(reg, addr) SFR_DEF(reg, addr)
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	sfr reg = addr; \
	sbit bit0 = reg^0; \
	sbit bit1 = reg^1; \
	sbit bit2 = reg^2; \
	sbit bit3 = reg^3; \
	sbit bit4 = reg^4; \
	sbit bit5 = reg^5; \
	sbit bit6 = reg^6; \
	sbit bit7 = reg^7;
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define SFR16_DEF(reg,reg1,reg2,addr1,addr2) \
	sfr16 reg = addr1; \
	sfr reg1 = addr1; \
	sfr reg2 = addr2;
#define XSFR_ADDR(addr) (*((volatile unsigned char xdata*)addr))
#endif
#ifdef __AX51__
#define SFR_DEF(reg, addr) sfr reg = addr;
#define SFR_DEFC(reg, addr)
SBIT_DEFM MACRO reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
	sfr reg = addr;
	sbit bit0 = reg^0;
	sbit bit1 = reg^1;
	sbit bit2 = reg^2;
	sbit bit3 = reg^3;
	sbit bit4 = reg^4;
	sbit bit5 = reg^5;
	sbit bit6 = reg^6;
	sbit bit7 = reg^7;
	ENDM
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	SBIT_DEFM reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define XSFR_ADDR(addr) addr
#endif

#ifdef __IAR_SYSTEMS_ICC__
#define SFR_DEF(reg, addr) __sfr __no_init volatile unsigned char reg @ addr;
#define SFR_DEFC(reg, addr) SFR_DEF(reg, addr)
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	__sfr __no_init volatile union { \
    unsigned char reg; \
    struct { \
		unsigned char bit0: 1; \
		unsigned char bit1: 1; \
		unsigned char bit2: 1; \
		unsigned char bit3: 1; \
		unsigned char bit4: 1; \
		unsigned char bit5: 1; \
		unsigned char bit6: 1; \
		unsigned char bit7: 1; \
    }; \
} @ addr;
#define SFR16_DEF(reg, reg1, reg2, addr1, addr2) \
	__sfr __no_init volatile union { \
    unsigned short reg; \
    struct { \
		unsigned char reg1; \
		unsigned char reg2; \
    }; \
} @ addr1;
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define XSFR_ADDR(addr) (*((volatile unsigned char __xdata*)addr))
#endif
#ifdef __IAR_SYSTEMS_ASM__
#define SFR_DEF(reg, addr) reg DEFINE addr
#define SFR_DEFC(reg, addr)
SBIT_DEFM MACRO reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
reg DEFINE addr
bit0 DEFINE addr.0
bit1 DEFINE addr.1
bit2 DEFINE addr.2
bit3 DEFINE addr.3
bit4 DEFINE addr.4
bit5 DEFINE addr.5
bit6 DEFINE addr.6
bit7 DEFINE addr.7
	ENDM
SFR16_DEFM MACRO reg, reg1, reg2, addr1, addr2
reg DEFINE addr1
reg1 DEFINE addr1
reg2 DEFINE addr2
	ENDM
#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	SBIT_DEFM reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define XSFR_ADDR(addr) addr
#define SFR16_DEF(reg, reg1, reg2, addr1, addr2) \
    SFR16_DEFM reg, reg1, reg2, addr1, addr2
#endif

/*  BYTE Registers  */
//========= 80H ----> 8FH =========//
SFR_DEF(P0,                0x80)
SFR_DEFC(SP,               0x81)
SFR_DEF(DPL0,              0x82)
SFR_DEF(DPH0,              0x83)
SFR16_DEF(DPTR1,DPL1,DPH1, 0x84,0x85)
SFR_DEF(DPCON,             0x86)
SFR_DEF(PCON0,             0x87)

SFR_DEF(SDCON0,            0x88)
SFR_DEF(SDCON1,            0x89)
SFR_DEF(SDCON2,            0x8A)
SFR_DEF(MEMCON,	   	       0x8B)
SFR_DEF(ATDAT,		       0x8C)
SFR_DEF(ERABYT0,	       0x8D)
SFR_DEF(P5,  	           0x8E)
SFR_DEF(ERABYT1,    	   0x8F)

//========= 90H ----> 9FH =========//
SFR_DEF(P1,                0x90)
//SFR_DEF(BFBYTEPTRL,        0x91)
//SFR_DEF(BFBYTEPTRH,        0x92)
SFR16_DEF(BFBYTEPTR,BFBYTEPTRL,BFBYTEPTRH,0x91,0x92)
//SFR_DEF(BFDATAL,           0x93)
//SFR_DEF(BFDATAH,           0x94)
SFR16_DEF(BFDATA,BFDATAL,BFDATAH,0x93,0x94)
SFR_DEF(BFBITPTR,          0x95)
SFR_DEF(BFCON,             0x96)
SFR_DEF(PCON3,		   	   0x97)
SFR_DEF(PWKPND,            0x98)
SFR_DEF(PWKEDGE,           0x99)
SFR_DEF(PIE0,              0x9A)
SFR_DEF(SPH,               0x9B)
SFR_DEF(PCON1,             0x9C)
SFR_DEF(ISDCHSUM,          0x9D)
SFR_DEF(IRTCDAT,           0x9E)
SFR_DEF(IRTCON,            0x9F)


//========= A0H ----> AFH =========//
SFR_DEF(P2,                0xA0)
SFR_DEF(IIS_CON2,          0xA1)
SFR_DEF(SPI1CON,           0xA2)
SFR_DEF(SPI1BUF,           0xA3)
SFR_DEF(ATADR,             0xA4)
SFR_DEF(SPI1DMACNTH,       0xA5)
SFR_DEF(SPI1DMASP,         0xA6)
SFR_DEF(IRCON0,            0xA7)
SBIT_DEF(IE0,              0xA8, IE_SINT, IE_AGC, IE_TM1, IE_TM2, IE_MP3,IE_HUFF,IE_USBSOF,IE_EA)
SFR_DEF(IE1,               0xA9)
SFR_DEF(SPI1DMACNTL,       0xAA)
SFR_DEF(IUBPCON,           0xAB)
//Reserve 0xAB
SFR_DEF(HFMCON1,           0xAC)
SFR_DEF(IRCON1,            0xAD)
SFR_DEF(AGCCON2,           0xAE)
SFR_DEF(SPMODE,            0xAF)

//========= B0H ----> BFH =========//
SFR_DEF(P3,                0xB0)
SFR_DEF(SQRT_DATA0,	   	   0xB1)
SFR_DEF(SQRT_DATA1,	   	   0xB2)
SFR_DEF(SQRT_DATA2,	   	   0xB3)
SFR_DEF(ERABYT2,	       0xB4)
SFR_DEF(ERABYT3,           0xB5)
SFR_DEF(EMIBUF,            0xB6)
SFR_DEF(PLLCON,            0xB7)

SFR_DEF(IP0,               0xB8)
SFR_DEF(IP1,               0xB9)
SFR_DEF(P0DIR,             0xBA)
SFR_DEF(P1DIR,             0xBB)
SFR_DEF(P2DIR,             0xBC)
SFR_DEF(P3DIR,             0xBD)
SFR_DEF(ERABYT4,           0xBE)
SFR_DEF(LVDCON,            0xBF)

//========= C0H ----> CFH =========//
SFR_DEF(IIS_CON0,          0xC0)
SFR_DEF(TMR2CON0,          0xC1)
SFR_DEF(TMR2CON1,          0xC2)
SFR_DEF(IIS_CON1,          0xC3)
SFR_DEF(IRTCON1,           0xC4)
SFR_DEF(SECCNT,            0xC5)
SFR_DEF(OTP_ADR,           0xC6)
SFR_DEF(IRAM_ADR,          0xC7)
SFR_DEF(HFMCON,            0xC8)
SFR_DEF(USBCON0,           0xC9)
SFR_DEF(USBCON1,           0xCA)
SFR_DEF(USBCON2,           0xCB)
SFR_DEF(USBDATA,           0xCC)
SFR_DEF(USBADR,            0xCD)
SFR_DEF(OIRAM_CNT,         0xCE)
SFR_DEF(OIRAM_CON,         0xCF)

//========= D0H ----> DFH =========//
SBIT_DEFC(PSW,             0xD0, P, F1, OV, RS0, RS1, F0, AC, CY)
SFR_DEF(HFMCNT,            0xD1)
SFR_DEF(ADCCON,            0xD2)
SFR_DEF(PCON2,             0xD3)
SFR_DEF(ADCDATAL,          0xD4)
SFR_DEF(ADCDATAH,          0xD5)
SFR_DEF(COS_VALH,          0xD6)
SFR_DEF(COS_VALL,          0xD7)
SFR_DEF(SPI0BUF,           0xD8)
SFR_DEF(SPI0CON,           0xD9)
SFR_DEF(ADCMODE,           0xDA)
SFR_DEF(CLKCON,            0xDB)
SFR_DEF(CLKCON1,           0xDC)
SFR_DEF(USBDPDM,           0xDD)
SFR_DEF(SQRT_DATA3,        0xDE)
SFR_DEF(PBANK0,            0xDF)

//========= E0H ----> EFH =========//
SFR_DEFC(ACC,              0xE0)
SFR_DEF(IPH0,              0xE1)
SFR_DEF(IPH1,              0xE2)
SFR_DEF(AUCON0,            0xE3)
SFR_DEF(AUCON1,            0xE4)
SFR_DEF(AUCON2,            0xE5)
SFR_DEF(AUCON3,            0xE6)
SFR_DEF(AUCON4,            0xE7)
SFR_DEF(AUCON5,            0xE8)
SFR_DEF(AUCON6,            0xE9)
SFR_DEF(AUCON7,            0xEA)
SFR_DEF(AUCON8,            0xEB)
SFR_DEF(AUCON9,            0xEC)
SFR_DEF(AUCON10,           0xED)
SFR_DEF(SQRT_CFG,          0xEE)
SFR_DEF(COS_IDX,           0xEF)

//========= F0H ----> FFH =========//
SFR_DEFC(B,                0xF0)
SFR_DEF(ER0H,	           0xF1)
SFR_DEF(ER0L,		   0xF2)
SFR_DEF(ER1H,		   0xF3)
SFR_DEF(ER1L,		   0xF4)
SFR_DEF(CRCREG,            0xF5)
SFR_DEF(CRCFIFO,           0xF6)
SFR_DEF(WDTCON,            0xF7)
SFR_DEF(TMR0CON,           0xF8)
SFR_DEF(TMR0CNT,           0xF9)
SFR_DEF(TMR0PR,            0xFA)
SFR_DEF(TMR0PWM,           0xFB)
SFR_DEF(IIS_CON3,          0xFE)
SFR_DEF(UARTSTA,           0xFC)
SFR_DEF(UARTCON,           0xFD)
//Reserve 0xFE
SFR_DEF(UARTDATA,          0xFF)

//FOR SIMULATION
SFR_DEF(SIMEND,            0xFF)

#define SOFT_INT0                0
#define SOFT_INT1                1
#define TIMER1_INT               2
#define TIMER2_INT               3
#define UART1_INT                6
#define PORT_INT                 9
#define SPI0_INT                 10
#define TIMER3_INT               11
#define TIMER0_INT               12
#define RTCC_INT                 13
#define SPI1_INT                 14
/* 注意三个中断共用一个入口，在中断函数里要判断一下标志位 */
#define AGC_INT1                 1
#define UART0_INT                13
#define LVD_INT                  13
#define WDT_INT                  13
#define IIS_INT                  13
#define VECTOR_OFFSET(x)		 (x*0x08 + 0x03)

#define DACCFG        0
#define DACSM         1
#define DACSPR        2
#define DACVOLL       3
#define DACVOLH       4
#define DACVCON       5
#define TRIMCON1      6
#define TRIMCON2      7
#define TRREGLL       8
#define TRREGLH       9
#define TRREGRL       10
#define TRREGRH       11
#define EQCON1        12
#define EQCOF         13
#define EQCON2        14
#define EQVOLIN       15

#define    FMCFG       32
#define    FMVOL       34
#define    FMFW2       37
#define    FMFW1       38
#define    FMFW0       39
#define    FMPC1H      40
#define    FMPC1L      41
#define    FMPC0H      42
#define    FMPC0L      43

// IRTCC Command
#define  WRITE_CFG             0x55
#define  READ_CFG              0x54
#define  WRITE_CFG3            0x59
#define  WRITE_RTC             0xf0
#define  READ_RTC              0xe0
#define  WRITE_ALM             0x53
#define  READ_ALM              0x52
#define  WRITE_RAM             0x57
#define  READ_RAM              0x56
#define  WRITE_PWR             0x5a
#define  WRITE_WKO             0x5b
#define  READ_WKO              0xa1
#define  WRITE_VCL             0xa2
#define  READ_VCL              0xa3
#define  READ_PWR              0x65
#define  READ_STA              0x62
#define  WRITE_STA             0x63

//XSFR
//----------------- 16'h780x -----------------//
#define SDBAUD                XSFR_ADDR(0x7800)
#define SDCPTR                XSFR_ADDR(0x7801)
#define SDDCNT                XSFR_ADDR(0x7802)
#define SDDPTR                XSFR_ADDR(0x7803)
#define RC_TEST               XSFR_ADDR(0x7804)
#define IIS_ADR1              XSFR_ADDR(0x7805)
#define RC_TRIM               XSFR_ADDR(0x7806)
#define PWRCON1               XSFR_ADDR(0x7807)
#define PWRCON2               XSFR_ADDR(0x7808)
#define PIE1                  XSFR_ADDR(0x7809)
#define SPMODE1               XSFR_ADDR(0x780a)
#define IIS_VALBIT            XSFR_ADDR(0x780b)
#define IRDAT0                XSFR_ADDR(0x780c)
#define IRDAT1                XSFR_ADDR(0x780d)
#define IRDAT2                XSFR_ADDR(0x780e)
#define IRDAT3                XSFR_ADDR(0x780f)

//----------------- 16'h781x -----------------//
#define BFEPTRL               XSFR_ADDR(0x7810)
#define BFEPTRH               XSFR_ADDR(0x7811)
#define HFMPTR                XSFR_ADDR(0x7812)
#define HFMTLIN               XSFR_ADDR(0x7813)
#define UART1CON              XSFR_ADDR(0x7814)
#define UART1BAUD             XSFR_ADDR(0x7815)
#define UART1DATA             XSFR_ADDR(0x7816)
#define UART1STA              XSFR_ADDR(0x7817)
#define UARTDMARXPTR          XSFR_ADDR(0x7818)    
#define UARTDMATXPTR          XSFR_ADDR(0x7819) 
#define UARTDMARXCNT          XSFR_ADDR(0x781a)  
#define UARTDMATXCNT          XSFR_ADDR(0x781b)
#define UART1DIV              XSFR_ADDR(0x781c)
#define IIS_BCLK_CFG          XSFR_ADDR(0x781d)
#define CRCRES0               XSFR_ADDR(0x781e)
#define CRCRES1               XSFR_ADDR(0x781f)

//----------------- 16'h782x -----------------//
#define SPIDMAPTRL            XSFR_ADDR(0x7820)
#define SPIDMAPTRH            XSFR_ADDR(0x7821)
#define SPIDMACNT             XSFR_ADDR(0x7822)
#define SPIBAUD               XSFR_ADDR(0x7823)
#define IIS_WSCNT0            XSFR_ADDR(0x7824)
#define P0DRV0                XSFR_ADDR(0x7825)
#define P1DRV0                XSFR_ADDR(0x7826)
#define P2DRV0                XSFR_ADDR(0x7827)
#define P3DRV0                XSFR_ADDR(0x7828)
#define IIS_DMA_WR_CNT0       XSFR_ADDR(0x7829)
#define IIS_DMA_WR_CNT1       XSFR_ADDR(0x782a)
#define IIS_DMA_RD_CNT0       XSFR_ADDR(0x782b)
#define IIS_DMA_RD_CNT1       XSFR_ADDR(0x782c)
#define IIS_ALLBIT            XSFR_ADDR(0x782d)
#define SPI1CON1              XSFR_ADDR(0x782e)
#define IIS_BAUD              XSFR_ADDR(0x782f)

//----------------- 16'h783x -----------------//
#define IIS_DAT0              XSFR_ADDR(0x7830)
#define IIS_DAT1              XSFR_ADDR(0x7831)
#define IIS_DAT2              XSFR_ADDR(0x7832)
#define IIS_DAT3              XSFR_ADDR(0x7833)
#define IIS_DAT4              XSFR_ADDR(0x7834)
#define IIS_DAT5              XSFR_ADDR(0x7835)
#define IIS_DAT6              XSFR_ADDR(0x7836)
#define IIS_DAT7              XSFR_ADDR(0x7837)
#define IIS_REFCLK_CFG        XSFR_ADDR(0x7838)
#define IIS_ADR0              XSFR_ADDR(0x7839)
#define PMUXCON2              XSFR_ADDR(0x783a)	
#define PMUXCON1              XSFR_ADDR(0x783b)		
#define IIS_WSCNT1            XSFR_ADDR(0x783c)
#define SDADCCON              XSFR_ADDR(0x783d)
#define PLL1DIV               XSFR_ADDR(0x783e)
#define PMUXCON0              XSFR_ADDR(0x783f)	

//----------------- 16'h784x -----------------//
#define P0PU                  XSFR_ADDR(0x7840)
#define P1PU                  XSFR_ADDR(0x7841)
#define P2PU                  XSFR_ADDR(0x7842)
#define P3PU                  XSFR_ADDR(0x7843)
#define PMUXCON3              XSFR_ADDR(0x7844)	
#define AEC_CFG0              XSFR_ADDR(0x7845)
#define PMUXCON4              XSFR_ADDR(0x7846)
#define P0PD                  XSFR_ADDR(0x7847)
#define P1PD                  XSFR_ADDR(0x7848)
#define P2PD                  XSFR_ADDR(0x7849)
#define P3PD                  XSFR_ADDR(0x784a)
#define PLL2CON               XSFR_ADDR(0x784b)
#define PLL2INTH              XSFR_ADDR(0x784c)
#define PLL2INTL              XSFR_ADDR(0x784d)
#define PLL2FRACH             XSFR_ADDR(0x784e)
#define PLL2FRACL             XSFR_ADDR(0x784f)

//----------------- 16'h785x -----------------//
#define TMR1CNTL              XSFR_ADDR(0x7850)
#define TMR1CNTH              XSFR_ADDR(0x7851)
#define TMR1PRL               XSFR_ADDR(0x7852)
#define TMR1PRH               XSFR_ADDR(0x7853)
#define TMR1PWML              XSFR_ADDR(0x7854)
#define TMR1PWMH              XSFR_ADDR(0x7855)
#define TMR3CON               XSFR_ADDR(0x7856)
#define TMR3CNT               XSFR_ADDR(0x7857)
#define TMR3PR                XSFR_ADDR(0x7858)
#define TMR3PWM               XSFR_ADDR(0x7859)
#define TMR1CON0              XSFR_ADDR(0x785a)
#define TMR1CON1              XSFR_ADDR(0x785b)
#define PLL1CON               XSFR_ADDR(0x785c)
#define PLL1INT               XSFR_ADDR(0x785d)
#define PLL1FRACH             XSFR_ADDR(0x785e)
#define PLL1FRACL             XSFR_ADDR(0x785f)

//----------------- 16'h786x -----------------//
#define TMR2CNTL              XSFR_ADDR(0x7860)
#define TMR2CNTH              XSFR_ADDR(0x7861)
#define TMR2PRL               XSFR_ADDR(0x7862)
#define TMR2PRH               XSFR_ADDR(0x7863)
#define TMR2PWML              XSFR_ADDR(0x7864)
#define TMR2PWMH              XSFR_ADDR(0x7865)
#define AUCON12               XSFR_ADDR(0x7866)
#define SFB_GEN               XSFR_ADDR(0x7867)
#define FIFO_TRI              XSFR_ADDR(0x7868)
#define FIFO_SET              XSFR_ADDR(0x7869)	
//#define AEC_RD_ADR			  XSFR_ADDR(0x786a)
#define APECON1		          XSFR_ADDR(0x786a)
#define EMICON0               XSFR_ADDR(0x786b)
#define EMICON1               XSFR_ADDR(0x786c)
#define AEC_AGGR			  XSFR_ADDR(0x786d)	
#define LFSR16_DAT0           XSFR_ADDR(0x786e)
#define LFSR16_DAT1           XSFR_ADDR(0x786f)

//----------------- 16'h787x -----------------//
#define USBEP0ADRL            XSFR_ADDR(0x7870)
#define USBEP0ADRH            XSFR_ADDR(0x7871)
#define USBEP1RXADRL          XSFR_ADDR(0x7872)
#define USBEP1RXADRH          XSFR_ADDR(0x7873)
#define USBEP1TXADRL          XSFR_ADDR(0x7874)
#define USBEP1TXADRH          XSFR_ADDR(0x7875)
#define USBEP2RXADRL          XSFR_ADDR(0x7876)
#define USBEP2RXADRH          XSFR_ADDR(0x7877)
#define USBEP2TXADRL          XSFR_ADDR(0x7878)
#define USBEP2TXADRH          XSFR_ADDR(0x7879)
#define USBEP3RXADRL          XSFR_ADDR(0x787a)
#define USBEP3RXADRH          XSFR_ADDR(0x787b)
#define USBEP3TXADRL          XSFR_ADDR(0x787c)
#define USBEP3TXADRH          XSFR_ADDR(0x787d)
#define IIS_DMA_RD_P_CNT0     XSFR_ADDR(0x787e)
#define IIS_DMA_RD_P_CNT1     XSFR_ADDR(0x787f)

//----------------- 16'h788x -----------------//
#define ADCBAUD               XSFR_ADDR(0x7880)
#define RANDOM_CNT            XSFR_ADDR(0x7881)
#define ECN                   XSFR_ADDR(0x7882)
#define ID0                   XSFR_ADDR(0x7883)
#define ID1                   XSFR_ADDR(0x7884)
#define IUDAT1                XSFR_ADDR(0x7885)
#define IUADR                 XSFR_ADDR(0x7886)
#define IUBP                  XSFR_ADDR(0x7887)
#define UARTBAUDL             XSFR_ADDR(0x7888)
#define UARTBAUDH             XSFR_ADDR(0x7889)
#define LFSR32_DAT0           XSFR_ADDR(0x788a)
#define LFSR32_DAT1           XSFR_ADDR(0x788b)
#define LFSR32_DAT2           XSFR_ADDR(0x788c)
#define LFSR32_DAT3           XSFR_ADDR(0x788d)
#define UARTDIV               XSFR_ADDR(0x788e)
#define SPI1BAUD              XSFR_ADDR(0x788f)

//----------------- 16'h789x -----------------//
#define ATCON0	              XSFR_ADDR(0x7890)
#define ATCON1                XSFR_ADDR(0x7891)
#define ATCON2                XSFR_ADDR(0x7892)
#define ATCON3                XSFR_ADDR(0x7893)
#define ATCON4                XSFR_ADDR(0x7894)
#define ATCON5                XSFR_ADDR(0x7895)
#define ATCON6                XSFR_ADDR(0x7896)
#define KVCON1                XSFR_ADDR(0x7897)
#define KVCON2                XSFR_ADDR(0x7898)
#define KVCADR                XSFR_ADDR(0x7899)
#define AUCON11               XSFR_ADDR(0x789a)
#define FIFO_SPEED            XSFR_ADDR(0x789b)
#define FIFO_BASE             XSFR_ADDR(0x789c)
#define DCT_CFG               XSFR_ADDR(0x789d)
#define ATCON7                XSFR_ADDR(0x789e)
#define ATCON8                XSFR_ADDR(0x789f)

//----------------- 16'h78ax -----------------//
#define FFT1CON                XSFR_ADDR(0x78a0)
#define FFT1CON1               XSFR_ADDR(0x78a1)
#define ATCON9                 XSFR_ADDR(0x78a2)
#define ATCON10                XSFR_ADDR(0x78a3)
#define CLKCON2                XSFR_ADDR(0x78a4)
#define UART1LOOPCNT           XSFR_ADDR(0x78a5)
#define UART1MINUS             XSFR_ADDR(0x78a6)
#define UART1POINTL            XSFR_ADDR(0x78a7)
#define UART1POINTH            XSFR_ADDR(0x78a8)
#define UART1CNTL              XSFR_ADDR(0x78a9)
#define UART1CNTH              XSFR_ADDR(0x78aa)
#define AGCCON4                XSFR_ADDR(0x78ab)
#define AGCSAMPLEL             XSFR_ADDR(0x78ac)
#define AGCSAMPLEH             XSFR_ADDR(0x78ad)
#define AGCRDATH               XSFR_ADDR(0x78ad)
#define AGCRDATL               XSFR_ADDR(0x78ae)

//----------------- 16'h78bx -----------------//
//#define P0PUS0                 XSFR_ADDR(0x78b0)
#define P1PUS0                 XSFR_ADDR(0x78b1)
#define P2PUS0                 XSFR_ADDR(0x78b2)
#define P3PUS0                 XSFR_ADDR(0x78b3)
//#define AEC_WR_ADR            XSFR_ADDR(0x78b4)
#define APECON2                XSFR_ADDR(0x78b4)
#define P1PUS1                 XSFR_ADDR(0x78b5)
#define P2PUS1                 XSFR_ADDR(0x78b6)
#define P3PUS1                 XSFR_ADDR(0x78b7)
#define AEC_DLY		           XSFR_ADDR(0x78b8)
#define P1PDS0                 XSFR_ADDR(0x78b9)
#define P2PDS0                 XSFR_ADDR(0x78ba)
#define P3PDS0                 XSFR_ADDR(0x78bb)
#define AEC_MU                 XSFR_ADDR(0x78bc)
#define P1PDS1                 XSFR_ADDR(0x78bd)
#define P2PDS1                 XSFR_ADDR(0x78be)
#define P3PDS1                 XSFR_ADDR(0x78bf)
	
//----------------- 16'h78cx -----------------//
#define PFENC                 XSFR_ADDR(0x78c0)
#define PFADR                 XSFR_ADDR(0x78c1)
#define IUBP0                 XSFR_ADDR(0x78c2)
#define IUBP1                 XSFR_ADDR(0x78c3)
#define IUBP2                 XSFR_ADDR(0x78c4)
#define IUBP3                 XSFR_ADDR(0x78c5)
//#define IUBPCON               XSFR_ADDR(0x78c6)
#define APECON0               XSFR_ADDR(0x78c6)
#define FFT1_DATAH_ADDR       XSFR_ADDR(0x78c7)
#define FFT1_DATAL_ADDR       XSFR_ADDR(0x78c8)
#define FFT1_BUFH_ADDR        XSFR_ADDR(0x78c9)
#define FFT1_BUFL_ADDR        XSFR_ADDR(0x78ca)
#define FFT1SCALE             XSFR_ADDR(0x78cb)
#define FFT1_SQRTH_ADDR       XSFR_ADDR(0x78cc)
#define FFT1_SQRTL_ADDR       XSFR_ADDR(0x78cd)
#define AUCON24               XSFR_ADDR(0x78ce)
#define AUCON25               XSFR_ADDR(0x78cf)

//----------------- 16'h78dx -----------------//
#define AGCCON0               XSFR_ADDR(0x78d0)
#define AGCCON1               XSFR_ADDR(0x78d1)
#define AGCANLCON             XSFR_ADDR(0x78d2)
#define AGCCON3               XSFR_ADDR(0x78d3)
#define AGCDMACON             XSFR_ADDR(0x78d4)
#define AGCDMAADR             XSFR_ADDR(0x78d5)
#define AGCDATH               XSFR_ADDR(0x78d6)
#define AGCDATL               XSFR_ADDR(0x78d7)
//Reserve 8'hd8	
//Reserve 8'hd9	
//Reserve 8'hda	
//Reserve 8'hdb	
#define BS_BEGIN_ADR          XSFR_ADDR(0x78dc)
#define BS_END_ADR            XSFR_ADDR(0x78dd)
#define AGCSETDATA            XSFR_ADDR(0x78de)
#define AGCSETCNT             XSFR_ADDR(0x78df)

//----------------- 16'h78ex -----------------//
#define P4                    XSFR_ADDR(0x78e0)
#define P4DIR                 XSFR_ADDR(0x78e1)
#define P4PU                  XSFR_ADDR(0x78e2)
#define P4PD                  XSFR_ADDR(0x78e3)
#define P4DRV0                XSFR_ADDR(0x78e4)
#define IIS_DMA_WR_P_CNT0     XSFR_ADDR(0x78e5)
#define IIS_DMA_WR_P_CNT1     XSFR_ADDR(0x78e6)
#define PWRCON3               XSFR_ADDR(0x78e7)
#define PWRCON4               XSFR_ADDR(0x78e8)
#define PWRCON5               XSFR_ADDR(0x78e9)
#define BTCON1                XSFR_ADDR(0x78ea)
#define BTCON2                XSFR_ADDR(0x78eb)
#define KEYCON                XSFR_ADDR(0x78ec)
#define PFCON                 XSFR_ADDR(0x78ed)
#define PFDAT                 XSFR_ADDR(0x78ee)
#define PFBAUD                XSFR_ADDR(0x78ef)

//----------------- 16'h78fx -----------------//
#define AEC_LEVEL			  XSFR_ADDR(0x78f0)
#define AEC_HNLFB			  XSFR_ADDR(0x78f1)
#define AEC_CFG1              XSFR_ADDR(0x78f2)
#define AEC_CFG2			  XSFR_ADDR(0x78f3)
#define MP3ENC0               XSFR_ADDR(0x78f4)
#define MP3ENC1               XSFR_ADDR(0x78f5)
#define MP3ENC2               XSFR_ADDR(0x78f6)
#define MP3ENC3               XSFR_ADDR(0x78f7)
#define MP3ENC4               XSFR_ADDR(0x78f8)
#define MP3ENC5               XSFR_ADDR(0x78f9)
#define MP3ENC6               XSFR_ADDR(0x78fa)
#define MP3ENC7               XSFR_ADDR(0x78fb)
#define MP3ENC8               XSFR_ADDR(0x78fc)
#define AUCON21               XSFR_ADDR(0x78fd)
#define AUCON22               XSFR_ADDR(0x78fe)
#define AUCON23               XSFR_ADDR(0x78ff)
//-------------------- 16'h790x -------------------//
#define  BTRAM_CON0            XSFR_ADDR(0x7900)
#define  BTRAM_ADRH            XSFR_ADDR(0x7901)
#define  BTRAM_ADRL            XSFR_ADDR(0x7902)
#define  BTRAM_DAT             XSFR_ADDR(0x7903)
#define  BTRAM_BYTH            XSFR_ADDR(0x7904)
#define  BTRAM_BYTL            XSFR_ADDR(0x7905)
#define  BTDMA_ADRH            XSFR_ADDR(0x7906)
#define  BTDMA_ADRL            XSFR_ADDR(0x7907)
#define  BTRAM_CON1            XSFR_ADDR(0x7908)
#define  BTRAM_CON2            XSFR_ADDR(0x7909)
#define  BTRAM_CON3            XSFR_ADDR(0x790a)
#define  AEC_GAIN              XSFR_ADDR(0x790b)
#define  AEC_NEAR_VALUE        XSFR_ADDR(0x790c)
#define  AEC_MIN_RANGE         XSFR_ADDR(0x790d)
#define  AEC_CFG3              XSFR_ADDR(0x790e)
//-------------------- 16'h791x -------------------//
#define  AEC_RD_ADR0           XSFR_ADDR(0x7910)
#define  AEC_RD_ADR1           XSFR_ADDR(0x7911)
#define  AEC_RD_ADR2           XSFR_ADDR(0x7912)
#define  AEC_RD_ADR3           XSFR_ADDR(0x7913)
#define  AEC_RD_ADR4           XSFR_ADDR(0x7914)
#define  AEC_RD_ADR5           XSFR_ADDR(0x7915)
#define  AEC_RD_ADR6           XSFR_ADDR(0x7916)
#define  AEC_RD_ADR7           XSFR_ADDR(0x7917)
#define  AEC_WR_ADR0           XSFR_ADDR(0x7918)
#define  AEC_WR_ADR1           XSFR_ADDR(0x7919)
#define  AEC_WR_ADR2           XSFR_ADDR(0x791a)
#define  AEC_WR_ADR3           XSFR_ADDR(0x791b)
#define  AEC_WR_ADR4           XSFR_ADDR(0x791c)
#define  AEC_WR_ADR5           XSFR_ADDR(0x791d)
#define  AEC_WR_ADR6           XSFR_ADDR(0x791e)

//-------------------- 16'h792x -------------------//
#define  PWKEN                 XSFR_ADDR(0x7920)
#define  PMUXCON5              XSFR_ADDR(0x7921)
#define  ATCON11               XSFR_ADDR(0x7922)
#define  ATCON12               XSFR_ADDR(0x7923)
#define  ATCON13               XSFR_ADDR(0x7924)
#define  ATCON14               XSFR_ADDR(0x7925)
#define  CHAGCON0              XSFR_ADDR(0x7926)
#define  CHAGCON1              XSFR_ADDR(0x7927)
#define  PCON4                 XSFR_ADDR(0x7928)
#define  PCON5                 XSFR_ADDR(0x7929)
#define  PCON6                 XSFR_ADDR(0x792a)
#define  SDADCZCD              XSFR_ADDR(0x792b)
#define  AGCDGL                XSFR_ADDR(0x792c)
#define  AGCDGR                XSFR_ADDR(0x792d)
#define  AGCANLG               XSFR_ADDR(0x792e)
#define  PIE2                  XSFR_ADDR(0x792f)

//-------------------- 16'h793x -------------------//
#define  PWMBUF0               XSFR_ADDR(0x7930)
#define  PWMBUF1               XSFR_ADDR(0x7931)
#define  PWMBUF2               XSFR_ADDR(0x7932)
#define  PWMBUF3               XSFR_ADDR(0x7933)
#define  PWMBUF4               XSFR_ADDR(0x7934)
#define  PWMBUF5               XSFR_ADDR(0x7935)
#define  PWMBUF6               XSFR_ADDR(0x7936)
#define  PWMBUF7               XSFR_ADDR(0x7937)
#define  PMUXCON6              XSFR_ADDR(0x7938)
#define  APECON3               XSFR_ADDR(0x7939)
#define  USBCON3               XSFR_ADDR(0x793A)
#endif
