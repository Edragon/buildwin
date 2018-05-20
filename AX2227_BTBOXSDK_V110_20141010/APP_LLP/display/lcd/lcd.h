/*****************************************************************************
 * Module    : Display
 * File      : lcd.h
 * Author    : Hanny
 * Email     : coldney@yahoo.com.cn
 * Function  : LCD����
*****************************************************************************/
#ifndef LCD_H
#define LCD_H
#define FRONT_STRUCT        8
#define ICON_FILE_HEAD      16

#define PAGE_MAX            8               //��������
#define COLUMN_MAX          128             //��������
#define SCREEN_WIDTH        (COLUMN_MAX)
#define SCREEN_HEIGHT       (PAGE_MAX*8)


#define CHAR_WIDTH_MAX      32              //�ַ������
#define MINI_SIZE           256             //С����ģʽ��BufferSize

#define LLP_STRING_MAX      63              //Flash���ִ�����󳤶�

//�Դ�λ�ã�MINIģʽΪ256Byte��FULLģʽΪ1024Byte
#define LCD_BUFF_ADDR                   0x2900

enum {
    LCD_FULL_MODE,      //1K�������ȫģʽ
    LCD_MINI_MODE,      //256ByteС����ģʽ
};

//LCD command define
#define LCD_ON                  0xaf
#define LCD_OFF                 0xae
#define LCD_ALL_ON              0xa5
#define LCD_ALL_NORMAL          0xa4
#define LCD_START_LINE          0x40
#define LCD_PAGE_ADDR           0xb0
#define LCD_COLUMN_HIGH         0x10
#define LCD_COLUMN_LOW          0x00
#define LCD_ADC_SEL             0xa0  //bit0 0=normal,1=reverse
#define LCD_DISP_MODE           0xa6  //bit0 0=normal,1=reverse
#define LCD_BIAS_SET            0xa2
#define LCD_END                 0xee
#define LCD_SOFT_RST            0xe2
#define LCD_COMMON_MODE         0xc8  //bit3 0=normal,1=reverse
#define LCD_POWER_MODE          0x2f
#define LCD_V0_RATIO            0x20
#define LCD_ELE_VOLUME          0x81
#define LCD_STATIC_IND_ON       0xad
#define LCD_STATIC_IND_OFF      0xac
#define LCD_PAGE_BLINK          0xd5
#define LCD_DRIV_MODE           0xd2
#define LCD_NOP                 0xe3
#define LCD_OSC_SET             0xe4
/*
#define DIS_SPI_IE              IE1 &= ~(1<<3)
#define EN_SPI_IE               IE1 |= (1<<3)

#define Update_Display()        EN_SPI_IE
*/

typedef struct
{
    u16 sign;			//=0XFEFE����ʾ���Ƕ�Ӧ����ʼ��ַ�ṹ
    u16 lang_id;			//���Ե�ID�ţ���Ӧfirmware�����и����Ա��
    u32 address;			//��ʼ��ַ�����ڲ���FRONTFIRSTSTRUCT
//    u8 reserve;			//����
}FontTable;

//Font library segment head
typedef struct
{
	u16 StartVaule;			// ��ʼ���ر���ֵ
	u16 EndVaule;				//�������ر���ֵ
	u32 startaddress;			//��ʼ��ַ
} FontSegment;

extern char lcd_buf[PAGE_MAX * COLUMN_MAX];
extern u8 disp_freq_show_buf[16];

//����LCD Buffer��λ��
void set_lcd_buf(u8 mode);

//clear and update mode, for disp_clear disp_update
void lcd_init(void);
void lcd_quick_init(void);
void lcd_contrast(u8 value);

//��̨��ͼ��������
void disp_clear(void);
void disp_invert(u8 x, u8 line, u8 width, u8 line_cnt);
void disp_num(u16 num, u8 x, u8 line, u8 mode);
void disp_small_time(u8 min, u8 sec, u8 x, u8 line);
void disp_medium_time1(u8 min, u8 sec, u8 x, u8 line);
void disp_medium_time(u8 hour, u8 min, u8 sec, u8 x, u8 line);
void disp_freq_line(u8 line);
char *disp_text(char *str, s16 x, u8 line, u8 mode);
char *disp_text_u(char *str, s16 x, u8 line, u8 mode);

#if SLIDER_EN
char *disp_addslider_text(char *str, s16 x, u8 line, u8 mode);
char *disp_addsleder_text_u(char *str, s16 x, u8 line, u8 mode);
#endif

void disp_draw(u16 imageindex, u8 x, u8 line);
void disp_update(void);

void lcd_update(u8 lcd_width, u8 lcd_height, u16 lcd_buff_add, u8 len);

void line_clear(void);
void line_update(u8 line);
void line_update_clear(u8 line);

void draw_clear(void);
void draw_img(u16 imageindex, u8 x, u8 line);

void progressbar(u8 percent, u8 line);
void draw_wait(void);
void disp_menu(u16 menuindex, u8 x, s16 mov_x, u8 line, u8 mode);
void disp_language_numget(void);

u8 utf8_char_size(char code);

#define ME_CENTER      0x01         //�Ƿ���ʾ����
#define ME_LINES       0x02         //�Ƿ���ʾ����
#define ME_MSG         0x04          //�Ƿ�Ϊ��Ϣ��
//ǰ̨��ͼ��������

#define DN_ZERO         0x80    //�����0
#define DN_RIGHT        0x40    //�����
#define DN_BIG          0x20    //�����
#define DN_SMALL        0x10    //С����
#define DN_SSMALL       0x08    //��С����
#define DN_BOUND_MASK   0x07    //��3bit��ʾ��ֵ�����λ���޶�




/*****************************�ַ���ʾ���******************************/
enum{
    TYPE_GBK,
    TYPE_UNICODE,
    TYPE_UTF8,
};
#endif
