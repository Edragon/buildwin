/*
定义说明
SFR_DEF：常用sfr定义
SFR_DEFC：常用sfr定义，为了避免汇编下的定义冲突
SBIT_DEF：常用sbit定义
SBIT_DEFC：常用sbit定义，为了避免汇编下的定义冲突
XSFR_ADDR：XSFR 类型
*/
#ifndef __SFR_H__
#define __SFR_H__

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

#define SBIT_DEF(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7) \
	SBIT_DEFM reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7
#define SBIT_DEFC(reg, addr, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7)
#define XSFR_ADDR(addr) addr
#endif

/*  BYTE Registers  */
//========= 80H ----> 8FH =========//
SFR_DEF(P0,                0x80)
SFR_DEFC(SP,               0x81)
SFR_DEF(DPL0,              0x82)
SFR_DEF(DPH0,              0x83)
SFR_DEF(DPL1,              0x84)
SFR_DEF(DPH1,              0x85)
SFR_DEF(DPCON,             0x86)
SFR_DEF(PCON0,             0x87)

SFR_DEF(SDCON0,            0x88)
SFR_DEF(SDCON1,            0x89)
SFR_DEF(SDCON2,            0x8A)
SFR_DEF(ATDAT,		   0x8C)
SFR_DEF(ERABYT0,	   0x8D)
SFR_DEF(P5,  	0x8E)
SFR_DEF(ERABYT1,    	   0x8F)

//========= 90H ----> 9FH =========//
SFR_DEF(P1,                0x90)
SFR_DEF(BFBYTEPTRL,        0x91)
SFR_DEF(BFBYTEPTRH,        0x92)
SFR_DEF(BFDATAL,           0x93)
SFR_DEF(BFDATAH,           0x94)
SFR_DEF(BFBITPTR,          0x95)
SFR_DEF(BFCON,             0x96)
SFR_DEF(PCON3,		   0x97)
SFR_DEF(PWKEN,             0x98)
SFR_DEF(PWKEDGE,           0x99)
SFR_DEF(PIE0,              0x9A)
SFR_DEF(SPH,               0x9B)
SFR_DEF(PCON1,             0x9C)
SFR_DEF(ISDCHSUM,          0x9D)
SFR_DEF(RTCDAT,            0x9E)
SFR_DEF(RTCON,             0x9F)


//========= A0H ----> AFH =========//
SFR_DEF(P2,                0xA0)
//SFR_DEF(,             0xA1)
SFR_DEF(SPI1CON,           0xA2)
SFR_DEF(SPI1BUF,           0xA3)
SFR_DEF(ATADR,             0xA4)
SFR_DEF(SPI1DMACNT,        0xA5)
SFR_DEF(SPI1DMASP,         0xA6)
SFR_DEF(IRCON0,            0xA7)
SBIT_DEF(IE0,              0xA8, IE_SINT, IE_AGC, IE_TM1, IE_TM2, IE_MP3, IE_HUFF, IE_USBSOF, IE_EA)
SFR_DEF(IE1,               0xA9)
SFR_DEF(IUCON,             0xAA)
SFR_DEF(IUDAT0,            0xAB)
SFR_DEF(HFMCON1,           0xAC)
SFR_DEF(IRCON1,            0xAD)
SFR_DEF(AGCCON2,           0xAE)
SFR_DEF(SPMODE,            0xAF)

//========= B0H ----> BFH =========//
SFR_DEF(P3,                0xB0)
SFR_DEF(AUCON8,	           0xB1)
SFR_DEF(AUCON9,		   0xB2)
SFR_DEF(AUCON10,    	   0xB3)
SFR_DEF(ERABYT2,	   0xB4)
SFR_DEF(ERABYT3,           0xB5)
SFR_DEF(EMIBUF,           0xB6)
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
SFR_DEF(IISCH0,            0xC0)
SFR_DEF(TMR2CON0,          0xC1)
SFR_DEF(TMR2CON1,          0xC2)
SFR_DEF(IISCH1,            0xC3)
SFR_DEF(RTCON1,             0xC4)
SFR_DEF(SECCNT,             0xC5)
SFR_DEF(OTP_ADR,           0xC6)
SFR_DEF(IRAM_ADR,          0xC7)
SFR_DEF(HFMCON,            0xC8)
SFR_DEF(USBCON0,           0xC9)
SFR_DEF(USBCON1,           0xCA)
SFR_DEF(USBCON2,           0xCB)
SFR_DEF(USBDATA,           0xCC)
SFR_DEF(USBADR,            0xCD)
SFR_DEF(OIRAM_CNT,           0xCE)
SFR_DEF(OIRAM_CON,           0xCF)

