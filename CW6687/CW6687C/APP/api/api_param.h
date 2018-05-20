/*****************************************************************************
 * Module    : API
 * File      : api_param.h
 * Author    : Hanny
 * Function  : API��������
 *****************************************************************************/
#ifndef API_PARAM_H
#define API_PARAM_H

//һ��2��PAGE����������֧��4��Page������ÿ���������ʹ��252 byte
#define PARAM_PAGE                      2
enum {
    PARAM_SYS,      //Page 0: ϵͳ����
    PARAM_BT,       //Page 1: ����������Ϣ
};

void spi_param_init(u8 page);
__near_func u32 spi_param_addr(u8 page, u8 addr);

//������SPI_PARAM_DATA�����ռ���Ϊbufʱ��ֻ֧��writepage
void spi_param_writepage(u8 page, void *buf);
void spi_param_write(u8 page, void *buf, u8 addr, u8 len);
void spi_param_write8(u8 page, u8 addr, u8 val);
void spi_param_write16(u8 page, u8 addr, u16 val);
void spi_param_write32(u8 page, u8 addr, u32 val);

#define param_init()                        spi_param_init(PARAM_PAGE)

/*****************************************************************************
 * Module    : �����������ò�������ȡ����Ҫ���������豸������ַ��
 *****************************************************************************/
bool sysinfo_load(u8 *buf, u8 addr, u8 len);

/*****************************************************************************
 * Module    : ϵͳ������غ���
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
 * Module    : ����������Ϣ��غ���
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
 * Module    : ���ݾ�ʽ�˵��������غ���
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
