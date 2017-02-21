/*******************************************************************************
°æ±¾ºÅ 1400

ÈÕÆÚ 2015.01.15

°æ±¾   ³ÌÐòÕûºÏ°æ
 
1ÔÂ12ÈÕ ÓÅ»¯¹Ø»ú
*--------------------------ÐÅ¸ë¶¨ÖÆ°æ 15.01.10 flw----------------------------*/
 
 
#include "stmlib.h"
#include "export.h"
#include "clock.h"
#include "led.h"
#include "hb.h"
//#include "stdlib.h"

#define		INTERVAL_PATTERN_ADDR			0x22		//¼ä¸ô»¨Ñù¿ª¹Ø
#define		TIME_ONOFF_ADDR			0x20		//Ê±¼ä¿ª¹Ø
#define		DISP_PATTERN_ADDR		0x21		//ÏÔÊ¾Ä£Ê½
#define		BRIGHT_BG_ADDR			0x2a		//±³¾°ÁÁ¶È
#define		BRIGHT_WORD_ADDR		0x29		//ÎÄ×ÖÁÁ¶È



 
//¶¨ÒåÍâ²¿±äÁ¿
extern struct AreaStruct area[8]; //·ÖÇøÊôÐÔ½á¹¹
extern struct ScrStruct  scr_info;//ÆÁÊôÐÔ½á¹¹
extern unsigned char pro_num,info_num,pro_c; //½ÚÄ¿×ÜÊý¡¢ÐÅÏ¢¸öÊý¡¢ÐÅÏ¢¼ÆÊýÆ÷
extern unsigned char sim_buf_red[];//»º³åÇø£¬´æ·Å´ÓÄÚ²¿flashÖÐ¶Á³öµÄÊý¾Ý
extern unsigned char flashcheckerr;//³õÊ¼ÖµÆÁ²Î
extern unsigned char leddata[];//ÏÔÊ¾»º³åÇø
extern void (*led_play)(void);

extern unsigned long flash_addr;
extern unsigned short g_wShowVersionCnt;
extern unsigned short screen_len;
extern unsigned char bright;
extern unsigned char g_ucChangeScan;
extern unsigned char wifi_init_flag,wifi_config,wifiinit,timer1_flag,config_step;                    //wifi ³õÊ¼»¯±êÖ¾Î» //ÉÏµçÆô¶¯ 0£»ÅäÖÃ 1£»Õý³£¹¤×÷ 2£»
extern unsigned char screeniniterr;

extern	unsigned char		mm_set_en;		//=1ÔÊÐíÉèÖÃWIFIÃÜÂë
extern	unsigned char		scr_onoff;					//ÆÁÄ»¿ª¹Ø±êÖ¾  0¿ª  1¹Ø



/**************************************************************************
** º¯ÊýÃû³Æ £º delay
** º¯Êý¹¦ÄÜ £º ÑÓÊ±º¯Êý  
** Èë¿Ú²ÎÊý £º ÐèÒªÑÓÊ±µÄÊ±¼ä
** ·µ»Ø²ÎÊý £º ÎÞ
** º¯ÊýËµÃ÷ £º 
** ÐÞ¸ÄÈÕÖ¾ £º 2013.3.18 ÖìÕñÍþ	
**************************************************************************/
static void delay(unsigned long time){
	while(time--);
}


/**************************************************************************
** º¯ÊýÃû³Æ £º PowerManagerInit
** º¯Êý¹¦ÄÜ £º µçÔ´¹ÜÀí³õÊ¼»¯ 
** Èë¿Ú²ÎÊý £º ÎÞ
** ·µ»Ø²ÎÊý £º ÎÞ
** º¯ÊýËµÃ÷ £º 
** ÐÞ¸ÄÈÕÖ¾ £º 2013.4.06 ÖìÕñÍþ	
**************************************************************************/
static void PowerManagerInit(void)
{
	#define rPWR_CR  (*(volatile unsigned long *)0x40007000)
	
	/* ¸ù¾ÝSTM32Êý¾ÝÊÖ²áµÚ44Ò³Éè¶¨µçÑ¹·§Öµ£¬ÕâÀïÊÇ2.9V */
	rPWR_CR |= (7 << 5); 
	/* Æô¶¯PWR¹¦ÄÜ */
	rPWR_CR |= (1 << 4);
}


