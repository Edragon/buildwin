/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Function  : API系统函数
 *****************************************************************************/
#ifndef API_SYSTEM_H
#define API_SYSTEM_H

//系统时钟配置
enum {
    SYS_48M = 0,        //PLL与系统时钟均为48M，主要应用于压缩、复读等比较占用CPU的应用
    SYS_24M,            //PLL与系统时钟均为24M，主要应用于音频播放
    SYS_12M,            //PLL与系统时钟均为12M，主要应用于非效率相关的应用

    SYS_3M,             //PLL12M，系统时钟为3M，主要应用于低功耗或假关机
    SYS_OSC_3M,         //关闭PLL，关闭RC，系统时钟为26MHz晶振，内核8分频，主要应用于低功耗或假关机
    SYS_RC_4M,          //关闭PLL，系统时钟为RC4M，主要应用于低功耗或假关机
};

void sys_pll_rc(void);              //使用内部RC作为PLL时钟源
void sys_pll_x32k(void);            //使用32K晶振作为PLL时钟源
void sys_pll_x12m(void);            //使用12M晶振作为PLL时钟源
void sys_pll_x26m(void);            //使用26M晶振作为PLL时钟源（共用蓝牙晶振）
void set_sys_clk(u8 mode);          //设置系统时钟模式
u8 get_sys_clk(void);               //获取当前的系统时钟

//void load_comm(u16 addr);
bool load_code(u16 addr, u8 segnum);
__near_func void my_memcpy(void* dst, const void* src, u16 size);
__near_func void smemcpy(void *des, void *src, u8 len);
__near_func void smemcpy_c(void *dst, const void __code *src, u8 len);
__near_func void memcpy_c(void *dest, const void __code *src, u16 len);

__near_func u8 smemcmp(void *des, void *src, u8 cnt);
__near_func u8 smemcmp_c(void *dst, const void __code *src, u8 cnt);
__near_func u16 mem_cmp(const void* dst, const void* src, u8 cnt);

__near_func void my_memset(void* dst, u8 c, u16 size);
__near_func void smemset_i(void __idata *buf, u8 ch, u8 cnt);

__near_func void delay_5ms(u8 cnt);        //5ms延时
__near_func void delay_ms(u8 cnt); //1ms延时
__near_func void delay(u16 cnt);
__near_func void msg_init(u8 *msg_pool, u8 depth);
__near_func void flush_msg(void);
__near_func void put_msg(u8 msg);
__near_func u8 get_msg(void);
__near_func char *string_tolower(char *str, u8 len);
void flush_dispensable_msg(void);


u16 rand(u16 total);                    //获取[0, num-1]之间的随机数

u32 get_rand_key(void);                 //获取一个32BIT的非零随机数，用于蓝牙的MAC地址

void uart_enable(void);
void uart_disable(void);

__near_func s16 mul_s16_u8(void *inptr, u8 gain);

__near_func u8 key_deal(u8 key_val, u8 key_scan_times, u8 key_up_times, u8 key_long_times, u8 key_long_hold_times);

/*************************rtc相关配置**********************************/
__near_func void irtcc_write(u8 x);
__near_func u8 irtcc_read(void);
__near_func u32 irtcc_read_rtc(u8 cmd); //获取rtc计时器或闹钟计时的值，单位为秒
void irtcc_write_cfg(u8 cmd,u8 irtcc_cfg);

__near_func u32 irtcc_read_rtc(u8 cmd);//获取rtc计时器或闹钟计时的值，单位为秒
void irtcc_write_alm(u8 hour,u8 minute,u8 second);

void irtcc_write_cfg(u8 cmd,u8 irtcc_cfg);
u8 irtcc_read_cfg(u8 cmd);
u32 irtcc_read_alm(u8 *rtc_buf);

void irtcc_write_ram(void *buf, u8 addr, u8 len);
void irtcc_read_ram(void *buf, u8 addr, u8 len);
void irtcc_write_ram8(u8 addr, u8 val);
void irtcc_write_ram16(u8 addr, u16 val);
void irtcc_write_ram32(u8 addr, u32 val);
u8 irtcc_read_ram8(u8 addr);
u16 irtcc_read_ram16(u8 addr);
u32 irtcc_read_ram32(u8 addr);

void irtcc_write_rtc(u8 cmd,u32 second);
void make_time(u32 secs);//将秒转换成t_time
void set_rtc_time(void);//将t_time写进RTC
void set_alarm_time(u32 secs);
__near_func u32 get_time(u8 *rtc_buf);
void irtcc_write_alm(u8 hour,u8 minute,u8 second);
//#define RTC_ALARM_EN()			irtcc_write_cfg(WRITE_WKO, BIT(0)|BIT(6))
//#define RTC_ALARM_DIS()			irtcc_write_cfg(WRITE_WKO, 0)
#define RTC_SECOND_INT_EN()	    IRTCON |= BIT(2) //second int enable
#define RTC_SECOND_INT_DIS()	IRTCON &= ~BIT(2) //second int disable
#define RTC_ALARM_INT_EN()	    IRTCON |= BIT(4) //alarm int enable
#define RTC_ALARM_INT_DIS()		IRTCON &= ~BIT(4) //alarm int disable
#define RTC_ALARM_CLR_PEND()	IRTCON &= ~BIT(5) //clear alarm pending

void set_timeout(u16 cnt);          //超时控制
extern IAR_BIT_A volatile bool time_out_flag;                 //超时标志


typedef struct {
    u8 year;
    u8 month;
    u8 day;
    u8 hour;
    u8 minute;
    u8 second;
    u8 wday;            //week_day
} TYPE_T_TIME;

extern TYPE_T_TIME t_time;
extern u8 timer5ms_cnt, timer100ms_cnt, timer1s_cnt;

void buck_init(void);
void xosc32k_select(void);//使能外部晶振32k
void power_down_enter(void);
void timer_init(void);

#endif
