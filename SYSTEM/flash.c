
#include "stmlib.h"
//#include <float.h>

#define 	CS_L	SET_CE
#define 	CS_H	OFF_CE

#define 	uint8_t 	u8
#define 	uint16_t 	u16
#define 	uint32_t 	u32

//定义外部变量
extern u32 flash_addr;//flash的地址

/*******************************************************************************
* 函数名称  : SPI_FlashReadByte
* 函数功能  : 从flash中读出一个字节             
* 入口参数  : 无
* 返回参数  : 读取的数据
* 函数说明  ：
* 修改日志  : ZZW 13.5.30
*******************************************************************************/
uint8_t flash_read(uint32_t dwAddr)			 
{
/* 片选拉高，取消片选 */	
	CS_H;
	
	/* 延时5个时钟周期 */
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
	/* 片选拉低，选中 */
	CS_L;
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
	/* 发读命令 */
	send_byte( 0x03 );
	
	/* 发送24位地址 */
	send_byte( (uint8_t)(dwAddr >> 16) );//发送读数据的地址
	send_byte( (uint8_t)(dwAddr >> 8) );
	send_byte( (uint8_t)dwAddr );
	
	/* 返回读取的数据 */
	return send_byte(0);  
}

/*******************************************************************************
* 函数名称  : isBusy
* 函数功能  : 判断FLASH是否空闲
* 入口参数  ：无
* 返回参数  : 无
* 函数说明  : 
* 修改日志  : ZZW 13.5.30
*******************************************************************************/
static void isBusy(void)
{
	uint8_t ucStatu;
	CS_L;
	__nop();
	__nop();

	send_byte( 0x05 ); //发送读状态寄存器命令字
	do {
		ucStatu = send_byte(0);
	} while ( ucStatu & 0x01 ); //等待空闲
		
	CS_H;
}


/**************************************************************************
** 函数名称 ： FlashUnprotect
** 函数功能 ： flash的写使能			   
** 入口参数 ： 无
** 返回参数 ： 无
** 修改日志 ： ZZW 13.5.30
**************************************************************************/
static void FlashUnprotect( void )	   		
{
	CS_L;
	__nop();
	__nop();
	/* 使能写状态寄存器，SST的flash这个命令必须是0X06,
	 *华邦FLASH0X50和0X06都可以，华邦FLASH手册上介绍的是0X50
	 */
	send_byte( 0x50 );	 			//使能写状态寄存器 0x50
	CS_H;
	__nop();
	__nop();
	__nop();
	__nop();	

  CS_L;
	__nop();
	__nop();

	send_byte( 0x01 );	 			//写状态寄存器
	send_byte( 0x00 );
	__nop();
	__nop();	
	CS_H;
	
	isBusy();

	CS_L;
	__nop();
	__nop();

	send_byte( 0x06 );				//写使能
	CS_H;
}


/*******************************************************************************
* 函数名称  : flash_byte_write
* 函数功能  : 向flash中写数据
* 入口参数  : 
* 返回参数  : 无
* 函数说明  : 向flash中写数据，每次写一个数据
* 修改日志  ：ZZW 13.5.30
*******************************************************************************/
void flash_byte_write( uint8_t ucDat ) 	//字节写
{
	FlashUnprotect( );
	__nop();
	__nop();	
	CS_L;
	__nop();
	__nop();

	send_byte( 0x02 );	 			//写命令
	send_byte( (uint8_t)(flash_addr >> 16) );//发送写入数据的首地址
	send_byte( (uint8_t)(flash_addr >> 8) );
	send_byte( (uint8_t)flash_addr);
	send_byte( ucDat );    //发送写入的数据

	CS_H;
	__nop();
	__nop();
	
	isBusy( );//等待操作完成
}


/*******************************************************************************
* 函数名称  : flash_chip_erase
* 函数功能  : 擦除flash芯片
* 入口参数  : 无
* 返回参数  : 无
* 函数说明  : 擦除flash整个芯片
* 修改日志  ：ZZW 13.5.30
*******************************************************************************/
void flash_chip_erase( void )	   //芯片擦除
{
	FlashUnprotect(); //flash写使能
	CS_L;
	__nop();
	__nop();

	send_byte(0x60);   //发送芯片擦除的命令字
	CS_H;
	isBusy(  );
}

