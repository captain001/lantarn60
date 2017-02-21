#include "stmlib.h"
#include <led.h>

u8 cardNum[cardLength];//卡型显示对应的数组


#define NVIC_GROUP NVIC_PriorityGroup_3

const u8 year_date[]={
 0x0C,0x96,0x45,0x4d,0x4A,0xB8,0x0d,0x4A,0x4C, // 2000-2002
 0x0d,0xA5,0x41,0x25,0xAA,0xB6,0x05,0x6A,0x49, // 2003-2005
 0x7A,0xAd,0xBd,0x02,0x5d,0x52,0x09,0x2d,0x47, // 2006-2008
 0x5C,0x95,0xBA,0x0A,0x95,0x4e,0x0B,0x4A,0x43, // 2009-2011
 0x4B,0x55,0x37,0x0A,0xd5,0x4A,0x95,0x5A,0xBf, // 2012-2014
 0x04,0xBA,0x53,0x0A,0x5B,0x48,0x65,0x2B,0xBC, // 2015-2017
 0x05,0x2B,0x50,0x0A,0x93,0x45,0x47,0x4A,0xB9, // 2018-2020
 0x06,0xAA,0x4C,0x0A,0xd5,0x41,0x24,0xdA,0xB6, // 2021-2023
 0x04,0xB6,0x4A,0x69,0x57,0x3d,0x0A,0x4e,0x51, // 2024-2026
 0x0d,0x26,0x46,0x5e,0x93,0x3A,0x0d,0x53,0x4d, // 2027-2029
 0x05,0xAA,0x43,0x36,0xB5,0x37,0x09,0x6d,0x4B, // 2030-2032
 0xB4,0xAe,0xBf,0x04,0xAd,0x53,0x0A,0x4d,0x48, // 2033-2035
 0x6d,0x25,0xBC,0x0d,0x25,0x4f,0x0d,0x52,0x44, // 2036-2038
 0x5d,0xAA,0x38,0x0B,0x5A,0x4C,0x05,0x6d,0x41, // 2039-2041
 0x24,0xAd,0xB6,0x04,0x9B,0x4A,0x7A,0x4B,0xBe, // 2042-2044
 0x0A,0x4B,0x51,0x0A,0xA5,0x46,0x5B,0x52,0xBA, // 2045-2047
 0x06,0xd2,0x4e,0x0A,0xdA,0x42,0x35,0x5B,0x37, // 2048-2050                              	 
};


