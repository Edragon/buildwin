/*****************************************************************************
 * Module    : API
 * File      : api.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : APIϵͳ����
 *****************************************************************************/
#ifndef API_SYSTEM_H
#define API_SYSTEM_H

void load_comm(u16 addr);
bool load_code(u16 addr, u8 segnum);
__near_func void my_memcpy(void* dst, const void* src, u16 size);
__near_func void my_memset(void* dst, u8 c, u16 size);
__near_func u16 mem_cmp (const void* dst, const void* src, u8 cnt);

__near_func void delay_5ms(u8 cnt);        //5ms��ʱ
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
u16 rand(u16 total);                    //��ȡ[0, num-1]֮��������

void uart_enable(void);
void uart_disable(void);

__near_func s16 mul_s16_u8(void *inptr, u8 gain);

/*************************rtc�������**********************************/

void irtcc_write_rtc(u16 day,u8 hour,u8 minute,u8 second);
void make_time(u32 secs);//����ת����t_time
void set_rtc_time(void);//��t_timeд��RTC
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


//ϵͳʱ������
enum {
    SYS_1M,          //PLL12M��ϵͳʱ��Ϊ1M����ҪӦ���ڵ͹��Ļ�ٹػ�
    SYS_3M,          //PLL12M��ϵͳʱ��Ϊ3M����ҪӦ���ڵ͹��Ļ�ٹػ�
    SYS_6M,          //PLL12M��ϵͳʱ��Ϊ6M����ҪӦ���ڵ͹��Ļ�ٹػ�
    SYS_12M,        //PLL��ϵͳʱ�Ӿ�Ϊ12M����ҪӦ���ڷ�Ч����ص�Ӧ��
    SYS_24M,        //PLL��ϵͳʱ�Ӿ�Ϊ24M����ҪӦ������Ƶ����
    SYS_48M,        //PLL��ϵͳʱ�Ӿ�Ϊ48M����ҪӦ����ѹ���������ȱȽ�ռ��CPU��Ӧ��
    SYS_RC_3M,
};

void sys_pll_rc(void);              //ʹ���ڲ�RC��ΪPLLʱ��Դ
void sys_pll_x32k(void);            //ʹ��32K������ΪPLLʱ��Դ
void sys_pll_x12m(void);            //ʹ��12M������ΪPLLʱ��Դ
void set_sys_clk(u8 mode);          //����ϵͳʱ��ģʽ


void set_timeout(u16 cnt);          //��ʱ����
extern IAR_BIT_A volatile bool time_out_flag;                 //��ʱ��־

void dac_init(void);                //��ʼ��DAC��ʹ������������ֱ����
void dac_init_vcmbuf(void);         //��ʼ��DAC����ʡ����������ֱ���ݣ��ƶ���ʹ��
void dac_off(void);                 //�ر�DAC�����������ػ�ž��
void clr_dac_buf(void);             //���DAC���BUF

void dac_set_mono(void);            //����DAC���������(R+L)/2
void dac_enable(void);              //����DAC
void dac_disable(void);             //�ر�DAC

#endif
