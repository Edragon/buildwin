/*****************************************************************************
* Module    : Display
* File      : listboxa.c
* Author    : Hanny
* Function  : �ص��б�ؼ�����Ҫ�����ļ��������ֶ�̬�б��Ӧ��
*****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY
#include "task.h"
#include "listbox.h"

#pragma location="LIST_BOX_A_SEG"
void listboxa_event(void)
{
    if (!comm_event()) {
        t_list.ok_flag = 0xfe;
        return;
    }
    ocx_event();
    if (device_change()) {
        t_list.ok_flag = 0xfe;      //�豸�䶯���˳�
        task_ctl.work_sta = TASK_EXIT;
    }
}

#pragma location="LIST_BOX_SEG1"
void bt_standby_time_set(void)
{
    if(task_ctl.work_sta == TASK_BT){
        if(!BT_IS_SLEEP){
            sys_ctl.sleep_cnt = 0xffff;
        } else {
            sys_ctl.sleep_cnt = STANDBY_TIME;
        }
    }
}

//�б���һ��
#pragma location="LIST_BOX_SEG1"//LIST_BOX_A_SEG
void listboxa_prev(void)
{
    u8 page_change = 0;
    if (t_list.index <= t_list.page_index) {
        if (t_list.m_roll) {
            t_list.page_index--;
        } else {
            t_list.page_index -= t_list.n_page;
        }
        page_change = 1;
    }
    if (t_list.index == 0) {
        t_list.index = t_list.total;
        if (t_list.m_roll) {
            t_list.page_index = (t_list.total > t_list.n_page) ? t_list.total - t_list.n_page : 0;
        } else {
            t_list.page_index = ((t_list.total - 1) / t_list.n_page) * t_list.n_page;
        }
        page_change = 1;
    }
    t_list.index--;
    if (page_change) {
        t_list.item = (*t_list.get_list)(t_list.page_index, t_list.n_page);
    }
    bt_standby_time_set();
}

//�б���һ��
#pragma location="LIST_BOX_SEG1"//LIST_BOX_A_SEG
void listboxa_next(void)
{
    u16 num;
    u8 page_change = 0;
    t_list.index++;
    if (t_list.index - t_list.page_index >= t_list.n_page) {
        if (t_list.m_roll) {
            t_list.page_index++;
        } else {
            t_list.page_index += t_list.n_page;
        }
        page_change = 1;
    }
    if (t_list.index == t_list.total) {
        t_list.index = 0;
        t_list.page_index = 0;
        page_change = 1;
    }
    if (page_change) {
        num = t_list.total - t_list.page_index;
        if (t_list.total - t_list.page_index > t_list.n_page) {
            num = t_list.n_page;
        }
        t_list.item = (*t_list.get_list)(t_list.page_index, num);
    }
    bt_standby_time_set();
}

//�б�ؼ�������Ϣ
#pragma location="LIST_BOX_A_SEG"
void listboxa_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case KU_OK:
        t_list.ok_flag = 1;
        break;

        //ȡ��
#if THEME_SELECT==THEME_LCD_DEFAULT
    case KU_PLAY_POWER:
#else
    case KU_BACK:
#endif
        t_list.ok_flag = 0xff;
        break;

        //����
    case KL_MENU:
#if THEME_SELECT==THEME_LCD_DEFAULT
    case KL_PLAY_POWER:
#endif
        t_list.ok_flag = 0xfe;
        task_ctl.work_sta = TASK_EXIT;
        break;
#if !IS_LCD_DISPLAY
        //ɾ���ļ�
    case KU_FNAV_DEL:
    case KL_FNAV_DEL:
        t_list.ok_flag = 0x02;
        break;
#endif
        //��һ���һ��
    case KU_PREV:
        listboxa_prev();
        break;
    case KU_NEXT:
        listboxa_next();
        break;

        //���ù�����Ϣ����
    default:
        deal_msg(msg);
        break;
    }
}

#pragma location="LIST_BOX_A_SEG"
void listboxa_display_line(char *str, u8 line)
{
    u8 item_flag;
    u8 item_icon;
    u8 x = 18;
    if (t_list.m_icon) {
        item_flag = str[t_list.itemlen-1];
        if (item_flag & FB_GBK) {
#if SLIDER_EN
            disp_addslider_text(str, x+move_ctl.name_x, line,1);
#else
            disp_text(str, x+move_ctl.name_x, line,1);
#endif
        } else {
#if SLIDER_EN
            disp_addsleder_text_u(str, x+move_ctl.name_x, line,1);
#else
            disp_text_u(str, x+move_ctl.name_x, line,1);
#endif
        }

        switch (item_flag & FB_ICON_MASK) {
        case FB_FOLDERUP:
            item_icon = RES_ICON_FOLDER_UP_BMP;
            break;
        case FB_FOLDER:
            item_icon = RES_ICON_FOLDERC_BMP;
            break;
        case FB_FILE:
            item_icon = RES_ICON_FILE_BMP;
            break;
        case FB_MUSIC:
            item_icon = RES_ICON_FILE_MUSIC_BMP;
            break;
        default:
            item_icon = RES_ICON_ITEM_BMP;
            break;
        }
        disp_draw(item_icon, 0, line);
    }

}

//�б���ʾ�ӿ�
#pragma location="LIST_BOX_A_SEG"
void listboxa_display(void)
{
    u8 pos, line;
    u8 num;
    char *item = t_list.item;
    line_clear();
    if (t_list.index == t_list.disp_index) {
        if(move_ctl.ms_flag)
        {
            move_ctl.ms_flag=0;
            if(move_ctl.move_flag)
            {
#if SLIDER_EN
                if(move_ctl.name_x+(s16)move_ctl.name_wide>93)
#else
                    if(move_ctl.name_x+(s16)move_ctl.name_wide>100)
#endif
                    {

                        if(move_ctl.name_x==0)
                        {
                            move_ctl.wait++;
                            if(move_ctl.wait==11)//����ʾ�ļ���β��ͣ��һ��ʱ��
                            {
                                move_ctl.wait=0;
                                move_ctl.name_x--;
                            }
                        }
                        else
                            move_ctl.name_x--;
                    }
                    else
                    {
                        move_ctl.wait++;
                        if(move_ctl.wait==11)//����ʾ�ļ�����ͷͣ��һ��ʱ��
                        {
                            move_ctl.name_x=0;
                            move_ctl.wait=0;
                        }
                    }
                pos = t_list.index - t_list.page_index;
                item += t_list.itemlen * pos;//���㵱ǰѡ�еĲ˵�����ŵ�ַ
                listboxa_display_line(item, 0);
#if SLIDER_EN
                disp_invert(0, 0, 121, 2);           //��ѡ�˵�
                lcd_update(0,2 * pos, (u16)lcd_buf,121);
                lcd_update(0,2*pos+1, (u16)lcd_buf+128,121);
#else
                disp_invert(0, 0, 128, 2);           //��ѡ�˵�
                line_update_clear(2 * (pos+(bool)t_list.m_title));
#endif
            }
        }
        return;
    }
    t_list.disp_index = t_list.index;
    num = t_list.total - t_list.page_index;
    if (t_list.total - t_list.page_index > t_list.n_page) {
        num = t_list.n_page;
    }
    pos = t_list.index - t_list.page_index;
    line = 0;
    line_clear();
    if (t_list.m_title) {
        //������ģʽ
        if(task_ctl.work_sta == TASK_BT){
            //disp_menu(*(*t_list.get_list)(0xff, 1),0,0,0,ME_CENTER);
            disp_menu(STR_BT_CONTACTS,0,0,0,ME_CENTER);    //��ϵ��
        }
        line_update_clear(line);
        //item++;
        line += 2;
    } else {
        //�ޱ�����ģʽ

    }
#if SLIDER_EN
    listbox_display_slider_init(num);
#endif
    while (num--) {
        move_ctl.name_x=0;
        listboxa_display_line(item, 0);
#if SLIDER_EN
        listbox_display_slider(line,num);
#endif
        if (pos-- == 0) {
            move_ctl.name_wide=name_width(item,t_list.itemlen-1);//��ȡ�ļ�������ʾ���
#if SLIDER_EN
            if(move_ctl.name_wide>101)
#else
                if(move_ctl.name_wide>108)
#endif
                {
                    move_ctl.move_flag=1;
                }
                else
                {
                    move_ctl.move_flag=0;
                }
#if SLIDER_EN
            disp_invert(0, 0, 121, 2);           //��ѡ�˵�
#else
            disp_invert(0, 0, 128, 2);           //��ѡ�˵�
#endif
        }
        line_update_clear(line);
        item += t_list.itemlen;
        line += 2;
    }
    while (line < 8) {
#if SLIDER_EN
        listbox_display_slider(line,255);
#endif
        line_update(line);
        line += 2;
    }
}

//index: ��ʼ����;  total: ��������;  modeģʽѡ��;  param: ���ݲ���
//����ѡ�е�ֵ
#pragma location="LIST_BOX_A_SEG"
u16 listboxa(u16 index, u16 total, u8 mode, u8 itemlen, u32 param)
{
    t_list.disp_index = 0xFFFF;
    t_list.index = (index >= total) ? 0 : index;
    t_list.total = total;
    t_list.m_title = (mode & LB_NO_TITLE) ? 0 : 1;
    t_list.m_res = (mode & LB_RES) ? 1 : 0;
    t_list.m_icon = (mode & LB_NO_ICON) ? 0 : 1;
    t_list.m_roll = (mode & LB_PAGE) ? 0 : 1;
    t_list.itemlen = itemlen;
    t_list.n_page = 4;
    t_list.ok_flag = 0;

    if (t_list.m_title) {
        t_list.n_page--;        //������ģʽ�£�ÿҳ��ʾ����һ��
        t_list.total--;         //��һ��Ϊ���⣬������Ч��ѡ��
    }
    if (t_list.m_roll) {
        if (t_list.index == 0) {
            t_list.page_index = 0;
        } else if (t_list.index - 1 + t_list.n_page > t_list.total) {
            t_list.page_index = (t_list.total > t_list.n_page) ? t_list.total - t_list.n_page : 0;
        } else {
            t_list.page_index = t_list.index - 1;
        }
    } else {
        t_list.page_index = (t_list.index / t_list.n_page) * t_list.n_page;
    }
#pragma diag_remark=Pe1053
    //��ʱ����Pe1053������棬�����תСָ��
    t_list.get_list = (char *(*)(u16 index, u8 cnt))param;
    t_list.item = (*t_list.get_list)(t_list.page_index, t_list.n_page);
#pragma diag_warning=Pe1053
    while (!t_list.ok_flag) {
        listboxa_event();
        listboxa_deal_msg();
        listboxa_display();
#if ALARM_EN
        if (task_ctl.work_sta == TASK_EXIT || task_ctl.work_sta == TASK_ALARM) {
#else
        if (task_ctl.work_sta == TASK_EXIT) {
#endif
            t_list.ok_flag = 0xfe;
        }
    }

    if (t_list.ok_flag == 1) {

        return t_list.index;
    }
    if (t_list.ok_flag == 2) {
        return t_list.index;                        //����ɾ����
    }
    return 0xffff;
}


#endif
