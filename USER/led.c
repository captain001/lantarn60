#include "led.h"
#include "clock.h"
#include "stmlib.h"
#include  "hb.h"

u8 ledtime[14];//保存节目时间
//版本号取模
const u8 ucaShowVersion[12][16] = {	//12.11.14
	{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"0",0*/
	{0x00,0x00,0x00,0x10,0x70,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7C,0x00,0x00},/*"1",1*/
	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x04,0x04,0x08,0x10,0x20,0x42,0x7E,0x00,0x00},/*"2",2*/
	{0x00,0x00,0x00,0x3C,0x42,0x42,0x04,0x18,0x04,0x02,0x02,0x42,0x44,0x38,0x00,0x00},/*"3",3*/
	{0x00,0x00,0x00,0x04,0x0C,0x14,0x24,0x24,0x44,0x44,0x7E,0x04,0x04,0x1E,0x00,0x00},/*"4",4*/
	{0x00,0x00,0x00,0x7E,0x40,0x40,0x40,0x58,0x64,0x02,0x02,0x42,0x44,0x38,0x00,0x00},/*"5",5*/
	{0x00,0x00,0x00,0x1C,0x24,0x40,0x40,0x58,0x64,0x42,0x42,0x42,0x24,0x18,0x00,0x00},/*"6",6*/
	{0x00,0x00,0x00,0x7E,0x44,0x44,0x08,0x08,0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00},/*"7",7*/
	{0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x3C,0x00,0x00},/*"8",8*/
	{0x00,0x00,0x00,0x18,0x24,0x42,0x42,0x42,0x26,0x1A,0x02,0x02,0x24,0x38,0x00,0x00},/*"9",9*/
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,0x00},/*".",10*/
	{0x00,0x00,0x00,0xE7,0x42,0x42,0x44,0x24,0x24,0x28,0x28,0x18,0x10,0x10,0x00,0x00},/*"V",11*/

};

const u8 PowerManager[3][32]={
{0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x0A,0x01,0x02,0x01,0x02,0x00,0xFE},/*"电",0*/
{0x00,0x00,0x3F,0xFE,0x20,0x00,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x2F,0xFC,0x20,0x80,0x20,0x80,0x20,0x90,0x20,0x88,0x20,0x88,0x40,0x80,0x5F,0xFE,0x80,0x00},/*"压",1*/
{0x08,0x08,0x08,0x3C,0x0B,0xE0,0x12,0x20,0x12,0x20,0x32,0x20,0x32,0x20,0x53,0xFE,0x92,0x20,0x12,0x10,0x12,0x10,0x12,0x12,0x12,0x0A,0x12,0x8A,0x13,0x26,0x12,0x12},/*"低",2*/
};
const u8 lenerr[4][32]={//超范围
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x08,0x00,0x09,0xFC,0x08,0x44,0x7E,0x44,0x08,0x44,0x08,0x94,0xFF,0x08,0x08,0xFC,
0x28,0x84,0x28,0x84,0x2E,0x84,0x28,0xFC,0x28,0x00,0x58,0x00,0x4F,0xFE,0x80,0x00},/*"?",2*/
{0x04,0x40,0x04,0x40,0xFF,0xFE,0x04,0x40,0x10,0x00,0x09,0xF8,0x49,0x08,0x21,0x08,
0x25,0x08,0x05,0x28,0x09,0x10,0x71,0x02,0x11,0x02,0x11,0x02,0x10,0xFE,0x00,0x00},/*"?",3*/
{0x00,0x00,0x7F,0xFC,0x41,0x04,0x41,0x04,0x5F,0xF4,0x41,0x04,0x4F,0xE4,0x41,0x04,
0x5F,0xF4,0x41,0x14,0x41,0x14,0x41,0x54,0x41,0x24,0x41,0x04,0x7F,0xFC,0x40,0x04},/*"?",4*/
};
const u8 scanerr[4][32]={//请升级卡
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x00,0x40,0x40,0x40,0x27,0xFC,0x20,0x40,0x03,0xF8,0x00,0x40,0xE7,0xFE,0x20,0x00,
0x23,0xF8,0x22,0x08,0x23,0xF8,0x22,0x08,0x2B,0xF8,0x32,0x08,0x22,0x28,0x02,0x10},/*"?",2*/
{0x01,0x20,0x07,0xA0,0x3C,0x20,0x04,0x20,0x04,0x20,0x04,0x20,0x04,0x20,0xFF,0xFE,
0x04,0x20,0x04,0x20,0x04,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x20,0x20,0x40,0x20},/*"?",3*/
{0x10,0x00,0x13,0xFC,0x20,0x84,0x20,0x88,0x48,0x88,0xF8,0x90,0x10,0x9C,0x20,0x84,
0x41,0x44,0xF9,0x44,0x41,0x28,0x01,0x28,0x1A,0x10,0xE2,0x28,0x44,0x44,0x01,0x82},/*"?",4*/
};
const u8 proerr[4][32]={//校验失败
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",3*/
{0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,
0x54,0x88,0x50,0x88,0x90,0x50,0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06},/*"?",4*/
{0x00,0x20,0xF8,0x20,0x08,0x50,0x48,0x50,0x48,0x88,0x49,0x04,0x4A,0xFA,0x7C,0x00,
0x04,0x44,0x04,0x24,0x1D,0x24,0xE4,0xA8,0x44,0x88,0x04,0x10,0x2B,0xFE,0x10,0x00},/*"?",5*/
};
const u8 longerr[4][32]={//节目大
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x08,0x20,0x08,0x20,0xFF,0xFE,0x08,0x20,0x08,0x20,0x00,0x00,0x7F,0xF8,0x02,0x08,
0x02,0x08,0x02,0x08,0x02,0x08,0x02,0x50,0x02,0x20,0x02,0x00,0x02,0x00,0x02,0x00},/*"?",2*/
{0x00,0x00,0x3F,0xF8,0x20,0x08,0x20,0x08,0x20,0x08,0x3F,0xF8,0x20,0x08,0x20,0x08,
0x20,0x08,0x3F,0xF8,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x3F,0xF8,0x20,0x08},/*"?",3*/
{0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,
0x02,0x80,0x02,0x80,0x04,0x40,0x04,0x40,0x08,0x20,0x10,0x10,0x20,0x08,0xC0,0x06},/*"?",4*/
};
const u8 rtcerr[4][32]={//节目大

{0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,0x54,0x88,0x50,0x88,0x90,0x50,0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06},/*"校",0*/
{0x00,0x08,0x00,0x08,0x7C,0x08,0x44,0x08,0x45,0xFE,0x44,0x08,0x44,0x08,0x7C,0x08,0x44,0x88,0x44,0x48,0x44,0x48,0x44,0x08,0x7C,0x08,0x44,0x08,0x00,0x28,0x00,0x10},/*"时",1*/
{0x01,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x3F,0xF8,0x21,0x00,0x41,0x00,0x01,0x00,0xFF,0xFE,0x02,0x80,0x04,0x40,0x04,0x40,0x08,0x20,0x10,0x10,0x20,0x08,0xC0,0x06},/*"失",2*/
{0x00,0x40,0x7C,0x40,0x44,0x40,0x54,0x80,0x54,0xFE,0x55,0x08,0x56,0x88,0x54,0x88,0x54,0x88,0x54,0x50,0x54,0x50,0x10,0x20,0x28,0x50,0x24,0x88,0x45,0x04,0x82,0x02},/*"败",3*/
};
//函数声明
void (*led_play)(void);

//定义结构体变量
struct ScrStruct  scr_info; //屏属性结构
struct AreaStruct area[8];  //分区属性结构
struct AreaStruct *pArg;    //分区属性结构
struct SimStruct sim;       //模拟时钟属性结构
extern struct hbStruct hb;  //花边属性结构

//定义全局变量
unsigned short g_wShowVersionCnt = 0;
unsigned char g_ucChangeScan = 0;

unsigned char flashcheckerr=0;//初始值屏参
unsigned char screeniniterr=0,scan_play=0;//屏参长度宽度，或扫描出错
unsigned char offset_green,pro_flag; //绿色数据的偏移量、节目播放的标志
unsigned short screen_len;  //屏长(按字节)
unsigned char area_num,area_addr,area_c=0,bright,ticks;	//分区个数、分区地址、分区计数器、亮度、计数
unsigned char sec_flag,pro_num,info_num,pro_c,offset=1; //秒处理标志、节目总数、信息总数、节目计数器、单双色偏移
unsigned short datn16,show_timer; //存放农历、定长播放计数
unsigned long time_sj=0,flash_addr,pro_addr;//随机时间的取值变量、flash的地址、节目的地址
unsigned short var;  //调节亮度

unsigned char		mm_set_en;		//=1允许设置WIFI密码
unsigned char		scr_onoff;		//屏幕开关标志  0开  1关

#define		INTERVAL_PATTERN_ADDR			0x22		//间隔花样开关
#define		TIME_ONOFF_ADDR			0x20		//时间开关
#define		DISP_PATTERN_ADDR		0x21		//显示模式
#define		BRIGHT_BG_ADDR			0x2a		//背景亮度
#define		BRIGHT_WORD_ADDR		0x29		//文字亮度


//#define		TIME_ONOFF_ADDR			0X84
//
//time_on_off   disp_Pattern   time_on_off  set_bright_hei

//定义数组
unsigned char leddata[8*1024];  //显示缓冲数组
//unsigned char leddata[8*128];  //显示缓冲数组

/* flash是否为空标记  2013.3.18朱振威 */
//static unsigned char g_ucisFlashEmpty = 0;

unsigned char buf[40];

//定义外部变量
extern unsigned char h,hx;
extern unsigned char dis_length;
extern unsigned long time32,date32;
extern unsigned short usBright;
extern u8 cardNum[cardLength];//

/***********模拟时钟全局变量*****************/
unsigned char sim_buf_red[1024];		//红色缓冲区 
unsigned char sim_buf_green[1024];	//绿色缓冲区
#ifdef CONFIG_SEVEN_COLOR
 	unsigned char sim_buf_blue[1024];		//蓝色缓冲区
#endif

/*
 *显示“电压低”
 *13.5.31 ZZW
// */
void LEDPowerLow(void)
{
	u16 i,j;
		/* 电压低时关闭FLASH片选 */
		OFF_CE;
// 		for(j=0;j<16;j++){
// 	  for(i = 0; i< 8; i++){
// 	  	leddata[(j + 1) * SCREEN_LEN - 6 + i] = PowerManager[i / 2][i % 2 + 2 * j];	
// 			}
// 		}
}

/**************************************************************************
** 函数名称 ： delay
** 函数功能 ： 延时函数
** 入口参数 ： 延时时间time
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月10日修改	
**************************************************************************/
static void delay(unsigned long time){
	while(time--);
}


/**************************************************************************
** 函数名称 ： flash_read_write
** 函数功能 ： 内部flash读写操作
** 入口参数 ： 读写标志s
** 返回参数 ： 无
** 函数说明 ： s=1，从内部flash中读出0x50个数据写入sim_buf_red中
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void flash_read_write(u8 s)
{
	u8 *p=(u8*)(0x0800FC00),i;
	u32 r1,addr=0x0800FC00;
	if ( rPWR_CSR & 4 )return;
	if(s){	 	//保存flash的值到缓存中
	for(i=0;i<0x80;i++) sim_buf_red[i]=*p++;
	}
	else {		 //擦除并写flash
	FLASH_Unlock();	 //FLASH解锁
//    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	//清标志位
	FLASH_ErasePage(addr); //按页擦除
	for(i=0;i<0x80;i+=4){
	r1=sim_buf_red[i+3];
	r1<<=8;
	r1|=sim_buf_red[i+2];
	r1<<=8;
	r1|=sim_buf_red[i+1];
	r1<<=8;
	r1|=sim_buf_red[i];
	STMFLASH_WriteWord(addr, r1); //读出0x50个数据写入flash中
	addr+=4;
	}
	scr_info.st=1; //节目停留标志
	pro_c=0;				
	}	

}

/*****************播放节目之前显示版本号12.11.14********************************/
/**************************************************************************
** 函数名称 ： LEDShowVersion
** 函数功能 ： 给显示缓冲区leddata赋值为版本号
** 入口参数 ： 取模数组的第index个数组，写入的个数n，写入缓冲区的位置pos
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/



void ShowRtcError(void)
{

	u16 i,j;
	for(j=0;j<16;j++)
				{
					for(i = 0; i< 8; i++)
					{
						leddata[(j + 1) * SCREEN_LEN - 8 + i] = rtcerr[i / 2][i % 2 + 2 * j];
					}
				}	
	GREEN_BEGIN
		for(j=0;j<16;j++)
				{
					for(i = 0; i< 8; i++)
					{
						leddata[(j + 1) * SCREEN_LEN - 8 + i] = rtcerr[i / 2][i % 2 + 2 * j];
					}
				}	
	GREEN_END	



}



static void LEDShowVersion(u8 index, u8 n, u8 pos)		//字模位置，字模宽度，目标位置
{
	u16 i,j;
	for(j=0;j<16;j++)
	  for(i = 0; i < n; i++){
		leddata[0x68+(j + 1) * screen_len - 8 + pos+i] = ucaShowVersion[index+i][j];	//加0x68是为了显示居中  8*13=104
 	  }  
	GREEN_BEGIN
		for(j=0;j<16;j++)
		  for(i = 0; i < n; i++){
			leddata[(j + OFFSET_GREEN +1) * screen_len - 8 + pos+i] = ucaShowVersion[index+i][j];	
 		  } 		
	GREEN_END	
  
}
 
/**************************************************************************
** 函数名称 ： LEDShowModifyDate
** 函数功能 ： 给显示缓冲区leddata赋值为修改日期
** 入口参数 ： 取模数组的第index个数组，写入的个数n，写入缓冲区的位置pos
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
static void LEDShowModifyDate(u8 index, u8 n, u8 pos)
{
	u16 i,j;
	for(j=16;j<32;j++)
	  for(i = 0; i < n; i++){
		leddata[(j + 1) * screen_len - 8 + pos+i] = ucaShowVersion[index+i][j - 16];	
 	  }  

  	GREEN_BEGIN
		for(j=16;j<32;j++)
		  for(i = 0; i < n; i++){
			leddata[(j + OFFSET_GREEN + 1) * screen_len - 8 + pos+i] = ucaShowVersion[index+i][j - 16];	
			
		  } 
	GREEN_END 
}



/**************************************************************************
** 函数名称 ： ShowVersion
** 函数功能 ： 显示版本号
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
// void ShowVersion(void)
// { 	
// 	LEDShowVersion(11, 1, 3);//显示‘V’
// 	LEDShowVersion(VISION/0x100%0x10, 1, 4);//显示‘1’
// 	LEDShowVersion(10, 1, 5);//显示‘.’
// 	LEDShowVersion(VISION/0x10%0x10, 1, 6);//显示‘2’
// 	LEDShowVersion(VISION%0x10, 1, 7);//显示‘3’(当前版本号)
// 	
// 	LEDShowModifyDate(MODIFYDATE/0X100000%0x10, 1, 0);
// 	LEDShowModifyDate(MODIFYDATE/0X10000%0x10, 1, 1);	
// 	LEDShowModifyDate(10, 1, 2);//显示‘.’
// 	LEDShowModifyDate(MODIFYDATE/0X1000%0x10, 1, 3);
// 	LEDShowModifyDate(MODIFYDATE/0X100%0x10, 1, 4);
// 	LEDShowModifyDate(10, 1, 5);//显示‘.’
// 	LEDShowModifyDate(MODIFYDATE/0X10%0x10, 1, 6);
// 	LEDShowModifyDate(MODIFYDATE%0x10, 1, 7);
// }

void ShowVersion(void)
{ 	
	LEDShowVersion(11, 1, 0);//显示‘V’
	LEDShowVersion(VISION/0x100%0x10, 1, 1);//显示‘1’
	LEDShowVersion(10, 1, 2);//显示‘.’
	LEDShowVersion(VISION/0x10%0x10, 1, 3);//显示‘2’
	LEDShowVersion(VISION%0x10, 1, 4);//显示‘3’(当前版本号)
	ledplay_denglong();
// 	IWDG_Feed();	
// 	delay(8880000);	
// 	delay(8880000);	
// 	IWDG_Feed();	
// 	delay(8880000);	
// 	delay(8880000);	
	//while(1);
}

/**************************************************************************
** 函数名称 ： clr_screen
** 函数功能 ： 清屏
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
// void clr_screen()
// {
//   unsigned short i,j;
// //	register u16 *p=(u16 *)0x40010c0c;//GPIOB输出数据寄存器的地址
// 	register volatile unsigned long *p;
// 	p=( volatile unsigned long *)0x40010c10;
// 	SET_OE(!scr_info.OE);//关OE
// 	memset(leddata,0,sizeof(leddata));//将缓冲区清0
// 	for(j=0;j<16;j++){
// 		for(i=0;i<2048;i++){
// 		*p = scr_info.data ? 0xf0800000 : 0xf000;//*p =scr_info.data ? 0 : 0xff00; //如果数据是高有效，则每个点赋值为0；反之，赋值为1
// 		*p=0x80; 
// 		}  
// 		ROW_SCAN(j);//输入行选
// //		HL_LC();      //拉低LC电平
// 			i=10;
// 			GPIOA->BSRR=0x100;
// 			while(i--);
// 			GPIOA->BRR=0x100;
// 		}
// 	ROW_SCAN(0);//关行选
// }

void clr_screen()
{
	ledplay_denglongHei(0x00);					//clr_screen
}

 
void clear_disp_ram()
{
		memset(leddata, 0x00, scr_info.high*screen_len);
}
/**************************************************************************
** 函数名称 ： init_t4
** 函数功能 ： 初始化定时器4，用于调节亮度
** 入口参数 ： 定时器的值b
** 返回参数 ： 无
** 函数说明 ： 根据不同的屏设置不同的亮度值
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void init_t4(unsigned char b)
{
	u32 value;
	value=(b/16)*var;
	if((scr_info.scan==0) || (scr_info.scan==3) || (scr_info.scan==7)||(scr_info.scan==8))
	{
		value=(value*((5)));  		
	}
	else if(scr_info.scan==1)
	{
		if((OFFSET_GREEN == 64) || (OFFSET_GREEN == 48))
		{
			value=(value*((20)));
		}
		else if((OFFSET_GREEN == 32) || (OFFSET_GREEN == 16))
		{
 			 value=(value*((22)));
		}
 	}
	else if((scr_info.scan==2)||(scr_info.scan==6))
	{
 		value=(value*((10)));
	}
	else if((scr_info.scan==4) || (scr_info.scan==5))
	{
 		value=(value*((18)));
	}	
	
	timer_init4(value/16);
}

/**************************************************************************
** 函数名称 ： defaultplay
** 函数功能 ： 屛参初始化
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 2014.5.26 wwd
**************************************************************************/
void defaultplay(){}

/**************************************************************************
** 函数名称 ： screen_init
** 函数功能 ： 屛参初始化
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 2013.3.18 朱振威
**************************************************************************/

void screen_init(void)		
{
   unsigned short  S_64,S_48,S_32,D_64,D_48,D_32;//单双色不同高度对应的宽度

	unsigned short wVirHigh = 0;

	led_play=defaultplay;

	 S_32 = MaxWidth/16;       //32高  双色 最大屏宽
	
   S_48 = MaxWidth*32/(48*16);  //48高  双色  最大屏宽
	
   S_64 = MaxWidth*32/(64*16); //64高  双色 最大屏宽
	
   D_32 = MaxWidth/8;    //32高 单色 最大屏宽
	
   D_48 = MaxWidth*32/(48*8);   //48高  单色  最大屏宽
	
   D_64 = MaxWidth*32/(64*8);    //64高  单色 最大屏宽
	
	/*
if ((flashcheckerr==2))
		{

			memset(buf,0,10);   //清空buf缓冲区的前10个字节

		scr_info.addr = 0x11;
		scr_info.width=0x02;
		scr_info.width<<=8;
		scr_info.width|=0;	 //屏宽
		scr_info.high=0;
		scr_info.high<<=8;
		scr_info.high|=0x20;	 //屏高
		scr_info.dColor=0;	 //单双色
		scr_info.OE=1;		 //oe 极性
		scr_info.data=0;	 //数据极性
		scr_info.scan=1;	 //扫描方式
		scr_info.hsel = 1;	 //行顺序
			if ( MaxWidth < 512 )//15.3.27
			{
					scr_info.width = MaxWidth;
			}
		}
		else if(flashcheckerr==1) //当校验错误读出屏参
		{
         read_buf(10,buf,7);  //读取10字节屏信息放到buf
        scr_info.addr = 0x11;
		scr_info.width=0x02;
		scr_info.width<<=8;
		scr_info.width|=0;	 //屏宽
		scr_info.high=0;
		scr_info.high<<=8;
		scr_info.high|=0x20;	 //屏高
		if ( MaxWidth < 512 )
		{
				scr_info.width = MaxWidth;
		}	


		 if(buf[5]>2)
        {
		 scr_info.dColor=0;	 //单双色
        }
		else
		{
        scr_info.dColor=buf[5];	 //单双色
		}

		 if(buf[6]>1)
        {
		scr_info.OE=1;		 //oe 极性
        }
		else
		{
        scr_info.OE=buf[6];	 //单双色
		}
		 if(buf[7]>1)
        {
		scr_info.data=0;		 //oe 极性
        }
		else
		{
        scr_info.data=buf[7];	 //单双色
		}
		
		 if(buf[8]>9)
        {
	//	scr_info.scan=1;		 //oe 极性
					
	
		scr_info.dColor=0;	 //单双色
		scr_info.OE=1;		 //oe 极性
		scr_info.data=0;	 //数据极性
		scr_info.scan=1;	 //扫描方式
		scr_info.hsel = 1;	 //行顺序
        }
		else
		{
        scr_info.scan=buf[8];	 //单双色
		}

		 if(buf[9]>16)				//正顺序
     {
				scr_info.hsel=1;		 
			}
		else
		{
        scr_info.hsel=buf[9];	 //反向顺序
		}
 
		}
		else					*/														//正常数据
		{
//			flash_addr=0x07;	//屏信息起始地址
			read_buf(10,buf,7);  //读取10字节屏信息放到buf

		scr_info.addr = buf[0];
// 		scr_info.width=buf[1];
// 		scr_info.width<<=8;
// 		scr_info.width|=buf[2];	 //屏宽
		scr_info.width=104;
// 		scr_info.high=buf[3];
// 		scr_info.high<<=8;
// 		scr_info.high|=buf[4];	 //屏高
		scr_info.high=32;	
		//scr_info.dColor=buf[5];	 //单双色
		scr_info.dColor=0;	 //单色	
		scr_info.OE=buf[6];		 //oe 极性
		scr_info.data=buf[7];	 //数据极性
		scr_info.scan=buf[8];	 //扫描方式
		scr_info.hsel = buf[9];	 //行顺序
		
		}
	

	
	if(scr_info.dColor==1){//双色
		offset=2;//此变量在数据处理时会用到，单色和双色的数据偏移不一样
	}else if(scr_info.dColor==0){  //单色
		offset=1;
	}

	#ifdef CONFIG_SEVEN_COLOR	 //七色
		//offset=3;
	#endif
	
	if(scr_info.width>MaxWidth)	 //限制长高，防止下标越界发生硬件异常
	{
		screeniniterr=2;
		scr_info.width=MaxWidth;
		all_scall_face.all_scall_color=0;//双色
		all_scall_face.all_scall_screen_high=1; //屏高 有几块屏	
	}
	if(scr_info.high>MaxHigh)    //高度限制
	{
		screeniniterr=2;
		scr_info.high=MaxHigh;
		all_scall_face.all_scall_color=0;//双色
		all_scall_face.all_scall_screen_high=1; //屏高 有几块屏	
	}
	//45扫描高度减半
	if(((scr_info.scan ==4 )||(scr_info.scan ==5 )) && (scr_info.high > MaxHigh/2))
	{
		screeniniterr=2;
		scr_info.high = MaxHigh/2;
	}
 


	if (scr_info.width < 32)
	{
		screeniniterr=2;
		scr_info.width = 32; 
		all_scall_face.all_scall_color=0;//双色
		all_scall_face.all_scall_screen_high=1; //屏高 有几块屏
	}
	if (scr_info.high < 8)
	{
		screeniniterr=2;
		scr_info.high = 8; 
		all_scall_face.all_scall_color=0;//双色
		all_scall_face.all_scall_screen_high=1; //屏高 有几块屏
	}
/*----------------------------------------------------------*/		
	
	flash_addr=scr_info.addr;  
//  screen_len=scr_info.width/8; //屏长字节数
	screen_len=13; //屏长字节数
	offset_green=scr_info.high;  //绿色数据的高度
	pro_num=flash_read(flash_addr);//读取节目总数
 	pro_c=0;

	scr_onoff = 0;
// 		scr_info.of = 0;
// 		flash_read_write(1);
// 		if(sim_buf_red[0x38] == 0xa0)  scr_info.of=0x01;		//关机
// 		if((*(unsigned char*)0x0800FC38) == 0xa0)
// 		{
// 			scr_info.of = 1;
// 			flash_read_write(1);    
// 			sim_buf_red[0x38] = 0xff;
// 			flash_read_write(0);					
// 		}	
		
	bright=*(u8*)0x0800fC29;			//读取亮度		
	scr_info.s_t=0;								//定长播放标志
	
	memset(leddata,0,8*1024);
	scan_play=0;
	led_play();

	scr_info.scan=0;
	scr_info.OE=0;
 	led_play=defaultplay;
		var=screen_len;
		timer_init(22 * var);
		init_t4(bright);	
}  	  

