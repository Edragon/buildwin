/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : API系统函数
 *****************************************************************************/
#ifndef API_SYSTEM_H
#define API_SYSTEM_H

void load_comm(u16 addr);
bool load_code(u16 addr, u8 segnum);
__near_func void my_memcpy(void* dst, const void* src, u16 size);
__near_func void my_memset(void* dst, u8 c, u16 size);
__near_func u16 mem_cmp (const void* dst, const void* src, u8 cnt);

__near_func void delay_5ms(u8 cnt);        //5ms延时
__near_func void delay(u16 cnt);
__near_func void msg_init(u8 *msg_pool, u8 depth);
__near_func void flush_msg(void);
__near_func void put_msg(u8 msg);
__near_func u8 get_msg(void);
__near_func char *string_tolow(char *str, u8 len);

__near_func void select_dcram(void);
__near_func void restore_memcon(void);

__near_func void irtcc_write(u8 x);
__near_func u8 irtcc_read(void);
u16 rand(u16 total);                    //获取[0, num-1]之间的随机数

void uart_enable(void);
void uart_disable(void);

__near_func s16 mul_s16_u8(void *inptr, u8 gain);

/*************************rtc相关配置**********************************/

void irtcc_write_rtc(u16 day,u8 hour,u8 minute,u8 second);
void make_time(u32 secs);//将秒转换成t_time
void set_rtc_time(void);//将t_time写进RTC
void set_alarm_time(u32 secs);
__near_func u32 get_time(u8 *rtc_buf);
void irtcc_write_alm(u8 hour,u8 minute,u8 second);
#define RTC_ALARM_EN()			irtcc_write_cfg(WK_EN_WCMD, BIT(0)|BIT(6))
#define RTC_ALARM_DIS()			irtcc_write_cfg(WK_EN_WCMD, 0)
#define RTC_SECOND_INT_EN()	    RTCON |= BIT(2) //second int enable
#define RTC_SECOND_INT_DIS()	RTCON &= ~BIT(2) //second int disable
#define RTC_ALARM_INT_EN()	    RTCON |= BIT(4) //alarm int enable
#define RTC_ALARM_INT_DIS()		RTCON &= ~BIT(4) //alarm int disable
#define RTC_ALARM_CLR_PEND()	RTCON &= ~BIT(5) //clear alarm pending


//系统时钟配置
enum {
    SYS_1M,          //PLL12M，系统时钟为1M，主要应用于低功耗或假关机
    SYS_3M,          //PLL12M，系统时钟为3M，主要应用于低功耗或假关机
    SYS_6M,          //PLL12M，系统时钟为6M，主要应用于低功耗或假关机
    SYS_12M,        //PLL与系统时钟均为12M，主要应用于非效率相关的应用
    SYS_24M,        //PLL与系统时钟均为24M，主要应用于音频播放
    SYS_48M,        //PLL与系统时钟均为48M，主要应用于压缩、复读等比较占用CPU的应用
    SYS_RC_3M,
};

void sys_pll_rc(void);              //使用内部RC作为PLL时钟源
void sys_pll_x32k(void);            //使用32K晶振作为PLL时钟源
void sys_pll_x12m(void);            //使用12M晶振作为PLL时钟源
void set_sys_clk(u8 mode);          //设置系统时钟模式


void set_timeout(u16 cnt);          //超时控制
extern IAR_BIT_A volatile bool time_out_flag;                 //超时标志

void dac_init(void);                //初始化DAC，使用左右声道隔直电容
void dac_init_vcmbuf(void);         //初始化DAC，节省左右声道隔直电容，推耳机使用
void dac_off(void);                 //关闭DAC，用于消除关机啪声
void clr_dac_buf(void);             //清除DAC输出BUF

void dac_set_mono(void);            //设置DAC单声道输出(R+L)/2
void dac_enable(void);              //开启DAC
void dac_disable(void);             //关闭DAC

#endif
