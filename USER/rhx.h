#ifndef __RHX_H__
#define __RHX_H__

#define RHX_AP 1
//#define XGTD_AP 1
//#define JS_AP 1
/*瑞合信系列*/
//#define RH_32MU 1
//#define RH_32UA 1
//#define RH_48UA 1
//#define RH_64UA 1

//#define RHX_64W512 1 
//#define RHX_64W1024 1 
//#define RHX_48W512 1 
//#define RHX_48W1024 1 
#define RHX_32W1024 1 

//#define RHX_32WU1024 1 

/*信邦系列*/
//#define XB_W2_512 1 
//#define XB_W3_512 1 
//#define XB_W3_1024 1 
//#define XB_W4_512 1 
//#define XB_W4_1024 1 


//#define XB_WU2_320 1   //
//#define XB_WU2_512 1
//#define XB_WU2_1024 1   //32high
//#define XB_WUS3_512 1
//#define XB_WUS3_1024 1
//#define XB_WUS4_512 1
//#define XB_WUS4_1024 1
//#define XB_U2_1024 1
//#define XB_U3_512 1
//#define XB_U3  1
//#define XB_U4_512  1


//#define XB_W4_1024 1   //新加的卡型  64W--ESP8266

/*XGTD定制卡*/
//#define XG_U2A   1
//#define XG_W2A 1
//#define XG_W2B  1
//#define XG_W3A 1
//#define XG_W3B  1
//#define XG_W4A  1
//#define XG_U3B 1
//#define XG_U4B 1
// #define XG_W4B 1  

/*简实定制*/
//#define JS_5W3   1
//#define JS_5W4   1
/*----------------------------------------*/

#ifdef RHX_32WU1024
  #define ConMod1 1
  #define cardLength 12
  #define card 0xA4 //32WU1024
	#define MaxHigh  32
	#define MaxWidth  1024
	#define dotmatrix 0x8000
	#define dotmatrixhigh 0x20 
	#define NameCard "\\32WU1024.BIN"
	#define WIFI_SSID_DEV "2B"
	#define WIFI_CARD_5350  1 
	#define U_DISK 1

#elif RH_32MU //add by zzw 15.6.10
   #define ConMod1 1
  #define card 0x60
  #define cardLength 7
	#define MaxHigh  32
	#define MaxWidth    1024
	#define dotmatrixhigh 32 
	#define dotmatrix 0x4000
	#define WIFI_SSID_DEV "4A"
	#define NameCard "\\ROM.BIN"
	#define U_DISK 1
	
#elif RH_32UA //add by zzw 15.3.25
   #define ConMod1 1
  #define card 0xaa
  #define cardLength 7
	#define MaxHigh  32
	#define MaxWidth    320
	#define dotmatrixhigh 32 
	#define dotmatrix 10240
	#define NameCard "\\ROM.BIN"
	#define U_DISK 1
	
#elif RH_48UA //add by zzw 15.3.25
  #define ConMod2 1
  #define card 0xab
  #define cardLength 7
	#define MaxHigh  48
	#define MaxWidth    768
	#define dotmatrixhigh 48
	#define dotmatrix 24576
	#define NameCard "\\ROM.BIN"
	#define U_DISK 1
#elif RH_64UA //add by zzw 15.3.25
  #define ConMod2 1
  #define card 0xac
  #define cardLength 7
	#define MaxHigh  64
	#define MaxWidth    1024
	#define dotmatrixhigh 64
	#define dotmatrix 32768
	#define NameCard "\\ROM.BIN"	
	#define U_DISK 1

#elif RHX_64W512 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0xa0
  #define cardLength 10
	#define MaxHigh  64
	#define MaxWidth    1024
	#define dotmatrixhigh 0x40
	#define dotmatrix 0x8000
	#define NameCard "\\ROM.BIN"
	#define WIFI_SSID_DEV "4A"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define U_DISK 0
	
#elif RHX_64W1024 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0xa1
  #define cardLength 11
	#define MaxHigh  64
	#define MaxWidth    2048
	#define dotmatrix 0x10000
	#define dotmatrixhigh 0x40
	#define WIFI_SSID_DEV "4B"
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define U_DISK 0
	
#elif RHX_48W512 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0x9e
  #define cardLength 10
	#define MaxHigh  48
	#define MaxWidth    768//2016
	#define dotmatrixhigh 48
	#define dotmatrix 0x6000
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "3A"
	#define U_DISK 0
	
#elif RHX_48W1024 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0x9f
  #define cardLength 11
	#define MaxHigh  48
	#define MaxWidth    1536
	#define dotmatrix 0xC000
	#define dotmatrixhigh 48
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "3B"
	#define U_DISK 0
