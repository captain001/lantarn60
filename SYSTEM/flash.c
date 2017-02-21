
#include "stmlib.h"
//#include <float.h>

#define 	CS_L	SET_CE
#define 	CS_H	OFF_CE

#define 	uint8_t 	u8
#define 	uint16_t 	u16
#define 	uint32_t 	u32

//�����ⲿ����
extern u32 flash_addr;//flash�ĵ�ַ

/*******************************************************************************
* ��������  : SPI_FlashReadByte
* ��������  : ��flash�ж���һ���ֽ�             
* ��ڲ���  : ��
* ���ز���  : ��ȡ������
* ����˵��  ��
* �޸���־  : ZZW 13.5.30
*******************************************************************************/
uint8_t flash_read(uint32_t dwAddr)			 
{
/* Ƭѡ���ߣ�ȡ��Ƭѡ */	
	CS_H;
	
	/* ��ʱ5��ʱ������ */
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
	/* Ƭѡ���ͣ�ѡ�� */
	CS_L;
	__nop();
	__nop();
	__nop();
	__nop();
	__nop();
	
	/* �������� */
	send_byte( 0x03 );
	
	/* ����24λ��ַ */
	send_byte( (uint8_t)(dwAddr >> 16) );//���Ͷ����ݵĵ�ַ
	send_byte( (uint8_t)(dwAddr >> 8) );
	send_byte( (uint8_t)dwAddr );
	
	/* ���ض�ȡ������ */
	return send_byte(0);  
}

/*******************************************************************************
* ��������  : isBusy
* ��������  : �ж�FLASH�Ƿ����
* ��ڲ���  ����
* ���ز���  : ��
* ����˵��  : 
* �޸���־  : ZZW 13.5.30
*******************************************************************************/
static void isBusy(void)
{
	uint8_t ucStatu;
	CS_L;
	__nop();
	__nop();

	send_byte( 0x05 ); //���Ͷ�״̬�Ĵ���������
	do {
		ucStatu = send_byte(0);
	} while ( ucStatu & 0x01 ); //�ȴ�����
		
	CS_H;
}


/**************************************************************************
** �������� �� FlashUnprotect
** �������� �� flash��дʹ��			   
** ��ڲ��� �� ��
** ���ز��� �� ��
** �޸���־ �� ZZW 13.5.30
**************************************************************************/
static void FlashUnprotect( void )	   		
{
	CS_L;
	__nop();
	__nop();
	/* ʹ��д״̬�Ĵ�����SST��flash������������0X06,
	 *����FLASH0X50��0X06�����ԣ�����FLASH�ֲ��Ͻ��ܵ���0X50
	 */
	send_byte( 0x50 );	 			//ʹ��д״̬�Ĵ��� 0x50
	CS_H;
	__nop();
	__nop();
	__nop();
	__nop();	

  CS_L;
	__nop();
	__nop();

	send_byte( 0x01 );	 			//д״̬�Ĵ���
	send_byte( 0x00 );
	__nop();
	__nop();	
	CS_H;
	
	isBusy();

	CS_L;
	__nop();
	__nop();

	send_byte( 0x06 );				//дʹ��
	CS_H;
}


/*******************************************************************************
* ��������  : flash_byte_write
* ��������  : ��flash��д����
* ��ڲ���  : 
* ���ز���  : ��
* ����˵��  : ��flash��д���ݣ�ÿ��дһ������
* �޸���־  ��ZZW 13.5.30
*******************************************************************************/
void flash_byte_write( uint8_t ucDat ) 	//�ֽ�д
{
	FlashUnprotect( );
	__nop();
	__nop();	
	CS_L;
	__nop();
	__nop();

	send_byte( 0x02 );	 			//д����
	send_byte( (uint8_t)(flash_addr >> 16) );//����д�����ݵ��׵�ַ
	send_byte( (uint8_t)(flash_addr >> 8) );
	send_byte( (uint8_t)flash_addr);
	send_byte( ucDat );    //����д�������

	CS_H;
	__nop();
	__nop();
	
	isBusy( );//�ȴ��������
}


