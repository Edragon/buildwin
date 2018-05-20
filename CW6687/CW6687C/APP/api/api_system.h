/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Function  : APIϵͳ����
 *****************************************************************************/
#ifndef API_SYSTEM_H
#define API_SYSTEM_H

//ϵͳʱ������
enum {
    SYS_48M = 0,        //PLL��ϵͳʱ�Ӿ�Ϊ48M����ҪӦ����ѹ���������ȱȽ�ռ��CPU��Ӧ��
    SYS_24M,            //PLL��ϵͳʱ�Ӿ�Ϊ24M����ҪӦ������Ƶ����
    SYS_12M,            //PLL��ϵͳʱ�Ӿ�Ϊ12M����ҪӦ���ڷ�Ч����ص�Ӧ��

    SYS_3M,             //PLL12M��ϵͳʱ��Ϊ3M����ҪӦ���ڵ͹��Ļ�ٹػ�
    SYS_OSC_3M,         //�ر�PLL���ر�RC��ϵͳʱ��Ϊ26MHz�����ں�8��Ƶ����ҪӦ���ڵ͹��Ļ�ٹػ�
    SYS_RC_4M,          //�ر�PLL��ϵͳʱ��ΪRC4M����ҪӦ���ڵ͹��Ļ�ٹػ�
};

void sys_pll_rc(void);              //ʹ���ڲ�RC��ΪPLLʱ��Դ
void sys_pll_x32k(void);            //ʹ��32K������ΪPLLʱ��Դ
void sys_pll_x12m(void);            //ʹ��12M������ΪPLLʱ��Դ
void sys_pll_x26m(void);            //ʹ��26M������ΪPLLʱ��Դ��������������
void set_sys_clk(u8 mode);          //����ϵͳʱ��ģʽ
u8 get_sys_clk(void);               //��ȡ��ǰ��ϵͳʱ��

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

__near_func void delay_5ms(u8 cnt);        //5ms��ʱ
__near_func void delay_ms(u8 cnt); //1ms��ʱ
__near_func void delay(u16 cnt);
__near_func void msg_init(u8 *msg_pool, u8 depth);
__near_func void flush_msg(void);
__near_func void put_msg(u8 msg);
__near_func u8 get_msg(void);
__near_func char *string_tolower(char *str, u8 len);
void flush_dispensable_msg(void);


u16 rand(u16 total);                    //��ȡ[0, num-1]֮��������

u32 get_rand_key(void);                 //��ȡһ��32BIT�ķ��������������������MAC��ַ

void uart_enable(void);
void uart_disable(void);

__near_func s16 mul_s16_u8(void *inptr, u8 gain);

__near_func u8 key_deal(u8 key_val, u8 key_scan_times, u8 key_up_times, u8 key_long_times, u8 key_long_hold_times);

/*************************rtc�������**********************************/
__near_func void irtcc_write(u8 x);
__near_func u8 irtcc_read(void);
__near_func u32 irtcc_read_rtc(u8 cmd); //��ȡrtc��ʱ�������Ӽ�ʱ��ֵ����λΪ��
void irtcc_write_cfg(u8 cmd,u8 irtcc_cfg);

__near_func u32 irtcc_read_rtc(u8 cmd);//��ȡrtc��ʱ�������Ӽ�ʱ��ֵ����λΪ��
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
void make_time(u32 secs);//����ת����t_time
void set_rtc_time(void);//��t_timeд��RTC
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

void set_timeout(u16 cnt);          //��ʱ����
extern IAR_BIT_A volatile bool time_out_flag;                 //��ʱ��־


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
void xosc32k_select(void);//ʹ���ⲿ����32k
void power_down_enter(void);
void timer_init(void);

#endif
