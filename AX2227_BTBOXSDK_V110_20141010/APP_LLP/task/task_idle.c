/*****************************************************************************
 * Module    : Task
 * File      : task_idle.c
 * Author    : Hanny
 * Email     : coldney@163.com
 * Function  : 空闲任务
 *****************************************************************************/
#include "include.h"
#include "task.h"
#include "api_device.h"

#if DAC_EN
#pragma data_alignment=2
IAR_XDATA_A u8 spi_pcm_buf[288] @ "SPI_BUF";

#pragma data_alignment=32
IAR_CONST u8 period[576] = {
	0x00, 0x00, 0x00, 0x00, 0xCC, 0x02, 0x34, 0xFD, 0x97, 0x05, 0x69, 0xFA, 0x60, 0x08, 0xA1, 0xF7,
	0x24, 0x0B, 0xDC, 0xF4, 0xE2, 0x0D, 0x1E, 0xF2, 0x9B, 0x10, 0x66, 0xEF, 0x4A, 0x13, 0xB7, 0xEC,
	0xF1, 0x15, 0x0F, 0xEA, 0x8C, 0x18, 0x74, 0xE7, 0x1C, 0x1B, 0xE4, 0xE4, 0x9F, 0x1D, 0x62, 0xE2,
	0x14, 0x20, 0xED, 0xDF, 0x79, 0x22, 0x88, 0xDD, 0xCB, 0x24, 0x35, 0xDB, 0x0E, 0x27, 0xF3, 0xD8,
	0x3C, 0x29, 0xC4, 0xD6, 0x57, 0x2B, 0xA9, 0xD4, 0x5D, 0x2D, 0xA4, 0xD2, 0x4C, 0x2F, 0xB4, 0xD0,
	0x24, 0x31, 0xDC, 0xCE, 0xE5, 0x32, 0x1B, 0xCD, 0x8C, 0x34, 0x73, 0xCB, 0x1A, 0x36, 0xE5, 0xC9,
	0x8F, 0x37, 0x72, 0xC8, 0xE7, 0x38, 0x1A, 0xC7, 0x24, 0x3A, 0xDC, 0xC5, 0x44, 0x3B, 0xBC, 0xC4,
	0x48, 0x3C, 0xB8, 0xC3, 0x2E, 0x3D, 0xD2, 0xC2, 0xF7, 0x3D, 0x09, 0xC2, 0xA1, 0x3E, 0x5F, 0xC1,
	0x2D, 0x3F, 0xD3, 0xC0, 0x9A, 0x3F, 0x66, 0xC0, 0xE8, 0x3F, 0x18, 0xC0, 0x16, 0x40, 0xE9, 0xBF,
	0x26, 0x40, 0xD9, 0xBF, 0x17, 0x40, 0xE9, 0xBF, 0xE8, 0x3F, 0x17, 0xC0, 0x9A, 0x3F, 0x66, 0xC0,
	0x2E, 0x3F, 0xD3, 0xC0, 0xA1, 0x3E, 0x5F, 0xC1, 0xF7, 0x3D, 0x09, 0xC2, 0x2E, 0x3D, 0xD1, 0xC2,
	0x48, 0x3C, 0xB8, 0xC3, 0x44, 0x3B, 0xBC, 0xC4, 0x24, 0x3A, 0xDD, 0xC5, 0xE7, 0x38, 0x19, 0xC7,
	0x8E, 0x37, 0x72, 0xC8, 0x1B, 0x36, 0xE5, 0xC9, 0x8C, 0x34, 0x73, 0xCB, 0xE4, 0x32, 0x1C, 0xCD,
	0x24, 0x31, 0xDB, 0xCE, 0x4B, 0x2F, 0xB4, 0xD0, 0x5C, 0x2D, 0xA3, 0xD2, 0x56, 0x2B, 0xA9, 0xD4,
	0x3C, 0x29, 0xC4, 0xD6, 0x0D, 0x27, 0xF3, 0xD8, 0xCB, 0x24, 0x34, 0xDB, 0x78, 0x22, 0x88, 0xDD,
	0x14, 0x20, 0xEC, 0xDF, 0xA0, 0x1D, 0x61, 0xE2, 0x1D, 0x1B, 0xE4, 0xE4, 0x8D, 0x18, 0x74, 0xE7,
	0xF1, 0x15, 0x10, 0xEA, 0x4A, 0x13, 0xB6, 0xEC, 0x9A, 0x10, 0x66, 0xEF, 0xE3, 0x0D, 0x1D, 0xF2,
	0x24, 0x0B, 0xDC, 0xF4, 0x5F, 0x08, 0xA1, 0xF7, 0x98, 0x05, 0x69, 0xFA, 0xCD, 0x02, 0x33, 0xFD,
	0x00, 0x00, 0xFF, 0xFF, 0x34, 0xFD, 0xCD, 0x02, 0x69, 0xFA, 0x98, 0x05, 0xA1, 0xF7, 0x60, 0x08,
	0xDC, 0xF4, 0x24, 0x0B, 0x1D, 0xF2, 0xE3, 0x0D, 0x66, 0xEF, 0x9B, 0x10, 0xB6, 0xEC, 0x4A, 0x13,
	0x10, 0xEA, 0xF1, 0x15, 0x74, 0xE7, 0x8C, 0x18, 0xE3, 0xE4, 0x1D, 0x1B, 0x61, 0xE2, 0x9F, 0x1D,
	0xED, 0xDF, 0x13, 0x20, 0x88, 0xDD, 0x78, 0x22, 0x35, 0xDB, 0xCC, 0x24, 0xF4, 0xD8, 0x0D, 0x27,
	0xC4, 0xD6, 0x3C, 0x29, 0xA9, 0xD4, 0x57, 0x2B, 0xA4, 0xD2, 0x5C, 0x2D, 0xB5, 0xD0, 0x4C, 0x2F,
	0xDC, 0xCE, 0x24, 0x31, 0x1C, 0xCD, 0xE5, 0x32, 0x73, 0xCB, 0x8C, 0x34, 0xE5, 0xC9, 0x1B, 0x36,
	0x72, 0xC8, 0x8E, 0x37, 0x19, 0xC7, 0xE6, 0x38, 0xDC, 0xC5, 0x23, 0x3A, 0xBB, 0xC4, 0x44, 0x3B,
	0xB8, 0xC3, 0x48, 0x3C, 0xD2, 0xC2, 0x2E, 0x3D, 0x09, 0xC2, 0xF7, 0x3D, 0x5E, 0xC1, 0xA2, 0x3E,
	0xD3, 0xC0, 0x2C, 0x3F, 0x66, 0xC0, 0x9A, 0x3F, 0x18, 0xC0, 0xE8, 0x3F, 0xEA, 0xBF, 0x17, 0x40,
	0xDA, 0xBF, 0x27, 0x40, 0xE9, 0xBF, 0x17, 0x40, 0x18, 0xC0, 0xE7, 0x3F, 0x66, 0xC0, 0x9A, 0x3F,
	0xD3, 0xC0, 0x2D, 0x3F, 0x5F, 0xC1, 0xA2, 0x3E, 0x09, 0xC2, 0xF7, 0x3D, 0xD2, 0xC2, 0x2F, 0x3D,
	0xB8, 0xC3, 0x48, 0x3C, 0xBC, 0xC4, 0x44, 0x3B, 0xDC, 0xC5, 0x24, 0x3A, 0x19, 0xC7, 0xE8, 0x38,
	0x71, 0xC8, 0x8F, 0x37, 0xE6, 0xC9, 0x1B, 0x36, 0x74, 0xCB, 0x8D, 0x34, 0x1B, 0xCD, 0xE5, 0x32,
	0xDC, 0xCE, 0x24, 0x31, 0xB4, 0xD0, 0x4C, 0x2F, 0xA4, 0xD2, 0x5C, 0x2D, 0xA9, 0xD4, 0x56, 0x2B,
	0xC4, 0xD6, 0x3D, 0x29, 0xF2, 0xD8, 0x0D, 0x27, 0x34, 0xDB, 0xCB, 0x24, 0x88, 0xDD, 0x78, 0x22,
	0xED, 0xDF, 0x13, 0x20, 0x61, 0xE2, 0x9E, 0x1D, 0xE3, 0xE4, 0x1D, 0x1B, 0x74, 0xE7, 0x8C, 0x18,
	0x0F, 0xEA, 0xF1, 0x15, 0xB6, 0xEC, 0x4A, 0x13, 0x66, 0xEF, 0x9B, 0x10, 0x1D, 0xF2, 0xE3, 0x0D,
	0xDC, 0xF4, 0x23, 0x0B, 0xA0, 0xF7, 0x5F, 0x08, 0x69, 0xFA, 0x97, 0x05, 0x34, 0xFD, 0xCC, 0x02
};
#endif

