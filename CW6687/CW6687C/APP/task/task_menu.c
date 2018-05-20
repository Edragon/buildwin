/*****************************************************************************
 * Module    : Task
 * File      : task_menu.c
 * Author    : Hanny
 * Function  : ���˵���������
 *****************************************************************************/
#include "include.h"
#if TASK_MENU_EN
#include "task.h"
#include "task_menu.h"
#include "listbox.h"

//task menu controller
type_task_menu_ctl t_menu;

//ѡ��ͼƬ��Դ
#include SET_USER_PATH(CFG_UI_MENU_DIR, /ui/ui_menu.c)
#include SET_USER_PATH(CFG_UI_MENU_DIR, /ui/ui_menu.h)

#pragma constseg="TASK_MENU_CONST"
//������ص�����˵�������˳��Ϊ�˵�˳��
extern IAR_CONST type_menu_item menu_item[] = {
    //SET_MENU_ITEM(����״̬��, ����ͼ����, �����ַ�����, ������Ӧ�����˵���Դ)
#if TASK_MUSIC_EN
    //���ֲ˵�
    SET_MENU_ITEM(TASK_MUSIC, RES_MAIN_MUSIC, STR_MAIN_MUSIC, RES_MP3_MUSIC)
#endif

#if TASK_TAPE_EN
    //�Ŵ��˵�
    SET_MENU_ITEM(TASK_TAPE, RES_MAIN_TAPE, STR_MAIN_TAPE, RES_MP3_TAPE)
#endif

#if TASK_AUX_EN
    //AUX LINEIN�˵�
    SET_MENU_ITEM(TASK_AUX, RES_MAIN_AUX, STR_MAIN_AUX, RES_MP3_LINE_IN)
#endif

#if TASK_AUX_EQ_EN
    //AUX_EQ LINEIN�˵�
    SET_MENU_ITEM(TASK_AUX_EQ, RES_MAIN_AUX, STR_MAIN_AUX, RES_MP3_LINE_IN)
#endif

#if TASK_MIC_EN
    //MIC�˵�
    SET_MENU_ITEM(TASK_MIC, RES_MAIN_MIC, STR_MAIN_RECORDER, RES_MP3_RECORD)
#endif

#if TASK_VOICE_EN
    //¼�����Ų˵�
    SET_MENU_ITEM(TASK_VOICE, RES_MAIN_VOICE, STR_MAIN_VOICE, RES_MP3_VOICE)
#endif


#if TASK_FM_EN
    //�������˵�
    SET_MENU_ITEM(TASK_FM, RES_MAIN_RADIO, STR_MAIN_FM, RES_MP3_FM)
#endif

#if TASK_BT_EN
    //�������˵�
    SET_MENU_ITEM(TASK_BT, RES_MAIN_BT, STR_MAIN_BT, RES_MP3_BT)
#endif

#if TASK_BTHID_EN
    //�����������˵�
    SET_MENU_ITEM(TASK_BT_HID, RES_MAIN_BT, STR_MAIN_BT, RES_MP3_BT)
#endif

#if TASK_RTC_EN
    //�����Ӳ˵�
    SET_MENU_ITEM(TASK_CLOCK, RES_MAIN_RTC, STR_MAIN_CLOCK, RES_MP3_CLOCK)
#endif

#if TASK_USBDEV_EN
    //USB�豸�˵�
    SET_MENU_ITEM(TASK_USBDEV, RES_MAIN_USB, STR_MAIN_USB_DEVICE, RES_MP3_USB_DEVICE)
#endif

#if TASK_EBOOK_EN
    //������˵�
    SET_MENU_ITEM(TASK_EBOOK, RES_MAIN_EBOOK, STR_MAIN_EBOOK, RES_MP3_EBOOK)
#endif

    //SET_MENU_ITEM(TASK_NAV, RES_MAIN_EXPLORER0_BMP,)

#if TASK_SETTING_EN
    //ϵͳ���ò˵�
    SET_MENU_ITEM(TASK_SETTING, RES_MAIN_SYSTEM, STR_MAIN_SETTING, RES_MP3_SETTING)
#endif

#if TASK_SPEAKER_EN
    //�������˵�
    SET_MENU_ITEM(TASK_SPEAKER, RES_MAIN_SPEARKER, STR_MAIN_LOUD_SPEAKER, RES_MP3_speaker)
#endif

#if TASK_TOMCAT_EN
    //��ķè�˵�
    SET_MENU_ITEM(TASK_TOMCAT, RES_MAIN_TOMCAT, STR_MAIN_TOMCAT, RES_MP3_TOMCAT)
#endif
};
#pragma constseg=default

