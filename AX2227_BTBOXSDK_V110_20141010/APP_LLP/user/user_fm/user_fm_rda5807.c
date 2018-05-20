/*****************************************************************************
 * Module    : Task
 * File      : user_fm_rda5807.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : RDA 5807 驱动
 *****************************************************************************/
#include "include.h"
#include "user_fm.h"

#if (FM_TYPE == FM_RDA5807)

//RDA 5807 初始化表
#pragma constseg="FM_INIT_CONST_RDA5807"
IAR_CONST u8 tbl_rda5807p_init[104] =           //    原来的5807SP
{
    //#if defined(_SHARE_CRYSTAL_24MHz_)
    //    0xc4, 0x51, //02H:
    //#elif defined(_SHARE_CRYSTAL_12MHz_)
    //    0xc4, 0x11, //02H:
    //#elif defined(_SHARE_CRYSTAL_32KHz_)
    //    0xc4, 0x01, //02H:
    //#else
    0xC0, 0x01,
    //#endif

    //#if defined(_FM_STEP_50K_)
    //    0x1b, 0x92,
    //    0x0C, 0x00,
    //#else       //Step 100K
    0x00, 0x10, //87~108MHz
    //0x00,0x18,//76~108MHz
    0x04, 0x00,
    //#endif

    0x88, 0xad, //05H:
    0x80, 0x00,
    0x76, 0xc6, //07H
    0x5e, 0xc6,
    0x00, 0x00,
    0x40, 0x6e, //0AH:
    0x2d, 0x80,
    0x58, 0x03,
    0x58, 0x04,
    0x58, 0x04,
    0x58, 0x04,
    0x00, 0x47, //10H:
    0x90, 0x00,
    0xF5, 0x87,
    0x7F, 0x0B, //13H:
    0x04, 0xF1,
    0x5E, 0xc0, //15H: 0x42, 0xc0
    0x41, 0xe0,
    0x50, 0x6f,
    0x55, 0x92,
    0x00, 0x7d,
    0x10, 0xC0,//1AH
    0x07, 0x80,
    0x41, 0x1d,//1CH,
    0x40, 0x06,
    0x1f, 0x9B,
    0x4c, 0x2b,//1FH.
    0x81, 0x10, //20H:
    0x45, 0xa0,// 21H

    //#if defined(_FM_STEP_50K_)
    //    0x55, 0x3F, //22H
    //#else
    0x19, 0x3F, //22H
    //#endif

    0xaf, 0x40,
    0x06, 0x81,
    0x1b, 0x2a, //25H
    0x0D, 0x04,
    0x80, 0x9F, //0x80, 0x2F,
    0x17, 0x8A,
    0xD3, 0x49,
    0x11, 0x42,
    0xA0, 0xC4, //2BH
    0x3E, 0xBB,
    0x00, 0x00,
    0x58, 0x04,
    0x58, 0x04, //2FH
    0x58, 0x04,
    0x00, 0x74,
    0x3D, 0x00,
    0x03, 0x0C,
    0x2F, 0x68,
    0x38, 0x77, //35H
};

IAR_CONST u8 tbl_rda5807h_init[110] =      //更新后的5807，型号5807HP
{
    0xc0,0x01,
    0x00, 0x10,
    0x04, 0x00,
    0x88, 0xB0, //05h://86    如打开20H寄存器，可在此调搜台个数,与SP版一样，0X80搜台最多
    0x40,0x00,
    0x7E,0xc6,
                        //以下寄存器配置不可省去
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,  //0x0ah
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x00,
    0x00,0x06,  //0x10h
    0x00,0x19,// //0x00,0x09,//0830
    0x2a,0x11,
    0x00,0x2e,
    0x2a,0x30,
    0xb8,0x3c,  //0x15h
    0x90,0x00,
    0x2a,0x91,
    0x84,0x12,
    0x00,0xa8,
    0xc4,0x00,  //0x1ah
    0xe0,0x00,
    0x30,0x1d,//0x24,0x9d,1ch
    0x81,0x6a,
    0x46,0x08,
    0x00,0x86,  //0x1fh
    0x06,0x61,// 0x16,0x61, 20h  //0X16为打开第05H寄存器，可在05H调搜台数
    0x00,0x00,
    0x10,0x9e,
    0x25,0x4A,//0x24,0Xc9台多  //   0x23,0x46 //0x25,0x4a //0x25,0xCB  //0x26,0x4c台相对少,但清楚台更多，根据客户需求选择  //23h  搜台门限值调整
    0x04,0x08,//0830
    0x0c,0x16,  //0x06,0x08,//0830  //0x25h
    0xe1,0x05,
    0x3b,0x6c,
    0x2b,0xec,
    0x09,0x0f,
    0x34,0x14,  //0x2ah
    0x14,0x50,
    0x09,0x6d,
    0x2d,0x96,
    0x01,0xda,
    0x2a,0x7b,
    0x08,0x21,   //0x30h
    0x13,0xd5,
    0x48,0x91,
    0x00,0xbc,
    0x08,0x96,//0830  0x34h
    0x15,0x3c,
    0x0b,0x80,
    0x25,0xc7,
    0x00,0x00,
};

