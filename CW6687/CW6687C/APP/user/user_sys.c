/*****************************************************************************
 * Module    : User
 * File      : user_spi.c
 * Author    : Hanny
 * Function  : �û���SPI��������Ҫ���ڸ�����Ŵ������ȹ���
 *****************************************************************************/
#include "include.h"

extern void set_sys_clk_lib(u8 mode);

//SPIFlash�Ƿ�֧��4K Sector����
#pragma location="INIT_SEG"
bool spi_4k_support(void)
{
    return SPI_4K_SECTOR_SUPPORT;
}

#pragma location="INIT_SEG"
bool bt_phone_book_support(void)
{
    return BT_PHONE_BOOK_EN;
}

//ʹ�ö��ٸ�����ҳ�����֧��4������ҳ
#pragma location="INIT_SEG"
u8 spi_param_page(void)
{
    return PARAM_PAGE;
}

#pragma location="INIT_SEG"
__root void set_sys_clk_hook(u8 mode)  //
{
    //printf("clkhook%d\n",mode);
}

#pragma location="INIT_SEG"
void set_sys_clk(u8 mode)
{
    set_sys_clk_lib(mode);  //������ǰlib�е�clk���ú���
    switch (mode) {         //Ԥ���ⲿ�޸Ľӿ�
    case SYS_48M:
        break;

    case SYS_24M:
        break;

    case SYS_12M:
        break;

    case SYS_3M:
        break;

    case SYS_OSC_3M:
        break;

    case SYS_RC_4M:
        break;
    }
}

//LQFP48/LQFP64 pmu���ã�LDO mode��VOUT1V5��ѹ����1.35V
#pragma location="POWER_CTL_SEG"
void lqfp48_pmu_init(void)
{
#if !BUCK_MODE_EN
    //�Ӵ�RVDD����󣬸���GFSK���Ը��Ʋ���оƬͨ��Ч�������������
    PWRCON4 &= ~(BIT(4)|BIT(5));  //set RVDD voltage 00:1.15, 01:1.23, 10:1.28, 11:1.32
    PWRCON4 |= BIT(5);
    PWRCON4 |= BIT(4);

    PWRCON2 &= ~(BIT(3) | BIT(2) | BIT(1));
    PWRCON1 &= ~(BIT(5) | BIT(4) | BIT(3)); //set LDO mode voltage to 1.35v
    PWRCON1 |= BIT(5);
    PWRCON1 |= BIT(3);
#endif
}

//SSOP28 pmu���ã�LDO mode��VOUT1V5��ѹ����1.32V(���ܽ���1.28V)
#pragma location="POWER_CTL_SEG"
void ssop28_pmu_init(void)
{
#if !BUCK_MODE_EN
    irtcc_write_cfg(WRITE_PWR,0x00);
    PWRCON2 &= ~(BIT(3) | BIT(2) | BIT(1));
    PWRCON1 &= ~(BIT(5) | BIT(4) | BIT(3));  //vout:1.35
    PWRCON1 |=  BIT(5);
    PWRCON1 |=  BIT(4);
#endif
}