SBIT_DEFC(PSW,             0xD0, P, F1, OV, RS0, RS1, F0, AC, CY)

SFR_DEF(HFMCNT,            0xD1)
SFR_DEF(ADCCON,            0xD2)
SFR_DEF(PCON2,             0xD3)
SFR_DEF(ADCDATAL,          0xD4)
SFR_DEF(ADCDATAH,          0xD5)
SFR_DEF(MP3ENC0,           0xD6)
SFR_DEF(MP3ENC1,           0xD7)
SFR_DEF(SPI0BUF,           0xD8)
SFR_DEF(SPI0CON,           0xD9)
SFR_DEF(ADCMODE,           0xDA)
SFR_DEF(CLKCON,            0xDB)
SFR_DEF(CLKCON1,           0xDC)
SFR_DEF(USBDPDM,           0xDD)
SFR_DEF(MP3ENC2,          0xDE)
SFR_DEF(MP3ENC3,          0xDF)

SFR_DEFC(ACC,              0xE0)
SFR_DEF(IPH0,              0xE1)
SFR_DEF(IPH1,              0xE2)
//SFR_DEF(TMR1CON0,          0xE1)
//SFR_DEF(TMR1CON1,          0xE2)
SFR_DEF(MP3ENC4,           0xE3)
SFR_DEF(MP3ENC5,           0xE4)
SFR_DEF(MP3ENC6,           0xE5)
SFR_DEF(MP3ENC7,           0xE6)
SFR_DEF(MP3ENC8,           0xE7)
SFR_DEF(AUCON0,            0xE8)
SFR_DEF(AUCON1,            0xE9)
SFR_DEF(AUCON2,            0xEA)
SFR_DEF(AUCON3,            0xEB)
SFR_DEF(AUCON4,            0xEC)
SFR_DEF(AUCON5,            0xED)
SFR_DEF(AUCON6,            0xEE)
SFR_DEF(AUCON7,            0xEF)

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
SFR_DEF(UARTSTA,           0xFC)
SFR_DEF(UARTCON,           0xFD)
SFR_DEF(IISCON1,           0xFE)
SFR_DEF(UARTDATA,          0xFF)

//FOR SIMULATION
SFR_DEF(SIMEND,            0xFF)

#define SOFT_INT0                0
#define SOFT_INT1                1
#define TIMER1_INT               2
#define TIMER2_INT               3
#define AUDIO_INT                4
#define HUFFMAN_INT              5
#define USB_SOF_INT              6
#define USB_CTL_INT              7
#define SDC_INT                  8
#define PORT_INT                 9
#define SPI0_INT                 10
#define TIMER3_INT               11
#define TIMER0_INT               12
#define RTCC_INT                 13
#define SPI1_INT                 14
/* 注意三个中断共用一个入口，在中断函数里要判断一下标志位 */
#define AGC_INT1                 1
#define UART0_INT                13
#define UART1_INT                13
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
#define BASSCON1      12
#define BASSCOF       13
#define BASSCON2      14
#define BASSVOLIN     15
#define AUADCON1      16
#define AUADCON2      17
#define AUADCBAUD     18

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
#define    RTC_CFG_WCMD      0x55
#define    RTC_CFG_RCMD      0x54
#define    PWR_CTL_WCMD      0x59
#define    PWR_CTL_RCMD      0x58
#define    LCD_CFG0_WCMD     0x5b
#define    LCD_CFG0_RCMD     0x5a
#define    LCD_CFG1_WCMD     0x5d
#define    LCD_CFG1_RCMD     0x5c
#define    OSC_DRV_WCMD      0xa5
#define    OSC_DRV_RCMD      0xa4
#define    WK_EN_WCMD        0xa7
#define    WK_EN_RCMD        0xa6
#define    WK_STA_RCMD       0xa1
#define    LCD_COM_WCMD      0x5f
#define    RTC_CNT_WCMD      0xf0
#define    RTC_CNT_RCMD      0xe0
#define    RTC_ALM_WCMD      0x53
#define    RTC_ALM_RCMD      0x52
#define    RTC_RAM_WCMD      0x57
#define    RTC_RAM_RCMD      0x56
#define    LCD_MAP_WCMD      0xa3
#define    LCD_MAP_RCMD      0xa2