IAR_CONST u8 tbl_rda5807m_init[70] =    //更新后的5807，型号5807MP
{
  //  #if defined(_SHARE_CRYSTAL_24MHz_)
//    0xC4, 0x51, //02H:
//#elif defined(_SHARE_CRYSTAL_12MHz_)
//    0xC4, 0x11, //02H:
//#elif defined(_SHARE_CRYSTAL_32KHz_)
//    0xC4, 0x01, //02H:

    0xC0, 0x05,
//#endif
    0x00, 0x12,
    0x04, 0x00,
    0x86, 0xad, //05h
    0x60, 0x00,
    0x76, 0xc6,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,  //0x0ah
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,  //0x10h
    0x00, 0x19,
    0x2a, 0x11,
    0x80, 0x53,
    0x22, 0x11,
    0xf8, 0x30,  //0x15h
    0xc0, 0x00,
    0x2a, 0x91,
    0x94, 0x00,
    0x00, 0xa8,
    0xc4, 0x00,  //0x1ah
    0xe0, 0x00,
    0x3a, 0x14,  //0x1ch
    0x81, 0x6a,
    0x46, 0x08,
    0x00, 0x86,
    0x06, 0x61,  //0x20h
    0x00, 0x00,
    0x10, 0x9e,
    0x22, 0x44,
    0x04, 0x08,  //0x24h
};

#pragma constseg=default

//将读取RDA5807芯片的寄存器，从0xA开始。
#pragma location="FM_SEG_RDA5807"
void fm_rda5807_read(u8 len)
{
    u8 i;
    if (len == 0) {
        return;
    }
    //sys_ctl.iic_work_sta = 1;
    iic_start();
    iic_sendbyte(0x21);                 //连续写命令
    iic_getack();
    for (i=0; i!=len-1; i++) {
        fm_ctl.r_reg[i] = iic_getbyte();
        iic_sendack();
    }
    fm_ctl.r_reg[i] = iic_getbyte();
    iic_sendnack();
    iic_stop();
   // sys_ctl.iic_work_sta = 0;
}

#pragma location="FM_SEG_RDA5807"
void fm_rda5807_write_a(u8 *buf, u8 len)
{
    u8 i;
    if (len == 0) {
        return;
    }
    iic_start();
    iic_sendbyte(0x20);                 //连续写命令
    iic_getack();
    for (i=0; i!=len; i++) {
        iic_sendbyte(buf[i]);
        iic_getack();
    }
    iic_stop();
}

//将数据写入RDA5807芯片的寄存器中，从0x2开始
#pragma location="FM_SEG_RDA5807"
void fm_rda5807_write(u8 len)
{
    fm_rda5807_write_a(fm_ctl.w_reg, len);
}

#pragma location="FM_SEG_RDA5807"
void fmrda5807_set_vol(u8 vol)
{
    if(vol > 15)
        vol = 15;

    fm_ctl.w_reg[7] &=~0x0F;
    fm_ctl.w_reg[7] |= vol;

    fm_ctl.w_reg[3] &= ~BIT(4);  //03H.4=0,disable tune
    fm_rda5807_write(8);
}