/**************************************************************************
** 函数名称 ： read_buf
** 函数功能 ： 读取flash中的数据，并将读出的数据放在缓冲区p中
** 入口参数 ： 读取的个数n，数据存放的缓冲区p，读取数据的地址addr
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void read_buf(unsigned char n,unsigned char *p,unsigned long addr)
{
  unsigned char  i=0,d;
  d=flash_read(addr);
  while(n--)
  {
   p[i]=d;spisd;   
   d=SPI1->DR;	
   i++;
  }
}

/*************************************************************************
局部变量：
**************************************************************************/
unsigned char __didi;//此变量在本函数和data_deal函数中使用

/**************************************************************************
** 函数名称 ： program_deal
** 函数功能 ： 节目信息初始化
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void program_deal()
{
	  unsigned char i,save; 
 	  scr_info.sec=1;
	  sec_flag=0xff;
	  hb.flag=0;         //花边标志
	  scr_info.text = 0;
	  scr_info.is_init_hb=1;
	  scr_info.s_t_end=0; //标志清零
	  scr_info.st=0;
	  memset(leddata,0,8*1024);	
	 pro_str:
	 IWDG_Feed();//喂狗
	  flash_addr = scr_info.addr+1+pro_c*4;	//节目n地址
	  read_buf(4,buf,flash_addr);			//读4个字节到read_buf
	  pro_addr=(u32)buf[1];   //节目的存储地址 
	  pro_addr<<=8;
	  pro_addr|=(u32)buf[2];
	  pro_addr<<=8;
	  pro_addr|=(u32)buf[3];
	  	 
	  area_num=flash_read(pro_addr);  //分区总数
	  __didi = ~(0xff<<area_num);

		for(i=0;i<area_num;i++){	  
			area[i].temp=21;
		}

	  pro_flag=flash_read(pro_addr+1);	//节目标志 
	  //低2位  00顺序播放 01定时播放 10定长播放  第2位 1有花边0无花边  第3位 1有背景动画0无背景动画 第4位 1背景动画0背景图片
  	  flash_addr=pro_addr+area_num*4+10;
	  save=flash_read(flash_addr);     //预留字节数
	  flash_addr=flash_addr+save+1;	  
	  if((pro_flag&0x03)==0x01){        //定时播放
	  	 if(show_timers())
             goto pro_sw;  	
	  }
	  else if((pro_flag&0x03)==0x02){	//定长播放
	  	 scr_info.s_t=1; 
		 show_timer=flash_read(flash_addr);
		 show_timer <<= 8; spisd;
		 show_timer |= SPI1->DR;   //定长播放的长度
	  }
	  if(pro_flag&0x04){  //花边处理
		  //hb.flag=1;      //花边标志 					软件没处理好乱码之前禁止花边功能
			flash_addr=pro_addr+2+4*area_num; //花边地址存储地址
			read_buf(4,buf,flash_addr);
			flash_addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
			read_buf(7,buf,flash_addr);	 
			hb.style = buf[0];//花边样式
			hb.color = buf[1];//花边颜色
			hb.width = buf[2];//花边长度
			hb.width /=8;     //花边长度(字节数)
			hb.high = buf[3]; //花边高度
			//hb.high = 0; //花边高度
			hb.move_way = buf[4];	//旋转方向

			if(hb.move_way==3){
				hb.move_speed=buf[5]*12; //移动速度
			}
			else hb.move_speed = buf[5]*4;	//旋转速度
			hb.c_speed=7; 			//当前速度	 
			hb.move_step = buf[6];  //移动步长
			flash_addr+=7;      //花边点阵地址
			hb.addr=flash_addr;
	  }
	  area_deal();	//分区处理					
	  return ;
	pro_sw:
		IWDG_Feed();//喂狗
		pro_c++;      //节目自加，显示下一条节目
		pro_c %= pro_num;
		goto pro_str; 	
}

/**************************************************************************
** 函数名称 ： info_init
** 函数功能 ： 信息初始化
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void info_init(struct AreaStruct *pAr)
{
	 unsigned long addr;
	 if((pAr->style>1)&&pAr->st)return;
	 pAr->sj=0;
	 pAr->sj_cnt=0;
	 pAr->cnt=0;

	 flash_addr=pAr->addr+13+4*pAr->info_c;  //信息属性地址存放的地址 
	 read_buf(4,buf,flash_addr);
	 addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
	 read_buf(13,buf,addr);
	 pAr->info_mode = buf[0];  //运行方式							 

	 pAr->info_speed=buf[1];   //运行速度
	
	 /* 上下移动太快，如果为上下移动速度值就调大点 2013.3.20ZZW */
	 if ( (pAr->info_mode == 17)
		 || (pAr->info_mode == 18) 
	   || (pAr->info_mode == 13)
	   || (pAr->info_mode == 14) )
		{
			pAr->info_speed *= 2;
		}
   /***************************end*******************************/	
	
 	 pAr->info_c_stand=pAr->info_standtime=buf[2]; //停留时间
	 
	 pAr->info_data_w=(unsigned short)buf[5]<<8 | (unsigned short)buf[6]; //信息长度
	 pAr->info_data_h=(unsigned short)buf[7]<<8 | (unsigned short)buf[8];	//信息宽度
	 pAr->info_content_addr =(unsigned long)buf[10]<<16 |(unsigned long)buf[11]<<8 | (unsigned long)buf[12];//内容存放地址

	 pAr->info_end=0;      //信息结束标志
	 pAr->info_data_w /=8; //信息长度字节数
	
	 if(pAr->info_standtime==0)
     {
         pAr->st_end=1;//停留时间为0，则说明停留时间到
     }

	 if(pAr->info_mode==2){ //快速打出
	 	pAr->sj=1;
	 	pAr->info_mode=pAr->temp;
	  }		
}

/**************************************************************************
** 函数名称 ： area_deal
** 函数功能 ： 分区处理
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void area_deal()
{
 	 struct AreaStruct *pAr;
	 flash_addr=pro_addr; //分区属性地址
 	 area_num=flash_read(flash_addr);  //分区个数 
	 for(area_c=0;area_c<area_num;area_c++){//分区处理
		  pAr=&area[area_c]; //当前分区的属性地址
 		  flash_addr=pro_addr+2+4*area_c; //分区属性地址存放的地址
		  pAr->addr = flash_read(flash_addr);
		  pAr->addr <<= 8; spisd;
		  pAr->addr |= SPI1->DR; 
		  pAr->addr <<= 8; spisd;
		  pAr->addr |= SPI1->DR;
		  pAr->addr <<= 8; spisd;
		  pAr->addr |= SPI1->DR; 
		
		  flash_addr = pAr->addr;  //分区属性地址
		  read_buf(13,buf,pAr->addr);
		
		  pAr->style = buf[0];  //信息类别  
		  pAr->screen_w = ((unsigned short)buf[1]<<8 | (unsigned short)buf[2]);//显示框尺寸  
		  pAr->screen_h = ((unsigned short)buf[3]<<8 | (unsigned short)buf[4]);
		  pAr->place_w = ((unsigned short)buf[5]<<8 | (unsigned short)buf[6]); //显示位置	  
		  pAr->place_h = ((unsigned short)buf[7]<<8 | (unsigned short)buf[8]);
		  pAr->info_number = buf[9];  //信息个数 
 		  pAr->area_hb_flag = buf[10];//分区花边
		  pAr->area_back = buf[11];    //分区背景
		  pAr->info_c=0;  //分区信息计数清0 
		  pAr->st=0;      //正在停留标志清0   
		  pAr->st_end=0;  //停留结束标志清0
		  pAr->end=0;     //分区结束标志清0
		 
		  pAr->speed_hb_c=10; //分区花边速度为固定值10
		  pAr->is_init_sim=1; //模拟时钟初始化标记
		  pAr->is_init_dig=1; //数字时钟初始化标记
		  pAr->is_init_count=1;//计时初始化标记
		  pAr->is_init_datn=1;//农历初始化标记
		  pAr->place_w=pAr->place_w/8;
		  pAr->place_w=scr_info.width/8-pAr->place_w; //显示位置的横坐标
		  pAr->screen_w=pAr->screen_w/8;//显示的长度(字节数)

		  pAr->parameter = 0;
		
		  dis_length=scr_info.width/8;//节目的长度(字节数)
		  info_init(pAr);//信息初始化
	  }
}

/**************************************************************************
** 函数名称 ： write_hsel
** 函数功能 ： 写一行数据到显存
** 入口参数 ： 行数r1，
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void write_hsel(unsigned char r1,unsigned long addr)
{
	unsigned short i;
	register unsigned short len;
	unsigned char place_w=pArg->place_w;  			   
	unsigned short screen_w=pArg->screen_w;

	//红色数据
	flash_read(flash_addr);
	len=r1*SCREEN_LEN;  //已经读出的点阵的字数 
	for(i=place_w;i<place_w+screen_w;i++){ 
		leddata[i+len]=srs();		 //将节目的点阵数据存入缓存中 			  
	}
	GREEN_BEGIN    //绿色数据 
		r1+=OFFSET_GREEN;
		len=r1*SCREEN_LEN;
		flash_read(addr);	
		for(i=place_w;i<place_w+screen_w;i++){ 
			leddata[i+len]=srs();
		}
	GREEN_END 
	#ifdef CONFIG_SEVEN_COLOR //蓝色数据
		r1+=OFFSET_GREEN;
		len=r1*SCREEN_LEN;
		flash_read(flash_addr+BLUE_DATA);
		for(i=place_w;i<place_w+screen_w;i++){
			leddata[i+len]=srs();
		}
	#endif
}

/**************************************************************************
** 函数名称 ： getbit
** 函数功能 ： 数据某一位的值
** 入口参数 ： 第n位
** 返回参数 ： 如果n小于8，则返回此位为1时的数；反之，返回0
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
unsigned char getbit(unsigned char n)
{
	 switch(n){
		  case(0):return 0x80;
		  case(1):return 0x40;
		  case(2):return 0x20;
		  case(3):return 0x10;
		  case(4):return 0x08;
		  case(5):return 0x04;
		  case(6):return 0x02;
		  case(7):return 0x01;
	 }
	 return 0;	
}

/**************************************************************************
** 函数名称 ： clr_buf
** 函数功能 ： 清缓冲区
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void clr_buf()
{
	unsigned short i;
	unsigned char j,place_h,screen_h,place_w;
	unsigned short temp,screen_w,len=SCREEN_LEN,size_green=len*OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned short size_blue=len*OFFSET_BLUE;
	#endif
	place_h=pArg->place_h;  //节目的起始纵坐标
	screen_h=pArg->screen_h;//节目的高
	screen_w=pArg->screen_w;//节目的长
	place_w=pArg->place_w;  //节目的起始横坐标
	for(j=place_h;j<screen_h+place_h;j++){
		temp=j*len;
		for(i=place_w;i<place_w+screen_w;i++){
			leddata[i+temp]=0;
			GREEN_BEGIN
				leddata[i+temp+size_green]=0;
			GREEN_END
			#ifdef CONFIG_SEVEN_COLOR
				leddata[i+temp+size_blue]=0;
			#endif
		}
	}
}

/**************************************************************************
** 函数名称 ： still_display
** 函数功能 ： 静止显示
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void still_display()
{ 
	unsigned short i;
	unsigned char j,place_h=pArg->place_h,screen_h=pArg->screen_h,place_w=pArg->place_w;
		   		  
	unsigned short temp,screen_w=pArg->screen_w,len=SCREEN_LEN,size_green=len*OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned short size_blue=len*OFFSET_BLUE;
	#endif
			
	flash_addr=pArg->info_content_addr;	 //信息内容地址							
	flash_read(flash_addr);
	for(j=place_h;j<screen_h+place_h;j++){ //将红色数据写入leddata中
		temp=j*len;
		for(i=place_w;i<place_w+screen_w;i++){ 
			leddata[i+temp]=srs(); 
		} 
	}	
	GREEN_BEGIN  //将绿色数据写入leddata中
		flash_addr = pArg->info_content_addr+GREEN_DATA;
		flash_read(flash_addr);
		for(j=place_h;j<screen_h+place_h;j++){
			temp=j*len+size_green;
			for(i=place_w;i<place_w+screen_w;i++){
				leddata[i+temp]=srs(); 
			} 
		}
	GREEN_END 
	#ifdef CONFIG_SEVEN_COLOR //将蓝色数据写入leddata中
		flash_addr = pArg->info_content_addr+BLUE_DATA;
		flash_read(flash_addr);
		for(j=place_h;j<screen_h+place_h;j++){
			temp=j*len+size_blue;
			for(i=place_w;i<place_w+screen_w;i++){
				leddata[i+temp]=srs();
			}
		}
	#endif
	pArg->cnt++;	
	pArg->st=1;  //当前分区正在停留标志
	if(pArg->cnt==pArg->info_data_h){ //显示的行等于节目的高
		 pArg->cnt=0;
		 pArg->st=1;
		 pArg->info_end=1;//当前节目显示完标志
	 	 return;
	}
	 pArg->info_content_addr+=screen_w*screen_h*offset;	//信息地址指向下个信息的地址 	 	 
}

/**************************************************************************
** 函数名称 ： pull_up_down_screen
** 函数功能 ： 上，下拉幕/上下镭射	
** 入口参数 ： 拉幕的方向ch，拉幕和镭射的标志ls
** 返回参数 ： 无
** 函数说明 ： ch=1向上拉幕，反之向下拉幕；ls=1为镭射，反之为拉幕
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void pull_up_down_screen(unsigned char ch,unsigned char ls)	//ch=1向上拉幕
{
	unsigned char h_sel,j;
	unsigned long hsel_addr;
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h;
	unsigned short screen_w=pArg->screen_w;   
	unsigned char param=pArg->parameter;   		   
	if(param==screen_h){//如果操作次数为节目的高度，则说明一屏数据显示完
		pArg->cnt++; //屏数自加1
		pArg->st=1;  //节目正在停留标志
		pArg->parameter=0;//屏参数为0
		pArg->info_content_addr+=screen_w*screen_h*offset;
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){	  
			pArg->cnt=0;
			pArg->info_end=1;
		}
		return; 
	}
	if(param==0)clr_buf(); // 一屏的数据开始显示前，清除缓冲区   
	if(ch)h_sel=place_h+screen_h-param-1;//从最后一行起向上拉幕
	else h_sel=param+place_h; 	//从第一行起向下拉幕
	flash_addr=pArg->info_content_addr+screen_w*(h_sel-place_h); //红色数据存放的首地址
	hsel_addr = pArg->info_content_addr+screen_w*(h_sel-place_h)+GREEN_DATA; //绿色数据存放的首地址 
	if(ch){ //向上
		if(ls)//镭射
			for(j=h_sel;j>place_h;j--)
				write_hsel(j,hsel_addr); //将点阵数据写入缓冲区
		else  //拉幕
			write_hsel(h_sel,hsel_addr);
	}else{//向下
		if(ls)//镭射
			for(j=h_sel;j<screen_h+place_h;j++)
				write_hsel(j,hsel_addr); 
		else //拉幕
			write_hsel(h_sel,hsel_addr);
	} 
	pArg->parameter++;
}

/**************************************************************************
** 函数名称 ： clear
** 函数功能 ： 清除缓冲区的某一列
** 入口参数 ： 第len个字节
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void clear(unsigned short len)
{
	unsigned char j;
  	for(j=pArg->place_h;j<pArg->screen_h+pArg->place_h;j++){
	 	leddata[j*SCREEN_LEN+len]=0;
	}
}

/**************************************************************************
** 函数名称 ： pull_left_right_screen
** 函数功能 ： 向左,向右拉幕	/左,向镭射
** 入口参数 ： 拉幕的方向ch，拉幕和镭射的标志ls
** 返回参数 ： 无
** 函数说明 ： ch=1向左，反之向右；ls=1为镭射，反之为拉幕
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void pull_left_right_screen(unsigned char ch,unsigned char ls) //ch=1向左拉幕
{
	unsigned char r_sel,j,d,place_w,pos=0,_pos=0,bit_l=0,bit_r=0;
	register unsigned short len=SCREEN_LEN;
	register unsigned short temp;
	unsigned char place_h,screen_h;
	unsigned short i,param,size_green,screen_w=pArg->screen_w;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned short size_blue;
	#endif
	place_h=pArg->place_h;
	screen_h=pArg->screen_h; 
	param=pArg->parameter;//拉幕或镭射的操作次数
	if(ls){//镭射
		if((pArg->area_hb_flag&0x01) || hb.flag){//花边
			if(pArg->place_w==0){//如果起始的位置为屏幕的左边
				bit_l=1;
				if(param==0)
					clear(0);//清除第0个字节的一列
			}
			if(len==pArg->place_w+screen_w){//如果起始位置为屏幕的右边
				bit_r=1;
				if(param==0)
					clear(SCREEN_LEN-1);//清除最后一个字节所在的列
			}
		}
	}
	if(param==pArg->screen_w*8){//如果拉幕或者镭射的点够一屏
		pArg->cnt++;	    //屏数自加
		pArg->parameter=0;//操作的次数清0		
		pArg->st=1;       //节目正在停留标志
		
			pArg->info_content_addr+=screen_w*screen_h*offset;//节目点阵存放的首地址
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//如果屏数为数据的屏数，并且非随机播放
			pArg->cnt=0;     //将屏数清0
			pArg->info_end=1;//节目结束
		}
		return;  //返回
	}
	if(!pArg->sj){//如果非随机播放
		if(param==0){clr_buf();}//如果拉幕的第0列，则清除buf内容
	}	 
	if(ch){	//向左
		place_w=pArg->place_w+screen_w-1-param/8; //拉幕或镭射的起始位置为最右端
		flash_addr=pArg->info_content_addr+screen_w-1-param/8; //节目的起始地址 
		r_sel=getbit(7-param%8);//每个字节从左向右显示
		if(ls){//镭射
			pos=place_w;//起始位置
			_pos=pArg->place_w+bit_l;//结束位置为屏幕位置的起始位置的下一个字节的位置
		}
	}else{//向右
		place_w=pArg->place_w+param/8;
		flash_addr=pArg->info_content_addr+param/8;//节目的起始地址
		r_sel=getbit(param%8);//每个字节从右向左显示
		if(ls){ //镭射
			pos=pArg->place_w+screen_w-bit_r;	 //起始位置为节目显示的最后位置的下一个字节的位置
			_pos=place_w+1;//结束位置为节目结束位置的前一个字节的位
		}
	} 
	size_green=len*OFFSET_GREEN;//绿色数据的个数
	#ifdef CONFIG_SEVEN_COLOR
		size_blue=len*OFFSET_BLUE;//蓝色数据的个数
	#endif
	for(j=place_h;j<screen_h+place_h;j++){
		temp=j*len;
		//红色数据
		d=flash_read(flash_addr);
		leddata[temp+place_w] &= ~r_sel;
		leddata[temp+place_w] |= d&r_sel;
		if(ls){//镭射
			for(i=_pos;i<pos;i++)
				leddata[temp+i] = ((d&r_sel)?0xff:0); //显示的哪一位的数为1时，赋值为0xFF，反之赋值为0    
		}
		GREEN_BEGIN //绿色数据
			 d=flash_read(flash_addr+GREEN_DATA);
			 leddata[temp+size_green+place_w] &= ~r_sel;
			 leddata[temp+size_green+place_w] |= d&r_sel;
			 if(ls){//镭射
				 for(i=_pos;i<pos;i++)
					leddata[temp+size_green+i] = ((d&r_sel)?0xff:0);	 
			 }
		GREEN_END 
		
		#ifdef CONFIG_SEVEN_COLOR //蓝色数据
			d=flash_read(flash_addr+BLUE_DATA);
			leddata[temp+size_blue+place_w] &= ~r_sel;
			leddata[temp+size_blue+place_w] |= d&r_sel;
			if(ls){
				for(i=place_w+1;i<place_w+screen_w-pos;i++)
					leddata[temp+size_blue+i] = ((d&r_sel)?0xff:0);  
			}
		#endif	
		flash_addr+=pArg->screen_w; //赋值之后，地址加上屏宽，之后读下一行的数据
	} 
	pArg->parameter++;//每次显示之后，拉幕或者镭射的次数加一
}

/**************************************************************************
** 函数名称 ： closed_down_screen
** 函数功能 ： 上下闭合
** 入口参数 ： 闭合的方向ch
** 返回参数 ： 无
** 函数说明 ： ch=1上下闭合，其中r2为1为上半屏、反之为下半屏
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void closed_down_screen(unsigned char ch)  //ch=1上下闭合
{
	unsigned char r2=1,place_h=pArg->place_h,screen_h=pArg->screen_h;
	unsigned short r1,screen_w=pArg->screen_w,param=pArg->parameter;
	unsigned long hsel_addr;
	if(param==pArg->screen_h){//闭合操作的次数为屏高
		pArg->cnt++;	//屏数自加1
		pArg->st=1;	  //停留标志置1 
		pArg->parameter=0;//每屏显示完操作次数清0
		pArg->info_content_addr+=screen_w*screen_h*offset;//节目存放的地址为下一屏节目存放的首地址
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//如果屏数为数据的屏数，并且非随机播放
			pArg->cnt=0;      //屏数清0
			pArg->info_end=1; //节目结束
		}
		return; //返回
	}
	if(param==(ch?0:screen_h/2))clr_buf(); //上下闭合时，第0次操作时，清缓冲区； 
	//补充：ch=1，为上下闭合，此时每屏需要操作的次数为屏高的一半 
	 
	
	display:		
	if(r2)r1=place_h+screen_h-param-1;//下半屏的尾行的位置
	else r1=param+place_h;//上半屏的首行位置
	flash_addr=pArg->info_content_addr+screen_w*(r1-place_h);//红色数据的起始位置
	hsel_addr=flash_addr+GREEN_DATA;//绿色数据的起始位置
	write_hsel(r1,hsel_addr);	//第r1行写数据
	
	if(r2){//如果下半屏显示完之后，将r2清0，显示上半屏
		r2=0;goto display;
	}
	pArg->parameter++;  //每次显示将显示次数加1 
}

/**************************************************************************
** 函数名称 ： down_on_screen
** 函数功能 ： 上下对开
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void down_on_screen() 
{
	if(pArg->parameter==0)pArg->parameter=pArg->screen_h/2;			
	closed_down_screen(0);
}

/**************************************************************************
** 函数名称 ： left_right_close
** 函数功能 ： 左右闭合
** 入口参数 ： 闭合的标志ch
** 返回参数 ： 无
** 函数说明 ： ch=0，左右闭合；ch=1，左右对开
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void left_right_close(unsigned char ch)	//ch=1 左右对开	
{
	unsigned char r_sel,d,r2=1;
 	register unsigned short len=SCREEN_LEN;
	register unsigned short size_green,temp;
	#ifdef CONFIG_SEVEN_COLOR
		register unsigned short size_blue;
	#endif
	unsigned short j,screen_w=pArg->screen_w*8; 
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h,
				  place_w=pArg->place_w;
	unsigned short DATA=pArg->screen_w*screen_h; //每种颜色的数据的个数					  	

	if(pArg->parameter==(ch?screen_w:screen_w/2)){ //ch=0，左右闭合，参数等于screen_w/2时，说明操作了一屏数据 
		//补充：ch=1，左右对开，此时需要操作的起始值为screen_w/2，终止值为screen_w  	
	  pArg->cnt++;        //屏数加1
		pArg->parameter = 0;//屏参清0	 	
		pArg->st=1;         //停留标志置1
		
 		pArg->info_content_addr+=pArg->screen_w*screen_h*offset; //节目的点阵存放地址指向下一条节目的首地址
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//如果节目的屏数为数据的屏数，并且非随机播放
			pArg->cnt=0;     //将屏数清0
			pArg->info_end=1;//节目结束标志置位   
		}
		return;//返回
	}
	if(pArg->parameter==(ch?screen_w/2:0)){clr_buf();}//ch=1,为左右对开，开始时位置为中间，此时清楚缓存
	                                                   //ch=0,为左右闭合，起始位置为0行，此时清除缓存
	
	display:
	if(r2){	//r2=1为右半屏的数据
		place_w=pArg->place_w+pArg->screen_w-pArg->parameter/8-1;//右半屏的右边的位置											 
		flash_addr=pArg->info_content_addr+pArg->screen_w-1-pArg->parameter/8;
		r_sel=getbit(7-pArg->parameter%8);	//每次操作的位的数赋给r_sel，从低到高位
	}else{
		place_w=pArg->place_w+pArg->parameter/8;//左半屏的左边的位置
		flash_addr=pArg->info_content_addr+pArg->parameter/8;
		r_sel=getbit(pArg->parameter%8);//每次操作的位的数赋给r_sel，从高到低位
	}
	size_green=OFFSET_GREEN*len+place_w;
	#ifdef CONFIG_SEVEN_COLOR
		size_blue=OFFSET_BLUE*len+place_w;
	#endif
	for(j=place_h;j<screen_h+place_h;j++){
		temp=j*len;
		//红色数据
		d=flash_read(flash_addr);
		leddata[place_w+temp] |= d&r_sel;
		GREEN_BEGIN//绿色数据
			d=flash_read(flash_addr+DATA);
			leddata[temp+size_green] |= d&r_sel;
		GREEN_END
		#ifdef CONFIG_SEVEN_COLOR //蓝色数据
			d=flash_read(flash_addr+2*DATA);
			leddata[temp+size_blue] |= d&r_sel;
		#endif
		flash_addr+=pArg->screen_w; 	 
	} 
	if(r2){//右半屏的数据显示完之后，将r2清0，然后显示左半屏的数据
		r2=0; goto display;	
	}
	pArg->parameter++;	 //每次操作之后，参数自加1 	 						
}

/**************************************************************************
** 函数名称 ： left_right_open
** 函数功能 ： 左右闭合
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void left_right_open()
{
	 if(pArg->parameter==0){
		  pArg->parameter = pArg->screen_w*8/2;//左右闭合的起始列为屏的中间
	 }
	 left_right_close(1);
}

/**************************************************************************
** 函数名称 ： V_blinds
** 函数功能 ： 垂直百叶
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void V_blinds()
{
	unsigned char j,d,r1,n=0; 
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h,
				  place_w=pArg->place_w;
	unsigned short r2,i,screen_w=pArg->screen_w;
	register unsigned short len=SCREEN_LEN;
	register unsigned short temp,size_green=len*OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		register unsigned short size_blue=len*OFFSET_BLUE;
	#endif
	
	if(pArg->parameter==8){//参数为8说明一屏信息百叶完
		pArg->cnt++;         //屏数加1
		pArg->parameter=0;   //屏的参数清0
		pArg->st=1;          //停留时间标志
		
 		pArg->info_content_addr+=screen_w*screen_h*offset;//节目的点阵存放地址指向下一条节目的首地址
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){  //如果节目的屏数为数据的屏数，并且非随机播放
			pArg->cnt=0;      //屏数清0
			pArg->info_end=1; //节目结束标志
		}
		return; //返回
	}
	if(pArg->parameter==0){ //如果屏参为0
		clr_buf(); //将缓冲区清0
	}
	r2=screen_w;
	while(r2--){  //垂直百叶的叶数
		i=place_w+n;//垂直百叶开始的位置，n是百叶的次数			
		flash_addr=pArg->info_content_addr+n;//显示的数据的点阵的首地址
		n++; n%=screen_w;//如果显示的字数超过一屏，则分屏百叶
		r1=getbit(pArg->parameter%8);//此次显示的那一位
		for(j=place_h;j<screen_h+place_h;j++){//显示起始的坐标开始
			temp=j*len;
			//红色数据
			d=flash_read(flash_addr);//读出点阵信息
			leddata[i+temp] |= d&r1; //显示点阵的某一位
			
			GREEN_BEGIN  //绿色数据
				d=flash_read(flash_addr+GREEN_DATA);
				leddata[i+temp+size_green] |= d&r1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				d=flash_read(flash_addr+BLUE_DATA);
				leddata[i+temp+size_blue] |= d&r1;
			#endif
			flash_addr+=screen_w;  //每次百叶操作是一行
		}
	}
	pArg->parameter++;  //屏的参数自加1
}

/**************************************************************************
** 函数名称 ： H_blinds
** 函数功能 ： 水平百叶
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
void H_blinds()//水平百叶
{
	unsigned char j;
 	unsigned long hsel_addr;
	unsigned short screen_w=pArg->screen_w;
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h;
	unsigned char param=pArg->parameter;//屏的参数
	if(param==8){ //屏的参数为8时说明一屏数据百叶完成
		pArg->cnt++;//每显示完一屏数据，显示的屏数自加1
		pArg->st=1; //节目停留时间
		pArg->parameter=0;//每显示完一屏的数据，屏的参数清0
 
		pArg->info_content_addr+=screen_w*screen_h*offset;//节目的点阵存放地址
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//如果节目的屏数为数据的屏数，并且非随机播放
			pArg->cnt=0;     //屏数清0
			pArg->info_end=1;//节目结束标志
		}
		return; //返回
	}
	if(param==0){clr_buf();}   //每次百叶前，将缓冲区清0
	for(j=place_h+param;;j+=8){
		if(j>place_h+screen_h-1)break;//如果行号大于最后一行，则跳出for循环
	
		flash_addr=pArg->info_content_addr+screen_w*(j-place_h);//红色数据的地址
		hsel_addr=flash_addr+GREEN_DATA;//绿色数据的地址
		write_hsel(j,hsel_addr);//将第j行数据写入缓存
	}
	pArg->parameter++;	//屏的参数自加1	
}

/**************************************************************************
** 函数名称 ： up_move
** 函数功能 ： 向上移动
** 入口参数 ： 连移的标志r2
** 返回参数 ： 无
** 函数说明 ： r2=0 连移，反之 有停顿
** 修改日志 ： 2014年04月11日修改	ZZW	
**************************************************************************/
void up_move(unsigned char r2)//向上移动	 r2=0 连移
{
	unsigned char isHb = 0,isBk = 0,ucStep=0; // ZZW 14.4.11
	unsigned short i; 
	unsigned char j,r1,save=0,__save=0,save1=0,__save1=0;
	unsigned char r,_r,l,_l;
	unsigned long DATA; 
	register unsigned short len=SCREEN_LEN;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned char save2,__save2;
		unsigned short size_blue=len*OFFSET_BLUE;
	#endif
	
 	unsigned long hsel_addr;
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h,place_w=pArg->place_w;			  
	unsigned short data_w,temp,screen_w=pArg->screen_w,param=pArg->parameter,ps_len=screen_w+place_w,size_green=len*OFFSET_GREEN;
	
	if(pArg->sj){//随机播放
		DATA=screen_w*screen_h;//红色点阵的字数
		data_w=screen_w;
	}else{  //非随机播放
	 	DATA=pArg->info_data_w*pArg->info_data_h; //红色点阵的字数
		data_w=pArg->info_data_w;
	}

	r=0x7f;_r=~r;//左侧花边处理时用
	l=0xfe;_l=~l;	 	//右侧花边处理时用
	if(pArg->area_hb_flag&0x01){//有分区花边(即边框)
		ucStep=1;isBk = 1; //ZZW 14.4.11
		place_h=place_h+1;   //起始的位置后移一位
		screen_h=screen_h-2; //屏高少两行，留作花边用
	}
	if(hb.flag){  //有花边
		isHb = 1;
		if(place_w==0){//如果节目显示的起始位置为左侧
			r=0xff>>(hb.high+1);
		    _r=~r;
		}
		if(len==(place_w+screen_w)){//如果屏长为节目末端
			l=0xff<<(hb.high+1);
			_l=~l;
		}
	}		   
	if(r2){   //非连移
		if((param + ucStep) == pArg->screen_h*(pArg->cnt+1)){//一屏的数据移动完
			pArg->cnt++; //屏数自加
			pArg->st=1;  //每屏数据移动完之后停顿一下
			return;
		}	 
	}
	if(pArg->sj){ //随机播放
		if(param==screen_h){//播放到最后一行
			pArg->st=1;         //没屏停顿
			pArg->parameter=0;  //每屏数据移动完将屏参数清0
			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//节目的地址指向下一屏数据的起始地址
 			return;
		}		
	}
	else if((param+ucStep)==pArg->info_data_h){//非随机播放，当移动完所有数据
		pArg->parameter=0;  //将屏参数清0
		pArg->info_end=1;   //节目结束标志
		pArg->st=1;         //停留标志
		pArg->cnt=0;        //屏数清0
		return;
	}


	for(j=place_h;j<screen_h+place_h-1;j++){//显示的节目从起始位置开始，共(screen_h-1)行,每次往缓冲区写一行的数据
	  if ( isBk || isHb )// ZZW 14.4.11
		{	
			//红色数据
			temp=j*len;
			save=leddata[temp+place_w]&_r;    //左侧花边的位置
			__save=leddata[temp+ps_len-1]&=_l;//右侧花边的位置
			
			GREEN_BEGIN //绿色数据
				temp=j*len+size_green;
				save1=leddata[temp+place_w]&_r;
				__save1=leddata[temp+ps_len-1]&=_l;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR  //蓝色数据
				temp=j*len+size_blue;
				save2=leddata[temp+place_w]&_r;
				__save2=leddata[temp+ps_len-1]&=_l;
			#endif
		}	
		
		for(i=place_w;i<ps_len;i++){//每行数据从起始位置开始，长度为ps_len=screen_w+place_w
			//红色数据
			 temp= j*len;
			 leddata[i+temp]=leddata[i+temp+len];//将第j行数据存入缓冲区,屏的最上边一行留作花边用
			 
			 GREEN_BEGIN  //绿色数据
				temp= j*len+size_green;
			 	leddata[i+temp]=leddata[i+temp+len];   
			 GREEN_END
			
			 #ifdef CONFIG_SEVEN_COLOR //蓝色数据
			 	temp=j*len+size_blue;
			 	leddata[i+temp]=leddata[i+temp+len];
			 #endif
		}
			if ( isBk || isHb )// ZZW 14.4.11
			{	
				//红色数据
				temp=j*len+place_w; //数据指向该行的最左边的一个字节
				leddata[temp]&=r;   //清数据的最高位 0x7f;
				leddata[temp]|=save;//数据加上花边
				temp=j*len+ps_len-1;//数据指向该行的最右边的一个字节
				leddata[temp]&=l;   //清数据的最低位 0xfe;
				leddata[temp]|=__save;//数据加上花边
				
				GREEN_BEGIN //绿色数据
					temp=j*len+size_green+place_w;
					leddata[temp]&=r;
					leddata[temp]|=save1;
					temp=j*len+size_green+ps_len-1;
					leddata[temp]&=l;
					leddata[temp]|=__save1;
				GREEN_END
				
				#ifdef CONFIG_SEVEN_COLOR //蓝色数据
					temp=j*len+size_blue+place_w;
					leddata[temp]&=r;
					leddata[temp]|=save2;
					temp=j*len+size_blue+ps_len-1;
					leddata[temp]&=l;
					leddata[temp]|=__save2;
				#endif
		}
	}
	flash_addr=pArg->info_content_addr+param*data_w;//节目的下一行的数据的位置
	hsel_addr=flash_addr+DATA;//绿色数据的位置
	r1=screen_h+place_h-1;    //行号
	write_hsel(r1,hsel_addr); //将该行数据写入缓存
	pArg->parameter++;
}