//XSFR
//----------------- 16'h780x -----------------//
#define SDBAUD                XSFR_ADDR(0x7800)
#define SDCPTR                XSFR_ADDR(0x7801)
#define SDDCNT                XSFR_ADDR(0x7802)
#define SDDPTR                XSFR_ADDR(0x7803)
#define RC_TEST               XSFR_ADDR(0x7804)
//Reserve 8'h05	
#define RC_TRIM               XSFR_ADDR(0x7806)
#define PWRCON1               XSFR_ADDR(0x7807)
#define PWRCON2               XSFR_ADDR(0x7808)
#define PIE1                  XSFR_ADDR(0x7809)
#define SPMODE1               XSFR_ADDR(0x780a)
//Reserve 8'h0b	
#define IRDAT0                XSFR_ADDR(0x780c)
#define IRDAT1                XSFR_ADDR(0x780d)
#define IRDAT2                XSFR_ADDR(0x780e)
#define IRDAT3                XSFR_ADDR(0x780f)

//----------------- 16'h781x -----------------//
#define BFEPTRL               XSFR_ADDR(0x7810)
#define BFEPTRH               XSFR_ADDR(0x7811)
#define HFMPTRL               XSFR_ADDR(0x7812)
#define HFMPTRH               XSFR_ADDR(0x7813)
#define UART1CON              XSFR_ADDR(0x7814)
#define UART1BAUD             XSFR_ADDR(0x7815)
#define UART1DATA             XSFR_ADDR(0x7816)
#define UART1STA              XSFR_ADDR(0x7817)
#define UARTDMARXPTR          XSFR_ADDR(0x7818)
#define UARTDMATXPTR          XSFR_ADDR(0x7819)
#define UARTDMARXCNT          XSFR_ADDR(0x781a)
#define UARTDMATXCNT          XSFR_ADDR(0x781b)
#define UARTDIV               XSFR_ADDR(0x781c)
//Reserve 8'h1d	
#define CRCRES0               XSFR_ADDR(0x781e)
#define CRCRES1               XSFR_ADDR(0x781f)

//----------------- 16'h782x -----------------//
#define SPIDMAPTRL            XSFR_ADDR(0x7820)
#define SPIDMAPTRH            XSFR_ADDR(0x7821)
#define SPIDMACNT             XSFR_ADDR(0x7822)
#define SPIBAUD               XSFR_ADDR(0x7823)
//Reserve 8'h24	
#define P1DRV0                XSFR_ADDR(0x7826)
#define P2DRV0                XSFR_ADDR(0x7827)
#define P3DRV0                XSFR_ADDR(0x7828)
#define P1DRV1                XSFR_ADDR(0x7829)
#define P3DRV1                XSFR_ADDR(0x782a)
//Reserve 8'h2b	
//Reserve 8'h2c	
//Reserve 8'h2d	
#define SPI1CON1              XSFR_ADDR(0x782e)
//Reserve 8'h2f	

//----------------- 16'h784x -----------------//
#define P0PU0                 XSFR_ADDR(0x7825)     //控制P00 P01 P04 P05 P06
#define P0PU1                 XSFR_ADDR(0x7840)     //控制P07
#define P1PU0                 XSFR_ADDR(0x7841)
#define P2PU0                 XSFR_ADDR(0x7842)
#define P3PU0                 XSFR_ADDR(0x7843)
//Reserve 8'h44	
#define P2PU1                 XSFR_ADDR(0x7845)
//Reserve 8'h46	
#define P0PD0                 XSFR_ADDR(0x7847)
#define P1PD0                 XSFR_ADDR(0x7848)
#define P2PD0                 XSFR_ADDR(0x7849)
#define P3PD0                 XSFR_ADDR(0x784a)
#define P3PUD1                XSFR_ADDR(0x784b)
#define P2PD1                 XSFR_ADDR(0x784c)
#define P3PD1                 XSFR_ADDR(0x784d)
#define PMUXCON0              XSFR_ADDR(0x784e)
#define PLLCON2               XSFR_ADDR(0x784f)

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
#define PLLFRACH	      XSFR_ADDR(0x785c)
#define PLLFRACL              XSFR_ADDR(0x785d)
#define PLLINTH               XSFR_ADDR(0x785e)
#define PLLINTL               XSFR_ADDR(0x785f)

//----------------- 16'h786x -----------------//
#define TMR2CNTL              XSFR_ADDR(0x7860)
#define TMR2CNTH              XSFR_ADDR(0x7861)
#define TMR2PRL               XSFR_ADDR(0x7862)
#define TMR2PRH               XSFR_ADDR(0x7863)
#define TMR2PWML              XSFR_ADDR(0x7864)
#define TMR2PWMH              XSFR_ADDR(0x7865)
#define FMAMCON0              XSFR_ADDR(0x7866)
#define FMAMCON1	      XSFR_ADDR(0x7867)
#define FMAMCNTL              XSFR_ADDR(0x7868)
#define FMAMCNTH              XSFR_ADDR(0x7869)
#define FMAMTPR               XSFR_ADDR(0x786a)
#define EMICON0               XSFR_ADDR(0x786b)
#define EMICON1               XSFR_ADDR(0x786c)
//Reserve 8'h6d	
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
//Reserve 8'h7e	
//Reserve 8'h7f	