#elif RHX_32W1024
	#define ConMod1 1
  #define card 0x9D
  #define cardLength 11
	#define MaxHigh  32
	#define MaxWidth    1024
	//#define MaxHigh  24
	//#define MaxWidth    160
	#define dotmatrix 0x8000
	#define dotmatrixhigh 32
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "2B"
	#define U_DISK 0
/********************XB*****************************/
#elif XB_W2_512
	#define ConMod1 1
  #define card 0x86
  #define cardLength 9
	#define MaxHigh  32
	#define MaxWidth    512
	#define dotmatrix 0x4000
	#define dotmatrixhigh 32
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "2A"
	#define U_DISK 0

#elif XB_W3_512 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0x89
  #define cardLength 9
	#define MaxHigh  48
	#define MaxWidth    768
	#define dotmatrixhigh 48
	#define dotmatrix 0x6000
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "3A"
	#define U_DISK 0
	
#elif XB_W3_1024 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0x90
  #define cardLength 10
	#define MaxHigh  48
	#define MaxWidth    1536
	#define dotmatrixhigh 48
	#define dotmatrix 0xC000
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "3B"
	#define U_DISK 0
#elif XB_W4_512 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0x91
  #define cardLength 9
	#define MaxHigh  64
	#define MaxWidth    1024
	#define dotmatrixhigh 0x40
	#define dotmatrix 0x8000
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "4A"
	#define U_DISK 0
#elif XB_W4_1024 //add by zzw 15.3.12
   #define ConMod1 1
  #define card 0x92
  #define cardLength 10
	#define MaxHigh  64
	#define MaxWidth    2048
	#define dotmatrixhigh 0x40
	#define dotmatrix 0x10000
	#define NameCard "\\ROM.BIN"
	#define WIFI_CARD_8266  1   //WIFI_CARD_5350
	#define WIFI_SSID_DEV "4B"
	#define U_DISK 0

#elif XB_WU2_320
  #define ConMod1 1   //通信方式1(32高)(2对应48，64)
  #define cardLength 10  //卡型长度
  #define card 0x37     //编号
	#define MaxHigh 0x20    //限制高(能带的最大高度)
	#define MaxWidth 0x400   //限制款(能带的最大宽度)
	#define dotmatrixhigh 0x20      //84返回值  高
	#define NameCard "\\WU2-320.BIN"      //U盘升级文件名
	#define U_DISK 1
	
#elif XB_WU2_512
  #define ConMod1 1
  #define cardLength 10
  #define card 0x82
	#define MaxHigh  32
	#define MaxWidth  512
	#define dotmatrixhigh 0x20
	#define NameCard "\\WU2-512.BIN"
	#define U_DISK 1

#elif XB_WU2_1024
  #define ConMod1 1
  #define cardLength 11
  #define card 0x68 //0xA4 32WU1024
	#define MaxHigh  32
	#define MaxWidth  1024
	#define dotmatrix 0x8000
	#define dotmatrixhigh 0x20 
	#define NameCard "\\ROM.BIN"
	#define WIFI_SSID_DEV "2B"
	#define WIFI_CARD_5350  1 
	#define U_DISK 1

#elif XB_WUS3_512
  #define ConMod2 1
  #define card 0x83
  #define cardLength 11
	#define MaxHigh 0x30
	#define MaxWidth 0x800
	#define dotmatrixhigh 0x30
	#define NameCard "\\WUS3-512.BIN"
	#define U_DISK 1

#elif XB_WUS3_1024
  #define ConMod2 1
  #define card 0x67
  #define cardLength 12
	#define MaxHigh  48
	#define MaxWidth   1536
	#define dotmatrixhigh 0x30
	#define NameCard "\\ROM.BIN"
	#define U_DISK 1

#elif XB_WUS4_512
  #define ConMod2 1
  #define card 0x84
  #define cardLength 11
	#define MaxHigh 64
	#define MaxWidth  1024 
	#define dotmatrixhigh 0x40
	#define NameCard  "\\WUS4-512.BIN"
	#define U_DISK 1

#elif XB_WUS4_1024
  #define ConMod2 1
  #define card 0x64
  #define cardLength 12
	#define MaxHigh  64
	#define MaxWidth    2048
	#define dotmatrixhigh 0x40
	#define NameCard "\\ROM.BIN"
	#define U_DISK 1
	
	