/**************************************************************************
** 函数名称 ： down_move
** 函数功能 ： 向下移动
** 入口参数 ： 连移的标志r2
** 返回参数 ： 无
** 函数说明 ： r2=0 连移，反之 有停顿
** 修改日志 ： 2014年04月11日修改	ZZW
**************************************************************************/
void down_move(unsigned char r2)   
{
	unsigned char isHb = 0,isBk = 0,ucStep=0; // ZZW 14.4.11
	unsigned char r1,j,save=0,__save=0,save1=0,__save1=0;
	unsigned short i,len=SCREEN_LEN,screen_w=pArg->screen_w,ps_len;
	unsigned char r,_r,l,_l;
	unsigned long DATA;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned char save2,__save2;
		unsigned short size_blue=len*OFFSET_BLUE;
	#endif
	unsigned long hsel_addr;
 	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h,
				  place_w=pArg->place_w;
	unsigned short data_w,data_h,temp,param=pArg->parameter,size_green=OFFSET_GREEN*len;

	if(pArg->sj){  //随机播放
		DATA=screen_w*screen_h; //红色点阵的字数
		data_h=screen_h;//数据的高
		data_w=screen_w;//数据的长
	}else{//非随机播放
		DATA=pArg->info_data_w*pArg->info_data_h;//红色点阵的字数
		data_h=pArg->info_data_h;//数据的高
		data_w=pArg->info_data_w;//数据的长
	}
	r=0x7f;_r=~r; //左侧花边处理时用
	l=0xfe;_l=~l; //右侧花边处理时用
	if(pArg->area_hb_flag&0x01){//有分区花边(即边框)
		ucStep=1;isBk = 1; // ZZW 14.4.11
		place_h=place_h+1;        //起始的位置后移一位
		screen_h=screen_h-2;	    //屏高少两行，留作花边用
	}
	if(hb.flag){ //有花边
		isHb = 1;
		if(place_w==0){  //如果节目显示的起始位置为左侧
			r=0xff>>(hb.high+1);
			_r=~r;
		}
		if(len==(screen_w+place_w)){ //如果屏长为节目末端
			l=0xff<<(hb.high+1);
			_l=~l;
		}			
	}
	ps_len=place_w+screen_w; //节目显示的终点位置
 	if(r2){  //非连移
		if((param + ucStep) == pArg->screen_h*(pArg->cnt+1)){//一屏的数据移动完
			pArg->cnt++;  //屏数自加
			pArg->st=1; 	//每屏数据移动完之后停顿一下
			return;
		}
	}
	if(pArg->sj){ //随机播放
		if(param==screen_h){//播放到最后一行
			pArg->st=1;       //每屏停顿
			pArg->parameter=0;//每屏数据移动完将屏参数清0
 			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//节目的地址指向下一屏数据的起始地址
			return;
		} 	
	}
	else if((param+ucStep)==pArg->info_data_h){   //非随机播放，当移动完所有数据 
		pArg->parameter=0;//将屏参数清0
		pArg->info_end=1; //节目结束标志
		pArg->st=1;       //停留标志
		pArg->cnt=0;      //屏数清0
		return;	
	}


	for(j=place_h+screen_h-1;j>place_h;j--){//显示的节目从最后一行位置开始，共(screen_h-1)行,每次往缓冲区写一行的数据
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//红色数据
			temp=j*len;//第j行的第0个数据
			save=leddata[temp+place_w]&_r;   //左侧花边的位置 _r=0x80;
			__save=leddata[temp+ps_len-1]&_l;//右侧花边的位置 _l=0x01;
			
			GREEN_BEGIN //绿色数据
				temp=j*len+size_green;
				save1=leddata[temp+place_w]&_r;
				__save1=leddata[temp+ps_len-1]&_l;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				temp=j*len+size_blue;
				save2=leddata[temp+place_w]&_r;
				__save2=leddata[temp+ps_len-1]&_l;
			#endif
		}	
		
		for(i=place_w;i<ps_len;i++){//每行数据从起始位置开始，长度为ps_len=screen_w+place_w
			//红色数据
			temp=j*len;
			leddata[i+temp]=leddata[i+(j-1)*len];//将第j行数据存入缓冲区,屏的最下边一行留作花边用
			
			GREEN_BEGIN //绿色数据
			leddata[i+temp+size_green]=leddata[i+(j-1)*len+size_green];
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
			leddata[i+temp+size_blue]=leddata[i+(j-1)*len+size_blue];
			#endif
		}
		if ( isBk || isHb )// ZZW 14.4.11
		{
			//红色数据
			temp=j*len+place_w;//数据指向该行的最左边的一个字节
			leddata[temp]&=r;    //清数据的最高位 r=0x7f
			leddata[temp]|=save; //数据加上花边
			temp=j*len+ps_len-1; //数据指向该行的最右边的一个字节
			leddata[temp]&=l;    //清数据的最低位 l=0xfe
			leddata[temp]|=__save;//数据加上花边
			
			GREEN_BEGIN //绿色数据
				temp=j*len+size_green+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save1;
				temp=j*len+size_green+ps_len-1;
				leddata[temp]&=l;//0xfe;
				leddata[temp]|=__save1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				temp=j*len+size_blue+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save2;
				temp=j*len+size_blue+ps_len-1;
				leddata[temp]&=l;//0xfe;
				leddata[temp]|=__save2;
			#endif
		}	
	}

	flash_addr=pArg->info_content_addr+(data_h-1-param)*data_w; //节目的上一行的数据的位置
	hsel_addr=flash_addr+DATA; //绿色数据的位置 
	r1=place_h;  //行号  
	write_hsel(r1,hsel_addr);	 //将该行数据写入缓存
	pArg->parameter++;
}

/**************************************************************************
** 函数名称 ： left_move
** 函数功能 ： 向左移动
** 入口参数 ： 连移的标志r2
** 返回参数 ： 无
** 函数说明 ： r2=0 连移，反之 有停顿
** 修改日志 ： 2014年04月11日修改	ZZW	
**************************************************************************/
void left_move(unsigned char r2) //r2=0 连移
{
	unsigned char isHb = 0,isBk = 0,ucStep = 0; // ZZW 14.4.11
	unsigned char d,j,r1,save=0,save1=0;
	unsigned char r,_r,l,_l,bit;
	unsigned long DATA;
	unsigned short i,data_w;
	register unsigned char d2,d1;
	register unsigned short len=SCREEN_LEN;	
	#ifdef CONFIG_SEVEN_COLOR
		unsigned char save2;
		register unsigned short size_blue=len*OFFSET_BLUE;
	#endif
 	register unsigned short size_green=len*OFFSET_GREEN;
 	unsigned short temp,param=pArg->parameter;
	unsigned short ps_len,screen_w=pArg->screen_w,place_w=pArg->place_w;  //2.10
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h;
	
	if(pArg->sj){ //随机播放
		DATA=screen_w*screen_h; //红色点阵的字数
		data_w=screen_w;
	}else{ //非随机播放
		DATA=pArg->info_data_w*pArg->info_data_h; //红色点阵的字数
		data_w=pArg->info_data_w;
	}
	r=0x7f;_r=0x80; //左侧花边处理时用
	l=0xfe;_l=0x01; //右侧花边处理时用
	bit=0x01; // ZZW 14.4.11
	if(pArg->area_hb_flag&0x01){ //有分区花边(即边框)
		ucStep = 1; 
		//isBk = 1;
		isBk = 0; 							//=======================================
		//bit = 0x02;// ZZW 14.4.11==========================================
		bit = 0x01;
		place_h=pArg->place_h+1;   //起始的位置后移一位
		//place_h=pArg->place_h; 
		screen_h=pArg->screen_h-2; //屏高少两行，留作花边用
	}

	ps_len=place_w+screen_w;   //数据的最右边的位置
	if(hb.flag){ //有花边
		//isHb = 1; // ZZW 14.4.11
		isHb = 0;
		bit=0x02;
		//bit=0x02; // ZZW 14.4.11================================================================
		if((len==(screen_w+place_w))&&((hb.high+1)<8)){  //屏长等于节目显示的最右边的位置，并且花边的高度小于7
			l=0xff<<(hb.high+1);  //右侧花边处理时用
			//l=0xff<<(hb.high); 
			_l=~l;
			bit=1<<(hb.high+1);			
		}else if((len == (screen_w+place_w))&&(hb.high==7)){//屏长等于节目显示的最右边的位置，并且花边的高度等于7
			ps_len=place_w+screen_w-1;
		}
		if((place_w==0)&&((hb.high+1)<8)){//起始位置为最左边，并且花边的高度小于7
			r=0xff>>(hb.high+1); //左侧花边处理时用
			//r=0xff>>(hb.high); 
			_r=~r;			
		}else if((place_w==0)&&(hb.high==7)){//起始位置为最左边，并且花边的高度等于7
			place_w+=1;//起始位置移动一个字节
		}
	} 

	if(r2){ //非连移
		if((param + ucStep) == screen_w*8*(pArg->cnt+1)){ //一屏的数据移动完
			pArg->cnt++; //屏数自加
			pArg->st=1;	 //每屏数据移动完之后停顿一下
			return;
		}
	}
 
	if(pArg->sj){ //随机播放
		if(param==screen_w*8){//播放到最后一列
			pArg->st=1;          //每屏停顿
			pArg->parameter=0;    //每屏数据移动完将屏参数清0
 			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//节目的地址指向下一屏数据的起始地址
			return;
		}
	}
	else if(param + ucStep == pArg->info_data_w*8){ 	
		 pArg->parameter=0; //将屏参数清0
		 pArg->info_end=1;  //信息结束标志
		 pArg->st = 0;
			/* ADD BY ZZW 2013.5.7 */
		 if ( ((pro_num > 1) && (pArg->info_number > 1)) || (pArg->info_standtime > 0) )
		 {
				pArg->st = 1;
		 } 
		 pArg->info_c_speed = 0; /* ADD BY ZZW 2013.5.3 */
		 pArg->cnt=0;       //屏数清0
		 return;
	}
	for(j=place_h;j<screen_h+place_h;j++){//显示的节目从起始位置开始，共screen_h行,每次往缓冲区写一行的数据
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//红色数据
			temp=j*len; //第j行的第0个数据
			save=leddata[temp+place_w]&_r;//左侧花边的位置 r=0x7f;_r=0x80;

			GREEN_BEGIN	//绿色数据											
				save1=leddata[temp+size_green+place_w]&_r;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				save2=leddata[temp+size_blue+place_w]&_r;
			#endif
		}
		for(i=place_w;i<ps_len-1;i++){//每行数据从起始位置开始，长度为ps_len=screen_w+place_w 的前一个字节
			//红色数据
			temp=i+j*len;
			d1  = leddata[temp]<<1;   //每个数据向左移动一位
			d2 = leddata[temp+1]&0x80;//下一个数据的最高位取出来，补到上一个数据移动之后
			d2>>=7;
			leddata[temp] = d1 | d2;
 
			GREEN_BEGIN //绿色数据
			temp=i+j*len+size_green;
			d1  = leddata[temp]<<1;
			d2 = leddata[temp+1]&0x80;
			d2>>=7;
			leddata[temp] = d1 | d2;
 			GREEN_END	
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				temp=i+j*len+size_blue;
				d1  = leddata[temp]<<1;
				d2 = leddata[temp+1]&0x80;
				d2>>=7;
				leddata[temp] = d1 | d2;
 		 #endif
		}
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//红色数据
			temp=j*len+place_w;   //数据指向该行的最左边的一个字节
			leddata[temp]&=r;     //清数据的最高位 r=0x7f
			leddata[temp]|=save;  //数据加上花边
			
			GREEN_BEGIN //绿色数据
				temp=j*len+size_green+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				temp=j*len+size_blue+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save2;
			#endif
		}	
	}
 
	flash_addr=pArg->info_content_addr+isBk*data_w+param/8; //
	r1=getbit(param%8);	//移动到数据的第几位								 
	for(j=place_h;j<screen_h+place_h;j++){//每行数据从起始位置开始，长度为screen_h
	 //红色数据	
		temp=j*len+ps_len-1;      //第j行的最后一个字节的数
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			save=(leddata[temp]&_l);	//右侧花边的位置的数
			leddata[temp]&=l;         //清数据的最低位 l=0xfe
			leddata[temp] = ((leddata[temp]<<1) | save);//数据加上花边	
		}
		else
		{
			leddata[temp] <<= 1;
		}
		d=flash_read(flash_addr);
		if(d&r1){ //如果移动的那一位数据为1
			leddata[temp]|= bit; 
		}
		
		GREEN_BEGIN //绿色数据
			temp=j*len+size_green+ps_len-1;
			if ( isBk || isHb )// ZZW 14.4.11
			{	
				save=leddata[temp]&_l;
				leddata[temp]&=l;
				leddata[temp]=(leddata[temp]<<1)|save;
			}
			else
      {
				leddata[temp] <<= 1;
			}				
			d=flash_read(flash_addr+DATA);
			//if(d&r1)leddata[temp] |= bit;
		GREEN_END
		
		#ifdef CONFIG_BLUE_COLOR //蓝色数据
			temp=j*len+size_blue+ps_len-1;
			save=leddata[temp]&_l;
			leddata[temp]&=l;
			leddata[temp]=(leddata[temp]<<1)|save;
			d=flash_read(flash_addr+2*DATA);
			if(d&r1)leddata[temp] |= bit;
			//if(d&r1)leddata[temp] |= 0x01;
		#endif	
		flash_addr+=data_w; //每次写缓冲区之后，地址指向下一行数据的起始位置
	}
	pArg->parameter++; 
}

