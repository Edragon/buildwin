/*****************************************************************************
* Module    : Display
* File      : listbox.c
* Author    : Hanny
* Email     : coldney@yahoo.com.cn
* Function  : �б�ؼ������ڴ���UI���õ��ľ�̬�б���
*****************************************************************************/
#include "include.h"
#if LCD_THEME
#include "task.h"
#include "listbox.h"

#if SLIDER_EN
//�������ṹ�����
IAR_XDATA_A type_slider_ctl t_slider;
#endif
//list box controller
IAR_XDATA_A type_list_ctl t_list @ "OCX_XDATA";

//�б��¼�
#pragma location="LIST_BOX_SEG"
void listbox_event(void)
{
    if (!comm_event()) {
        t_list.ok_flag = 0xfe;
    }
    ocx_event();
    if (t_list.show_time && ocx_show_cnt >= t_list.show_time) {
        t_list.ok_flag = 0xff;
    }
}

//�б���һ��
#pragma location="LIST_BOX_SEG1"
void listbox_prev(void)
{
    if (t_list.index <= t_list.page_index) {
        if (t_list.m_roll) {
            t_list.page_index--;
        } else {
            t_list.page_index -= t_list.n_page;
        }
    }
    if (t_list.index == 0) {
        t_list.index = t_list.total;
        if (t_list.m_roll) {
            t_list.page_index = (t_list.total > t_list.n_page) ? t_list.total - t_list.n_page : 0;
        } else {
            t_list.page_index = ((t_list.total - 1) / t_list.n_page) * t_list.n_page;
        }
    }
    t_list.index--;
    ocx_show_cnt = 0;
    if(t_list.m_eq)
    {
        music_eq(t_list.index);
    }
}

//�б���һ��
#pragma location="LIST_BOX_SEG1"
void listbox_next(void)
{
    t_list.index++;
    if (t_list.index - t_list.page_index >= t_list.n_page) {
        if (t_list.m_roll) {
            t_list.page_index++;
        } else {
            t_list.page_index += t_list.n_page;
        }
    }
    if (t_list.index == t_list.total) {
        t_list.index = 0;
        t_list.page_index = 0;
    }
    ocx_show_cnt = 0;
    if(t_list.m_eq)
    {
        music_eq(t_list.index);
    }
}

//�б�ؼ�������Ϣ
#pragma location="LIST_BOX_SEG"
void listbox_deal_msg(void)
{
    u8 msg = get_msg();
    switch(msg) {
        //ȷ��
    case KU_OK:
        t_list.ok_flag = 1;
        break;

        //ȡ��
    case KU_BACK:
        t_list.ok_flag = 0xff;
        break;

        //�����������˵�
    case KL_MENU:
        task_ctl.work_sta = TASK_EXIT;
        break;

        //��һ���һ��
    case KU_PREV:
        listbox_prev();
        break;
    case KU_NEXT:
        listbox_next();
        break;

        //���ù�����Ϣ����
    default:
        ocx_msg(msg);
        break;
    }
}

#pragma location="LIST_BOX_SEG"
void listbox_display_line(string menu_str)
{
    if (t_list.m_icon) {
        disp_draw(RES_ICON_ITEM_BMP, 0, 0);
        disp_menu(menu_str, 18, move_ctl.name_x, 0, 0);
    } else {
        disp_menu(menu_str, 5, move_ctl.name_x, 0, 0);
    }
}
#if SLIDER_EN
//����������λ��ȷ��
#pragma location="LIST_BOX_SEG"
void listbox_display_slider_init(u8 num)
{
    t_slider.pre_num = num;
    t_slider.line_remainder = 0;
    if (t_list.m_title){
        if(t_list.total<43){
            t_slider.angle_line_nums = 42/t_list.total;
            t_slider.line_remainder = 42%t_list.total;
        }
        else{
            t_slider.angle_line_nums = 1;
        }
        if(t_slider.line_remainder){ // �ѹ������հײ��ַ�̯������
            if(t_slider.line_remainder>=t_list.index)
                t_slider.line_start = t_slider.angle_line_nums * t_list.index + 19;
            else
                t_slider.line_start = t_slider.angle_line_nums * t_list.index + t_slider.line_remainder+ 19;
        }
        else
            t_slider.line_start = t_slider.angle_line_nums * t_list.index + 19;
    }
    else {
        if(t_list.total<59){
            t_slider.angle_line_nums = 58/t_list.total;
            t_slider.line_remainder = 58%t_list.total;
        }
        else
            t_slider.angle_line_nums = 1;

        if(t_slider.line_remainder){ // �ѹ������հײ��ַ�̯������
            if(t_slider.line_remainder>=t_list.index)
                t_slider.line_start = (t_slider.angle_line_nums+1) * t_list.index + 3;
            else
                t_slider.line_start = t_slider.angle_line_nums * t_list.index + t_slider.line_remainder+ 3;
        }
        else
            t_slider.line_start = t_slider.angle_line_nums * t_list.index + 3;

    }
    if(t_slider.line_start>60)
        t_slider.line_start = 60;
    t_slider.line = (t_slider.line_start/16) <<1;
    t_slider.line_start %=16;
}

