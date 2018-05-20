/*****************************************************************************
 * Module    : User
 * File      : user_fm.c
 * Author    : Hanny
 * Function  : FM��������
 *****************************************************************************/
#include "include.h"
#include "user_fm.h"

type_fm_ctl fm_ctl;
u8 fm_online_flag = TASK_FM_EN;


#if !FM_VOUT_THROUGH_MCU
    #define FM_VOL      sys_ctl.volume
#else
    #define FM_VOL      15
#endif

#if ICC_REUSE_EN
volatile u8 iic_busy_flag = 0;
#endif


//1us Delay
#pragma location="FM_IIC_SEG"
__near_func void iic_delay(void)
{
   u8 i;
#if (FM_CLK_SEL == FMOSC_REUSE_32KSDCLK)
   i = 4;  //sys_clk = 12m
#else
   i = 10; //sys_clk = 24m
#endif
   for (; i!=0; i--) {  //SysCLK_12M //60-- 35K   //10--86k  //10
        asm("nop");
    }
}

//2us Delay
#pragma location="FM_IIC_SEG"
__near_func void iic_delay_2(void)
{
    u8 i;
#if (FM_CLK_SEL == FMOSC_REUSE_32KSDCLK)
   i = 7; //sys_clk = 12m
#else
   i = 15;  //sys_clk = 24m
#endif
    for (i=7; i!=0; i--) {
        asm("nop");
    }
}

#pragma location="FM_IIC_SEG"
__near_func void iic_start(void)
{
#if IIC_REUSE_SD
    SD_CMD_PU_DIS();
#endif
    IIC_DATA_CLK_OUT();
    IIC_DATA_CLK_HIGH();
    iic_delay();
    IIC_DATA_LOW();
    iic_delay();
    IIC_CLK_LOW();
}

//����һ��IICͨ��
#pragma location="FM_IIC_SEG"
__near_func void iic_stop(void)
{
    IIC_DATA_OUT();
    IIC_DATA_LOW();
    iic_delay_2();
    IIC_CLK_HIGH();
    iic_delay();
    IIC_DATA_HIGH();
#if IIC_REUSE_SD
    SD_CMD_DIR_IN();
    SD_CMD_PU_10K();
#endif
}

//IIC���߷���1byte
#pragma location="FM_IIC_SEG"
__near_func void iic_sendbyte(u8 val)
{
    u8 i;
    IIC_DATA_OUT();
    for (i=8; i!=0; i--) {
        if (val & BIT(7)) {
            IIC_DATA_HIGH();
        } else {
            IIC_DATA_LOW();
        }
        iic_delay_2();
        IIC_CLK_HIGH();
        iic_delay();
        IIC_CLK_LOW();
        val <<= 1;
    }
}

//IIC���߽���1byte
#pragma location="FM_IIC_SEG"
__near_func u8 iic_getbyte(void)
{
    u8 i;
    u8 val = 0;
    IIC_DATA_IN();
    for (i=8; i!=0; i--) {
        iic_delay_2();
        IIC_CLK_HIGH();
        iic_delay();
        val <<= 1;
        if (IIC_DATA_IS_HIGH()) {
            val |= BIT(0);
        }
        IIC_CLK_LOW();
    }
    return val;
}

//IIC���߷���ACK
#pragma location="FM_IIC_SEG"
__near_func void iic_sendack(void)
{
    IIC_DATA_OUT();
    IIC_DATA_LOW();
    iic_delay_2();
    IIC_CLK_HIGH();
    iic_delay();
    IIC_CLK_LOW();
}

//IIC���߷���NACK
#pragma location="FM_IIC_SEG"
__near_func void iic_sendnack(void)
{
    IIC_DATA_OUT();
    IIC_DATA_HIGH();
    iic_delay_2();
    IIC_CLK_HIGH();
    iic_delay();
    IIC_CLK_LOW();
}

//IIC���߻�ȡACK״̬
#pragma location="FM_IIC_SEG"
__near_func bool iic_getack(void)
{
    bool ack = false;
    IIC_DATA_IN();
    iic_delay_2();
    IIC_CLK_HIGH();
    iic_delay();
    if (!IIC_DATA_IS_HIGH()) {
        //printf("t\n");
        ack = true;
    }
    IIC_CLK_LOW();
   // printf("f\n");
    return ack;
}

#pragma location="FM_SEG"
#pragma optimize=no_inline  //�˴�no_inline����ȥ�����������ʱ�ᱻ�Ż����������Ӷ�������Bank����iic_stop()���ܻᵼ������
void iic_reuse_init(void)
{
#if IICCLK_REUSE_USB && !USB_GPIO_EN
    USB_DPDM_GPIO_EN();
#endif

#if ICC_REUSE_EN
    //��ֹ��ICC���ã��ڷ�IICͨ��״̬�¸��ýŵ�ƽ�ı仯���ܻ�ʹ��ICC�жϵ�iic_start�������Ӷ�����һ��ICCͨ��ʱ����
#if FM_RECORD_EN
      SDCON0 &= ~BIT(0); //�ٴιر�SD������ֹռ��IIC_CLK
#endif
    iic_busy_flag = 1;
    iic_stop();
#endif
}

#pragma location="FM_SEG"
void iic_reuse_end(void)
{
#if IICCLK_REUSE_USB && !USB_GPIO_EN
    USB_DPDM_GPIO_DIS();
#endif

#if ICC_REUSE_EN
    iic_busy_flag = 0;
#endif
}


