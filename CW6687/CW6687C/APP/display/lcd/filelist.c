/*****************************************************************************
 * Module    : Display
 * File      : filelist.c
 * Author    : Hanny
 * Function  : 文件导航列表，用于文件选择
 *****************************************************************************/
#include "include.h"
#if IS_LCD_DISPLAY
#include "task.h"
#include "listbox.h"
#include "ocx.h"

IAR_XDATA_A struct {
    u8 item_cdup_flag;                  //是否支持返回上一级菜单，1:支持返回上一级菜单，0：否
    u8 first_in;                        //第一次进入
    u16 index;                          //当前选项编号
    u16 file_total;                     //当前目录文件总数
    u16 dir_total;                      //当前目录文件夹总数
    u16 item_total;                     //当前目录的文件与文件夹总数
    u8 item_reserved;                   //保留项。有返回上一级时，会有一项保留项。最多只能保留一页
    u16 page_start;                     //当前页的起始位置
    u16 page_end;                       //当前页的结束位置
    u8 page0_ok;                        //Page 0 的Buffer是否有效
} t_flist @ "FLIST_XDATA";

#define FNAV_PAGE_0                 8
#define FNAV_LIST_PAGE              40          //缓存40项
#define FNAV_LIST_PAGE_PREV         10          //缓存向前10项，向后30项
#define FNAV_NAME_LEN               100


IAR_XDATA_A char flist_pagebuf[FNAV_LIST_PAGE][FNAV_NAME_LEN] @ "FLIST_BUF";
IAR_XDATA_A char page0_buf[FNAV_PAGE_0][FNAV_NAME_LEN] @ "FLIST_BUF";        //Page0单独开一个Buffer

#pragma constseg="FILE_LIST_CONST"
char IAR_CONST tbl_fnav_cdup[3][20] = {"返回上一级", "UP LEVEL","返回上一级"};
#pragma constseg=default

//获取列表内容
#pragma location="FILE_LIST_SEG"
char *filelist_getlist(u16 index, u8 cnt)
{
    u16 start, num, rest;
    u16 list_total = t_flist.item_total - t_flist.item_reserved;
    u8 refill_flag = 0;     //是否需要重填
    char *fnav_buf;
    //printf("index=%d; cnt=%d\n", index, cnt);
    //printf("page start=%d; end=%d\n", t_flist.page_start, t_flist.page_end);

    //检测当前的缓存是否有效
    if (t_flist.page_start <= t_flist.page_end) {
        if (index < t_flist.page_start || index + cnt - 1 > t_flist.page_end) {
            refill_flag = 1;                //不需要跨文件尾的情况
        }
    } else {
        if (index < t_flist.page_start && index + cnt - 1 > t_flist.page_end) {
            refill_flag = 1;                //需要跨文件尾的情况
        }
    }

    //重填缓存
    if (refill_flag) {
        fnav_buf = flist_pagebuf[0];
        if (list_total <= FNAV_LIST_PAGE) {
            //缓存够显示目录下的所有项，则全部缓存
            start = 0;
            num = list_total;
            t_flist.page_start = 0;
        } else {
            //缓存不够显示目录下的所有项，则填满缓存
            t_flist.page_start = (index > FNAV_LIST_PAGE_PREV) ? (index - FNAV_LIST_PAGE_PREV) : 0;
            start = t_flist.page_start;
            num = FNAV_LIST_PAGE;
            if (start + num > list_total) {                 //超过文件尾，循环回文件头
                rest = list_total - start;
                fnav_fill(fnav_buf, start, rest);           //需要跨文件尾的话，先填文件尾的一半
                fnav_buf += rest * FNAV_NAME_LEN;
                start = 0;
                num -= rest;
            }
        }
        t_flist.page_end = start + num - 1;
        fnav_fill(fnav_buf, start, num);                    //填充缓存
    }

    //找到正确条目位置，返回
    if (index < t_flist.page_start) {
        index += list_total;
    }
    index = index - t_flist.page_start;
    return flist_pagebuf[index];
}

//获取列表内容
#pragma location="FILE_LIST_SEG"
char *filelist_get(u16 index, u8 cnt)
{
    if (index < 4) {
        //需要增加保留项
        return page0_buf[index];                                //Page 0 特殊处理
    }
    return filelist_getlist(index - t_flist.item_reserved, cnt);  //其它Page 统一处理
}

