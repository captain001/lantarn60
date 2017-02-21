#include "stmlib.h"
#include <led.h>

u8 cardNum[cardLength];//������ʾ��Ӧ������


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


const u8 card_style[22][16] = {  //��ʾ����
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

//�����ⲿ����
extern u32 date32,time32; //������ڡ�ʱ��
extern u8 buf[20]; //����
extern u16 datn16; //���ũ��

#define RTC_PRLL 0x8002  //32778
u8 Frequency=128;
extern int rtc_error_flag;
	
extern struct ScrStruct  scr_info;
extern unsigned char sim_buf_red[ ];		


/******************************************************************************************
** �������� �� RCC_Configuration
** �������� �� ��������Ƶ��ʼ������			   
** ��ڲ��� �� ��
** ���ڲ��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�
*******************************************************************************************/
void RCC_Configuration(void)
{
	unsigned short temp=0;
	unsigned char PLL=0;
	
	PLL=0x1C;
	temp=0x05;
	
	Frequency=(temp+4)*8;	
///////////////RCC/������������ȫ��λ��������Ƶ�Ĳ�����///////////
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
  
	RCC->CR |=0x00010000;     //ʹ���ⲿ����ʱ��HSEON	 	
	while( (!(RCC->CR>>17))&&(temp<0xfff) )//�ȴ��ⲿʱ�Ӿ���
	{
		temp++;
	}
	if(temp>0xff0)
	{
		RCC->CR &=~0x00010000;
		RCC->CR |=0x00000001;
		RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
		RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
		RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
		RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
		RCC->CIR = 0x00000000;     //�ر������ж�		
	}
	else
	{
		temp=0;
		RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1; 
		RCC->CFGR|=PLL<<16;   //����PLLֵ 2~16 
		RCC->CFGR|=1<<16;	  //PLLSRC ON  
		FLASH->ACR|=0x32;	  //FLASH 2����ʱ���� 

		RCC->CR|=0x01000000;  //PLLON 
		while(!(RCC->CR>>25));//�ȴ�PLL���� 
		RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��	  
		while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ� 
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
///////////////RCC/������������ȫ��λ��������Ƶ�Ĳ�����///////////
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
  
	RCC->CR |=0x00010000;     //ʹ���ⲿ����ʱ��HSEON	 	
// 	while(!(RCC->CR>>17));
	while( (!(RCC->CR>>17))&&(temp<0xfff) )//�ȴ��ⲿʱ�Ӿ���
	{
		temp++;
	}
	if(temp>0xff0)
	{
		RCC->CR &=~0x00010000;
		RCC->CR |=0x00000001;
		RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
		RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
		RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
		RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
		RCC->CIR = 0x00000000;     //�ر������ж�		
	}
	else
	{
		temp=0;
		RCC->CFGR=0X00000400; //APB1=DIV2;APB2=DIV1;AHB=DIV1; 
		RCC->CFGR|=PLL<<16;   //����PLLֵ 2~16 
		RCC->CFGR|=1<<16;	  //PLLSRC ON  
		FLASH->ACR|=0x32;	  //FLASH 2����ʱ���� 

		RCC->CR|=0x01000000;  //PLLON 
		while(!(RCC->CR>>25));//�ȴ�PLL���� 
		RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��	  
		while(temp!=0x02)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ� 
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
** �������� �� uart_init
** �������� �� ���ڵĳ�ʼ������			   
** ��ڲ��� �� ��
** ���ڲ��� �� ��
** ����˵�� �� ����1����Ϊ115200bps��Ĭ�ϣ�,8λ���ݣ���Ч��λ
** �޸���־ �� ������ ��2013��01��07���޸�
*******************************************************************************************/
void uart_init(unsigned long b)
{	
	float temp;
	u16 mantissa;
	u16 fraction;
	
	temp=(float)(Frequency*1000000)/(b*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	RCC->APB2RSTR|=1<<14;   //��λ����1
	RCC->APB2RSTR&=~(1<<14);//ֹͣ��λ	   	   
	//����������
 	USART1->BRR=mantissa; // ����������	 
	USART1->CR1|=0X202C;  //1λֹͣ,��У��λ.
}
/**************************************************************************
** �������� �� uart2_init
** �������� �� ����2��ʼ��
** ��ڲ��� �� bound������
** ���ز��� �� 
** ����˵�� �� 
** �޸���־ �� 2013.5.20 wwd
**************************************************************************/
void uart2_init(unsigned long b)
{
	float temp;
	u16 mantissa;
	u16 fraction;
	
	temp=(float)((Frequency/2)*1000000)/(b*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction;
	
	RCC->APB1RSTR|=1<<17;   //��λ����2
	RCC->APB1RSTR&=~(1<<17);//ֹͣ��λ	   	   7
	//����������
 	USART2->BRR=mantissa; // ����������	 
	USART2->CR1|=0X202C;  //1λֹͣ,��У��λ.//�����ж�//ʹ�ܴ���
}
/******************************************************************************************
** �������� �� timer1_init
** �������� �� ��ʱ��1��ʼ��		   
** ��ڲ��� �� ��ʱ������ֵs
** ���ڲ��� �� ��
** ����˵�� �� 
** �޸���־ �� ZGF ��2013��01��07���޸�
***********************************************************************************/
 void timer1_init(void) 
 {
	u16  arr=0;
	arr=(Frequency*1000000)/2000;//1000000Ϊ1�룬2000ΪԤ��Ƶ
	
	RCC->APB2ENR|=1<<11;//TIM1ʱ��ʹ�� 
	TIM1->CR1&=0xFE;
	RCC->APB1RSTR|=1<<11;   //��λ
	RCC->APB1RSTR&=~(1<<11);//ֹͣ��λ		
	
 	TIM1->ARR=arr;  //�趨�������Զ���װֵ//�պ�1s  
	TIM1->PSC=2000;  //Ԥ��Ƶ��2000
	TIM1->DIER|=0x01;   //��������ж�				   
  TIM1->CR1|=0x01;    //ʹ�ܶ�ʱ��1			

}
/******************************************************************************************
** �������� �� timer_init
** �������� �� ��ʱ��2��ʼ��		   
** ��ڲ��� �� ��ʱ������ֵs
** ���ڲ��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�
*******************************************************************************************/
void timer_init(unsigned short s)	//���һ��100΢�� 5/16  5/4		   
{
	TIM2->CR1&=0xFE;
	RCC->APB1RSTR|=1<<0;   //��λ
	RCC->APB1RSTR&=~(1<<0);//ֹͣ��λ		
	TIM2->ARR=s;
	TIM2->PSC=0x003F;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM2->DIER|=1;   //��������ж�	  
	TIM2->CR1|=0x01;    //ʹ�ܶ�ʱ�� 	
}

/******************************************************************************************
** �������� �� timer3_init
** �������� �� ��ʱ��3��ʼ��		   
** ��ڲ��� �� ��
** ���ڲ��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�
*******************************************************************************************/
void timer3_init() 
{
	u16  arr=0;
	arr=(Frequency*5000)/2000;//1000000Ϊ1�룬2000ΪԤ��Ƶ

	TIM3->CR1&=0xFE;
	RCC->APB1RSTR|=1<<1;   //��λ
	RCC->APB1RSTR&=~(1<<1);//ֹͣ��λ		
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ
	TIM3->PSC=2000;  	//Ԥ��Ƶ��2000	  
	TIM3->DIER|=1;   //��������ж�	  
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��		
}

/******************************************************************************************
** �������� �� timer4_init
** �������� �� ��ʱ��4��ʼ��		   
** ��ڲ��� �� ��ʱ����ֵ��ֵn
** ���ڲ��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�
*******************************************************************************************/
void timer_init4(unsigned short n)  //19.5US  MIN n=40
{
	TIM4->CR1&=0xFE;    //��ʹ�ܶ�ʱ��
	RCC->APB1RSTR|=1<<2;   //��λ
	RCC->APB1RSTR&=~(1<<2);//ֹͣ��λ		
 	TIM4->ARR=n;  	//�趨�������Զ���װֵ
	TIM4->PSC=0x003F;  	//Ԥ��Ƶ��7200,�õ�10Khz�ļ���ʱ��		  
	TIM4->DIER|=1;   //��������ж�	  
	TIM4->CR1&=0xFE;    //��ʹ�ܶ�ʱ��
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
** �������� �� NVIC_Configuration
** �������� �� �жϿ�������ʼ��		   
** ��ڲ��� �� ��
** ���ڲ��� �� ��
** ����˵�� �� ���ø����жϵ����ȼ�
** �޸���־ �� ������ ��2013��01��07���޸�
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
** �������� �� GPIO_Configuration
** �������� �� GPIO�˿ڵĳ�ʼ������			   
** ��ڲ��� �� ��
** ���ڲ��� �� ��
** ����˵�� �� ���ø����ܽ�
** �޸���־ �� ������ ��2013��01��07���޸�
*******************************************************************************************/
void GPIO_Configuration(void)
{	
	#ifdef ConMod1
	RCC->APB2ENR |= (RCC_APB2Periph_GPIOA |RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_USART1|RCC_APB2Periph_SPI1); //����ʱ��		   
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
			|RCC_APB2Periph_TIM1 | RCC_APB2Periph_USART1|RCC_APB2Periph_SPI1|RCC_APB2Periph_AFIO); //����ʱ��		   
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
* ��������  : spi_init
* ��������  : stm32f051��Ƭ����spi�˿ڳ�ʼ��
* ��ڲ���  : ���÷�Ƶ��set
* ���ز���  : ��
* ����˵��  : spi�˿ڳ�ʼ��
* �޸���־  : ������ ��2013��01��07���޸�
*******************************************************************************/
 void spi_init(u16 set)
 {
	 #ifdef ConMod1
	 SPI1->CR1&=0xFFBF; 		//SPI�豸��ʹ��
	 RCC->APB2RSTR|=1<<12;   //��λ
	 RCC->APB2RSTR&=~(1<<12);//ֹͣ��λ		
	 SPI1->CR1=set; 		//SPI�豸ʹ��
	 SPI1->CRCPR =7;
	 SPI1->CR1|=0x40; 		//SPI�豸ʹ��
	 #elif ConMod2
	 	 //spi moni zhuan yingjian
	 GPIOA->CRL&=0x000FFFFF;
	 GPIOA->CRL|=0xBBB00000;	
	 
	 SPI1->CR1&=0xFFBF; 		//SPI�豸��ʹ��
	 RCC->APB2RSTR|=1<<12;   //��λ
	 RCC->APB2RSTR&=~(1<<12);//ֹͣ��λ		
	 SPI1->CR1=set; 		//SPI�豸ʹ��
	 SPI1->CRCPR =7;
	 SPI1->CR1|=0x40; 		//SPI�豸ʹ��
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
#define FTIR   1  //�½��ش���
#define RTIR   2  //�����ش���
#define FRIR   3  //˫����

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
** �������� �� exti_init
** �������� �� �ⲿ�жϳ�ʼ��
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
void exti_init()
{
	
  Ex_NVIC_Config(GPIO_A,13,FTIR); 			//�½��ش���
  Ex_NVIC_Config(GPIO_A,14,FTIR); 			//�½��ش���
	Ex_NVIC_Config(GPIO_A,16,FRIR);
}

/**************************************************************************
** �������� �� RTC_GetCounter
** �������� �� timecount������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� wwd 2013.5.23
**************************************************************************/
u32 RTC_GetCounter()
{
	u32 timecount=0;  
	   
if(rtc_error_flag!=0x01)
{ 
	timecount=RTC->CNTH;//�õ��������е�ֵ(������)
	timecount<<=16;
	timecount+=RTC->CNTL;	
}
	return timecount;
}
/**************************************************************************
** �������� �� RTC_SET
** �������� ��count ������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� wwd 2013.5.23
**************************************************************************/
void RTC_SET(u32 count)
{
// 	u32 r3;
	if(rtc_error_flag!=0x01)
	{
	while(!(RTC->CRL&(1<<5)));   //�ȴ�RTC�Ĵ����������		

	
	RCC->APB1ENR|=1<<28;//ʹ�ܵ�Դʱ��
	RCC->APB1ENR|=1<<27;//ʹ�ܱ���ʱ��
	PWR->CR|=1<<8;    //ȡ��������д����

	RTC->CRL|=1<<4;   //�������� 
		
	RTC->CNTL=count&0xffff;
	RTC->CNTH=count>>16;
	RTC->CRL&=~(1<<4);//���ø���
	
	while(!(RTC->CRL&(1<<5))) ;  //�ȴ�RTC�Ĵ����������		

  }
}
/******************************************************
** �������� �� gettime
** �������� �� ��õ�ǰ��ʱ��
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� ���Ĵ����ж�������ֵ��תΪ���������ں�ʱ����
**             �ֱ����data32��time32��
** �޸���־ �� 2013.3.18 ������	
**********************************************************/
void gettime()
{
	
	u8 x,i;
	u32 r1,r2,r3;
	r1=RTC_GetCounter();//����rtc�������е���ֵ
	r2=24*3600;
	r3=r1/r2;  //��ǰʱ�������-��
	r2=r1%r2;  //�����ʱ����ֵ
	time32=r2/3600; //��ʱ��ʱ
	time32<<=8;
	time32|=(r2%3600)/60;//��ʱ�ķ�
	time32<<=8;
	time32|=(r2%3600)%60;//��ʱ����ֵ

	i=1;
	r1=0;
	for(i=1;i<50;i++){
		if(r3>r1&&r3<=(r1+date_year[i]+365))
			break;
		r1+=date_year[i]+365;
	}

	date32=i;		    	//�� +2000	  
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
	if((r1>0)&&(r1<=31))			r2=r1;										//1��
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
	
/* ���û��Уʱ��д��Ĭ��ʱ��.modify by zzw 2013.3.20 */
	if ( ((date32 >> 8) & 0xff) > 12 )
	{	
		RTC_SET(15552000);
		
/*-----------------LXL �޸Ŀ�ʼ---------------------------------------------*/		
//ԭ��д��Ĭ��ʱ��֮��û�и���date32��ֵ������ʱ����ʾ�Ļ���֮ǰ�Ĵ���ʱ��
		date32 = 0x0105061D; //LXL ��2013.03.25 ����V1.24
/*-----------------LXL �޸Ľ���----------------------------------------------*/
	}
/********************end****************/	
	/*2013.3.20ע�ͣ�ȥ���������ƹ���  modify by zzw*/
 	//date_limt();
}

/**************************************************************************
** �������� �� date_switch
** �������� �� ��ʱ����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
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
** �������� �� settime
** �������� �� ��������ʱ����ת��Ϊ��ֵ
** ��ڲ��� �� ������*p������ֵ�ı�־
** ���ز��� �� ���flag=1��������ֵr3����֮������0
** ����˵�� �� ����������*p�е�ֵ
** �޸���־ �� ������ ��2013��02��27���޸�	
**********************************************************/
u32 settime(u8 *p,u8 flag)
{  
	u8 r1,x;
	u32 r3;	
	r3=((*(p+6)-1)/4)*1461+((*(p+6)-1)%4)*365; //0E 3B 08 05 01 04 0C
	if(*(p+6)%4)x=0;
	else x=1;
	r1=*(p+3);
	switch(*(p+4)){  //�����·ݵĲ�ͬ�����㵱ǰ������
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
	r3=r3*24*3600;//�������ֵ
	r3=r3+(*(p+2))*3600+(*(p+1))*60+*p;//��ʱ����ֵ
	if(flag)return r3;//��ǰʱ�����ֵ
	RTC_SET(r3);//����ǰʱ��д��RTC��������
	
	gettime(); //��ȡ��ǰ��ʱ��ֵ
	date_switch();  //��ʱ����
	return 0;  
}

/**************************************************************************
** �������� �� RTC_Configuration
** �������� �� //ʵʱʱ������
** ��ڲ��� �� ��
** ���ز��� �� //����0:����//����:�������
** ����˵�� �� //��ʼ��RTCʱ��,ͬʱ���ʱ���Ƿ�������//BKP->DR1���ڱ����Ƿ��һ�����õ�����
** �޸���־ �� wwd 2013.5.23
**************************************************************************/
u8 rtc_init(void)
{
	//����ǲ��ǵ�һ������ʱ��
	u16 cnt;
	u8 temp=0;
	if(BKP->DR1!=0X5050)//��һ������
	{	 
		RCC->APB1ENR|=1<<28;     //ʹ�ܵ�Դʱ��	    
		RCC->APB1ENR|=1<<27;     //ʹ�ܱ���ʱ��	    
		PWR->CR|=1<<8;           //ȡ��������д����
		RCC->BDCR|=1<<16;        //����������λ	   
		RCC->BDCR&=~(1<<16);     //����������λ����	  
		RCC->BDCR|=1<<0;         //�����ⲿ�������� 

		while((!(RCC->BDCR&0X02))&&temp<250)//�ȴ��ⲿʱ�Ӿ���	 
		{
			temp++;
			cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);
		};
		if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	    

		RCC->BDCR|=1<<8; //LSI��ΪRTCʱ�� 	    
		RCC->BDCR|=1<<15;//RTCʱ��ʹ��	
		
	
		temp=0;
		  while((!(RTC->CRL&(1<<5)))&&(temp<250))
		{
			temp++;
      cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);


    } //�ȴ�RTC�Ĵ���ͬ��  
		if(temp>=250)
		{
			return 1;//��ʼ��ʱ��ʧ��,����������	  
		}			
		

		while(!(RTC->CRL&(1<<3)));//�ȴ�RTC�Ĵ���ͬ��  

		
		RTC->CRH|=0X01;  		  //�������ж�

		while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������	 

		RTC->CRL|=1<<4;              //��������	 
		 
		RTC->PRLH=0X0000;
		RTC->PRLL=RTC_PRLL;          //ʱ����������(�д��۲�,���Ƿ�������?)����ֵ��32767										 
											    
		RTC_SET(15552001);  //����ʱ��	  
		RTC->CRL&=~(1<<4);           //���ø���
		while(!(RTC->CRL&(1<<5)));   //�ȴ�RTC�Ĵ����������		
		BKP->DR1=0X5050;  
		
	}else//ϵͳ������ʱ
	{
		temp=0;
		while((!(RTC->CRL&(1<<3)))&&(temp<250))
		{
			temp++;
      cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);


    } //�ȴ�RTC�Ĵ���ͬ��  
		if(temp>=250)return 1;//��ʼ��ʱ��ʧ��,����������	    
		
    RTC->CRH|=0X01;  		  //�������ж�
  //  while(!(RTC->CRL&(1<<5)));//�ȴ�RTC�Ĵ����������	

    temp=0;
    while((!(RTC->CRL&(1<<5)))&&(temp<250))
		{
			temp++;
      cnt=0xffff;while(cnt--);
			cnt=0xffff;while(cnt--);


    } //�ȴ�RTC�Ĵ���ͬ��  
		if(temp>=250)
		{
			 RTC->CRH&=~0X01; 
			return 1;//��ʼ��ʱ��ʧ��,����������	  
		}			
	}		    				  
	
	
	gettime();     //��ȡ��ǰ��ʱ��
	date_switch();  //��ʱ����
	return 0; //ok
}	

/**************************************************************************
** �������� �� ur
** �������� �� ��Ƭ���Ĵ��ڽ��պ���			   
** ��ڲ��� �� ��
** ���ز��� �� ���յ�������
** ����˵�� �� ���ڲ�����У���������
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
u8 ur()
{
	unsigned long dwTime;
	dwTime = GetCnt();//15.3.19

	while(((USART1->SR&0x20)==0) && ( dwTime --> 5 ));//�ȴ����յ�����
	return (USART1->DR&0xFF);//��������
}
/**************************************************************************
** �������� �� ur
** �������� �� ��Ƭ���Ĵ��ڽ��պ���			   
** ��ڲ��� �� ��
** ���ز��� �� ���յ�������
** ����˵�� �� ���ڲ�����У���������
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
u8 ur2()
{
	unsigned long dwTime;
	dwTime = GetCnt();//15.3.19
	
	while(((USART2->SR&0x20)==0)  && ( dwTime --> 5 ));//�ȴ����յ�����
	return (USART2->DR&0xFF);//��������
}

/**************************************************************************
** �������� �� ut
** �������� �� ��Ƭ���Ĵ��ڷ��ͺ���   
** ��ڲ��� �� ��Ҫ���͵�����Data
** ���ز��� �� ��
** ����˵�� �� ���ڲ�����У�鷢������
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void ut(u8 Data)
{	                   
	while((USART1->SR&0X40)==0);	
	USART1->DR = Data;
	//while((USART1->SR&0X40)==0);	
}
/**************************************************************************
** �������� �� ut
** �������� �� ��Ƭ���Ĵ��ڷ��ͺ���   
** ��ڲ��� �� ��Ҫ���͵�����Data
** ���ز��� �� ��
** ����˵�� �� ���ڲ�����У�鷢������
** �޸���־ �� ������ ��2013��01��07���޸�	
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
	IWDG->KR=0X5555;//ʹ�ܶ�IWDG->PR��IWDG->RLR��д		 										  
  	IWDG->PR=prer;  //���÷�Ƶϵ��   
  	IWDG->RLR=rlr;  //�Ӽ��ؼĴ��� IWDG->RLR  
	IWDG->KR=0XAAAA;//reload											   
  	IWDG->KR=0XCCCC;//ʹ�ܿ��Ź�	
}
//ι�������Ź�
void IWDG_Feed(void)
{
	IWDG->KR=0XAAAA;//reload											   
}
//SoftReset
void SoftReset()
{
	__set_PRIMASK(1);  //�����ٽ�״̬
	SCB->AIRCR  = (0x05fa0004|NVIC_GROUP);  //SCB_AIRCR_PRIGROUP_Msk                                  /* Keep priority group unchanged */
	while(1);       /* wait until reset */
}
