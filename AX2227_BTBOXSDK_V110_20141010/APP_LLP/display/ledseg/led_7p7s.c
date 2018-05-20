/*****************************************************************************
 * Module    : ledseg
 * File      : led_7p7s.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : led_7p7s�������ʾ������
 *****************************************************************************/
#include "include.h"
#include "led_seg.h"

#if THEME_SELECT == LEDSEG_THEME_DEFAULT && (LEDSEG_TYPE_SELECT == LEDSEG_7P7S_FM || LEDSEG_TYPE_SELECT == LEDSEG_7P7S_MHZ)

#include "led_7p7s.h"
extern __no_init u8 disp_buf[5];
__no_init u8 pin_disp_buf[7];
IAR_XDATA_A u8 COM_7P7S;

//led_7p7s��ʼ������
#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_init(void)
{
    P2DIR |= 0x78;  //�����õ������Ŷ���Ϊ����
    P3DIR |= 0x07;

    COM_7P7S = 0;
    my_memset((u8*)disp_buf,0x00,5);
    my_memset((u8 *)pin_disp_buf, 0x00, 7);
}

//����7��LED����ֵ�����ӳ��(ÿ������ߵ�IO��ΪCOM�� 7*7)COM:0~6  SEG:0~6
//#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_value_set(void)
{
    u8 ie0, ie1;
    bool ie_ea = IE_EA; //2014-3-5, ����С��2S�������ֻ�����ͣ�١���������
    IE_EA = 0;          //�����жϣ�������IE0/IE1���ٴ�
    ie0 = IE0;          //����IE0
    ie1 = IE1;          //����IE1
    IE0 = 0;            //�ص�IE0
    IE1 &= BIT(6);      //�ص�IE1������UART�ж�(��Ϊ��δ���ϳ�������һֱ��IE_EA���������������UART�ж�)
    IE_EA = ie_ea;      //�ָ����ж�

    my_memset((u8 *)pin_disp_buf, 0x00, 7);

/*********** SEG ****************** COM ******** SEG(out)  *******/
#if LEDSEG_TYPE_SELECT == LEDSEG_7P7S_FM

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[1] |= BIT(0); //��Ϊ1��ɨ��ʱ��Ӧ��SEGλ��Ϊ���(FM)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[2] |= BIT(0);
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= BIT(0);
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= BIT(0);
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[5] |= BIT(0);
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[6] |= BIT(0);
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= BIT(1);

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[0] |= BIT(1);
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[0] |= BIT(2);
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[0] |= BIT(3);
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[0] |= BIT(4);
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[0] |= BIT(5);
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[0] |= BIT(6);
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[1] |= BIT(2);

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[3] |= BIT(2);
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[4] |= BIT(2);
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[5] |= BIT(2);
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[6] |= BIT(2);
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[4] |= BIT(3);
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[5] |= BIT(3);
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[6] |= BIT(3);

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[2] |= BIT(3);
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[2] |= BIT(4);
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[2] |= BIT(5);
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[2] |= BIT(6);
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= BIT(4);
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[3] |= BIT(5);
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[3] |= BIT(6);

    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[3] |= BIT(1);
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[4] |= BIT(1);
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= BIT(1);
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[6] |= BIT(1);
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[1] |= BIT(3);
    if(disp_buf[4] & ICON_FM)       pin_disp_buf[1] |= BIT(5);
    if(disp_buf[4] & ICON_MP3)      pin_disp_buf[1] |= BIT(6);

#elif LEDSEG_TYPE_SELECT == LEDSEG_7P7S_MHZ

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[0] |= BIT(1); //��Ϊ1��ɨ��ʱ��Ӧ��SEGλ��Ϊ���(MHZ)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[0] |= BIT(2);
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= BIT(0);
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= BIT(0);
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[0] |= BIT(3);
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[1] |= BIT(0);
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= BIT(0);

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[1] |= BIT(2);
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[1] |= BIT(3);
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[4] |= BIT(1);
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[1] |= BIT(5);
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[1] |= BIT(4);
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[2] |= BIT(1);
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[3] |= BIT(1);

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[4] |= BIT(3);
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[2] |= BIT(4);
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[3] |= BIT(4);
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[5] |= BIT(0);
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[5] |= BIT(2);
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[3] |= BIT(2);
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[4] |= BIT(2);

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[6] |= BIT(5);
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[5] |= BIT(6);
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[4] |= BIT(5);
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[5] |= BIT(3);
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= BIT(5);
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[5] |= BIT(4);
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[4] |= BIT(6);

    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[0] |= BIT(5);
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[2] |= BIT(5);
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= BIT(1);
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[0] |= BIT(4);
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[2] |= BIT(3);
    if(disp_buf[4] & ICON_FM) {
        pin_disp_buf[2] |= BIT(6);
        pin_disp_buf[6] |= BIT(4);
    }
    if(disp_buf[4] & ICON_MP3)    pin_disp_buf[6] |= BIT(2);
#endif

    IE0 = ie0;          //�Ȼָ�IE0����ʱ���IE_EA
    IE1 = ie1;          //�ٻָ�IE1
    IE_EA = ie_ea;      //�ָ����ж�
}