const u8 card_style[22][16] = {  //显示卡型
{0x00,0x00,0x00,0xFC,0x42,0x42,0x42,0x7C,0x48,0x48,0x44,0x44,0x42,0xE3,0x00,0x00},//"R",0
{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x7E,0x42,0x42,0x42,0x42,0xE7,0x00,0x00},//"H",1
{0x00,0x00,0x00,0xE7,0x42,0x24,0x24,0x18,0x18,0x18,0x24,0x24,0x42,0xE7,0x00,0x00},//"X",2
{0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},//"B",3
{0x00,0x00,0x00,0xD6,0x92,0x92,0x92,0x92,0xAA,0xAA,0x6C,0x44,0x44,0x44,0x00,0x00},//"W",4
{0x00,0x00,0x00,0xE7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x3C,0x00,0x00},//"U",5
{0x00,0x00,0x00,0x3E,0x42,0x42,0x40,0x20,0x18,0x04,0x02,0x42,0x42,0x7C,0x00,0x00},//"S",6
{0x00,0x00,0x00,0x3C,0x44,0x44,0x80,0x80,0x80,0x8E,0x84,0x44,0x44,0x38,0x00,0x00},//"G",7
{0x00,0x00,0x00,0x10,0x10,0x18,0x28,0x28,0x24,0x3C,0x44,0x42,0x42,0xE7,0x00,0x00},//"A",8
{0x00,0x00,0x00,0xF8,0x44,0x44,0x44,0x78,0x44,0x42,0x42,0x42,0x44,0xF8,0x00,0x00},//"B",9
{0x00,0x00,0x00,0x3E,0x42,0x42,0x80,0x80,0x80,0x80,0x80,0x42,0x44,0x38,0x00,0x00},//"C",10
{0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},//"1",11
{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00},//"2",12
{0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x02,0x42,0x44,0x38,0x00,0x00},//"3",13
{0x00,0x00,0x00,0x04,0x0C,0x14,0x24,0x24,0x44,0x44,0x7E,0x04,0x04,0x1E,0x00,0x00},//"4",14
{0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00},//"5",15
{0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00},//"6",16
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//"-",17
{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},//"0",18
{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",19*/
{0x00,0x00,0x00,0x3E,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x88,0xF0},/*"J",20*/
{0x00,0x00,0x00,0xEE,0x6C,0x6C,0x6C,0x6C,0x54,0x54,0x54,0x54,0x54,0xD6,0x00,0x00},/*"M",21*/
};

const u8 date_table[]={31,28,31,30,31,30,31,31,30,31,30,31};
const u8 date_year[]={0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1};

//定义外部变量
extern u32 date32,time32; //存放日期、时间
extern u8 buf[20]; //缓存
extern u16 datn16; //存放农历

#define RTC_PRLL 0x8002  //32778
u8 Frequency=128;
extern int rtc_error_flag;
	
extern struct ScrStruct  scr_info;
extern unsigned char sim_buf_red[ ];		


/******************************************************************************************
** 函数名称 ： RCC_Configuration
** 函数功能 ： 处理器倍频初始化程序			   
** 入口参数 ： 无
** 出口参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改
*******************************************************************************************/
void RCC_Configuration(void)
{
	unsigned short temp=0;
	unsigned char PLL=0;
	
	PLL=0x1C;
	temp=0x05;
	
	Frequency=(temp+4)*8;	
///////////////RCC/在线升级需完全复位，否则主频改不过来///////////
	  /* Disable APB2 Peripheral Reset */
  RCC->APB2RSTR = 0x00000000;
  /* Disable APB1 Peripheral Reset */
  RCC->APB1RSTR = 0x00000000;
  /* FLITF and SRAM Clock ON */
  RCC->AHBENR = 0x00000014;
  /* Disable APB2 Peripheral Clock */
  RCC->APB2ENR = 0x00000000;
  /* Disable APB1 Peripheral Clock */
  RCC->APB1ENR = 0x00000000;
  /* Set HSION bit */
  RCC->CR |= (u32)0x00000001;
  /* Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits*/
  RCC->CFGR &= 0xF8FF0000;  
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= 0xFEF6FFFF;
  /* Reset HSEBYP bit */
  RCC->CR &= 0xFFFBFFFF;
  /* Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits */
  RCC->CFGR &= 0xFF80FFFF;
  /* Disable all interrupts */
  RCC->CIR = 0x00000000;
////////////////////////////////////////////
  
	RCC->CR |=0x00010000;     //使能外部高速时钟HSEON	 	
	while( (!(RCC->CR>>17))&&(temp<0xfff) )//等待外部时钟就绪
	{
		temp++;
	}
	if(temp>0xff0)
	{
		RCC->CR &=~0x00010000;
		RCC->CR |=0x00000001;
		RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
		RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
		RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
		RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
		RCC->CIR = 0x00000000;     //关闭所有中断		
	}
	else
	{
		temp=0;
		RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1; 
		RCC->CFGR|=PLL<<16;   //设置PLL值 2~16 
		RCC->CFGR|=1<<16;	  //PLLSRC ON  
		FLASH->ACR|=0x32;	  //FLASH 2个延时周期 

		RCC->CR|=0x01000000;  //PLLON 
		while(!(RCC->CR>>25));//等待PLL锁定 
		RCC->CFGR|=0x00000002;//PLL作为系统时钟	  
		while(temp!=0x02)     //等待PLL作为系统时钟设置成功 
		{    
		temp=RCC->CFGR>>2; 
		temp&=0x03;
		} 
	}
/**************************************************/
	temp=*(u8*)(0x0800FC18);					   //0x14-0x38
	if((temp>0xac)||(temp<0xa0))
	{
		PLL=0x38;
		temp=0x0c;
	}
	else 
	{

		temp&=0x0f;
		if(temp>=5)
		{
			temp=0x0c;
		}
		else if(temp==4)
		{
			temp=5;//4m
		}
		PLL=0x08+temp*4;
	}
	Frequency=(temp+4)*8;	
///////////////RCC/在线升级需完全复位，否则主频改不过来///////////
	  /* Disable APB2 Peripheral Reset */
  RCC->APB2RSTR = 0x00000000;
  /* Disable APB1 Peripheral Reset */
  RCC->APB1RSTR = 0x00000000;
  /* FLITF and SRAM Clock ON */
  RCC->AHBENR = 0x00000014;
  /* Disable APB2 Peripheral Clock */
  RCC->APB2ENR = 0x00000000;
  /* Disable APB1 Peripheral Clock */
  RCC->APB1ENR = 0x00000000;
  /* Set HSION bit */
  RCC->CR |= (u32)0x00000001;
  /* Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], ADCPRE[1:0] and MCO[2:0] bits*/
  RCC->CFGR &= 0xF8FF0000;  
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= 0xFEF6FFFF;
  /* Reset HSEBYP bit */
  RCC->CR &= 0xFFFBFFFF;
  /* Reset PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE bits */
  RCC->CFGR &= 0xFF80FFFF;
  /* Disable all interrupts */
  RCC->CIR = 0x00000000;
////////////////////////////////////////////
  
	RCC->CR |=0x00010000;     //使能外部高速时钟HSEON	 	
// 	while(!(RCC->CR>>17));
	while( (!(RCC->CR>>17))&&(temp<0xfff) )//等待外部时钟就绪
	{
		temp++;
	}
	if(temp>0xff0)
	{
		RCC->CR &=~0x00010000;
		RCC->CR |=0x00000001;
		RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
		RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
		RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
		RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
		RCC->CIR = 0x00000000;     //关闭所有中断		
	}
	else
	{
		temp=0;
		RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1; 
		RCC->CFGR|=PLL<<16;   //设置PLL值 2~16 
		RCC->CFGR|=1<<16;	  //PLLSRC ON  
		FLASH->ACR|=0x32;	  //FLASH 2个延时周期 

		RCC->CR|=0x01000000;  //PLLON 
		while(!(RCC->CR>>25));//等待PLL锁定 
		RCC->CFGR|=0x00000002;//PLL作为系统时钟	  
		while(temp!=0x02)     //等待PLL作为系统时钟设置成功 
		{    
		temp=RCC->CFGR>>2; 
		temp&=0x03;
		} 
	}	
	
	
	#ifdef VECT_TAB_0x800
		 SCB->VTOR = FLASH_BASE | 0x800; /* Vector Table Relocation in Internal FLASH. */
	#else
		 SCB->VTOR = FLASH_BASE | 0;
	#endif
}


/******************************************************************************************
** 函数名称 ： uart_init
** 函数功能 ： 串口的初始化程序			   
** 入口参数 ： 无
** 出口参数 ： 无
** 函数说明 ： 串口1设置为115200bps（默认）,8位数据，无效验位
** 修改日志 ： 刘晓琳 于2013年01月07日修改
*******************************************************************************************/
void uart_init(unsigned long b)
{	
	float temp;
	u16 mantissa;
	u16 fraction;
	
	temp=(float)(Frequency*1000000)/(b*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2RSTR|=1<<14;   //复位串口1
	RCC->APB2RSTR&=~(1<<14);//停止复位	   	   
	//波特率设置
 	USART1->BRR=mantissa; // 波特率设置	 
	USART1->CR1|=0X202C;  //1位停止,无校验位.
}
/**************************************************************************
** 函数名称 ： uart2_init
** 函数功能 ： 串口2初始化
** 入口参数 ： bound波特率
** 返回参数 ： 
** 函数说明 ： 
** 修改日志 ： 2013.5.20 wwd
**************************************************************************/
void uart2_init(unsigned long b)
{
	float temp;
	u16 mantissa;
	u16 fraction;
	
	temp=(float)((Frequency/2)*1000000)/(b*16);//得到USARTDIV
	mantissa=temp;				 //得到整数部分
	fraction=(temp-mantissa)*16; //得到小数部分	 
    mantissa<<=4;
	mantissa+=fraction;
	
	RCC->APB1RSTR|=1<<17;   //复位串口2
	RCC->APB1RSTR&=~(1<<17);//停止复位	   	   7
	//波特率设置
 	USART2->BRR=mantissa; // 波特率设置	 
	USART2->CR1|=0X202C;  //1位停止,无校验位.//开启中断//使能串口
}
/******************************************************************************************
** 函数名称 ： timer1_init
** 函数功能 ： 定时器1初始化		   
** 入口参数 ： 定时器赋的值s
** 出口参数 ： 无
** 函数说明 ： 
** 修改日志 ： ZGF 于2013年01月07日修改
***********************************************************************************/
 void timer1_init(void) 
 {
	u16  arr=0;
	arr=(Frequency*1000000)/2000;//1000000为1秒，2000为预分频
	
	RCC->APB2ENR|=1<<11;//TIM1时钟使能 
	TIM1->CR1&=0xFE;
	RCC->APB1RSTR|=1<<11;   //复位
	RCC->APB1RSTR&=~(1<<11);//停止复位		
	
 	TIM1->ARR=arr;  //设定计数器自动重装值//刚好1s  
	TIM1->PSC=2000;  //预分频器2000
	TIM1->DIER|=0x01;   //允许更新中断				   
  TIM1->CR1|=0x01;    //使能定时器1			

}
/******************************************************************************************
** 函数名称 ： timer_init
** 函数功能 ： 定时器2初始化		   
** 入口参数 ： 定时器赋的值s
** 出口参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改
*******************************************************************************************/
void timer_init(unsigned short s)	//输出一行100微秒 5/16  5/4		   
{
	TIM2->CR1&=0xFE;
	RCC->APB1RSTR|=1<<0;   //复位
	RCC->APB1RSTR&=~(1<<0);//停止复位		
	TIM2->ARR=s;
	TIM2->PSC=0x003F;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM2->DIER|=1;   //允许更新中断	  
	TIM2->CR1|=0x01;    //使能定时器 	
}

/******************************************************************************************
** 函数名称 ： timer3_init
** 函数功能 ： 定时器3初始化		   
** 入口参数 ： 无
** 出口参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改
*******************************************************************************************/
void timer3_init() 
{
	u16  arr=0;
	arr=(Frequency*5000)/2000;//1000000为1秒，2000为预分频

	TIM3->CR1&=0xFE;
	RCC->APB1RSTR|=1<<1;   //复位
	RCC->APB1RSTR&=~(1<<1);//停止复位		
 	TIM3->ARR=arr;  	//设定计数器自动重装值
	TIM3->PSC=2000;  	//预分频器2000	  
	TIM3->DIER|=1;   //允许更新中断	  
	TIM3->CR1|=0x01;    //使能定时器		
}

/******************************************************************************************
** 函数名称 ： timer4_init
** 函数功能 ： 定时器4初始化		   
** 入口参数 ： 定时器赋值的值n
** 出口参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改
*******************************************************************************************/
void timer_init4(unsigned short n)  //19.5US  MIN n=40
{
	TIM4->CR1&=0xFE;    //不使能定时器
	RCC->APB1RSTR|=1<<2;   //复位
	RCC->APB1RSTR&=~(1<<2);//停止复位		
 	TIM4->ARR=n;  	//设定计数器自动重装值
	TIM4->PSC=0x003F;  	//预分频器7200,得到10Khz的计数时钟		  
	TIM4->DIER|=1;   //允许更新中断	  
	TIM4->CR1&=0xFE;    //不使能定时器
 }
void TIM4_Cmd()
{
	TIM4->CR1|=0x01;
}


void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel)
{
	uint32_t tmppriority = 0x00;
	
    tmppriority = (uint32_t)NVIC_PreemptionPriority << 2;
    tmppriority |=  NVIC_SubPriority & 3;
    tmppriority = tmppriority << 0x04;
	
    NVIC->IP[NVIC_Channel] = tmppriority;
    
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[NVIC_Channel >> 0x05] =
      (uint32_t)0x01 << (NVIC_Channel & (uint8_t)0x1F);
}
/******************************************************************************************
** 函数名称 ： NVIC_Configuration
** 函数功能 ： 中断控制器初始化		   
** 入口参数 ： 无
** 出口参数 ： 无
** 函数说明 ： 配置各个中断的优先级
** 修改日志 ： 刘晓琳 于2013年01月07日修改
*******************************************************************************************/
void NVIC_Configuration(void) 					
{

	SCB->AIRCR = 0x05FA0000 | NVIC_GROUP ;
    MY_NVIC_Init(0,0,USART1_IRQn);
	MY_NVIC_Init(0,1,USART2_IRQn);
	MY_NVIC_Init(3,1,EXTI15_10_IRQn);
	MY_NVIC_Init(5,0,TIM1_UP_IRQn);
	MY_NVIC_Init(1,1,RTC_IRQn);
	//MY_NVIC_Init(2,0,TIM2_IRQn);
	MY_NVIC_Init(4,1,TIM3_IRQn);
	//MY_NVIC_Init(1,0,TIM4_IRQn);
	MY_NVIC_Init(6,1,PVD_IRQn);
}


/******************************************************************************************
** 函数名称 ： GPIO_Configuration
** 函数功能 ： GPIO端口的初始化程序			   
** 入口参数 ： 无
** 出口参数 ： 无
** 函数说明 ： 配置各个管脚
** 修改日志 ： 刘晓琳 于2013年01月07日修改
*******************************************************************************************/
void GPIO_Configuration(void)
{	
	#ifdef ConMod1
	RCC->APB2ENR |= (RCC_APB2Periph_GPIOA |RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1|RCC_APB2Periph_SPI1); //外设时钟		   
  RCC->APB1ENR |= (RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | RCC_APB1Periph_PWR | RCC_APB1Periph_BKP); 
		
	GPIOA->CRH=0;
	GPIOA->CRH=0x444334B3;
	
	GPIOA->CRL=0;
	GPIOA->CRL=0xBBB33333;	
	
	GPIOB->CRH=0;
	GPIOB->CRH=0x33333333;
	
	GPIOB->CRL=0;
	GPIOB->CRL=0x33484344;
	#elif ConMod2
	RCC->APB2ENR |= (RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB 
			|RCC_APB2Periph_TIM1 | RCC_APB2Periph_USART1|RCC_APB2Periph_SPI1|RCC_APB2Periph_AFIO); //外设时钟		   
  RCC->APB1ENR |= (RCC_APB1Periph_USART2 |RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 
			| RCC_APB1Periph_TIM4 | RCC_APB1Periph_PWR | RCC_APB1Periph_BKP); 
		
	GPIOA->CRH=0;
	GPIOA->CRH=0x444334B3;
	GPIOA->CRL=0;
	GPIOA->CRL=0x34334B44;//spi moni
	
	GPIOB->CRH=0;
	GPIOB->CRH=0x33333333;
	GPIOB->CRL=0;
	GPIOB->CRL=0x33383333;

	DBGMCU->CR&=0xFFFFFFDF;
	AFIO->MAPR&=0XF8FFFFFF;
	AFIO->MAPR|=0x02000000;
	#endif
}

/*******************************************************************************
* 函数名称  : spi_init
* 函数功能  : stm32f051单片机的spi端口初始化
* 入口参数  : 设置分频数set
* 返回参数  : 无
* 函数说明  : spi端口初始化
* 修改日志  : 刘晓琳 于2013年01月07日修改
*******************************************************************************/
 void spi_init(u16 set)
 {
	 #ifdef ConMod1
	 SPI1->CR1&=0xFFBF; 		//SPI设备不使能
	 RCC->APB2RSTR|=1<<12;   //复位
	 RCC->APB2RSTR&=~(1<<12);//停止复位		
	 SPI1->CR1=set; 		//SPI设备使能
	 SPI1->CRCPR =7;
	 SPI1->CR1|=0x40; 		//SPI设备使能
	 #elif ConMod2
	 	 //spi moni zhuan yingjian
	 GPIOA->CRL&=0x000FFFFF;
	 GPIOA->CRL|=0xBBB00000;	
	 
	 SPI1->CR1&=0xFFBF; 		//SPI设备不使能
	 RCC->APB2RSTR|=1<<12;   //复位
	 RCC->APB2RSTR&=~(1<<12);//停止复位		
	 SPI1->CR1=set; 		//SPI设备使能
	 SPI1->CRCPR =7;
	 SPI1->CR1|=0x40; 		//SPI设备使能
	 #endif
 }  

u8 send_byte(u8 TxData)
{					 
	while((SPI1->SR&2)==0);		
	SPI1->DR=TxData;	 	  		
	while((SPI1->SR&1)==0); 						    
	return SPI1->DR;          				    
}

#define GPIO_A 0
#define GPIO_B 1
#define GPIO_C 2
#define GPIO_D 3
#define GPIO_E 4
#define GPIO_F 5
#define GPIO_G 6 
#define FTIR   1  //下降沿触发
#define RTIR   2  //上升沿触发
#define FRIR   3  //双边沿

void Ex_NVIC_Config(u8 GPIOx,u8 BITx,u8 TRIM) 
{
	u8 EXTADDR;
	u8 EXTOFFSET;
	EXTADDR=BITx/4;
	EXTOFFSET=(BITx%4)*4;						   
	RCC->APB2ENR|=0x01;
	AFIO->EXTICR[EXTADDR]&=~(0x000F<<EXTOFFSET);
	AFIO->EXTICR[EXTADDR]|=GPIOx<<EXTOFFSET;
	EXTI->IMR|=1<<BITx;
 	if(TRIM&0x01)EXTI->FTSR|=1<<BITx;
	if(TRIM&0x02)EXTI->RTSR|=1<<BITx;
} 

/**************************************************************************
** 函数名称 ： exti_init
** 函数功能 ： 外部中断初始化
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 2013.3.18 朱振威	
**************************************************************************/
void exti_init()
{
	
  Ex_NVIC_Config(GPIO_A,13,FTIR); 			//下降沿触发
  Ex_NVIC_Config(GPIO_A,14,FTIR); 			//下降沿触发
	Ex_NVIC_Config(GPIO_A,16,FRIR);
}

/**************************************************************************
** 函数名称 ： RTC_GetCounter
** 函数功能 ： timecount秒钟数
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： wwd 2013.5.23
**************************************************************************/
u32 RTC_GetCounter()
{
	u32 timecount=0;  
	   
if(rtc_error_flag!=0x01)
{ 
	timecount=RTC->CNTH;//得到计数器中的值(秒钟数)
	timecount<<=16;
	timecount+=RTC->CNTL;	
}
	return timecount;
}
/**************************************************************************
** 函数名称 ： RTC_SET
** 函数功能 ：count 秒钟数
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： wwd 2013.5.23
**************************************************************************/
void RTC_SET(u32 count)
{
// 	u32 r3;
	if(rtc_error_flag!=0x01)
	{
	while(!(RTC->CRL&(1<<5)));   //等待RTC寄存器操作完成		

	
	RCC->APB1ENR|=1<<28;//使能电源时钟
	RCC->APB1ENR|=1<<27;//使能备份时钟
	PWR->CR|=1<<8;    //取消备份区写保护

	RTC->CRL|=1<<4;   //允许配置 
		
	RTC->CNTL=count&0xffff;
	RTC->CNTH=count>>16;
	RTC->CRL&=~(1<<4);//配置更新
	
	while(!(RTC->CRL&(1<<5))) ;  //等待RTC寄存器操作完成		

  }
}
/******************************************************
** 函数名称 ： gettime
** 函数功能 ： 获得当前的时间
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 将寄存器中读出的秒值，转为年月日星期和时分秒
**             分别存入data32和time32中
** 修改日志 ： 2013.3.18 朱振威	
**********************************************************/
void gettime()
{
	
	u8 x,i;
	u32 r1,r2,r3;
	r1=RTC_GetCounter();//读出rtc计数器中的秒值
	r2=24*3600;
	r3=r1/r2;  //当前时间的日期-日
	r2=r1%r2;  //当天的时间秒值
	time32=r2/3600; //此时的时
	time32<<=8;
	time32|=(r2%3600)/60;//此时的分
	time32<<=8;
	time32|=(r2%3600)%60;//此时的秒值

	i=1;
	r1=0;
	for(i=1;i<50;i++){
		if(r3>r1&&r3<=(r1+date_year[i]+365))
			break;
		r1+=date_year[i]+365;
	}

	date32=i;		    	//年 +2000	  
	date32<<=8;
	x=r3%7;
	if(x==0)x=7;
	date32|=x;
	date32<<=8;
	r1=r3-((i-1)/4)*1461-((i-1)%4)*365;
	if(!(i%4))x=1;
	else x=0;

	if(r1<=31+28+x) {
		if(r1<=31)date32|=1;
		else date32|=2;
	}
	else {	   
	r3=x;
	
	for(i=0;i<12;i++){
		if((r1>r3)&&(r1<=r3+date_table[i]))
			break;
		r3+=date_table[i];
	}
	
	date32|=i+1;
	}

	date32<<=8;
	if((r1>0)&&(r1<=31))			r2=r1;										//1月
	else if((r1>31)&&(r1<=59+x))	r2=r1-31; 									//2
	else if((r1>59+x)&&(r1<=90+x)) 	r2=r1-(59+x);								//3
	else if((r1>90+x)&&(r1<=120+x))	r2=r1-(90+x);								//4
	else if((r1>120+x)&&(r1<=151+x))r2=r1-(120+x);								//5
	else if((r1>151+x)&&(r1<=181+x))r2=r1-(151+x);								//6
	else if((r1>181+x)&&(r1<=212+x))r2=r1-(181+x);								//7
	else if((r1>212+x)&&(r1<=243+x))r2=r1-(212+x);								//8
	else if((r1>243+x)&&(r1<=273+x))r2=r1-(243+x);								//9
	else if((r1>273+x)&&(r1<=304+x))r2=r1-(273+x);								//10
	else if((r1>304+x)&&(r1<=334+x))r2=r1-(304+x);								//11
	else if((r1>334+x)&&(r1<=365+x))r2=r1-(334+x);								//12
	date32|=r2&0xff;
	
/* 如果没有校时，写入默认时间.modify by zzw 2013.3.20 */
	if ( ((date32 >> 8) & 0xff) > 12 )
	{	
		RTC_SET(15552000);
		
/*-----------------LXL 修改开始---------------------------------------------*/		
//原因：写入默认时间之后，没有更改date32的值，导致时间显示的还是之前的错误时间
		date32 = 0x0105061D; //LXL 于2013.03.25 增加V1.24
/*-----------------LXL 修改结束----------------------------------------------*/
	}
/********************end****************/	
	/*2013.3.20注释，去掉日期限制功能  modify by zzw*/
 	//date_limt();
}

/**************************************************************************
** 函数名称 ： date_switch
** 函数功能 ： 计时处理
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/

void date_switch()
{
	u8 i,j,m,n;
	u16 month;
	u32 temp,sec;
	i=date32>>24;
	i*=3;
	temp=0;
	for(j=0;j<3;j++){
		temp<<=8;
		temp|=year_date[i++];
	}
	i=(temp>>5)&0x03;
	j=temp&0x1f;
	m=date32>>8;
	n=date32;
	
	month=0;
	if(m<i){
		month=1;
	}else if(m==i){
		if(n<j)
			month=1;
	}

	if(month){
		i=(date32>>24)-1;
		i*=3;
		temp=0;
		for(j=0;j<3;j++){
			temp<<=8;
			temp|=year_date[i++];
		}
		buf[6]=(date32>>24)-1;
		buf[4]=(temp>>5)&0x03;
		buf[3]=temp&0x1f;
		buf[2]=buf[1]=buf[0]=0;	
	}else{
		buf[6]=date32>>24;
		buf[4]=i;
		buf[3]=j;
		buf[2]=buf[1]=buf[0]=0;		
	}
	sec=settime(buf,1);
	sec=RTC_GetCounter()-sec;
	sec=1+sec/(24*3600);
	memset(buf,29,13);
	month=temp>>4;
		
	m=temp>>20;
	n=0;

	for(i=1;i<13;i++){	
		if(month&0x8000)
			buf[i]++;
		month<<=1;
	}

	m=temp>>20;
	n=13;
	if(m){
		n=14;
		if(temp&0x80)
			buf[13]=30;
		else
			buf[13]=29;		
	}
	month=0;
	for(i=1;i<n;i++){
		if(sec>month&&sec<=(month+buf[i])){
			j=sec-month;
			break;	
		}
		month+=buf[i];	
	}
	if(m>0){
		if(m<i)
			i--;
	}

	datn16=i<<8;
	datn16|=j;
}
/******************************************************
** 函数名称 ： settime
** 函数功能 ： 将年月日时分秒转化为秒值
** 入口参数 ： 缓冲区*p，返回值的标志
** 返回参数 ： 如果flag=1，返回秒值r3；反之，返回0
** 函数说明 ： 年月日来自*p中的值
** 修改日志 ： 刘晓琳 于2013年02月27日修改	
**********************************************************/
u32 settime(u8 *p,u8 flag)
{  
	u8 r1,x;
	u32 r3;	
	r3=((*(p+6)-1)/4)*1461+((*(p+6)-1)%4)*365; //0E 3B 08 05 01 04 0C
	if(*(p+6)%4)x=0;
	else x=1;
	r1=*(p+3);
	switch(*(p+4)){  //根据月份的不同，计算当前的天数
	case(1):r3+=r1;		break;
	case(2):r3+=r1+31;	break;
	case(3):r3+=r1+59+x;break;
	case(4):r3+=r1+90+x;break;
	case(5):r3+=r1+120+x;break;
	case(6):r3+=r1+151+x;break;
	case(7):r3+=r1+181+x;break;
	case(8):r3+=r1+212+x;break;
	case(9):r3+=r1+243+x;break;
	case(10):r3+=r1+273+x;break;
	case(11):r3+=r1+304+x;break;
	case(12):r3+=r1+334+x;break;
	default:break;
	}
	r3=r3*24*3600;//当天的秒值
	r3=r3+(*(p+2))*3600+(*(p+1))*60+*p;//此时的秒值
	if(flag)return r3;//当前时间的秒值
	RTC_SET(r3);//将当前时间写入RTC计数器中
	
	gettime(); //读取当前的时间值
	date_switch();  //计时处理
	return 0;  
}

/**************************************************************************
** 函数名称 ： RTC_Configuration
** 函数功能 ： //实时时钟配置
** 入口参数 ： 无
** 返回参数 ： //返回0:正常//其他:错误代码
** 函数说明 ： //初始化RTC时钟,同时检测时钟是否工作正常//BKP->DR1用于保存是否第一次配置的设置
** 修改日志 ： wwd 2013.5.23
**************************************************************************/
u8 rtc_init(void)
{
	//检查是不是第一次配置时钟
	u16 cnt;
	u8 temp=0;
	if(BKP->DR1!=0X5050)//第一次配置
	{	 
		RCC->APB1ENR|=1<<28;     //使能电源时钟	    
		RCC->APB1ENR|=1<<27;     //使能备份时钟	    
		PWR->CR|=1<<8;           //取消备份区写保护
		RCC->BDCR|=1<<16;        //备份区域软复位	   
		RCC->BDCR&=~(1<<16);     //备份区域软复位结束	  
		RCC->BDCR|=1<<0;         //开启外部低速振荡器 

		while((!(RCC->BDCR&0X02))&&temp<250)//等待外部时钟就绪	 
		{
			temp++;
			cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);
		};
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题	    

		RCC->BDCR|=1<<8; //LSI作为RTC时钟 	    
		RCC->BDCR|=1<<15;//RTC时钟使能	
		
	
		temp=0;
		  while((!(RTC->CRL&(1<<5)))&&(temp<250))
		{
			temp++;
      cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);


    } //等待RTC寄存器同步  
		if(temp>=250)
		{
			return 1;//初始化时钟失败,晶振有问题	  
		}			
		

		while(!(RTC->CRL&(1<<3)));//等待RTC寄存器同步  

		
		RTC->CRH|=0X01;  		  //允许秒中断

		while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成	 

		RTC->CRL|=1<<4;              //允许配置	 
		 
		RTC->PRLH=0X0000;
		RTC->PRLL=RTC_PRLL;          //时钟周期设置(有待观察,看是否跑慢了?)理论值：32767										 
											    
		RTC_SET(15552001);  //设置时间	  
		RTC->CRL&=~(1<<4);           //配置更新
		while(!(RTC->CRL&(1<<5)));   //等待RTC寄存器操作完成		
		BKP->DR1=0X5050;  
		
	}else//系统继续计时
	{
		temp=0;
		while((!(RTC->CRL&(1<<3)))&&(temp<250))
		{
			temp++;
      cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);


    } //等待RTC寄存器同步  
		if(temp>=250)return 1;//初始化时钟失败,晶振有问题	    
		
    RTC->CRH|=0X01;  		  //允许秒中断
  //  while(!(RTC->CRL&(1<<5)));//等待RTC寄存器操作完成	

    temp=0;
    while((!(RTC->CRL&(1<<5)))&&(temp<250))
		{
			temp++;
      cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);


    } //等待RTC寄存器同步  
		if(temp>=250)
		{
			 RTC->CRH&=~0X01; 
			return 1;//初始化时钟失败,晶振有问题	  
		}			
	}		    				  
	
	
	gettime();     //读取当前的时间
	date_switch();  //计时处理
	return 0; //ok
}	

