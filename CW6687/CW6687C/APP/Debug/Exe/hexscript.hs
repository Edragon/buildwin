;*****************************************************************************
;* Module    : HexScript
;* File      : hexscript.hs
;* Author    : Hanny
;* Function  : �����������ص�bin�ļ����޸ĸ��ļ�ʱ���������ȫ����
;*****************************************************************************
load(hex, APP.a51);                             ����
map(2000, 0, 1E00);                             ӳ��(2000-3DFF) -> (0-1DFF)
map(12033, 33, 3);                              ����MAPһ���ж����
map(803F00, 1F00, 100);                         �����ӳ��һ��MINIBANK
mapx(1800, 2000, 800, 10000, 800, 40);          ӳ��0x40��Bank
set(out, 8, le);
save(bin, app.bin);                             ���������BIN
clear(out);
mapx(803F00, 0, 100, 10000, 100, 30);           ӳ��0x30��MiniBank
set(out, 8, le);
save(bin, mini.bin);                            ���MINIBANK BIN
clear(out);
;map(11000, 0, 800);                             ��������Ĺ�����
map(F64800, 800, 800);                          DCODE4 ����SCO
map(F54800, 1000, 1800);                        DCODE3 ����
map(FA4000, 2800, 1800);                        DCODE2 FM
map(F94000, 4000, 1800);                        DCODE1 USB�豸
map(F75800, 5800, 800);                         DCODE5 WAV DEC
map(F75800, 6000, 200);                         DCODE5 WAV COM
map(F85A00, 6200, 600);                         DCODE6 WAV ENC
map(FB4000, 6800, 800);                         DCODE7 SPEAKER
map(FB4000, 7000, 100);                         DCODE7 DAC OUT
map(FC4100, 7100, 700);                         DCODE8 AUX EQ
map(FD5800, 7800, 800);                         DCODE9 TOMCAT
map(FE0E00, 8000, 800);                         DCODE9 MP3 ENC
set(out, 8, le);
save(bin, manu.bin);                            ���MANUBANK BIN
clear(out);
map(FF0000, 0, 20);
save(bin, makecfg.bin);
clear(out);
map(FF0100, 0, 200);
save(bin, copyres.bat);
;clear(out);
;map(1800, 1800, 1000);
;setoutput(1800, 1000);
;save(hex, app.hex);                             ���HEX��UART MODEʹ��