#if 0   //0��7������SEGΪ��λ����ɨ��(����0.5ms�ж�����ɨ��)��  1��7������COMΪ��λɨ��
/*----------------------------------------------------------------------------*/
/***@brief 7����ɨ�躯��
    @details
    @param  ��
    @return ��
    @note����Ӧ�ε���������COM�ż�Ҫ������SEG����Ϊ�������COM������ߵ�ƽ��
           SEG������͵�ƽ������5��SEG����Ϊ����
*/
/*----------------------------------------------------------------------------*/
//LEDɨ�躯��(1COM 6 SEG ��COMΪ��λɨ��)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    u8 dis_seg = 0,dir_temp2,out_temp2,dir_temp3,out_temp3;

    if(COM_7P7S > 6) {
        COM_7P7S = 0;
    }
    dis_seg = pin_disp_buf[COM_7P7S];

    out_temp2 = P2 & 0x87;      //�����õ��Ľ�ȫ����0 (������û�õ���P2��P3��ԭ���ĵ�ƽ)
    out_temp3 = P3 & 0xf8;
    dir_temp2 = P2DIR | 0x78;   //�����õ��Ľ�ȫ����Ϊ���� (������û�õ���P2��P3��ԭ���ķ���)
    dir_temp3 = P3DIR | 0x07;

    dir_temp2 &= ~((dis_seg & 0x0f) << 3);  //��Ҫ��ʾ�Ķ���Ϊ��� (ǰ���Ѿ������н���0)
    dir_temp3 &= ~((dis_seg & 0x70) >> 4);

    if(COM_7P7S < 4) {          //��COM����Ϊ�������������ߵ�ƽ
        dir_temp2 &= ~BIT(COM_7P7S + 3);
        out_temp2 |= BIT(COM_7P7S + 3);
    } else {
        dir_temp3 &= ~BIT(COM_7P7S - 4);
        out_temp3 |= BIT(COM_7P7S - 4);
    }

    P3 = out_temp3;
    P2 = out_temp2;
    P3DIR = dir_temp3;
    P2DIR = dir_temp2;

    COM_7P7S++;     //ɨ����һCOM
}

#else

//LEDɨ�躯��(1COM 1 SEG ��SEGΪ��λɨ��)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    static u8 count = 0;
    u8 dis_seg = 0,dir_temp2,out_temp2,dir_temp3,out_temp3;

    if (count > 6) {
        count = 0;
        COM_7P7S++;
    }
    if(COM_7P7S > 6) {
        COM_7P7S = 0;
    }
    dis_seg = pin_disp_buf[COM_7P7S];
    if(count == COM_7P7S) {     //SEG��COMͬһIO���������,ɨ����һ��
        count += 1;
    }
    dis_seg = dis_seg & BIT(count);

    out_temp2 = P2 & 0x87;      //�����õ��Ľ�ȫ����0 (������û�õ���P2��P3��ԭ���ĵ�ƽ)
    out_temp3 = P3 & 0xf8;
    dir_temp2 = P2DIR | 0x78;   //�����õ��Ľ�ȫ����Ϊ���� (������û�õ���P2��P3��ԭ���ķ���)
    dir_temp3 = P3DIR | 0x07;

    if(COM_7P7S < 4) {          //��COM����Ϊ�������������ߵ�ƽ
        dir_temp2 &= ~BIT(COM_7P7S + 3);
        out_temp2 |= BIT(COM_7P7S + 3);
    } else {
        dir_temp3 &= ~BIT(COM_7P7S - 4);
        out_temp3 |= BIT(COM_7P7S - 4);
    }

    if(count < 4) {             //��Ҫ������SEG����Ϊ��� (ǰ���Ѿ������н���0)
        dir_temp2 &= ~(dis_seg << 3);
    } else {
        dir_temp3 &= ~(dis_seg >> 4);
    }

    P3 = out_temp3;
    P2 = out_temp2;
    P3DIR = dir_temp3;
    P2DIR = dir_temp2;

    count++;        //ɨ����һSEG
}
#endif

#endif
