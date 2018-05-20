/*****************************************************************************
 * Module    : ledseg
 * File      : led_7p7s.c
 * Author    : Hanny
 * Function  : led_7p7s�������ʾ������
 *****************************************************************************/
#include "include.h"
#include "led_seg.h"

#if IS_LEDSEG_7PIN_DISPLAY
#include "led_7p7s.h"

__no_init u8 pin_disp_buf[7];
u8 led_seg_valid[7];
IAR_XDATA_A u8 COM_7P7S;

//��Чλ��1
IAR_CONST u8 tbl_ledseg_valid[7] = {
#if (THEME_SELECT == THEME_LEDSEG_7PIN)
    PIN1 | PIN2 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN2 | PIN3 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN3,
    PIN0 | PIN1 | PIN2 | PIN3,
    PIN0 | PIN1 | PIN2 | PIN3,
#else
    PIN1 | PIN2 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN2 | PIN3 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN3 | PIN4 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN4 | PIN5,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN5 | PIN6,
    PIN0 | PIN1 | PIN2 | PIN3 | PIN4 | PIN6,
    PIN2 | PIN4 | PIN5,
#endif
};

#include SET_USER_PATH(CFG_IO_KEY_DIR, /io/io_led_seg.c)

//led_7p7s��ʼ������
#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_init(void)
{
    led_7p7s_io_init();

    COM_7P7S = 0;

    my_memset((u8*)disp_buf,0x00,5);
    my_memset((u8 *)pin_disp_buf, 0x00, 7);
}