#if !FM_VOUT_THROUGH_MCU
#pragma location="FM_SEG"
void fm_set_vol(u8 volume)
{
    iic_reuse_init();

#if(FM_TYPE == FM_RDA5807)
    fmrda5807_set_vol(volume);
#elif(FM_TYPE == FM_QN8035)
    qn8035_set_vol(volume);
#elif(FM_TYPE == FM_QN8065)
    qn8065_set_vol(volume);
#elif(FM_TYPE == FM_AX5111)
    ax5111_set_vol(volume);
#elif(FM_TYPE == FM_BK1080)
    //bk1080_unmute();
#elif(FM_TYPE == FM_RTC6218)
    RTC6218_set_vol(volume);
#elif(FM_TYPE == FM_RTC6207)
    RTC6207_set_vol(volume);
#endif

    iic_reuse_end();
}
#endif

#pragma location="FM_SEG"
void fm_unmute(void)
{
    iic_reuse_init();

#if(FM_TYPE == FM_RDA5807)
    fmrda5807_set_vol(FM_VOL);
#elif(FM_TYPE == FM_QN8035)
    qn8035_unmute();
#elif(FM_TYPE == FM_QN8065)
    QN8065_SetMute(0);
#elif(FM_TYPE == FM_AX5111)
    ax5111_set_vol(FM_VOL);
#elif(FM_TYPE == FM_BK1080)
    bk1080_unmute();
#elif(FM_TYPE == FM_RTC6218)
    RTC6218_set_vol(FM_VOL);
#elif(FM_TYPE == FM_RTC6207)
    RTC6207_set_vol(FM_VOL);
#endif

    iic_reuse_end();
}


//����Ƶ�ʣ����ж��Ƿ�Ϊ��̨
#pragma location="FM_SEG"
void fm_set_freq(u16 freq)
{
    iic_reuse_init();

#if(FM_TYPE == FM_RDA5807)
    rda5807_set_freq(freq);
   // fmrda5807_set_vol(15);
#elif(FM_TYPE == FM_QN8035)
    qn8035_set_freq(freq);
#elif(FM_TYPE == FM_QN8065)
    QN8065_TuneToCH(freq);
#elif(FM_TYPE == FM_AX5111)
    ax5111_set_freq(freq/10);
#elif(FM_TYPE == FM_BK1080)
    bk1080_set_freq(freq);
   // bk1080_unmute();
#elif(FM_TYPE == FM_RTC6218)
    RTC6218_set_freq(freq);
#elif(FM_TYPE == FM_RTC6207)
    RTC6207_set_freq(freq);
#endif

    iic_reuse_end();
    fm_unmute();
}

//����Ƶ��,��Ϊ��̨������ture
#pragma location="FM_SEG"
u8 fm_check_freq(u16 freq)
{
    iic_reuse_init();

    u8 ret;
#if(FM_TYPE == FM_RDA5807)
    ret = fm_rda5807_set_freq(freq);
#elif(FM_TYPE == FM_QN8035)
    ret = fm_qn8035_set_freq(freq);
#elif(FM_TYPE == FM_QN8065)
    ret = qn8065_seek(freq);
#elif(FM_TYPE == FM_AX5111)
    ret = ax5111_seek(freq/10);
#elif(FM_TYPE == FM_BK1080)
    ret = fm_bk1080_set_freq(freq);
#elif(FM_TYPE == FM_RTC6218)
    ret = RTC6218_seek(freq);
#elif(FM_TYPE == FM_RTC6207)
    ret = RTC6207_seek(freq);
#else
    ret = 0;
#endif

    iic_reuse_end();
    return ret;
}

#pragma location="FM_INIT"
bool fm_init(void)
{
    iic_reuse_init();

    bool ret;
#if(FM_TYPE == FM_RDA5807)
    ret = fm_rda5807_init();                           //��ʼ��
#elif(FM_TYPE == FM_QN8035)
    ret = fm_qn8035_init();
#elif(FM_TYPE == FM_QN8065)
    ret = qn8065_online();
#elif(FM_TYPE == FM_AX5111)
    ret = ax5111_online();
#elif(FM_TYPE == FM_BK1080)
    ret = fm_bk1080_init();
#elif(FM_TYPE == FM_RTC6218)
    ret = RTC6218_online();
#elif(FM_TYPE == FM_RTC6207)
    ret = RTC6207_online();
#else
    ret = 0;
#endif

    iic_reuse_end();
    fm_online_flag = ret;
    return ret;
}

#pragma location="FM_INIT"
void fm_off(void)
{
    iic_reuse_init();

#if IICCLK_REUSE_USB
    USB_DP_OUTPUT();            //2014-4-15, ��������U������ʱ�˳�FMʱ����FM�ز���������
    USB_DP_HIHG();
#endif

#if(FM_TYPE == FM_RDA5807)
    fm_rda5807_off();
#elif(FM_TYPE == FM_QN8035)
    fm_qn8035_off();
#elif(FM_TYPE == FM_QN8065)
    qn8065_off();
#elif(FM_TYPE == FM_AX5111)
    ax5111_off();
#elif(FM_TYPE == FM_BK1080)
    fm_bk1080_off();
#elif(FM_TYPE == FM_RTC6218)
    RTC6218_off();
#elif(FM_TYPE == FM_RTC6207)
    RTC6207_off();
#endif

    iic_reuse_end();
}

#pragma location="FM_INIT"
void fm_io_init(void)
{
#if !IICCLK_REUSE_USB
    IIC_DATA_CLK_OUT();                 //IIC������Ϊ���
    IIC_DATA_HIGH();                    //IIC DATA����
#endif
}