/**************************************************************************
** 函数名称 ： right_move
** 函数功能 ： 向右移动
** 入口参数 ： 连移的标志r2
** 返回参数 ： 无
** 函数说明 ： r2=0 连移，反之 有停顿
** 修改日志 ： 2014年04月11日修改	ZZW	
**************************************************************************/
void right_move(unsigned char r2)	
{
	unsigned char isHb = 0,isBk = 0,ucStep=0; // ZZW 14.4.11
	unsigned char r1,d,j,save=0,save1=0; 
	unsigned char l,_l,r,_r,bit;
	unsigned long DATA;
	unsigned short i,data_w;
	register unsigned char d1,d2;
	register unsigned short len=SCREEN_LEN;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned char save2;
		unsigned short size_blue=len*OFFSET_BLUE;
	#endif
 	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h,place_w=pArg->place_w;			  
	unsigned short  screen_w=pArg->screen_w,ps_len;
	unsigned short temp,size_green=len*OFFSET_GREEN;
	
	if(pArg->sj){ //随机播放
		DATA=screen_w*screen_h;//红色点阵的字数
		data_w=screen_w;
	}else{//非随机播放
		DATA=pArg->info_data_w*pArg->info_data_h;//红色点阵的字数
		data_w=pArg->info_data_w;
	}
	l=0xfe;_l=0x01; //右侧花边处理时用
	r=0x7f;_r=0x80; //左侧花边处理时用
	bit=0x80; // ZZW 14.4.11
	if(pArg->area_hb_flag&0x01){ //有分区花边(即边框)
		ucStep=1;isBk = 1; bit = 0x40;// ZZW 14.4.11=========================================================
		place_h=pArg->place_h+1;   //起始的位置后移一位
		screen_h=pArg->screen_h-2; //屏高少两行，留作花边用
	}

	ps_len=screen_w+place_w;//数据的最右边的位置
	if(hb.flag){    //有花边
		isHb = 1;// ZZW 14.4.11
		bit=0x40; // ZZW 14.4.11
		if((place_w==0)&&((hb.high+1)<8)){//屏长等于节目显示的最左边的位置，并且花边的高度小于7
			r=0xff>>(hb.high+1); //左侧花边处理时用
			_r=~r;
			bit=0x80>>(hb.high+1);			
		}else if((place_w==0)&&(hb.high==7)){//屏长等于节目显示的最左边的位置，并且花边的高度等于7
			place_w+=1;  //节目的起始地址往右移动一位
		}
		if((len==(screen_w+pArg->place_w))&&((hb.high+1)<8)){ //起始位置为最右边，并且花边的高度小于7
			l=0xff<<(hb.high+1); //右侧花边处理时用
			_l=~l;	
		}else if((len==(screen_w+pArg->place_w))&&(hb.high==7)){//起始位置为最右边，并且花边的高度等于7
			ps_len=screen_w+pArg->place_w-1;//屏幕的终止地址为屏的右边倒数第二位
		}
	}
	

	if(r2){//非连移
		if((pArg->parameter+ucStep)==screen_w*8*(pArg->cnt+1)){ //一屏的数据移动完 
			pArg->cnt++;//屏数自加
			pArg->st=1; //每屏数据移动完之后停顿一下
			return;
		}
	}
 	if(pArg->sj){ //随机播放
		if(pArg->parameter==screen_w*8){//播放到最后一列
			pArg->st=1;        //每屏停顿
			pArg->parameter=0; //每屏数据移动完将屏参数清0
			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//节目的地址指向下一屏数据的起始地址
			return;
		}
	}
	else if((pArg->parameter+ucStep)==pArg->info_data_w*8){//非随机播放，当移动完所有数据
		pArg->parameter=0;  //将屏参数清0
		pArg->info_end=1;   //节目结束标志 
		pArg->st = 0;         //停留标志
		 if ( ((pro_num > 1) && (pArg->info_number > 1)) || (pArg->info_standtime > 0) )
		 {
				pArg->st = 1;
		 } 			
		pArg->info_c_speed = 0; /* ADD BY ZZW 2013.5.3 */
		pArg->cnt=0;        //屏数清0
		return;	 
	}

	for(j=place_h;j<screen_h+place_h;j++){//显示的节目从起始位置开始，共screen_h行,每次往缓冲区写一行的数据
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//红色数据
			temp=j*len; //第j行的第0个数据
			save=leddata[temp+ps_len-1]&_l;//右侧花边的位置 _l=0x01;
			
			GREEN_BEGIN //绿色数据
				save1=leddata[temp+size_green+ps_len-1]&_l;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				save2=leddata[temp+size_blue+ps_len-1]&_l;
			#endif	
		}
		for(i=ps_len-1;i>place_w;i--){//每行数据从屏的末端开始，长度为ps_len=screen_w+place_w ，到数据开始的位置结束
			//红色数据
			temp=j*len;	
			d1=leddata[i+temp]>>1; //每个数据向右移动一位
			d2=	leddata[i-1+temp]&0x01;//前一个数据的最低位取出来，补到数据移动之后的最高位
			d2<<=7;
			leddata[i+temp]=d1 | d2;
			
			GREEN_BEGIN  //绿色数据
				temp=j*len+size_green;
				d1=leddata[i+temp]>>1;
				d2=	leddata[i-1+temp]&0x01;
				d2<<=7;
				leddata[i+temp]=d1 | d2;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				temp=j*len+size_blue;
				d1=leddata[i+temp]>>1;
				d2=	leddata[i-1+temp]&0x01;
				d2<<=7;
				leddata[i+temp]=d1 | d2;	
			#endif
		}
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//红色数据
			temp=j*len+ps_len;//数据指向该行的最右边的一个字节
			leddata[temp-1]&=l;   //清数据的最高位 l=0xfe
			leddata[temp-1]|=save;//数据加上花边
			
			GREEN_BEGIN //绿色数据
				temp=j*len+size_green+ps_len;
				leddata[temp-1]&=l;//0xfe;
				leddata[temp-1]|=save1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //蓝色数据
				temp=j*len+size_blue+ps_len;
				leddata[temp-1]&=l;//0xfe;
				leddata[temp-1]|=save2;
			#endif
		}	
	}
	flash_addr=pArg->info_content_addr+isBk*data_w+(data_w-pArg->parameter/8-1);
	r1=getbit(7-pArg->parameter%8); //移动到数据的第几位
	for(j=place_h;j<screen_h+place_h;j++){//每行数据从起始位置开始，长度为screen_h
	 //红色数据	
		temp= place_w+j*len;//第j行的第一个字节的数
		if ( isBk || isHb )
		{	
			save=leddata[temp]&_r; //左侧花边的位置的数 _r=0x80;
			leddata[temp]&=r;      //清数据的最高位 r=0x7f
			leddata[temp] = ((leddata[temp]>>1)|save);//数据加上花边
		}
		else
		{
			leddata[temp] >>= 1;
		}		
		d=flash_read(flash_addr);	 
		if(d&r1){ //如果移动的那一位数据为1
			leddata[temp] |= bit;//0x40; 
		 }
		 
		GREEN_BEGIN //绿色数据
			temp=place_w+j*len+size_green;
			if ( isBk || isHb )
			{		 
				save=leddata[temp]&_r;
				leddata[temp]&=r;
				leddata[temp]=(leddata[temp]>>1)|save;
			}
			else
			{
				leddata[temp] >>= 1;
			}
			d=flash_read(flash_addr+DATA);	 
			if(d&r1)leddata[temp]|=bit;
		GREEN_END
		 
		#ifdef CONFIG_BLUE_COLOR  //蓝色数据
			temp=place_w+j*len+size_blue;
			save=leddata[temp]&_r;
			leddata[temp]&=r;
			leddata[temp]=(leddata[temp]>>1)|save;
			d=flash_read(flash_addr+2*DATA);
			if(d&r1)leddata[temp]|=bit;
		#endif
		flash_addr+=data_w; //每次写缓冲区之后，地址指向下一行数据的起始位
	}
	pArg->parameter++; 
}
 
/**************************************************************************
** 函数名称 ： datadeal_move
** 函数功能 ： 动画处理
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void datadeal_move()
	{ 
	unsigned short i; 
	unsigned short scr_len=SCREEN_LEN,screen_w=pArg->screen_w;
	unsigned char place_h=pArg->place_h,place_w=pArg->place_w,screen_h=pArg->screen_h;
	unsigned short j,data_h=pArg->info_data_h,DATA=screen_w*screen_h;
 
	if(pArg->area_hb_flag&0x01){//分区花边(即边框)
		place_h=place_h+1;   //节目开始的位置向下移动一行，留作边框用
		screen_h=screen_h-2; //节目的高度减2
		i=1;
	}
	
	flash_addr=pArg->info_content_addr+pArg->parameter*screen_w*pArg->screen_h*offset;//当前屏所需显示的数据的首地址
	pArg->parameter++;   //每显示一行数据，屏参数自加1
	if(pArg->parameter ==data_h){   //显示完一屏数据
		pArg->parameter=0;  //每显示完一屏数据，屏参数清0
		pArg->st=1;         //停留标志，每屏停顿一下
		pArg->info_end=1;   //节目结束标志 
		if(pArg->style==4)pArg->end=1; //动画，分区结束
	}
	//红色数据	
	flash_read(flash_addr);	 
	for(j=place_h;j<screen_h+place_h;j++){
 		for(i=place_w;i<place_w+screen_w;i++){
		leddata[i+j*scr_len]=srs();//将动画的点阵数据写入leddata中

 		} 
 	}
	GREEN_BEGIN //绿色数据
	flash_read(flash_addr+DATA);
	for(j=place_h;j<screen_h+place_h;j++){
 	 for(i=place_w;i<place_w+screen_w;i++){
	   leddata[i+(j+OFFSET_GREEN)*scr_len]=srs();
 	 }
 	}
	GREEN_END
	#ifdef CONFIG_BLUE_COLOR //蓝色数据
	flash_read(flash_addr+2*DATA);
	for(j=place_h;j<screen_h+place_h;j++){
 	 for(i=place_w;i<place_w+screen_w;i++){
	 	leddata[i+(j+OFFSET_BLUE)*scr_len]=srs();
 	 }
 	}
	#endif 
}

/**************************************************************************
** 函数名称 ： datadeal_word
** 函数功能 ： 文本图片处理
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void datadeal_word() 
{
 if(pArg->info_mode>24){
 	datadeal_move();
 }else{
	 switch(pArg->info_mode){
	  case 1: 
	  	still_display();     	  //静止显示 
	   	break;
	  case 3: 
	  	pull_up_down_screen(1,0); //向上拉幕
	   	break;
	  case 4:
	  	 pull_up_down_screen(0,0); //向下拉幕
	   	break;
	  case 5: 
	  	pull_left_right_screen(1,0);     //向左拉幕
	   	break;
	  case 6:
	  	pull_left_right_screen(0,0);     //向右拉幕
	   	break;
	  case 7: 
	  	closed_down_screen(1);   //上下闭合
	   	break;
	  case 8: 
	  	down_on_screen();       //上下对开
	   	break;
	  case 9: 
	  	left_right_close(0);      //左右闭合
	   	break;
	  case 10: 
	  	left_right_open();       //左右对开
	   	break;
	  case 11: 
	  	V_blinds();    		  //垂直百叶
	  	break;
	  case 12:
	  	H_blinds();			  //水平百叶				  
	   	break;
	
	  case 13: 
	  	if(pArg->sj){up_move(0);break;}
	  	else{up_move(1);break;}			   //向上移动
	   
	  case 14: 
	  	if(pArg->sj){
			down_move(0);
			break;
		}else{
			down_move(1); 
			break;
		}          //向下移动
	   
	  case 15: 
	  	if(pArg->sj){
			left_move(0);
			break;
		}else{
			left_move(1);
			break;
		}			  //向左移动
	   
	  case 16: 
	  	if(pArg->sj){
			right_move(0);
			break;
		}else{
			right_move(1);
			break;
		}		  //向右移动
	   
	  case 17: 
	  	up_move(0);			  //向上连移
	   	break;
	  case 18: 
	  	down_move(0);		  //向下连移
	   	break;
	  case 19: 
	  	left_move(0);  //向左连移		  
	   	break;
	  case 20: 
	  	right_move(0);		  //向右连移
	  	break;
	  case 21: 
	  	pull_up_down_screen(1,1);   //向下镭射
	   	break;
	  case 22: 
	  	pull_up_down_screen(0,1);  //向上镭射   
	   	break;
	  case 23: 
	  	pull_left_right_screen(0,1);//向左镭射	 
	  	break;
	  case 24: 
	  	pull_left_right_screen(1,1);//向右镭射	 
	  	break;
	  default:  
	   break;
	 }
  }
 
}

/**************************************************************************
** 函数名称 ： srs
** 函数功能 ： 读flash中的数据
** 入口参数 ： 无
** 返回参数 ： 读出的flash中的数据
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
unsigned char srs()
{
	unsigned char i=spdats;	 //读出*(u16*)0x4001300c指向的寄存器中的数据
	spdelay;				 
	return i;
}

/**************************************************************************
** 函数名称 ： PutPixel
** 函数功能 ： 画一个点
** 入口参数 ： 需要画的点的位置：横坐标x，纵坐标y
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void PutPixel(unsigned short x,unsigned short y)    // x<1024  y<64
{
	unsigned char i;
	register unsigned short len=SCREEN_LEN;
	register unsigned short size_green=len*OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		register unsigned short size_blue=len*OFFSET_BLUE;
	#endif
	if(x>1024||y>64)return;//限制长高：长超过1024或高超过64时，返回。
	i=x%8;     //长字节数
	i=0x80>>i; //该写的位置
	if(pArg->info_mode&0x01)leddata[y*len+x/8]|=i;//红色数据，第y行的第x位置1
	if(pArg->info_mode&0x02)leddata[y*len+size_green+x/8]|=i;//绿色数据
	#ifdef CONFIG_SEVEN_COLOR//蓝色数据
		if(pArg->info_mode&0x04)leddata[y*len+size_blue+x/8]|=i;
	#endif				
}

/**************************************************************************
** 函数名称 ： printf_line
** 函数功能 ： 画直线
** 入口参数 ： 起始位置的坐标(x1,y1),终点位置的坐标(x2,y2)
** 返回参数 ： 无
** 函数说明 ： 算法按数控加工中的插补法中的逐点比较法来算
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void printf_line(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
       int dx,dy,e;
       dx = x2 -x1;  //直线在x坐标上的映射
       dy = y2 -y1;  //直线在y坐标上的映射
       if(dx >= 0)   //直线在x坐标上有映射
       {
              if(dy >= 0)  //直线在y坐标上有映射
              {
                     if(dx>=dy) // 1/8 octant
                     {
                            e=dy-dx/2; //用来判断x或y坐标进给的标志，e>0沿y方向进给，反之沿x方向进给
                            while(x1<=x2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){y1+=1;e-=dx;}//每画一个点，将x或y坐标加1，同时计算e的值		
                                   x1+=1; //每画完一个点，x的起始位置往后移一位
                                   e+=dy;
                            }
                     }
                     else		// 2/8 octant
                     {
                            e=dx-dy/2;
                            while(y1<=y2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){x1+=1;e-=dy;}	
                                   y1+=1;
                                   e+=dx;
                            }
                     }
              }
              else		   // dy<0
              {
                     dy=-dy;   // dy=abs(dy)

                     if(dx>=dy) // 8/8 octant
                     {
                            e=dy-dx/2;
                            while(x1<=x2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){y1-=1;e-=dx;}	
                                   x1+=1;
                                   e+=dy;
                            }
                     }
                     else		// 7/8 octant
                     {
                            e=dx-dy/2;
                            while(y1>=y2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){x1+=1;e-=dy;}	
                                   y1-=1;
                                   e+=dx;
                            }
                     }
              }	
       }
       else //dx<0
       {
              dx=-dx;		//dx=abs(dx)
              if(dy >= 0) // dy>=0
              {
                     if(dx>=dy) // 4/8 octant
                     {
                            e=dy-dx/2;
                            while(x1>=x2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){y1+=1;e-=dx;}	
                                   x1-=1;
                                   e+=dy;
                            }
                     }
                     else		// 3/8 octant
                     {
                            e=dx-dy/2;
                            while(y1<=y2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){x1-=1;e-=dy;}	
                                   y1+=1;
                                   e+=dx;
                            }
                     }
              }
              else		   // dy<0
              {
                     dy=-dy;   // dy=abs(dy)

                     if(dx>=dy) // 5/8 octant
                     {
                            e=dy-dx/2;
                            while(x1>=x2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){y1-=1;e-=dx;}	
                                   x1-=1;
                                   e+=dy;
                            }
                     }
                     else		// 6/8 octant
                     {
                            e=dx-dy/2;
                            while(y1>=y2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){x1-=1;e-=dy;}	
                                   y1-=1;
                                   e+=dx;
                            }
                     }
              }	
       }
} 

/**************************************************************************
** 函数名称 ： show_second
** 函数功能 ： 显示秒针
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void show_second()
{
	unsigned char i,j,r5,r6,n=0;
	unsigned short r1,r2,r3,r4,x;
 	r1=pArg->place_w; 	   	//坐标x
	r3=pArg->screen_w; 				
 	if(pArg->area_hb_flag&0x01){//分区花边(边框)
		r2=pArg->place_h+1; //起始坐标y
		r4=pArg->screen_h-2;//节目显示的高度
		n=1; //花边高度
	}else{	//无边框		
		r2=pArg->place_h;  //起始坐标y		 
		r4=pArg->screen_h; //节目显示的高度
		n=0; //花边高度
	}
	//红色数据
	r5=n;
	for(i=r2;i<r2+r4;i++,r5++){
		for(j=r1,r6=0;j<r1+r3;j++,r6++){
			leddata[j+i*SCREEN_LEN]=sim_buf_red[r5*16+r6];		 	
		}
	}
	GREEN_BEGIN //绿色数据
		r5=n;
		for(i=r2;i<r2+r4;i++,r5++){
			for(j=r1,r6=0;j<r1+r3;j++,r6++){
				leddata[j+(i+OFFSET_GREEN)*SCREEN_LEN]=sim_buf_green[r5*16+r6];		 	
			}
		}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR //蓝色数据
		r5=n;
		for(i=r2;i<r2+r4;i++,r5++){
			for(j=r1,r6=0;j<r1+r3;j++,r6++){
				leddata[j+(i+OFFSET_BLUE)*SCREEN_LEN]=sim_buf_blue[r5*16+r6];		 	
			}
		}
	#endif
		
	i=time32;//读取此时的秒值
	flash_addr=pArg->info_data_w+i*4;//此时秒针存放的地址 
	flash_read(flash_addr);
 	x=pArg->place_w*8;//屏长(按点计算)
	j=pArg->place_h;  //屏高
	r1=srs()+x;
	r2=srs()+j;
	r3=srs()+x;
	r4=srs()+j;
	printf_line(r1,r2,r3,r4);//画出此时秒针
	pArg->st=1;	 //停留标志
	pArg->end=1; //分区结束		
}

/**************************************************************************
** 函数名称 ： datadeal_sim
** 函数功能 ： 模拟时钟处理
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
void datadeal_sim()
{
	unsigned char h,m,n,i,d,color_ch,asc_vir,hz_vir,asc_size,hz_size,week_w,week_h,j,fg;
	unsigned char r1,r2,r3,r4,width;
	unsigned short green_data; //绿色数据的个数
	#ifdef CONFIG_BLUE_DATA
		unsigned short blue_data;//蓝色数据的个数
	#endif
	
 	memset(sim_buf_red,0,1024);   //缓冲区清0
	memset(sim_buf_green,0,1024);
	#ifdef CONFIG_SEVEN_COLOR
		memset(sim_buf_blue,0,1024);
	#endif

	if(pArg->is_init_sim){ //模拟时钟初始值   
		pArg->is_init_sim=0;
		flash_addr=	pArg->info_content_addr;
		read_buf(34,buf,flash_addr);
		sim.addr=(unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
		sim.h_color = buf[4];    //时针颜色
		sim.m_color = buf[5];    //分针颜色
	    sim.s_color = buf[6];  //秒针颜色
	    sim.date_flag = buf[7];//日期标志
	    sim.asc_w = buf[8];    //字符长
	    sim.asc_h = buf[9];    //字符高
	    sim.hz_w = buf[10];    //汉字长
	    sim.hz_h = buf[11];    //汉字高
		sim.format_addr = (unsigned long)buf[13]<<16 | (unsigned long)buf[14]<<8 | (unsigned long)buf[15];//格式地址
	    sim.asc_addr = (unsigned long)buf[17]<<16 | (unsigned long)buf[18]<<8 | (unsigned long)buf[19]; //字符地址
	    sim.hz_addr  = (unsigned long)buf[21]<<16 | (unsigned long)buf[22]<<8 | (unsigned long)buf[23]; //汉字地址
	    sim.date_place_x = (unsigned short)buf[24]<<8 | (unsigned short)buf[25]; //日期位置横坐标
	    sim.date_place_y = (unsigned short)buf[26]<<8 | (unsigned short)buf[27]; //日期位置纵坐标
	    sim.date_color = buf[28];//日期颜色
	    sim.week_place_x = (unsigned short)buf[29]<<8 | (unsigned short)buf[30]; //星期位置横坐标 
	    sim.week_place_y = (unsigned short)buf[31]<<8 | (unsigned short)buf[32]; //星期位置纵坐标
	    sim.week_color = buf[33];//星期颜色
		sim.week_place_x /= 8;
	  }
		if(sim.asc_w%8) asc_vir = sim.asc_w/8+1;
	    else asc_vir = sim.asc_w/8; //字符长度的字节数
	    if(sim.hz_w%8) hz_vir = sim.hz_w/8+1;
	    else hz_vir = sim.hz_w/8;    //汉字的长度的字节数
	    asc_size = asc_vir*sim.asc_h;//字符的个数
	    hz_size = hz_vir*sim.hz_h;   //汉字的个数
	 
	/********************************时针*****************************************/
	flash_addr=sim.addr;
	h=time32>>16;  //时
	if(h>12)h-=12;  //表盘显示的是12进制
	m=time32>>8;    //分
	h=h*5+m/12;     //时针显示的位置
	flash_addr+=h*4;
	read_buf(4,buf,flash_addr);
	flash_addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
	
	flash_read(flash_addr);	 //时针
	r1=srs()/8;	 //坐标
	r2=srs();
	r3=srs()/8;	 //长高
	r4=srs();
	if(sim.h_color&0x01){ //单色
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){ 
				sim_buf_red[16*m+h]|=srs();	 
			}
		}
	}
	if(sim.h_color&0x02){ //双色
		flash_read(flash_addr+4);
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){
				sim_buf_green[16*m+h]|=srs();	
			}
		}	
	}
	#ifdef CONFIG_SEVEN_COLOR
		if(sim.h_color&0x04){ //七色
			flash_read(flash_addr+4);
			for(m=r2;m<r2+r4;m++){
				for(h=r1;h<r1+r3;h++){
					sim_buf_blue[16*m+h]|=srs();	
				}
			}		
		}
	#endif
   /********************************分针******************************/
	flash_addr=sim.addr;
	h=time32>>8;         //分
	flash_addr+=240+h*4;
	read_buf(4,buf,flash_addr);
	flash_addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
	flash_read(flash_addr);	 //分针
	r1=srs()/8;	  //坐标
	r2=srs();
	r3=srs()/8;	  //长高
	r4=srs();
	if(sim.m_color&0x01){  //单色
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){
				sim_buf_red[16*m+h]|=srs();
			}
		}
	}
	if(sim.m_color&0x02){  //双色
		flash_read(flash_addr+4);
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){
				sim_buf_green[16*m+h]|=srs();
			}
		}		
	}
	#ifdef CONFIG_SEVEN_COLOR
		if(sim.m_color&0x04){  //七色
			flash_read(flash_addr+4);
			for(m=r2;m<r2+r4;m++){
				for(h=r1;h<r1+r3;h++){
					sim_buf_blue[16*m+h]|=srs();
				}
			}		
		}
	#endif
	/*************************************表盘/背影**************************************/
    flash_addr=sim.addr;
	flash_addr+=240*3;
	flash_read(flash_addr);

	r3=pArg->screen_w; //屏的长度（字节数）
	r4=pArg->screen_h; //屏的高度
	for(m=0;m<r4;m++){
		for(h=0;h<r3;h++){
			sim_buf_red[16*m+h]|=srs();
		}
	}
	//GREEN_BEGIN
		green_data=pArg->screen_w*pArg->screen_h;//绿色数据的个数
		flash_read(flash_addr+green_data);	 
		for(m=0;m<r4;m++){
			for(h=0;h<r3;h++){
				sim_buf_green[16*m+h]|=srs();
			}
		}
	//GREEN_END  
	#ifdef CONFIG_SEVEN_COLOR
		flash_read(flash_addr+2*green_data);
		for(m=0;m<r4;m++){
			for(h=0;h<r3;h++){
				sim_buf_blue[16*m+h]|=srs();
			}
		}		
	#endif	 
	/****************************模拟时钟日期***********************************/
    width=sim.date_place_x;	 //日期位置的横坐标即日期的长
	if(sim.date_flag&0x01){	   //显示日期
		flash_addr=sim.format_addr; //格式地址
		n=flash_read(flash_addr);   //格式
		read_buf(n,buf,flash_addr+1);
		
		r2=n; 
		for(i=2;i<n;i++){
			if((buf[i]==0x6e)||(buf[i]==0x79)||(buf[i]==0x72)){//汉字年月日
				for(r1=i;r1<n;r1++)buf[r1]=buf[r1+1];  
				r2--;
			}
		}	
		i=0; scr_info.fg3=0;
		if(buf[1]==0x4d)scr_info.fg3=1;	 //字符月
		if(buf[2]==0x59){		//字符年
			flash_addr=sim.asc_addr+2*asc_size;	 
			color_ch=sim.date_color; //日期颜色
			__dig_write(width,sim.date_place_y,sim.asc_w,sim.asc_h,color_ch);
		    flash_addr = sim.asc_addr;//字符地址
			__dig_write(width+sim.asc_w,sim.date_place_y,sim.asc_w,sim.asc_h,color_ch);
			i+=2; width+=2*sim.asc_w;
		}  
	  	fg=0;
		scr_info.fg2=0;
		for(;i<r2;i++){
			d=asc_check(buf[i],fg);//fg=0获取高位 fg=1获取低位
			if(scr_info.fg1){ //scr_info.fg1=1汉字 ??????
				 flash_addr=sim.hz_addr+d*hz_size;	fg=0;
				 if(sim.date_flag&0x08)color_ch=sim.week_color;//星期颜色
				 else color_ch=sim.date_color;//日期颜色  
			}
			else{
				flash_addr=sim.asc_addr+d*asc_size;
				color_ch=sim.date_color;
				if(!scr_info.fg2) fg=!fg;
				else scr_info.fg2=0;
			}
			__dig_write(width,sim.date_place_y,scr_info.fg1?sim.hz_w:sim.asc_w,scr_info.fg1?sim.hz_h:sim.asc_h,color_ch);  
			width+=scr_info.fg1?sim.hz_w:sim.asc_w;
	    }
	}
	if(sim.date_flag&0x02){	 //显示星期
		i=flash_read(sim.format_addr);//读出日期的格式长度
		flash_addr=sim.format_addr+1+i;
		i=flash_read(flash_addr); //读出时间的格式长度
		flash_addr=	flash_addr+1+i;  //星期内容起始地址
		flash_read(flash_addr);  
		week_w=srs(); week_w /=8; //星期的长 
		week_h=srs();             //星期的高
		d=date32>>16; d-=1;       //读取星期的值
		flash_addr=flash_addr+2+d*week_w*week_h;
		if(sim.week_color&0x01){ //单色
			flash_read(flash_addr);
			for(j=sim.week_place_y;j<sim.week_place_y+week_h;j++){
				for(i=sim.week_place_x;i<sim.week_place_x+week_w;i++){
					sim_buf_red[j*16+i]|=srs();
				} 
			}
	    }
		if(sim.week_color&0x02){  //双色
			flash_read(flash_addr);
			for(j=sim.week_place_y;j<sim.week_place_y+week_h;j++){
				for(i=sim.week_place_x;i<sim.week_place_x+week_w;i++){
					sim_buf_green[j*16+i]|=srs();
				}
			}		
		}
		#ifdef CONFIG_SEVEN_COLOR
			if(sim.week_color&0x04){  //七色
				flash_read(flash_addr);
				for(j=sim.week_place_y;j<sim.week_place_y+week_h;j++){
					for(i=sim.week_place_x;i<sim.week_place_x+week_w;i++){
						sim_buf_blue[j*16+i]|=srs();
					}  
				}		
			}
	   #endif
	} 
	/***************************************************************************/
	pArg->info_mode=sim.s_color;      //保存秒针颜色
	pArg->info_data_w=sim.addr+240*2;	//保存秒针地址 						//显示秒针，传递秒针坐标地址 
} 