/*******************************************************************************
* ��������  : flash_chip_erase
* ��������  : ����flashоƬ
* ��ڲ���  : ��
* ���ز���  : ��
* ����˵��  : ����flash����оƬ
* �޸���־  ��ZZW 13.5.30
*******************************************************************************/
void flash_chip_erase( void )	   //оƬ����
{
	FlashUnprotect(); //flashдʹ��
	CS_L;
	__nop();
	__nop();

	send_byte(0x60);   //����оƬ������������
	CS_H;
	isBusy(  );
}

/*******************************************************************************
* ��������  : flash_sum
* ��������  : У��flash���ݺ�
* ��ڲ���  : ��
* ���ز���  : У���
* ����˵��  : 
* �޸���־  ������ ��2013��06��22���޸�
*******************************************************************************/
uint8_t flash_sum(uint32_t dwAddr,u32 lenth)			 
{
	u8 sums;
///* Ƭѡ���ߣ�ȡ��Ƭѡ */	
//	CS_H;
//	
//	/* ��ʱ5��ʱ������ */
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	
//	/* Ƭѡ���ͣ�ѡ�� */
//	CS_L;
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	__nop();
//	
//	/* �������� */
//	send_byte( 0x03 );
//	
//	/* ����24λ��ַ */
//	send_byte( (uint8_t)(dwAddr >> 16) );//���Ͷ����ݵĵ�ַ
//	send_byte( (uint8_t)(dwAddr >> 8) );
//	send_byte( (uint8_t)dwAddr );
	
	/* ���ض�ȡ������ У��� */
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
//�õ�FLASH״̬
u8 STMFLASH_GetStatus(void)
{	
	u32 res;		
	res=FLASH->SR; 
	if(res&(1<<0))return 1;		    //æ
	else if(res&(1<<2))return 2;	//��̴���
	else if(res&(1<<4))return 3;	//д��������
	return 0;						//�������
}
//�ȴ��������
//time:Ҫ��ʱ�ĳ���
//����ֵ:״̬.
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res=STMFLASH_GetStatus();
		if(res!=1)break;//��æ,����ȴ���,ֱ���˳�.
		time--;
	 }while(time);
	 if(time==0)res=0xff;//TIMEOUT
	 return res;
}
//����ҳ
//paddr:ҳ��ַ
//����ֵ:ִ�����
u8 FLASH_ErasePage(u32 paddr)
{
	u8 res=0;
	res=STMFLASH_WaitDone(0X5FFF);//�ȴ��ϴβ�������,>20ms    
	if(res==0)
	{ 
		FLASH->CR|=1<<1;//ҳ����
		FLASH->AR=paddr;//����ҳ��ַ 
		FLASH->CR|=1<<6;//��ʼ����		  
		res=STMFLASH_WaitDone(0X5FFF);//�ȴ���������,>20ms  
		if(res!=1)//��æ
		{
			FLASH->CR&=~(1<<1);//���ҳ������־.
		}
	}
	return res;
}
//��FLASHָ����ַд�����
//faddr:ָ����ַ(�˵�ַ����Ϊ2�ı���!!)
//dat:Ҫд�������
//����ֵ:д������
u8 STMFLASH_WriteWord(u32 faddr, u32 dat)
{
	u8 res;	   	    
	res=STMFLASH_WaitDone(0XFF);	 
	if(res==0)//OK
	{
		FLASH->CR|=1<<0;//���ʹ��
		*(vu16*)faddr=dat;//д������
		res=STMFLASH_WaitDone(0XFF);//�ȴ��������
		if(res==0)//�����ɹ�
		{
			*(vu16*)(faddr+2)=dat>>16;//д������
			res=STMFLASH_WaitDone(0XFF);//�ȴ��������
			if(res!=1)//�����ɹ�
			{
				FLASH->CR&=~(1<<0);//���PGλ.
			}
		}
		else if(res!=1)//�����ɹ�
		{
			//FLASH->CR&=~(1<<0);//���PGλ.
			FLASH->CR&=0x00001FFE;
		}
	} 
	
	//FLASH->CR |= 0x80;
	return res;
} 
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
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
//	    WriteAddr+=2;//��ַ����2.
//	}  
//} 

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
//void STMFLASH_Read(u32 ReadAddr,u8 *pBuffer,u16 NumToRead)   	
//{
//	u16 i,dt;
//	for(i=0;i<NumToRead;i++)
//	{
//		dt=*(vu16*)ReadAddr;//STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
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
//		ReadAddr+=2;//ƫ��2���ֽ�.	
//	}
//}

