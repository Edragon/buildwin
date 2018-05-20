/*****************************************************************************
 * Module    : API
 * File      : api_spi.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : SPIFlash 的操作接口
 *****************************************************************************/
#ifndef API_SPI_H
#define API_SPI_H

extern IAR_BIT_A bool spi_critical;

__near_func void spi_read(void *buf, u32 addr, u16 len);      //SPIFlash 读接口
__near_func u8 spi_read8(u32 flashaddress);                   //读1Byte
__near_func u16 spi_read16(u32 flashaddress);                 //按小端读2Byte
__near_func u32 spi_read32(u32 flashaddress);                 //按小端读4Byte
void spi_write(void *buf, u16 addr);
void spi_erase(u16 addr);

void spi_param_factory(void);                                   //恢复出厂设置

u16 spi_get_datalen(void);                                      //获取DATA区大小
__near_func bool spi_read_data(void *buf, u16 addr);            //DATA区读接口，addr = 0 ~ datalen
bool spi_write_data(void *buf, u16 addr);                       //DATA区写接口
bool spi_erase_data(u16 addr);                                  //DATA区擦除接口


void spi_bt_paired_write_2(void *buf,u8 addr,u16 len);          //写配对信息，定义ALIGN_COD时使用
bool spi_bt_paired_read_2(void *buf,u8 addr,u16 len);           //读配对信息，定义ALIGN_COD时使用
bool spi_bt_rfinfo_read_2(u8 *buf);                             //读RF信息，定义ALIGN_COD时使用
u32 spi_bt_get_addr_2(void);                                    //获取蓝牙参数的Block地址，定义ALIGN_COD时使用

#endif
