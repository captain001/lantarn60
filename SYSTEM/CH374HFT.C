
#include <stdio.h>
#include <string.h>
#include "SPI.h"
#include "led.h"
//#include "CH374HF9.H"

#include "CH374HFM.H"


//定义的外部变量
extern unsigned char leddata[8*1024];	//显示缓冲数组 	
extern struct ScrStruct  scr_info; 	  //屏属性结构	
//extern u8 com;           //读u盘标志
extern u32 flash_addr;   //flash的地址
extern u8 sim_buf_red[]; //红色缓冲区 
extern u8 buf[]; //缓冲区
extern unsigned char screeniniterr;

//定义的全局变量
UINT16 SecCount;
UINT8	*my_buffer=&leddata[4096];

//声明的外部函数
extern void flash_read_write(u8 s); 

/*******************************************************************************
* 函数名称  : CH374DelaymSs
* 函数功能  : 延时
* 入口参数  : 延时的时间值
* 返回参数  : 无
* 函数说明  : 
* 修改日志  ：2013.3.18 朱振威	
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
//* 函数名称  : mStopIfError
//* 函数功能  : 检查操作状态
//* 入口参数  : 操作的状态iError
//* 返回参数  : 无
//* 函数说明  : 
//* 修改日志  ：2013.3.18 朱振威	
//*******************************************************************************/
// /* 检查操作状态,如果错误则显示错误代码并停机 */
//void	mStopIfError( UINT8 iError )
//{
//	if ( iError == ERR_SUCCESS ) return;  						/* 操作成功 */
// 
//	if(iError==0x81)
//	{
//	CH374Reset(); //如果操作失败则370芯片复位
// 	return ;
//	}
// 
//}
void UdiskOperate()
{
	u16 i, c;	
	u16 n=*(u16*)0x0800fC30,m;//配置文件时间戳
	u32 d,cnt;
	unsigned char  ucTmp,ucCnt;
	unsigned int LEDFileSize = 0, TotalCount = 0;
	unsigned int dwWriteCount = 0, dwFileSum = 0, dwSum = 0;
	
	OFF_CE;               //关闭flash的CS
	
	
	/////////////////////////////升级开始，结束后复位，在判断版本，读取节目/////////////
	strcpy( mCmdParam.Open.mPathName, NameCard);  
	
	if ( CH374FileOpen( ) == ERR_SUCCESS ) /* 打开文件 */ 
	{	
		mCmdParam.ByteRead.mByteCount = 4;
		CH374ByteRead( );  /* 以字节为单位读取数据块,单次读写的长度不能超过30,第二次调用时接着刚才的向后读 */
		
		LEDFileSize = (UINT32)mCmdParam.ByteRead.mByteBuffer[0] << 24 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[1] << 16 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[2] << 8
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[3];
		
		mCmdParam.ByteLocate.mByteOffset = LEDFileSize+1;  //定位到卡型
		CH374ByteLocate( );
		mCmdParam.ByteRead.mByteCount = 3;//读取卡型与版本
		CH374ByteRead( );  /* 以字节为单位读取数据块,单次读写的长度不能超过30,第二次调用时接着刚才的向后读 */
		
		if(mCmdParam.ByteRead.mByteBuffer[0] == card)
		{
				
				//if( (mCmdParam.ByteRead.mByteBuffer[1] == ((VISION>>8)&0xff))
				//	&& (mCmdParam.ByteRead.mByteBuffer[2] == (VISION&0xff)) )
			if( ((mCmdParam.ByteRead.mByteBuffer[1]<<8)|mCmdParam.ByteRead.mByteBuffer[2]) <= (VISION&0xffff))
				{
					//版本相同不升级
					CH374FileClose();                 //关闭文件
				}
				else
				{
					if(LEDFileSize > 1024*1024 - 3)LEDFileSize = 1024*64;
					
					ucCnt = 0;
					do 
					{
						dwSum = 0;

						mCmdParam.ByteLocate.mByteOffset = 4;  //定位到文件起始地址
						CH374ByteLocate( );
						
						flash_chip_erase();//擦除flash
						flash_addr = 0;
						
						TotalCount = LEDFileSize+1;//校验和 
						
						while ( TotalCount ) 
						{	/* 如果文件比较大,一次读不完,可以再调用CH374ByteRead继续读取,文件指针自动向后移动 */
							if ( TotalCount > 30 )
							{
								c = 30;  /* 剩余数据较多,限制单次读写的长度不能超过 sizeof( mCmdParam.ByteRead.mByteBuffer ) */
							}
							else 
							{
								c = TotalCount;  /* 最后剩余的字节数 */
							}
							mCmdParam.ByteRead.mByteCount = c;	/* 请求读出几十字节数据 */
				
							CH374ByteRead( );  /* 以字节为单位读取数据块,单次读写的长度不能超过MAX_BYTE_IO,第二次调用时接着刚才的向后读 */
							TotalCount -= c;//mCmdParam.ByteRead.mByteCount;  /* 计数,减去当前实际已经读出的字符数 */
					
							for ( i = 0; i != mCmdParam.ByteRead.mByteCount; i++ )//mCmdParam.ByteRead.mByteCount
							{
								ucTmp = mCmdParam.ByteRead.mByteBuffer[i];
								flash_byte_write(ucTmp);
								IWDG_Feed();//喂狗
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
					flash_read_write(1);//读
					/* 校验通过 ，写88 */
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
					flash_read_write(0);//写
					CH374FileClose();                 //关闭文件
					cnt = 0xfff;
					while (cnt --);
					SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位，升级
					while(1);
				}//end else 版本不同
			}//card
		CH374FileClose();                 //关闭文件
	}//ERR_SUCCESS
	///////////////////////////////升级结束，复位//////////////////////////////////////
	////////////////////////////////配置开始///////////////////////////////////////
	strcpy( mCmdParam.Open.mPathName, "\\CONROM" );
	if ( CH374FileOpen( ) == ERR_SUCCESS ) /* 打开文件 */ 
	{	
		i = CH374FileQuery( );  	 //查询当前文件的信息
		m=mCmdParam.Modify.mFileTime;
		if(n==m)
		{
			CH374FileClose();                 //关闭文件
//			return;
		}
		else 
		{
			flash_read_write(1);      //读内部flash的值0x50个，存到缓冲区sim_buf_red中 
			sim_buf_red[0x30]=m;
			sim_buf_red[0x31]=m>>8;
			flash_read_write(0);      //将flash擦除，并将sim_buf_red中的数合并为32位，存入flash中
				 
			mCmdParam.ReadX.mSectorCount = 1; 				// 指定读取的扇区数
			mCmdParam.ReadX.mDataBuffer = &my_buffer[0]; 	// 指向文件数据缓冲区的起始地址
			CH374vFileSize=CH374vSectorSize;
			CH374FileReadX();
			if(my_buffer[0]){	   							//调整时间
				if(my_buffer[0]==1)settime(&my_buffer[1],0);
				else {
				d=my_buffer[0x0e]*3600+my_buffer[0x0f]*60;
				cnt=RTC_GetCounter();
				if(my_buffer[1])cnt-=d;			//往后调时间
				else cnt+=d;								//往前调时间
		//		RTC_Configuration();
		//    	RTC_WaitForLastTask();     //等待上次操作完成
		//    	RTC_SetCounter(cnt);       //将调整后的数据写入RTC的计数器中
		//    	RTC_WaitForLastTask();     //等待上次操作完成
				RTC_SET(cnt);
				gettime();          //读取时间，并将时间的值转化为年月日星期和时分秒分别存入data32和time32中
				date_switch();      //根据时间和日期计算出农历
				}
			}
		 
			if(my_buffer[0x10]){								//定时开关机
				flash_read_write(1);
				for(i = 0; i < 9; i++)buf[i] = my_buffer[0x11 + i];
				if((buf[2] | (buf[3] << 8)) == 0){	  
					sim_buf_red[0x28] = 0xff;
					flash_read_write(0); //将flash擦除，并将sim_buf_red中的数合并为32位，存入flash中
					scr_info.of = 0;     //开机
				}
				else {
					for(i = 0; i < 9; i++)sim_buf_red[i + 0x20] = buf[i];
					flash_read_write(0); //将flash擦除，并将sim_buf_red中的数合并为32位，存入flash中
				}
			}
		//     if(my_buffer[0x20]){ 							//日期限制
		// 	for(i=0;i<18;i++)buf[i]=my_buffer[0x21+i];
		// 	date_limt_set();
		// 	}
			if(my_buffer[0x40]){  								
				if(my_buffer[0x41]){  //定时调亮
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
				else {			  //手动调亮
					i=0xff-my_buffer[0x42]; 
					if(i==0)i=16;
					flash_read_write(1); 						//0x29 存放亮度值
					sim_buf_red[0x29]=i;
					flash_read_write(0);

				}
			}
			rcc_set(my_buffer[0x50]);					//点频
			CH374FileClose();                 //关闭文件
			cnt = 0xfff;
			while (cnt --);
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位，升级
		}
	}
	////////////////////////////配置结束////////////////////////////////////////
	///////////////////////////////节目开始////////////////////////////////////
	strcpy( mCmdParam.Open.mPathName, "\\LEDROM" );  
	
	if ( CH374FileOpen( ) == ERR_SUCCESS ) /* 打开文件 */ 
	{	
		mCmdParam.ByteRead.mByteCount = 4;
		CH374ByteRead( );  /* 以字节为单位读取数据块,单次读写的长度不能超过30,第二次调用时接着刚才的向后读 */
		
		LEDFileSize = (UINT32)mCmdParam.ByteRead.mByteBuffer[0] << 16 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[1] << 8 
					| (UINT32)mCmdParam.ByteRead.mByteBuffer[2];
		
			if(LEDFileSize>bitmaxlen)
			{
				CH374FileClose();                 //关闭文件
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
			
			flash_chip_erase();//擦除flash
			flash_addr = 2;
			
			TotalCount = LEDFileSize+8+4;
			LEDFileSize=LEDFileSize+8;
			
			while ( TotalCount ) 
			{	/* 如果文件比较大,一次读不完,可以再调用CH374ByteRead继续读取,文件指针自动向后移动 */
				if ( TotalCount > 30 )
				{
					c = 30;  /* 剩余数据较多,限制单次读写的长度不能超过 sizeof( mCmdParam.ByteRead.mByteBuffer ) */
				}
				else 
				{
					c = TotalCount;  /* 最后剩余的字节数 */
				}
				mCmdParam.ByteRead.mByteCount = c;	/* 请求读出几十字节数据 */
	
				CH374ByteRead( );  /* 以字节为单位读取数据块,单次读写的长度不能超过MAX_BYTE_IO,第二次调用时接着刚才的向后读 */
				TotalCount -= c;//mCmdParam.ByteRead.mByteCount;  /* 计数,减去当前实际已经读出的字符数 */
		
				for ( i = 0; i != mCmdParam.ByteRead.mByteCount; i++ )//mCmdParam.ByteRead.mByteCount
				{
					ucTmp = mCmdParam.ByteRead.mByteBuffer[i];
					flash_byte_write(ucTmp);
					IWDG_Feed();//喂狗
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
		
		/* 校验通过 ，写88 */
		if ( ucCnt < 4 )
		{	
			flash_read_write(1);//读
			sim_buf_red[0x78]=LEDFileSize;
			sim_buf_red[0x79]=LEDFileSize>>8;
			sim_buf_red[0x7a]=LEDFileSize>>16;
			sim_buf_red[0x7b]=LEDFileSize>>24;
			flash_read_write(0);//读
			flash_addr=0; 
			flash_byte_write(0x88);
			CH374FileClose();
		}
		CH374FileClose();                 //关闭文件
	}
	/////////////////////////////////节目结束////////////////////////////////////////
}



void ch734init()
{
	u8 i,d,ConnectStatus;
	
	OFF_CE;  //初始化u盘芯片时关掉spi与flash传输的片选
	GPIOB->BSRR=0x20;    //PB5 = 1;
	CH374DelaymSs( 200 );
	GPIOB->BRR=0x20;      //PB5 = 0;
	CH374Reset();
	CH374DelaymSs( 200 );  										/* 延时100毫秒 */  											/* 为了让计算机通过串口监控演示过程 */
	ConnectStatus=0;
Init:					
	i = CH374LibInit( ); //370芯片初始化,返回初始化之后的状态
	CH374DelaymSs(100);
	if(i)goto Init;//说明初始化失败，再次初始化

	for ( i = 0; i < 20; i ++ ) 
	{   /* 最长等待时间,20*50mS */
		CH374DelaymSs( 200 );
		d=CH374DiskConnect();    //检测u盘是否准备好，并把状态赋值d
		if ( d == ERR_SUCCESS ) 	/* 查询磁盘是否准备好 */
		{
			ConnectStatus=1;
			break;
		}
		if ( CH374DiskStatus < DISK_CONNECT ) break;  		/* 检测到断开,重新检测并计时 */
		if ( CH374DiskStatus >= DISK_MOUNTED && i > 5 ) break;  /* 有的U盘总是返回未准备好,不过可以忽略,只要其建立连接MOUNTED且尝试5*50mS */
	}
	
	if(ConnectStatus==0)
		return;
	
	for ( i = 0; i < 20; i ++ ) 
	{   /* 最长等待时间,20*50mS */
		CH374DelaymSs( 200 );
		d=CH374DiskReady();    //检测u盘是否准备好，并把状态赋值d
		if ( d == ERR_SUCCESS ) 	/* 查询磁盘是否准备好 */
		{
			ConnectStatus=2;
			break;
		}
	}
	if(ConnectStatus==0)
		goto Init;
	
	
	UdiskOperate();
 
}

