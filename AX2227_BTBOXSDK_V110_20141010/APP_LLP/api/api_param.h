/*****************************************************************************
 * Module    : API
 * File      : api_param.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : API参数管理
 *****************************************************************************/
#ifndef API_PARAM_H
#define API_PARAM_H

void spi_param_init(void);
__near_func u32 spi_param_addr(u8 addr);
void spi_param_write(void *buf, u8 addr, u16 len);
void spi_param_write8(u8 addr, u8 val);
void spi_param_write16(u8 addr, u16 val);
void spi_param_write32(u8 addr, u32 val);

#define param_init()                     spi_param_init()
#define param_write(buf, addr, len)      spi_param_write(buf, addr, len)
#define param_write8(addr, val)          spi_param_write8(addr, val)
#define param_write16(addr, val)         spi_param_write16(addr, val)
#define param_write32(addr, val)         spi_param_write32(addr, val)

#define param_read(buf, addr, len)       spi_read(buf, spi_param_addr(addr), len)
#define param_read8(addr)                spi_read8(spi_param_addr(addr))
#define param_read16(addr)               spi_read16(spi_param_addr(addr))
#define param_read32(addr)               spi_read32(spi_param_addr(addr))

#endif