//填弃Page0，必须与tbl_fnav_cdup放在同一个段
#pragma location="FILE_LIST_SEG"
void filelist_fill_page0(void)
{
    //填充Page 0
    if (t_flist.item_reserved) {
        //填充保留项
        memcpy(page0_buf[0], (char *)tbl_fnav_cdup[sys_ctl.lang_id], sizeof(tbl_fnav_cdup[0]));
        page0_buf[0][FILE_NAME_FLAG] = FB_GBK | FB_FOLDERUP;
    }
    memcpy(page0_buf[t_flist.item_reserved], filelist_getlist(0, FNAV_PAGE_0),
           (FNAV_PAGE_0-t_flist.item_reserved) * FNAV_NAME_LEN);
}

//列表框目录初始化
#pragma location="FILE_LIST_SEG_1"
void filelist_dir_init(void)
{
    //清空缓存
    t_flist.page_start = 0xffff;                            //使缓存页无效
    t_flist.page_end = 0xffff;

    t_flist.item_reserved = 0;                              //根目录不需要保留项
    if (t_flist.item_cdup_flag && get_scan_dept()) {
        t_flist.item_reserved = 1;                          //保留项
    }

    t_flist.file_total = 0;                                 //扫描、统计目录下的项目
    t_flist.dir_total = 0;
    t_flist.item_total = fnav_dir_scan();
    t_flist.dir_total = fnav_get_dirtotal();
    //printf("itemtot:%d,dir_tol:%d\n",t_flist.item_total,t_flist.dir_total);
    t_flist.index += t_flist.item_reserved;
    t_flist.item_total += t_flist.item_reserved;

    if (t_flist.first_in) {
        t_flist.first_in = 0;
        t_flist.index += t_flist.dir_total;                 //第一次，需要选定指定的文件
    }

    filelist_fill_page0();
}

//返回当前文件编号。未选择时，返回0xffff
#pragma location="FILE_LIST_SEG"
u16 filelist(u16 index, u8 mode)
{
    void (*event_func_bak)(void);
    bool (*msg_func_bak)(u8 msg);
    event_func_bak = ocx_ctl.event_func;
    msg_func_bak = ocx_ctl.msg_func;
#if IS_LCD_DISPLAY
    ocx_init(NULL, NULL);                   //屏蔽OCX需要响应的消息及事件
#endif
    t_flist.item_cdup_flag = (mode & FL_CDUP) ? 1 : 0;

    t_flist.index = fnav_init(index);               //扫描到该文件
    t_flist.first_in = 1;
//    file_navigation();
    //计算要显示光标所在位置
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
        //显示列表框
        index = listboxa(t_flist.index, t_flist.item_total, LB_NO_TITLE, FNAV_NAME_LEN, (u32)&filelist_get);
        t_flist.index = index;
        if (t_list.ok_flag == 0xfe) {
            break;                                  //长按，直接退出
        }
        if (t_list.ok_flag == 0x02) {               //删除
            if (index >= (t_flist.item_reserved + t_flist.dir_total)) {
                //判断选择为文件
                if (msgbox(STR_DELETE_CONFIRM, MB_YESNO, 0)) {
                    //确认删除对话框
                    index -= t_flist.item_reserved + t_flist.dir_total;
                    if (fnav_delete(index)) {          //获取编号进行删除
                        //重新设定当前的选中项
                        if (t_flist.index == t_flist.item_total - 1) {
                            t_flist.index--;            //最后一项
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
        //处理列表框的选择状况
        if (index == 0xffff) {
            //按下取消键
            index = fnav_cdup();
            if (index != 0xffff) {                   //返回上一层
                t_flist.index = index;
            } else {
                task_ctl.work_sta = TASK_EXIT;
                break;
            }
        } else if (index < t_flist.item_reserved) {
            //保留项区
            index = fnav_cdup();
            if (index != 0xffff) {                   //返回上一层
                t_flist.index = index;
            } else {
                t_flist.index = 0;
            }
        } else {
            //选择有效目录
            index -= t_flist.item_reserved;         //去除保留项
            if (index < t_flist.dir_total) {
                //选择目录
                if (fnav_cd(index)) {               //进入子目录
                    t_flist.index = 0;
                }
            } else {
                //选择文件
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
    ocx_init(event_func_bak, msg_func_bak);       //设置OCX需要响应的消息及事件
    memset(filename,0,100);
    return index;
}

#endif