/**************************************************************************
** 函数名称 ： datadeal_digi
** 函数功能 ： 数字时钟处理(对秒、分、时分别操作)
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
void datadeal_digi()
{
	unsigned char r1,r3;
	unsigned short pos;
	pArg->st=1;  
	pArg->end=1;	 
	pos=pArg->miao_pos; //有无秒的标志
	r1=0;	r3=0;
	if((pArg->miao_pos&0xff)==0xaa){ //没有秒
		if((time32&0xff)==0){ 
			pos=pArg->fen_pos;
			r1=8;r3=0x33;
		}
		else return;
	}
	while(1){
 		
		flash_addr=pArg->asc_addr+(((time32>>r1)&0xff)/10)*pArg->asc_size;
		dig_write(pos,pArg->info_data_h,pArg->parameter,pArg->parameter>>16,pArg->info_exit_mode);
		flash_addr=pArg->asc_addr+(((time32>>r1)&0xff)%10)*pArg->asc_size;
		dig_write(pos+(pArg->parameter&0xff),pArg->info_data_h,pArg->parameter,pArg->parameter>>16,pArg->info_exit_mode);		
		if(r3==0x55) break;
		if(r3!=0x33){
			if((time32&0xff)==0){
				pos=pArg->fen_pos;   //分的标志
				r1=8;r3=0x33;
				continue;
			}
		}else{
			if((time32&0xffff)==0){
				pos=pArg->info_data_w; //时的位置
				r1=16;r3=0x55;
				continue;
			}
		} 
		break;
	}
	if(time32==0)dig_clock_deal();  
}
 
/**************************************************************************
** 函数名称 ： datn_deal
** 函数功能 ： 农历显示处理
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
void datn_deal() 										
{
  unsigned char i,vir_width,datn_color,size;
  unsigned short width,high,r1;
  unsigned long addr=0;
  pArg->is_init_datn=0;
  flash_addr=pArg->info_content_addr;
  width = flash_read(flash_addr);
  width <<= 8;
  spisd; width |= SPI1->DR;
  spisd; high = SPI1->DR;
  high <<= 8;
  spisd; high |= SPI1->DR;
  spisd; i=SPI1->DR;
  spisd; addr = SPI1->DR; addr <<= 8;
  spisd; addr |= SPI1->DR; addr <<= 8;
  spisd; addr |= SPI1->DR;
  spisd; datn_color=SPI1->DR; 
  if(width%8>0)vir_width = width/8+1;
  else vir_width = width/8;	

  r1=pArg->place_w*8; 
  size=((datn16>>8)%10)+1;
  if((datn16>>8)==10)size=11;
  else if((datn16>>8)/10){
	flash_addr = addr+11*vir_width*high;
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//月高位
  }
	r1+=width;
	flash_addr=addr+size*vir_width*high;
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//月低位
	r1+=width;
	flash_addr=addr;
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//月
	r1+=width;
	if(((datn16&0xff)/10==0)||((datn16&0xff)==10))i=1; 
	else if((datn16&0xff)/10==1)i=11; 
	else if((datn16&0xff)==30)i=4; 
	else i=12; 
	flash_addr=addr+i*vir_width*high;
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//日高位
	r1+=width;
	i=(datn16&0xff)%10+1; 
	if((datn16&0xff)%10==0)i=11;  
	flash_addr=addr+i*vir_width*high;
	dig_write(r1,pArg->place_h,width,high,datn_color);	    //日 
	
	pArg->st=1;  
	pArg->end=1; 
}

/**************************************************************************
** 函数名称 ： pro_end
** 函数功能 ： 节目结束
** 入口参数 ： 无
** 返回参数 ： 结束标志，显示完结束返回1，否则返回0
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
extern unsigned char pro_end1;
unsigned char pro_end()
{
	unsigned char i,temp=0,__temp;
	for(i=0;i<area_num;i++){ //分区的个数    
		if(area[i].end){    //分区结束
			temp |= (1<<i);   //分区结束，则temp的对应位置1
			pArg->info_c=0;   //分区计数清0
			info_init(pArg);
		} 
	}
	__temp=(0xff>>(8-area_num));//从低位开始，为1说明有分区，0位说明没有分区
	if(temp == __temp)pro_end1=1;
	if((temp == __temp) && ((pro_num > 1) || ((pro_flag&0x03) == 0x01)) || ((pro_flag&0x03)==0x01) && (__didi == scr_info.text)){
		scr_info.st=1;//停留标志
		return 1;
	}
	
	else return 0;	
}  

/**************************************************************************
** 函数名称 ： sj
** 函数功能 ： 随机播放
** 入口参数 ： 无
** 返回参数 ： 随机播放的值，即下次播放的运行方式
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
unsigned char sj()
{
	unsigned char cnt=0,n;
	n=pArg->info_mode;   //运行方式
	while(1){
 		pArg->temp=(time_sj+area_c+cnt)%25+3; //随机播放的值
 		cnt++;	
		if((pArg->temp<25)&&(pArg->temp!=n))break;//如果随机值代码大于25，并且不等于上次运行方式，则跳出while循环
	}
	return pArg->temp;
}

/**************************************************************************
** 函数名称 ： info_delay
** 函数功能 ： 信息延时	
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 停留时间到,为达到负载均衡，信息进行空循环操作
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void info_delay()
{
	unsigned char i,n;
	unsigned long r1;
	r1=pArg->screen_w*pArg->screen_h; //屏的点数
 
	if(pArg->style==4)n=30;//如果是动画，则延时时间设为30
	else n=12;  //非动画，则延时设为12
	while(r1--){
		i=n;
		while(i--);	
	}
		
}

/**************************************************************************
** 函数名称 ： datadeal
** 函数功能 ： 数据处理	
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
unsigned char didi=0;  //此变量在本函数和time3函数中使用
void datadeal()
{

	 static unsigned char cnt=0;
	 for(area_c=0;area_c<area_num;area_c++){//按分区分别操作
	  pArg=&area[area_c]; //指针指向分区的起始位置
	  if(pArg->style>1)scr_info.text |= (1<<area_c);
	  switch(pArg->style){
	   case 1: 		//文本，图片处理
/*****************20131230添加，防止多节目，多信息停留时间为0不会切换**************/
            if(pArg->info_c_stand==0){
                if(pArg->info_end){        //当前信息结束           
					if(pArg->info_c==(pArg->info_number-1)){  //当前分区的最后一条信息结束
						pArg->info_c=0;  //分区信息计数清零
						pArg->end=1;	    //当前分区结束
						if(pro_end())return;  //当前节目结束
					}else{	   
						pArg->info_c++; //信息计数自增1
						pArg->info_c %= pArg->info_number;
						info_init(pArg);		   //读取下条信息属性
					}
               }
           }
/*****************20131230添加，防止多节目，多信息停留时间为0不会切换**************/
		if(pArg->st){			//正在停留
			//info_delay();   //停留时,为达到负载均衡，信息进行延时  //使用万能输出的时候 对停顿的亮度影响很严重
			if(pArg->st_end){     //停留时间到	分区处理的时候这些标志清零			
				pArg->st=0;
                pArg->st_end=0;//清除停留标志和清除结束标志  &&(pArg->info_end==1)
				if(pArg->sj){ //随机播放
					pArg->info_mode=sj(); //运行模式为随机取的值
					pArg->sj_cnt++;       //随机播放计数自加1
					if(pArg->sj_cnt==pArg->info_data_h){//随机播放的计数为数据的高，说明一屏的数据显示完
						pArg->sj_cnt=0;     //将随机播放的计数器清0
						pArg->info_end=1;   //节目结束标志
					}
				}
				if(pArg->info_end){ //节目结束
					if(pArg->info_c==(pArg->info_number-1)){  //当前分区的最后一条信息结束
						pArg->info_c=0;  //分区信息计数清零
						pArg->end=1;	    //当前分区结束
						if(pro_end())return;  //当前节目结束
						else {goto end;} 
					}else{	   
						pArg->info_c++; //信息计数自增1
						pArg->info_c %= pArg->info_number;
						info_init(pArg);		   //读取下条信息属性
						goto end; 
					}
				}				
			}else {goto end;} 
		}else
		{
		   	if(pArg->info_c_speed){goto end;} 		//速度阀值
			pArg->info_c_speed=pArg->info_speed;    //当前速度阈值为0时，赋值为节目的速度
	    	datadeal_word(); //调用文本图片处理函数
            
          
		}		  
	    break;
		
	   case 2: 				   //模拟时钟
	   	if(pArg->is_init_sim)datadeal_sim();   //sim_init在分区初始化时置1		
	    if(!((sec_flag>>area_c)&0x01))goto end;// 为了边框显示正常
		sec_flag &= ~(0x01<<area_c);
		if(0x00==(time32&0xff))datadeal_sim();    //当达到一分钟后，调用模拟时间处理
		show_second();	  //在一分钟之内，只调用更改秒针
	    break;
		 
	   case 3: 					  //数字时钟
	    if(pArg->is_init_dig)dig_clock_deal();  //is_init_dig在分区初始化时置1	
	    if(pArg->miao_pos==0xee)goto end;       //有秒
	    if(!((sec_flag>>area_c)&0x01))goto end; 
		sec_flag &= ~(0x01<<area_c);
		datadeal_digi();	   //每秒都要处理		
	    break;
		 
	   case 4: 	 //动画
	    if(pArg->info_c_speed)goto end;     //速度阈值不为0时，一直显示
	    pArg->info_c_speed=pArg->info_speed;//速度阈值为0时，赋值为速度值 
	    datadeal_move();	  
	    break;
		 
	   case 5: 	 //计时
			if(pArg->is_init_count)time_count();
	    if(!((sec_flag>>area_c)&0x01))goto end;
	    sec_flag &= ~(0x01<<area_c);
		time_count();  
	    break;
		 
	   case 6:    //农历
	    if(pArg->is_init_datn)datn_deal(); 
		break;
	   default:break;
	  }	
	  end: 
	  if(pArg->area_hb_flag&0x01){//分区花边(边框)
		if(pArg->speed_hb_c)continue;
		pArg->speed_hb_c=10;
		area_hb(pArg,pArg->area_hb_flag);
	  }
	 } 
	 if((__didi == scr_info.text) && (didi>5)){	  
	 	didi = 0;
	 	pro_end();
	 }

	 if(hb.flag){  //花边 
	 	if(scr_info.is_init_hb)hb_deal();//花边的初始值为1时，显示花边
		if(hb.move_way==0)hb_deal();	   //花边静止显示
		else if(hb.move_way>0){
			if(hb.c_speed)return;      //当前速度阈值不为0时，则一直显示
			hb.c_speed=hb.move_speed;	 //当前速度阈值为0时，重新赋值
			if(hb.move_way==1){	  //顺时针移动
				hb_move_deal(1);
			} 
			else if(hb.move_way==2){  //逆时针移动
				hb_move_deal(0);
			}else if(hb.move_way==3){ //闪烁显示
				if(!cnt){
					cnt=1;
					hb_deal();
				}else if(cnt){
					cnt=0;
					clear_hb();
				}		
			}
		} 
	 }
}
unsigned char year[]=

{0x00,0x00,0x00,0x06,0x00,0x00,0x04,0x00,0x00,0x0C,0x00,0xC0,0x0B,0xFF,0x00,0x10,0x20,0x00,0x10,0x20,
0x00,0x30,0x21,0x00,0x4F,0xFF,0x80,0x0C,0x20,0x00,0x0C,0x20,0x00,0x0C,0x20,0x00,0x0C,0x20,0x40,0x33,
0xFF,0x80,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x00,0x00};/*"年",0*/


unsigned char month[]=
{0x00,0x00,0x00,0x04,0x02,0x00,0x07,0xFF,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x07,0xFE,
0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x04,0x02,0x00,0x07,0xFE,0x00,0x04,0x02,0x00,0x04,
0x02,0x00,0x04,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x10,0x0E,0x00,0x60,0x04,0x00,0x00,0x00,0x00};/*"月",0*/



unsigned char xing[]=

{0x00,0x00,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x08,0x01,
0x00,0x0F,0xFF,0x00,0x08,0x40,0x00,0x04,0x60,0x00,0x08,0x61,0x80,0x0F,0xFE,0x00,0x10,0x60,0x00,0x20,
0x62,0x00,0x47,0xFC,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x7F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00};/*"星",0*/

unsigned char qi[]=


{0x00,0x00,0x00,0x08,0x80,0x00,0x08,0x8F,0xE0,0x08,0xA8,0x40,0x3F,0xF8,0x40,0x08,0x88,0x40,0x08,0x88,
0x40,0x0F,0x8F,0xC0,0x08,0x88,0x40,0x0F,0x88,0x40,0x08,0x88,0x40,0x08,0x8F,0xC0,0x7F,0xE8,0x40,0x09,
0x08,0x40,0x0C,0x98,0x40,0x18,0x50,0x40,0x10,0x60,0x40,0x20,0x41,0xC0,0x00,0x80,0x80,0x00,0x00,0x00};/*"期",0*/



 unsigned char shi[]=


{0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x02,0x00,0x22,0x02,0x00,0x3E,0x02,0x00,0x22,0x02,0x40,0x23,0xFF,
0x80,0x22,0x02,0x00,0x22,0x02,0x00,0x3E,0x62,0x00,0x22,0x32,0x00,0x22,0x32,0x00,0x22,0x02,0x00,0x22,
0x02,0x00,0x3E,0x02,0x00,0x22,0x02,0x00,0x20,0x02,0x00,0x00,0x0E,0x00,0x00,0x04,0x00,0x00,0x00,0x00};/*"时",0*/



unsigned char fen[]=


{0x00,0x00,0x00,0x01,0x18,0x00,0x01,0x90,0x00,0x03,0x08,0x00,0x02,0x08,0x00,0x06,0x04,0x00,0x04,0x02,
0x00,0x08,0x03,0x00,0x10,0x05,0xC0,0x2F,0xFE,0x40,0x40,0x84,0x00,0x00,0x84,0x00,0x01,0x84,0x00,0x01,
0x04,0x00,0x01,0x04,0x00,0x02,0x04,0x00,0x04,0x04,0x00,0x08,0x38,0x00,0x30,0x10,0x00,0x00,0x00,0x00};/*"分",0*/


unsigned char miao[]=



{0x00,0x00,0x00,0x01,0x06,0x00,0x0F,0x04,0x00,0x34,0x04,0x00,0x04,0x04,0x00,0x04,0x35,0x80,0x05,0xA4,
0x40,0x3E,0x24,0x60,0x0C,0x44,0x00,0x0F,0x44,0x00,0x15,0x44,0xC0,0x14,0x84,0x80,0x24,0x05,0x80,0x44,
0x03,0x00,0x04,0x06,0x00,0x04,0x0C,0x00,0x04,0x10,0x00,0x04,0x60,0x00,0x03,0x80,0x00,0x00,0x00,0x00};/*"秒",0*/


 unsigned char shuzi[ ]=

{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x21,0x00,0x21,0x00,0x40,0x80,0x40,0x80,0x40,0x80,
0x40,0x80,0x40,0x80,0x40,0x80,0x40,0x80,0x21,0x00,0x21,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"0",0*/




0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x38,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,
0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x08,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"1",0*/




0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x00,0x41,0x00,0x40,0x80,0x40,0x80,0x40,0x80,0x01,0x00,
0x02,0x00,0x04,0x00,0x08,0x00,0x10,0x00,0x20,0x80,0x40,0x80,0x7F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,/*"2",0*/




0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,0x42,0x00,0x41,0x00,0x41,0x00,0x02,0x00,0x0E,0x00,
0x01,0x00,0x00,0x80,0x00,0x80,0x40,0x80,0x40,0x80,0x41,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"3",0*/
 


0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x02,0x00,0x06,0x00,0x0A,0x00,0x12,0x00,0x12,0x00,0x22,0x00,
0x42,0x00,0x42,0x00,0x7F,0x80,0x02,0x00,0x02,0x00,0x02,0x00,0x0F,0x80,0x00,0x00,0x00,0x00,0x00,0x00,/*"4",0*/




0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3F,0x80,0x20,0x00,0x20,0x00,0x20,0x00,0x20,0x00,0x2E,0x00,
0x31,0x00,0x00,0x80,0x00,0x80,0x40,0x80,0x40,0x80,0x41,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"5",0*/

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x00,0x10,0x80,0x20,0x80,0x20,0x00,0x40,0x00,0x5E,0x00,
0x61,0x00,0x40,0x80,0x40,0x80,0x40,0x80,0x20,0x80,0x21,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"6",0*/




0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x80,0x41,0x00,0x42,0x00,0x02,0x00,0x04,0x00,0x08,0x00,
0x08,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"7",0*/


0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x21,0x00,0x40,0x80,0x40,0x80,0x40,0x80,0x21,0x00,
0x1E,0x00,0x21,0x00,0x40,0x80,0x40,0x80,0x40,0x80,0x21,0x00,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"8",0*/



0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x00,0x21,0x00,0x41,0x00,0x40,0x80,0x40,0x80,0x40,0x80,
0x21,0x80,0x1E,0x80,0x00,0x80,0x01,0x00,0x41,0x00,0x42,0x00,0x3C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"9",0*/
};