/*******************************************************************************
* 函数名称  : flash_sum
* 函数功能  : 校验flash数据和
* 入口参数  : 无
* 返回参数  : 校验和
* 函数说明  : 
* 修改日志  ：王万冬 于2013年06月22日修改
*******************************************************************************/
uint8_t flash_sum(uint32_t dwAddr,u32 lenth)			 
{
	u8 sums;
///* 片选拉高，取消片选 */	
//	CS_H;
//	
//	/* 延时5个时钟周期 */
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	
//	/* 片选拉低，选中 */
//	CS_L;
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	
//	/* 发读命令 */
//	send_byte( 0x03 );
//	
//	/* 发送24位地址 */
//	send_byte( (uint8_t)(dwAddr >> 16) );//发送读数据的地址
//	send_byte( (uint8_t)(dwAddr >> 8) );
//	send_byte( (uint8_t)dwAddr );
	
	/* 返回读取的数据 校验和 */
// 	return send_byte(); 
	sums=flash_read(dwAddr);
	lenth--;
	while(lenth)
	{
		lenth--;
		sums+=send_byte(0);
	}	
	return sums;
}




#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

void FLASH_Unlock(void)
{
  /* Authorize the FPEC of Bank1 Access */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}
void FLASH_Lock(void)
{
  /* Set the Lock Bit to lock the FPEC and the CR of  Bank1 */
  FLASH->CR|=1<<7;
}
//得到FLASH状态
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //忙
	else if(res&(1<<2))return 2;	//编程错误
	else if(res&(1<<4))return 3;	//写保护错误
	return 0;						//操作完成
}
//等待操作完成
//time:要延时的长短
//返回值:状态.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//非忙,无需等待了,直接退出.
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//擦除页
//paddr:页地址
//返回值:执行情况
u8 FLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);//等待上次操作结束,>20ms    
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//页擦除
		FLASH->AR=paddr;//设置页地址 
		FLASH->CR|=1<<6;//开始擦除		  
		res=STMFLASH_WaitDone(0X5FFF);//等待操作结束,>20ms  
		if(res!=1)//非忙
		{
			FLASH->CR&=~(1<<1);//清除页擦除标志.
		}
	}
	return res;
}
//在FLASH指定地址写入半字
//faddr:指定地址(此地址必须为2的倍数!!)
//dat:要写入的数据
//返回值:写入的情况
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;//编程使能
		*(vu16*)faddr=dat;//写入数据
		res=STMFLASH_WaitDone(0XFF);//等待操作完成
		if(res==0)//操作成功
		{
			*(vu16*)(faddr+2)=dat>>16;//写入数据
			res=STMFLASH_WaitDone(0XFF);//等待操作完成
			if(res!=1)//操作成功
			{
				FLASH->CR&=~(1<<0);//清除PG位.
			}
		}
		else if(res!=1)//操作成功
		{
			//FLASH->CR&=~(1<<0);//清除PG位.
			FLASH->CR&=0x00001FFE;
		}
	} 
	
	//FLASH->CR |= 0x80;
	return res;
} 
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
//void STMFLASH_Write(u32 WriteAddr,u8 *pBuffer,u16 NumToWrite)   
//{ 			 		 
//	u16 i,dt;
//	for(i=0;i<NumToWrite;i++)
//	{
//		dt=pBuffer[i];
//// 		IAP_sum+=pBuffer[i];
//		i++;
//		
//		if(i<NumToWrite)
//		{
//			dt+=pBuffer[i]<<8;
//// 			IAP_sum+=pBuffer[i];
//		}
//		else dt+=0xff00;
//		
//		
//		STMFLASH_WriteHalfWord(WriteAddr,dt);
//	    WriteAddr+=2;//地址增加2.
//	}  
//} 

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
//void STMFLASH_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
//{
//	u16 i,dt;
//	for(i=0;i<NumToRead;i++)
//	{
//		dt=*(vu16*)ReadAddr;//STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
//		pBuffer[i]=dt;
//// 		IAP_sum+=pBuffer[i];
//		i++;
//		if(i<NumToRead)
//		{
//			pBuffer[i]=dt>>8;
//// 			IAP_sum+=pBuffer[i];
//		}
//		
//		
//		ReadAddr+=2;//偏移2个字节.	
//	}
//}

