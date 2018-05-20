/*****************************************************************************
 * Module    : API
 * File      : api_spi.h
 * Author    : Hanny
 * Function  : SPIFlash �Ĳ����ӿ�
 *****************************************************************************/
#ifndef API_SPI_H
#define API_SPI_H

__near_func void spi_read(void *buf, u32 addr, u16 len);        //SPIFlash ���ӿ�
__near_func u8 spi_read8(u32 flashaddress);                     //��1Byte
__near_func u16 spi_read16(u32 flashaddress);                   //��С�˶�2Byte
__near_func u32 spi_read32(u32 flashaddress);                   //��С�˶�4Byte
__near_func void spi_write(u8 *buf, u32 addr, u16 len);
__near_func void spi_erase(u8 cmd, u32 addr);
void spi_write_page(void *buf, u16 addr);          	            //д256byte
void spi_param_factory(u8 page);                             	//�ָ���������

u16 spi_get_datalen(void);                                      //��ȡDATA����С
__near_func bool spi_read_data(void *buf, u16 addr);            //DATA�����ӿڣ�addr = 0 ~ datalen
bool spi_write_data(void *buf, u16 addr);           			//DATA��д�ӿ�
bool spi_erase_data(u16 addr);                      			//DATA�������ӿ�

void spi_phonebook_erase(void);                                     //�绰����Ϣ����������
void spi_phonebook_write(u8 *phonebook_buf, u16 addr_offset);       //�绰����Ϣд�ӿڣ�һ��д64Byte
void spi_phonebook_read(u8 *phonebook_buf, u16 addr_offset);        //�绰����Ϣ���ӿڣ�һ�ζ�64Byte
void spi_phonebook_phonenum_read(u8 *phonenum_buf, u16 addr_offset);//�绰����Ϣ֮�绰������ӿڣ�һ�ζ�24Byte

#endif