#elif XB_U2_1024 
  #define ConMod1 1
  #define cardLength 10
  #define card 0x94
	#define MaxHigh 32
	#define MaxWidth   1024
	#define dotmatrixhigh 0x20
	#define NameCard "\\ROM.BIN"//"\\U2-1024.BIN"
	#define WIFI_SSID_DEV "2B"
	#define U_DISK 1



#elif XB_U3_512
  #define ConMod2 1
  #define card 0x95
  #define cardLength 9
	#define MaxHigh   48
	#define MaxWidth   768
	#define dotmatrixhigh 0x30
	#define NameCard "\\U3-512.BIN"
	#define U_DISK 1

#elif XB_U3 
  #define ConMod2 1
  #define card 0x66  
  #define cardLength 5
	#define MaxHigh   48
	#define MaxWidth 1536
	#define dotmatrixhigh 0x30
	#define NameCard "\\ROM.BIN"
	#define U_DISK 1

#elif XB_U4_512
  #define ConMod2 1
  #define card 0x96
  #define cardLength 9
	#define MaxHigh  64
	#define MaxWidth   1024
	#define dotmatrixhigh 0x40
	#define NameCard "\\U4-512.BIN"	
	#define U_DISK 1
	
/*------------------------------------XGTD----------------------*/	
#elif XG_U3B 
  #define ConMod2 1
  #define card 0x66  
  #define cardLength 6
	#define MaxHigh   48
	#define MaxWidth 1536
	#define dotmatrixhigh 0x30
	#define NameCard "\\ROM.BIN"
	#define U_DISK 1

#elif XG_W4B
  #define ConMod2 1
  #define card 0x64
  #define cardLength 6
	#define MaxHigh  64
	#define MaxWidth    2048
	#define dotmatrixhigh 0x40
	#define NameCard "\\ROM.BIN"
	#define U_DISK 0
	
#elif XG_U4B
    #define ConMod2 1
  #define card  0x68 
  #define cardLength 6
	#define MaxHigh  64
	#define MaxWidth    2048
	#define dotmatrixhigh 0x40
	#define NameCard  "\\64Ux.BIN" 
	#define U_DISK 1
	
#elif XG_U2A
  #define ConMod1 1
  #define cardLength 6
	#define card 0x52 
	#define MaxHigh  32
	#define MaxWidth  512
	#define dotmatrixhigh 0x20
	#define NameCard   "\\ROM.BIN"
	#define U_DISK 1
	
#elif XG_W2A
  #define ConMod1 1
  #define cardLength 6
  #define card 0x82
	#define MaxHigh  32
	#define MaxWidth  512
	#define dotmatrixhigh 0x20
	#define NameCard "\\WU2-512.BIN"
	#define U_DISK 0
	
#elif XG_W2B
  #define ConMod1 1
  #define cardLength 6
  #define card 0x65
	#define MaxHigh  32
	#define MaxWidth  1024
	#define dotmatrixhigh 0x20
	#define NameCard "\\ROM.BIN"
	#define U_DISK 0
	
#elif XG_W3A
  #define ConMod2 1
  #define card 0x83
  #define cardLength 6
	#define MaxHigh 48
	#define MaxWidth 768
	#define dotmatrixhigh 0x30
	#define NameCard "\\WUS3-512.BIN"
	#define U_DISK 0
	
#elif XG_W3B
  #define ConMod2 1
  #define card 0x67
  #define cardLength 6
	#define MaxHigh  48
	#define MaxWidth   1536
	#define dotmatrixhigh 0x30
	#define NameCard "\\ROM.BIN"
	#define U_DISK 0
	
#elif XG_W4A
  #define ConMod2 1
  #define card 0x84
  #define cardLength 6
	#define MaxHigh 64
	#define MaxWidth  1024 
	#define dotmatrixhigh 0x40
	#define NameCard  "\\WUS4-512.BIN"
	#define U_DISK 0
	
#elif JS_5W3
  #define ConMod2 1
  #define card 0x83
  #define cardLength 6
	#define MaxHigh 0x30
	#define MaxWidth 768
	#define dotmatrixhigh 0x30
	#define dotmatrix 0x6000
	#define NameCard "\\JS-5W3.BIN"
	#define WIFI_CARD_5350  1   //WIFI_CARD_5350 WIFI_CARD_8266
	#define U_DISK 1
#elif JS_5W4
	#define ConMod2 1
  #define card 0x64
  #define cardLength 6
	#define MaxHigh 0x40
	#define MaxWidth 2048
	#define dotmatrixhigh 0x40
	#define dotmatrix 0x10000
	#define NameCard "\\JS-5W4.BIN"
	#define WIFI_CARD_5350  1   //WIFI_CARD_5350
	#define U_DISK 1
#endif 

#endif
