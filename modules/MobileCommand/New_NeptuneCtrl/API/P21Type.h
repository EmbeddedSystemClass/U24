
#ifndef _P21_TYPE_H_
#define _P21_TYPE_H_


//FF:設定有關DT的常數

/*
    02: IPL.ipl
    03: DT.img                          BDK0-BIND
    04: OS0 (IPLMDOC.nb0)               BDK1-BIN0
    05: OS1 (EBOOTMDOC.nb0)             BDK1-BIN1 
    06. OS2 (RESERVED.dio, GSM, GSMFFS) BDTL0
    07: OS3 (FLASH.dio)                 BDTL1
    08: OS4 (mobicon?)
    09: OS5 user file ?                 BDTL2
  => ipl : 0x30 0x32

The Mapping is

#define P21_DT_FLASH                         8
The Submenu item is
IPL    ->2
DT     ->3
msipl  ->4
eboot  ->5
os2    ->10
os3	   ->11
os4    ->?
os5    ->12
*/

#define P21_MENUITEM_IPL    2
#define P21_MENUITEM_DT     3
#define P21_MENUITEM_OS0    4
#define P21_MENUITEM_MSIPL  4
#define P21_MENUITEM_OS1    5
#define P21_MENUITEM_EBOOT  5
#define P21_MENUITEM_OS2   10
#define P21_MENUITEM_BDTL0 10
#define P21_MENUITEM_OS3   11
#define P21_MENUITEM_BDTL1 11
//#define P21_MENUITEM_OS4 ? mobicom is unknow
#define P21_MENUITEM_OS5   12
#define P21_MENUITEM_MEPD  13

#define P21_RECORE_HEADER   0X30
#define P21_RECORE_IPL_FILE 0X32
#define P21_RECORE_DT_FILE  0X33
#define P21_RECORE_OS0_FILE 0X34
#define P21_RECORE_OS1_FILE 0X35
#define P21_RECORE_OS2_FILE 0X36
#define P21_RECORE_OS3_FILE 0X37
#define P21_RECORE_OS4_FILE 0X38
#define P21_RECORE_OS5_FILE 0X39

#define P21_DL_METHOD_USB    "03"
#define P21_DL_METHOD_SDCARD "04"

#define P21_TYPE_IPL_FILE    "02"
#define P21_TYPE_DT_FILE     "03"
#define P21_TYPE_MSIPL_FILE  "04"
#define P21_TYPE_EBOOT_FILE  "05"
#define P21_TYPE_BDTL0_FILE  "06"
#define P21_TYPE_BDTL1_FILE  "07"
#define P21_TYPE_OS4_FILE    "08"
#define P21_TYPE_OS5_FILE    "09"
#define P21_TYPE_MEPD_FILE   "0A"

//MEPD Related
#define FA_SIZE_2G_FLASH 512*1024 
#define FA_SIZE_1G_FLASH 256*1024
#define MEPD_OFFSET        5*1024 // Factory Area + BT address + BSP audio params


#endif //! _P21_TYPE_H_
