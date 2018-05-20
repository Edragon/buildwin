;*****************************************************************************
;* Module    : HexScript
;* File      : hexscript.hs
;* Author    : Hanny
;* Email     : coldney@yahoo.com.cn
;* Function  : 用于生成下载的bin文件。修改该文件时，建议进行全编译
;*****************************************************************************
load(hex, APP_LLP.a51);                         载入
map(0, 0, 4000);                                映射(0-3fff) -> (0-3fff)
set(out, 16, le);                               设置输出位宽为16BIT，小端
save(hex, APP_LLP.hex);                         输出Hex
mapx(11800, 2000, 800, 10000, 800, 3d);         映射0x30个Bank
map(11000, 20800, 800);                         蓝牙任务的公共区
map(864800, 21000, 800);                        DCODE4 蓝牙SCO
map(854800, 21800, 1800);                       DCODE3 蓝牙SBC
map(844000, 23000, 1800);                       DCODE2 FM
map(834000, 24800, 1800);                       DCODE1 USB设备
map(824000, 26000, 800);                        DCODE0 LOUD SPEAKER
map(874000, 26800, 800);                        DCODE4 文件系统
set(out, 8, le);
save(bin, APP_LLP.bin);

clear(out);
map(FF0000, 0, 20);
save(bin, makecfg.bin);