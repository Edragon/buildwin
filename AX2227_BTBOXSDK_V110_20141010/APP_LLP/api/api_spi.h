/*****************************************************************************
 * Module    : API
 * File      : api_spi.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : SPIFlash �Ĳ����ӿ�
 *****************************************************************************/
#ifndef API_SPI_H
#define API_SPI_H

extern IAR_BIT_A bool spi_critical;

__near_func void spi_read(void *buf, u32 addr, u16 len);      //SPIFlash ���ӿ�
__near_func u8 spi_read8(u32 flashaddress);                   //��1Byte
__near_func u16 spi_read16(u32 flashaddress);                 //��С�˶�2Byte
__near_func u32 spi_read32(u32 flashaddress);                 //��С�˶�4Byte
void spi_write(void *buf, u16 addr);
void spi_erase(u16 addr);

void spi_param_factory(void);                                   //�ָ���������

u16 spi_get_datalen(void);                                      //��ȡDATA����С
__near_func bool spi_read_data(void *buf, u16 addr);            //DATA�����ӿڣ�addr = 0 ~ datalen
bool spi_write_data(void *buf, u16 addr);                       //DATA��д�ӿ�
bool spi_erase_data(u16 addr);                                  //DATA�������ӿ�


void spi_bt_paired_write_2(void *buf,u8 addr,u16 len);          //д�����Ϣ������ALIGN_CODʱʹ��
bool spi_bt_paired_read_2(void *buf,u8 addr,u16 len);           //�������Ϣ������ALIGN_CODʱʹ��
bool spi_bt_rfinfo_read_2(u8 *buf);                             //��RF��Ϣ������ALIGN_CODʱʹ��
u32 spi_bt_get_addr_2(void);                                    //��ȡ����������Block��ַ������ALIGN_CODʱʹ��

#endif
