/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的IO设置
 *****************************************************************************/
#ifndef USER_IO_H
#define USER_IO_H

//USB DPDM设置成普通IO
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

//P34下降沿唤醒设置
#define PWK_P34_IS_PEND()               (PWKEN & BIT(6))
#define PWK_P34_INIT()                  {P3DIR |= BIT(4);\
                                        P3PU0 |= BIT(7);\
                                        PMUXCON0 &= ~(BIT(3)|BIT(2));\
                                        PWKEDGE |= BIT(2);\
                                        PWKEN &= ~(BIT(6)|BIT(2));}

//P07下降沿唤醒设置
#define PWK_P07_IS_PEND()               (PWKEN & BIT(7))
#define PWK_P07_INIT()                  {P0DIR |= BIT(7);\
                                        P0PU1 |= BIT(7);\
                                        PMUXCON0 &= ~(BIT(3)|BIT(2));\
                                        PWKEDGE |= BIT(3);\
                                        PWKEN &= ~(BIT(7)|BIT(3));}

//以下是自动选择的宏
#if BT_PWK_SEL == 1
    #define PWK_INIT()                  PWK_P07_INIT()
    #define PWK_IS_PEND()               PWK_P07_IS_PEND()
#else
    #define PWK_INIT()                  PWK_P34_INIT()
    #define PWK_IS_PEND()               PWK_P34_IS_PEND()
#endif


//SDIO定义
#if (SD_PORT_SEL == 0 || SD_PORT_SEL == 1)
    #define SD_PORT_SET()               PMUXCON0 |= BIT(0)
    #define SD_CLK_DIR_OUT()            P2DIR &= ~BIT(1)
    #define SD_CLK_DIR_IN()             P2DIR |= BIT(1)
    #define SD_CLK_PU_10K()             P2PU0 = (P2PU0 & 0xf3) | BIT(2)
    #define SD_CLK_PU_DIS()             P2PU0 &= ~(BIT(3)|BIT(2))
    #define SD_CLK_IS_10KPU()           (0 == (P2PU0 & BIT(2)))
    #define SD_CLK_IS_IN()              (0 != (P2DIR & BIT(1)))
    #define SD_CLK_STA()                (P2 & BIT(1))
    #define SD_CLK_SET()                P2 |= BIT(1)
    #define SD_CLK_CLR()                P2 &= ~BIT(1)

    #define SD_CMD_DIR_OUT()            P2DIR &= ~BIT(0)
    #define SD_CMD_DIR_IN()             P2DIR |= BIT(0)
    #define SD_CMD_PU_10K()             P2PU0 = (P2PU0 & 0xfc) | BIT(0)
    #define SD_CMD_PU_500R()            P2PU0 = (P2PU0 & 0xfc) | BIT(1)
    #define SD_CMD_PU_DIS()             P2PU0 &= ~(BIT(1)|BIT(0))
    #define SD_CMD_IS_500RPU()          (0 == (P2PU0 & BIT(0)))
    #define SD_CMD_STA()                (P2 & BIT(0))
    #define SD_CMD_SET()                P2 |= BIT(0)
    #define SD_CMD_CLR()                P2 &= ~BIT(0)

    #define SD_DAT_DIR_OUT()            P2DIR &= ~BIT(7)
    #define SD_DAT_DIR_IN()             P2DIR |= BIT(7)
    #define SD_DAT_PU_10K()             P2PU0 |= BIT(6); P2PU0 &= ~BIT(7)
    #define SD_DAT_PU_DIS()             P2PU0 &= ~(BIT(7)|BIT(6))