//������һ�ε�����
#pragma location="TASK_MENU_SEG"
void task_menu_set_lastwork(void)
{
    u8 index;
    t_menu.index = 0;
    for (index=0; index!=(sizeof(menu_item)/sizeof(type_menu_item)); index++) {
        if (task_ctl.last_work == menu_item[index].sta) {
            t_menu.index = index;
            break;
        }
    }

    if (t_menu.index >= MENU_NUM) {
        t_menu.index = MENU_NUM - 1;
    }
    t_menu.page_index = (t_menu.index / NUM_PAGE) * NUM_PAGE;
    if (t_menu.index == 0) {
        t_menu.page_index = 0;
    } else if (t_menu.index - 1 + NUM_PAGE > MENU_NUM) {
        t_menu.page_index = (MENU_NUM > NUM_PAGE) ? MENU_NUM - NUM_PAGE : 0;
    } else {
        t_menu.page_index = t_menu.index - 1;
    }
    task_ctl.back_up_work_sta = TASK_MENU;

}

//�����ʼ��
#pragma location="TASK_MENU_SEG"
void task_menu_enter(void)
{
#if MENU_MUSIC
        set_sys_clk(SYS_24M);
#else
        set_sys_clk(SYS_12M);
#endif
    task_ctl.work_sta = TASK_MENU;

    task_menu_set_lastwork();
    //��ʼ���˵���
    t_menu.total = MENU_NUM;

    if (t_menu.index >= MENU_NUM) {
        t_menu.index = MENU_NUM - 1;
    }

    //���˵��£��Դ�Ƚϳ�ԣ��ʹ�ô��Դ�
    set_lcd_buf(LCD_FULL_MODE);

    t_menu.disp_index = 0xff;                //������index��ͬ
    t_menu.disp_flag = 1;                   //ˢ����ʾ
    t_menu.icon_index = 1;
    t_menu.icon_cnt = 0;
#if TASK_SHOWTIME_EN
    t_menu.back_sec = 0;
#endif
    //sys_ctl.bat_val = get_bat_val();
#if MENU_MUSIC
    sound_ctl_init(3, 0x800, 25, 0x800);     //����3�γ���0x800��Ϊ��ʼ����������25��0x800��Ϊ����
    user_change_volume(sys_ctl.volume);//��������
    dac_enable();
#endif
}

//�����˳�
#pragma location="TASK_MENU_SEG"
void task_menu_exit(void)
{
    //printf("task_menu_exit\n");
#if MENU_MUSIC
    dac_disable();
#endif
}

//����������
#pragma location="TASK_MENU_SEG"
void task_menu_event(void)
{
    if (move_ctl.ms_flag) {
        move_ctl.ms_flag = 0;
        if(move_ctl.move_flag)
        {
            menu_move_control(101);
        }
        t_menu.icon_cnt++;
        if (t_menu.icon_cnt >= 3) {
            t_menu.icon_cnt = 0;
            if (t_menu.icon_index >= 3) {
                t_menu.icon_index = 0;
            }
            t_menu.icon_index++;
            t_menu.disp_flag = 1;
        }
    }
    music_event();
    if (!comm_event()) {
        return;
    }
    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
#if MENU_MUSIC
        music_stop();
#endif
        draw_wait();
        t_menu.disp_flag = 1;
    }
    if (device_need_activate(DEVICE_PC)) {
        if (device_activate(DEVICE_PC)) {
            task_ctl.work_sta = TASK_USBDEV;
            return;
        }
    }
#if TASK_SHOWTIME_EN
    if(t_menu.back_sec >= RETURN_SHOW_TIME_SEC)
    {
        task_ctl.back_up_work_sta = TASK_MENU;
        task_ctl.work_sta = TASK_SHOW_TIME;
        t_menu.back_sec = 0;
    }
#endif
    device_change();
    if (fs_need_mounted()) {
        draw_wait();
        t_menu.disp_flag = 1;                   //ˢ����ʾ
        set_sys_clk(SYS_48M);                   //�����Ƶ������
        fs_init();
#if MENU_MUSIC
        set_sys_clk(SYS_24M);
#else
        set_sys_clk(SYS_12M);
#endif
    }
    if (fs_is_mounted()) {
        //task_ctl.work_sta = TASK_MUSIC;
    }
}

