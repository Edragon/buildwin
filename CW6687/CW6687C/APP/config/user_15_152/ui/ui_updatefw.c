/*****************************************************************************
* Module    : UI
* File      : ui_updatefw.c
* Author    :
* Function  : 时钟用户自定义UI设计，处理按键功能映射与显示
*****************************************************************************/
#ifndef CONFIG_C

#pragma location="TASK_UPDATE_SEG"
void task_updatefw_dispmem_init(void)
{

}
#if IS_LEDSEG_5C7S_DISPLAY
#pragma constseg="TASK_UPDATE_SEG_CONST"
IAR_CONST u8 tbl_ledseg_fw_num[10] =
{
    LEDSEG_0, LEDSEG_1, LEDSEG_2, LEDSEG_3, LEDSEG_4,
    LEDSEG_5, LEDSEG_6, LEDSEG_7, LEDSEG_8, LEDSEG_9,
};
#pragma location="TASK_UPDATE_SEG"
void menu_update_fw_ledseg(void)
{
    disp_buf[0] = tbl_ledseg_fw_num[t_upd.disp_percent / 100];
    disp_buf[1] = tbl_ledseg_fw_num[(t_upd.disp_percent / 10) % 10];
    disp_buf[2] = tbl_ledseg_fw_num[t_upd.disp_percent % 10];
    bool ie_ea = IE_EA;                   //2014-3-5, 修正小米2S及部分手机播歌停顿、无声问题
    IE_EA = 0;                            //2014-3-5, 短时间处理，直接关IE_EA
    smemcpy((u8*)scan_buf, disp_buf,5);
    IE_EA = ie_ea;
}
//进度显示
#pragma location="TASK_UPDATE_SEG"
void task_update_fw_progress_display(u8 percent)
{
    if(t_upd.disp_percent == percent) {
        return;
    }
    t_upd.disp_percent = percent;
    memset((u8*)disp_buf,0x00,5);
    menu_update_fw_ledseg();
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
