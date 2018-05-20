/*****************************************************************************
 * Module    : API
 * File      : api_spi.h
 * Author    : Hanny
 * Function  : SPIFlash 的操作接口
 *****************************************************************************/
#ifndef API_SPI_H
#define API_SPI_H

__near_func void spi_read(void *buf, u32 addr, u16 len);        //SPIFlash 读接口
__near_func u8 spi_read8(u32 flashaddress);                     //读1Byte
__near_func u16 spi_read16(u32 flashaddress);                   //按小端读2Byte
__near_func u32 spi_read32(u32 flashaddress);                   //按小端读4Byte
__near_func void spi_write(u8 *buf, u32 addr, u16 len);
__near_func void spi_erase(u8 cmd, u32 addr);
void spi_write_page(void *buf, u16 addr);          	            //写256byte
void spi_param_factory(u8 page);                             	//恢复出厂设置

u16 spi_get_datalen(void);                                      //获取DATA区大小
__near_func bool spi_read_data(void *buf, u16 addr);            //DATA区读接口，addr = 0 ~ datalen
bool spi_write_data(void *buf, u16 addr);           			//DATA区写接口
bool spi_erase_data(u16 addr);                      			//DATA区擦除接口

void spi_phonebook_erase(void);                                     //电话本信息参数区擦除
void spi_phonebook_write(u8 *phonebook_buf, u16 addr_offset);       //电话本信息写接口，一次写64Byte
void spi_phonebook_read(u8 *phonebook_buf, u16 addr_offset);        //电话本信息读接口，一次读64Byte
void spi_phonebook_phonenum_read(u8 *phonenum_buf, u16 addr_offset);//电话本信息之电话号码读接口，一次读24Byte

#endif
