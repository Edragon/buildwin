/*****************************************************************************
 * Module    : API
 * File      : api_param.h
 * Author    : Hanny
 * Function  : API参数管理
 *****************************************************************************/
#ifndef API_PARAM_H
#define API_PARAM_H

//一共2个PAGE参数。最多可支持4个Page参数。每个参数最多使用252 byte
#define PARAM_PAGE                      2
enum {
    PARAM_SYS,      //Page 0: 系统参数
    PARAM_BT,       //Page 1: 蓝牙回连信息
};

void spi_param_init(u8 page);
__near_func u32 spi_param_addr(u8 page, u8 addr);

//当复用SPI_PARAM_DATA变量空间做为buf时，只支持writepage
void spi_param_writepage(u8 page, void *buf);
void spi_param_write(u8 page, void *buf, u8 addr, u8 len);
void spi_param_write8(u8 page, u8 addr, u8 val);
void spi_param_write16(u8 page, u8 addr, u16 val);
void spi_param_write32(u8 page, u8 addr, u32 val);

#define param_init()                        spi_param_init(PARAM_PAGE)

/*****************************************************************************
 * Module    : 量产工具配置参数区获取。主要包括蓝牙设备名、地址等
 *****************************************************************************/
bool sysinfo_load(u8 *buf, u8 addr, u8 len);

/*****************************************************************************
 * Module    : 系统参数相关函数
 *****************************************************************************/
#define sys_param_writepage(buf)            spi_param_writepage(PARAM_SYS, buf)
#define sys_param_write(buf, addr, len)     spi_param_write(PARAM_SYS, buf, addr, len)
#define sys_param_write8(addr, val)         spi_param_write8(PARAM_SYS, addr, val)
#define sys_param_write16(addr, val)        spi_param_write16(PARAM_SYS, addr, val)
#define sys_param_write32(addr, val)        spi_param_write32(PARAM_SYS, addr, val)

#define sys_param_read(buf, addr, len)      spi_read(buf, spi_param_addr(PARAM_SYS, addr), len)
#define sys_param_read8(addr)               spi_read8(spi_param_addr(PARAM_SYS, addr))
#define sys_param_read16(addr)              spi_read16(spi_param_addr(PARAM_SYS, addr))
#define sys_param_read32(addr)              spi_read32(spi_param_addr(PARAM_SYS, addr))


/*****************************************************************************
 * Module    : 蓝牙回连信息相关函数
 *****************************************************************************/
#define bt_param_writepage(buf)             spi_param_writepage(PARAM_BT, buf)
#define bt_param_write(buf, addr, len)      spi_param_write(PARAM_BT, buf, addr, len)
#define bt_param_write8(addr, val)          spi_param_write8(PARAM_BT, addr, val)
#define bt_param_write16(addr, val)         spi_param_write16(PARAM_BT, addr, val)
#define bt_param_write32(addr, val)         spi_param_write32(PARAM_BT, addr, val)

#define bt_param_read(buf, addr, len)       spi_read(buf, spi_param_addr(PARAM_BT, addr), len)
#define bt_param_read8(addr)                spi_read8(spi_param_addr(PARAM_BT, addr))
#define bt_param_read16(addr)               spi_read16(spi_param_addr(PARAM_BT, addr))
#define bt_param_read32(addr)               spi_read32(spi_param_addr(PARAM_BT, addr))

/*****************************************************************************
 * Module    : 兼容旧式菜单所需的相关函数
 *****************************************************************************/
//#define param_init()                     spi_param_init()
#define param_write(buf, addr, len)      spi_param_write(buf, addr, len)
#define param_write8(addr, val)          spi_param_write8(PARAM_SYS, addr, val)
#define param_write16(addr, val)         spi_param_write16(PARAM_SYS, addr, val)
#define param_write32(addr, val)         spi_param_write32(PARAM_SYS, addr, val)

#define param_read(buf, addr, len)       spi_read(buf, spi_param_addr(PARAM_SYS,a ddr), len)
#define param_read8(addr)                spi_read8(spi_param_addr(PARAM_SYS, addr))
#define param_read16(addr)               spi_read16(spi_param_addr(PARAM_SYS, addr))
#define param_read32(addr)               spi_read32(spi_param_addr(PARAM_SYS, addr))

#endif