/**************************************************************************
** º¯ÊýÃû³Æ £º PowerManagerInit
** º¯Êý¹¦ÄÜ £º µÍÑ¹¼ì²âÖÐ¶Ï
** Èë¿Ú²ÎÊý £º ÎÞ
** ·µ»Ø²ÎÊý £º ÎÞ
** º¯ÊýËµÃ÷ £º µçÑ¹Ô½¹ý·§Öµ£¨2.9£©·üÊ±»á½øÈë´ËÖÐ¶Ï
** ÐÞ¸ÄÈÕÖ¾ £º 2013.4.06 ÖìÕñÍþ	
**************************************************************************/
void PVD_IRQHandler(void)
{

	EXTI->PR=1<<16;//EXTI_ClearITPendingBit(EXTI_Line16);
	/* µçÑ¹Ô½¹ý·§Öµ¸´Î»Ò»´Î */
	if ( !(rPWR_CSR & 4) )
	{
		while (1)
		{
			//memset(leddata, 0, 8*1024);
			if( (rPWR_CSR & 4))
			{
				SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
      }
    }
	}	
   else 
	 {
			memset(leddata, 0, 8*1024);
			LEDPowerLow( );
			while (1)
			{
				if( !(rPWR_CSR & 4))
				{
					SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
				}
			}			
   }
}

//Ö÷º¯Êý
int rtc_error_flag;//Õâ¸öÈç¹ûRTC³õÊ¼»¯Ê§°ÜÔòÖÃÎ»
unsigned char wait_wifi_tt=0;
extern unsigned char start_wifi_init,start_wifi_config;
extern void Get_SerialNum(void);
extern unsigned char init_finish;
unsigned char time_flag=0;
unsigned char pro_end1=0;

