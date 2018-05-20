/*****************************************************************************
 * Module    : Task
 * File      : task_update_fw.c
 * Author    : Hanny
 * Function  : 离线升级程序
 *****************************************************************************/
#include "include.h"

#if TASK_UPDATE_FW_EN
#include "task.h"
#include "task_update_fw.h"
#include "user_spi.h"

#include SET_USER_PATH(CFG_UI_UPDATEFW_DIR, /ui/ui_updatefw.c)         //UI控制

IAR_XDATA_A type_update_task t_upd @ "UPDATE_XDATA";

//任务初始化
#pragma location="TASK_UPDATE_SEG_1"
bool task_update_fw_init(void)
{
    set_sys_clk(SYS_24M);
    t_upd.disp_percent = 0xff;

    task_ctl.work_sta = TASK_UPDATE_FW;
    task_updatefw_dispmem_init();
    if (!device_is_actived(fs_cur_dev())) {
        task_ctl.work_sta = TASK_EXIT;      //设备无效，返回主菜单
        msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);
        return false;
    }
    if (!fs_is_mounted()) {
        task_ctl.work_sta = TASK_EXIT;      //文件系统无效，返回主菜单
        msgbox(STR_INVALID_FAT, NULL, MSGBOX_TIME);
        return false;
    }
    task_update_fw_progress_display(1);
    if (!f_open_binfile()) {
        task_ctl.work_sta = TASK_EXIT;      //升级文件打开失败，返回主菜单
        msgbox(STR_UPDATEFILE_ERROR, NULL, MSGBOX_TIME);
        return false;
    }
    task_update_fw_progress_display(2);
    t_upd.file_len = GetFileLength();  //实际长度高2Byte
    if(!f_read_opt((void __xdata *)0x9000)){
        task_ctl.work_sta = TASK_EXIT;
        return false;
    }
    task_update_fw_progress_display(4);
    return true;
}

#pragma location="TASK_UPDATE_SEG"
bool task_setting_update_event(void)
{

    //解析和显示版本号
    u16 i,j;
    j=(t_upd.file_len/(30-5));
    //全部擦除预写块
    for(i=0; i<t_upd.file_len; i+=FW_SPI_ERASE_BLOCK)
    {
        spi_erase(FW_ERASE_CMD, (u32)i << 8);
        task_update_fw_progress_display(i/j + 5);
    }

    //从512Byte开始写,每次512B
    //j=(FileLen/(60-30));
    for(i=2; i<t_upd.file_len; i+=2)
    {
        if(!f_read_opt((void __xdata *)0x9200)){
            return false;
        }
       // spi_write((void __xdata *)0x8200, (u32)i << 8, 256);
        spi_write_page((void __xdata *)0x9200, i);             //256 per one time
        spi_write_page((void __xdata *)0x9300, i+1);
        task_update_fw_progress_display(i/j + 31);
    }
    //读校验,每次校验4KB
    j=(t_upd.file_len/(80-60));
    if(!f_seek_opt(F_SEEK_ORG,1)){
        return false;
    }
    for(i=2; i<t_upd.file_len; i+=2)
    {
        spi_read((void __xdata *)0x9200, (u32)(((u32)i)<<8), 512);
        if(!f_read_opt(((void __xdata *)(0x9400)))){
            return false;
        }

        if((0 != memcmp((void __xdata *)(0x9200), (void __xdata *)(0x9400), 0xc0)) ||
           (0 != memcmp((void __xdata *)(0x92C0), (void __xdata *)(0x94C0), 0xc0)) ||
           (0 != memcmp((void __xdata *)(0x9380), (void __xdata *)(0x9580), 0x80)) )
        {//校验失败
            return false;
        }
        task_update_fw_progress_display(i/j + 61);
    }
    //最后写和校验第一个512B
    //f_seek_opt(F_SEEK_ORG,0);
    //f_read_opt((void __xdata *)0x8000);
    spi_write_page((void __xdata *)0x9000, 0);             //256 per one time
    spi_write_page((void __xdata *)0x9100, 1);
    spi_read((void __xdata *)0x9200, 0, 512);
    //f_close();
    task_update_fw_progress_display(90);

    if((0 != memcmp((void __xdata *)(0x9000), (void __xdata *)(0x9200), 0xc0)) ||
       (0 != memcmp((void __xdata *)(0x90C0), (void __xdata *)(0x92C0), 0xc0)) ||
       (0 != memcmp((void __xdata *)(0x9180), (void __xdata *)(0x9380), 0x80)) )
    {//校验失败
        spi_erase(FW_ERASE_CMD,0);
        return false;
    }
    //升级成功
    task_update_fw_progress_display(100);
    return true;
}

//任务事务处理
#pragma location="TASK_UPDATE_SEG"
void task_update_fw_event(void)
{
    if(!task_setting_update_event()) {
        task_update_fw_progress_display(t_upd.disp_percent+100);
    }
}

//任务主流程
#pragma location="TASK_UPDATE_SEG"
void task_update_fw(void)
{
    //printf("task_update_fw\n");
    if(task_update_fw_init()) {
        task_update_fw_event();
        while(device_is_actived(fs_cur_dev())) {
            WATCHDOG_CLR();
        }
        WDTCON = 0x18;
        while (1);          //此任务不可返回
    }
}
#endif
