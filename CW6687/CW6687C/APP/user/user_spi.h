/*****************************************************************************
 * Module    : User
 * File      : user_spi.h
 * Author    : Hanny
 * Function  : �û���SPI������ͷ����
 *****************************************************************************/
#ifndef USER_SPI_H
#define USER_SPI_H

#define     ERASBLK		0xD8	//0x20
#define     ERASSEC     0x20


#define FW_ERASE_CMD   ERASBLK       //������������ʹ��64k��������
#if(FW_ERASE_CMD == ERASBLK)
#define FW_SPI_ERASE_BIT     8                               //64K����
#else
#define FW_SPI_ERASE_BIT     4                               //4K����
#endif

#define FW_SPI_ERASE_BLOCK         (1 << FW_SPI_ERASE_BIT)
#define FW_SPI_ERASE_BLOCK_MASK    (FW_SPI_ERASE_BLOCK - 1)


#define REC_ERASE_CMD   ERASSEC       //¼����spiʹ��4k��������
#if(REC_ERASE_CMD == ERASBLK)
#define REC_SPI_ERASE_BIT     8                               //64K����
#else
#define REC_SPI_ERASE_BIT     4                               //4K����
#endif

#define REC_SPI_ERASE_BLOCK         (1 << REC_SPI_ERASE_BIT)
#define REC_SPI_ERASE_BLOCK_MASK    (REC_SPI_ERASE_BLOCK - 1)


#define SPI_BITRATE             64                  //¼����SPIFlash������


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
void spctl_init(void);

#endif
