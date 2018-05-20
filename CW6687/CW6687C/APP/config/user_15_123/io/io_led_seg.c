/*****************************************************************************
 * Module    : IO
 * File      : io_led_seg.c
 * Author    : Hanny
 * Function  : LED�������ʾ������ز�������
 ******************************************************************************/
#ifndef CONFIG_C

#if IS_LEDSEG_7PIN_DISPLAY

#define LEDSEG_7PIN_SCAN_MODE   SCAN_MODE_SEG  //�߶�����ܵ�ɨ�跽ʽ IOȫ��������ѡ��SCAN_MODE_COM ����ѡ��SCAN_MODE_SEG

//LED 7����IO��ʼ�������������õ������Ŷ���Ϊ����
#pragma location="INIT_SEG"
void led_7p7s_io_init(void)
{
    P2DIR |= 0x6f;
    P0DIR |= BIT(6);
#if (LEDSEG_7PIN_SCAN_MODE == SCAN_MODE_COM)
    P2PDS1 |= 0x6f;
    P1PDS1 |= BIT(6);
#endif
}

#if (LEDSEG_7PIN_SCAN_MODE == SCAN_MODE_COM)

//LEDɨ�躯��(1COM 6 SEG ��COMΪ��λɨ��)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    u8 dis_seg = 0, dir_temp, out_temp, dir_temp2, out_temp2;
    u8 dir_dp = 0, dir_dp2 = 0;

    out_temp = P2 & 0x90;      //�����õ��Ľ�ȫ����0
    out_temp2 = P1 & 0xbf;
    dir_temp = P2DIR | 0x6f;   //�����õ��Ľ�ȫ����Ϊ����
    dir_temp2 = P1DIR | 0x40;

    if(COM_7P7S > 6) {
        COM_7P7S = 0;
        if (led_7p7s_reuse()) {
            return;
        }	
    }
    dis_seg = pin_disp_buf[COM_7P7S];

    dir_dp |= ((dis_seg & 0x0f) | ((dis_seg & 0x30)<<1));  //��Ҫ��ʾ�Ķο�����
    dir_dp2 |= (dis_seg & 0x40);         //P16

    if(COM_7P7S == 0) {
        dir_temp2 &= ~BIT(6);
        out_temp2 |= BIT(6);
    } else if(COM_7P7S < 3) {
        dir_temp &= ~BIT(7-COM_7P7S);
        out_temp |= BIT(7-COM_7P7S);
    } else {
        dir_temp &= ~BIT(6-COM_7P7S);
        out_temp |= BIT(6-COM_7P7S);
    }

    P2 = out_temp;
    P2DIR = dir_temp;
    P2PD = dir_dp;
    P1 = out_temp2;
    P1DIR = dir_temp2;
    P1PD = dir_dp2;

    COM_7P7S++;     //ɨ����һCOM
}
#else

//LEDɨ�躯��(1COM 1 SEG ��SEGΪ��λɨ��)
#pragma location="DISP_LEDSEG_SCAN"
__near_func void led_7p7s_scan(void)
{
    static u8 count = 0;
    u8 dis_seg = 0, dir_temp, out_temp, dir_temp2, out_temp2;

    out_temp = P2 & 0x90;      //�����õ��Ľ�ȫ����0
    out_temp2 = P0 & 0xbf;
    dir_temp = P2DIR | 0x6f;   //�����õ��Ľ�ȫ����Ϊ����
    dir_temp2 = P0DIR | 0x40;

    while (1) {
        if (count > 6) {
            count = 0;
            COM_7P7S++;
        }
        if(COM_7P7S > 6) {
            COM_7P7S = 0;
            if (led_7p7s_reuse()) {
                return;
            }
        }
        if (tbl_ledseg_valid[COM_7P7S] & BIT(count)) {         //��ԼCPU��ֻɨ����Ч��SEG��
            break;
        }
        count++;
    }
    dis_seg = pin_disp_buf[COM_7P7S] & BIT(count);

    dir_temp &= ~((dis_seg & 0x0f) | ((dis_seg & 0x30)<<1));  //��Ҫ��ʾ�Ķ���Ϊ��� (ǰ���Ѿ������н���0)
    dir_temp2 &= ~(dis_seg & 0x40);         //p16

    if(COM_7P7S == 0) {
        dir_temp2 &= ~BIT(6);
        out_temp2 |= BIT(6);
    } else if(COM_7P7S < 3) {
        dir_temp &= ~BIT(7-COM_7P7S);
        out_temp |= BIT(7-COM_7P7S);
    } else {
        dir_temp &= ~BIT(6-COM_7P7S);
        out_temp |= BIT(6-COM_7P7S);
    }

    P2 = out_temp;
    P2DIR = dir_temp;
    P0 = out_temp2;
    P0DIR = dir_temp2;

    count++;        //ɨ����һSEG
}
#endif
#endif

#endif