unsigned char xingqi[ ]=
{

0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x3F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"一",0*/



0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x0F,0xFE,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"二",0*/



0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0xC0,0x1F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x0F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x7F,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"三",0*/


0x00,0x00,0x00,0x00,0x00,0x80,0x1F,0xFF,0x80,0x11,0x98,0x80,0x11,0x98,0x80,0x11,0x18,0x80,0x11,0x18 ,
 0x80,0x11,0x18,0x80,0x11,0x18,0x80,0x11,0x18,0x80,0x11,0x18,0x80,0x12,0x18,0x80,0x12,0x0F,0x80,0x14 ,
 0x00,0x80,0x18,0x00,0x80,0x10,0x00,0x80,0x1F,0xFF,0x80,0x10,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x00,/*"四",0*/




0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x1F,0xFE,0x00,0x00,0x40,0x00,0x00,0xC0,0x00,0x00,0x80,
0x00,0x00,0x80,0x00,0x00,0x82,0x00,0x0F,0xFE,0x00,0x00,0x82,0x00,0x00,0x82,0x00,0x01,0x02,0x00,0x01,
0x02,0x00,0x01,0x02,0x00,0x01,0x02,0x00,0x01,0x02,0x40,0x7F,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,/*"五",0*/

0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x40,0x00,0x00,0x60,0x00,0x00,0x20,0x00,0x00,0x00,0x40,0x7F,0xFF,
 0xE0,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x88,0x00,0x03,0x04,0x00,0x02,0x06,0x00,0x06,0x03,0x00,0x04,
 0x01,0x00,0x08,0x01,0x80,0x10,0x00,0xC0,0x10,0x00,0xC0,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"六",0*/


0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,
0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,
0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00/*"日",0*/

};
unsigned char move_num=0;
unsigned char time_num=0;
unsigned char K=0;
// void fun_time_asc(unsigned char *p)
// {
// 	unsigned char i,j,k;
// 	unsigned int temp;
// 	for(i=0;i<32;i++)				//行
// 	{
// 		temp=p[i*2];
// 		temp<<=8;
// 		temp|=p[i*2+1];
// 		if(i<6)
// 		{
// 				for(j=0;j<screen_len-1;j++)
// 				{
// 					leddata[(i)*screen_len+j]<<=1;
// 					k=leddata[(i)*screen_len+j+1]&0x80;
// 					k>>=7;
// 					leddata[(i)*screen_len+j]|=k;
// 				}
// 				leddata[(i)*screen_len+screen_len-1]<<=1;
// 		}
// 		else if(i<26)
// 		{
// 				for(j=0;j<screen_len-1;j++)
// 				{
// 					leddata[(i)*screen_len+j]<<=1;
// 					k=leddata[(i)*screen_len+j+1]&0x80;
// 					k>>=7;
// 					leddata[(i)*screen_len+j]|=k;
// 				}
// 				leddata[(i)*screen_len+screen_len-1]<<=1;
// 		}
// 		else
// 		{
// 				
// 		}
// 		temp<<=K;
// 		temp&=0x8000;
// 		temp>>=15;
// 		leddata[(i+6)*screen_len+screen_len-1]|=temp;
// 	}
// 	K++;
// 	if(K>10){K=0;time_num++;}
// }

void fun_time_asc(unsigned char *p)
{
	unsigned char i,j,k;
	unsigned int temp;
	for(i=0;i<20;i++)				//行
	//for(i=4;i<24;i++)				//行
	{
		temp=p[i*2];
		temp<<=8;
		temp|=p[i*2+1];
		for(j=0;j<screen_len-1;j++)
		{
			leddata[(i+6)*screen_len+j]<<=1;
			k=leddata[(i+6)*screen_len+j+1]&0x80;
			k>>=7;
			leddata[(i+6)*screen_len+j]|=k;
		}
		leddata[(i+6)*screen_len+screen_len-1]<<=1;
		temp<<=K;
		temp&=0x8000;
		temp>>=15;
		leddata[(i+6)*screen_len+screen_len-1]|=temp;
	}
	K++;
	if(K>10){K=0;time_num++;}
}
void fun_time_zi(unsigned char *p)
{
	unsigned char i,j,k;
	unsigned int temp;
	for(i=0;i<20;i++)				//行
	//for(i=4;i<24;i++)				//行
	{
		temp=p[i*3];
		temp<<=8;
		temp|=p[i*3+1];
		temp<<=8;
		temp|=p[i*3+2];
		for(j=0;j<screen_len-1;j++)
		{
			leddata[(i+6)*screen_len+j]<<=1;
			k=leddata[(i+6)*screen_len+j+1]&0x80;
			k>>=7;
			leddata[(i+6)*screen_len+j]|=k;
		}
		leddata[(i+6)*screen_len+screen_len-1]<<=1;
		temp<<=K;
		temp&=0x800000;
		temp>>=23;
		leddata[(i+6)*screen_len+screen_len-1]|=temp;
	}
	K++;
	if(K>20){K=0;time_num++;}
	
}
extern unsigned char time_flag;
void  time_datadeal(void)
{
	unsigned char r0,r1,r2,r3,r4,r5,r6;
		if(pArg->info_c_speed){goto end;} 		//速度阀值
			pArg->info_c_speed=pArg->info_speed;    //当前速度阈值为0时，赋值为节目的速度
		gettime();
		r0=date32>>24;//
		r1=(date32>>16);
		r2=date32>>8;
		r3=date32;
		r4=time32>>16;
		r5=time32>>8;
		r6=time32;
	
		switch(time_num)
			{
				case 0:fun_time_asc(&shuzi[80]);break;//2
				case 1:fun_time_asc(&shuzi[0]);break;//0
				case 2:fun_time_asc(&shuzi[(r0/10)*40]);break; //1
				case 3:fun_time_asc(&shuzi[(r0%10)*40]);break; //5
				case 4:fun_time_zi(&year[0]);break; //nian
				case 5:fun_time_asc(&shuzi[(r2/10)*40]);break; //5
				case 6:fun_time_asc(&shuzi[(r2%10)*40]);break; //5
				case 7:fun_time_zi(&month[0]);break; //yue
				case 8:fun_time_asc(&shuzi[(r3/10)*40]);break; //5
				case 9:fun_time_asc(&shuzi[(r3%10)*40]);break; //5
				case 10:fun_time_zi(&xingqi[360]);break; //ri
				case 11:fun_time_zi(&xing[0]);break; //
				case 12:fun_time_zi(&qi[0]);break; //ri
				case 13:fun_time_zi(&xingqi[((r1-1)%7)*60]);break; //5
				
				case 14:fun_time_asc(&shuzi[(r4/10)*40]);break; //1
				case 15:fun_time_asc(&shuzi[(r4%10)*40]);break; //5
				case 16:fun_time_zi(&shi[0]);break; //时
				case 17:fun_time_asc(&shuzi[(r5/10)*40]);break; //5
				case 18:fun_time_asc(&shuzi[(r5%10)*40]);break; //5
				case 19:fun_time_zi(&fen[0]);break; //分
				case 20:fun_time_asc(&shuzi[(r6/10)*40]);break; //5
				case 21:fun_time_asc(&shuzi[(r6%10)*40]);break; //5
				case 22:fun_time_zi(&miao[0]);break; //秒
				default:time_num=0;time_flag=1;
			}

	end:;
}


unsigned long GetCnt(void)
{
		switch ( *(u8*)0x0800FC18 )//*(u8*)0x0800FC18
		{
				case 0xa0: return 28835840;
				case 0xa2: return 43253760;
				case 0xa4: return 57571680;
				case 0xa5: return 64760640;
				case 0xa6: return 72088600;
				case 0xa8: return 86507520;
				case 0xaa: return 0x603FFFF;
				case 0xac: return 0x6dfffff;
			  default: return 0x6dfffff;
		}
}

/********************************************************************/
#ifdef ConMod1
/**************************************************************************
** 函数名称 ： USART1_IRQHandler
** 函数功能 ： 串口1中断接收
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
extern unsigned char wifi_init_flag;                    //wifi 初始化标志位上电启动 0；配置 1；正常工作 2；



unsigned char usart_buf[200];
unsigned char buf_num=0;
#if 1
void USART1_IRQHandler(void)   					
{
	unsigned char *U=(u8*)(0x0800FC20);

	
  unsigned char ucTmp, i = 0,z, buf1[]={0xaa,0x55,0xaa,0x55,0x00,0x00},*p=(u8*)0x0800fC00;
  unsigned long cnt,r1, dwTmpCnt;
  static unsigned char sw=0,temp=0;
	u8 sums,u_getsum;
	USART1->CR1&=0Xffdf;//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关中断
	if(wifi_init_flag!=1)
	{
		for(i=0;i<6;i++){//检测接收到的前导符是否正确
		 if(buf1[i]!=ur()){
		goto out; //如果接收到的错误，则跳出中断
		 }
		}

    i=ur(); //接收命令字
	switch(i){
		
		case(0x07):goto set_time;        //校时
		case(0x08):goto screen_on;       //开机
		case(0x09):goto screen_off;      //关机
		case(0x0a):goto set_bright;      //手动调节亮度
		
		
		case(0x61):goto interval_Pattern;  //间隔花样开关=========================================================================	 
		case(0x62):goto disp_Pattern;      //花样============================================================================		
		case(0x63):goto time_on_off; 
		case(0x64):goto set_bright_bg;    //背景亮度======================================================================
					
			
		case(0x0c):goto set_bright_timer;//定时调亮
		case(0x0d):goto time_sw;         //定时开关机
	
		case(0x78):goto flash_write;     //接收节目到外部flash
		case 0x7c: goto SW;              //修改波特率
		case 0x7d: goto rate;            //修改波特率命令
		case(0x82):goto config_info;     //读取控制卡的配置
		case(0x83):goto vision;          //读取版本号
  	case(0x84):goto s_high;          //读取版本号

		case(0x86):goto set_rcc;        //设置点频

		case(0xab):goto flash_read;     //读flash的内容
		case(0xFE):goto read_Time;	//读取时间   ///TEMP

     case(0x77):goto set_wifi_key;
     case(0x80): goto set_wifi_ssid;

	default:goto out;
	}
read_Time:
	ut(0x04);
	for(i=0;i<10;i++)
		ut(*U++);
	ut(0x04);
	goto out;

rate:   //修改波特率命令
	i=ur();
	if((i>0)&&(i<4)){
		temp=i;
		ut(0xa5);ut(0x5a);
		sw=1;	
	}
	goto out;
	
SW:   //修改波特率
	if(sw){  
	  sw=0;
		flash_read_write(1); 						//0x14 存放卡类型
		sim_buf_red[0x1e]=temp;
		flash_read_write(0);
		ut(0x88);
		r1=0x800000;
		while(r1--); //延时
		SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	}
	goto out;
	
set_rcc:  //设置点频
	i=ur();
	rcc_set(i);
	ut(0x88);
	r1=0x800000;
	while(r1--); //延时
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	goto out;
	
set_bright_timer:  //设置实时亮度
	for(i=6;i<18;i++)buf[i]=ur();
	timer_bright_set();
	goto out;
	

	
config_info:  //读取控制卡的配置
	flash_addr=8;
	flash_read(flash_addr);
	for(i=0;i<9;i++){
	ut(SPI1->DR);
	spisd;
	}
	goto vision;

set_bright:	 									//手动调亮
	i=0xff-ur(); 
	if(i==0)i=16;
	flash_read_write(1); 						//0x29 存放亮度值
	sim_buf_red[BRIGHT_WORD_ADDR]=i;
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //延时
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位
	goto out;
	
set_bright_bg:	 									//背景亮度===============================================================
	i=ur(); 
	flash_read_write(1); 						//
	sim_buf_red[BRIGHT_BG_ADDR]=i;    
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //延时
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位
	goto out;
	
interval_Pattern:	 									//间隔花样===============================================================
	//i=ur(); 
	flash_read_write(1); 						//TIME_ONOFF_ADDR
	if(sim_buf_red[INTERVAL_PATTERN_ADDR])	sim_buf_red[INTERVAL_PATTERN_ADDR]=0x00;
	else sim_buf_red[INTERVAL_PATTERN_ADDR]=0x01;
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //延时
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位
	goto out;			


time_on_off:	 									//时间开关===============================================================
	//i=ur(); 
	flash_read_write(1); 						//TIME_ONOFF_ADDR
	if(sim_buf_red[TIME_ONOFF_ADDR])	sim_buf_red[TIME_ONOFF_ADDR]=0x00;
	else sim_buf_red[TIME_ONOFF_ADDR]=0x01;
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //延时
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位
	goto out;			
	
disp_Pattern:	 									//DISP_PATTERN_ADDR  花样测试开关===============================================================
	//i=ur(); 
  	flash_read_write(1); 						//DISP_PATTERN_ADDR
// 	if(sim_buf_red[DISP_PATTERN_ADDR]<0x07)	sim_buf_red[DISP_PATTERN_ADDR]+=0x01;
// 	else if(sim_buf_red[DISP_PATTERN_ADDR]>=0x07)sim_buf_red[DISP_PATTERN_ADDR]=0x00;
	if(sim_buf_red[DISP_PATTERN_ADDR]>0x00)	sim_buf_red[DISP_PATTERN_ADDR]=0x00;
	else if(sim_buf_red[DISP_PATTERN_ADDR]<0x01)sim_buf_red[DISP_PATTERN_ADDR]=0x01;

// 	if(sim_buf_red[0x21])	sim_buf_red[0x21]=0x00;
// 	else sim_buf_red[0x21]=0x01;
 	flash_read_write(0);
 	ut(0x88);
 	r1=0x800000;
 	while(r1--); //延时
  	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位
	goto out;	
	
screen_on:  //开机
	scr_info.of=0;
	scr_info.st=1;
	scr_onoff=0;
// 		flash_read_write(1);    
// 		sim_buf_red[0x38] = 0xff;
// 		flash_read_write(0);	
		
	ut(0x88);
	r1=0x800000;
	while(r1--); //延时
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	goto out;
	
screen_off:  //关机
// 	flash_read_write(1);    
// 	sim_buf_red[0x38] = 0xa0;
// 	flash_read_write(0);	
	clr_screen();
	ut(0x88);
	scr_onoff=1;
	//SoftReset();
	if(scr_info.d_l)goto out;
	
	
	scr_info.of=1;
	scr_info.st=1;
	GPIOA->ODR |= 0x0f; 
	goto out;
	

	
vision:  //读取卡型和版本号
	ut(card); 
	ut((u8)(VISION>>8));
	ut((u8)VISION);
	goto out; 
	
	s_high:  //读取卡型和版本号
	
	ut(card); 
	ut((u8)(VISION>>8));
	ut((u8)VISION);
	ut((u8)(dotmatrix>>24));
	ut((u8)(dotmatrix>>16));
	ut((u8)(dotmatrix>>8));
	ut((u8)dotmatrix);
	
	ut((u8)(dotmatrixhigh>>8));
	ut((u8)dotmatrixhigh);
	
	goto out; 
		


flash_write: //将节目存入flash uiFileSum = 0, uiSum = 0
	ur();
	clr_screen(); //清屏
	//ledplay_denglongHei(0x00);
 	flash_chip_erase(); //擦除flash
	flash_addr=0;
	if(flash_read(0)!=0xff){ //判断是否擦除完
	flash_byte_write(0x0);
	flash_chip_erase(); //再次擦除
	}
	flash_read_write(1);//读
	cnt=0xffff;
	while(cnt--);
	//2013.10.30 wwd *clr03
	ut(0x03);   //擦除完，返回0x03

	cnt=0;
	dwTmpCnt = GetCnt();
	dwTmpCnt &= 0xfffffff;	   								//等待约30秒
	r1 = dwTmpCnt;
     while((USART1->SR&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//喂狗
	 	if(--r1==0)
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//复位
	}
	cnt=USART1->DR;
	cnt<<=8;
	cnt|=ur();
	cnt<<=8;
	cnt|=ur();   //将节目的大小赋值给cnt
	cnt += 8;// 	cnt -= 3; 2013.10.30 wwd 删除，完全接收字节
//	cnt += 20;// 	20个0xff 2013.10.30 wwd 删除，完全接收字节
	sim_buf_red[0x78]=cnt;
	sim_buf_red[0x79]=cnt>>8;
	sim_buf_red[0x7a]=cnt>>16;
	sim_buf_red[0x7b]=cnt>>24;
	if(cnt>bitmaxlen)
	{
		screeniniterr=4;
		goto out;
// 		SoftReset();//15.3.19
	}
	flash_addr=2;
	sums=u_getsum=0;
	while ( cnt -- )
	{
		r1=dwTmpCnt;	   								//等待约30秒
    while((USART1->SR&0x20)==0)//while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{IWDG_Feed();//喂狗
			if ( --r1 == 0 )
			{	
				SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//复位
			}	
		}
		ucTmp = USART1->DR;
		flash_byte_write( ucTmp ); //将接收到的数据存入外部flash中
		flash_addr++;	
		IWDG_Feed();//喂狗
	 if(cnt>0)u_getsum+=ucTmp;
	 else if(cnt==0)sums=ucTmp;	 
	}

	r1=0xffff;
	while(r1--); //延时
	if(sums==u_getsum)
	{
		flash_addr=0;
		flash_byte_write(0x88);
		flash_read_write(0);//写
		ut(0x88);    //返回0x88
		r1=0x800000;
		while(r1--); //延时
	}
	
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位 
	goto out;
	
flash_read:   //读取flash的数据
	r1=ur();   //读取 flash数据的个数
	i=ur();    //读取内部flash和外部flash的标志
	switch(i){
	case 0xfe:
		r1=r1*256;
		while(r1--)ut(*p++);
		break;
 
	default:
		cnt=r1;
		cnt<<=8;
		cnt|=i; //读取数据的个数
 		flash_addr=ur();
		flash_addr<<=8;
		flash_addr|=ur();
		flash_addr<<=8;
		flash_addr|=ur();	 //读取的数据的地址	
     	i=flash_read(flash_addr);
		while(cnt--){
		ut(i);
		spisd;
		i=SPI1->DR;
		}
		OFF_CE;		
	}
	goto out;
	
set_time:  //校时   
	ut(0x88);  // 给上位机发送0x88
	for(i=0;i<8;i++)
	buf[i]=ur();    //将实时的年、月、日、星期、时、分、秒存入buf缓存
	settime(buf,0); //将实时时间和日期转换为秒值写入RTC的计数器中
	for(i=0;i<8;i++)
	{
		area[i].is_init_sim=1;
		area[i].is_init_dig=1;
	}
	scr_info.st=1;
	pro_c=0;
	ut(0x88); //完成操作之后，给上位机发送0x88
// 	if(scr_info.d_l)SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	
	goto out;
	
time_sw: //定时开关机
	ut(0x88);
	flash_read_write(1);
	cnt=0;
	for(i=0;i<9;i++)buf[i]=ur(); //将时间存入buf中
	if((buf[2]|(buf[3]<<8))==0){	  
	sim_buf_red[0x28]=0xff;
	flash_read_write(0);
	scr_info.of=0; //开机
	goto out;
	}
	for(i=0;i<9;i++)sim_buf_red[i+0x20]=buf[i];
	flash_read_write(0);
	goto out;
 set_wifi_key:                   //设置Wi-Fi热点密码
		if(mm_set_en) {								//上电40S内 允许设置WIFI密码
			flash_read_write(1);            
			i=ur();
								 //存储密码长度小于19字节且大于8个字节
			if(i>=8&&i<20)
			{
				 sim_buf_red[0x40]=i;
			z=0x41;                         
			for(;i>0;i--)                   
			{
				 sim_buf_red[z]=ur();
				 z++;
			}
			sim_buf_red[0x55]=0xab;
			flash_read_write(0);
			ut(0x88);
			 r1=0xcfffff;
					while(r1--);
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位 
			}   
		}
    goto out;
    
set_wifi_ssid:
		if(mm_set_en) {								//上电40S内 允许设置WIFI密码
				flash_read_write(1);            
				sim_buf_red[0x56]=ur();
				i=sim_buf_red[0x56];            //存储密码长度小于19字节且大于8个字节
				if(i>0&&i<20)
				{
						z=0x57;                         
						for(;i>0;i--)                   
						{
							 sim_buf_red[z]=ur();
							 z++;
						}
						sim_buf_red[0x55]=0xab;             //标志位，当复位后需要重新配置模块是配置此位,在IAP程序中清除
						flash_read_write(0);
						ut(0x88);
						 r1=0xcfffff;
								while(r1--);
						SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位 
				}
		}
    goto out;
}
 else  
// if(wifi_init_flag==1)
{
	if(USART1->SR&0x20)
	{
		ucTmp=USART1->DR;
		if(buf_num<200)
		{
			usart_buf[buf_num]=ucTmp;
			buf_num++;
		}
	}
}
 	
out:
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//打开中断
USART1->CR1|=0X20;//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}			
#endif

/**************************************************************************
** 函数名称 ： TIM2_IRQHandler
** 函数功能 ： 定时器2中断
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
// void TIM2_IRQHandler(void)
// { 												
//     TIM2->SR&=0xfffe;//TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 	//清除TIM2的中断待处理位
// 	if(scr_info.of)return; //如果是关机状态，则返回
// 	(*led_play)(); //显示               
// }

void TIM2_IRQHandler(void)
{ 												
    TIM2->SR&=0xfffe;//TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 	//清除TIM2的中断待处理位
	if(scr_info.of)return; //如果是关机状态，则返回
	//(*led_play)(); //显示               
}

/**************************************************************************
** 函数名称 ： TIM4_IRQHandler
** 函数功能 ： 定时器4中断关OE
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
// void TIM4_IRQHandler(void)
// { 			
// 	TIM4->SR&=0xfffe;			//清除TIM4的中断待处理位
// 	TIM4->CR1&=0xFE;			//关定时器TIM4										
// //     TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update); 	//清除TIM4的中断待处理位
// // 	TIM_Cmd(TIM4, DISABLE);//使能定时器4
// 	SET_OE(!scr_info.OE); //关OE
// 	if(scr_info.scan==4)
// 		GPIOA->ODR |= 0x0f;	//t08接口的室外双色处理	   
// }

void TIM4_IRQHandler(void)
{ 			
	TIM4->SR&=0xfffe;			//清除TIM4的中断待处理位
	TIM4->CR1&=0xFE;			//关定时器TIM4										
 
}

/*************************************************************************/
#elif ConMod2

/**************************************************************************
** 函数名称 ： USART1_IRQHandler
** 函数功能 ： 串口1中断接收
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月07日修改	
**************************************************************************/
extern unsigned char wifi_init_flag;                    //wifi 初始化标志位上电启动 0；配置 1；正常工作 2；
unsigned char usart_buf[200];
unsigned char buf_num=0;
u8 usart_num=0;

void usart_send(u8 data)
{
	if(usart_num==2)
	ut2(data);
	else if(usart_num==1)
	ut(data);
}

