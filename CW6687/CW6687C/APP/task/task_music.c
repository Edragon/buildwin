/*****************************************************************************
 * Module    : Task
 * File      : task_music.c
 * Author    : Hanny
 * Function  : ���ֲ�����������
 *****************************************************************************/
#include "include.h"

#if TASK_MUSIC_EN
#include "task.h"
#include "task_music.h"
#include "user_spi.h"

//�����������ʹ��
#define DBG_TASK_MUSIC(...)          //printf(__VA_ARGS__)

//type_move_ctl move_ctl;
extern u8 work_sta_interrupt;   //�����жϱ�־
#if MUSIC_ENCRYPT_EN
extern u8 music_encrypt;
extern void music_head_decrypt(void);
extern bool lrc_open_encrypt(void);
#endif
extern u16 crc_calculate(void *buf, u16 len);
void task_music_comm_event(void);
void task_music_comm_deal_msg(u8 msg);
void task_music_get_num(void);
void task_music_prev(void);
void task_music_next(void);
void task_music_event(void);
void task_music_break_save(void);
void task_music_select_numsic(u8 num);
extern u8 dec_ibuf[0xa08];
void task_music_repeat_play(void);
bool task_music_auto_next_device(void);
enum {
    PLAYER_NORMAL,                  //��������
    PLAYER_REPEAT,                  //����ģʽ
};

//task music controller
IAR_XDATA_A type_task_music_ctl t_msc @ "MUSIC_XDATA";

void task_music_set_a(void);
void task_music_clear_a(void);
bool task_music_get_total(void);
void task_music_repeat_start(u8 repeat_cnt);
void task_music_lrc_init(void);
void task_music_device_remount(void);
void task_music_play_init(void);
void music_play_vol_music(void);
extern type_music_point pt;
IAR_CONST u8 tbl_speed_order[] = {0, 3, 4, 0, 1, 2};

#define TASK_SUB
#include SET_USER_PATH(CFG_UI_MUSIC_DIR, /ui/ui_music.c)
#include "sub/tasksub_music_menu.c"

#if BREAK_POINT_EN
#pragma location="TASK_MUSIC_SEG"
//����ϵ����
void task_music_break_save(void)
{
    if (fs_cur_dev() == DEVICE_UDISK)
    {
        irtcc_write_ram32(USB_BREAK_FRAME, t_msc.break_point.frame);
        irtcc_write_ram32(USB_BREAK_POS, t_msc.break_point.pos);
    }
    else
    {
        irtcc_write_ram32(SD_BREAK_FRAME, t_msc.break_point.frame);
        irtcc_write_ram32(SD_BREAK_POS, t_msc.break_point.pos);
        //printf("fra:%ld,pos:%ld\n",t_msc.break_point.frame,t_msc.break_point.pos);
    }
}

//������һ��ʱ������ϵ���Ϣ
void task_music_break_clr(void)
{
    if(task_ctl.last_work == TASK_MUSIC && !sys_ctl.voice_play_flag && !t_msc.music_voice_flag) {
        u32 cur_sec = music_get_mtime()/10;
        //printf("mtime: %04x, alltime: %04x\n", (u16)cur_sec, music_get_alltime());
        if(cur_sec >= 35999) {  //alltime���ֵΪ35999s
            return;             //����35999s���ļ�����ϵ�
        }
        if((u16)cur_sec+2 >= music_get_alltime()) {
            //printf("clr_bp\n");
            if (t_msc.break_dev == DEVICE_UDISK) {  //2014-1-6, �������ϵ�����
                irtcc_write_ram16(USB_NAME_CRC, ~t_msc.filename_crc);
            } else {
                irtcc_write_ram16(SD_NAME_CRC, ~t_msc.filename_crc);
            }
        }
    }
}

