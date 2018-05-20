/*****************************************************************************
 * Module    : Display
 * File      : filelist.c
 * Author    : Hanny
 * Function  : �ļ������б������ļ�ѡ��
 *****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY
#include "task.h"
#include "listbox.h"
#include "ocx.h"

IAR_XDATA_A struct {
    u8 item_cdup_flag;                  //�Ƿ�֧�ַ�����һ���˵���1:֧�ַ�����һ���˵���0����
    u8 first_in;                        //��һ�ν���
    u16 index;                          //��ǰѡ����
    u16 file_total;                     //��ǰĿ¼�ļ�����
    u16 dir_total;                      //��ǰĿ¼�ļ�������
    u16 item_total;                     //��ǰĿ¼���ļ����ļ�������
    u8 item_reserved;                   //������з�����һ��ʱ������һ�������ֻ�ܱ���һҳ
    u16 page_start;                     //��ǰҳ����ʼλ��
    u16 page_end;                       //��ǰҳ�Ľ���λ��
    u8 page0_ok;                        //Page 0 ��Buffer�Ƿ���Ч
} t_flist @ "FLIST_XDATA";

#define FNAV_PAGE_0                 8
#define FNAV_LIST_PAGE              40          //����40��
#define FNAV_LIST_PAGE_PREV         10          //������ǰ10����30��
#define FNAV_NAME_LEN               100


IAR_XDATA_A char flist_pagebuf[FNAV_LIST_PAGE][FNAV_NAME_LEN] @ "FLIST_BUF";
IAR_XDATA_A char page0_buf[FNAV_PAGE_0][FNAV_NAME_LEN] @ "FLIST_BUF";        //Page0������һ��Buffer

#pragma constseg="FILE_LIST_CONST"
char IAR_CONST tbl_fnav_cdup[3][20] = {"������һ��", "UP LEVEL","������һ��"};
#pragma constseg=default

//��ȡ�б�����
#pragma location="FILE_LIST_SEG"
char *filelist_getlist(u16 index, u8 cnt)
{
    u16 start, num, rest;
    u16 list_total = t_flist.item_total - t_flist.item_reserved;
    u8 refill_flag = 0;     //�Ƿ���Ҫ����
    char *fnav_buf;
    //printf("index=%d; cnt=%d\n", index, cnt);
    //printf("page start=%d; end=%d\n", t_flist.page_start, t_flist.page_end);

    //��⵱ǰ�Ļ����Ƿ���Ч
    if (t_flist.page_start <= t_flist.page_end) {
        if (index < t_flist.page_start || index + cnt - 1 > t_flist.page_end) {
            refill_flag = 1;                //����Ҫ���ļ�β�����
        }
    } else {
        if (index < t_flist.page_start && index + cnt - 1 > t_flist.page_end) {
            refill_flag = 1;                //��Ҫ���ļ�β�����
        }
    }

    //�����
    if (refill_flag) {
        fnav_buf = flist_pagebuf[0];
        if (list_total <= FNAV_LIST_PAGE) {
            //���湻��ʾĿ¼�µ��������ȫ������
            start = 0;
            num = list_total;
            t_flist.page_start = 0;
        } else {
            //���治����ʾĿ¼�µ����������������
            t_flist.page_start = (index > FNAV_LIST_PAGE_PREV) ? (index - FNAV_LIST_PAGE_PREV) : 0;
            start = t_flist.page_start;
            num = FNAV_LIST_PAGE;
            if (start + num > list_total) {                 //�����ļ�β��ѭ�����ļ�ͷ
                rest = list_total - start;
                fnav_fill(fnav_buf, start, rest);           //��Ҫ���ļ�β�Ļ��������ļ�β��һ��
                fnav_buf += rest * FNAV_NAME_LEN;
                start = 0;
                num -= rest;
            }
        }
        t_flist.page_end = start + num - 1;
        fnav_fill(fnav_buf, start, num);                    //��仺��
    }

    //�ҵ���ȷ��Ŀλ�ã�����
    if (index < t_flist.page_start) {
        index += list_total;
    }
    index = index - t_flist.page_start;
    return flist_pagebuf[index];
}

//��ȡ�б�����
#pragma location="FILE_LIST_SEG"
char *filelist_get(u16 index, u8 cnt)
{
    if (index < 4) {
        //��Ҫ���ӱ�����
        return page0_buf[index];                                //Page 0 ���⴦��
    }
    return filelist_getlist(index - t_flist.item_reserved, cnt);  //����Page ͳһ����
}

//����Page0��������tbl_fnav_cdup����ͬһ����
#pragma location="FILE_LIST_SEG"
void filelist_fill_page0(void)
{
    //���Page 0
    if (t_flist.item_reserved) {
        //��䱣����
        memcpy(page0_buf[0], (char *)tbl_fnav_cdup[sys_ctl.lang_id], sizeof(tbl_fnav_cdup[0]));
        page0_buf[0][FILE_NAME_FLAG] = FB_GBK | FB_FOLDERUP;
    }
    memcpy(page0_buf[t_flist.item_reserved], filelist_getlist(0, FNAV_PAGE_0),
           (FNAV_PAGE_0-t_flist.item_reserved) * FNAV_NAME_LEN);
}

//�б��Ŀ¼��ʼ��
#pragma location="FILE_LIST_SEG_1"
void filelist_dir_init(void)
{
    //��ջ���
    t_flist.page_start = 0xffff;                            //ʹ����ҳ��Ч
    t_flist.page_end = 0xffff;

    t_flist.item_reserved = 0;                              //��Ŀ¼����Ҫ������
    if (t_flist.item_cdup_flag && get_scan_dept()) {
        t_flist.item_reserved = 1;                          //������
    }

    t_flist.file_total = 0;                                 //ɨ�衢ͳ��Ŀ¼�µ���Ŀ
    t_flist.dir_total = 0;
    t_flist.item_total = fnav_dir_scan();
    t_flist.dir_total = fnav_get_dirtotal();
    //printf("itemtot:%d,dir_tol:%d\n",t_flist.item_total,t_flist.dir_total);
    t_flist.index += t_flist.item_reserved;
    t_flist.item_total += t_flist.item_reserved;

    if (t_flist.first_in) {
        t_flist.first_in = 0;
        t_flist.index += t_flist.dir_total;                 //��һ�Σ���Ҫѡ��ָ�����ļ�
    }

    filelist_fill_page0();
}

//���ص�ǰ�ļ���š�δѡ��ʱ������0xffff
#pragma location="FILE_LIST_SEG"
u16 filelist(u16 index, u8 mode)
{
    void (*event_func_bak)(void);
    bool (*msg_func_bak)(u8 msg);
    event_func_bak = ocx_ctl.event_func;
    msg_func_bak = ocx_ctl.msg_func;
#if IS_LCD_DISPLAY
    ocx_init(NULL, NULL);                   //����OCX��Ҫ��Ӧ����Ϣ���¼�
#endif
    t_flist.item_cdup_flag = (mode & FL_CDUP) ? 1 : 0;

    t_flist.index = fnav_init(index);               //ɨ�赽���ļ�
    t_flist.first_in = 1;
//    file_navigation();
    //����Ҫ��ʾ�������λ��
//    t_flist.index = index - file_naviga.first_file_num + file_naviga.folder_total + t_flist.title_back_flag;
//    if(t_flist.index >= FNAV_LIST_PAGE){
//        file_naviga.first_in = 0;
//    }
    while (1) {
        filelist_dir_init();
        if( !t_flist.dir_total && !t_flist.item_total)
        {
            task_ctl.work_sta = TASK_EXIT;
            if(task_ctl.work_sta == TASK_MUSIC)
            {
                msgbox(STR_NO_MUSIC, NULL, MSGBOX_TIME);
            }
            else if(task_ctl.work_sta == TASK_EBOOK)
            {
                msgbox(STR_NO_EBOOK, NULL, MSGBOX_TIME);
            }
            else if(task_ctl.work_sta == TASK_VOICE)
            {
                msgbox(STR_NO_VOICE, NULL, MSGBOX_TIME);
            }

            break;
        }
        //��ʾ�б��
        index = listboxa(t_flist.index, t_flist.item_total, LB_NO_TITLE, FNAV_NAME_LEN, (u32)&filelist_get);
        t_flist.index = index;
        if (t_list.ok_flag == 0xfe) {
            break;                                  //������ֱ���˳�
        }
        if (t_list.ok_flag == 0x02) {               //ɾ��
            if (index >= (t_flist.item_reserved + t_flist.dir_total)) {
                //�ж�ѡ��Ϊ�ļ�
                if (msgbox(STR_DELETE_CONFIRM, MB_YESNO, 0)) {
                    //ȷ��ɾ���Ի���
                    index -= t_flist.item_reserved + t_flist.dir_total;
                    if (fnav_delete(index)) {          //��ȡ��Ž���ɾ��
                        //�����趨��ǰ��ѡ����
                        if (t_flist.index == t_flist.item_total - 1) {
                            t_flist.index--;            //���һ��
                        }
                    } else {
                        msgbox(STR_DELETE_FAIL, NULL, MSGBOX_TIME);
                    }
                }
            }
            if (t_flist.index > t_flist.item_reserved) {
                t_flist.index -= t_flist.item_reserved;
            } else {
                t_flist.index = 0;
            }
            continue;
        }
        //�����б���ѡ��״��
        if (index == 0xffff) {
            //����ȡ����
            index = fnav_cdup();
            if (index != 0xffff) {                   //������һ��
                t_flist.index = index;
            } else {
                task_ctl.work_sta = TASK_EXIT;
                break;
            }
        } else if (index < t_flist.item_reserved) {
            //��������
            index = fnav_cdup();
            if (index != 0xffff) {                   //������һ��
                t_flist.index = index;
            } else {
                t_flist.index = 0;
            }
        } else {
            //ѡ����ЧĿ¼
            index -= t_flist.item_reserved;         //ȥ��������
            if (index < t_flist.dir_total) {
                //ѡ��Ŀ¼
                if (fnav_cd(index)) {               //������Ŀ¼
                    t_flist.index = 0;
                }
            } else {
                //ѡ���ļ�
                index -= t_flist.dir_total;
                break;
            }
        }
    }
    if (t_list.ok_flag == 0xfe) {
        index = 0;
    } else {
        index = fnav_get_index(index);
    }
    fnav_exit();
    ocx_init(event_func_bak, msg_func_bak);       //����OCX��Ҫ��Ӧ����Ϣ���¼�
    memset(filename,0,100);
    return index;
}

#endif
