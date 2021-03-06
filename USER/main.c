/*******************************************************************************
版本号 1400

日期 2015.01.15

版本   程序整合版
 
1月12日 优化关机
*--------------------------信鸽定制版 15.01.10 flw----------------------------*/
 
 
#include "stmlib.h"
#include "export.h"
#include "clock.h"
#include "led.h"
#include "hb.h"
//#include "stdlib.h"

#define		INTERVAL_PATTERN_ADDR			0x22		//间隔花样开关
#define		TIME_ONOFF_ADDR			0x20		//时间开关
#define		DISP_PATTERN_ADDR		0x21		//显示模式
#define		BRIGHT_BG_ADDR			0x2a		//背景亮度
#define		BRIGHT_WORD_ADDR		0x29		//文字亮度



 
//定义外部变量
extern struct AreaStruct area[8]; //分区属性结构
extern struct ScrStruct  scr_info;//屏属性结构
extern unsigned char pro_num,info_num,pro_c; //节目总数、信息个数、信息计数器
extern unsigned char sim_buf_red[];//缓冲区，存放从内部flash中读出的数据
extern unsigned char flashcheckerr;//初始值屏参
extern unsigned char leddata[];//显示缓冲区
extern void (*led_play)(void);

extern unsigned long flash_addr;
extern unsigned short g_wShowVersionCnt;
extern unsigned short screen_len;
extern unsigned char bright;
extern unsigned char g_ucChangeScan;
extern unsigned char wifi_init_flag,wifi_config,wifiinit,timer1_flag,config_step;                    //wifi 初始化标志位 //上电启动 0；配置 1；正常工作 2；
extern unsigned char screeniniterr;

extern	unsigned char		mm_set_en;		//=1允许设置WIFI密码
extern	unsigned char		scr_onoff;					//屏幕开关标志  0开  1关



/**************************************************************************
** 函数名称 ： delay
** 函数功能 ： 延时函数  
** 入口参数 ： 需要延时的时间
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 2013.3.18 朱振威	
**************************************************************************/
static void delay(unsigned long time){
	while(time--);
}


/**************************************************************************
** 函数名称 ： PowerManagerInit
** 函数功能 ： 电源管理初始化 
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 2013.4.06 朱振威	
**************************************************************************/
static void PowerManagerInit(void)
{
	#define rPWR_CR  (*(volatile unsigned long *)0x40007000)
	
	/* 根据STM32数据手册第44页设定电压阀值，这里是2.9V */
	rPWR_CR |= (7 << 5); 
	/* 启动PWR功能 */
	rPWR_CR |= (1 << 4);
}


/**************************************************************************
** 函数名称 ： PowerManagerInit
** 函数功能 ： 低压检测中断
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 电压越过阀值（2.9）伏时会进入此中断
** 修改日志 ： 2013.4.06 朱振威	
**************************************************************************/
void PVD_IRQHandler(void)
{

	EXTI->PR=1<<16;//EXTI_ClearITPendingBit(EXTI_Line16);
	/* 电压越过阀值复位一次 */
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

//主函数
int rtc_error_flag;//这个如果RTC初始化失败则置位
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
	unsigned char wz_paly_n=0;		//汉字显示次数
	unsigned char interval_pattern_flag;		//间隔播放花样标志
	unsigned char Pattern_n;			//花样播放几遍
	
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
	
	u32 r1,r2;										//存储时间秒数
	
	
	scr_info.d_l=0;
	
	if(RCC->CSR&0x10000000)
		zz=1; 
	
	RCC_Configuration();  //RCC时钟初始化
	GPIO_Configuration(); //GPIO初始化
	clr_screen();					//清屏=================================================================
	NVIC_Configuration(); //中断控制器初始化		
	PowerManagerInit();   //电源管理初始化 2013.04.06增加
	IWDG_Init(7,4000);    	//与分频数为64,重载值为625,溢出时间为25s   
	rtc_error_flag=rtc_init();    //RTC初始化
	
	
	
#ifdef ConMod2	
 uart2_init(115200);

	if(BR==1)  
  		uart_init(115200);	//初始化波特率
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

		spi_init(0x327);//spi 初始化，32分频
		ch734init();//初始化并读取数据
	#endif
	spi_init(0x30f);//spi 初始化，4分频
		
 	//flash_check();//检测flash中有无数据		=========================================================
 	screen_init();//屏信息初始化
	timer3_init();//定时器3初始化
	clr_screen();//清屏
		IWDG_Feed();					//喂狗
/************************自检*********12.11.15******************************/
	if (!(GPIOA->IDR & (1 << 13))) 
	{		   //如果GPA13管脚为低电平   按键
			dwTime = 0x20000;//dwTime = 0x200000;//2014.08.28 延时100ms
			while (dwTime --> 5)
			{IWDG_Feed();//喂狗
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
			dwTime = 0xA000000;//0x600000;//2014.08.28 延时60s
			while ((dwTime --> 5) && (ucCh != 1))
			{IWDG_Feed();//喂狗
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
				{IWDG_Feed();//喂狗
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
					
					if(flash_read(0)!=0xff){ //判断是否擦除完
							flash_byte_write(0x0);
							flash_chip_erase(); //再次擦除
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
					sim_buf_red[0x55] = 0xab;//0xab; //消除复位重新配制
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
					IWDG_Feed();//喂狗//2014.09.13
					if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))led_play();///15.03.19
				}
				
				
					#endif
				

	// 			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位
	// 			
	// 			while(1);
								
				} 
			}
	}
	IWDG_Feed();					//喂狗
