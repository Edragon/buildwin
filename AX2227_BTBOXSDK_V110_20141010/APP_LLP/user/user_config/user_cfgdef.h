/*****************************************************************************
 * Module    : User
 * File      : user_cfgdef.h
 * Author    : jingfa
 * Email     :
 * Function  : �û������б�
 *****************************************************************************/
#ifndef USER_CFGDEF_H
#define USER_CFGDEF_H


//���ط�װѡ��
#define PKG_LQFP48                      1       //LQFP48��װ    2227
#define PKG_SSOP24                      2       //SSOP24��װ    2226��1006
#define PKG_LQFP48_6680E                3       //LQFP48��װ    6680E��6681E
#define PKG_QFN48_6680M                 4       //QFN48��װ     6680M��6681M��6682M��6682E


//ϵͳʱ��Դѡ��
#define CRY_32K                         1       //32K����
#define CRY_12M                         2       //12M����
#define CRY_RC                          3       //�ڲ�RC(�޾��񷽰�)


//FMʱ��Ƶ��ѡ��
#define FMOSC_32K                       1       //32K
#define FMOSC_12M                       2       //12M


//ADKEY PORT��ѡ��
#define ADKEY_P33   0
#define ADKEY_P06   1
#define ADKEY_P04   2
#define ADKEY_P05   3
#define ADKEY_P30   4
#define ADKEY_P13   5


//FMģ��ѡ��
#define FM_RDA5807                      1
#define FM_BK1080                       2
#define FM_QN8035                       3
#define FM_QN8065                       4
#define FM_AX5111                       5       //ʹ��AX5111 FMģ��ʱ��Ŀǰֻ֧��ʹ��12M���񹩸�AX5111
#define FM_RTC6218                      6
#define FM_RTC6207                      7


//����ģ��ѡ��
#define BT6635                          0       //����6635(δ����)
#define BT6638                          1       //����6638(δ����)
#define BT6639                          2       //����6639


//ledseg������ѡ��
#define LEDSEG_5C7S_MHZ                 0       //5c7s����FMģʽ����ʾMHZ
#define LEDSEG_7P7S_FM                  1       //7������FMģʽ����ʾFM
#define LEDSEG_7P7S_MHZ                 2       //7������FMģʽ����ʾMHZ


//����ʾ
#define NO_THEME                        0       //����ʾ����

//ѡ��LCD���Ĳ�ͬģ��
#define LCD_THEME_DEFAULT               0x01    //LCDĬ������
#define LCD_THEME_ACT                   0x02    //LCD ACT����(������Ŀǰֻ����������ģʽ������ģʽδ����)

//ѡ��LCD�������Ĳ�ͬģ��
#define LCDSEG_THEME_DEFAULT            0x80    //������Ĭ������(δ����)

//ѡ��LED���Ĳ�ͬģ��
#define LEDSEG_THEME_DEFAULT            0xC0    //LED��Ĭ������


//¼���ļ�������ѡ��
#define SHORT_NAME                      1       //���ļ���
#define LONG_NAME                       2       //���ļ�����δ���ԣ�


#endif
