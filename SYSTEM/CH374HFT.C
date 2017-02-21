
#include <stdio.h>
#include <string.h>
#include "SPI.h"
#include "led.h"
//#include "CH374HF9.H"

#include "CH374HFM.H"


//������ⲿ����
extern unsigned char leddata[8*1024];	//��ʾ�������� 	
extern struct ScrStruct  scr_info; 	  //�����Խṹ	
//extern u8 com;           //��u�̱�־
extern u32 flash_addr;   //flash�ĵ�ַ
extern u8 sim_buf_red[]; //��ɫ������ 
extern u8 buf[]; //������
extern unsigned char screeniniterr;

//�����ȫ�ֱ���
UINT16 SecCount;
UINT8	*my_buffer=&leddata[4096];

//�������ⲿ����
extern void flash_read_write(u8 s); 

/*******************************************************************************
* ��������  : CH374DelaymSs
* ��������  : ��ʱ
* ��ڲ���  : ��ʱ��ʱ��ֵ
* ���ز���  : ��
* ����˵��  : 
* �޸���־  ��2013.3.18 ������	
*******************************************************************************/
void CH374DelaymSs(u16 i)
{
	u16 j=5000;
	while(i--){
		while(j--);
		j=5000;
		}
}


///*******************************************************************************
//* ��������  : mStopIfError
//* ��������  : ������״̬
//* ��ڲ���  : ������״̬iError
//* ���ز���  : ��
//* ����˵��  : 
//* �޸���־  ��2013.3.18 ������	
//*******************************************************************************/
// /* ������״̬,�����������ʾ������벢ͣ�� */
//void	mStopIfError( UINT8 iError )
//{
//	if ( iError == ERR_SUCCESS ) return;  						/* �����ɹ� */
// 
//	if(iError==0x81)
//	{
//	CH374Reset(); //�������ʧ����370оƬ��λ
// 	return ;
//	}
// 
//}
void UdiskOperate()
{
	u16 i, c;	
	u16 n=*(u16*)0x0800fC30,m;//�����ļ�ʱ���
	u32 d,cnt;
	unsigned char  ucTmp,ucCnt;
	unsigned int LEDFileSize = 0, TotalCount = 0;
	unsigned int dwWriteCount = 0, dwFileSum = 0, dwSum = 0;
	
	OFF_CE;               //�ر�flash��CS
	
	
	/////////////////////////////������ʼ��������λ�����жϰ汾����ȡ��Ŀ/////////////
	strcpy( mCmdParam.Open.mPathName, NameCard);  
	
	if ( CH374FileOpen( ) == ERR_SUCCESS ) /* ���ļ� */ 
	{	
		mCmdParam.ByteRead.mByteCount = 4;
		CH374ByteRead( );  /* ���ֽ�Ϊ��λ��ȡ���ݿ�,���ζ�д�ĳ��Ȳ��ܳ���30,�ڶ��ε���ʱ���Ÿղŵ����� */
		
		LEDFileSize = (UINT32)mCmdParam.ByteRead.mByteBuffer[0] << 24 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[1] << 16 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[2] << 8
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[3];
		
		mCmdParam.ByteLocate.mByteOffset = LEDFileSize+1;  //��λ������
		CH374ByteLocate( );
		mCmdParam.ByteRead.mByteCount = 3;//��ȡ������汾
		CH374ByteRead( );  /* ���ֽ�Ϊ��λ��ȡ���ݿ�,���ζ�д�ĳ��Ȳ��ܳ���30,�ڶ��ε���ʱ���Ÿղŵ����� */
		
		if(mCmdParam.ByteRead.mByteBuffer[0] == card)
		{
				
				//if( (mCmdParam.ByteRead.mByteBuffer[1] == ((VISION>>8)&0xff))
				//	&& (mCmdParam.ByteRead.mByteBuffer[2] == (VISION&0xff)) )
			if( ((mCmdParam.ByteRead.mByteBuffer[1]<<8)|mCmdParam.ByteRead.mByteBuffer[2]) <= (VISION&0xffff))
				{
					//�汾��ͬ������
					CH374FileClose();                 //�ر��ļ�
				}
				else
				{
					if(LEDFileSize > 1024*1024 - 3)LEDFileSize = 1024*64;
					
					ucCnt = 0;
					do 
					{
						dwSum = 0;

						mCmdParam.ByteLocate.mByteOffset = 4;  //��λ���ļ���ʼ��ַ
						CH374ByteLocate( );
						
						flash_chip_erase();//����flash
						flash_addr = 0;
						
						TotalCount = LEDFileSize+1;//У��� 
						
						while ( TotalCount ) 
						{	/* ����ļ��Ƚϴ�,һ�ζ�����,�����ٵ���CH374ByteRead������ȡ,�ļ�ָ���Զ�����ƶ� */
							if ( TotalCount > 30 )
							{
								c = 30;  /* ʣ�����ݽ϶�,���Ƶ��ζ�д�ĳ��Ȳ��ܳ��� sizeof( mCmdParam.ByteRead.mByteBuffer ) */
							}
							else 
							{
								c = TotalCount;  /* ���ʣ����ֽ��� */
							}
							mCmdParam.ByteRead.mByteCount = c;	/* ���������ʮ�ֽ����� */
				
							CH374ByteRead( );  /* ���ֽ�Ϊ��λ��ȡ���ݿ�,���ζ�д�ĳ��Ȳ��ܳ���MAX_BYTE_IO,�ڶ��ε���ʱ���Ÿղŵ����� */
							TotalCount -= c;//mCmdParam.ByteRead.mByteCount;  /* ����,��ȥ��ǰʵ���Ѿ��������ַ��� */
					
							for ( i = 0; i != mCmdParam.ByteRead.mByteCount; i++ )//mCmdParam.ByteRead.mByteCount
							{
								ucTmp = mCmdParam.ByteRead.mByteBuffer[i];
								flash_byte_write(ucTmp);
								IWDG_Feed();//ι��
								if ( dwWriteCount >= LEDFileSize )
								{
									 dwFileSum <<= 8;
									 dwFileSum |=  ucTmp;
								}
								else
								{
										dwSum += ucTmp;
								}	
								dwWriteCount ++;
								flash_addr ++;
							}
						}

					} 
					while (  ((dwSum&0xff) != (dwFileSum&0xff)) && (ucCnt ++< 3)  );
					
					CH374FileClose();
					flash_read_write(1);//��
					/* У��ͨ�� ��д88 */
					if ( ucCnt < 4 )
					{	
//						sim_buf_red[0x78]=LEDFileSize;
//						sim_buf_red[0x79]=LEDFileSize>>8;
//						sim_buf_red[0x7a]=LEDFileSize>>16;
//						sim_buf_red[0x7b]=LEDFileSize>>24;
						
						sim_buf_red[0x70]=0x01;
						sim_buf_red[0x71]=0x00;
						
						sim_buf_red[0x72]=LEDFileSize;
						sim_buf_red[0x73]=LEDFileSize>>8;
						sim_buf_red[0x74]=LEDFileSize>>16;
						sim_buf_red[0x75]=LEDFileSize>>24;
						
						sim_buf_red[0x76]=(dwSum&0xff);
					}
					else
					{
						sim_buf_red[0x70]=0x00;
						sim_buf_red[0x71]=0x01;
					}
					flash_read_write(0);//д
					CH374FileClose();                 //�ر��ļ�
					cnt = 0xfff;
					while (cnt --);
					SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ������
					while(1);
				}//end else �汾��ͬ
			}//card
		CH374FileClose();                 //�ر��ļ�
	}//ERR_SUCCESS
	///////////////////////////////������������λ//////////////////////////////////////
	////////////////////////////////���ÿ�ʼ///////////////////////////////////////
	strcpy( mCmdParam.Open.mPathName, "\\CONROM" );
	if ( CH374FileOpen( ) == ERR_SUCCESS ) /* ���ļ� */ 
	{	
		i = CH374FileQuery( );  	 //��ѯ��ǰ�ļ�����Ϣ
		m=mCmdParam.Modify.mFileTime;
		if(n==m)
		{
			CH374FileClose();                 //�ر��ļ�
//			return;
		}
		else 
		{
			flash_read_write(1);      //���ڲ�flash��ֵ0x50�����浽������sim_buf_red�� 
			sim_buf_red[0x30]=m;
			sim_buf_red[0x31]=m>>8;
			flash_read_write(0);      //��flash����������sim_buf_red�е����ϲ�Ϊ32λ������flash��
				 
			mCmdParam.ReadX.mSectorCount = 1; 				// ָ����ȡ��������
			mCmdParam.ReadX.mDataBuffer = &my_buffer[0]; 	// ָ���ļ����ݻ���������ʼ��ַ
			CH374vFileSize=CH374vSectorSize;
			CH374FileReadX();
			if(my_buffer[0]){	   							//����ʱ��
				if(my_buffer[0]==1)settime(&my_buffer[1],0);
				else {
				d=my_buffer[0x0e]*3600+my_buffer[0x0f]*60;
				cnt=RTC_GetCounter();
				if(my_buffer[1])cnt-=d;			//�����ʱ��
				else cnt+=d;								//��ǰ��ʱ��
		//		RTC_Configuration();
		//    	RTC_WaitForLastTask();     //�ȴ��ϴβ������
		//    	RTC_SetCounter(cnt);       //�������������д��RTC�ļ�������
		//    	RTC_WaitForLastTask();     //�ȴ��ϴβ������
				RTC_SET(cnt);
				gettime();          //��ȡʱ�䣬����ʱ���ֵת��Ϊ���������ں�ʱ����ֱ����data32��time32��
				date_switch();      //����ʱ������ڼ����ũ��
				}
			}
		 
			if(my_buffer[0x10]){								//��ʱ���ػ�
				flash_read_write(1);
				for(i = 0; i < 9; i++)buf[i] = my_buffer[0x11 + i];
				if((buf[2] | (buf[3] << 8)) == 0){	  
					sim_buf_red[0x28] = 0xff;
					flash_read_write(0); //��flash����������sim_buf_red�е����ϲ�Ϊ32λ������flash��
					scr_info.of = 0;     //����
				}
				else {
					for(i = 0; i < 9; i++)sim_buf_red[i + 0x20] = buf[i];
					flash_read_write(0); //��flash����������sim_buf_red�е����ϲ�Ϊ32λ������flash��
				}
			}
		//     if(my_buffer[0x20]){ 							//��������
		// 	for(i=0;i<18;i++)buf[i]=my_buffer[0x21+i];
		// 	date_limt_set();
		// 	}
			if(my_buffer[0x40]){  								
				if(my_buffer[0x41]){  //��ʱ����
					if(my_buffer[0x42]==0xff){
							if(*(u8*)(0x0800FC05)==0x88){
							flash_read_write(1); 					
							sim_buf_red[0x05]=0xff;
							flash_read_write(0);	
						}	
					}else{
						for(i=6;i<18;i++)buf[i]=my_buffer[0x42-6+i];
						timer_bright_set();	
					}
				}
				else {			  //�ֶ�����
					i=0xff-my_buffer[0x42]; 
					if(i==0)i=16;
					flash_read_write(1); 						//0x29 �������ֵ
					sim_buf_red[0x29]=i;
					flash_read_write(0);

				}
			}
			rcc_set(my_buffer[0x50]);					//��Ƶ
			CH374FileClose();                 //�ر��ļ�
			cnt = 0xfff;
			while (cnt --);
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ������
		}
	}
	////////////////////////////���ý���////////////////////////////////////////
	///////////////////////////////��Ŀ��ʼ////////////////////////////////////
	strcpy( mCmdParam.Open.mPathName, "\\LEDROM" );  
	
	if ( CH374FileOpen( ) == ERR_SUCCESS ) /* ���ļ� */ 
	{	
		mCmdParam.ByteRead.mByteCount = 4;
		CH374ByteRead( );  /* ���ֽ�Ϊ��λ��ȡ���ݿ�,���ζ�д�ĳ��Ȳ��ܳ���30,�ڶ��ε���ʱ���Ÿղŵ����� */
		
		LEDFileSize = (UINT32)mCmdParam.ByteRead.mByteBuffer[0] << 16 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[1] << 8 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[2];
		
			if(LEDFileSize>bitmaxlen)
			{
				CH374FileClose();                 //�ر��ļ�
				screeniniterr=4;
				return;
			}
// 		if(LEDFileSize > 1024*1024 - 3)LEDFileSize = 1024*64;
		
		ucCnt = 0;
		do 
		{
			dwSum = 0;

			mCmdParam.ByteLocate.mByteOffset = 3;  
			CH374ByteLocate( );
			
			flash_chip_erase();//����flash
			flash_addr = 2;
			
			TotalCount = LEDFileSize+8+4;
			LEDFileSize=LEDFileSize+8;
			
			while ( TotalCount ) 
			{	/* ����ļ��Ƚϴ�,һ�ζ�����,�����ٵ���CH374ByteRead������ȡ,�ļ�ָ���Զ�����ƶ� */
				if ( TotalCount > 30 )
				{
					c = 30;  /* ʣ�����ݽ϶�,���Ƶ��ζ�д�ĳ��Ȳ��ܳ��� sizeof( mCmdParam.ByteRead.mByteBuffer ) */
				}
				else 
				{
					c = TotalCount;  /* ���ʣ����ֽ��� */
				}
				mCmdParam.ByteRead.mByteCount = c;	/* ���������ʮ�ֽ����� */
	
				CH374ByteRead( );  /* ���ֽ�Ϊ��λ��ȡ���ݿ�,���ζ�д�ĳ��Ȳ��ܳ���MAX_BYTE_IO,�ڶ��ε���ʱ���Ÿղŵ����� */
				TotalCount -= c;//mCmdParam.ByteRead.mByteCount;  /* ����,��ȥ��ǰʵ���Ѿ��������ַ��� */
		
				for ( i = 0; i != mCmdParam.ByteRead.mByteCount; i++ )//mCmdParam.ByteRead.mByteCount
				{
					ucTmp = mCmdParam.ByteRead.mByteBuffer[i];
					flash_byte_write(ucTmp);
					IWDG_Feed();//ι��
					if ( dwWriteCount >= LEDFileSize )
					{
						 dwFileSum <<= 8;
						 dwFileSum |=  ucTmp;
					}
					else
					{
							dwSum += ucTmp;
					}	
					dwWriteCount ++;
					flash_addr ++;
				}
			}

		} 
		while ((dwSum != dwFileSum) && (ucCnt ++< 3));
		
		/* У��ͨ�� ��д88 */
		if ( ucCnt < 4 )
		{	
			flash_read_write(1);//��
			sim_buf_red[0x78]=LEDFileSize;
			sim_buf_red[0x79]=LEDFileSize>>8;
			sim_buf_red[0x7a]=LEDFileSize>>16;
			sim_buf_red[0x7b]=LEDFileSize>>24;
			flash_read_write(0);//��
			flash_addr=0; 
			flash_byte_write(0x88);
			CH374FileClose();
		}
		CH374FileClose();                 //�ر��ļ�
	}
	/////////////////////////////////��Ŀ����////////////////////////////////////////
}