int main(void)								//===============================================MAIN=====================================================
{	
	unsigned long dwTime;
	unsigned char ucCh = 0, i,ii,zz=0,j,jj;
	unsigned char light;	
	unsigned char light_bg;	
	unsigned char time_onoff;	
	unsigned char test_Pattern;
	unsigned char n_time_paly;
	unsigned char wz_paly_n=0;		//ºº×ÖÏÔÊ¾´ÎÊý
	unsigned char interval_pattern_flag;		//¼ä¸ô²¥·Å»¨Ñù±êÖ¾
	unsigned char Pattern_n;			//»¨Ñù²¥·Å¼¸±é
	
	unsigned char  	n;
	unsigned char   lh;
//	unsigned char  	ch = 0;
	unsigned char  	dat = 0;
	unsigned int  	m = 0;
	unsigned int 		m2 ,m3;
	unsigned char		every_hang_byte;
	unsigned char		hang_shu;
	//unsigned char		disp_nbytes;
//	unsigned char		dat2;
//	unsigned char		begin;	
	
//	unsigned char		tmp1;	
//	unsigned char		tmp2;	
//	unsigned char		indi;
	
	u32 r1,r2;										//´æ´¢Ê±¼äÃëÊý
	
	
	scr_info.d_l=0;
	
	if(RCC->CSR&0x10000000)
		zz=1; 
	
	RCC_Configuration();  //RCCÊ±ÖÓ³õÊ¼»¯
	GPIO_Configuration(); //GPIO³õÊ¼»¯
	clr_screen();					//ÇåÆÁ=================================================================
	NVIC_Configuration(); //ÖÐ¶Ï¿ØÖÆÆ÷³õÊ¼»¯		
	PowerManagerInit();   //µçÔ´¹ÜÀí³õÊ¼»¯ 2013.04.06Ôö¼Ó
	IWDG_Init(7,4000);    	//Óë·ÖÆµÊýÎª64,ÖØÔØÖµÎª625,Òç³öÊ±¼äÎª25s   
	rtc_error_flag=rtc_init();    //RTC³õÊ¼»¯
	
	
	
#ifdef ConMod2	
 uart2_init(115200);

	if(BR==1)  
  		uart_init(115200);	//³õÊ¼»¯²¨ÌØÂÊ
  	else if(BR==2) 
  		uart_init(38400);
    else 
		 uart_init(9600);
#elif ConMod1		
		uart_init(115200);
#endif	
	wait_wifi_tt=0;		
		
#ifdef	WIFI_CARD_8266
       
       Get_SerialNum();

   if(*(volatile unsigned char*)0x0800FC55 == 0xab)      
	{
	   
	   zz=0;
	  send_string("+++",3); 				
	}
	if(zz==0)
	{        
	  init_finish=0;
		wifi_init_flag=0;
		config_step=0;
		timer1_flag=0;
	 	timer1_init(); 
	}
	else
	{

   init_finish=0x55;
		wifi_init_flag=0;
  
  }


#elif WIFI_CARD_5350		
		if(zz==1)
		{
			if(*(unsigned char*)0x0800FC55 == 0xa0)     
			{
				wifi_init_flag=5;		
			}	
			else if(*(unsigned char*)0x0800FC55 == 0xab)     
			{
   
				flash_read_write(1);
				sim_buf_red[0x55] = 0xa0;
				flash_read_write(0);
				start_wifi_init=0;
				start_wifi_config=0;
				wifiinit=0;
        config_step=0;
				wifi_init_flag=0;
				timer1_flag=0;
				wifi_config=1;		
				timer1_init(); 

				wait_wifi_tt=0x55;
				
}
			else if(*(unsigned char*)0x0800FC55 == 0xFF)      
			{	
				 wifi_init_flag=5;				 
			}
		}
		else
		{
			if(*(unsigned char*)0x0800FC55 != 0xFF)     
			{
					 flash_read_write(1);
					 sim_buf_red[0x55] = 0xFF;
					 flash_read_write(0);
			}
			wifi_init_flag=5;
		}
		
		
#endif				


	flashcheckerr=0;
	screeniniterr=0;
	#if U_DISK

		spi_init(0x327);//spi ³õÊ¼»¯£¬32·ÖÆµ
		ch734init();//³õÊ¼»¯²¢¶ÁÈ¡Êý¾Ý
	#endif
	spi_init(0x30f);//spi ³õÊ¼»¯£¬4·ÖÆµ
		
 	//flash_check();//¼ì²âflashÖÐÓÐÎÞÊý¾Ý		=========================================================
 	screen_init();//ÆÁÐÅÏ¢³õÊ¼»¯
	timer3_init();//¶¨Ê±Æ÷3³õÊ¼»¯
	clr_screen();//ÇåÆÁ
		IWDG_Feed();					//Î¹¹·
/************************×Ô¼ì*********12.11.15******************************/
	if (!(GPIOA->IDR & (1 << 13))) 
	{		   //Èç¹ûGPA13¹Ü½ÅÎªµÍµçÆ½   °´¼ü
			dwTime = 0x20000;//dwTime = 0x200000;//2014.08.28 ÑÓÊ±100ms
			while (dwTime --> 5)
			{IWDG_Feed();//Î¹¹·
				if (GPIOA->IDR & (1 << 13))
				{
					ucCh = 1;
					break;
				}		
			}
			if (ucCh != 1)
			{
				for(i = 0; i < 6; i ++)
						show(1, 1, cardLength-4 + i);
			}
			dwTime = 0xA000000;//0x600000;//2014.08.28 ÑÓÊ±60s
			while ((dwTime --> 5) && (ucCh != 1))
			{IWDG_Feed();//Î¹¹·
				 if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))
				 {
					 if((dwTime%5000)==0)led_play();//2014.8.19
				 }
				if (GPIOA->IDR & (1 << 13))
				{
					delay(0x100000);
					if (GPIOA->IDR & (1 << 13))
					{
						ucCh = 2;
						break;
					}
				}
			}
			if ((GPIOA->IDR & (1 << 13)) && (2 == ucCh))
			{
				dwTime = 0x300000;
				while (dwTime --> 5)
				{IWDG_Feed();//Î¹¹·
					if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))
				 {
					 if((dwTime%5000)==0)led_play();//2014.8.19
				 }
					if (!(GPIOA->IDR & (1 << 13)))
					{ 
						ucCh = 1;
						break;
					}
				}
				if ((GPIOA->IDR & (1 << 13)) && (2 == ucCh))
				{

					FLASH_Unlock();
					FLASH_ErasePage(0x0800FC00);
						flash_chip_erase();
					flash_addr=0;
					
					if(flash_read(0)!=0xff){ //ÅÐ¶ÏÊÇ·ñ²Á³ýÍê
							flash_byte_write(0x0);
							flash_chip_erase(); //ÔÙ´Î²Á³ý
					}
				
					
				for(i=0;i<6;i++)//15.3.27
				show(0,1,cardLength-4 + i);
		
				g_wShowVersionCnt = 0;
				while (g_wShowVersionCnt < 300)//2013.5.30
				{
					if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))led_play();///15.03.19
				}
					
					#ifdef	WIFI_CARD_8266
						flash_read_write(1);
					sim_buf_red[0x55] = 0xab;//0xab; //Ïû³ý¸´Î»ÖØÐÂÅäÖÆ
					flash_read_write(0);
					
					////RRRR	
					#elif WIFI_CARD_5350
					
							#ifdef ConMod1
							uart_init(115200);
							#elif ConMod2
							uart2_init(115200);
							#endif 
					wifiinit=1;
					timer1_flag=0;
					config_step=0;	
					wifi_init_flag=0;		
					wifi_config=1;	
									start_wifi_config=0;
					start_wifi_init=0;
					timer1_init(); 	
					
					for(i=0;i<6;i++)
						show(0,1,cardLength-4 + i);
		
				g_wShowVersionCnt = 0;
				while (g_wShowVersionCnt < 300)//2013.5.30
				{
					if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))led_play();///15.03.19
				}
				while(wifiinit!=3)
				{
					IWDG_Feed();//Î¹¹·//2014.09.13
					if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))led_play();///15.03.19
				}
				
				
					#endif
				

	// 			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//¸´Î»
	// 			
	// 			while(1);
								
				} 
			}
	}
	IWDG_Feed();					//Î¹¹·
