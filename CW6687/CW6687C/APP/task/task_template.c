/*****************************************************************************
 * Module    : Task
 * File      : task_template.c
 * Author    : Hanny
 * Function  : 任务模板，只是一个空闲任务
 *****************************************************************************/
#include "include.h"
#include "task.h"

//task_template controller
struct {
    u8 sub_sta;                         //子状态
} t_temp;

//任务初始化
void task_template_enter(void)
{
    task_ctl.work_sta = TASK_TEMPLATE;
}

//任务退出
void task_template_exit(void)
{
}

//任务事务处理
void task_template_event(void)
{
    WATCHDOG_CLR();

}

//任务消息处理
void task_template_deal_msg(u8 msg)
{
    switch(msg) {
        //播放、暂停
    case KU_PLAY:
        break;

        //调用公共消息函数
    default:
        deal_msg(msg);
        break;
    }
}

//任务显示处理
void task_template_display(void)
{

}

//任务主流程
void task_template(void)
{
    printf("task_template\n");
    task_template_enter();
    while (task_ctl.work_sta == TASK_TEMPLATE) {
        u8 msg = get_msg();
        task_template_event();
        task_template_deal_msg(msg);
        task_template_display();
    }
    task_template_exit();
}