void ch734init()
{
	u8 i,d,ConnectStatus;
	
	OFF_CE;  //��ʼ��u��оƬʱ�ص�spi��flash�����Ƭѡ
	GPIOB->BSRR=0x20;    //PB5 = 1;
	CH374DelaymSs( 200 );
	GPIOB->BRR=0x20;      //PB5 = 0;
	CH374Reset();
	CH374DelaymSs( 200 );  										/* ��ʱ100���� */  											/* Ϊ���ü����ͨ�����ڼ����ʾ���� */
	ConnectStatus=0;
Init:					
	i = CH374LibInit( ); //370оƬ��ʼ��,���س�ʼ��֮���״̬
	CH374DelaymSs(100);
	if(i)goto Init;//˵����ʼ��ʧ�ܣ��ٴγ�ʼ��

	for ( i = 0; i < 20; i ++ ) 
	{   /* ��ȴ�ʱ��,20*50mS */
		CH374DelaymSs( 200 );
		d=CH374DiskConnect();    //���u���Ƿ�׼���ã�����״̬��ֵd
		if ( d == ERR_SUCCESS ) 	/* ��ѯ�����Ƿ�׼���� */
		{
			ConnectStatus=1;
			break;
		}
		if ( CH374DiskStatus < DISK_CONNECT ) break;  		/* ��⵽�Ͽ�,���¼�Ⲣ��ʱ */
		if ( CH374DiskStatus >= DISK_MOUNTED && i > 5 ) break;  /* �е�U�����Ƿ���δ׼����,�������Ժ���,ֻҪ�佨������MOUNTED�ҳ���5*50mS */
	}
	
	if(ConnectStatus==0)
		return;
	
	for ( i = 0; i < 20; i ++ ) 
	{   /* ��ȴ�ʱ��,20*50mS */
		CH374DelaymSs( 200 );
		d=CH374DiskReady();    //���u���Ƿ�׼���ã�����״̬��ֵd
		if ( d == ERR_SUCCESS ) 	/* ��ѯ�����Ƿ�׼���� */
		{
			ConnectStatus=2;
			break;
		}
	}
	if(ConnectStatus==0)
		goto Init;
	
	
	UdiskOperate();
 
}
