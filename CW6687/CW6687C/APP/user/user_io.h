/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Function  : �û���IO����
 *****************************************************************************/
#ifndef USER_IO_H
#define USER_IO_H

//USB DPDM���ó���ͨIO
#define USB_DPDM_GPIO_EN()              USBDPDM |= BIT(4); USBCON0 |= BIT(6)|BIT(5)|BIT(4)  //DPDM is controlled by CPU
#define USB_DPDM_GPIO_DIS()             USBCON0 &= ~BIT(4); USBDPDM &= ~BIT(4)              //DPDM is controlled by SIE
#define USB_DP_OUTPUT()                 USBDPDM &= ~BIT(3)
#define USB_DP_INPUT()                  USBDPDM |= BIT(3)
#define USB_DM_OUTPUT()                 USBDPDM &= ~BIT(2)
#define USB_DM_INPUT()                  USBDPDM |= BIT(2)
#define USB_DM_HIHG()                   USBDPDM |= BIT(0)
#define USB_DM_LOW()                    USBDPDM &= ~BIT(0)
#define USB_DM_TOG()                    USBDPDM ^= BIT(0)
#define USB_DP_HIHG()                   USBDPDM |= BIT(1)
#define USB_DP_LOW()                    USBDPDM &= ~BIT(1)
#define USB_DP_TOG()                    USBDPDM ^= BIT(1)
#define GET_DM_STATE()                  (USBDPDM & BIT(6))
#define GET_DP_STATE()                  (USBDPDM & BIT(7))

//SDIO����
#if (SD_PORT_SEL == 0 || SD_PORT_SEL == 1)
    #define SD_PORT_SET()               PMUXCON0 |= BIT(0)
    #define SD_CLK_DIR_OUT()            P2DIR &= ~BIT(1)
    #define SD_CLK_DIR_IN()             P2DIR |= BIT(1)
    #define SD_CLK_PU_10K()             P2PU |= BIT(1)
    #define SD_CLK_PU_DIS()             P2PU &= ~BIT(1))
    #define SD_CLK_IS_10KPU()           (0 == (P2PU & BIT(1)))
    #define SD_CLK_IS_IN()              (0 != (P2DIR & BIT(1)))
    #define SD_CLK_STA()                (P2 & BIT(1))
    #define SD_CLK_SET()                P2 |= BIT(1)
    #define SD_CLK_CLR()                P2 &= ~BIT(1)

    #define SD_CMD_DIR_OUT()            P2DIR &= ~BIT(0)
    #define SD_CMD_DIR_IN()             P2DIR |= BIT(0)
    #define SD_CMD_PU_10K()             P2PUS0 &= ~BIT(0);P2PU |= BIT(0)
    #define SD_CMD_PU_500R()            P2PUS0 |= BIT(0);P2PUS1 &= ~BIT(0);P2PU |= BIT(0)
    #define SD_CMD_PU_DIS()             P2PU &= ~BIT(0)
    #define SD_CMD_IS_500RPU()          (P2PUS0 & BIT(0))
    #define SD_CMD_STA()                (P2 & BIT(0))

    #define SD_DAT_DIR_OUT()            P2DIR &= ~BIT(7)
    #define SD_DAT_DIR_IN()             P2DIR |= BIT(7)
    #define SD_DAT_PU_10K()             P2PU |= BIT(7)
    #define SD_DAT_PU_DIS()             P2PU &= ~BIT(7)

#elif (SD_PORT_SEL == 2 || SD_PORT_SEL == 3)
    #define SD_PORT_SET()               PMUXCON0 &= ~BIT(0)
    #define SD_CLK_DIR_OUT()            P3DIR &= ~BIT(0)
    #define SD_CLK_DIR_IN()             P3DIR |= BIT(0)
    #define SD_CLK_PU_10K()             P3PU |= BIT(0)
    #define SD_CLK_PU_DIS()             P3PU &= ~BIT(0)
    #define SD_CLK_IS_10KPU()           (0 == (P3PU & BIT(0)))
    #define SD_CLK_IS_IN()              (0 != (P3DIR & BIT(0)))
    #define SD_CLK_STA()                (P3 & BIT(0))
    #define SD_CLK_SET()                P3 |= BIT(0)
    #define SD_CLK_CLR()                P3 &= ~BIT(0)

    #define SD_CMD_DIR_OUT()            P3DIR &= ~BIT(1)
    #define SD_CMD_DIR_IN()             P3DIR |= BIT(1)
    #define SD_CMD_PU_10K()             P3PUS0 &= ~BIT(1);P3PU |= BIT(1)
    #define SD_CMD_PU_500R()            P3PUS0 |= BIT(1);P3PU |= BIT(1)
    #define SD_CMD_PU_DIS()             P3PU &= ~BIT(1)
    #define SD_CMD_IS_500RPU()          (P3PUS0 & BIT(1))
    #define SD_CMD_STA()                (P3 & BIT(1))

    #define SD_DAT_DIR_OUT()            P3DIR &= ~BIT(2)
    #define SD_DAT_DIR_IN()             P3DIR |= BIT(2)
    #define SD_DAT_PU_10K()             P3PU |= BIT(2);
    #define SD_DAT_PU_DIS()             P3PU &= ~BIT(2)
