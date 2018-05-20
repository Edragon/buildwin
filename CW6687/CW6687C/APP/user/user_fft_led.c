#include "include.h"
#include "task.h"
#include "user_fft_led.h"
#if FFT_LED_EN
#define    FFT_LED_0_DIROUT()   P2DIR &= ~BIT(0)
#define    FFT_LED_1_DIROUT()   P2DIR &= ~BIT(1)
#define    FFT_LED_2_DIROUT()   P2DIR &= ~BIT(2)
#define    FFT_LED_3_DIROUT()   P2DIR &= ~BIT(3)

#define    FFT_LED_0_ON()   P2 |= BIT(0)
#define    FFT_LED_1_ON()   P2 |= BIT(1)
#define    FFT_LED_2_ON()   P2 |= BIT(2)
#define    FFT_LED_3_ON()   P2 |= BIT(3)

#define    FFT_LED_0_OFF()  P2 &= ~BIT(0)
#define    FFT_LED_1_OFF()  P2 &= ~BIT(1)
#define    FFT_LED_2_OFF()  P2 &= ~BIT(2)
#define    FFT_LED_3_OFF()  P2 &= ~BIT(3)

#define FFT_ALL_LED_DIROUT()  \
        FFT_LED_0_DIROUT();  \
        FFT_LED_1_DIROUT();  \
        FFT_LED_2_DIROUT();  \
        FFT_LED_3_DIROUT();

#define FFT_ALL_LED_ON()  \
        FFT_LED_0_ON();  \
        FFT_LED_1_ON();  \
        FFT_LED_2_ON();  \
        FFT_LED_3_ON();


#define FFT_ALL_LED_OFF()  \
        FFT_LED_0_OFF();  \
        FFT_LED_1_OFF();  \
        FFT_LED_2_OFF();  \
        FFT_LED_3_OFF();


#define FFT_PCM_SETP   4      //4个点中取一个点作能量统计
#define FFT_LED_NUM    4      //一共5个LED灯
u8 fft_pwm_buf[FFT_LED_NUM];  //pwm闪灯值
#define ENERGY_TABLE_ADJ   1   //1：能量表调节  //0: 线性调节

#if ENERGY_TABLE_ADJ
//此能量表可以调整灯的闪亮状态及亮度.
u8 const __code FFT_ENERGY_TBL[FFT_LED_NUM * 8] =
{
    0x10,0x12,0x14,0x16,0x18,0x24,0x2A,0x30,  //led 1
    0x32,0x35,0x38,0x3B,0x3E,0x41,0x43,0x45,  //led 2
    0x4A,0x50,0x58,0x5C,0x60,0x66,0x6A,0x70,  //led 2
    0x74,0x78,0x7C,0x80,0x84,0x88,0x8C,0x90,  //led 3
};
#endif

#if 0
#pragma vector = VECTOR_OFFSET(TIMER0_INT)
#pragma register_bank=1      //0优先级(最低优先级)必须用bank1 //2优先级必须用bank2 //3优先级必须用bank3
__near_func __interrupt void timer3_isr(void)
{
    static u8 timer3_cnt = 0;
    ISR_ENTER();
    TMR3CON &= ~BIT(7);             //clear pending
    timer3_cnt++;
    if(timer3_cnt%2 == 0){  //1ms中断
        //fft_led_scan();
    }
    if(timer3_cnt%30 == 0){  //15ms中断
        //fft_energy_get();
    }
    if(200 == timer3_cnt){  //100ms中断
        //uart_putchar(0x77);
        timer3_cnt = 0;
    }
    ISR_EXIT();
}
void timer3_init(void)
{
    TMR3CON = 0x17;    //timer3en, 128 div
    TMR3PR = 0xBB;     //0.5/(1/48000*128)-1 = 0xBA.8 = 0xBB.
    IP1 &= ~BIT(4);    //0级，最低优先级。
    IPH1 &= ~BIT(4);

    IE1 |= BIT(4);     //timer3 int enable
}
#endif