/**********************************************************************************/
  exti_init(); //°´¼üÍâ²¿ÖÐ¶Ï³õÊ¼»¯	
	
	flash_read_write(1); 						//0x29 ´æ·ÅÎÄ×ÖÁÁ¶ÈÖµ
	interval_pattern_flag=sim_buf_red[INTERVAL_PATTERN_ADDR];				//read¼ä¸ô»¨Ñù±êÖ¾
	light=sim_buf_red[0x29]&0xf0;
	light>>=4;
	if(light>0X0f)light=0x0f;
	//light=16-light;
	
	
	//light+=1;								//×î´óÁÁ¶È½µµÍÁ½¸öµµ
	
	light_bg=sim_buf_red[BRIGHT_BG_ADDR]+0x01;   //±³¹âÁÁ¶ÈÃüÁî0~15   ===================================
	if(light_bg>0x10)	light_bg=0x10;
	if(light_bg<0x02) light_bg=0x02;						//ÏÞÖÆ×î´óÁÁ¶È£¬´ËÖµÔ½Ð¡  Ô½ÁÁ
	light_bg*=0X03;	
	//light_bg-=0X02;						
	
	time_onoff=sim_buf_red[TIME_ONOFF_ADDR];
	test_Pattern=sim_buf_red[DISP_PATTERN_ADDR];			//ÏÔÊ¾»¨Ñù
//	if(test_Pattern>0x01)test_Pattern=0x01;
	//if

#ifdef  WIFI_CARD_5350	
	if(wait_wifi_tt!=0x55)
	{
		start_wifi_init=1;  //¿ªÊ¼ÉÏµçÅäÖÆ
		wifiinit=0;
		wifi_init_flag=0;
		timer1_flag=0;
		config_step=0;
		timer1_init(); 				
	}	
#endif
	
	
	IWDG_Feed();					//Î¹¹·
	
	clr_screen();
	if(screeniniterr==0)
	{
		if(flashcheckerr==1)//¶ÁÈ¡½ÚÄ¿ÆÁ²Î
		{
			screeniniterr=1;
		}
		else if((flashcheckerr==2)||(flashcheckerr==3))//¶ÁÈ¡½ÚÄ¿ÆÁ²Î
		{
			screeniniterr=5;
		}
	}
	
	if(screeniniterr)//½ÚÄ¿³ö´í
		show_err(screeniniterr);

	if(rtc_error_flag==0x01)//ÏÔÊ¾  Ð£Ê±Ê§°Ü
	{
	g_wShowVersionCnt = 0;
	
		ShowRtcError(); // ²¥·Å½ÚÄ¿Ö®Ç°ÏÔÊ¾°æ±¾ºÅºÍ·¢²¼ÈÕÆÚ
	while (g_wShowVersionCnt < 300)//2013.5.30
	{
		IWDG_Feed();					//Î¹¹·
		if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))led_play();///15.03.19
	}
	clr_screen();
 
  }
		
	g_wShowVersionCnt = 0;
	ShowVersion(); // ²¥·Å½ÚÄ¿Ö®Ç°ÏÔÊ¾°æ±¾ºÅ
	while (g_wShowVersionCnt < 300)//2013.5.30
	{
		IWDG_Feed();					//Î¹¹·
		//ledplay_denglong();
		//clr_screen();
	}
	IWDG_Feed();					//Î¹¹·
	clr_screen();

//=======================================================ÎÒµÄ»¨ÑùÄ£Ê½===========================================================			
				//test_Pattern=3;	
			every_hang_byte=screen_len;
			hang_shu=scr_info.high;
			lh = 32;
			n_time_paly=0x10;
			
			/***¹Ø»úÇåÆÁ  2013.3.20 modify by zzw **/