/**************************************************************************
** 函数名称 ： ur
** 函数功能 ： 单片机的串口接收函数			   
** 入口参数 ： 无
** 返回参数 ： 接收到的数据
** 函数说明 ： 串口采用无校验接收数据
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
u8 ur()
{
	unsigned long dwTime;
	dwTime = GetCnt();//15.3.19

	while(((USART1->SR&0x20)==0) && ( dwTime --> 5 ));//等待接收到数据
	return (USART1->DR&0xFF);//读出数据
}
/**************************************************************************
** 函数名称 ： ur
** 函数功能 ： 单片机的串口接收函数			   
** 入口参数 ： 无
** 返回参数 ： 接收到的数据
** 函数说明 ： 串口采用无校验接收数据
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
u8 ur2()
{
	unsigned long dwTime;
	dwTime = GetCnt();//15.3.19
	
	while(((USART2->SR&0x20)==0)  && ( dwTime --> 5 ));//等待接收到数据
	return (USART2->DR&0xFF);//读出数据
}

/**************************************************************************
** 函数名称 ： ut
** 函数功能 ： 单片机的串口发送函数   
** 入口参数 ： 需要发送的数据Data
** 返回参数 ： 无
** 函数说明 ： 串口采用无校验发送数据
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void ut(u8 Data)
{	                   
	while((USART1->SR&0X40)==0);	
	USART1->DR = Data;
	//while((USART1->SR&0X40)==0);	
}
/**************************************************************************
** 函数名称 ： ut
** 函数功能 ： 单片机的串口发送函数   
** 入口参数 ： 需要发送的数据Data
** 返回参数 ： 无
** 函数说明 ： 串口采用无校验发送数据
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void ut2(u8 Data)
{	
	while((USART2->SR&0X40)==0);	
	USART2->DR = Data;
	//while((USART1->SR&0X40)==0);	
}



void send_string(unsigned char *p,unsigned char len)
{
	#ifdef ConMod1
    USART1->SR&=~(1<<7);
    while(len--)
    {
        ut(*p);
        p++;
    }
	#elif ConMod2
		    while(len--)
    {
        ut2(*p); 
        p++;
    }
	#endif	
}

void IWDG_Init(u8 prer,u16 rlr) 
{
	IWDG->KR=0X5555;//使能对IWDG->PR和IWDG->RLR的写		 										  
  	IWDG->PR=prer;  //设置分频系数   
  	IWDG->RLR=rlr;  //从加载寄存器 IWDG->RLR  
	IWDG->KR=0XAAAA;//reload											   
  	IWDG->KR=0XCCCC;//使能看门狗	
}
//喂独立看门狗
void IWDG_Feed(void)
{
	IWDG->KR=0XAAAA;//reload											   
}
//SoftReset
void SoftReset()
{
	__set_PRIMASK(1);  //进入临界状态
	SCB->AIRCR  = (0x05fa0004|NVIC_GROUP);  //SCB_AIRCR_PRIGROUP_Msk                                  /* Keep priority group unchanged */
	while(1);       /* wait until reset */
}
