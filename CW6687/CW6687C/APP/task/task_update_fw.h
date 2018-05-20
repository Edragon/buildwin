/*****************************************************************************
 * Module    : Task
 * File      : task_update_fw.h
 * Author    : Hanny
 * Function  : �����������������ض���
 *****************************************************************************/
#ifndef TASK_UPDATE_FW_H
#define TASK_UPDATE_FW_H

typedef struct {
    u8 disp_percent;                         //��״̬
    char *num_buf;
    u8 num_len;
    u16 file_len;
} type_update_task;

extern type_update_task t_upd;

void task_updatefw_dispmem_init(void);
void task_update_fw_progress_display(u8 percent);
#endif