//��ȡ�ϵ����
#pragma location="TASK_MUSIC_SEG_1"
void task_music_break_read(void)
{
    if (fs_cur_dev() == DEVICE_UDISK)
    {
        t_msc.break_point.frame = irtcc_read_ram32(USB_BREAK_FRAME);
        t_msc.break_point.pos = irtcc_read_ram32(USB_BREAK_POS);
        t_msc.filename_crc = irtcc_read_ram16(USB_NAME_CRC);
    }
    else
    {
        t_msc.break_point.frame = irtcc_read_ram32(SD_BREAK_FRAME);
        t_msc.break_point.pos = irtcc_read_ram32(SD_BREAK_POS);
       // printf("frar:%ld,posr:%ld\n",t_msc.break_point.frame,t_msc.break_point.pos);
        t_msc.filename_crc = irtcc_read_ram16(SD_NAME_CRC);
    }
}

#pragma location="TASK_MUSIC_SEG_1"
void task_music_save_num(void)
{
    if(t_msc.music_voice_flag) {
        sys_param_write16(PARAM_VOICE_NUM, t_msc.music_num);        //����򿪵��ļ����
    } else {
        if (fs_cur_dev() == DEVICE_UDISK) {
            sys_param_write16(PARAM_MUSIC_NUM_USB, t_msc.music_num);        //����򿪵��ļ����
        } else {
            sys_param_write16(PARAM_MUSIC_NUM_SD, t_msc.music_num);        //����򿪵��ļ����
        }
    }
}

//������������Զϵ����Ĳ���
#pragma location="TASK_MUSIC_SEG_2"
void task_music_break_init(void)
{
    if(t_msc.break_flag)
    {
        task_music_break_read();
        if(crc_calculate(filename,8) == t_msc.filename_crc)
        {
            music_jump(&t_msc.break_point);
        }
    }
    t_msc.filename_crc = crc_calculate(filename,8);
    t_msc.break_dev = fs_cur_dev();             //2014-1-6, �������ϵ�����
    if (fs_cur_dev() == DEVICE_UDISK)
    {
        irtcc_write_ram16(USB_NAME_CRC, t_msc.filename_crc);
    }
    else
    {
        irtcc_write_ram16(SD_NAME_CRC, t_msc.filename_crc);
    }
}
#endif

//������������Ը���Ŀ�Ļ�ȡ
#pragma location="TASK_MUSIC_SEG_1"
void task_music_get_num(void)
{
    if (fs_cur_dev() == DEVICE_UDISK) {
        t_msc.music_num = sys_param_read16(PARAM_MUSIC_NUM_USB);        //�򿪵��ļ����
    } else {
        t_msc.music_num = sys_param_read16(PARAM_MUSIC_NUM_SD);        //�򿪵��ļ����
    }
    if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
        t_msc.music_num = 1;
    }
}

/*****************************************************************************
 * Module    : �������񹫹�����
 *****************************************************************************/
//���������ʼ��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_enter(void)
{
    led_mode_off();
    led_idle();
    set_sys_clk(SYS_48M);
    draw_wait();
#if FIRST_PLAY_UDISK                                  //2014-1-2, ����U������ʱ��������ģʽ����SD���Բ�U��
    if(device_is_actived(DEVICE_SDMMC) && device_is_actived(DEVICE_UDISK)) { //U�̺�SD��ͬʱ����ʱ���Ȳ���U��
        if(fs_cur_dev() != DEVICE_UDISK){
            fs_sel(DEVICE_UDISK);
        }
    }
#endif

/*    if (!device_is_actived(DEVICE_SDMMC)) {
        if (!device_is_actived(DEVICE_UDISK))
        {
            printf("A\n");
            msgbox(STR_NO_DEVICE, NULL, MSGBOX_TIME);
            task_ctl.work_sta = TASK_EXIT;      //�豸��Ч���������˵�
            return;
        }
    } */
    draw_wait();
    //��������
    task_ctl.work_sta = TASK_MUSIC;
    memset(&t_msc,0, sizeof(t_msc));
#if BREAK_POINT_EN
    t_msc.break_flag = 1;
#endif
    //������Ϣ��ʼ��
    t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
    t_msc.music_num = 1;                    //�򿪵��ļ����
    sys_ctl.dig_vol = 13;
    sys_ctl.mic_volume = 8;
    ocx_init(task_music_event, NULL);       //����OCX��Ҫ��Ӧ����Ϣ���¼�
    t_msc.disp_flag = 1; //��ʾ����
    //change_digital_volume(sys_ctl.dig_vol);
    //sound_ctl_init(3, 0x800, 25, 0x800);     //����3�γ���0x800��Ϊ��ʼ����������25��0x800��Ϊ����
    user_change_volume(sys_ctl.volume);//ģ������
    dac_enable();
    //if (!sys_ctl.mute_flag) {
    //    sys_unmute();
    //}

    //����ɨ�������
    f_scan_set(SCAN_SUB_FOLDER|SCAN_SPEED, &file_filter_music, &dir_filter_music);
    set_sys_clk(SYS_48M);
    if(!music_mount_device()) {
        msgbox(STR_NO_MUSIC, NULL, MSGBOX_TIME);
        if(task_ctl.work_sta != TASK_UPDATE_FW)
            task_ctl.work_sta = TASK_EXIT;          //�豸��Ч���˳�
        return;
    }
    task_music_get_num();
    //��ʼ����

#if MUSIC_MODE_WARNING_MUSIC
    if (work_sta_interrupt == TASK_NULL) {
         mp3_res_play_wait(RES_MP3_MUSIC_MODE);
    }
#endif
    work_sta_interrupt = TASK_NULL;
    led_idle();

    dac_dynamic(DYNAMIC_NO);
#if MIC_DIRECT_OUT_EN
    mic_direct_out_enter(MIC_CHANNEL_SEL,11);
#endif
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf = lcd_buf2;   // lcdʹ��Сbuf��buf1���lrc��buf������
#endif
}

