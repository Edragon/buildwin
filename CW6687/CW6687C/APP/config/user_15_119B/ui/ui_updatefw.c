/*****************************************************************************
* Module    : UI
* File      : ui_updatefw.c
* Author    :
* Function  : 时钟用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

//注，离线模式下面不能切bank，否则会引起死机
#pragma location="TASK_UPDATE_SEG"
void task_updatefw_dispmem_init(void)
{

}
#if IS_LEDSEG_7PIN_DISPLAY
#pragma location="TASK_UPDATE_SEG"
void led_7p7s_value_fw_set(void)
{
#if 1
    bool ie_tm1 = IE_TM1;
    IE_TM1 = 0;

/*********** SEG ****************** COM ******** SEG(out)  *******/
#if (THEME_SELECT == THEME_LEDSEG_7PIN)
    my_memset((u8 *)pin_disp_buf, 0x00, 7);
    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[1] |= PIN0; //设为1，扫描时对应此SEG位设为输出(FM)
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

    /*if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[2] |= PIN3;
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
    if(disp_buf[4] & ICON_MP3)      pin_disp_buf[1] |= PIN6;  */

#elif (THEME_SELECT == THEME_LEDSEG_7PIN_MHZ)
    my_memset((u8 *)pin_disp_buf, 0x00, 7);
    if(disp_buf[0] & LED_BIT_A)     pin_disp_buf[0] |= PIN1; //设为1，扫描时对应此SEG位设为输出(MHZ)
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

   /* if(disp_buf[3] & LED_BIT_A)     pin_disp_buf[6] |= PIN5;
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
    if(disp_buf[4] & ICON_MP3)    pin_disp_buf[6] |= PIN2;  */
#endif

    IE_TM1 = ie_tm1;
#endif
}

#pragma constseg="TASK_UPDATE_SEG_CONST"
IAR_CONST u8 tbl_ledseg_fw_num[10] =
{
    LEDSEG_0, LEDSEG_1, LEDSEG_2, LEDSEG_3, LEDSEG_4,
    LEDSEG_5, LEDSEG_6, LEDSEG_7, LEDSEG_8, LEDSEG_9,
};

//进度显示
#pragma location="TASK_UPDATE_SEG"
void task_update_fw_progress_display(u8 percent)
{
    if(t_upd.disp_percent == percent) {
        return;
    }
    t_upd.disp_percent = percent;
    memset((u8*)disp_buf,0x00,5);
    disp_buf[0] = tbl_ledseg_fw_num[t_upd.disp_percent / 100];
    disp_buf[1] = tbl_ledseg_fw_num[(t_upd.disp_percent / 10) % 10];
    disp_buf[2] = tbl_ledseg_fw_num[t_upd.disp_percent % 10];
    led_7p7s_value_fw_set();
   // ledseg_disp(MENU_UPDATE_FW);
}
#else
//进度显示
#pragma location="TASK_UPDATE_SEG"
void task_update_fw_progress_display(u8 percent)
{
}
#endif
#endif