/**********************************************************************************/
  exti_init(); //按键外部中断初始化	
	
	flash_read_write(1); 						//0x29 存放文字亮度值
	interval_pattern_flag=sim_buf_red[INTERVAL_PATTERN_ADDR];				//read间隔花样标志
	light=sim_buf_red[0x29]&0xf0;
	light>>=4;
	if(light>0X0f)light=0x0f;
	//light=16-light;
	
	
	//light+=1;								//最大亮度降低两个档
	
	light_bg=sim_buf_red[BRIGHT_BG_ADDR]+0x01;   //背光亮度命令0~15   ===================================
	if(light_bg>0x10)	light_bg=0x10;
	if(light_bg<0x02) light_bg=0x02;						//限制最大亮度，此值越小  越亮
	light_bg*=0X03;	
	//light_bg-=0X02;						
	
	time_onoff=sim_buf_red[TIME_ONOFF_ADDR];
	test_Pattern=sim_buf_red[DISP_PATTERN_ADDR];			//显示花样
//	if(test_Pattern>0x01)test_Pattern=0x01;
	//if

#ifdef  WIFI_CARD_5350	
	if(wait_wifi_tt!=0x55)
	{
		start_wifi_init=1;  //开始上电配制
		wifiinit=0;
		wifi_init_flag=0;
		timer1_flag=0;
		config_step=0;
		timer1_init(); 				
	}	
#endif
	
	
	IWDG_Feed();					//喂狗
	
	clr_screen();
	if(screeniniterr==0)
	{
		if(flashcheckerr==1)//读取节目屏参
		{
			screeniniterr=1;
		}
		else if((flashcheckerr==2)||(flashcheckerr==3))//读取节目屏参
		{
			screeniniterr=5;
		}
	}
	
	if(screeniniterr)//节目出错
		show_err(screeniniterr);

	if(rtc_error_flag==0x01)//显示  校时失败
	{
	g_wShowVersionCnt = 0;
	
		ShowRtcError(); // 播放节目之前显示版本号和发布日期
	while (g_wShowVersionCnt < 300)//2013.5.30
	{
		IWDG_Feed();					//喂狗
		if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5))led_play();///15.03.19
	}
	clr_screen();
 
  }
		
	g_wShowVersionCnt = 0;
	ShowVersion(); // 播放节目之前显示版本号
	while (g_wShowVersionCnt < 300)//2013.5.30
	{
		IWDG_Feed();					//喂狗
		//ledplay_denglong();
		//clr_screen();
	}
	IWDG_Feed();					//喂狗
	clr_screen();

//=======================================================我的花样模式===========================================================			
				//test_Pattern=3;	
			every_hang_byte=screen_len;
			hang_shu=scr_info.high;
			lh = 32;
			n_time_paly=0x10;
			
			/***关机清屏  2013.3.20 modify by zzw **/