//��ʼ����music_num�׸裬������LRC�ļ�
#pragma location="MUSIC_SEG"
bool task_music_init(u16 music_num)
{
    set_sys_clk(SYS_48M);
    if (!f_open_opt(music_num)) {
        return false;
    }
    t_msc.dir_num = fs_apiinfo.dir_count;
#if MUSIC_ENCRYPT_EN
    music_check_encrypt();//�ж��Ƿ�Ϊ����mp3
#endif
    f_get_opt_filename(filename);
#if MUSIC_ENCRYPT_EN
    music_head_decrypt();
#endif
#if MUSIC_LRC_EN
    task_music_lrc_init();                  //�����mp3��ʼ��֮ǰ���ң���������ƻ�MP3����
#endif
    //printf("dir: %d %d\n", t_msc.dir_start, t_msc.dir_total);

    t_msc.music_type = music_init();
#if MUSIC_ID3_EN
    music_decode_ID3();
#endif
    if (t_msc.music_type == TYPE_MP3) {
        set_sys_clk(SYS_24M);               //MP3ʹ��24MHz
    }
    return true;
}

#if MUSIC_LRC_EN
//��ʼ�����
#pragma location="MUSIC_SEG"
bool music_lrc_init(void)
{
#if MUSIC_ENCRYPT_EN
    if (!lrc_open_encrypt())
#endif
    {
        if (!f_open_lrcfile()) {
            return false;
        }
    }
    t_msc.lrc_type = lrc_init();

#if (LRC_FILENAME_TIME == 1)
    //ֱ����ʾ���
    t_msc.lrc_change = music_get_lrc(music_lrc_cur_time());
#endif
    return true;
}

//�մ򿪸�ʱ����Ҫ���Ҳ���ʼ��LRC�ļ�
#pragma location="TASK_MUSIC_SEG_1"
void task_music_lrc_init(void)
{
    t_msc.lrc_flag = music_lrc_init();            //�򿪸��
    if (!t_msc.lrc_flag) {
        t_msc.filename_change = 1;          //ˢ���ļ�����ʾ
    } else {
        t_msc.lrc_change = 1;               //ˢ�¸����ʾ
    }
}
#endif

//�˳�Music����
#pragma location="TASK_MUSIC_SEG_1"
void task_music_exit(void)
{
    //printf("task_music_exit\n");
#if MODE_CHANGE_BLUE_ON
    led_mode_on();
#endif
    music_stop();                           //����Music����
#if MIC_DIRECT_OUT_EN
    mic_direct_out_exit();
#endif
    sys_unmute();
    dac_disable();
    dac_dynamic(DYNAMIC_NO);
#if THEME_SELECT==THEME_LCD_DEFAULT
    lcd_buf = lcd_buf1;   // lcd�ָ�ʹ�ô�buf
#endif
}

