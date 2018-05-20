;*****************************************************************************
;* Module    : HexScript
;* File      : hexscript.hs
;* Author    : Hanny
;* Function  : 用于生成下载的bin文件。修改该文件时，建议进行全编译
;*****************************************************************************
load(hex, APP.a51);                             载入
map(2000, 0, 1E00);                             映射(2000-3DFF) -> (0-1DFF)
map(12033, 33, 3);                              暴力MAP一个中断入口
map(803F00, 1F00, 100);                         这里多映射一份MINIBANK
mapx(1800, 2000, 800, 10000, 800, 40);          映射0x40个Bank
set(out, 8, le);
save(bin, app.bin);                             输出主程序BIN
clear(out);
mapx(803F00, 0, 100, 10000, 100, 30);           映射0x30个MiniBank
set(out, 8, le);
save(bin, mini.bin);                            输出MINIBANK BIN
clear(out);
;map(11000, 0, 800);                             蓝牙任务的公共区
map(F64800, 800, 800);                          DCODE4 蓝牙SCO
map(F54800, 1000, 1800);                        DCODE3 蓝牙
map(FA4000, 2800, 1800);                        DCODE2 FM
map(F94000, 4000, 1800);                        DCODE1 USB设备
map(F75800, 5800, 800);                         DCODE5 WAV DEC
map(F75800, 6000, 200);                         DCODE5 WAV COM
map(F85A00, 6200, 600);                         DCODE6 WAV ENC
map(FB4000, 6800, 800);                         DCODE7 SPEAKER
map(FB4000, 7000, 100);                         DCODE7 DAC OUT
map(FC4100, 7100, 700);                         DCODE8 AUX EQ
map(FD5800, 7800, 800);                         DCODE9 TOMCAT
map(FE0E00, 8000, 800);                         DCODE9 MP3 ENC
set(out, 8, le);
save(bin, manu.bin);                            输出MANUBANK BIN
clear(out);
map(FF0000, 0, 20);
save(bin, makecfg.bin);
clear(out);
map(FF0100, 0, 200);
save(bin, copyres.bat);
;clear(out);
;map(1800, 1800, 1000);
;setoutput(1800, 1000);
;save(hex, app.hex);                             输出HEX在UART MODE使用