// 			while ( scr_info.of ) 								//	关机
// 			{
// 				IWDG_Feed();//喂狗
// 				memset(leddata, 0 ,8 * 1024);
// 				clr_screen();				//清屏
// 				if ( ucCh )
// 				{
// 					ucCh = 0;
// 					//led_play();
// 				}	
// 				while(1);						//	关机循环
// 			}
			
			
			//if(test_Pattern)					//测试模式
			if(!test_Pattern)
			while(1)
			{
				test_Pattern=rand()%0x07;												//不要全亮和全灭
				//if(test_Pattern<4)test_Pattern=7;
				//if(!test_Pattern)	test_Pattern+=0x01;
				//test_Pattern=2;
				switch(test_Pattern)
				{
					case 1:			//竖条左移
					{
						n_time_paly=0x28;
						dat=0xfc;
						clr_screen();//清屏
						while(n_time_paly)
						{
								dat<<=1;
								dat|=0x01;
								if(dat==0x7f)
									dat&=0xfe;
								if(dat==0xfd)
									dat&=0xfe;
								
								IWDG_Feed();//喂狗	
								
								for(zz=0;zz<18;	zz++)
								{
									ledplay_denglongHei(~dat);
									delay(30000);	
									//clr_screen();//清�
									delay(20000);	
									
									//ledplay_denglongHei(dat);
									delay(30000);	
									//clr_screen();//清�
									delay(20000);	
									
								}
							
							if(n_time_paly)	n_time_paly--;	
						}
					} break;
					case 2:			//竖条右移
					{
						n_time_paly=0x28;
						dat=0xc0;
						clr_screen();//清屏
						while(n_time_paly)
						{
								dat>>=1;
								if(dat==0x01)
									dat|=0x80;
								else if(dat==0x40)
									dat|=0x80;
							IWDG_Feed();//喂狗		
							for(zz=0;zz<18;	zz++)
								{
									ledplay_denglongHei(dat);
									delay(20000);	
								//	clr_screen();//清屏
									delay(30000);	
									
									//ledplay_denglongHei(~dat);
									delay(20000);	
								//	clr_screen();//清屏
									delay(30000);	
									delay(30000);	
								}
							if(n_time_paly)	n_time_paly--;
						}
					} break;
					case 3:			//横条下移
					{
						n_time_paly=0x05;
						clr_screen();//清屏
						memset(leddata, 0x00, hang_shu*screen_len);
						while(n_time_paly)
						{
							for(m=0;m<8;	) 		//ever 8  scr_info.high
							{
								for(m2=0;m2<hang_shu/8;m2++)		//每隔8行
								{
									for(m3=0;m3<every_hang_byte;m3++)
									{
										leddata[( (m+(m2*8) ) * every_hang_byte )+m3 ]=0xff;

										leddata[( (m+1+(m2*8) ) * every_hang_byte )+m3 ]=0xff;
									}
								}
								IWDG_Feed();//喂狗	
								for(zz=0;zz<25;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
								//	clr_screen();//清屏
									delay(30000);	
									
									ledplay_denglong();	
									delay(15000);	
								//	clr_screen();//清屏
									delay(25000);	
									
									//ledplay_denglong();	
									delay(16000);	
							//		clr_screen();//清屏
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
					case 4:			//横条上移
					{
						clr_screen();//清屏
						n_time_paly=0x05;
						memset(leddata, 0x00, hang_shu*screen_len);
						while(n_time_paly)
						{
							for(m=8;m>0;	) 		//ever 8  scr_info.high
							{
								for(m2=0;m2<hang_shu/8;m2++)		//每隔8行
								{
									for(m3=0;m3<every_hang_byte;m3++)
									{
										leddata[( (m+m2*8) * every_hang_byte )+m3 ]=0xff;
										leddata[( (m-1+m2*8) * every_hang_byte )+m3 ]=0xff;
									}
								}
								IWDG_Feed();//喂狗
								for(zz=0;zz<25;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
							//		clr_screen();//清屏
									delay(10000);	
									
									ledplay_denglong();	
									delay(15000);	
							//		clr_screen();//清屏
									delay(10000);	
									
									//ledplay_denglong();	
									delay(16000);	
							//		clr_screen();//清屏
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
					case 5:			//斜条左移 
					{
						n_time_paly=0x60;
							//n=0xfc;
						n=0x03;
						clr_screen();//清屏
						while(n_time_paly)
						{

							for(i = 0; i < lh; i++){
								memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
								n = ((n << 1) | (n & 0x80) >> 7);
								//n = ((n << 1) & (n0x01);
								IWDG_Feed();//喂狗	
								
								//for(zz=0;zz<10;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
								//	clr_screen();//清屏
									delay(20000);	
									
								}
								
							}	
							n = ((n << 1) | (n & 0x80) >> 7);//左移
							if(n_time_paly)	n_time_paly--;	
						}

					}break;
					case 6:			                             //斜条右移	
					{
						n_time_paly=0x60;
						n=0xc0;
						//n=0xc0;
						clr_screen();//清屏
						while(n_time_paly)
						{
							for(i = 0; i < lh; i++){
								memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
								n = ((n >> 1) | (n & 0x01) << 7);
								
								IWDG_Feed();//喂狗	
								//for(zz=0;zz<10;	zz++)
								{
									ledplay_denglong();	
									delay(15000);	
									delay(20000);	
								}
								
							}	
							n = ((n >> 1) | (n & 0x01) << 7);	//右移	
						if(n_time_paly)	n_time_paly--;	
						}

					} break;
				} 
 			}														//花样模式
	
	pro_c=0;
	mm_set_en=1;						//允许设置WIFI密码
	r1=RTC_GetCounter();		//读出rtc计数器中的秒值						
	while(1)
		{
			if(mm_set_en)
			{
					r2=RTC_GetCounter();//读出rtc计数器中的秒值
					if((r2-r1)>40)	mm_set_en=0;	//超过40秒后关闭密码设定
					delay(20000);	
			}
			
			
			IWDG_Feed();//喂狗
			/***关机清屏  2013.3.20 modify by zzw **/
			//while ( scr_info.of ) 								//	关机
			while ( scr_onoff )
			{
				IWDG_Feed();//喂狗
				memset(leddata, 0 ,8 * 1024);
				clr_screen();
				if ( ucCh )
				{
					ucCh = 0;
					//led_play();
					
				}	
				while(1)
				{
						IWDG_Feed();//喂狗
						delay(20000);	
				}					//关机循环
			}
			if ( !ucCh )
			{
				ucCh = 1;
			}
	/********************end*****************/
			//led_play = ledplay_denglong;
			program_deal();		//节目处理
			while(1)
			{	  //当前节目结束标志
					IWDG_Feed();//喂狗
					
				
					if(i<=light)
						ledplay_denglong();	
					else
						clr_screen();	
					if(i<0x0f)	i++;		//亮度调整
						else i=0;		
				
				//ledplay_denglong();	

					if(light_bg<0X30)		//light_bg>此值为取消背光功能
						if(j>=light_bg)																				//light_bg 
						{
							j=0;
							ledplay_denglongHei(0xff);				//送全亮
						}
						j++;
						
					 datadeal();			  //数据处理  
				if(scr_info.s_t){
					if(scr_info.s_t_end)	break; //节目初始化时清零
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
								IWDG_Feed();//喂狗
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
									
									time_datadeal();											//显示时间
								}
								//if(light_bg==0x50)	time_datadeal();	
								else 				
										time_flag=1;

								if(ii<=light)
										ledplay_denglong();	
								else
										clr_screen();	
								if(ii<0x0f)	ii++;		//亮度调整
										else ii=0;	
								
								//ledplay_denglong();	

								jj++;
								if(light_bg<0X30)	//light_bg>此值为取消背光功能
										if(jj>=light_bg)																				//light_bg 
										{
												jj=0;
												ledplay_denglongHei(0xff);		//送全亮
										}
								if(time_flag==1){time_flag=0;break;}
						}
						
						if(!interval_pattern_flag)					//间隔花样标志，启动默认关闭
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
								Pattern_n=0x04;							//每次播放几个花样
								while(Pattern_n)
								{
									test_Pattern=rand()%0x07;
									switch(test_Pattern)
									{
										case 1:			//竖条左移
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x28;
											dat=0xfc;
											clr_screen();//清屏
											while(n_time_paly)
											{
													dat<<=1;
													dat|=0x01;
													if(dat==0x7f)
														dat&=0xfe;
													if(dat==0xfd)
														dat&=0xfe;
													
													IWDG_Feed();//喂狗	
													
													for(zz=0;zz<18;	zz++)
													{
														ledplay_denglongHei(~dat);
														delay(30000);	
														//clr_screen();//清�
														delay(20000);	
														
														//ledplay_denglongHei(dat);
														delay(30000);	
														//clr_screen();//清�
														delay(20000);	
														
													}
												
												if(n_time_paly)	n_time_paly--;	
											}
										} break;
										case 2:			//竖条右移
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x28;
											dat=0xc0;
											clr_screen();//清屏
											while(n_time_paly)
											{
													dat>>=1;
													if(dat==0x01)
														dat|=0x80;
													else if(dat==0x40)
														dat|=0x80;
												IWDG_Feed();//喂狗		
												for(zz=0;zz<18;	zz++)
													{
														ledplay_denglongHei(dat);
														delay(20000);	
													//	clr_screen();//清屏
														delay(30000);	
														
														//ledplay_denglongHei(~dat);
														delay(20000);	
													//	clr_screen();//清屏
														delay(30000);	
														delay(30000);	
													}
												if(n_time_paly)	n_time_paly--;
											}
										} break;
										case 3:			//横条下移
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x05;
											clr_screen();//清屏
											memset(leddata, 0x00, hang_shu*screen_len);
											while(n_time_paly)
											{
												for(m=0;m<8;	) 		//ever 8  scr_info.high
												{
													for(m2=0;m2<hang_shu/8;m2++)		//每隔8行
													{
														for(m3=0;m3<every_hang_byte;m3++)
														{
															leddata[( (m+(m2*8) ) * every_hang_byte )+m3 ]=0xff;

															leddata[( (m+1+(m2*8) ) * every_hang_byte )+m3 ]=0xff;
														}
													}
													IWDG_Feed();//喂狗	
													for(zz=0;zz<25;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
													//	clr_screen();//清屏
														delay(30000);	
														
														ledplay_denglong();	
														delay(15000);	
													//	clr_screen();//清屏
														delay(25000);	
														
														//ledplay_denglong();	
														delay(16000);	
												//		clr_screen();//清屏
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
										case 4:			//横条上移
										{
											if(Pattern_n)Pattern_n--;
											clr_screen();//清屏
											n_time_paly=0x05;
											memset(leddata, 0x00, hang_shu*screen_len);
											while(n_time_paly)
											{
												for(m=8;m>0;	) 		//ever 8  scr_info.high
												{
													for(m2=0;m2<hang_shu/8;m2++)		//每隔8行
													{
														for(m3=0;m3<every_hang_byte;m3++)
														{
															leddata[( (m+m2*8) * every_hang_byte )+m3 ]=0xff;
															leddata[( (m-1+m2*8) * every_hang_byte )+m3 ]=0xff;
														}
													}
													IWDG_Feed();//喂狗
													for(zz=0;zz<25;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
												//		clr_screen();//清屏
														delay(10000);	
														
														ledplay_denglong();	
														delay(15000);	
												//		clr_screen();//清屏
														delay(10000);	
														
														//ledplay_denglong();	
														delay(16000);	
												//		clr_screen();//清屏
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
										case 5:			//斜条左移 
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x60;
												//n=0xfc;
											n=0x03;
											clr_screen();//清屏
											while(n_time_paly)
											{

												for(i = 0; i < lh; i++){
													memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
													n = ((n << 1) | (n & 0x80) >> 7);
													//n = ((n << 1) & (n0x01);
													IWDG_Feed();//喂狗	
													
													//for(zz=0;zz<10;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
													//	clr_screen();//清屏
														delay(20000);	
														
													}
													
												}	
												n = ((n << 1) | (n & 0x80) >> 7);//左移
												if(n_time_paly)	n_time_paly--;	
											}

										}break;
										case 6:			                             //斜条右移	
										{
											if(Pattern_n)Pattern_n--;
											n_time_paly=0x60;
											n=0xc0;
											//n=0xc0;
											clr_screen();//清屏
											while(n_time_paly)
											{
												for(i = 0; i < lh; i++){
													memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
													n = ((n >> 1) | (n & 0x01) << 7);
													
													IWDG_Feed();//喂狗	
													//for(zz=0;zz<10;	zz++)
													{
														ledplay_denglong();	
														delay(15000);	
														delay(20000);	
													}
													
												}	
												n = ((n >> 1) | (n & 0x01) << 7);	//右移	
											if(n_time_paly)	n_time_paly--;	
											}

										} break;
										
									} 
									//delay(3000);
								}							//花样模式
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
