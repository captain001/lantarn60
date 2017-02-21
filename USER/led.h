#ifndef __LED_H__
#define __LED_H__

#include "export.h"

//#define CONFIG_SEVEN_COLOR	 	//双色-》七色切换
#define BR		(*(u8*)0x0800fC1e)	//波特率存储位置

/* 电源管理状态寄存器 */
#define rPWR_CSR (*(volatile unsigned long *)0x40007004)


#define  SCREEN_LEN   screen_len   //屏幕宽度的字节数
#define  OFFSET_GREEN offset_green
#define  OFFSET_BLUE  offset_green*2

#define bitmaxlen 1048570

#define SIM_ROW 11

//#define VISION		0x1420//版本信息	
#define VISION		0x1101//版本信息	 
#define MODIFYDATE  0x151111	//发布日期
 
#define GREEN_DATA screen_w*screen_h  
#define BLUE_DATA  GREEN_DATA*2  

#ifdef  CONFIG_SEVEN_COLOR
 #define GREEN_BEGIN 
 #define GREEN_END	 
#else   
 #define GREEN_BEGIN if(1==scr_info.dColor){
 #define GREEN_END	 }
  	
#endif
/**************************************************************************************************
屏属性结构
***************************************************************************************************/
struct ScrStruct{
	      unsigned long addr ;  //屏属性的起始地址
	  		 unsigned short width; //屏宽（按点）
				 unsigned short high;  //屏高
				 unsigned char dColor; //颜色
				 unsigned char text;   //是否全是文本标志
				 unsigned long is_init_hb:1; //花边初始化(占用一个位的空间)
				 unsigned long  OE:1;  //OE极性
				 unsigned long  data:1;//数据极性
				 unsigned long  scan:4;//扫描方式(占用三个位的空间)//4bit 2014.6.23
				 unsigned long  hsel:2;//行顺序(占用两个位的空间)
				 unsigned long sec :1;		 
				 unsigned long fg1 :1;
				 unsigned long fg2 :1;
				 unsigned long fg3 :1;
				 unsigned long st  :1;  //节目暂停标志
				 unsigned long hb  :1;  //节目有无花边标志
				 unsigned long of:1;			//开关机标志 on-off
				 unsigned long s_t:1;			//定长播放
				 unsigned long s_t_end;	   //定长播放结束标记 11.25
				 unsigned long d_l:1;			//日期限制标志
				 unsigned long lk:1;			//锁定标志	
				 unsigned long :11;
};

/**************************************************************************************************
分区属性结构
***************************************************************************************************/
struct AreaStruct{
    /*********************************上位机begin**************************************************/
          unsigned long addr;		   
				  unsigned char style;     
				  unsigned long screen_w;  
				  unsigned long screen_h;
				  unsigned long place_w;	   
				  unsigned long place_h;
				  unsigned long info_number;   
				  unsigned char area_hb_flag;  
				  unsigned char area_back;

				  unsigned char info_mode;      	
				  unsigned char info_speed;	   		
				  unsigned char info_hb_flag;     
				  unsigned char info_standtime;		
				  unsigned char info_stand_mode;	
				  unsigned char info_exit_mode;		
				  unsigned long info_data_w;	 		
				  unsigned long info_data_h;	 		
				  unsigned long info_content_addr;

	 /*********************************上位机end**************************************************/
				  unsigned char info_c_speed;
				  unsigned char info_c_stand; 
				  unsigned char temp;           
				  unsigned char speed_hb_c;   
				  unsigned char cnt;          
				  unsigned char sj_cnt;      
				 
				  unsigned short sj:1;     
				  unsigned short st:1;	    
				  unsigned short st_end:1;	
				  unsigned short end:1;		  
				  unsigned short info_end:1;
				  unsigned short is_init_sim:1; 
				  unsigned short is_init_dig:1; 
				  unsigned short is_init_count:1;
				  unsigned short is_init_datn:1; 

				  unsigned char fg:1;
				  unsigned char st_over:1;
				  unsigned char st_ly:1;
				  
				  unsigned short fen_pos;  
				  unsigned short miao_pos; 
				  unsigned short asc_size; 
				  unsigned long  asc_addr; 
				  
				  unsigned long parameter; 
				  unsigned char info_c;  
};

/**************************************************************************************************
模拟时钟属性结构
***************************************************************************************************/
struct SimStruct{
     		     unsigned long addr;			//模拟时钟内容地址
	           unsigned char h_color;		//时针颜色
	           unsigned char m_color;		//分针颜色
	           unsigned char s_color;   //秒针颜色
	           unsigned char date_flag;	//是否显示日期标志
	           unsigned char asc_w;			//字符尺寸
	           unsigned char asc_h;
	           unsigned char hz_w;			//汉字尺寸
	           unsigned char hz_h;
			       unsigned long format_addr; //格式地址
	           unsigned long asc_addr;		//ace地址
	           unsigned long hz_addr;     //汉字地址
	           unsigned short date_place_x;	 //日期显示位置
	           unsigned short date_place_y;	 
	           unsigned char date_color;	 //日期颜色标志
	           unsigned short week_place_x;	 //星期显示位置
	           unsigned short week_place_y;
	           unsigned char week_color;	 //星期颜色标志
};

/**********************************************************************************************************
函数声明
**********************************************外部函数*****************************************************/
void clr_screen(void);
void clr_buf(void);
void screen_init(void);
void program_deal(void);
void area_deal(void);
void info_init(struct AreaStruct *);
void datadeal(void);
void datadeal_word(void);
void read_buf(unsigned char n,unsigned char *p,unsigned long addr);
void flash_check(void);
void date_limt(void);
void id_check(void);
void show_info(unsigned char ch);
void show_err(unsigned char ch);
unsigned char srs(void);
void init_t4(unsigned char);
/**********************************内部函数************************************************/
void info_parameter_deal(void);
void show_debug(void);
void rcc_set(unsigned char vl);
void date_limt_set(void);
void flash_read_write(unsigned char);
void timer_bright_set(void);
void flash_deal(void);
void timer_bright_check(void);

void ShowVersion(void);
void ShowRtcError(void);
void show(unsigned char index,unsigned char n,unsigned char pos);//12.11.14
void LEDPowerLow(void);
void sendtime(void);

unsigned long GetCnt(void);

void ledplay_denglong(void);
void ledplay_denglongHei(unsigned char data);
void time_datadeal(void);
/*************************************************************************************************/

#endif