u8 usart_get( )
{
	if(usart_num==2)
	return ur2();
	else  if(usart_num==1)
	return ur();
	else return 0;
}

u8 USART_DR()
{
	if(usart_num==2)
	return USART2->DR;
	else  if(usart_num==1)
	return USART1->DR;
	else return 0;
}

u8 USART_SR()
{
	if(usart_num==2)
	return USART2->SR;
	else if(usart_num==1)
	return USART1->SR;
	else return 0;
}

void USART_config()
{
	unsigned char ucTmp, i = 0,z, buf1[]={0xaa,0x55,0xaa,0x55,0x00,0x00},*p=(u8*)0x0800fC00;
  unsigned long cnt,r1, dwTmpCnt;
  // unsigned int uiFileSum = 0, uiSum = 0;
  static unsigned char sw=0,temp=0;
	u8 sums,u_getsum;
	
	if((wifi_init_flag!=1)||(usart_num!=2))
	{
		
		for(i=0;i<6;i++)
		{//检测接收到的前导符是否正确
		 if(buf1[i]!=usart_get())
			 return; //如果接收到的错误，则跳出中断
		 
		}

    i=usart_get(); //接收命令字
	switch(i){
	case(0x07):goto set_time;        //校时
	case(0x08):goto screen_on;       //开机
	case(0x09):goto screen_off;      //关机
	case(0x0a):goto set_bright;      //手动调节亮度
		
	case(0x61):goto time_on_off;      //时间开关=========================================================================	 
	case(0x62):goto disp_Pattern;      //花样============================================================================		
	case(0x63):goto time_on_off; 
	case(0x64):goto set_bright_bg;    //背景亮度======================================================================
		
	case(0x0c):goto set_bright_timer;//定时调亮
	case(0x0d):goto time_sw;         //定时开关机		
	case(0x70):goto flash_iap;			//IAP update
	case(0x78):goto flash_write;     //接收节目到外部flash
	case 0x7c: goto SW;              //修改波特率
	case 0x7d: goto rate;            //修改波特率命令
	case(0x82):goto config_info;     //读取控制卡的配置
	case(0x83):goto vision;          //读取版本号
 	case(0x84):goto s_high;          //读取版本号
		/*2013.3.20注释，去掉日期限制功能  modify by zzw*/
//	case(0x85):goto date_limt;
	case(0x86):goto set_rcc;        //设置点频
//	case(0x98):goto init;           //初始化
// 	case(0x99):goto reset_ram;      //复位ram
	case(0xab):goto flash_read;     //读flash的内容
	case(0xFE):goto sendtime;	//读取时间
//	case(0xad):goto rst; //复位
    case(0x77):goto set_wifi_key;
    case(0x80): goto set_wifi_ssid;
	default:return;
	}
sendtime:			//发送时间到模块
	usart_send(0x2A);usart_send(0x74);usart_send(0x69);usart_send(0x6D);usart_send(0x65);usart_send(0x3A);
	for(i=0;i<14;i++)
		usart_send(ledtime[i]);
	usart_send(0x04);
	return;
rate:   //修改波特率命令
	i=usart_get();
	if((i>0)&&(i<4)){
		temp=i;
		usart_send(0xa5);usart_send(0x5a);
		sw=1;	
	}
	return;
	
SW:   //修改波特率
	if(sw){  
	  sw=0;
		flash_read_write(1); 						//0x14 存放卡类型
		sim_buf_red[0x1e]=temp;
		flash_read_write(0);
		usart_send(0x88);
		r1=0x800000;
		while(r1--); //延时
		SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	}
	return;
	
set_rcc:  //设置点频
	i=usart_get();
	rcc_set(i);
	usart_send(0x88);
	r1=0x800000;
	while(r1--); //延时
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	return;
	
set_bright_timer:  //设置实时亮度
	for(i=6;i<18;i++)buf[i]=usart_get();
	timer_bright_set();
	return;
	
//init:  //初始化
//// 	flash_deal();
//// 	scr_info.lk=0;
//	FLASH_Unlock();	 	//FLASH
////	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
//	FLASH_ErasePage(0x0800FC00);
//	flash_chip_erase(); //擦除flash
//	usart_send(0x88);
//	r1=0x800000;
//	while(r1--); //延时
//	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
//	goto out;
	
// reset_ram:
// 	PWR_BackupAccessCmd(ENABLE);
// 	BKP_WriteBackupRegister(BKP_DR8, 0);
// 	BKP_WriteBackupRegister(BKP_DR9, 0);
// 	BKP_WriteBackupRegister(BKP_DR10, 0);
// 	goto out;
	
config_info:  //读取控制卡的配置
	flash_addr=8;
	flash_read(flash_addr);
	for(i=0;i<9;i++){
	usart_send(SPI1->DR);
	spisd;
	}
	return;
	
set_bright:	 									//手动调亮
	i=0xff-usart_get(); 
	if(i==0)i=16;
	flash_read_write(1); 						//0x29 存放亮度值
	sim_buf_red[0x29]=i;
	flash_read_write(0);
	usart_send(0x88);
	r1=0x800000;
	while(r1--); //延时
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//复位
	return;
	
screen_on:  //开机
	scr_info.of=0;
	scr_info.st=1;
	usart_send(0x88);
	r1=0x800000;
	while(r1--); //延时
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	return;
	
screen_off:  //关机
	usart_send(0x88);
	if(scr_info.d_l)return;
	clr_screen();
	scr_info.of=1;
	scr_info.st=1;
	GPIOA->ODR |= 0x0f; 
	return;
	
vision:  //读取卡型和版本号
	usart_send(card); 
	usart_send((u8)(VISION>>8));
	usart_send((u8)VISION);
	return; 
	
	s_high:  //读取卡型和版本号
	
	usart_send(card); 
	usart_send((u8)(VISION>>8));
	usart_send((u8)VISION);
	usart_send((u8)(dotmatrix>>24));
	usart_send((u8)(dotmatrix>>16));
	usart_send((u8)(dotmatrix>>8));
	usart_send((u8)dotmatrix);	
	usart_send((u8)(dotmatrixhigh>>8));
	usart_send((u8)dotmatrixhigh);	
	return; 
		
	flash_iap:   //IAP升级
	usart_get();
	clr_screen(); //清屏
 	flash_chip_erase(); //擦除flash
	flash_addr=0;
	if(flash_read(0)!=0xff){ //判断是否擦除完
	flash_byte_write(0x0);
	flash_chip_erase(); //再次擦除
	}
	
	flash_read_write(1);//读
	cnt=0xffff;
	while(cnt--);
	
	sim_buf_red[0x70]=0x00;
	sim_buf_red[0x71]=0x01;
	flash_read_write(0);//写

	usart_send(0x04);   //擦除完，返回0x04
	//2013.7.29 wwd *clr04
// 	usart_send(0x2A);usart_send(0x63);usart_send(0x6C);
// 	usart_send(0x72);usart_send(0x30);usart_send(0x34); 
		cnt=0;
	 r1=0x8ffffff;	   								//等待约30S
     while((USART_SR()&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//喂狗
	 	if(--r1==0)
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//复位
	}
	
	cnt=USART_DR();
	sim_buf_red[0x75]=cnt&0xff;
	cnt<<=8;
	cnt|=usart_get();
	sim_buf_red[0x74]=cnt&0xff;
	cnt<<=8;
	cnt|=usart_get();
	sim_buf_red[0x73]=cnt&0xff;
	cnt<<=8;
	cnt|=usart_get();
	sim_buf_red[0x72]=cnt&0xff;   //将节目的大小赋值给cnt
	cnt+=1;
	flash_addr=0;
	sums=u_getsum=0;
	while(cnt--){
	 r1=0x8ffffff;	   								//等待约30S
     while((USART_SR()&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//喂狗
	 if(--r1==0)
		 SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//复位
	 }
	 ucTmp=USART_DR();
	 flash_byte_write(ucTmp); //将接收到的数据存入外部flash中
	 flash_addr++;
	 IWDG_Feed();//喂狗
	 if(cnt>0)u_getsum+=ucTmp;
	 else if(cnt==0)sums=ucTmp;
	 
	}
	r1=0xffff;
	while(r1--); //延时
	if(sums==u_getsum)
	{
		usart_send(0x88);    //返回0x88
		sim_buf_red[0x70]=0x01;
		sim_buf_red[0x71]=0x00;
// 		sim_buf_red[0x72]=0x01;
// 		sim_buf_red[0x73]=0x01;
// 		sim_buf_red[0x74]=0x01;
// 		sim_buf_red[0x75]=0x01;
		sim_buf_red[0x76]=sums;
	}
	else 
	{
		sim_buf_red[0x70]=0x00;
		sim_buf_red[0x71]=0x01;
		usart_send(0x77);
	}
		
	flash_read_write(0);//写	
	
	r1=0x800000;
	while(r1--); //延时
	
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位 
	return; 
	
flash_write: //将节目存入flash uiFileSum = 0, uiSum = 0
	usart_get();
	clr_screen(); //清屏
 	flash_chip_erase(); //擦除flash
	flash_addr=0;
	if(flash_read(0)!=0xff){ //判断是否擦除完
	flash_byte_write(0x0);
	flash_chip_erase(); //再次擦除
	}
	flash_read_write(1);//读
	cnt=0xffff;
	while(cnt--);
	//2013.10.30 wwd *clr03
	usart_send(0x03);   //擦除完，返回0x03
// 			usart_send(0x2A);usart_send(0x63);usart_send(0x6C);
// 		usart_send(0x72);usart_send(0x30);usart_send(0x33); 
	
	cnt=0;
	dwTmpCnt = GetCnt();
	dwTmpCnt &= 0xfffffff;	   								//等待约30秒
	r1 = dwTmpCnt;
     while((USART_SR()&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//喂狗
	 	if(--r1==0)
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//复位
	}
	cnt=USART_DR();
	cnt<<=8;
	cnt|=usart_get();
	cnt<<=8;
	cnt|=usart_get();   //将节目的大小赋值给cnt
	cnt += 8;// 	cnt -= 3; 2013.10.30 wwd 删除，完全接收字节
//	cnt += 20;// 	20个0xff 2013.10.30 wwd 删除，完全接收字节
	sim_buf_red[0x78]=cnt;
	sim_buf_red[0x79]=cnt>>8;
	sim_buf_red[0x7a]=cnt>>16;
	sim_buf_red[0x7b]=cnt>>24;
	if(cnt>bitmaxlen)
	{
		screeniniterr=4;
		return;
// 		SoftReset();//15.3.19
	}
	flash_addr=2;
	sums=u_getsum=0;
	while ( cnt -- )
	{
		r1=dwTmpCnt;	   								//等待约30秒
    while((USART_SR()&0x20)==0)//while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{IWDG_Feed();//喂狗
			if ( --r1 == 0 )
			{	
				SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//复位
			}	
		}
		ucTmp = USART_DR();
		flash_byte_write( ucTmp ); //将接收到的数据存入外部flash中
		flash_addr++;	
		IWDG_Feed();//喂狗
	 if(cnt>0)u_getsum+=ucTmp;
	 else if(cnt==0)sums=ucTmp;	 
	}

	r1=0xffff;
	while(r1--); //延时
	if(sums==u_getsum)
	{
		flash_addr=0;
		flash_byte_write(0x88);
		flash_read_write(0);//写
		usart_send(0x88);    //返回0x88
		r1=0x800000;
		while(r1--); //延时
	}

	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位 
	return; 
	
flash_read:   //读取flash的数据
	r1=usart_get();   //读取 flash数据的个数
	i=usart_get();    //读取内部flash和外部flash的标志
	switch(i){
	case 0xfe:
		r1=r1*256;
		while(r1--)usart_send(*p++);
		break;
 
	default:
		cnt=r1;
		cnt<<=8;
		cnt|=i; //读取数据的个数
 		flash_addr=usart_get();
		flash_addr<<=8;
		flash_addr|=usart_get();
		flash_addr<<=8;
		flash_addr|=usart_get();	 //读取的数据的地址	
     	i=flash_read(flash_addr);
		while(cnt--){
		usart_send(i);
		spisd;
		i=SPI1->DR;
		}
		OFF_CE;		
	}
	return; 
	
set_time:  //校时   
// 	usart_send(0x88);  // 给上位机发送0x88
	for(i=0;i<8;i++)
	buf[i]=usart_get();    //将实时的年、月、日、星期、时、分、秒存入buf缓存
	settime(buf,0); //将实时时间和日期转换为秒值写入RTC的计数器中
	for(i=0;i<8;i++)
	{
		area[i].is_init_sim=1;
		area[i].is_init_dig=1;
	}
	scr_info.st=1;
	pro_c=0;
	usart_send(0x88); //完成操作之后，给上位机发送0x88
// 	if(scr_info.d_l)SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	
	return; 
	
time_sw: //定时开关机
	usart_send(0x88);
	flash_read_write(1);
	cnt=0;
	for(i=0;i<9;i++)buf[i]=usart_get(); //将时间存入buf中
	if((buf[2]|(buf[3]<<8))==0){	  
	sim_buf_red[0x28]=0xff;
	flash_read_write(0);
	scr_info.of=0; //开机
	return; 
	}
	for(i=0;i<9;i++)sim_buf_red[i+0x20]=buf[i];
	flash_read_write(0);

	return; 
 set_wifi_key:                   //设置Wi-Fi热点密码
    flash_read_write(1);            
    i=usart_get();
               //存储密码长度小于19字节且大于8个字节
    if(i>=8&&i<20)
    {
       sim_buf_red[0x40]=i;
    z=0x41;                         
    for(;i>0;i--)                   
    {
       sim_buf_red[z]=usart_get();
       z++;
    }
    sim_buf_red[0x55]=0xab;
    flash_read_write(0);
    usart_send(0x88);
     r1=0xcfffff;
     while(r1--);
		
    SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位 
    }   
	return; 
    
set_wifi_ssid:
      flash_read_write(1);            
    sim_buf_red[0x56]=usart_get();
    i=sim_buf_red[0x56];            //存储密码长度小于19字节且大于8个字节
    if(i>0&&i<20)
    {
    z=0x57;                         
    for(;i>0;i--)                   
    {
       sim_buf_red[z]=usart_get();
       z++;
    }
    sim_buf_red[0x55]=0xab;             //标志位，当复位后需要重新配置模块是配置此位,在IAP程序中清除
    flash_read_write(0);
    usart_send(0x88);
     r1=0xcfffff;
     while(r1--);
		
    SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位 
    }
	  return; 
}
 else if(usart_num==2)
	{
		if(USART_SR()&0x20)
		{
			ucTmp=USART_DR();
			if(buf_num<200)
			{
				usart_buf[buf_num]=ucTmp;
				buf_num++;
			}
		}
	}
	usart_num=0;
}


void USART1_IRQHandler(void) //15.3.19  					
{
	USART1->CR1&=0Xffdf;//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//关中断
	usart_num=1;
	USART_config();
	USART1->CR1|=0X20;//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}			

/**************************************************************************
** 函数名称 ： USART2_IRQHandler
** 函数功能 ： 串口1中断接收
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： wwd于2015年05月15日修改	
**************************************************************************/
void USART2_IRQHandler(void)   					
{
	USART2->CR1&=0Xffdf;//USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//关中断
	usart_num=2;
	USART_config();
	USART2->CR1|=0X20;//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}			

/**************************************************************************
** 函数名称 ： TIM2_IRQHandler
** 函数功能 ： 定时器2中断
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
void TIM2_IRQHandler(void)
{ 												
    TIM2->SR&=0xfffe;//TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 	//清除TIM2的中断待处理位
	if(scr_info.of || (scr_info.scan ==6))return; //如果是关机状态，则返回
	(*led_play)(); //显示               
}

/**************************************************************************
** 函数名称 ： TIM4_IRQHandler
** 函数功能 ： 定时器4中断关OE
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
void TIM4_IRQHandler(void)
{ 			
	TIM4->SR&=0xfffe;			//清除TIM4的中断待处理位
	TIM4->CR1&=0xFE;			//关定时器TIM4										
//     TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update); 	//清除TIM4的中断待处理位
// 	TIM_Cmd(TIM4, DISABLE);//使能定时器4
	SET_OE(!scr_info.OE); //关OE
	if(scr_info.scan==4)
		GPIOB->BSRR=0xf; //关行选	//t08接口的室外双色处理	   
}

#endif 


/**************************************************************************
** 函数名称 ： TIM3_IRQHandler
** 函数功能 ： 定时器3中断
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
void TIM3_IRQHandler(void)
{ 	
	unsigned char i;											
    TIM3->SR&=0xfffe;//TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update); 	//清除TIM3的中断待处理位
	
	if ( g_wShowVersionCnt < 400 ) 
	{	
		g_wShowVersionCnt ++;
	}
	
	if(scr_info.of)return; //如果是关机状态，则返回
	ticks--; 

	for(i=0;i<area_num;i++){
		if(area[i].speed_hb_c)area[i].speed_hb_c--; //花边的速度，每次减一  
		if(!area[i].info_c_speed)continue;//如果花边的速度值减至0，则跳出本次循环
	    area[i].info_c_speed--;
	}

	if(hb.c_speed)hb.c_speed--;
	if(ticks==0){
		didi++;
		ticks=200; 
		time_sj++; 	
		
		/* 2013.3.20去掉节目运行一遍锁定 modify by zzw */
		//if(scr_info.lk)return; 
		/*******************end*************************/

		for(i=0;i<area_num;i++){
			if(area[i].info_c_stand==0){ //分区内信息停留时间到
			 	//area[i].st=0;			 //分区信息停留标志清零
			  	area[i].st_end=1;	 //设置停留时间到标记
			 	area[i].info_c_stand=area[i].info_standtime;  //重装停留时间
			 	continue;
			}
			if(area[i].st)area[i].info_c_stand--;  //停留时间每次减一
		}	  
		if(scr_info.s_t){//定长播放
			if(show_timer)show_timer--;
			else {scr_info.s_t=0;scr_info.st=1;} 
		}
	}            
}

/**************************************************************************
** 函数名称 ： time_sw
** 函数功能 ： 定时开关机
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月08日修改	
**************************************************************************/
void time_sw() 							//定时开关机
{
	unsigned char flag,*p=(u8*)(0x0800FC20);
	unsigned short time=time32>>8;
	unsigned short r1,r2,r3,r4;
	flag=*(unsigned char*)(0x0800FC28);//关机标志存放的位置
	if(flag==0xff)return;	 //如果没有设置定时开关机，则返回 
	flag=~flag;     //如果设置定时开关机，则将标志取反
	r1=*p<<8|*(p+1);    //第一次开机时间(时分)
	r2=*(p+3)|*(p+2)<<8;//第一次关机时间(时分)
	r3=*(p+5)|*(p+4)<<8;//第二次开机时间(时分)
	r4=*(p+7)|*(p+6)<<8;//第二次关机时间(时分)
	if(time == r1){     //如果当前的时间到开机时间
		scr_info.st = 1;  //停留时间标志置位
		scr_info.of = flag >> 3; //设置开关机标志
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}else if(time == r2){//如果当前的时间到关机时间
		scr_info.st = 1;   //停留时间标志置位
		scr_info.of = flag >> 2; //设置开关机标志
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}else if(time == r3){ //如果当前的时间到开机时间
		scr_info.st = 1;    //停留时间标志置位
		scr_info.of = flag >> 1; //设置开关机标志
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}else if(time == r4){//如果当前的时间到开机时间
		scr_info.st = 1;   //停留时间标志置位
		scr_info.of = flag;      //设置开关机标志 
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}	
}

/**************************************************************************
** 函数名称 ： RTC_IRQHandler
** 函数功能 ： RTC秒中断
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 2013.3.18 朱振威	
**************************************************************************/
void RTC_IRQHandler(void)
{
	u8 r2,r3,r4;
	u32 r1;
	RTC->CRL&=0X0FFA; 
	r1=RTC->CNTH<<16|RTC->CNTL;
	r2=r1%60;
	time32=(time32&0xffff00)|r2;
	
	scr_info.sec=1;
	sec_flag=0xff;//~0;
	if(r2<3)//判断秒值，如果超过一分钟，则分值加一
	{
		r3=(r1%3600)/60;	//分
		r4=(r1%86400)/3600;				//时
		time32=(r4<<16)|(r3<<8)|r2;
		
	if(r4>=24){    //时值到达24点
			if ( scr_info.of )//15.3.20
			{
				flash_read_write(1);
				sim_buf_red[0x38] = 0xa0;
				flash_read_write(0);
			}	
								
		SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//复位
	}
	time_sw(); //定时开关机
	timer_bright_check();//定时检测亮度
	
	
		if (((r4%4) == 0) && ((time32 & 0xffff) == 0))//4 hours  reset
		{
// 			flash_read_write(1);
// 			sim_buf_red[0x38] = 0xa0|scr_info.of;
// 			flash_read_write(0);		
				if ( scr_info.of )
				{
					flash_read_write(1);
					sim_buf_red[0x38] = 0xa0;
					flash_read_write(0);
				}

			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
		}
	
	}
	
		while(!(RTC->CRL&(1<<5)));
}

const u8 scanbuf[4][32]={//开始扫描 
{0x00,0x00,0x7F,0xFC,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0xFF,0xFE,
0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x10,0x20,0x20,0x20,0x40,0x20},/*"?",0*/
{0x10,0x20,0x10,0x20,0x10,0x20,0x10,0x40,0xFC,0x48,0x24,0x84,0x25,0xFE,0x24,0x82,
0x24,0x00,0x48,0xFC,0x28,0x84,0x10,0x84,0x28,0x84,0x44,0x84,0x80,0xFC,0x00,0x84},/*"?",1*/
{0x10,0x00,0x10,0x00,0x13,0xFC,0x10,0x04,0xFC,0x04,0x10,0x04,0x10,0x04,0x15,0xFC,
0x18,0x04,0x30,0x04,0xD0,0x04,0x10,0x04,0x10,0x04,0x13,0xFC,0x50,0x04,0x20,0x00},/*"?",2*/
{0x10,0x88,0x10,0x88,0x13,0xFE,0x10,0x88,0xFC,0x88,0x10,0x00,0x11,0xFC,0x15,0x24,
0x19,0x24,0x31,0x24,0xD1,0xFC,0x11,0x24,0x11,0x24,0x11,0x24,0x51,0xFC,0x21,0x04},/*"?",3*/
};
/**************************************************************************
** 函数名称 ： EXTI15_10_IRQHandler
** 函数功能 ： 按键测试,u盘中断
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 2013.3.18 朱振威	
**************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	u8 type=0;//测试类型
	u8 n;
	u16 i, lh;
	u8 ch = 0;
	u32 time = 400, dwTime;	
	
// 		if (EXTI_GetITStatus(EXTI_Line13) != RESET){	 //测试按键
// 		EXTI_ClearITPendingBit(EXTI_Line13);
			if (EXTI->PR&(1<<13)){
			EXTI->PR=1<<13;
				
 		delay(100);
		/* 去抖 */
		dwTime = 100000;//300000
		while ( dwTime --> 6 )
		{
			/* 在这段时间内如果有按键弹起动作，则退出中断 */
			if ( GPIOA->IDR & (1 << 13) )  
			{
				return;
			}
		}

		
		/* 等待按键松开 */
		dwTime = 10000000;
		while ( !(GPIOA->IDR & (1 << 13)) && (dwTime --> 5) );
 
		memset(leddata, 0, 8*1024);
		lh = 64 * 2;
		
		timer_init(900);
		timer_init4(700);
		scr_info.OE = 0;
		//SCREEN_LEN=9;
		SCREEN_LEN=13;

		led_play=ledplay_denglong;
		while(1)
		{
		if(ch == 0)
		{	//按第一下
			for(i = 0; i < lh; i++){
				memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
				n = ((n << 1) | (n & 0x80) >> 7);
			}	
			n = ((n << 1) | (n & 0x80) >> 7);//左移
		}
		else if(ch == 3)
		{//按第四下
			for(i = 0; i < lh; i++){
				memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
				n = ((n >> 1) | (n & 0x01) << 7);
			}	
			n = ((n >> 1) | (n & 0x01) << 7);	//右移		
		} 
		else if(ch == 1) 
		{//按第二下
			memset(leddata, 0, 8*1024);//全灭
		}
		else if(ch == 2) 
		{//按第三下
			memset(leddata, 0xff, 8*1024);//全亮
		}
			
		if(--time < 5){//超时复位
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
		}
		delay(500000);

		if (!(GPIOA->IDR & (1 << 13))) {
			delay(1000);//延时去抖
			if (!(GPIOA->IDR & (1 << 13))) {
				
				while (!(GPIOA->IDR & (1 << 13)));//(!(i&0x20)){i=GPIOA->IDR>>8;}
				time = 400;
				ch++;
				if(ch >= 4){
					SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
				}	
			}		
		}	
		}
	}
	
	if (EXTI->PR&(1<<14)){
			EXTI->PR=1<<14;
 		SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	} 
}