//����7��LED����ֵ�����ӳ�䣨ÿ������ߵ�IO��ΪCOM�� 7*7��COM:0~6  SEG:0~6
//#pragma location="DISP_LED_7P7S_SEG"
void led_7p7s_value_set(void)
{
    u8 ie0, ie1;
    bool ie_ea = IE_EA;
    IE_EA = 0;          //�����жϣ�������IE0/IE1���ٴ�
    ie0 = IE0;          //����IE0
    ie1 = IE1;          //����IE1
    IE0 = 0;            //�ص�IE0
    IE1 &= BIT(6);      //�ص�IE1������UART�ж�(��Ϊ��δ���ϳ�������һֱ��IE_EA���������������UART�ж�)
    IE_EA = ie_ea;      //�ָ����ж�

    my_memset((u8 *)pin_disp_buf, 0x00, 7);

/*********** SEG ****************** COM ******** SEG(out)  *******/
#if (THEME_SELECT == THEME_LEDSEG_7PIN)

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[1] |= PIN0; //��Ϊ1��ɨ��ʱ��Ӧ��SEGλ��Ϊ���(FM)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[2] |= PIN0;
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= PIN0;
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= PIN0;
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[5] |= PIN0;
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[6] |= PIN0;
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= PIN1;

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[0] |= PIN1;
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[0] |= PIN2;
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[0] |= PIN3;
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[0] |= PIN4;
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[0] |= PIN5;
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[0] |= PIN6;
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[1] |= PIN2;

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[3] |= PIN2;
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[4] |= PIN2;
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[5] |= PIN2;
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[6] |= PIN2;
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[4] |= PIN3;
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[5] |= PIN3;
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[6] |= PIN3;

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[2] |= PIN3;
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[2] |= PIN4;
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[2] |= PIN5;
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[2] |= PIN6;
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= PIN4;
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[3] |= PIN5;
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[3] |= PIN6;
    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[3] |= PIN1;
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[4] |= PIN1;
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= PIN1;
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[6] |= PIN1;
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[1] |= PIN3;
    if(disp_buf[4] & ICON_FM)       pin_disp_buf[1] |= PIN5;
    if(disp_buf[4] & ICON_MP3)      pin_disp_buf[1] |= PIN6;

#elif (THEME_SELECT == THEME_LEDSEG_7PIN_MHZ)

    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[0] |= PIN1; //��Ϊ1��ɨ��ʱ��Ӧ��SEGλ��Ϊ���(MHZ)
    if(disp_buf[0] & LED_BIT_B)     pin_disp_buf[0] |= PIN2;
    if(disp_buf[0] & LED_BIT_C)     pin_disp_buf[3] |= PIN0;
    if(disp_buf[0] & LED_BIT_D)     pin_disp_buf[4] |= PIN0;
    if(disp_buf[0] & LED_BIT_E)     pin_disp_buf[0] |= PIN3;
    if(disp_buf[0] & LED_BIT_F)     pin_disp_buf[1] |= PIN0;
    if(disp_buf[0] & LED_BIT_G)     pin_disp_buf[2] |= PIN0;

    if(disp_buf[1] & LED_BIT_A)     pin_disp_buf[1] |= PIN2;
    if(disp_buf[1] & LED_BIT_B)     pin_disp_buf[1] |= PIN3;
    if(disp_buf[1] & LED_BIT_C)     pin_disp_buf[4] |= PIN1;
    if(disp_buf[1] & LED_BIT_D)     pin_disp_buf[1] |= PIN5;
    if(disp_buf[1] & LED_BIT_E)     pin_disp_buf[1] |= PIN4;
    if(disp_buf[1] & LED_BIT_F)     pin_disp_buf[2] |= PIN1;
    if(disp_buf[1] & LED_BIT_G)     pin_disp_buf[3] |= PIN1;

    if(disp_buf[2] & LED_BIT_A)     pin_disp_buf[4] |= PIN3;
    if(disp_buf[2] & LED_BIT_B)     pin_disp_buf[2] |= PIN4;
    if(disp_buf[2] & LED_BIT_C)     pin_disp_buf[3] |= PIN4;
    if(disp_buf[2] & LED_BIT_D)     pin_disp_buf[5] |= PIN0;
    if(disp_buf[2] & LED_BIT_E)     pin_disp_buf[5] |= PIN2;
    if(disp_buf[2] & LED_BIT_F)     pin_disp_buf[3] |= PIN2;
    if(disp_buf[2] & LED_BIT_G)     pin_disp_buf[4] |= PIN2;

    if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[6] |= PIN5;
    if(disp_buf[3] & LED_BIT_B)     pin_disp_buf[5] |= PIN6;
    if(disp_buf[3] & LED_BIT_C)     pin_disp_buf[4] |= PIN5;
    if(disp_buf[3] & LED_BIT_D)     pin_disp_buf[5] |= PIN3;
    if(disp_buf[3] & LED_BIT_E)     pin_disp_buf[3] |= PIN5;
    if(disp_buf[3] & LED_BIT_F)     pin_disp_buf[5] |= PIN4;
    if(disp_buf[3] & LED_BIT_G)     pin_disp_buf[4] |= PIN6;

    if(disp_buf[4] & ICON_PLAY)     pin_disp_buf[0] |= PIN5;
    if(disp_buf[4] & ICON_PAUSE)    pin_disp_buf[2] |= PIN5;
    if(disp_buf[4] & ICON_USB)      pin_disp_buf[5] |= PIN1;
    if(disp_buf[4] & ICON_SD)       pin_disp_buf[0] |= PIN4;
    if(disp_buf[4] & ICON_DOT2)     pin_disp_buf[2] |= PIN3;
    if(disp_buf[4] & ICON_FM) {
        pin_disp_buf[2] |= PIN6;
        pin_disp_buf[6] |= PIN4;
    }
    if(disp_buf[4] & ICON_MP3)    pin_disp_buf[6] |= PIN2;
#endif

    IE0 = ie0;          //�Ȼָ�IE0����ʱ���IE_EA
    IE1 = ie1;          //�ٻָ�IE1
    IE_EA = ie_ea;      //�ָ����ж�
}

#pragma location="DISP_LEDSEG_SCAN"
__near_func bool led_7p7s_reuse(void)
{
    bool res = false;

#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    LINEIN_SEG_ENTER_DETECT();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    EAR_SEG_ENTER_DETECT();
#endif			
    delay(20);  //SYSCLK_24Mʱ,20ԼΪ20 uS
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    linein_detect();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    earphone_detect();
#endif
#if (LINEIN_DETECT_EN && LINEIN_SEG_REUSE)
    LINEIN_SEG_EXIT_DETECT();
#endif
#if (EAR_DETECT_EN && EAR_SEG_REUSE)
    EAR_SEG_EXIT_DETECT();
#endif		
#if ((LINEIN_DETECT_EN && LINEIN_SEG_REUSE) || (EAR_DETECT_EN && EAR_SEG_REUSE))
    res = true;
#endif	

    return res;
}

#endif