#endif


#if SD_CMD_REUSE_SD_DETECT  //SDCMD����SD���
    #define IS_SD_IN()                  (0 == SD_CMD_STA())
    #define IS_SD_DETECT_BUSY()         SD_CMD_IS_500RPU()
#else   //SDCLK����SD��ⶨ�壨SDCLK����ʱ10K�������룬ͨ��ʱ�����
    #if (SD_PORT_SEL == 1)  //��⸴��SD_CLK
        #define IS_SD_CHK_BUSY()            SD_CLK_IS_10KPU()
        #define IS_SD_IN()                  (0 == SD_CLK_STA())
    #elif (SD_PORT_SEL == 3)  //��⸴��SD_CLK
        #define IS_SD_CHK_BUSY()            SD_CLK_IS_10KPU()
        #define IS_SD_IN()                  (0 == SD_CLK_STA())
    #endif
#endif

//SDCMD���ö�����ⶨ�壨SDCMD����ʱ��10K������ͨ��ʱ��500R������
#if SDCMD_REUSE_EARDET
    #define LINEIN_DETECT_INIT()
    #define IS_EAR_DETECT_BUSY()        SD_CMD_IS_500RPU()
    #define EAR_IS_IN()                 (0 == SD_CMD_STA())
#elif EAR_DETECT_REUSE_IICCLK
    #define IS_EAR_DETECT_BUSY()        iic_busy_flag
#else
    #define IS_EAR_DETECT_BUSY()        0
#endif

//SDCMD����LineIn��ⶨ�壨SDCMD����ʱ��10K������ͨ��ʱ��500R������
#if SDCMD_REUSE_LINEIN_DET && !LINEIN_DETECT_REUSE_IICCLK
    #define LINEIN_DETECT_INIT()
    #define IS_LINEIN_DETECT_BUSY()     SD_CMD_IS_500RPU()
    #define IS_LINEIN_IN()              (0 == SD_CMD_STA())
    #define LINEIN_DETECT_END()
#elif SDCMD_REUSE_LINEIN_DET && LINEIN_DETECT_REUSE_IICCLK
    #define LINEIN_DETECT_INIT()        SD_CMD_DIR_IN(); SD_CMD_PU_10K();\
                                        asm("nop");asm("nop");asm("nop");asm("nop");\
                                        asm("nop");asm("nop");asm("nop");asm("nop");
    #define IS_LINEIN_DETECT_BUSY()     SD_CMD_IS_500RPU() || iic_busy_flag
    #define IS_LINEIN_IN()              (0 == SD_CMD_STA())
    #define LINEIN_DETECT_END()         SD_CMD_DIR_OUT();// SD_CMD_PU_DIS();
#else
    #define IS_LINEIN_DETECT_BUSY()     0
#endif


#if WKO_OUTPUT_EN
extern bool volatile wko_set_kick;
extern bool volatile wko_set_sta;
#define SET_WKO_HIGH()   {wko_set_kick = true;  wko_set_sta = true;}   //�����ֱ�ӵ���SET_WKO_HIGH()����
#define SET_WKO_LOW()    {wko_set_kick = true;  wko_set_sta = false;}  //�����ֱ�ӵ���SET_WKO_LOW()����
extern __near_func wko_state_set(void);
#endif   //WKO_OUTPUT_EN

void user_io_init(void);
void sd_port_reuse(void);
void ssop28_pmu_init(void);
void lqfp48_pmu_init(void);
__near_func void usb_detect(void);
__near_func void sd_detect(void);
__near_func void linein_detect(void);
__near_func void earphone_detect(void);
__near_func void adc_key_kick(void);
__near_func u8 usb_remove_check(void);
__near_func void speaker_mute(void);            //MUTE ���ţ����Ⱦ���
__near_func void speaker_unmute(void);          //UNMUTE ���ţ����ȳ���

#define WATCHDOG_INIT()             WDTCON = 0x1d           //���Ź�ʱ�䣺2s
#define WATCHDOG_CLR()              WDTCON |= BIT(5)

#endif