//RDA5807 调台
#pragma location="FM_SEG_RDA5807"
void rda5807_set_freq(u16 freq)
{
    if (freq < 7600 || freq > 10800) {
        return;
    }
   // u8 try_cnt = 2;

    freq = (freq - 7600) / 10;
   // while (try_cnt--)
    //{
    fm_ctl.w_reg[0] |= BIT(6);
    fm_ctl.w_reg[2] = freq >> 2;
    fm_ctl.w_reg[3] = ((freq & 0x3) << 6) | BIT(4) | BIT(3);
    fm_rda5807_write(4);
    //}
    //delay_5ms(11);                      //wait 50ms
    delay_5ms(18);
}


//RDA5807 调台
#pragma location="FM_SEG_RDA5807"
bool fm_rda5807_set_freq(u16 freq)
{
    fmrda5807_set_vol(0);
    rda5807_set_freq(freq);
    do {
        fm_rda5807_read(4);
    } while (0 == (fm_ctl.r_reg[3] & BIT(7)));      //FM_READY
    if (fm_ctl.r_reg[2] & BIT(0)) {                 //FM_TRUE
        fmrda5807_set_vol(15);
        return true;
    }
    return false;
}
//将数据写入RDA5807芯片的寄存器中，从0x2开始
#pragma location="FM_INIT_RDA5807"
void fm_rda5807_write_tbl(void *tbl_init, u8 len)
{
    memcpy((void *)0x8000, (void *)tbl_init, len);
    fm_rda5807_write_a((u8 *)0x8000, len);
}

#pragma location="FM_INIT_RDA5807"
bool fm_rda5807chip_init(char *tbl_init, u8 tbl_size)
{
    memcpy(fm_ctl.w_reg, tbl_init, 8);

    fm_rda5807_write(2);
    delay_5ms(15);
    fm_rda5807_write_tbl(tbl_init, tbl_size);
    delay_5ms(15);

    fm_ctl.w_reg[7] &= ~0x0F;
    fm_ctl.w_reg[7] |= 15;

    fm_ctl.w_reg[3] &= ~BIT(4);  //03H.4=0,disable tune
    fm_rda5807_write(8);
    delay_5ms(11);

/*
    fm_ctl.w_reg[0] |= BIT(6);
    fm_ctl.w_reg[2] = 5 >> 2;
    fm_ctl.w_reg[3] = ((5 & 0x3) << 6) | BIT(4);
    fm_rda5807_write(4);
    delay_5ms(1);
*/
    return true;
}

#pragma location="FM_INIT_RDA5807"
u16 fm_rda5807_readid(void)
{
    fm_ctl.w_reg[0] = 0x00;             //复位模块
    fm_ctl.w_reg[1] = BIT(1);
    fm_rda5807_write(2);
    delay_5ms(11);
    fm_rda5807_read(10);                //读ID
    //printf("CHIP ID: %04x\n", LOAD_U16B(&fm_ctl.r_reg[8]));
    return LOAD_U16B(&fm_ctl.r_reg[8]);
}

#pragma location="FM_INIT_RDA5807"
bool fm_rda5807_init(void)
{
    u16 chip_id;

    for(u8 i=0; i<3; i++){      //IIC与其他引脚复用时 FM ID 改为读三次比较保险
        chip_id = fm_rda5807_readid();
        //printf("rid:%04x\n",chip_id);
        if(chip_id == 0x5804 || chip_id == 0x5801 || chip_id == 0x5808){
            break;
        }
    }

    if (chip_id == 0x5804) {
        return fm_rda5807chip_init((char *)tbl_rda5807p_init, sizeof(tbl_rda5807p_init));
    } else if (chip_id == 0x5801) {
        return fm_rda5807chip_init((char *)tbl_rda5807h_init, sizeof(tbl_rda5807h_init));
    } else if (chip_id == 0x5808) {
        return fm_rda5807chip_init((char *)tbl_rda5807m_init, sizeof(tbl_rda5807m_init));
    } else {
        return false;
    }
}

#pragma location="FM_INIT_RDA5807"
void fm_rda5807_off(void)
{
    fm_ctl.w_reg[1] &= ~BIT(0);
    fm_rda5807_write(2);
}

#endif
