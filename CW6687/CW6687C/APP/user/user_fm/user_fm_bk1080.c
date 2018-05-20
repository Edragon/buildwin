/*--------------------------------------------------------------------------*/
/**@file BK1080.C
@brief FM模块QN8035驱动
@details FM模块QN8035的驱动函数
@author LZZ
@date 2010-08-17
@note 通过宏定义来选择驱动方式
*/
/*----------------------------------------------------------------------------*/

#include "include.h"
#include "user_fm.h"

#if (FM_TYPE == FM_BK1080)

/*
此项宏定义说明是使用何种FM驱动方式
USE_P33_AS_FM_CLOCK，指使用P33作为时钟信号
USE_32K_CRYSTAL_AS_FM_CLOCK，使用外部32K,作为驱动信号
USE_MODULE，使用模块,FM芯片自起振晶振属于这种模式
*/

#define USE_P33_AS_FM_CLOCK          1
#define USE_32K_CRYSTAL_AS_FM_CLOCK  2
#define USE_MODULE                   3 //KT 没有模块
#define _SHARE_CRYSTAL_24MHz_                   4 //共晶振
#define _SHARE_CRYSTAL_12MHz_                   5//KT 没有模块

#define FM_CLOCK _SHARE_CRYSTAL_12MHz_//32K

void bk1080_mute(void);
void bk1080_unmute(void);

#if (FM_CLOCK == USE_MODULE)
#define BK1080_REG2                 0x5201
#else
#define BK1080_REG2                 0x4201
#endif

#define BK1080_REG_MUTE             0x4000
#define BK1080_REG3                 0x0000
#define BK1080_REG_TUNE             0x8000
#define BK1080_REG_SEEK             0x0100
#define BK1080_REG_OFF              0x0041

#pragma constseg="FM_INIT_CONST_BK1080"
IAR_CONST u16 bk1080_init_tbl[34] =
{
    0x0006,	//REG0
    0x1080,	//REG1
    //#if (FM_CLOCK == USE_MODULE)
    //   0x5201,///Internal Clock  //模块  32.768K    //REG2
    //#else
    BK1080_REG2,	// External Clock   //芯片 32k GONG JING ZHEN    //REG2
    //#endif
    0x0000,	//REG3
    0x00C0,	//REG4
    0x0a1f,//0x0a1b,//0x0a1b,	//REG5
    0x002E,	//REG6//0x002E,
    0x02FF,	//REG7
    0x5B11,	//REG8
    0x0000,	//REG9
    0x411E,	//REG10
    0x0000,	//REG11
    0xCE00,	//REG12
    0x0000,	//REG13
    0x0000,	//REG14
    0x1000,	//REG15
#if(FM_CLOCK==_SHARE_CRYSTAL_24MHz_)
    0x2DC6,//REG16
    0x8000,//REG17
#elif(FM_CLOCK == _SHARE_CRYSTAL_12MHz_)
    0x16E3,//REG16
    0x8000,//REG17
#else
    0x0010,	//REG16
    0x0000,	//REG17
#endif
    0x13FF,	//REG18
    0x9852,	//REG19
    0x0000,	//REG20
    0x0000,	//REG21
    0x0008,	//REG22
    0x0000,	//REG23
    0x51E0,	//REG24
#if(FM_CLOCK==_SHARE_CRYSTAL_24MHz_)
    0xA8DC,//REG25
#elif (FM_CLOCK == _SHARE_CRYSTAL_12MHz_)
    0x28DC,//REG25
#else
    0x28BC,//0x28DC,	//REG25
#endif
    0x2645,	//REG26
    0x00E4,	//REG27
    0x1CD8,	//REG28
    0x3A50,	//REG29
    0xEAF0,	//REG30
    0x3000,	//REG31
    0x0200,//0x0000,	//REG32
    0x0000	//REG33
};
#pragma constseg=default


#pragma location="FM_SEG_BK1080"
void bk1080_Write_reg(u8 reg, u16 dat)
{
    iic_start();
    iic_sendbyte(0x80);
    iic_getack();
    iic_sendbyte(reg*2);
    iic_getack();
    iic_sendbyte(dat>>8);
    iic_getack();
    iic_sendbyte(dat);
    iic_getack();
    iic_stop();
}

#pragma location="FM_SEG_BK1080"
u16 bk1080_Read_reg(u16 reg)
{
    u16 temp;
    iic_start();
    iic_sendbyte(0x80);
    iic_getack();

    iic_sendbyte(reg*2+1);
    iic_getack();
    temp = iic_getbyte();
    temp <<= 8;
    iic_sendack();
    temp |= iic_getbyte();
    iic_sendnack();
    iic_stop();
    return temp;
}



