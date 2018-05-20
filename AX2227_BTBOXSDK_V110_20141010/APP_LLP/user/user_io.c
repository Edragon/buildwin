/*****************************************************************************
 * Module    : User
 * File      : user_io.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : �û���IO����
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "user_fm.h"
#include "user_bt.h"
#include "btapi.h"


//�û�IO��ʼ��
void user_io_init(void)
{
#if USB_GPIO_EN
    USB_DPDM_GPIO_EN();
    USBCON1 &= ~(BIT(7)|BIT(5)|BIT(4));        //�ص�DP/DM��������DM��100k�����ɲ���
#endif

#if LED_GREEN_BLINK
    LED_GREEN_INIT();
#endif

#if LED_BLUE_BLINK
    LED_BLUE_INIT();
#endif

#if SOFT_POWER_ON_OFF
    SOFT_POWER_CTL_INIT();
    SOFT_POWER_CHK_INIT();
#endif

    MUSIC_MUTE();
    OPA_CTL_INIT();

#if(USE_P35_USBPOW_CTL == 1)
    PIE1 |= BIT(3);                 //USBģ�鹩��
    P3DIR |= BIT(5);
    irtcc_write_cfg(LCD_MAP_WCMD, BIT(6));
#elif(!USE_P35_USBPOW_CTL)
    PIE1 &= ~BIT(3);                //MUTE��,��Ϊ�������
#endif

#if TASK_FM_EN
    fm_io_init();                   //FM IO��ʼ��
#endif

#if MP3BT_DECT_EN && !MP3BT_DECT_REUSE_EN
    MP3BT_DECT_INIT();              //����MP3-BT���
#endif

#if LINEIN_DETECT_EN
    LINEIN_DETECT_INIT();           //LINEIN���
#endif

#if EAR_DETECT_EN
    EAR_DETECT_INIT();              //�������
#endif

#if SD_CARD_EN
  //SD CLK���ü��ų�ʼ��
  #if(SD_PORT_SEL == 0)
    PMUXCON0 |= BIT(0);
  #elif(SD_PORT_SEL == 1)
    PMUXCON0 |= BIT(0);
    P2DIR |= BIT(1) | BIT(0);       //CLK����SD�����
    P2PU0 |= BIT(2) | BIT(0);       //������
    P2PU0 &= ~BIT(3);
  #elif (SD_PORT_SEL == 2)
    PMUXCON0 &= ~BIT(0);
  #elif (SD_PORT_SEL == 3)
    PMUXCON0 &= ~BIT(0);
    P3DIR |= BIT(0);                //CLK����SD�����
    P3PU0 |= BIT(0);                //������
    P3PU0 &= ~BIT(1);
  #endif

  //SD �������ų�ʼ��
    SD_DETECT_INIT();
  #if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET
    SD_CMD_DIR_IN();
    SD_CMD_PU_10K();
  #endif

  #if SDCLK_REUSE_ADKEY
    SD_CLK_DIR_IN();
  #endif
#endif


    DCIN_INIT();
    BTLDO_DIR_INIT();
    USBPOW_INIT();
}

//USB��⺯�y
__near_func void usb_detect(void)
{
#if IICCLK_REUSE_USB
    if((USBDPDM & BIT(4)) != 0) {   //dpdm as gpio
        return;
    }
#endif

    u8 usb_conn = usb_connected();

#if TASK_USBDEV_EN
    if (USB_CONNECTED_PC == usb_conn) {
        if (device_insert(DEVICE_PC)) {
            //printf("PC IN\n");
        }
    } else {
        if (device_remove(DEVICE_PC)) {
            //printf("PC OUT\n");
        }
    }
#endif

    if (USB_CONNECTED_UDISK == usb_conn) {
        if (device_insert(DEVICE_UDISK)) {
            task_ctl.dev_change = 1;
            //printf("UDISK IN\n");
        }
    } else {
        if (usb_remove_check()) {
            //ͨ�Ŵ���ż��γ�
            if (device_remove(DEVICE_UDISK)) {
                task_ctl.dev_change = 1;
                //printf("UDISK OUT\n");
            }
        }
    }
}

//���SD���Ƿ�����
__near_func void sd_detect(void)
{
#if (SD_PORT_SEL == 1) || (SD_PORT_SEL == 3)
    if(IS_SD_CHK_BUSY()) {  //���ü��
        return;             //����δ�򿪣�����CLK���ģʽ
    }
#endif

    if (IS_SD_IN()) {
        SD_DETECT_END();
        if (device_insert(DEVICE_SDMMC)) {
            task_ctl.dev_change = 1;
            //printf("SD IN\n");
        }
    }
    else {
        SD_DETECT_END();
        if (device_remove(DEVICE_SDMMC)) {
            task_ctl.dev_change = 1;
            //printf("SD OUT\n");
        }
    }
}

#if LINEIN_DETECT_EN
//���LineIn
__near_func void linein_detect(void)
{
#if SDCMD_REUSE_LINEIN_DET || LINEIN_DETECT_REUSE_IICCLK
    if (IS_LINEIN_DETECT_BUSY()) {
        return;
    }
#endif

#if LINEIN_DETECT_REUSE_IICCLK
    LINEIN_DETECT_INIT();
#endif

    if (IS_LINEIN_IN()) {
        LINEIN_DETECT_END();
        if (device_insert(DEVICE_LINEIN)) {
            //printf("LINE IN\n");
        }
    } else {
        LINEIN_DETECT_END();
        if (device_remove(DEVICE_LINEIN)) {
            //printf("LINE OUT\n");
        }
    }
}
#endif

#if EAR_DETECT_EN
//�������Ƿ����
__near_func void earphone_detect(void)
{
#if SDCMD_REUSE_EARDET || EAR_DETECT_REUSE_IICCLK
    if (IS_EAR_DETECT_BUSY()) {
        return;
    }
#endif

#if EAR_DETECT_REUSE_IICCLK
    EAR_DETECT_INIT();
#endif

    if(IS_EAR_IN()) {
        EAR_DETECT_END();
        if(device_insert(DEVICE_EAR)) {
            //printf("EAR IN\n");
        }
    } else {
        EAR_DETECT_END();
        if(device_remove(DEVICE_EAR)) {
            //printf("EAR OUT\n");
        }
    }
    if (device_is_online(DEVICE_EAR)) {
        MUSIC_MUTE();
    } else {
        mute_ctl_auto();
    }
}
#endif

//�ж��Ƿ���ҪMUTE���ţ����жϵ���
__near_func bool is_sys_mute(void)
{
    return (sys_flag.mute_flag)? true : false;
}

//�ж��Ƿ���ҪMUTE���ţ����жϵ���
__near_func bool is_need_mute(void)
{
    //ϵͳMUTE������Ϊ0���߷ǲ���״̬ʱ������MUTE����
    return (sys_flag.mute_flag || sys_flag.need_mute || sys_ctl.volume == 0 || DAC_VOL_IS_ZERO())? true : false;
}

__near_func void mute_ctl_auto(void)
{
    if((BT_STATE != StateNone) && (HSFCallActive | HSFRingActive | scoflag)) {
        if(HSHF_VOL_IS_ZERO()) {
            MUSIC_MUTE();
        } else {
            MUSIC_UNMUTE();     //ͨ�����̲�MUTE
        }
    } else {
        if((!sys_flag.voice_play_flag) && is_need_mute()) {
            MUSIC_MUTE();
        } else {
            MUSIC_UNMUTE();
        }
    }
}

//��ӡ����ѡ��1��P01��0��P16
void printf_io_select(u8 iotmp)
{
    if(iotmp)
    {
#if (SYS_PKG == PKG_SSOP24)
        P1DIR |= BIT(6)|BIT(7);
        P1PU0 &= ~(BIT(6)|BIT(7));
        P1PD0 &= ~(BIT(6)|BIT(7));
#endif
        UARTSTA = 0x01;
        P0DIR &= ~BIT(1);
    }
    else
    {
        UARTSTA = 0x00;
        P0DIR |= BIT(1);
        P1DIR &= ~BIT(6);
    }
}

//����
void music_mute(void)
{
    sys_flag.mute_flag = 1;
}

//�Ǿ������������ţ���Ϊ������delay_5ms�����뿪�жϺ���ܵ���
void music_unmute(void)
{
    if(sys_flag.mute_flag || sys_ctl.volume==0) {//�����ظ�����ʱ��ʱ��, 2014-1-2, ϵͳ����Ϊ0ʱҲ��Ҫ��ʱ
        sys_flag.mute_flag = 0;
        delay_5ms(2);                           //2014-5-9���˴���ʱ���ûᵼ������ģʽ�³���һЩ�쳣�����Ž�MUTE��Ҫ����ʱ�ģ���������ʱ�ж���ʱ��
    }
}

__near_func void adc_key_kick(void);
__near_func sd_port_reuse_hook(u8 type)
{
#if SDCMD_REUSE_EARDET || SDCMD_REUSE_LINEIN_DET
    if (type == 0 || type == 1) {
        SD_CMD_PU_500R();           //SDCMD���ü��ʱ����500R����ͨ��
    } else {
        SD_CMD_PU_10K();            //����ʱ����10K�������ڼ��
    }
#endif

#if SDCLK_REUSE_ADKEY
    bool ie_ea = IE_EA;
    IE_EA = 0;
    if (type == 0 || type == 1) {
        SD_CLK_DIR_OUT();           //CLK����ʱ�����
    } else {
        SD_CLK_DIR_IN();            //CLK����ADKEY���
        SD_CLK_PU_DIS();            //������
        adc_key_kick();
    }
    IE_EA = ie_ea;
#endif
}