#pragma location="TASK_MUSIC_SEG_1"
bool task_music_auto_prev_device(void)
{
    if(is_other_device()) {                 //����Ƿ��������豸
        //t_msc.music_err = 0;
        if(!music_next_device()) {          //ѡ����һ�豸�����ļ�ϵͳ
            if(!music_next_device()) {      //�������һ�豸����ԭ�豸
                task_ctl.work_sta = TASK_EXIT;  //�豸��Ч���˳�
                return false;
            }
        }
    }

    t_msc.music_num = t_msc.music_total;    //�ӵ����һ�׸貥��
    t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
    return true;
}

//�Զ���һ������һ�豸����
#pragma location="TASK_MUSIC_SEG"
bool task_music_auto_next_device(void)
{
    if(is_other_device()) {                 //����Ƿ��������豸
        //t_msc.music_err = 0;
        if(!music_next_device()) {          //ѡ����һ�豸�����ļ�ϵͳ
            if(!music_next_device()) {      //�������һ�豸����ԭ�豸
                task_ctl.work_sta = TASK_EXIT;  //�豸��Ч���˳�
                return false;
            }
        }
    }

    t_msc.music_num = 1;                    //�ӵ�һ�׸貥��
    t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
    return true;
}

#if AB_REPEAT_EN
//����ABʱ��
#pragma location="TASK_MUSIC_SEG_1"
u32 task_music_calc_pointab(void)
{
    return (music_calc_mtime(t_msc.point.pb.frame) - music_calc_mtime(t_msc.point.pa.frame));
}
#endif

//��һ�ļ���
#if FOLDER_SELECT
#pragma location="TASK_MUSIC_SEG_1"
void task_music_prev_dir(void)
{
    if(t_msc.dir_total <= 1) {
        return;
    }

    if(t_msc.dir_num <= 1) {
        t_msc.dir_num = t_msc.dir_total;
    } else {
        t_msc.dir_num--;
    }

    music_stop();
    f_open_folder(t_msc.dir_num);
    t_msc.file_change = 1;
    t_msc.music_num = fs_apiinfo.file_count;
}

//��һ�ļ���
#pragma location="TASK_MUSIC_SEG_1"
void task_music_next_dir(void)
{
    if(t_msc.dir_total <= 1) {
        return;
    }

    if(t_msc.dir_num >= t_msc.dir_total) {
        t_msc.dir_num = 1;
    } else {
        t_msc.dir_num++;
    }

    music_stop();
    f_open_folder(t_msc.dir_num);
    t_msc.file_change = 1;
    t_msc.music_num = fs_apiinfo.file_count;
}
#endif

//��һ��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_prev(void)
{
#if AUTO_CHANGE_DEVICE
    if(sys_ctl.mode_play == ITEM_LOOP_ALL)  //ȫ��ѭ�����Զ����豸
    {
        if(t_msc.music_num == 1)
        {
            if(task_music_auto_prev_device())
            {
#if BREAK_POINT_EN
                t_msc.break_flag = 0;
#endif
                return;
            }
        }
    }
#endif
    u16 dir_start = f_get_indexbefore() + 1;        //��ǰĿ¼��һ���ļ��ı��
    switch (sys_ctl.mode_play) {
    case ITEM_LOOP_ALL:
    case ITEM_LOOP_SINGLE:
    case ITEM_LOOP_NORMAL:
        //ȫ��ѭ��
        t_msc.music_num--;
        if (t_msc.music_num == 0 || t_msc.music_num > t_msc.music_total) {
            t_msc.music_num = t_msc.music_total;
        }
        break;
    case ITEM_LOOP_FOLDER:
    case ITEM_LOOP_BROWSE:
        //Ŀ¼ѭ��
	    music_stop();
        t_msc.dir_last = dir_start + f_get_dirtotal() - 1;
        t_msc.music_num--;
        if (t_msc.music_num < dir_start || t_msc.music_num > t_msc.dir_last) {
            t_msc.music_num = t_msc.dir_last;
        }
        break;
    case ITEM_LOOP_RAMDOM:
        t_msc.music_num = rand(t_msc.music_total) + 1;
        break;
    }
    t_msc.file_change = 1;
    t_msc.file_prev_next = 1;                       //���÷��򣬹��������ʱʹ��
}

