/*****************************************************************************
 * Module    : User
 * File      : user_spi.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : 用户的SPI操作的头定义
 *****************************************************************************/
#ifndef USER_SPI_H
#define USER_SPI_H

#define SPI_ERASE_BLOCK_BIT     4                               //4K管理
#define SPI_ERASE_BLOCK         (1 << SPI_ERASE_BLOCK_BIT)
#define SPI_ERASE_BLOCK_MASK    (SPI_ERASE_BLOCK - 1)

#define SPI_BITRATE             64                  //录音到SPIFlash的码率


void spi_music_init(void);
bool spi_music_open(u16 start_addr, u16 data_len);
void spi_music_set_end(void);
__near_func bool spi_music_read(void *buf);
bool spi_music_write(void *buf);
void spi_record_start(void);
u16 spi_music_start(void);
u16 spi_music_addr(void);
u16 spi_music_len(void);
u16 spi_music_end(void);
bool music_init_spi(u16 data_start, u16 data_len);

#endif