/**************************************************************************
** 函数名称 ： show_debug
** 函数功能 ： 显示调试中
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年01月10日修改	
**************************************************************************/
// extern const unsigned char card_debug[3][32];  

//void show_debug() 	//显示调试中
//{
//  unsigned char i,j,k;
//  for(j=0;j<16;j++)
//	for(i=0;i<3;i++)
//	  for(k=0;k<2;k++)leddata[j*SCREEN_LEN+122+i*2+k]=card_debug[i][j*2+k];
//  scr_info.width=128*8;
//  screen_len=128;
//  timer_init(1000);//timer_init(128,0);
//  while(scr_info.d_l);
//}

/**************************************************************************
** 函数名称 ： show
** 函数功能 ： 显示3B卡
** 入口参数 ： 取模数组的第index个数组，写入的个数n，写入缓冲区的位置pos
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
extern const unsigned char card_style[19][16];


void show(unsigned char index,unsigned char n,unsigned char pos)
{
  unsigned char i,j;
  for(j=0;j<16;j++)
	  for(i=0;i<n;i++){
	  	leddata[(j + 1)*screen_len - cardLength-2+ pos + i]=card_style[index+i][j];	
	  }
}

/**************************************************************************
** 函数名称 ： show_info
** 函数功能 ： 显示卡型
** 入口参数 ： 卡型参数ch
** 返回参数 ： 无
** 函数说明 ： ch=1显示卡型，否则显示时间
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void show_info(unsigned char ch)	//显示卡型
{ 
#ifdef RHX_32WU1024
     cardNum[0]=0;cardNum[1]=1;cardNum[2]=2;cardNum[3]=17;cardNum[4]=13;cardNum[5]=12;
	   cardNum[6]=4;cardNum[7]=5; cardNum[8]=11;cardNum[9]=18;cardNum[10]=12;cardNum[11]=14;	
	
#elif RH_32UA //15.3.25
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=17;cardNum[3]=13;cardNum[4]=12;cardNum[5]=5;
	   cardNum[6]=8;	
#elif RH_32MU //15.6.10
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=17;cardNum[3]=13;cardNum[4]=12;cardNum[5]=21;
	   cardNum[6]=5;	
#elif RH_48UA //15.3.25
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=17;cardNum[3]=14;cardNum[4]=19;cardNum[5]=5;
	   cardNum[6]=8;
#elif RH_64UA //15.3.25
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=17;cardNum[3]=16;cardNum[4]=14;cardNum[5]=5;
	   cardNum[6]=8;
	
#elif RHX_64W512//15.3.12
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=2;cardNum[3]=17;cardNum[4]=16;cardNum[5]=14;
	   cardNum[6]=4;cardNum[7]=15;cardNum[8]=11;cardNum[9]=12;
#elif RHX_64W1024//15.3.12
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=2;cardNum[3]=17;cardNum[4]=16;cardNum[5]=14;
	   cardNum[6]=4;cardNum[7]=11;cardNum[8]=18;cardNum[9]=12;cardNum[10]=14;
#elif RHX_48W512//15.3.14
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=2;cardNum[3]=17;cardNum[4]=14;cardNum[5]=19;
	   cardNum[6]=4;cardNum[7]=15;cardNum[8]=11;cardNum[9]=12;
#elif RHX_48W1024//15.3.14
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=2;cardNum[3]=17;cardNum[4]=14;cardNum[5]=19;
	   cardNum[6]=4;cardNum[7]=11;cardNum[8]=18;cardNum[9]=12;cardNum[10]=14;
#elif RHX_32W1024//15.5.5
		 cardNum[0]=0;cardNum[1]=1;cardNum[2]=2;cardNum[3]=17;cardNum[4]=13;cardNum[5]=12;
	   cardNum[6]=4;cardNum[7]=11;cardNum[8]=18;cardNum[9]=12;cardNum[10]=14;

#elif XB_W2_512//15.8.29
		 cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=12;cardNum[5]=17;
	   cardNum[6]=15;cardNum[7]=11;cardNum[8]=12;

#elif XB_W3_512//15.3.14
		 cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=13;cardNum[5]=17;
	   cardNum[6]=15;cardNum[7]=11;cardNum[8]=12;
#elif XB_W3_1024//15.3.14
		 cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=13;cardNum[5]=17;
	   cardNum[6]=11;cardNum[7]=18;cardNum[8]=12;cardNum[9]=14;	
#elif XB_W4_512//15.3.14
		 cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=14;cardNum[5]=17;
	   cardNum[6]=15;cardNum[7]=11;cardNum[8]=12;
#elif XB_W4_1024//15.3.14
		 cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=14;cardNum[5]=17;
	   cardNum[6]=11;cardNum[7]=18;cardNum[8]=12;cardNum[9]=14;	
		 
#elif XB_WU2_320
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=12;
	   cardNum[6]=17;cardNum[7]=13;cardNum[8]=12;cardNum[9]=18;
#elif XB_WU2_512
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=12;
	   cardNum[6]=17;cardNum[7]=15;cardNum[8]=11;cardNum[9]=12;   
#elif XB_WU2_1024
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=12;
	   cardNum[6]=17;cardNum[7]=11;cardNum[8]=18;cardNum[9]=12;cardNum[10]=14;
#elif XB_WUS3_512
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=6;
	   cardNum[6]=13;cardNum[7]=17;cardNum[8]=15;cardNum[9]=11;cardNum[10]=12;

#elif XB_WUS3_1024
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=6;
	   cardNum[6]=13;cardNum[7]=17; cardNum[8]=11;cardNum[9]=18;cardNum[10]=12;cardNum[11]=14;
#elif XB_WUS4_512
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=6;
	   cardNum[6]=14;cardNum[7]=17;cardNum[8]=15;cardNum[9]=11;cardNum[10]=12;
#elif XB_WUS4_1024
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=6;
	   cardNum[6]=14;cardNum[7]=17;cardNum[8]=11;cardNum[9]=18;cardNum[10]=12;cardNum[11]=14;
#elif XB_U2_1024 
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=5;cardNum[4]=12;cardNum[5]=17;
		 cardNum[6]=11;cardNum[7]=18;cardNum[8]=12;cardNum[9]=14;
#elif XB_U3_512
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=5;cardNum[4]=13;cardNum[5]=17;
		 cardNum[6]=15;cardNum[7]=11;cardNum[8]=12;
#elif XB_U3 
   cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=5;cardNum[4]=13;
#elif XB_U4_512
     cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=5;cardNum[4]=14;cardNum[5]=17;
		 cardNum[6]=15;cardNum[7]=11;cardNum[8]=12;
		 
		 
		 
#elif XB_W4_1024  		  //新增加的
		  cardNum[0]=2;cardNum[1]=3;cardNum[2]=17;cardNum[3]=4;cardNum[4]=5;cardNum[5]=6;
	   cardNum[6]=14;cardNum[7]=17;cardNum[8]=11;cardNum[9]=18;cardNum[10]=12;cardNum[11]=14;
		 
#elif XG_W2A 
    cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=4;cardNum[4]=12;cardNum[5]=8;
#elif XG_W2B
     cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=4;cardNum[4]=12;cardNum[5]=9;
#elif XG_W3A
     cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=4;cardNum[4]=13;cardNum[5]=8;
#elif XG_W3B
     cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=4;cardNum[4]=13;cardNum[5]=9;
#elif XG_W4A
     cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=4;cardNum[4]=14;cardNum[5]=8;
#elif XG_W4B
     cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=4;cardNum[4]=14;cardNum[5]=3;
#elif XG_U3B
     cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=5;cardNum[4]=13;cardNum[5]=3;
#elif XG_U4B
     cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=5;cardNum[4]=14;cardNum[5]=3;	
#elif XG_U2A
      cardNum[0]=2;cardNum[1]=7;cardNum[2]=17;cardNum[3]=5;cardNum[4]=12;cardNum[5]=8;

#elif	JS_5W3
			cardNum[0]=20;cardNum[1]=6;cardNum[2]=17;cardNum[3]=15;cardNum[4]=4;cardNum[5]=13;

#elif	JS_5W4
			cardNum[0]=20;cardNum[1]=6;cardNum[2]=17;cardNum[3]=15;cardNum[4]=4;cardNum[5]=14;

#endif



  
	if(ch){
		int i;
		for(i=0;i<cardLength;i++)
			{
		       show(cardNum[i],1,2+i); // 显示 
			}
//		if((card>0x50)&&(card<0x53)){	//32
//			show(4,1,5);  
//			show(3,1,6);
//		}else if((card>0x52)&&(card<0x56)){	 //64
//			show(6,1,5);
//			show(5,1,6);	
//		}
//		else if(card==0x65)
//		{
//			show(4,1,5);  
//			show(3,1,6);
//		}
//	
//		if(card==0x51 || card==0x53)
//			show(7,1,7);//显示S
//		else if(card==0x52 || card==0x54)
//			show(8,1,7);//显示H
//		else if(card==0x55)
//			show(9,1,7);//显示N
//		else if(card==0x65)
//			show(9,1,7);//显示W
	}
// 	RTC_Configuration();
// 	while(1){
//		IWDG_Feed();//喂狗
		show((time32&0xff)/10,1,0);//显示时间-秒(十位)
		show((time32&0xff)%10,1,1);//显示时间-秒(个位)
//		if (scr_info.scan==3)led_play();///14.07.18
//		LEDShowVersion((time32&0xff)/10, 1, -1);
//		LEDShowVersion((time32&0xff)%10, 1, 0);
//	}	  
}

/**************************************************************************
** 函数名称 ： show_info
** 函数功能 ： 显示卡型
** 入口参数 ： 卡型参数ch
** 返回参数 ： 无
** 函数说明 ： ch=1显示卡型，否则显示时间
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void show_err(unsigned char ch)	//显示卡型
{
	u8 errbuf[4][32];
	u16 i,j;	
	
	ch=0;
	while(1)
	{
		if(screeniniterr!=ch)
		{
			ch=screeniniterr;
			clr_screen();
		}
		if(screeniniterr==5)
		{
			show_info(1);
//			show(0,5,3);
//			show((time32&0xff)/10,1,1);//显示时间-秒(十位)
//			show((time32&0xff)%10,1,2);//显示时间-秒(个位)
		}
		else
		{	
			if(screeniniterr==1)
				memcpy(errbuf,proerr,4*32);
			else if(screeniniterr==2)
				memcpy(errbuf,lenerr,4*32);
			else if(screeniniterr==4)
				memcpy(errbuf,longerr,4*32);
			else //if(ch==3)
				memcpy(errbuf,scanerr,4*32);
			
			
				for(j=0;j<16;j++)
				{
					for(i = 0; i< 8; i++)
					{
						leddata[(j + 1) * SCREEN_LEN - 8 + i] = errbuf[i / 2][i % 2 + 2 * j];
					}
				}	
			}	
		IWDG_Feed();//喂狗
		if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5)){led_play();}////14.7.18
//			while(1)
//			{
//				IWDG_Feed();//喂狗
//				if(ch==5)
//				{
//					show((time32&0xff)/10,1,1);//显示时间-秒(十位)
//					show((time32&0xff)%10,1,2);//显示时间-秒(个位)
//				}
//				if (scr_info.scan==3){led_play();}////14.7.18
//				
//				if(ch!=screeniniterr)
//				{
//					ch=screeniniterr;
//					clr_screen();
//					break;
//				}
//			}
		}
}

/**************************************************************************
** 函数名称 ： flash_check
** 函数功能 ： 检查flash有无数据
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void flash_check()
{
  unsigned char j,d;
//   unsigned short time;
  u8 f_sum,sum;
//	u8 a_data=0;//节目时间戳与校验标志
  u32 a_pro_len;
	a_pro_len=*(u32*)0x0800FC78;
//	b_pro_len=*(u32*)0x0800FC7c;
	
//	if((*(unsigned char*)0x0800FC39 == 0xab)){	  
//	    flash_read_write(1);  
//		sim_buf_red[0x39] = 0xff;
//		flash_read_write(0);
//		/* add by zzw 2013.3.20 */
//// 		if ( 0x88 == flash_read(0) )
//// 		{
//// 			ucisEmpty = 0;
//// 		}
//		/***************************/
//		
//		flash_chip_erase(); //擦除flash
//// 		flash_addr=0;
//// 		if(flash_read(flash_addr)!=0xff){//判断是否擦除完
//// 			flash_byte_write(0x0);
//// 			flash_chip_erase(); //再次擦除
//// 		}
////  					
//// 		flash_addr=1;
//// 		for(j=1;j<7;j++){ //flash的第1个地址开始写入1~7
//// 			flash_byte_write(j);
//// 			flash_byte_write(j);//防止第一次没写进去
//// 			flash_addr++;
//// 			time=1000;
//// 			while(time--);//延时	
//// 		}
//// 		for(j=1;j<7;j++){
//// 			if(flash_read(j)!=j)//读出数据，如果读出的数据和写入的数据不同则跳出for循环
//// 				break;
//// 		}
//		/* 如果FLASH为空就默认双色64高输出;2013.3.15修改 */
//// 		if ( ucisEmpty )
//// 		{
//// // 			timer_init(900);
//// // 			led_play=ledplay_dcolor_128;
//// // 			scr_info.scan=3;
//// 			timer_init(1800);
//// 			led_play=ledplay_Single_64;
//// 			scr_info.scan=1;
//// 			scr_info.OE = 1;			
//// 		}	
//		
//		if(j>=7){
//			for(j=0;j<6;j++)
//				show(0,1,2+j); //在屏的第(2+j)个位置显示R
//		}
//		
//		ut(0x2A);ut(0x74);ut(0x69);ut(0x6D);ut(0x65);ut(0x3A);
//		ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);
//		ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);
//		ut(0x04);
//		show_info(0); //显示时间													
//	}
	flash_addr=2;//a_data=0;
	d=flash_read(0);  //读取flash的首地址中的数据
	if((d == 0x88) && (a_pro_len != 0) && (a_pro_len != 0xffffffff))
	{	
// 		timer_init(900);
		f_sum=flash_read(a_pro_len-1+flash_addr);
		sum=flash_sum(flash_addr,a_pro_len-1);
		if(f_sum==sum)
		{
			//a_data=1;
			for(j=0;j<7;j++)
			{
				ledtime[j]=flash_read(a_pro_len-8+flash_addr+j);
			}
		}
		else
			flashcheckerr=1;
	}
	else 
	{
		//a_data=0;
		flashcheckerr=2;
		return;
	}
	flash_addr=2; 
	d=flash_read(flash_addr); //读取flash中地址2的数据
	if((d!=card)&&(d!=0)) //卡型不对
	{
		//a_data=0;
		flashcheckerr=3;
		for(j=0;j<7;j++)
		{
			ledtime[j]=0;
		}
	}	
	sendtime();
//	ut(0x2A);ut(0x74);ut(0x69);ut(0x6D);ut(0x65);ut(0x3A);
//	for(j=0;j<14;j++)
//	{
//		ut(ledtime[j]);
//	}
//	ut(0x04);
//	if(a_data==0)
//	{
//		flashcheckerr=1;
////		timer_init(1800);
////		led_play=ledplay_Single_64;
////		scr_info.scan=1;
////		scr_info.OE = 1;
////        screen_len=32;
////		show_info(1); //显示卡型	
//	}
}

void sendtime()
{
	u8 j;
	ut(0x2A);ut(0x74);ut(0x69);ut(0x6D);ut(0x65);ut(0x3A);
	for(j=0;j<14;j++)
	{
		ut(ledtime[j]);
	}
	ut(0x04);
}

/**************************************************************************
** 函数名称 ： rcc_set
** 函数功能 ： 设置波特率
** 入口参数 ： 写波特率的标志
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void rcc_set(u8 vl)
{
	if((vl>16))return;
	vl|=0xa0;
	flash_read_write(1);
	sim_buf_red[0x18]=vl;
	//sim_buf_red[0x7E]=vl;
	flash_read_write(0);	
}


/**************************************************************************
** 函数名称 ： date_limt
** 函数功能 ： 限制使用期
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
// void date_limt()
// {
// 	/*
//  	bkp8存储用户名
//  	bkp9存储到期时间
//  	bkp10存储当前时间
//  	*/
//     unsigned short r1,r2,r3,r4;
// 	scr_info.d_l=0;
// 	flash_read_write(1);
// 	r2=sim_buf_red[0x2a]|(sim_buf_red[0x2b]<<8);
// 	if(r2==0xffff)return;
// 	r3=(date32>>24)<<9;
// 	r4=(date32&0xff00)>>3;
// 	r1=r3|r4|(date32&0x1f);
// 	r2=sim_buf_red[0x2e]|(sim_buf_red[0x2f]<<8);
// 	if(r2<=r1||r2==0xffff){	 						//与当前日期比较
// 	sim_buf_red[0x2e]=r1;
// 	sim_buf_red[0x2f]=r1>>8;
// 	flash_read_write(0); 							//保存当前日期的值
// 	}
// 	else scr_info.d_l=1;							//与设定日期比较
// 	r2=sim_buf_red[0x2c]|(sim_buf_red[0x2d]<<8);
// 	if(r2<=r1)scr_info.d_l=1; 
// 		
// }

// /**************************************************************************
// ** 函数名称 ： date_limt_set
// ** 函数功能 ： 限制使用期设置
// ** 入口参数 ： 无
// ** 返回参数 ： 无
// ** 函数说明 ： 
// ** 修改日志 ： 刘晓琳 于2013年03月14日修改	
// **************************************************************************/
// /*  00 01 02 03     用户名0a 0b  锁定日期0c 0d  当前日期0e 0f */
// void date_limt_set()
// {
// 	u16 r1,r2;
// 	flash_read_write(1);
// 	r2=sim_buf_red[0x2a]|(sim_buf_red[0x2b]<<8);
// 	r1=buf[0]+buf[1]+buf[2]+buf[3];
// 	if((r2)==0xffff){
// 	sim_buf_red[0x2a]=r1;
// 	sim_buf_red[0x2b]=r1>>8;
// 	}
// 	else {
// 	if(r1!=r2)return;			//用户名不等则返回
// 	if(buf[14]==0){				//解锁则返回
// 	sim_buf_red[0x2a]=0xff;
// 	sim_buf_red[0x2b]=0xff;
// 	sim_buf_red[0x2e]=0xff;
// 	sim_buf_red[0x2f]=0xff;
// 	scr_info.d_l=0;
// 	flash_read_write(0);   
// 	return;
// 	}
// 	}
// 	r1=buf[16]<<9;
// 	r1|=buf[14]<<5;
// 	r1|=buf[13];
// 	sim_buf_red[0x2c]=r1;
// 	sim_buf_red[0x2d]=r1>>8;
// 	flash_read_write(0);
//}

/**************************************************************************
** 函数名称 ： timer_bright_check
** 函数功能 ： 检测定时调亮
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void timer_bright_check()
{
  unsigned char *p=(u8*)(0x0800FC05);
  unsigned short time,time2,i;
  if(*p!=0x88)return;
  p++;	time=time32>>8; //时间的分
  for(i=0;i<4;i++){
  time2=*p;
  time2<<=8;
  p++;
  time2|=*p;
  p++;
  if(time==time2){  //当调节亮度的时间到

	bright=0xff-*p;
	if(bright==0)bright=16;
	init_t4(bright);  //定时器14重置	 		
	flash_read_write(1); 						//0x29 存放亮度值
	sim_buf_red[0x29]=bright;
	flash_read_write(0);			
	break ;
 }
 p++;
 }	
}

/************************************************************************************************************
函数功能: 
*************************************************************************************************************/
/*内存地址
 *0x00-0x03 
 *0x06-0x12 亮度信息
 *0x18		频率 0x38-0x14
 *0x20-0x2f 配置信息
 */

/**************************************************************************
** 函数名称 ： timer_bright_set
** 函数功能 ： 定时调亮设置
** 入口参数 ： 无
** 返回参数 ： 无
** 函数说明 ： 
** 修改日志 ： 刘晓琳 于2013年03月14日修改	
**************************************************************************/
void timer_bright_set()										//定时调亮					
{
 	unsigned char i;
 	flash_read_write(1);
	for(i=6;i<18;i++)sim_buf_red[i]=buf[i];
	if(sim_buf_red[6]!=0xff)sim_buf_red[5]=0x88;
	else sim_buf_red[5]=0xff;
	sim_buf_red[4]=0xff;
	flash_read_write(0);

}

// /**************************************************************************
// ** 函数名称 ： id_read
// ** 函数功能 ： 读CPU的id号
// ** 入口参数 ： 无
// ** 返回参数 ： cpu的id号
// ** 函数说明 ： 
// ** 修改日志 ： 刘晓琳 于2013年03月14日修改	
// **************************************************************************/
// unsigned long id_read()
// {
// 	u32 CpuID[3];
// 	u32 Lock_Code;
//  													 	//获取CPU唯一ID
//  	CpuID[0]=*(vu32*)(0x1ffff7e8);
//  	CpuID[1]=*(vu32*)(0x1ffff7ec);
//  	CpuID[2]=*(vu32*)(0x1ffff7f0);
//  
//  	Lock_Code=(CpuID[0]>>3)+(CpuID[1]>>1)+(CpuID[2]>>2);//加密算法,很简单的加密算法
// 	return Lock_Code;
// }

// /**************************************************************************
// ** 函数名称 ： id_check
// ** 函数功能 ： 检测id号
// ** 入口参数 ： 无
// ** 返回参数 ： 无
// ** 函数说明 ： 
// ** 修改日志 ： 刘晓琳 于2013年03月14日修改	
// **************************************************************************/
// void id_check()
// {
// 	u32 id,r1;
// 	scr_info.lk=0; //锁定标志初始值为0
// 	id=id_read();

// 	r1=*(vu32*)(0x0800FC00);
// 	if(id!=r1)scr_info.lk=1;//如果id号不正确，则将锁定标志置位
// } 

// /**************************************************************************
// ** 函数名称 ： flash_deal
// ** 函数功能 ： 将id号存入内部flash中
// ** 入口参数 ： 无
// ** 返回参数 ： 无
// ** 函数说明 ： 
// ** 修改日志 ： 刘晓琳 于2013年03月14日修改	
// **************************************************************************/
// void flash_deal()
// {
// 	typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
// 	u32 StartAddr=0xF800;
// 	u32 id;
// 	FLASH_Unlock();		 //FLASH解锁

//   FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	//清标志位

// 	id=id_read();   //读取id号

// 	FLASH_ErasePage(StartAddr);//擦除内部flash中StartAddr开始的页
// 	FLASH_ErasePage(StartAddr+FLASH_PAGE_SIZE);//擦除内部flash中(StartAddr+FLASH_PAGE_SIZE)开始的页
// 	FLASH_ProgramWord(StartAddr, id);//将id号写入内部flash中StartAddr地址位
// 	FLASH_ProgramWord(StartAddr+FLASH_PAGE_SIZE, id);//将id号写入内部flash中(StartAddr+FLASH_PAGE_SIZE)的地址位
// }