//��һ��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_next(void)
{
#if AUTO_CHANGE_DEVICE
    if(sys_ctl.mode_play == ITEM_LOOP_ALL)          //ȫ��ѭ�����Զ����豸
    {   //ȫ��ѭ�����Զ����豸
        if(t_msc.music_num == t_msc.music_total)
        {
            if(task_music_auto_next_device())
            {
#if BREAK_POINT_EN
                t_msc.break_flag = 0;
#endif
                return;
            }
        }
    }
#endif
    u16 dir_start = f_get_indexbefore() + 1;        //��ǰĿ¼��һ���ļ��ı��
    switch (sys_ctl.mode_play) {
    case ITEM_LOOP_ALL:
    case ITEM_LOOP_SINGLE:
    case ITEM_LOOP_NORMAL:
        //ȫ��ѭ��
        if (t_msc.music_num >= t_msc.music_total) {
            t_msc.music_num = 0;
        }
        t_msc.music_num++;
        break;
    case ITEM_LOOP_FOLDER:
    case ITEM_LOOP_BROWSE:
        //Ŀ¼ѭ��
		music_stop();
        t_msc.dir_last = dir_start + f_get_dirtotal() - 1;
        t_msc.music_num++;
        if (t_msc.music_num < dir_start || t_msc.music_num > t_msc.dir_last) {
            t_msc.music_num = dir_start;
        }
        break;
    case ITEM_LOOP_RAMDOM:
        t_msc.music_num = rand(t_msc.music_total) + 1;
        break;
    }
    t_msc.file_change = 1;
    t_msc.file_prev_next = 0;                       //���÷��򣬹��������ʱʹ��
}

//�Զ���һ��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_auto_next(void)
{
    if (music_get_mtime() < 3) {
        //���Ų���300ms����Ϊ�������
        if (t_msc.file_prev_next) {             //��ʼ��ʧ�ܣ����¶�ȡ��һ���ļ�
            task_music_prev();
        } else {
            task_music_next();
        }
    } else {
        t_msc.file_prev_next = 0;                       //�������
        t_msc.file_change = 1;
        if (sys_ctl.mode_play != ITEM_LOOP_SINGLE) {
            task_music_next();
        }
    }
}

//������ʾ��ʱ��
#pragma location="TASK_MUSIC_SEG"
void task_music_cal_time(void)
{
    u32 cur_sec = music_get_mtime() / 10;           //��ǰ����ʱ�䣬��ȷ��0.1s

    t_msc.cur_time.min = cur_sec / 60;
    t_msc.cur_time.sec = cur_sec % 60;

#if AB_REPEAT_EN
    u32 a_time = (music_point_a_mtime() + 5) / 10;
    t_msc.rtime = ((cur_sec > a_time) ? (cur_sec - a_time) : 0);
#endif
}

//���������һЩ��Ϣ��ʼ��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_play_init(void)
{
    t_msc.pause = 0;                        //�����ͣ״̬
    t_msc.speed = 0;                        //���ò����ٶ�
/*
#if AB_REPEAT_EN
    t_msc.auto_repeat_start = 0;
    umsc_sound_detect(t_msc.auto_repeat);
#endif*/
    //sound_set_flag(0, 0);

    music_set_speed(0);

    t_msc.play_sta = PLAYER_NORMAL;         //��������״̬
}

//�����ڲ��Ÿ���ʱ���������˵��Ĳ��ţ������������˵����������ԭ���ĸ���
void mp3res_play_musicmode(u8 music_name)
{
    type_music_point pt;
    music_get_point(&pt);                       //���÷��ز��ŵ�
    music_stop();                               //ֹͣ�������ֲ���

    mp3_res_play_wait(music_name);              //������ʾ��

    music_init();
#if MUSIC_LRC_EN
    if (t_msc.lrc_flag) {
        t_msc.lrc_type = lrc_init();            //�ٴγ�ʼ�����
    }
#endif
    task_music_play_init();
    music_jump(&pt);                            //�ָ�����
    music_play();
}