//��������ʾ
#pragma location="LIST_BOX_SEG"
void listbox_display_slider(u8 line,u8 num)
{
    memset(lcd_buf+121, 0, 6);
    memset(lcd_buf+249, 0, 6);
    lcd_buf[122]=0xff;
    lcd_buf[250]=0xff;
    if(( t_slider.line== line)&&t_slider.angle_line_nums)
    {
        if(t_slider.line_start>7)
		     	{
                    t_slider.line_start -= 8;
                    lcd_buf[124] = 0x00;
                    for(;t_slider.line_start<8;t_slider.line_start++){
                        lcd_buf[252] |= BIT(t_slider.line_start);
                        if(--t_slider.angle_line_nums < 1){
                            break;
                        }
                    }	
                }
        else
        {
            for(;t_slider.line_start<8;t_slider.line_start++){
                lcd_buf[124] |= BIT(t_slider.line_start);
                if(--t_slider.angle_line_nums < 1){
                    break;
                }
            }
            if(t_slider.angle_line_nums > 0){
                t_slider.line_start=0;
                for(;t_slider.line_start<8;t_slider.line_start++){
                    lcd_buf[252] |= BIT(t_slider.line_start);
                    if(--t_slider.angle_line_nums < 1){
                        break;
                    }
                }

            }	
        }
        if(t_slider.angle_line_nums > 0)
        {
            t_slider.line +=2;
            t_slider.line_start=0;
        }

    }
    if((t_slider.pre_num-1) == num){
        lcd_buf[122] = 0xfc;
        lcd_buf[123] |= 0x02;
        lcd_buf[124] |= 0x01;
    }
    if(line == 6){
        lcd_buf[250] = 0x3f;
        lcd_buf[251] |= 0x40;
        lcd_buf[252] |= 0x80;
    }
    lcd_buf[125] = lcd_buf[124];
    lcd_buf[126] = lcd_buf[123];
    lcd_buf[127] = lcd_buf[122];
    lcd_buf[253] = lcd_buf[252];
    lcd_buf[254] = lcd_buf[251];
    lcd_buf[255] = lcd_buf[250];
}
#endif
#pragma location="LIST_BOX_SEG1"
void menu_move_control(u8 wide)
{
  if(move_ctl.name_x+(s16)move_ctl.name_wide>wide)
  {

    if(move_ctl.name_x==0)
    {
      move_ctl.wait++;
      if(move_ctl.wait==11)//����ʾ�ļ�����ͷͣ��һ��ʱ��
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
    if(move_ctl.wait==11)//����ʾ�ļ���β��ͣ��һ��ʱ��
    {
      move_ctl.name_x=0;
      move_ctl.wait=0;
    }
  }
}


//�б���ʾ�ӿ�
#pragma location="LIST_BOX_SEG"
void listbox_display(void)
{
    u8 pos, line;
    u8 num;
    string *item = t_list.llp_item;
    line_clear();
    if (t_list.index == t_list.disp_index) {
        if(move_ctl.ms_flag)
        {
            move_ctl.ms_flag=0;
            if(move_ctl.move_flag)
            {
#if SLIDER_EN			
                menu_move_control(93);
#else
                menu_move_control(101);
#endif			
                pos = t_list.index - t_list.page_index;
                item += t_list.index ;//���㵱ǰѡ�еĲ˵�
                if (t_list.m_title) {
                    pos += 1;
                    item += 1;
                }
                listbox_display_line(*item);
                disp_invert(0, 0, 121, 2);           //��ѡ�˵�
                lcd_update(0,2 * pos, (u16)lcd_buf,121);
                lcd_update(0,2*pos+1, (u16)lcd_buf+128,121);
            }
        }
        return;
    }
    t_list.disp_index = t_list.index;
    num = t_list.total - t_list.page_index;         //��ʾ������
    if (t_list.total - t_list.page_index > t_list.n_page) {
        num = t_list.n_page;
    }
    pos = t_list.index - t_list.page_index;         //��ѡ��λ��
    item += t_list.page_index;
    line = 0;


    if (t_list.m_title) {
        //������ģʽ
        disp_menu(*t_list.llp_item,0,0, 0,ME_CENTER);
        line_update_clear(line);
        item++;
        line += 2;
    }
#if SLIDER_EN
     listbox_display_slider_init(num);
#endif
    //�ޱ�����ģʽ
    while (num--) {
        move_ctl.name_x=0;
        listbox_display_line(*item);
#if SLIDER_EN
       listbox_display_slider(line,num);
#endif
        if (pos-- == 0) {
            move_ctl.name_wide = move_ctl.name_wide1;
#if SLIDER_EN				
            if(move_ctl.name_wide >= 101)
#else
            if(move_ctl.name_wide >= 108)
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
        item++;
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

//index: ��ʼ����;  total: ��������;  modeģʽѡ��;  param: ���ݲ���; show_time: �б����ʾʱ��
//����ѡ�е�ֵ
#pragma location="LIST_BOX_SEG"
u8 listbox(u8 index, u8 total, u8 mode, u16 param, u8 show_time)
{
    t_list.disp_index = 0xFFFF;
    t_list.index = index;
    t_list.total = total;
    t_list.m_title = (mode & LB_NO_TITLE) ? 0 : 1;
    t_list.m_res = (mode & LB_RES) ? 1 : 0;
    t_list.m_icon = (mode & LB_NO_ICON) ? 0 : 1;
    t_list.m_roll = (mode & LB_PAGE) ? 0 : 1;
    t_list.m_eq = (mode & LB_EQ) ? 1 : 0;
    t_list.n_page = 4;
    t_list.ok_flag = 0;
    t_list.show_time = show_time;

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
    t_list.llp_item = (string *)param;

    ocx_show_cnt = 0;
    while (!t_list.ok_flag) {
        listbox_event();
        listbox_deal_msg();
        listbox_display();
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
    return 0xff;
}

#endif
