
#include "stm32f10x.h"
#include <string.h>
#include "rhx.h"

/* Preemption Priority Group -------------------------------------------------*/
#define NVIC_PriorityGroup_0         ((u32)0x700) /* 0 bits for pre-emption priority
                                                     4 bits for subpriority */
#define NVIC_PriorityGroup_1         ((u32)0x600) /* 1 bits for pre-emption priority
                                                     3 bits for subpriority */
#define NVIC_PriorityGroup_2         ((u32)0x500) /* 2 bits for pre-emption priority
                                                     2 bits for subpriority */
#define NVIC_PriorityGroup_3         ((u32)0x400) /* 3 bits for pre-emption priority
                                                     1 bits for subpriority */
#define NVIC_PriorityGroup_4         ((u32)0x300) /* 4 bits for pre-emption priority
                                                     0 bits for subpriority */
/* APB2 peripheral */
#define RCC_APB2Periph_AFIO              ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA             ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB             ((uint32_t)0x00000008)
#define RCC_APB2Periph_TIM1              ((uint32_t)0x00000800)
#define RCC_APB2Periph_USART1            ((uint32_t)0x00004000)
#define RCC_APB2Periph_SPI1              ((uint32_t)0x00001000)

/* APB1 peripheral */
#define RCC_APB1Periph_TIM2              ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3              ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4              ((uint32_t)0x00000004)
#define RCC_APB1Periph_BKP               ((uint32_t)0x08000000)
#define RCC_APB1Periph_PWR               ((uint32_t)0x10000000)
#define RCC_APB1Periph_USART2            ((u32)0x00020000)

#define SET_CE  GPIOA->BRR=0x10
#define OFF_CE 	GPIOA->BSRR=0x10
#define PLL_MULL RCC_PLLMul_16
#define BAUDRATE	115200
#define RTCClockSource_LSE

void FLASH_Unlock(void);
void FLASH_Lock(void);
u8 FLASH_ErasePage(u32 paddr);
u8 STMFLASH_WriteWord(u32 faddr, u32 dat);

	
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void RTC_Configuration(void);
void uart_init(unsigned long b);
void uart2_init(unsigned long b);///////////////
void ut2(u8 date);////////////
void ut(u8 date);
u8 ur(void);
u8 ur2(void);//////////////
 void timer1_init(void) ;
void timer_init(unsigned short s);//u8 x,u8 s
void timer3_init(void);
void timer_init4(unsigned short n);
void TIM4_Cmd(void);
//void date_limt(void);

u8 rtc_init(void);
void RTC_SET(u32 count);
u32 RTC_GetCounter(void);
void gettime(void);
void date_switch(void);
u32 settime(u8 *p,u8 flag);

void exti_init(void);

void spi_init(u16 set);
u8 send_byte(u8 byte);
u8 flash_read(u32 addr);
void flash_byte_write(u8 data);
void flash_chip_erase(void);

void ch734init(void);
//void up_init(u8 cmd);
//void readx(void);
//void config(void);

void flash_read_write(u8 s);
u8 flash_sum(u32 addrs,u32 lenth);

void IWDG_Init(u8 prer,u16 rlr);
void IWDG_Feed(void);
void SoftReset(void);

void send_string(unsigned char *p,unsigned char len);

#define	Spi374OutByte(d)		send_byte( d )						/* SPI输出8个位数据 */
#define	Spi374InByte()			send_byte(0xff)						/* SPI输入8个位数据 */

#define spisd send_byte(0)//SPI1->DR=0;while((SPI1->SR&1)==0);//while (SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE)== RESET);
#define spdat SPI1->DR

#define spdelay		send_byte(0)//do{	*(u16*)0x4001300c=0;while(((*(u16*)0x40013008)&0x01)==0);}while(0)									
#define spdats *(u16*)0x4001300c

#ifdef ConMod1
    #define ROW_SCAN(row) do{GPIOA->BSRR=0x000f0000|row;}while(0)//GPIOA->ODR=(GPIOA->ODR&(~0x0f))|(row)
//#define ROW_SCAN(row) do{GPIOB->BSRR=0x000f0000|row;}while(0)//GPIOA->ODR=(GPIOA->ODR&(~0x0f))|(row)
#elif ConMod2 
    #define ROW_SCAN(row) do{GPIOB->BSRR=0x000f0000|row;}while(0)//GPIOA->ODR=(GPIOA->ODR&(~0x0f))|(row)
//#define HL_LC   GPIOA->ODR=(GPIOA->ODR&(~0x100))|0x100;GPIOA->ODR=GPIOA->ODR&(~0x100)
#endif		

#define HIGH_OE GPIOA->BSRR=0x1000
#define LOW_OE  GPIOA->BRR=0x1000
#define H_CP   	GPIOB->ODR=(GPIOB->ODR&(~0x80))|0x80
#define L_CP	GPIOB->ODR=GPIOB->ODR&(~0x80)
#define SET_OE(i)	do{if(i)HIGH_OE;else LOW_OE;}while(0) //i?HIGH_OE:LOW_OE// 

#define FLASH_PAGE_SIZE 0x800					//扇区大小	 