//----------------- 16'h788x -----------------//
#define ADCBAUD               XSFR_ADDR(0x7880)
#define IUGAT                 XSFR_ADDR(0x7881)
#define ECN                   XSFR_ADDR(0x7882)
#define ID0                   XSFR_ADDR(0x7883)
#define ID1                   XSFR_ADDR(0x7884)
#define IUDAT1                XSFR_ADDR(0x7885)
#define IUADR                 XSFR_ADDR(0x7886)
#define IUBP                  XSFR_ADDR(0x7887)
#define UARTBAUD              XSFR_ADDR(0x7888)
#define UARTBAUDH             XSFR_ADDR(0x7889)
#define LFSR32_DAT0           XSFR_ADDR(0x788a)
#define LFSR32_DAT1           XSFR_ADDR(0x788b)
#define LFSR32_DAT2           XSFR_ADDR(0x788c)
#define LFSR32_DAT3           XSFR_ADDR(0x788d)
//Reserve 8'h8e	
//Reserve 8'h8f	

//----------------- 16'h789x -----------------//
#define ATCON0	              XSFR_ADDR(0x7890)
#define ATCON1		      XSFR_ADDR(0x7891)
#define ATCON2		      XSFR_ADDR(0x7892)
#define ATCON3                XSFR_ADDR(0x7893)
#define ATCON4                XSFR_ADDR(0x7894)
#define ATCON5                XSFR_ADDR(0x7895)
//Reserve 8'h96
#define KVCON1                XSFR_ADDR(0x7897)
#define KVCON2                XSFR_ADDR(0x7898)
#define KVCADR                XSFR_ADDR(0x7899)
//#define AGCDATH               XSFR_ADDR(0x789a)
//#define AGCDATL               XSFR_ADDR(0x789b)
//Reserve 8'h9c
//Reserve 8'h9d
//Reserve 8'h9e
//Reserve 8'h9f

//----------------- 16'h78ax -----------------//
#define FFTCON                XSFR_ADDR(0x78a0)
#define FFTADR                XSFR_ADDR(0x78a1)
//Reserve 8'ha2
//Reserve 8'ha3
#define CLKCON2               XSFR_ADDR(0x78a4)
//Reserve 8'ha5
//Reserve 8'ha6
//Reserve 8'ha7
//Reserve 8'ha8
//Reserve 8'ha9
//Reserve 8'haa
//Reserve 8'hab
//Reserve 8'hac
//Reserve 8'had
//Reserve 8'hae
//Reserve 8'haf
	
//----------------- 16'h78cx -----------------//
#define IISCNT                XSFR_ADDR(0x78c0)
#define IISBAUD               XSFR_ADDR(0x78c1)
#define IISCKCON              XSFR_ADDR(0x78c2)
#define IISADR                XSFR_ADDR(0x78c3)
#define IISCON0               XSFR_ADDR(0x78c4)
//Reserve 8'hc5
//Reserve 8'hc6
//Reserve 8'hc7
//Reserve 8'hc8
//Reserve 8'hc9
//Reserve 8'hca
//Reserve 8'hcb
//Reserve 8'hcc
//Reserve 8'hcd
//Reserve 8'hce
//Reserve 8'hcf

//----------------- 16'h78dx -----------------//
#define AGCCON0               XSFR_ADDR(0x78d0)
#define AGCCON1               XSFR_ADDR(0x78d1)
#define AGCANLCON             XSFR_ADDR(0x78d2)
#define AGCCON3               XSFR_ADDR(0x78d3)
#define AGCDMACON             XSFR_ADDR(0x78d4)
#define AGCDMAADR             XSFR_ADDR(0x78d5)
#define AGCDATH               XSFR_ADDR(0x78d6)
#define AGCDATL               XSFR_ADDR(0x78d7)
#define DCVAL_LH              XSFR_ADDR(0x78d8)
#define DCVAL_LL              XSFR_ADDR(0x78d9)
#define DCVAL_RH              XSFR_ADDR(0x78da)
#define DCVAL_RL              XSFR_ADDR(0x78db)
#define BS_BEGIN_ADR          XSFR_ADDR(0x78dc)
#define BS_END_ADR            XSFR_ADDR(0x78dd)
#define AGCSETDATA            XSFR_ADDR(0x78de)
#define AGCSETCNT             XSFR_ADDR(0x78df)

#endif
