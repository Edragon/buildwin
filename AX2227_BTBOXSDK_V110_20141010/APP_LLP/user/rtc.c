#include "include.h"
#include "task.h"

#pragma location="RTC_SEG"
void irtcc_init(void)
{
    PCON2 &= ~BIT(3);
#if IRTCC_WAKEUP_EN
    u8 irtcc_cfg = irtcc_read_cfg(WK_STA_RCMD);
    irtcc_write_cfg(WK_EN_WCMD,0x41);       //BIT(6) wko pin output enable  BIT(1) WKO pin wakeup enable
    #if SYS_CRY == CRY_32K
        irtcc_write_cfg(RTC_CFG_WCMD,0x98);
    #elif SYS_CRY == CRY_12M
        irtcc_write_cfg(RTC_CFG_WCMD,0x58); //32K:0x98  12M:0x58
        //irtcc_write_cfg(OSC_DRV_WCMD,0x51);  //12M
    #endif
#else
    u8 irtcc_cfg = irtcc_read_cfg(WK_STA_RCMD);
    #if SYS_CRY == CRY_32K
        irtcc_write_cfg(RTC_CFG_WCMD,0x88);
    #elif SYS_CRY == CRY_12M
        irtcc_write_cfg(RTC_CFG_WCMD,0x48); //32K:0x88  12M:0x48
        //irtcc_write_cfg(OSC_DRV_WCMD,0x51);  //12M
    #endif

    #if SOFT_POWER_ON_OFF
        SOFT_POWER_CTL_ON();    //使用IRTWKO引脚输出高电平来维持软开机状态时需要重新配置一下(上面配置RTC清除了IRTWKO输出高)
    #endif
    irtcc_write_cfg(WK_EN_WCMD,0x01);
#endif
    // 判断是否是首次上电RTC
    if(irtcc_cfg & BIT(5)) {
        memset(&t_time, 0, sizeof(t_time));
        t_time.year = 12;
        t_time.hour = 8;
        set_rtc_time();                     //初始化为2012年1月1日8:00
        irtcc_write_alm(0,0,0);
    }
#if IRTCC_WAKEUP_EN
	RTC_ALARM_CLR_PEND();		            //clear alarm pending
    //RTC_ALARM_INT_EN();			          //alarm int enable
#endif
    //IE1 |= 0x40;*/
    //rtccmd_write_nbyte(RTC_CNT_WCMD, tx_buf ,5);
}

//写入当前闹钟时钟
#pragma location="RTC_SEG"
void irtcc_write_alm(u8 hour,u8 minute,u8 second)
{
    RTCON |= 0x01;
    irtcc_write(RTC_ALM_WCMD);
    hour = ((hour/10)<<4) | (hour%10);
    minute = ((minute/10)<<4) | (minute%10);
    second = ((second/10)<<4) | (second%10);
    irtcc_write(hour);
    irtcc_write(minute);
    irtcc_write(second);
    RTCON &= ~(0x01);
}



//读取当前闹钟时钟
#pragma location="RTC_SEG"
u32 irtcc_read_alm(u8 *rtc_buf)
{
    u32 count;
    RTCON |= 0x01;
    irtcc_write(RTC_ALM_RCMD);
    rtc_buf[0] = irtcc_read();
    rtc_buf[1] = irtcc_read();
    rtc_buf[2] = irtcc_read();
    rtc_buf[0] = (rtc_buf[0]>>4)*10 + (rtc_buf[0]&0x0F);
    rtc_buf[1] = (rtc_buf[1]>>4)*10 + (rtc_buf[1]&0x0F);
    rtc_buf[2] = (rtc_buf[2]>>4)*10 + (rtc_buf[2]&0x0F);
    count = (u32)rtc_buf[0] << 16;
    count |= (u32)rtc_buf[1] << 8;
    count |= rtc_buf[2];
    RTCON &= ~(0x01);
    return count;
}

//读取RTC寄存器
#pragma location="RTC_SEG"
u8 irtcc_read_cfg(u8 cmd)
{
    u8 irtcc_cfg;
    RTCON |= 0x01;                  //irtcc communicate en
    irtcc_write(cmd);
    irtcc_cfg = irtcc_read();
    RTCON &= ~(0x01);
    return irtcc_cfg;
}
//设置RTC寄存器
#pragma location="RTC_SEG"
void irtcc_write_cfg(u8 cmd,u8 irtcc_cfg)
{
    RTCON |= 0x01;
    irtcc_write(cmd);
    irtcc_write(irtcc_cfg);
    RTCON &= ~(0x01);
}

#pragma location="RTC_SEG"
void irtcc_write_ram(void *buf, u8 addr, u8 len)
{
    u8 *rbuf = buf;
    RTCON |= 0x01;
    irtcc_write(RTC_RAM_WCMD);
    irtcc_write(addr);
    while (len--) {
        irtcc_write(*rbuf++);
    }
    RTCON &= ~(0x01);
}

#pragma location="RTC_SEG"
void irtcc_read_ram(void *buf, u8 addr, u8 len)
{
    u8 *rbuf = buf;
    RTCON |= 0x01;
    irtcc_write(RTC_RAM_RCMD);
    irtcc_write(addr);
    while (len--) {
        *rbuf++ = irtcc_read();
    }
    RTCON &= ~(0x01);
}

#pragma location="RTC_SEG"
void irtcc_write_ram8(u8 addr, u8 val)
{
    RTCON |= 0x01;
    irtcc_write(RTC_RAM_WCMD);
    irtcc_write(addr);
    irtcc_write(val);
    RTCON &= ~(0x01);
}

#pragma location="RTC_SEG"
void irtcc_write_ram16(u8 addr, u16 val)
{
    irtcc_write_ram(&val, addr, 2);
}

#pragma location="RTC_SEG"
void irtcc_write_ram32(u8 addr, u32 val)
{
    irtcc_write_ram(&val, addr, 4);
}

#pragma location="RTC_SEG"
u8 irtcc_read_ram8(u8 addr)
{
    u8 val;
    RTCON |= 0x01;
    irtcc_write(RTC_RAM_RCMD);
    irtcc_write(addr);
    val = irtcc_read();
    RTCON &= ~(0x01);
    return val;
}

#pragma location="RTC_SEG"
u16 irtcc_read_ram16(u8 addr)
{
    u16 val;
    irtcc_read_ram(&val, addr, 2);
    return val;
}

#pragma location="RTC_SEG"
u32 irtcc_read_ram32(u8 addr)
{
    u32 val;
    irtcc_read_ram(&val, addr, 4);
    return val;
}
