;*****************************************************************************
;* Module    : HexScript
;* File      : hexscript.hs
;* Author    : Hanny
;* Email     : coldney@yahoo.com.cn
;* Function  : �����������ص�bin�ļ����޸ĸ��ļ�ʱ���������ȫ����
;*****************************************************************************
load(hex, APP_LLP.a51);                         ����
map(0, 0, 4000);                                ӳ��(0-3fff) -> (0-3fff)
set(out, 16, le);                               �������λ��Ϊ16BIT��С��
save(hex, APP_LLP.hex);                         ���Hex
mapx(11800, 2000, 800, 10000, 800, 3d);         ӳ��0x30��Bank
map(11000, 20800, 800);                         ��������Ĺ�����
map(864800, 21000, 800);                        DCODE4 ����SCO
map(854800, 21800, 1800);                       DCODE3 ����SBC
map(844000, 23000, 1800);                       DCODE2 FM
map(834000, 24800, 1800);                       DCODE1 USB�豸
map(824000, 26000, 800);                        DCODE0 LOUD SPEAKER
map(874000, 26800, 800);                        DCODE4 �ļ�ϵͳ
set(out, 8, le);
save(bin, APP_LLP.bin);

clear(out);
map(FF0000, 0, 20);
save(bin, makecfg.bin);