//jump_byte: 需要为偶数
extern __near_func u32 get_total_energy(u8 *adr, u16 samples, u8 jump_byte);    //库中，使用扩展指令编写
/*
unsigned long get_total_energy_vc(unsigned char *adr, int samples, int jump_byte)
{
    unsigned long fft_total_energy;
    unsigned short *ptr_pcm;
    int i;
    fft_total_energy = 0;
    ptr_pcm = (unsigned short*)adr;
    for (i = 0; i<samples; i++)
    {
        fft_total_energy +=  (*ptr_pcm & 0x8000)? (0x10000- *ptr_pcm): *ptr_pcm;
        adr += jump_byte;
        ptr_pcm = (unsigned short*)adr;
    }
    return fft_total_energy;
}
*/

//只有DAC输出的模式才会有FFT灯。
__near_func bool fft_valid_task_check(void)
{
    if(( (TASK_MUSIC == task_ctl.work_sta)
         || ((TASK_BT == task_ctl.work_sta)&&(!task_ctl.work_sub)))
          /*||(other task)*/)
    {
        return true;
    }else{
        return false;
    }
}

__near_func void fft_energy_get(void) //5ms调用一次
{
    u16 i;
    u8 level;
    u32 fft_total_energy = 0;
    //u16 __xdata *out_buf;
    static u8 cnt = 0;
    static u8 last_level = 0;
    cnt++;
    if((cnt%3 == 0) && fft_valid_task_check())  //15ms调用一次。
    {
        fft_total_energy = get_total_energy((u8 __xdata * )0x8000,2304/10,10)>>14;
        //uart_putchar(0xAA);uart_putchar(fft_total_energy);
        //得到能量表索引
#if ENERGY_TABLE_ADJ
        level = 0;
        for(i = 0; i < FFT_LED_NUM*8; i++)
        {
            if((u8)fft_total_energy < FFT_ENERGY_TBL[i]){
                break;
            }
            level++;
        }
#else
        level = 0;
        for(i = 0; i < FFT_LED_NUM*8; i++)
        {
            if((u8)fft_total_energy < i*4){
                break;
            }
            level++;
        }
#endif
        if (level < last_level){
            level = --last_level;
        }
        else{
            last_level = level;
        }
        //uart_putchar(0xBB); uart_putchar(level);
        //能量表转PWM输出值到fft_led_buf
        for(i=0; i<FFT_LED_NUM; i++)  //FFT_LED_NUM -- 4  //value[0,24]
        {
            if(level >= 8)  //大于8则全亮了.  //eg 23
            {
                fft_pwm_buf[i] = 0xFF;
                level -= 8;              //8--8,表示一个LED有8级亮度
            }
            else
            {
                fft_pwm_buf[i] = BIT(level) - 1;   //最后BYTE的处理.
                level = 0;
            }
        }
    }
}

void fft_led_init(void)
{
    FFT_ALL_LED_DIROUT();
    FFT_ALL_LED_ON();
}

//fft_led_buf中，为1的位点亮，为0的位熄灭
//fft_pwm_buf[0]=0x3F=11000000b，则LED0占空比为25%
//上面得到每个灯的占空比后,放到 fft_pwm_buf[FFT_LED_NUM];
//此scan函数就根据fft_led_buf[FFT_LED_NUM]来扫描每一个灯了.
//此函数1ms调用一次,8ms就能把所有状态扫描一扁.
__near_func void fft_led_scan(void)
{
    static u8 scan_ofs = 0;
    if(!fft_valid_task_check()){
        return;
    }
    //FFT_ALL_LED_DIROUT();
    FFT_ALL_LED_OFF();
    if(BIT(scan_ofs) & fft_pwm_buf[3]){
        FFT_ALL_LED_ON();
    }
    else if((BIT(scan_ofs) & fft_pwm_buf[2]))
    {
        FFT_LED_0_ON();
        FFT_LED_1_ON();
        FFT_LED_2_ON();
    }
    else if((BIT(scan_ofs) & fft_pwm_buf[1]))
    {
        FFT_LED_0_ON();
        FFT_LED_1_ON();
    }
    else if((BIT(scan_ofs) & fft_pwm_buf[0]))
    {
        FFT_LED_0_ON();
    }
    if(++scan_ofs >= 8){  //8= 8
        scan_ofs = 0;
    }
}

#endif