#elif (SD_PORT_SEL == 2 || SD_PORT_SEL == 3)
    #define SD_PORT_SET()               PMUXCON0 &= ~BIT(0)
    #define SD_CLK_DIR_OUT()            P3DIR &= ~BIT(0)
    #define SD_CLK_DIR_IN()             P3DIR |= BIT(0)
    #define SD_CLK_PU_10K()             P3PU0 = (P3PU0 & 0xfc) | BIT(0)
    #define SD_CLK_PU_DIS()             P3PU0 &= ~(BIT(1)|BIT(0))
    #define SD_CLK_IS_10KPU()           (0 == (P3PU0 & BIT(0)))
    #define SD_CLK_IS_IN()              (0 != (P3DIR & BIT(0)))
    #define SD_CLK_STA()                (P3 & BIT(0))
    #define SD_CLK_SET()                P3 |= BIT(0)
    #define SD_CLK_CLR()                P3 &= ~BIT(0)

    #define SD_CMD_DIR_OUT()            P3DIR &= ~BIT(1)
    #define SD_CMD_DIR_IN()             P3DIR |= BIT(1)
    #define SD_CMD_PU_10K()             P3PU0 = (P3PU0 & 0xf3) | BIT(2)
    #define SD_CMD_PU_500R()            P3PU0 = (P3PU0 & 0xf3) | BIT(3)
    #define SD_CMD_PU_DIS()             P3PU0 &= ~(BIT(3)|BIT(2))
    #define SD_CMD_IS_500RPU()          (0 == (P3PU0 & BIT(2)))
    #define SD_CMD_STA()                (P3 & BIT(1))
    #define SD_CMD_SET()                P3 |= BIT(1)
    #define SD_CMD_CLR()                P3 &= ~BIT(1)

    #define SD_DAT_DIR_OUT()            P3DIR &= ~BIT(2)
    #define SD_DAT_DIR_IN()             P3DIR |= BIT(2)
    #define SD_DAT_PU_10K()             P3PU0 |= BIT(4); P2PU0 &= ~BIT(5)
    #define SD_DAT_PU_DIS()             P3PU0 &= ~(BIT(5)|BIT(4))
#endif

//SDCLK复用SD检测定义（SDCLK空闲时10K上拉输入，通信时输出）
#if (SD_PORT_SEL == 1)
    #define IS_SD_CHK_BUSY()            SD_CLK_IS_10KPU()
    #define IS_SD_IN()                  (0 == SD_CLK_STA())
#elif (SD_PORT_SEL == 3)
    #define IS_SD_CHK_BUSY()            SD_CLK_IS_10KPU()
    #define IS_SD_IN()                  (0 == SD_CLK_STA())
#else
    #define IS_SD_CHK_BUSY()            0
#endif

//SDCMD复用耳机检测定义（SDCMD空闲时开10K上拉，通信时开500R上拉）
#if SDCMD_REUSE_EARDET
    #define LINEIN_DETECT_INIT()
    #define IS_EAR_DETECT_BUSY()        SD_CMD_IS_500RPU()
    #define IS_EAR_IN()                 (0 == SD_CMD_STA())
#elif EAR_DETECT_REUSE_IICCLK
    #define IS_EAR_DETECT_BUSY()        iic_busy_flag
#else
    #define IS_EAR_DETECT_BUSY()        0
#endif

//SDCMD复用LineIn检测定义（SDCMD空闲时开10K上拉，通信时开500R上拉）
#if SDCMD_REUSE_LINEIN_DET && !LINEIN_DETECT_REUSE_IICCLK
    #define LINEIN_DETECT_INIT()
    #define IS_LINEIN_DETECT_BUSY()     SD_CMD_IS_500RPU()
    #define IS_LINEIN_IN()              (0 == SD_CMD_STA())
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


#if VOL_CTRL_MUTE
    #define HSHF_VOL_IS_ZERO()      (sys_ctl.hshf_vol == 0 && !sys_flag.voice_play_flag)    //通话音量为0时MUTE功放
    #define DAC_VOL_IS_ZERO()       (ATCON0 & BIT(0))                                       //动态降噪（DAC音量为0）时MUTE功放
#else
    #define HSHF_VOL_IS_ZERO()      0                                                       //通话时解MUTE功放
    #define DAC_VOL_IS_ZERO()       0                                                       //动态降噪（DAC音量为0）不影响MUTE功放
#endif


void user_io_init(void);
__near_func void usb_detect(void);
__near_func void sd_detect(void);
__near_func void linein_detect(void);
__near_func void earphone_detect(void);
__near_func bool is_sys_mute(void);
__near_func bool is_need_mute(void);
__near_func void mute_ctl_auto(void);
//打印串口选择，1：P01，0：P16
void printf_io_select(u8 iotmp);
void music_mute(void);
void music_unmute(void);

#define WATCHDOG_INIT()             WDTCON = 0x1d           //看门狗时间：2s
#define WATCHDOG_CLR()              WDTCON |= BIT(5)

#endif
