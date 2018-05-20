/*****************************************************************************
 * Module    : Theme
 * File      : lcd_default_updatefw.c
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 升级任务显示主题
 *****************************************************************************/
#include "include.h"
#if (THEME_SELECT_UPDATEFW == LCD_THEME_DEFAULT)
#if 0
#include "task_update_fw.h"
#include "display_mem.h"

/*****************************************************************************
 * Module    : 屏幕显示控制
 *****************************************************************************/
#pragma location="TASK_UPDATE_SEG"
void task_updatefw_dispmem_init(void)
{
    dispmem_init((char *)0x9000, 2048);
    t_upd.num_buf = dispmem_addnum(RES_BIGNUM_0_BMP,10);
    t_upd.num_len = dispmem_ctl.num_len;
}

//显示一个Memory里面的图片
#pragma location="TASK_UPDATE_SEG"
u8 task_updatefw_dispmem_draw(void *ptr, u8 x, u8 line)
{
    return dispmem_draw(ptr, x, line);
}

//显示一个数字的演示程序
#pragma location="TASK_UPDATE_SEG"
void task_updatefw_dispmem_big_num(u8 num, u8 x, u8 line)
{
    task_updatefw_dispmem_draw(t_upd.num_buf + num * t_upd.num_len, x, line);
}

//显示大格式频率
#pragma location="TASK_UPDATE_SEG"
void task_updatefw_disp_percent(u8 x, u8 line)
{
    u8 percent = t_upd.disp_percent;
    u8 num0, num1, num2;

    num0 = percent / 100;
    num1 = (percent / 10) % 10;
    num2 = percent % 10;
    task_updatefw_dispmem_big_num(num0, x, line);
    task_updatefw_dispmem_big_num(num1, x + 15, line);
    task_updatefw_dispmem_big_num(num2, x + 30, line);
}

//更新
#pragma location="TASK_UPDATE_SEG"
void task_updatefw_update(void)
{
    u8 i;
    char *line_buf = lcd_buf;
    for (i=0; i!=8; i++) {
        dispmem_update(0, i, line_buf, 128);
        line_buf += 128;
    }
}

#endif
//进度显示
#pragma location="TASK_UPDATE_SEG"
void task_update_fw_progress_display(u8 percent)
{
/*    if(t_upd.disp_percent == percent) {
        return;
    }
    t_upd.disp_percent = percent;
    memset(lcd_buf, 0, 1024);
    task_updatefw_disp_percent(41, 3);
    task_updatefw_update();*/
}

#endif
