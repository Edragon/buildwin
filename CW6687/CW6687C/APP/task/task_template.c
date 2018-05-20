/*****************************************************************************
 * Module    : Task
 * File      : task_template.c
 * Author    : Hanny
 * Function  : ����ģ�壬ֻ��һ����������
 *****************************************************************************/
#include "include.h"
#include "task.h"

//task_template controller
struct {
    u8 sub_sta;                         //��״̬
} t_temp;

//�����ʼ��
void task_template_enter(void)
{
    task_ctl.work_sta = TASK_TEMPLATE;
}

//�����˳�
void task_template_exit(void)
{
}

//����������
void task_template_event(void)
{
    WATCHDOG_CLR();

}

//������Ϣ����
void task_template_deal_msg(u8 msg)
{
    switch(msg) {
        //���š���ͣ
    case KU_PLAY:
        break;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

//������ʾ����
void task_template_display(void)
{

}

//����������
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

