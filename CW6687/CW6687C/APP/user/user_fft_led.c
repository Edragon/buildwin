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


#define FFT_PCM_SETP   4      //4������ȡһ����������ͳ��
#define FFT_LED_NUM    4      //һ��5��LED��
u8 fft_pwm_buf[FFT_LED_NUM];  //pwm����ֵ
#define ENERGY_TABLE_ADJ   1   //1�����������  //0: ���Ե���

#if ENERGY_TABLE_ADJ
//����������Ե����Ƶ�����״̬������.
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
#pragma register_bank=1      //0���ȼ�(������ȼ�)������bank1 //2���ȼ�������bank2 //3���ȼ�������bank3
__near_func __interrupt void timer3_isr(void)
{
    static u8 timer3_cnt = 0;
    ISR_ENTER();
    TMR3CON &= ~BIT(7);             //clear pending
    timer3_cnt++;
    if(timer3_cnt%2 == 0){  //1ms�ж�
        //fft_led_scan();
    }
    if(timer3_cnt%30 == 0){  //15ms�ж�
        //fft_energy_get();
    }
    if(200 == timer3_cnt){  //100ms�ж�
        //uart_putchar(0x77);
        timer3_cnt = 0;
    }
    ISR_EXIT();
}
void timer3_init(void)
{
    TMR3CON = 0x17;    //timer3en, 128 div
    TMR3PR = 0xBB;     //0.5/(1/48000*128)-1 = 0xBA.8 = 0xBB.
    IP1 &= ~BIT(4);    //0����������ȼ���
    IPH1 &= ~BIT(4);

    IE1 |= BIT(4);     //timer3 int enable
}
#endif

//jump_byte: ��ҪΪż��
extern __near_func u32 get_total_energy(u8 *adr, u16 samples, u8 jump_byte);    //���У�ʹ����չָ���д
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

//ֻ��DAC�����ģʽ�Ż���FFT�ơ�
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

__near_func void fft_energy_get(void) //5ms����һ��
{
    u16 i;
    u8 level;
    u32 fft_total_energy = 0;
    //u16 __xdata *out_buf;
    static u8 cnt = 0;
    static u8 last_level = 0;
    cnt++;
    if((cnt%3 == 0) && fft_valid_task_check())  //15ms����һ�Ρ�
    {
        fft_total_energy = get_total_energy((u8 __xdata * )0x8000,2304/10,10)>>14;
        //uart_putchar(0xAA);uart_putchar(fft_total_energy);
        //�õ�����������
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
        //������תPWM���ֵ��fft_led_buf
        for(i=0; i<FFT_LED_NUM; i++)  //FFT_LED_NUM -- 4  //value[0,24]
        {
            if(level >= 8)  //����8��ȫ����.  //eg 23
            {
                fft_pwm_buf[i] = 0xFF;
                level -= 8;              //8--8,��ʾһ��LED��8������
            }
            else
            {
                fft_pwm_buf[i] = BIT(level) - 1;   //���BYTE�Ĵ���.
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

//fft_led_buf�У�Ϊ1��λ������Ϊ0��λϨ��
//fft_pwm_buf[0]=0x3F=11000000b����LED0ռ�ձ�Ϊ25%
//����õ�ÿ���Ƶ�ռ�ձȺ�,�ŵ� fft_pwm_buf[FFT_LED_NUM];
//��scan�����͸���fft_led_buf[FFT_LED_NUM]��ɨ��ÿһ������.
//�˺���1ms����һ��,8ms���ܰ�����״̬ɨ��һ��.
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