#pragma location="TASK_MUSIC_SEG_1"
void task_music_new(void)
{
    u16 err_cnt = 0;

    led_music_play();

    while (device_is_actived(fs_cur_dev()) && t_msc.file_change) {
        DBG_TASK_MUSIC("music_stop\n");
        music_stop();                               //ֹͣ������
        t_msc.file_change = 0;
#if BT_SPP_MSC_EN
        spp_tx_msc_num(t_msc.music_num);            //�и裬֪ͨSPP��Ŀ�仯
#endif
        //����ָ������
        if (task_music_init(t_msc.music_num)) {
            task_music_play_init();
#if AB_REPEAT_EN
            t_msc.auto_repeat = 0;                  //����Զ��������
#endif
#if BREAK_POINT_EN
            if(!t_msc.music_voice_flag) {           //����music_playǰ����ֹwav�ϵ����ʱ�Ȳ���һС�ο�ͷ����������㲥��
                task_music_break_init();
            }
            task_music_save_num();
#endif
            //task_music_clear_a();                   //���A��
            music_play();
            //delay_5ms(5);                           //��ʱһС��ʱ�䣬�Խ⼸֡���������Ӧ��Ϣ
            t_msc.disp_music_time.sec = 0xff;       //�ݲ���ʾ��ʱ��
        } else {
            if (!fs_is_mounted() || ++err_cnt >= t_msc.music_total) {    //���е������ļ����޷�����
#if K_MODE_CHANGE_DEVICE
                if(is_other_device() && !t_msc.mode_flag) {
                    t_msc.mode_flag = 1;
                    task_music_auto_next_device();
                } else
#endif
                {
                    task_ctl.work_sta = TASK_EXIT;      //���������ļ����࣬�˳�Ŀ¼����
                    msgbox(STR_FILE_ERROR, NULL, MSGBOX_TIME);
                }
                break;
            }

            if (t_msc.file_prev_next) {             //��ʼ��ʧ�ܣ����¶�ȡ��һ���ļ�
                task_music_prev();
            } else {
                task_music_next();
            }
        }
    }
}