//task_idle controller
#pragma location="TASK_IDLE_SEG"
void task_idle_display(void);

//任务初始化
#pragma location="TASK_IDLE_SEG"
void task_idle_enter(void)
{
    led_mode_off();
    led_idle();
#if LCD_THEME
    LCD_BACKLIGHT_OFF();
#endif
}

//任务退出
#pragma location="TASK_IDLE_SEG"
void task_idle_exit(void)
{
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#if LCD_THEME
    LCD_BACKLIGHT_ON();
#endif
#endif
}

//任务事件
#pragma location="TASK_IDLE_SEG"
void task_idle_event(void)
{
#if DAC_EN
#if 1
    u16 i;
    //printf("%02x\n", spi_get_status());
    if ((spi_get_status() & 0xff) & BIT(0)) {
        spi_get_pcm(spi_pcm_buf);
        //printf_array(spi_pcm_buf, 288);
        //printf("ok\n");
        for (i=0; i<288; i+=4) {
            while(!(AUCON7 & BIT(7)));
            AUCON5 = spi_pcm_buf[i+1];
            AUCON5 = spi_pcm_buf[i];
            while(!(AUCON7 & BIT(7)));
            AUCON5 = spi_pcm_buf[i+3];
            AUCON5 = spi_pcm_buf[i+2];
        }
    }
#else
    u16 i;
    for (i=0; i<576; ) {
        while(!(AUCON7 & BIT(7)));
        AUCON5 = period[i+1];
        AUCON5 = period[i];
        i+=2;
        while(!(AUCON7 & BIT(7)));
        AUCON5 = period[i+1];
        AUCON5 = period[i];
        i+=2;
    }
#endif
#endif
    WATCHDOG_CLR();
    if (device_is_online(DEVICE_UDISK)) {
        task_ctl.work_sta = TASK_USBBT;            //连接PC，进入USBBT任务
    }
    if (device_need_activate(DEVICE_PC)) {
        if (device_activate(DEVICE_PC)) {
            task_ctl.work_sta = TASK_USBDEV;
        }
    }
}

//任务主流程
#pragma location="TASK_IDLE_SEG"
void task_idle(void)
{
    printf("task_idle\n");
    task_idle_enter();
    while (task_ctl.work_sta == TASK_IDLE) {
        task_idle_event();
    }
    task_idle_exit();
}

