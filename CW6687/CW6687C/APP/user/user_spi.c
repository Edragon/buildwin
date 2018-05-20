/*****************************************************************************
 * Module    : User
 * File      : user_spi.c
 * Author    : Hanny
 * Function  : �û���SPI��������Ҫ���ڸ�����Ŵ������ȹ���
 *****************************************************************************/
#include "include.h"
#include "user_spi.h"

struct {
    u16 start_addr;
    u16 data_len;

    u16 cur_addr;
    u16 end_addr;                       //����������������Ľ���������loop��
} spimsc_ctl;

//���ÿ�ʼ
void spi_music_init(void)
{
    spimsc_ctl.start_addr = 0;
    spimsc_ctl.data_len = 0;
    spimsc_ctl.cur_addr = 0;
    spimsc_ctl.end_addr = spi_get_datalen();
}

//���ÿ�ʼ
bool spi_music_open(u16 start_addr, u16 data_len)
{
    if (start_addr >= spimsc_ctl.end_addr) {
        spimsc_ctl.start_addr = 0;
        spimsc_ctl.data_len = 0;
        spimsc_ctl.cur_addr = 0;
        return false;
    }
    spimsc_ctl.start_addr = start_addr;
    spimsc_ctl.data_len = data_len;
    spimsc_ctl.cur_addr = start_addr;
    return true;
}

//���ý���
void spi_music_set_end(void)
{
    spimsc_ctl.cur_addr = spimsc_ctl.start_addr + spimsc_ctl.data_len;
}

//SPI Flash�����ļ����ӿڣ�ÿ�ζ�ȡһ��512Byte
#pragma location="FAT_COM_SEG"
__near_func bool spi_music_read(void *buf)
{
    //printf("spird: %04x\n", spimsc_ctl.cur_addr);
    if (spimsc_ctl.data_len == 0) {
        return false;
    }
    if (!spi_read_data(buf, spimsc_ctl.cur_addr)) {
        return false;
    }
    spimsc_ctl.data_len -= 2;
    spimsc_ctl.cur_addr += 2;
    if (spimsc_ctl.cur_addr >= spimsc_ctl.end_addr) {
        spimsc_ctl.cur_addr = 0;                        //д��������ѭ����0
    }
    return true;
}

//SPI Flash����seek�ӿ�
#pragma location="FAT_COM_SEG"
__near_func bool spi_music_seek(u8 mode, u32 offset)
{
    if (mode == F_SEEK_ORG) {
        spimsc_ctl.cur_addr = spimsc_ctl.start_addr + (u16)offset;
    } else {
        spimsc_ctl.cur_addr += (u16)offset;
    }
    return true;
}

#pragma location="SPI_WRITE_API"
bool spi_music_write(void *buf)
{
    //printf("spiwr: %04x\n", spimsc_ctl.cur_addr);
    if ((spimsc_ctl.cur_addr & REC_SPI_ERASE_BLOCK_MASK) == 0) {
        if (!spi_erase_data(spimsc_ctl.cur_addr)) {
            return false;
        }
    }
    if (!spi_write_data(buf, spimsc_ctl.cur_addr)) {
        return false;
    }

    //����д��������ܳ���
    spimsc_ctl.data_len += 2;
    if (spimsc_ctl.data_len >= spimsc_ctl.end_addr) {
        //�ܳ��ȹ�������ȡ��ͷ��һ��BLOCK����

        spimsc_ctl.data_len -= REC_SPI_ERASE_BLOCK;
        spimsc_ctl.start_addr += REC_SPI_ERASE_BLOCK;
       // printf("len:%d\n",spimsc_ctl.data_len);
        if (spimsc_ctl.start_addr >= spimsc_ctl.end_addr) {
            spimsc_ctl.start_addr = 0;                  //д��������ѭ����0
        }
    }

    //���Ƶ�ǰ��дBufferָ��
    spimsc_ctl.cur_addr += 2;
    if (spimsc_ctl.cur_addr >= spimsc_ctl.end_addr) {
        spimsc_ctl.cur_addr = 0;                        //д��������ѭ����0
    }
   // printf("spiwr:%d\n", spimsc_ctl.cur_addr);
    return true;
}

//���뵱ǰλ�ã���Ϊ�µ�¼����ʼλ��
#pragma location="TASK_TOMCAT_INIT"
void spi_record_start(void)
{
    u16 repeat_addr;
    repeat_addr = ((spimsc_ctl.cur_addr + REC_SPI_ERASE_BLOCK_MASK) >> REC_SPI_ERASE_BIT) << REC_SPI_ERASE_BIT;
    spi_music_open(repeat_addr, 0);
}

//��ȡMUSIC����̨
#pragma location="TASK_TOMCAT_INIT"
u16 spi_music_start(void)
{
    return spimsc_ctl.start_addr;
}

//��ȡMUSIC�ĵ�ǰ
#pragma location="TASK_TOMCAT_INIT"
u16 spi_music_addr(void)
{
    return spimsc_ctl.cur_addr;
}

//��ȡMUSIC�ĳ���
#pragma location="TASK_TOMCAT_INIT"
u16 spi_music_len(void)
{
    return spimsc_ctl.data_len;
}

#pragma location="TASK_TOMCAT_INIT"
u16 spi_music_end(void)
{
    return spimsc_ctl.end_addr;
}

//��ʼ����music_num�׸�
#pragma location="MUSIC_SEG"
bool music_init_spi(u16 data_start, u16 data_len)
{
#if DEBUG_TASK_TAPE
    printf("music_init_spi\n");
#endif
    if (!spi_music_open(data_start, data_len)) {
        return false;
    }
#if DEBUG_TASK_TAPE
    printf("mp3_init\n");
#endif
    return mp3_init_spi();
}