#if !IS_LCD_DISPLAY
//�˵���һ��
#pragma location="TASK_MENU_SEG"
void task_menu_prev(void)
{

    if (t_menu.index <= t_menu.page_index) {
#if MENU_ROLL
        t_menu.page_index--;
#else
        t_menu.page_index -= NUM_PAGE;
#endif
    }
    if (t_menu.index == 0) {
        t_menu.index = t_menu.total;
#if MENU_ROLL
        if (t_menu.total > NUM_PAGE) {
            t_menu.page_index = t_menu.total - NUM_PAGE;
        } else {
            t_menu.page_index = 0;
        }
#else
        t_menu.page_index = ((t_menu.total - 1) / NUM_PAGE) * NUM_PAGE;
#endif
    }
    t_menu.index--;
    t_menu.icon_index = 1;
    t_menu.icon_cnt = 0;
}

//�˵���һ��
#pragma location="TASK_MENU_SEG"
void task_menu_next(void)
{
    t_menu.index++;
    if (t_menu.index - t_menu.page_index >= NUM_PAGE) {
#if MENU_ROLL
        t_menu.page_index++;
#else
        t_menu.page_index += NUM_PAGE;
#endif
    }
    if (t_menu.index == t_menu.total) {
        t_menu.index = 0;
        t_menu.page_index = 0;
    }
    t_menu.icon_index = 1;
    t_menu.icon_cnt = 0;
}

//������ʾ����
#pragma location="TASK_MENU_SEG"
void task_menu_display(void)
{
    if (t_menu.disp_index != t_menu.index) {
        t_menu.disp_flag = 1;
        t_menu.disp_index = t_menu.index;
        move_ctl.name_x=0;
    }
    if (sys_ctl.bat_val != sys_ctl.disp_bat_val) {
        sys_ctl.disp_bat_val = sys_ctl.bat_val;
        t_menu.disp_flag = 1;             //���ø��±�־
    }
    if (!t_menu.disp_flag) {
        return;
    }
    t_menu.disp_flag = 0;

    u8 i, pos;
    disp_clear();

    pos = 12;
    for (i=0; i!=4 && t_menu.page_index+i<t_menu.total; i++) {
        disp_draw(menu_item[t_menu.page_index+i].r_index, pos, 0);
        if (t_menu.index == t_menu.page_index + i) {
            disp_draw(RES_ICON_FOCUSL_BMP, pos-4, 0);
            disp_menu(menu_item[t_menu.page_index+i].name, 0, move_ctl.name_x, 6, ME_CENTER);//ʾ��ǰ�˵��������
            move_ctl.name_wide = move_ctl.name_wide1;
            if(move_ctl.name_wide >= 105)
            {
                move_ctl.move_flag=1;
            }
            else
            {
                move_ctl.move_flag=0;
            }
            disp_draw(RES_ICON_FOCUSR_BMP, pos+16, 0);
            disp_draw(menu_item[t_menu.page_index+i].r_index+t_menu.icon_index, 48, 2);
        }
            pos += 24;
    }

    if (sys_ctl.bat_val >= 1) {
        disp_draw(RES_ICON_BAT0_BMP + sys_ctl.bat_val - 1, 108, 0);
    }
    disp_update();
}

//������Ϣ����
#pragma location="TASK_MENU_SEG"
void task_menu_deal_msg(u8 msg)
{
    switch(msg) {

        //ѡ��˵�
    case KU_OK:
        music_stop();
        task_ctl.work_sta = menu_item[t_menu.index].sta;
        break;

        //��һ���һ��
    case KU_PREV:
        task_menu_prev();
#if MENU_MUSIC
        music_stop();
        mp3_res_play(menu_item[t_menu.index].mp3res);
#endif
        break;
    case KU_NEXT:
        task_menu_next();
#if MENU_MUSIC
        music_stop();
        mp3_res_play(menu_item[t_menu.index].mp3res);
#endif
        break;
  /*  case KU_MENU:
            hold_mode_enter();
            break;*/
/*
    case KL_POWER:
        printf("power off\n");
        //task_ctl.work_sta = TASK_POWEROFF;
        break;
*/
        //����MENU�ػ�
#if SOFTKEY
    case KL_MENU:
        task_ctl.work_sta = TASK_POWEROFF;
        break;
#endif
    case QSYSTEM_1S:
#if TASK_SHOWTIME_EN
        t_menu.back_sec++;
#endif
        break;
    default:
        break;
    }
#if TASK_SHOWTIME_EN
    if(msg != NO_KEY && msg!= QSYSTEM_1S)
        t_menu.back_sec = 0;
#endif
}
#endif

//����������
#pragma location="TASK_MENU_SEG"
void task_menu(void)
{
    u8 msg;
    task_menu_enter();
    //printf("menu_enter\n");
    while (task_ctl.work_sta == TASK_MENU) {
        msg = get_msg();
        task_menu_event();
        task_menu_deal_msg(msg);
        task_menu_display();

    }
    task_menu_exit();
}

#endif