// 			while ( scr_info.of ) 								//	¹Ø»ú
// 			{
// 				IWDG_Feed();//Î¹¹·
// 				memset(leddata, 0 ,8 * 1024);
// 				clr_screen();				//ÇåÆÁ
// 				if ( ucCh )
// 				{
// 					ucCh = 0;
// 					//led_play();
// 				}	
// 				while(1);						//	¹Ø»úÑ­»·
// 			}
			
			
			//if(test_Pattern)					//²âÊÔÄ£Ê½
			if(!test_Pattern)
			while(1)
			{
				test_Pattern=rand()%0x07;												//²»ÒªÈ«ÁÁºÍÈ«Ãð
				//if(test_Pattern<4)test_Pattern=7;
				//if(!test_Pattern)	test_Pattern+=0x01;
				//test_Pattern=2;
				switch(test_Pattern)
				{
					case 1:			//ÊúÌõ×óÒÆ
					{
						n_time_paly=0x28;
						dat=0xfc;
						clr_screen();//ÇåÆÁ
						while(n_time_paly)
						{
								dat<<=1;
								dat|=0x01;
								if(dat==0x7f)
									dat&=0xfe;
								if(dat==0xfd)
									dat&=0xfe;
								
								IWDG_Feed();//Î¹¹·	
								
								for(zz=0;zz<18;	zz++)
								{
									ledplay_denglongHei(~dat);
									delay(30000);	
									//clr_screen();//ÇåÆ
									delay(20000);	
									
									//ledplay_denglongHei(dat);
									delay(30000);	
									//clr_screen();//ÇåÆ
									delay(20000);	
									
								}
							
							if(n_time_paly)	n_time_paly--;	
						}
					} break;
					case 2:			//ÊúÌõÓÒÒÆ
					{
						n_time_paly=0x28;
						dat=0xc0;
						clr_screen();//ÇåÆÁ
						while(n_time_paly)
						{
								dat>>=1;
								if(dat==0x01)
									dat|=0x80;
								else if(dat==0x40)
									dat|=0x80;
							IWDG_Feed();//Î¹¹·		
							for(zz=0;zz<18;	zz++)
								{
									ledplay_denglongHei(dat);
									delay(20000);	
								//	clr_screen();//ÇåÆÁ
									delay(30000);	
									
									//ledplay_denglongHei(~dat);
									delay(20000);	
								//	clr_screen();//ÇåÆÁ
									delay(30000);	
									delay(30000);	
								}
							if(n_time_paly)	n_time_paly--;
						}
					} break;
					case 3:			//ºáÌõÏÂÒÆ
					{
						n_time_paly=0x05;
						clr_screen();//ÇåÆÁ
						memset(leddata, 0x00, hang_shu*screen_len);
						while(n_time_paly)
						{
							for(m=0;m<8;	) 		//ever 8  scr_info.high
							{
								for(m2=0;m2<hang_shu/8;m2++)		//Ã¿¸ô8ÐÐ
								{
									for(m3=0;m3<every_hang_byte;m3++)
									{
										leddata[( (m+(m2*8) ) * every_hang_byte )+m3 ]=0xff;

										leddata[( (m+1+(m2*8) ) * every_hang_byte )+m3 ]=0xff;
									}
								}
								IWDG_Feed();//Î¹¹·	
								for(zz=0;zz<25;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
								//	clr_screen();//ÇåÆÁ
									delay(30000);	
									
									ledplay_denglong();	
									delay(15000);	
								//	clr_screen();//ÇåÆÁ
									delay(25000);	
									
									//ledplay_denglong();	
									delay(16000);	
							//		clr_screen();//ÇåÆÁ
									delay(25000);	
								}
								
								memset(leddata, 0x00, hang_shu*screen_len);

								if(m==8)
										m=0;
								else
										m++;
							}	
						if(n_time_paly)	n_time_paly--;
						}
					}break;
					case 4:			//ºáÌõÉÏÒÆ
					{
						clr_screen();//ÇåÆÁ
						n_time_paly=0x05;
						memset(leddata, 0x00, hang_shu*screen_len);
						while(n_time_paly)
						{
							for(m=8;m>0;	) 		//ever 8  scr_info.high
							{
								for(m2=0;m2<hang_shu/8;m2++)		//Ã¿¸ô8ÐÐ
								{
									for(m3=0;m3<every_hang_byte;m3++)
									{
										leddata[( (m+m2*8) * every_hang_byte )+m3 ]=0xff;
										leddata[( (m-1+m2*8) * every_hang_byte )+m3 ]=0xff;
									}
								}
								IWDG_Feed();//Î¹¹·
								for(zz=0;zz<25;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
							//		clr_screen();//ÇåÆÁ
									delay(10000);	
									
									ledplay_denglong();	
									delay(15000);	
							//		clr_screen();//ÇåÆÁ
									delay(10000);	
									
									//ledplay_denglong();	
									delay(16000);	
							//		clr_screen();//ÇåÆÁ
									delay(25000);	
								}
								memset(leddata, 0x00, hang_shu*screen_len);
								//clear_disp_ram();

								if(m==0)
										m=8;
								else
										m--;
							}	
							if(n_time_paly)	n_time_paly--;	
						}
					}break;
					case 5:			//Ð±Ìõ×óÒÆ 
					{
						n_time_paly=0x60;
							//n=0xfc;
						n=0x03;
						clr_screen();//ÇåÆÁ
						while(n_time_paly)
						{

							for(i = 0; i < lh; i++){
								memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
								n = ((n << 1) | (n & 0x80) >> 7);
								//n = ((n << 1) & (n0x01);
								IWDG_Feed();//Î¹¹·	
								
								//for(zz=0;zz<10;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
								//	clr_screen();//ÇåÆÁ
									delay(20000);	
									
								}
								
							}	
							n = ((n << 1) | (n & 0x80) >> 7);//×óÒÆ
							if(n_time_paly)	n_time_paly--;	
						}

					}break;
					case 6:			                             //Ð±ÌõÓÒÒÆ	
					{
						n_time_paly=0x60;
						n=0xc0;
						//n=0xc0;
						clr_screen();//ÇåÆÁ
						while(n_time_paly)
						{
							for(i = 0; i < lh; i++){
								memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
								n = ((n >> 1) | (n & 0x01) << 7);
								
								IWDG_Feed();//Î¹¹·	
								//for(zz=0;zz<10;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
									delay(20000);	
								}
								
							}	
							n = ((n >> 1) | (n & 0x01) << 7);	//ÓÒÒÆ	
						if(n_time_paly)	n_time_paly--;	
						}

					} break;
				} 
 			}														//»¨ÑùÄ£Ê½
	
	pro_c=0;
	mm_set_en=1;						//ÔÊÐíÉèÖÃWIFIÃÜÂë
	r1=RTC_GetCounter();		//¶Á³örtc¼ÆÊýÆ÷ÖÐµÄÃëÖµ						
	while(1)
		{
			if(mm_set_en)
			{
					r2=RTC_GetCounter();//¶Á³örtc¼ÆÊýÆ÷ÖÐµÄÃëÖµ
					if((r2-r1)>40)	mm_set_en=0;	//³¬¹ý40Ãëºó¹Ø±ÕÃÜÂëÉè¶¨
					delay(20000);	
			}
			
			
			IWDG_Feed();//Î¹¹·
			/***¹Ø»úÇåÆÁ  2013.3.20 modify by zzw **/
			//while ( scr_info.of ) 								//	¹Ø»ú
			while ( scr_onoff )
			{
				IWDG_Feed();//Î¹¹·
				memset(leddata, 0 ,8 * 1024);
				clr_screen();
				if ( ucCh )
				{
					ucCh = 0;
					//led_play();
					
				}	
				while(1)
				{
						IWDG_Feed();//Î¹¹·
						delay(20000);	
				}					//¹Ø»úÑ­»·
			}
			if ( !ucCh )
			{
				ucCh = 1;
			}
	/********************end*****************/
			//led_play = ledplay_denglong;
			program_deal();		//½ÚÄ¿´¦Àí
			while(1)
			{	  //µ±Ç°½ÚÄ¿½áÊø±êÖ¾
					IWDG_Feed();//Î¹¹·
					
				
					if(i<=light)
						ledplay_denglong();	
					else
						clr_screen();	
					if(i<0x0f)	i++;		//ÁÁ¶Èµ÷Õû
						else i=0;		
				
				//ledplay_denglong();	

					if(light_bg<0X30)		//light_bg>´ËÖµÎªÈ¡Ïû±³¹â¹¦ÄÜ
						if(j>=light_bg)																				//light_bg 
						{
							j=0;
							ledplay_denglongHei(0xff);				//ËÍÈ«ÁÁ
						}
						j++;
						
					 datadeal();			  //Êý¾Ý´¦Àí  
				if(scr_info.s_t){
					if(scr_info.s_t_end)	break; //½ÚÄ¿³õÊ¼»¯Ê±ÇåÁã
					else{							 
						scr_info.st=0;
						continue;
					}		
				}  
				if(pro_end1)							
				{
						while(1)
						{
								//ledplay_denglong();	
								pro_end1=0;
								IWDG_Feed();//Î¹¹·
								//if(time_onoff)	
								if(!time_onoff)	
								{
									memset(&leddata[0x00*SCREEN_LEN], 0x00, SCREEN_LEN);
									memset(&leddata[0x01*SCREEN_LEN], 0x00, SCREEN_LEN);
									memset(&leddata[0x02*SCREEN_LEN], 0x00, SCREEN_LEN);
									memset(&leddata[0x03*SCREEN_LEN], 0x00, SCREEN_LEN);
									
									memset(&leddata[0x04*SCREEN_LEN], 0x00, SCREEN_LEN);
									memset(&leddata[0x05*SCREEN_LEN], 0x00, SCREEN_LEN);

									
									memset(&leddata[0x1a*SCREEN_LEN], 0x00, SCREEN_LEN);
									memset(&leddata[0x1b*SCREEN_LEN], 0x00, SCREEN_LEN);
									
									memset(&leddata[0x1c*SCREEN_LEN], 0x00, SCREEN_LEN);
									memset(&leddata[0x1d*SCREEN_LEN], 0x00, SCREEN_LEN);
									
// 									memset(&leddata[0x02*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);
// 									memset(&leddata[0x03*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);
// 									
// 									memset(&leddata[0x04*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);
// 									memset(&leddata[0x05*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);

 									
// 									memset(&leddata[0x1a*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);
// 									memset(&leddata[0x1b*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);
// 									
// 									memset(&leddata[0x1c*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);
// 									memset(&leddata[0x1d*SCREEN_LEN+8], 0x00, SCREEN_LEN-8);
									
									memset(&leddata[0x1e*SCREEN_LEN], 0x00, SCREEN_LEN);
									memset(&leddata[0x1f*SCREEN_LEN], 0x00, SCREEN_LEN);
									
									time_datadeal();											//ÏÔÊ¾Ê±¼ä
								}
								//if(light_bg==0x50)	time_datadeal();	
								else 				
										time_flag=1;

								if(ii<=light)
										ledplay_denglong();	
								else
										clr_screen();	
								if(ii<0x0f)	ii++;		//ÁÁ¶Èµ÷Õû
										else ii=0;	
								
								//ledplay_denglong();	

								jj++;
								if(light_bg<0X30)	//light_bg>´ËÖµÎªÈ¡Ïû±³¹â¹¦ÄÜ
										if(jj>=light_bg)																				//light_bg 
										{
												jj=0;
												ledplay_denglongHei(0xff);		//ËÍÈ«ÁÁ
										}
								if(time_flag==1){time_flag=0;break;}
						}
						
						if(!interval_pattern_flag)					//¼ä¸ô»¨Ñù±êÖ¾£¬Æô¶¯Ä¬ÈÏ¹Ø±Õ
						{
							if(wz_paly_n<1) wz_paly_n++;
							else
							{
 								wz_paly_n=0;
// 								clear_disp_ram();	
// 								//clr_screen();
// 								ShowVersion(); 
// 								delay(8800000000);		
// 								clear_disp_ram();	
								Pattern_n=0x04;							//Ã¿´Î²¥·Å¼¸¸ö»¨Ñù
								while(Pattern_n)
								{
									test_Pattern=rand()%0x07;
									switch(test_Pattern)
									{
										case 1:			//ÊúÌõ×óÒÆ
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x28;
											dat=0xfc;
											clr_screen();//ÇåÆÁ
											while(n_time_paly)
											{
													dat<<=1;
													dat|=0x01;
													if(dat==0x7f)
														dat&=0xfe;
													if(dat==0xfd)
														dat&=0xfe;
													
													IWDG_Feed();//Î¹¹·	
													
													for(zz=0;zz<18;	zz++)
													{
														ledplay_denglongHei(~dat);
														delay(30000);	
														//clr_screen();//ÇåÆ
														delay(20000);	
														
														//ledplay_denglongHei(dat);
														delay(30000);	
														//clr_screen();//ÇåÆ
														delay(20000);	
														
													}
												
												if(n_time_paly)	n_time_paly--;	
											}
										} break;
										case 2:			//ÊúÌõÓÒÒÆ
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x28;
											dat=0xc0;
											clr_screen();//ÇåÆÁ
											while(n_time_paly)
											{
													dat>>=1;
													if(dat==0x01)
														dat|=0x80;
													else if(dat==0x40)
														dat|=0x80;
												IWDG_Feed();//Î¹¹·		
												for(zz=0;zz<18;	zz++)
													{
														ledplay_denglongHei(dat);
														delay(20000);	
													//	clr_screen();//ÇåÆÁ
														delay(30000);	
														
														//ledplay_denglongHei(~dat);
														delay(20000);	
													//	clr_screen();//ÇåÆÁ
														delay(30000);	
														delay(30000);	
													}
												if(n_time_paly)	n_time_paly--;
											}
										} break;
										case 3:			//ºáÌõÏÂÒÆ
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x05;
											clr_screen();//ÇåÆÁ
											memset(leddata, 0x00, hang_shu*screen_len);
											while(n_time_paly)
											{
												for(m=0;m<8;	) 		//ever 8  scr_info.high
												{
													for(m2=0;m2<hang_shu/8;m2++)		//Ã¿¸ô8ÐÐ
													{
														for(m3=0;m3<every_hang_byte;m3++)
														{
															leddata[( (m+(m2*8) ) * every_hang_byte )+m3 ]=0xff;

															leddata[( (m+1+(m2*8) ) * every_hang_byte )+m3 ]=0xff;
														}
													}
													IWDG_Feed();//Î¹¹·	
													for(zz=0;zz<25;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
													//	clr_screen();//ÇåÆÁ
														delay(30000);	
														
														ledplay_denglong();	
														delay(15000);	
													//	clr_screen();//ÇåÆÁ
														delay(25000);	
														
														//ledplay_denglong();	
														delay(16000);	
												//		clr_screen();//ÇåÆÁ
														delay(25000);	
													}
													
													memset(leddata, 0x00, hang_shu*screen_len);

													if(m==8)
															m=0;
													else
															m++;
												}	
											if(n_time_paly)	n_time_paly--;
											}
										}break;
										case 4:			//ºáÌõÉÏÒÆ
										{
											if(Pattern_n)Pattern_n--;
											clr_screen();//ÇåÆÁ
											n_time_paly=0x05;
											memset(leddata, 0x00, hang_shu*screen_len);
											while(n_time_paly)
											{
												for(m=8;m>0;	) 		//ever 8  scr_info.high
												{
													for(m2=0;m2<hang_shu/8;m2++)		//Ã¿¸ô8ÐÐ
													{
														for(m3=0;m3<every_hang_byte;m3++)
														{
															leddata[( (m+m2*8) * every_hang_byte )+m3 ]=0xff;
															leddata[( (m-1+m2*8) * every_hang_byte )+m3 ]=0xff;
														}
													}
													IWDG_Feed();//Î¹¹·
													for(zz=0;zz<25;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
												//		clr_screen();//ÇåÆÁ
														delay(10000);	
														
														ledplay_denglong();	
														delay(15000);	
												//		clr_screen();//ÇåÆÁ
														delay(10000);	
														
														//ledplay_denglong();	
														delay(16000);	
												//		clr_screen();//ÇåÆÁ
														delay(25000);	
													}
													memset(leddata, 0x00, hang_shu*screen_len);
													//clear_disp_ram();

													if(m==0)
															m=8;
													else
															m--;
												}	
												if(n_time_paly)	n_time_paly--;	
											}
										}break;
										case 5:			//Ð±Ìõ×óÒÆ 
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x60;
												//n=0xfc;
											n=0x03;
											clr_screen();//ÇåÆÁ
											while(n_time_paly)
											{

												for(i = 0; i < lh; i++){
													memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
													n = ((n << 1) | (n & 0x80) >> 7);
													//n = ((n << 1) & (n0x01);
													IWDG_Feed();//Î¹¹·	
													
													//for(zz=0;zz<10;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
													//	clr_screen();//ÇåÆÁ
														delay(20000);	
														
													}
													
												}	
												n = ((n << 1) | (n & 0x80) >> 7);//×óÒÆ
												if(n_time_paly)	n_time_paly--;	
											}

										}break;
										case 6:			                             //Ð±ÌõÓÒÒÆ	
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x60;
											n=0xc0;
											//n=0xc0;
											clr_screen();//ÇåÆÁ
											while(n_time_paly)
											{
												for(i = 0; i < lh; i++){
													memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
													n = ((n >> 1) | (n & 0x01) << 7);
													
													IWDG_Feed();//Î¹¹·	
													//for(zz=0;zz<10;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
														delay(20000);	
													}
													
												}	
												n = ((n >> 1) | (n & 0x01) << 7);	//ÓÒÒÆ	
											if(n_time_paly)	n_time_paly--;	
											}

										} break;
										
									} 
									//delay(3000);
								}							//»¨ÑùÄ£Ê½
								clear_disp_ram();	
							}	
						}
				}
				if(scr_info.st)break;	
			}
		
			pro_c++;	
			pro_c %= pro_num;
		}	
}