//��������������
#pragma location="TASK_MUSIC_SEG"
void task_music_event(void)
{
    task_music_comm_event();                        //���ù����¼�
    if(sys_ctl.vol_maxmin_music_play){  //���ڲ�������������ʾ��״̬
        return;
    }
    //����豸״̬
    if (!device_is_actived(fs_cur_dev())) {
        music_stop();                           //ֹͣ��ǰ����
        task_ctl.dev_change = 1;                //��ǰ�豸��Ч���豸�����䶯
    }

    if (device_need_activate(DEVICE_SDMMC) || device_need_activate(DEVICE_UDISK)) {
        music_stop();       //���ŵ�ǰ�豸ʱ������һ�豸������ֹͣ��ǰ����
        task_ctl.dev_change = 1;
        t_msc.file_change = 1;
#if BREAK_POINT_EN
        t_msc.break_flag = 1;
#endif
    }

    if (device_change()) {
        if (!music_remount_device()) {
            if (!music_next_device()) {
                task_ctl.work_sta = TASK_EXIT;  //�豸��Ч���˳�
                return;
            }
        }
        task_music_get_num();
        t_msc.file_change = 1;                  //��Ҫ��һ�����ļ�
#if BREAK_POINT_EN
        t_msc.break_flag = 1;
#endif
#if K_MODE_CHANGE_DEVICE
        t_msc.mode_flag = 0;
#endif
    }

#if MUSIC_LRC_EN
    //��ȡ���
    if (t_msc.lrc_flag == LRC_FILENAME_TIME) {
        if(music_get_lrc(music_get_mtime() + 3))  //��ʹ�����ǰ300ms��ʾ
        {
            t_msc.lrc_change = 1;
        }
    }
#endif

    //�����ļ�
    if (t_msc.file_change) {
        //set_sys_clk(SYS_24M);
        task_music_new();
        t_msc.disp_flag = 1;
        sys_ctl.sleep_cnt = 0xffff;
#if PLAY_DEVICE_SAVE_EN
        sys_param_write8(PARAM_PLAY_DEVICE,fs_cur_dev());
#endif
#if AB_REPEAT_EN
        t_msc.repeat_flag = 0;
        task_music_set_a();
#endif
#if (IS_LCDSEG_DISPLAY || IS_LEDSEG_DISPLAY)
        show_num(t_msc.music_num);         //LCD��������LED����ʾ�������
#endif
    }
    //���㵱ǰ��ʾ��ʱ��
    task_music_cal_time();

    if (t_msc.disp_music_time.sec == 0xff) {
        u16 all_time = music_get_alltime();
        //printf("t:%d\n",all_time);
        if (all_time != 0xffff) {
            t_msc.disp_music_time.min = all_time / 60;      //��ʱ��
            t_msc.disp_music_time.sec = all_time % 60;
            t_msc.disp_status_flag = 1;                     //����״̬��ʾ
        }
    }

    //�ж����ֵĲ���״��
    if (music_get_status() <= STATUS_PLAY_STOPPING) {
#if BREAK_POINT_EN
        if(t_msc.break_flag) {
            t_msc.break_flag = 0;
            t_msc.music_num = 0;
            t_msc.file_change = 1;
        }
        task_music_break_clr();
#endif
        if(sys_ctl.mode_play == ITEM_LOOP_NORMAL)
        {
            if(t_msc.music_num >= t_msc.music_total)
            {
                return;
            }
        }
        task_music_auto_next();                      //����������ϣ�������һ��
    }
#if BREAK_POINT_EN
    t_msc.break_flag = 0;
#endif

#if AB_REPEAT_EN
    //����״̬����
    switch (t_msc.play_sta) {
    case PLAYER_NORMAL:
        /*if (t_msc.auto_repeat) {
            task_music_autorepeat();
            t_msc.disp_status_flag = 1;
        }*/
        break;
    case PLAYER_REPEAT:
        if (music_get_repeat() == 0) {
            t_msc.play_sta = PLAYER_NORMAL;
            task_music_set_a();
            t_msc.disp_status_flag = 1;             //����״̬��ʾ
        }
        break;
    }
#endif
    //�������
    if(sys_ctl.mode_play == ITEM_LOOP_BROWSE) {
        if(music_get_mtime() > 100) {
            task_music_next();
        }
    }

#if MUSIC_AUTO_STANDBY_EN || IR_HARD_POWERDOWN_EN
    if((((!sys_ctl.sleep_cnt) && BT_IS_SLEEP) || sys_ctl.ir_power_flag) && !sys_ctl.lowpower_flag){
        if (music_get_status() < STATUS_PLAY_PLAYING) {
            task_hold();
            put_msg(KU_PLAY);   //U�̻�SD�����Ѻ�ֱ�Ӳ���
        }
    }
#endif

#if WARNING_LOWPOWER
    if(sys_ctl.lowpower_warning_flag == 1){
        sys_ctl.lowpower_warning_flag = 2;
        mp3res_play_musicmode(RES_MP3_PLEASE_CHARGE);
    }
#endif
}

#if AB_REPEAT_EN
/*****************************************************************************
 * Module    : ��������AB������׷��������
 *****************************************************************************/
//���A��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_clear_a(void)
{
    memset(&t_msc.point.pa, 0, sizeof(t_msc.point.pa));
    music_set_a_point(&t_msc.point.pa);
}

//����A��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_set_a(void)
{
    music_get_point(&t_msc.point.pa);
    music_set_a_point(&t_msc.point.pa);
    //printf("a fra:%08lx\n",t_msc.point.pa.frame);
    //printf("a pos:%08lx\n",t_msc.point.pa.pos);
}

//��ʼ����
#pragma location="TASK_MUSIC_SEG_2"
void task_music_repeat_start(u8 repeat_cnt)
{
    DBG_TASK_MUSIC("repeat\n");
    music_get_point(&t_msc.point.pb);
    music_jump(&t_msc.point.pa);
    //printf("b fra:%08lx\n",t_msc.point.pb.frame);
    //printf("b pos:%08lx\n",t_msc.point.pb.pos);
    music_repeat(repeat_cnt, &t_msc.point);
    t_msc.play_sta = PLAYER_REPEAT;
}

