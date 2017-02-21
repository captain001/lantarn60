#ifndef __LED_H__
#define __LED_H__

#include "export.h"

//#define CONFIG_SEVEN_COLOR	 	//˫ɫ-����ɫ�л�
#define BR		(*(u8*)0x0800fC1e)	//�����ʴ洢λ��

/* ��Դ����״̬�Ĵ��� */
#define rPWR_CSR (*(volatile unsigned long *)0x40007004)


#define  SCREEN_LEN   screen_len   //��Ļ��ȵ��ֽ���
#define  OFFSET_GREEN offset_green
#define  OFFSET_BLUE  offset_green*2

#define bitmaxlen 1048570

#define SIM_ROW 11

//#define VISION		0x1420//�汾��Ϣ	
#define VISION		0x1101//�汾��Ϣ	 
#define MODIFYDATE  0x151111	//��������
 
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
�����Խṹ
***************************************************************************************************/
struct ScrStruct{
	      unsigned long addr ;  //�����Ե���ʼ��ַ
	  		 unsigned short width; //�������㣩
				 unsigned short high;  //����
				 unsigned char dColor; //��ɫ
				 unsigned char text;   //�Ƿ�ȫ���ı���־
				 unsigned long is_init_hb:1; //���߳�ʼ��(ռ��һ��λ�Ŀռ�)
				 unsigned long  OE:1;  //OE����
				 unsigned long  data:1;//���ݼ���
				 unsigned long  scan:4;//ɨ�跽ʽ(ռ������λ�Ŀռ�)//4bit 2014.6.23
				 unsigned long  hsel:2;//��˳��(ռ������λ�Ŀռ�)
				 unsigned long sec :1;		 
				 unsigned long fg1 :1;
				 unsigned long fg2 :1;
				 unsigned long fg3 :1;
				 unsigned long st  :1;  //��Ŀ��ͣ��־
				 unsigned long hb  :1;  //��Ŀ���޻��߱�־
				 unsigned long of:1;			//���ػ���־ on-off
				 unsigned long s_t:1;			//��������
				 unsigned long s_t_end;	   //�������Ž������ 11.25
				 unsigned long d_l:1;			//�������Ʊ�־
				 unsigned long lk:1;			//������־	
				 unsigned long :11;
};

/**************************************************************************************************
�������Խṹ
***************************************************************************************************/
struct AreaStruct{
    /*********************************��λ��begin**************************************************/
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

	 /*********************************��λ��end**************************************************/
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
ģ��ʱ�����Խṹ
***************************************************************************************************/
struct SimStruct{
     		     unsigned long addr;			//ģ��ʱ�����ݵ�ַ
	           unsigned char h_color;		//ʱ����ɫ
	           unsigned char m_color;		//������ɫ
	           unsigned char s_color;   //������ɫ
	           unsigned char date_flag;	//�Ƿ���ʾ���ڱ�־
	           unsigned char asc_w;			//�ַ��ߴ�
	           unsigned char asc_h;
	           unsigned char hz_w;			//���ֳߴ�
	           unsigned char hz_h;
			       unsigned long format_addr; //��ʽ��ַ
	           unsigned long asc_addr;		//ace��ַ
	           unsigned long hz_addr;     //���ֵ�ַ
	           unsigned short date_place_x;	 //������ʾλ��
	           unsigned short date_place_y;	 
	           unsigned char date_color;	 //������ɫ��־
	           unsigned short week_place_x;	 //������ʾλ��
	           unsigned short week_place_y;
	           unsigned char week_color;	 //������ɫ��־
};

/**********************************************************************************************************
��������
**********************************************�ⲿ����*****************************************************/
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
/**********************************�ڲ�����************************************************/
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