#pragma location="FM_SEG_BK1080"
void fm_bk1080_write_a(u16 *buf, u8 len)
{
    u8 i;
    for (i=0; i!=len; i++) {
        bk1080_Write_reg(i, buf[i]);
    }
}

#pragma location="FM_INIT_BK1080"
void fm_bk1080_write_tbl(void *tbl_init, u8 len)
{
    memcpy((void *)0x8000, (void *)tbl_init, len<<1);
    fm_bk1080_write_a((u16 *)0x8000, len);
}

#pragma location="FM_INIT_BK1080"
void bk1080_init(u16 *tbl_init, u8 tbl_size)
{
    fm_bk1080_write_tbl(tbl_init, tbl_size);
    delay_5ms(20);
}



/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080设置频率
@param freq 要设置的频率
@return 无
@note 频率范围为875~1080
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_SEG_BK1080"
void bk1080_set_freq(u16 freq)
{
    freq = (freq - 8750) / 10;// freq = (freq - 8150) / 10;//freq = (freq - 7600) / 10;
    bk1080_Write_reg(0x02,BK1080_REG2 & (~BK1080_REG_SEEK));
    bk1080_Write_reg(0x03,BK1080_REG3 & (~BK1080_REG_TUNE));//BK1080_REG2
    bk1080_Write_reg(0x03, freq | BK1080_REG_TUNE);
    delay_5ms(10); //delay_5ms(10);                      //++ changed, SO @ 2010-08-26, really need 100ms ?
}



/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080关闭
@param 无
@return 无
@note
*/
/*----------------------------------------------------------------------------*/

#pragma location="FM_INIT_BK1080"
void fm_bk1080_off(void)
{
    bk1080_Write_reg(0x02,BK1080_REG2 | BK1080_REG_OFF);
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080自动搜台
@param freq 要搜索的频率
@return 指定的频率下有台返回1，否则返回0
@note
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_SEG_BK1080"
bool fm_bk1080_set_freq(u16 freq)
{
    static u16 g_freq_deviation_last=0;
   // static u16 last_fre=0;
    u16 cur_freq_deviation;
    u16 reg;
    bk1080_mute();
    //
    bk1080_set_freq(freq);
    delay_5ms(24);//24    // delay_5ms(16);                       //wait 55ms
    reg = bk1080_Read_reg(0x07);
    cur_freq_deviation = reg>>4;
    //bk1080_unmute();
    if( (reg & 0xf) <2)//2  //SNR 搜台较少可减小该值，假台较多可增大该值
    {
        g_freq_deviation_last=cur_freq_deviation;
        return 0;//false
    }
    reg = bk1080_Read_reg(0x0A);
    if(reg & 0x1000)    //check AFCRL bit12	
    {
        g_freq_deviation_last=cur_freq_deviation;
        return 0;
    }
    if((reg & 0x00FF)<10)//10    //RSSI 搜台较少可减小该值，假台较多可增大该值
    {
        g_freq_deviation_last=cur_freq_deviation;
        return 0;
    }
    if( (cur_freq_deviation>=250)&&(cur_freq_deviation<=(0xfff-250)))//250  搜台较少可减小该值，假台较多可增大该值
    {
        g_freq_deviation_last=cur_freq_deviation;
        return 0;
    }
    if(freq>=8760)//if( (freq>=8760)&&( (freq-last_fre)==10) )
    {
        if(g_freq_deviation_last&0x800)
        {
            g_freq_deviation_last=cur_freq_deviation;
            return 0;
        }
        if(g_freq_deviation_last<50)//50
        {
            g_freq_deviation_last=cur_freq_deviation;
            return 0;
        }
    }
    g_freq_deviation_last=cur_freq_deviation;
    bk1080_unmute();
    return 1;
}

#pragma location="FM_SEG_BK1080"
void bk1080_mute(void)
{
    bk1080_Write_reg(0x02, (BK1080_REG2 | BK1080_REG_MUTE));
}

#pragma location="FM_SEG_BK1080"
void bk1080_unmute(void)
{
    bk1080_Write_reg(0x02, (BK1080_REG2 & (~BK1080_REG_MUTE)));
}

/*----------------------------------------------------------------------------*/
/**@brief FM模块BK1080检测
@param 无
@return 检测到BK1080模块返回1，否则返回0
@note
*/
/*----------------------------------------------------------------------------*/
#pragma location="FM_INIT_BK1080"
bool fm_bk1080_init(void)
{
    u16 temp;
    for(u8 i=0; i<3; i++){      //IIC与其他引脚复用时 FM ID 改为读三次比较保险
        temp = bk1080_Read_reg(0x01);
        if(temp == 0x1080){
            bk1080_init((u16 *)bk1080_init_tbl, 34);
            return true;
        }
    }
    return false;
}

#endif