//����/����
#pragma location="TASK_MUSIC_SEG_2"
void task_music_repeat_play(void)
{
    if (t_msc.pause) {
        return;
    }
    if (t_msc.play_sta == PLAYER_NORMAL) {
        if (t_msc.auto_repeat) {
            t_msc.auto_repeat = 0;
            umsc_sound_detect(0);
            task_music_set_a();                     //����A��
            return;
        }
        task_music_repeat_start(REPEAT_LOOP);
    } /*else {
        DBG_TASK_MUSIC("play\n");
        t_msc.auto_repeat = 0;                      //�ر��Զ�����
        umsc_sound_detect(0);
        music_jump(&t_msc.point.pb);
        task_music_set_a();                         //����A��
        t_msc.play_sta = PLAYER_NORMAL;
    }*/
}
#endif


//�����ӿڣ����ڽ��������
__near_func bool music_repeat_hook(void)
{
#if AB_REPEAT_EN
    return music_repeat_hook2();
#else
    return true;
#endif
}

#if AB_REPEAT_EN
//�Զ����
#pragma location="TASK_MUSIC_SEG_1"
void task_music_msg_autorepeat(void)
{
    if (t_msc.pause) {
        return;
    }
    task_music_set_a();
    t_msc.auto_repeat = 1;
#if MUSIC_LRC_EN
    if (t_msc.lrc_flag) {
        t_msc.next_sentence = music_lrc_next_time();
    } else
#endif
    {
        umsc_sound_detect(1);                 //�޸�ʵĻ������þ������
    }
    t_msc.next_sentence = music_lrc_next_time();
    //printf("next:%ld\n", t_msc.next_sentence);
    //sound_set_flag(0, 0);
    t_msc.disp_status_flag = 1;
}
#endif

//���ֵ��
#pragma location="TASK_MUSIC_SEG_1"
void task_music_select_numsic(u8 num)
{
    u16 f_index = numbox(num, NULL);
    if (f_index != 0 && f_index <= t_msc.music_total) {
        t_msc.music_num = f_index;                //�򿪲�������
        t_msc.file_change = 1;
        t_msc.file_prev_next = 0;               //�������
#if (IS_LCDSEG_DISPLAY || IS_LEDSEG_DISPLAY)
        show_num(t_msc.music_num);         //LCD��������LED����ʾ�������
#endif
    }
}

#if(WARNING_VOL_MAXMIN)
type_music_point pt;
#endif

#if WARNING_VOL_MAXMIN
#pragma location="TASK_MUSIC_SEG_1"
void music_play_vol_music(void)
{
    if(!sys_ctl.vol_maxmin_music_play){
        sys_ctl.vol_maxmin_music_play = 1;
        music_get_point(&pt);               //���÷��ز��ŵ�
    }
    mp3_res_play_wait(RES_MP3_VOL_MAXMIN);
    flush_msg();
}
#endif

/*****************************************************************************
 * Module    : ��������Ĺ�����Ϣ���¼�
 *****************************************************************************/
//��������Ĺ����¼�
#pragma location="TASK_MUSIC_SEG"
void task_music_comm_event(void)
{
    comm_event();                           //���ù����¼�
    music_event();                          //������Ҫ���¼�
}

/*****************************************************************************
 * Module    : �����������
 *****************************************************************************/
//���ֲ�������
#pragma constseg="TASK_MUSIC_SEG_CONST"
IAR_CONST char str_show_taskmusic[] = "task_music\n";
#pragma constseg=default

#pragma location="TASK_MUSIC_SEG"
void task_music(void)
{
    u8 msg;
    printf(str_show_taskmusic);
    task_music_enter();
    DBG_TASK_MUSIC("Total: %d\n", t_msc.music_total);
    while (task_ctl.work_sta == TASK_MUSIC) {
#if IS_LCD_DISPLAY
        WATCHDOG_CLR();
        task_music_event();
        while(msg = get_msg()) {  // lcd��ʾ��ʱ̫���ȼ��д�����Ϣ�������������ʾ
            task_music_deal_msg(msg);
        }
        task_music_display();
#else
        msg = get_msg();
        WATCHDOG_CLR();
        task_music_event();
        task_music_deal_msg(msg);
        task_music_display();

#endif
    }
    task_music_exit();
}
#endif
