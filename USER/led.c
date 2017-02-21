#include "led.h"
#include "clock.h"
#include "stmlib.h"
#include  "hb.h"

u8 ledtime[14];//�����Ŀʱ��
//�汾��ȡģ
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
{0x01,0x00,0x01,0x00,0x01,0x00,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x08,0x21,0x08,0x21,0x08,0x3F,0xF8,0x21,0x0A,0x01,0x02,0x01,0x02,0x00,0xFE},/*"��",0*/
{0x00,0x00,0x3F,0xFE,0x20,0x00,0x20,0x80,0x20,0x80,0x20,0x80,0x20,0x80,0x2F,0xFC,0x20,0x80,0x20,0x80,0x20,0x90,0x20,0x88,0x20,0x88,0x40,0x80,0x5F,0xFE,0x80,0x00},/*"ѹ",1*/
{0x08,0x08,0x08,0x3C,0x0B,0xE0,0x12,0x20,0x12,0x20,0x32,0x20,0x32,0x20,0x53,0xFE,0x92,0x20,0x12,0x10,0x12,0x10,0x12,0x12,0x12,0x0A,0x12,0x8A,0x13,0x26,0x12,0x12},/*"��",2*/
};
const u8 lenerr[4][32]={//����Χ
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x08,0x00,0x09,0xFC,0x08,0x44,0x7E,0x44,0x08,0x44,0x08,0x94,0xFF,0x08,0x08,0xFC,
0x28,0x84,0x28,0x84,0x2E,0x84,0x28,0xFC,0x28,0x00,0x58,0x00,0x4F,0xFE,0x80,0x00},/*"?",2*/
{0x04,0x40,0x04,0x40,0xFF,0xFE,0x04,0x40,0x10,0x00,0x09,0xF8,0x49,0x08,0x21,0x08,
0x25,0x08,0x05,0x28,0x09,0x10,0x71,0x02,0x11,0x02,0x11,0x02,0x10,0xFE,0x00,0x00},/*"?",3*/
{0x00,0x00,0x7F,0xFC,0x41,0x04,0x41,0x04,0x5F,0xF4,0x41,0x04,0x4F,0xE4,0x41,0x04,
0x5F,0xF4,0x41,0x14,0x41,0x14,0x41,0x54,0x41,0x24,0x41,0x04,0x7F,0xFC,0x40,0x04},/*"?",4*/
};
const u8 scanerr[4][32]={//��������
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x00,0x40,0x40,0x40,0x27,0xFC,0x20,0x40,0x03,0xF8,0x00,0x40,0xE7,0xFE,0x20,0x00,
0x23,0xF8,0x22,0x08,0x23,0xF8,0x22,0x08,0x2B,0xF8,0x32,0x08,0x22,0x28,0x02,0x10},/*"?",2*/
{0x01,0x20,0x07,0xA0,0x3C,0x20,0x04,0x20,0x04,0x20,0x04,0x20,0x04,0x20,0xFF,0xFE,
0x04,0x20,0x04,0x20,0x04,0x20,0x08,0x20,0x08,0x20,0x10,0x20,0x20,0x20,0x40,0x20},/*"?",3*/
{0x10,0x00,0x13,0xFC,0x20,0x84,0x20,0x88,0x48,0x88,0xF8,0x90,0x10,0x9C,0x20,0x84,
0x41,0x44,0xF9,0x44,0x41,0x28,0x01,0x28,0x1A,0x10,0xE2,0x28,0x44,0x44,0x01,0x82},/*"?",4*/
};
const u8 proerr[4][32]={//У��ʧ��
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",2*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",3*/
{0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,
0x54,0x88,0x50,0x88,0x90,0x50,0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06},/*"?",4*/
{0x00,0x20,0xF8,0x20,0x08,0x50,0x48,0x50,0x48,0x88,0x49,0x04,0x4A,0xFA,0x7C,0x00,
0x04,0x44,0x04,0x24,0x1D,0x24,0xE4,0xA8,0x44,0x88,0x04,0x10,0x2B,0xFE,0x10,0x00},/*"?",5*/
};
const u8 longerr[4][32]={//��Ŀ��
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*" ",0*/
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",1*/
{0x08,0x20,0x08,0x20,0xFF,0xFE,0x08,0x20,0x08,0x20,0x00,0x00,0x7F,0xF8,0x02,0x08,
0x02,0x08,0x02,0x08,0x02,0x08,0x02,0x50,0x02,0x20,0x02,0x00,0x02,0x00,0x02,0x00},/*"?",2*/
{0x00,0x00,0x3F,0xF8,0x20,0x08,0x20,0x08,0x20,0x08,0x3F,0xF8,0x20,0x08,0x20,0x08,
0x20,0x08,0x3F,0xF8,0x20,0x08,0x20,0x08,0x20,0x08,0x20,0x08,0x3F,0xF8,0x20,0x08},/*"?",3*/
{0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0x01,0x00,0xFF,0xFE,0x01,0x00,0x01,0x00,
0x02,0x80,0x02,0x80,0x04,0x40,0x04,0x40,0x08,0x20,0x10,0x10,0x20,0x08,0xC0,0x06},/*"?",4*/
};
const u8 rtcerr[4][32]={//��Ŀ��

{0x10,0x40,0x10,0x20,0x10,0x20,0x11,0xFE,0xFC,0x00,0x10,0x88,0x31,0x04,0x3A,0x02,0x54,0x88,0x50,0x88,0x90,0x50,0x10,0x50,0x10,0x20,0x10,0x50,0x10,0x88,0x13,0x06},/*"У",0*/
{0x00,0x08,0x00,0x08,0x7C,0x08,0x44,0x08,0x45,0xFE,0x44,0x08,0x44,0x08,0x7C,0x08,0x44,0x88,0x44,0x48,0x44,0x48,0x44,0x08,0x7C,0x08,0x44,0x08,0x00,0x28,0x00,0x10},/*"ʱ",1*/
{0x01,0x00,0x11,0x00,0x11,0x00,0x11,0x00,0x3F,0xF8,0x21,0x00,0x41,0x00,0x01,0x00,0xFF,0xFE,0x02,0x80,0x04,0x40,0x04,0x40,0x08,0x20,0x10,0x10,0x20,0x08,0xC0,0x06},/*"ʧ",2*/
{0x00,0x40,0x7C,0x40,0x44,0x40,0x54,0x80,0x54,0xFE,0x55,0x08,0x56,0x88,0x54,0x88,0x54,0x88,0x54,0x50,0x54,0x50,0x10,0x20,0x28,0x50,0x24,0x88,0x45,0x04,0x82,0x02},/*"��",3*/
};
//��������
void (*led_play)(void);

//����ṹ�����
struct ScrStruct  scr_info; //�����Խṹ
struct AreaStruct area[8];  //�������Խṹ
struct AreaStruct *pArg;    //�������Խṹ
struct SimStruct sim;       //ģ��ʱ�����Խṹ
extern struct hbStruct hb;  //�������Խṹ

//����ȫ�ֱ���
unsigned short g_wShowVersionCnt = 0;
unsigned char g_ucChangeScan = 0;

unsigned char flashcheckerr=0;//��ʼֵ����
unsigned char screeniniterr=0,scan_play=0;//���γ��ȿ�ȣ���ɨ�����
unsigned char offset_green,pro_flag; //��ɫ���ݵ�ƫ��������Ŀ���ŵı�־
unsigned short screen_len;  //����(���ֽ�)
unsigned char area_num,area_addr,area_c=0,bright,ticks;	//����������������ַ�����������������ȡ�����
unsigned char sec_flag,pro_num,info_num,pro_c,offset=1; //�봦���־����Ŀ��������Ϣ��������Ŀ����������˫ɫƫ��
unsigned short datn16,show_timer; //���ũ�����������ż���
unsigned long time_sj=0,flash_addr,pro_addr;//���ʱ���ȡֵ������flash�ĵ�ַ����Ŀ�ĵ�ַ
unsigned short var;  //��������

unsigned char		mm_set_en;		//=1��������WIFI����
unsigned char		scr_onoff;		//��Ļ���ر�־  0��  1��

#define		INTERVAL_PATTERN_ADDR			0x22		//�����������
#define		TIME_ONOFF_ADDR			0x20		//ʱ�俪��
#define		DISP_PATTERN_ADDR		0x21		//��ʾģʽ
#define		BRIGHT_BG_ADDR			0x2a		//��������
#define		BRIGHT_WORD_ADDR		0x29		//��������


//#define		TIME_ONOFF_ADDR			0X84
//
//time_on_off   disp_Pattern   time_on_off  set_bright_hei

//��������
unsigned char leddata[8*1024];  //��ʾ��������
//unsigned char leddata[8*128];  //��ʾ��������

/* flash�Ƿ�Ϊ�ձ��  2013.3.18������ */
//static unsigned char g_ucisFlashEmpty = 0;

unsigned char buf[40];

//�����ⲿ����
extern unsigned char h,hx;
extern unsigned char dis_length;
extern unsigned long time32,date32;
extern unsigned short usBright;
extern u8 cardNum[cardLength];//

/***********ģ��ʱ��ȫ�ֱ���*****************/
unsigned char sim_buf_red[1024];		//��ɫ������ 
unsigned char sim_buf_green[1024];	//��ɫ������
#ifdef CONFIG_SEVEN_COLOR
 	unsigned char sim_buf_blue[1024];		//��ɫ������
#endif

/*
 *��ʾ����ѹ�͡�
 *13.5.31 ZZW
// */
void LEDPowerLow(void)
{
	u16 i,j;
		/* ��ѹ��ʱ�ر�FLASHƬѡ */
		OFF_CE;
// 		for(j=0;j<16;j++){
// 	  for(i = 0; i< 8; i++){
// 	  	leddata[(j + 1) * SCREEN_LEN - 6 + i] = PowerManager[i / 2][i % 2 + 2 * j];	
// 			}
// 		}
}

/**************************************************************************
** �������� �� delay
** �������� �� ��ʱ����
** ��ڲ��� �� ��ʱʱ��time
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��10���޸�	
**************************************************************************/
static void delay(unsigned long time){
	while(time--);
}


/**************************************************************************
** �������� �� flash_read_write
** �������� �� �ڲ�flash��д����
** ��ڲ��� �� ��д��־s
** ���ز��� �� ��
** ����˵�� �� s=1�����ڲ�flash�ж���0x50������д��sim_buf_red��
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void flash_read_write(u8 s)
{
	u8 *p=(u8*)(0x0800FC00),i;
	u32 r1,addr=0x0800FC00;
	if ( rPWR_CSR & 4 )return;
	if(s){	 	//����flash��ֵ��������
	for(i=0;i<0x80;i++) sim_buf_red[i]=*p++;
	}
	else {		 //������дflash
	FLASH_Unlock();	 //FLASH����
//    FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	//���־λ
	FLASH_ErasePage(addr); //��ҳ����
	for(i=0;i<0x80;i+=4){
	r1=sim_buf_red[i+3];
	r1<<=8;
	r1|=sim_buf_red[i+2];
	r1<<=8;
	r1|=sim_buf_red[i+1];
	r1<<=8;
	r1|=sim_buf_red[i];
	STMFLASH_WriteWord(addr, r1); //����0x50������д��flash��
	addr+=4;
	}
	scr_info.st=1; //��Ŀͣ����־
	pro_c=0;				
	}	

}

/*****************���Ž�Ŀ֮ǰ��ʾ�汾��12.11.14********************************/
/**************************************************************************
** �������� �� LEDShowVersion
** �������� �� ����ʾ������leddata��ֵΪ�汾��
** ��ڲ��� �� ȡģ����ĵ�index�����飬д��ĸ���n��д�뻺������λ��pos
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
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



static void LEDShowVersion(u8 index, u8 n, u8 pos)		//��ģλ�ã���ģ��ȣ�Ŀ��λ��
{
	u16 i,j;
	for(j=0;j<16;j++)
	  for(i = 0; i < n; i++){
		leddata[0x68+(j + 1) * screen_len - 8 + pos+i] = ucaShowVersion[index+i][j];	//��0x68��Ϊ����ʾ����  8*13=104
 	  }  
	GREEN_BEGIN
		for(j=0;j<16;j++)
		  for(i = 0; i < n; i++){
			leddata[(j + OFFSET_GREEN +1) * screen_len - 8 + pos+i] = ucaShowVersion[index+i][j];	
 		  } 		
	GREEN_END	
  
}
 
/**************************************************************************
** �������� �� LEDShowModifyDate
** �������� �� ����ʾ������leddata��ֵΪ�޸�����
** ��ڲ��� �� ȡģ����ĵ�index�����飬д��ĸ���n��д�뻺������λ��pos
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
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
** �������� �� ShowVersion
** �������� �� ��ʾ�汾��
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
// void ShowVersion(void)
// { 	
// 	LEDShowVersion(11, 1, 3);//��ʾ��V��
// 	LEDShowVersion(VISION/0x100%0x10, 1, 4);//��ʾ��1��
// 	LEDShowVersion(10, 1, 5);//��ʾ��.��
// 	LEDShowVersion(VISION/0x10%0x10, 1, 6);//��ʾ��2��
// 	LEDShowVersion(VISION%0x10, 1, 7);//��ʾ��3��(��ǰ�汾��)
// 	
// 	LEDShowModifyDate(MODIFYDATE/0X100000%0x10, 1, 0);
// 	LEDShowModifyDate(MODIFYDATE/0X10000%0x10, 1, 1);	
// 	LEDShowModifyDate(10, 1, 2);//��ʾ��.��
// 	LEDShowModifyDate(MODIFYDATE/0X1000%0x10, 1, 3);
// 	LEDShowModifyDate(MODIFYDATE/0X100%0x10, 1, 4);
// 	LEDShowModifyDate(10, 1, 5);//��ʾ��.��
// 	LEDShowModifyDate(MODIFYDATE/0X10%0x10, 1, 6);
// 	LEDShowModifyDate(MODIFYDATE%0x10, 1, 7);
// }

void ShowVersion(void)
{ 	
	LEDShowVersion(11, 1, 0);//��ʾ��V��
	LEDShowVersion(VISION/0x100%0x10, 1, 1);//��ʾ��1��
	LEDShowVersion(10, 1, 2);//��ʾ��.��
	LEDShowVersion(VISION/0x10%0x10, 1, 3);//��ʾ��2��
	LEDShowVersion(VISION%0x10, 1, 4);//��ʾ��3��(��ǰ�汾��)
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
** �������� �� clr_screen
** �������� �� ����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
// void clr_screen()
// {
//   unsigned short i,j;
// //	register u16 *p=(u16 *)0x40010c0c;//GPIOB������ݼĴ����ĵ�ַ
// 	register volatile unsigned long *p;
// 	p=( volatile unsigned long *)0x40010c10;
// 	SET_OE(!scr_info.OE);//��OE
// 	memset(leddata,0,sizeof(leddata));//����������0
// 	for(j=0;j<16;j++){
// 		for(i=0;i<2048;i++){
// 		*p = scr_info.data ? 0xf0800000 : 0xf000;//*p =scr_info.data ? 0 : 0xff00; //��������Ǹ���Ч����ÿ���㸳ֵΪ0����֮����ֵΪ1
// 		*p=0x80; 
// 		}  
// 		ROW_SCAN(j);//������ѡ
// //		HL_LC();      //����LC��ƽ
// 			i=10;
// 			GPIOA->BSRR=0x100;
// 			while(i--);
// 			GPIOA->BRR=0x100;
// 		}
// 	ROW_SCAN(0);//����ѡ
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
** �������� �� init_t4
** �������� �� ��ʼ����ʱ��4�����ڵ�������
** ��ڲ��� �� ��ʱ����ֵb
** ���ز��� �� ��
** ����˵�� �� ���ݲ�ͬ�������ò�ͬ������ֵ
** �޸���־ �� ������ ��2013��01��07���޸�	
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
** �������� �� defaultplay
** �������� �� �βγ�ʼ��
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2014.5.26 wwd
**************************************************************************/
void defaultplay(){}

/**************************************************************************
** �������� �� screen_init
** �������� �� �βγ�ʼ��
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������
**************************************************************************/

void screen_init(void)		
{
   unsigned short  S_64,S_48,S_32,D_64,D_48,D_32;//��˫ɫ��ͬ�߶ȶ�Ӧ�Ŀ��

	unsigned short wVirHigh = 0;

	led_play=defaultplay;

	 S_32 = MaxWidth/16;       //32��  ˫ɫ �������
	
   S_48 = MaxWidth*32/(48*16);  //48��  ˫ɫ  �������
	
   S_64 = MaxWidth*32/(64*16); //64��  ˫ɫ �������
	
   D_32 = MaxWidth/8;    //32�� ��ɫ �������
	
   D_48 = MaxWidth*32/(48*8);   //48��  ��ɫ  �������
	
   D_64 = MaxWidth*32/(64*8);    //64��  ��ɫ �������
	
	/*
if ((flashcheckerr==2))
		{

			memset(buf,0,10);   //���buf��������ǰ10���ֽ�

		scr_info.addr = 0x11;
		scr_info.width=0x02;
		scr_info.width<<=8;
		scr_info.width|=0;	 //����
		scr_info.high=0;
		scr_info.high<<=8;
		scr_info.high|=0x20;	 //����
		scr_info.dColor=0;	 //��˫ɫ
		scr_info.OE=1;		 //oe ����
		scr_info.data=0;	 //���ݼ���
		scr_info.scan=1;	 //ɨ�跽ʽ
		scr_info.hsel = 1;	 //��˳��
			if ( MaxWidth < 512 )//15.3.27
			{
					scr_info.width = MaxWidth;
			}
		}
		else if(flashcheckerr==1) //��У������������
		{
         read_buf(10,buf,7);  //��ȡ10�ֽ�����Ϣ�ŵ�buf
        scr_info.addr = 0x11;
		scr_info.width=0x02;
		scr_info.width<<=8;
		scr_info.width|=0;	 //����
		scr_info.high=0;
		scr_info.high<<=8;
		scr_info.high|=0x20;	 //����
		if ( MaxWidth < 512 )
		{
				scr_info.width = MaxWidth;
		}	


		 if(buf[5]>2)
        {
		 scr_info.dColor=0;	 //��˫ɫ
        }
		else
		{
        scr_info.dColor=buf[5];	 //��˫ɫ
		}

		 if(buf[6]>1)
        {
		scr_info.OE=1;		 //oe ����
        }
		else
		{
        scr_info.OE=buf[6];	 //��˫ɫ
		}
		 if(buf[7]>1)
        {
		scr_info.data=0;		 //oe ����
        }
		else
		{
        scr_info.data=buf[7];	 //��˫ɫ
		}
		
		 if(buf[8]>9)
        {
	//	scr_info.scan=1;		 //oe ����
					
	
		scr_info.dColor=0;	 //��˫ɫ
		scr_info.OE=1;		 //oe ����
		scr_info.data=0;	 //���ݼ���
		scr_info.scan=1;	 //ɨ�跽ʽ
		scr_info.hsel = 1;	 //��˳��
        }
		else
		{
        scr_info.scan=buf[8];	 //��˫ɫ
		}

		 if(buf[9]>16)				//��˳��
     {
				scr_info.hsel=1;		 
			}
		else
		{
        scr_info.hsel=buf[9];	 //����˳��
		}
 
		}
		else					*/														//��������
		{
//			flash_addr=0x07;	//����Ϣ��ʼ��ַ
			read_buf(10,buf,7);  //��ȡ10�ֽ�����Ϣ�ŵ�buf

		scr_info.addr = buf[0];
// 		scr_info.width=buf[1];
// 		scr_info.width<<=8;
// 		scr_info.width|=buf[2];	 //����
		scr_info.width=104;
// 		scr_info.high=buf[3];
// 		scr_info.high<<=8;
// 		scr_info.high|=buf[4];	 //����
		scr_info.high=32;	
		//scr_info.dColor=buf[5];	 //��˫ɫ
		scr_info.dColor=0;	 //��ɫ	
		scr_info.OE=buf[6];		 //oe ����
		scr_info.data=buf[7];	 //���ݼ���
		scr_info.scan=buf[8];	 //ɨ�跽ʽ
		scr_info.hsel = buf[9];	 //��˳��
		
		}
	

	
	if(scr_info.dColor==1){//˫ɫ
		offset=2;//�˱��������ݴ���ʱ���õ�����ɫ��˫ɫ������ƫ�Ʋ�һ��
	}else if(scr_info.dColor==0){  //��ɫ
		offset=1;
	}

	#ifdef CONFIG_SEVEN_COLOR	 //��ɫ
		//offset=3;
	#endif
	
	if(scr_info.width>MaxWidth)	 //���Ƴ��ߣ���ֹ�±�Խ�緢��Ӳ���쳣
	{
		screeniniterr=2;
		scr_info.width=MaxWidth;
		all_scall_face.all_scall_color=0;//˫ɫ
		all_scall_face.all_scall_screen_high=1; //���� �м�����	
	}
	if(scr_info.high>MaxHigh)    //�߶�����
	{
		screeniniterr=2;
		scr_info.high=MaxHigh;
		all_scall_face.all_scall_color=0;//˫ɫ
		all_scall_face.all_scall_screen_high=1; //���� �м�����	
	}
	//45ɨ��߶ȼ���
	if(((scr_info.scan ==4 )||(scr_info.scan ==5 )) && (scr_info.high > MaxHigh/2))
	{
		screeniniterr=2;
		scr_info.high = MaxHigh/2;
	}
 


	if (scr_info.width < 32)
	{
		screeniniterr=2;
		scr_info.width = 32; 
		all_scall_face.all_scall_color=0;//˫ɫ
		all_scall_face.all_scall_screen_high=1; //���� �м�����
	}
	if (scr_info.high < 8)
	{
		screeniniterr=2;
		scr_info.high = 8; 
		all_scall_face.all_scall_color=0;//˫ɫ
		all_scall_face.all_scall_screen_high=1; //���� �м�����
	}
/*----------------------------------------------------------*/		
	
	flash_addr=scr_info.addr;  
//  screen_len=scr_info.width/8; //�����ֽ���
	screen_len=13; //�����ֽ���
	offset_green=scr_info.high;  //��ɫ���ݵĸ߶�
	pro_num=flash_read(flash_addr);//��ȡ��Ŀ����
 	pro_c=0;

	scr_onoff = 0;
// 		scr_info.of = 0;
// 		flash_read_write(1);
// 		if(sim_buf_red[0x38] == 0xa0)  scr_info.of=0x01;		//�ػ�
// 		if((*(unsigned char*)0x0800FC38) == 0xa0)
// 		{
// 			scr_info.of = 1;
// 			flash_read_write(1);    
// 			sim_buf_red[0x38] = 0xff;
// 			flash_read_write(0);					
// 		}	
		
	bright=*(u8*)0x0800fC29;			//��ȡ����		
	scr_info.s_t=0;								//�������ű�־
	
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
** �������� �� read_buf
** �������� �� ��ȡflash�е����ݣ��������������ݷ��ڻ�����p��
** ��ڲ��� �� ��ȡ�ĸ���n�����ݴ�ŵĻ�����p����ȡ���ݵĵ�ַaddr
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
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
�ֲ�������
**************************************************************************/
unsigned char __didi;//�˱����ڱ�������data_deal������ʹ��

/**************************************************************************
** �������� �� program_deal
** �������� �� ��Ŀ��Ϣ��ʼ��
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void program_deal()
{
	  unsigned char i,save; 
 	  scr_info.sec=1;
	  sec_flag=0xff;
	  hb.flag=0;         //���߱�־
	  scr_info.text = 0;
	  scr_info.is_init_hb=1;
	  scr_info.s_t_end=0; //��־����
	  scr_info.st=0;
	  memset(leddata,0,8*1024);	
	 pro_str:
	 IWDG_Feed();//ι��
	  flash_addr = scr_info.addr+1+pro_c*4;	//��Ŀn��ַ
	  read_buf(4,buf,flash_addr);			//��4���ֽڵ�read_buf
	  pro_addr=(u32)buf[1];   //��Ŀ�Ĵ洢��ַ 
	  pro_addr<<=8;
	  pro_addr|=(u32)buf[2];
	  pro_addr<<=8;
	  pro_addr|=(u32)buf[3];
	  	 
	  area_num=flash_read(pro_addr);  //��������
	  __didi = ~(0xff<<area_num);

		for(i=0;i<area_num;i++){	  
			area[i].temp=21;
		}

	  pro_flag=flash_read(pro_addr+1);	//��Ŀ��־ 
	  //��2λ  00˳�򲥷� 01��ʱ���� 10��������  ��2λ 1�л���0�޻���  ��3λ 1�б�������0�ޱ������� ��4λ 1��������0����ͼƬ
  	  flash_addr=pro_addr+area_num*4+10;
	  save=flash_read(flash_addr);     //Ԥ���ֽ���
	  flash_addr=flash_addr+save+1;	  
	  if((pro_flag&0x03)==0x01){        //��ʱ����
	  	 if(show_timers())
             goto pro_sw;  	
	  }
	  else if((pro_flag&0x03)==0x02){	//��������
	  	 scr_info.s_t=1; 
		 show_timer=flash_read(flash_addr);
		 show_timer <<= 8; spisd;
		 show_timer |= SPI1->DR;   //�������ŵĳ���
	  }
	  if(pro_flag&0x04){  //���ߴ���
		  //hb.flag=1;      //���߱�־ 					���û���������֮ǰ��ֹ���߹���
			flash_addr=pro_addr+2+4*area_num; //���ߵ�ַ�洢��ַ
			read_buf(4,buf,flash_addr);
			flash_addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
			read_buf(7,buf,flash_addr);	 
			hb.style = buf[0];//������ʽ
			hb.color = buf[1];//������ɫ
			hb.width = buf[2];//���߳���
			hb.width /=8;     //���߳���(�ֽ���)
			hb.high = buf[3]; //���߸߶�
			//hb.high = 0; //���߸߶�
			hb.move_way = buf[4];	//��ת����

			if(hb.move_way==3){
				hb.move_speed=buf[5]*12; //�ƶ��ٶ�
			}
			else hb.move_speed = buf[5]*4;	//��ת�ٶ�
			hb.c_speed=7; 			//��ǰ�ٶ�	 
			hb.move_step = buf[6];  //�ƶ�����
			flash_addr+=7;      //���ߵ����ַ
			hb.addr=flash_addr;
	  }
	  area_deal();	//��������					
	  return ;
	pro_sw:
		IWDG_Feed();//ι��
		pro_c++;      //��Ŀ�Լӣ���ʾ��һ����Ŀ
		pro_c %= pro_num;
		goto pro_str; 	
}

/**************************************************************************
** �������� �� info_init
** �������� �� ��Ϣ��ʼ��
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void info_init(struct AreaStruct *pAr)
{
	 unsigned long addr;
	 if((pAr->style>1)&&pAr->st)return;
	 pAr->sj=0;
	 pAr->sj_cnt=0;
	 pAr->cnt=0;

	 flash_addr=pAr->addr+13+4*pAr->info_c;  //��Ϣ���Ե�ַ��ŵĵ�ַ 
	 read_buf(4,buf,flash_addr);
	 addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
	 read_buf(13,buf,addr);
	 pAr->info_mode = buf[0];  //���з�ʽ							 

	 pAr->info_speed=buf[1];   //�����ٶ�
	
	 /* �����ƶ�̫�죬���Ϊ�����ƶ��ٶ�ֵ�͵���� 2013.3.20ZZW */
	 if ( (pAr->info_mode == 17)
		 || (pAr->info_mode == 18) 
	   || (pAr->info_mode == 13)
	   || (pAr->info_mode == 14) )
		{
			pAr->info_speed *= 2;
		}
   /***************************end*******************************/	
	
 	 pAr->info_c_stand=pAr->info_standtime=buf[2]; //ͣ��ʱ��
	 
	 pAr->info_data_w=(unsigned short)buf[5]<<8 | (unsigned short)buf[6]; //��Ϣ����
	 pAr->info_data_h=(unsigned short)buf[7]<<8 | (unsigned short)buf[8];	//��Ϣ���
	 pAr->info_content_addr =(unsigned long)buf[10]<<16 |(unsigned long)buf[11]<<8 | (unsigned long)buf[12];//���ݴ�ŵ�ַ

	 pAr->info_end=0;      //��Ϣ������־
	 pAr->info_data_w /=8; //��Ϣ�����ֽ���
	
	 if(pAr->info_standtime==0)
     {
         pAr->st_end=1;//ͣ��ʱ��Ϊ0����˵��ͣ��ʱ�䵽
     }

	 if(pAr->info_mode==2){ //���ٴ��
	 	pAr->sj=1;
	 	pAr->info_mode=pAr->temp;
	  }		
}

/**************************************************************************
** �������� �� area_deal
** �������� �� ��������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void area_deal()
{
 	 struct AreaStruct *pAr;
	 flash_addr=pro_addr; //�������Ե�ַ
 	 area_num=flash_read(flash_addr);  //�������� 
	 for(area_c=0;area_c<area_num;area_c++){//��������
		  pAr=&area[area_c]; //��ǰ���������Ե�ַ
 		  flash_addr=pro_addr+2+4*area_c; //�������Ե�ַ��ŵĵ�ַ
		  pAr->addr = flash_read(flash_addr);
		  pAr->addr <<= 8; spisd;
		  pAr->addr |= SPI1->DR; 
		  pAr->addr <<= 8; spisd;
		  pAr->addr |= SPI1->DR;
		  pAr->addr <<= 8; spisd;
		  pAr->addr |= SPI1->DR; 
		
		  flash_addr = pAr->addr;  //�������Ե�ַ
		  read_buf(13,buf,pAr->addr);
		
		  pAr->style = buf[0];  //��Ϣ���  
		  pAr->screen_w = ((unsigned short)buf[1]<<8 | (unsigned short)buf[2]);//��ʾ��ߴ�  
		  pAr->screen_h = ((unsigned short)buf[3]<<8 | (unsigned short)buf[4]);
		  pAr->place_w = ((unsigned short)buf[5]<<8 | (unsigned short)buf[6]); //��ʾλ��	  
		  pAr->place_h = ((unsigned short)buf[7]<<8 | (unsigned short)buf[8]);
		  pAr->info_number = buf[9];  //��Ϣ���� 
 		  pAr->area_hb_flag = buf[10];//��������
		  pAr->area_back = buf[11];    //��������
		  pAr->info_c=0;  //������Ϣ������0 
		  pAr->st=0;      //����ͣ����־��0   
		  pAr->st_end=0;  //ͣ��������־��0
		  pAr->end=0;     //����������־��0
		 
		  pAr->speed_hb_c=10; //���������ٶ�Ϊ�̶�ֵ10
		  pAr->is_init_sim=1; //ģ��ʱ�ӳ�ʼ�����
		  pAr->is_init_dig=1; //����ʱ�ӳ�ʼ�����
		  pAr->is_init_count=1;//��ʱ��ʼ�����
		  pAr->is_init_datn=1;//ũ����ʼ�����
		  pAr->place_w=pAr->place_w/8;
		  pAr->place_w=scr_info.width/8-pAr->place_w; //��ʾλ�õĺ�����
		  pAr->screen_w=pAr->screen_w/8;//��ʾ�ĳ���(�ֽ���)

		  pAr->parameter = 0;
		
		  dis_length=scr_info.width/8;//��Ŀ�ĳ���(�ֽ���)
		  info_init(pAr);//��Ϣ��ʼ��
	  }
}

/**************************************************************************
** �������� �� write_hsel
** �������� �� дһ�����ݵ��Դ�
** ��ڲ��� �� ����r1��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void write_hsel(unsigned char r1,unsigned long addr)
{
	unsigned short i;
	register unsigned short len;
	unsigned char place_w=pArg->place_w;  			   
	unsigned short screen_w=pArg->screen_w;

	//��ɫ����
	flash_read(flash_addr);
	len=r1*SCREEN_LEN;  //�Ѿ������ĵ�������� 
	for(i=place_w;i<place_w+screen_w;i++){ 
		leddata[i+len]=srs();		 //����Ŀ�ĵ������ݴ��뻺���� 			  
	}
	GREEN_BEGIN    //��ɫ���� 
		r1+=OFFSET_GREEN;
		len=r1*SCREEN_LEN;
		flash_read(addr);	
		for(i=place_w;i<place_w+screen_w;i++){ 
			leddata[i+len]=srs();
		}
	GREEN_END 
	#ifdef CONFIG_SEVEN_COLOR //��ɫ����
		r1+=OFFSET_GREEN;
		len=r1*SCREEN_LEN;
		flash_read(flash_addr+BLUE_DATA);
		for(i=place_w;i<place_w+screen_w;i++){
			leddata[i+len]=srs();
		}
	#endif
}

/**************************************************************************
** �������� �� getbit
** �������� �� ����ĳһλ��ֵ
** ��ڲ��� �� ��nλ
** ���ز��� �� ���nС��8���򷵻ش�λΪ1ʱ��������֮������0
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
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
** �������� �� clr_buf
** �������� �� �建����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void clr_buf()
{
	unsigned short i;
	unsigned char j,place_h,screen_h,place_w;
	unsigned short temp,screen_w,len=SCREEN_LEN,size_green=len*OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned short size_blue=len*OFFSET_BLUE;
	#endif
	place_h=pArg->place_h;  //��Ŀ����ʼ������
	screen_h=pArg->screen_h;//��Ŀ�ĸ�
	screen_w=pArg->screen_w;//��Ŀ�ĳ�
	place_w=pArg->place_w;  //��Ŀ����ʼ������
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
** �������� �� still_display
** �������� �� ��ֹ��ʾ
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void still_display()
{ 
	unsigned short i;
	unsigned char j,place_h=pArg->place_h,screen_h=pArg->screen_h,place_w=pArg->place_w;
		   		  
	unsigned short temp,screen_w=pArg->screen_w,len=SCREEN_LEN,size_green=len*OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned short size_blue=len*OFFSET_BLUE;
	#endif
			
	flash_addr=pArg->info_content_addr;	 //��Ϣ���ݵ�ַ							
	flash_read(flash_addr);
	for(j=place_h;j<screen_h+place_h;j++){ //����ɫ����д��leddata��
		temp=j*len;
		for(i=place_w;i<place_w+screen_w;i++){ 
			leddata[i+temp]=srs(); 
		} 
	}	
	GREEN_BEGIN  //����ɫ����д��leddata��
		flash_addr = pArg->info_content_addr+GREEN_DATA;
		flash_read(flash_addr);
		for(j=place_h;j<screen_h+place_h;j++){
			temp=j*len+size_green;
			for(i=place_w;i<place_w+screen_w;i++){
				leddata[i+temp]=srs(); 
			} 
		}
	GREEN_END 
	#ifdef CONFIG_SEVEN_COLOR //����ɫ����д��leddata��
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
	pArg->st=1;  //��ǰ��������ͣ����־
	if(pArg->cnt==pArg->info_data_h){ //��ʾ���е��ڽ�Ŀ�ĸ�
		 pArg->cnt=0;
		 pArg->st=1;
		 pArg->info_end=1;//��ǰ��Ŀ��ʾ���־
	 	 return;
	}
	 pArg->info_content_addr+=screen_w*screen_h*offset;	//��Ϣ��ַָ���¸���Ϣ�ĵ�ַ 	 	 
}

/**************************************************************************
** �������� �� pull_up_down_screen
** �������� �� �ϣ�����Ļ/��������	
** ��ڲ��� �� ��Ļ�ķ���ch����Ļ������ı�־ls
** ���ز��� �� ��
** ����˵�� �� ch=1������Ļ����֮������Ļ��ls=1Ϊ���䣬��֮Ϊ��Ļ
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void pull_up_down_screen(unsigned char ch,unsigned char ls)	//ch=1������Ļ
{
	unsigned char h_sel,j;
	unsigned long hsel_addr;
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h;
	unsigned short screen_w=pArg->screen_w;   
	unsigned char param=pArg->parameter;   		   
	if(param==screen_h){//�����������Ϊ��Ŀ�ĸ߶ȣ���˵��һ��������ʾ��
		pArg->cnt++; //�����Լ�1
		pArg->st=1;  //��Ŀ����ͣ����־
		pArg->parameter=0;//������Ϊ0
		pArg->info_content_addr+=screen_w*screen_h*offset;
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){	  
			pArg->cnt=0;
			pArg->info_end=1;
		}
		return; 
	}
	if(param==0)clr_buf(); // һ�������ݿ�ʼ��ʾǰ�����������   
	if(ch)h_sel=place_h+screen_h-param-1;//�����һ����������Ļ
	else h_sel=param+place_h; 	//�ӵ�һ����������Ļ
	flash_addr=pArg->info_content_addr+screen_w*(h_sel-place_h); //��ɫ���ݴ�ŵ��׵�ַ
	hsel_addr = pArg->info_content_addr+screen_w*(h_sel-place_h)+GREEN_DATA; //��ɫ���ݴ�ŵ��׵�ַ 
	if(ch){ //����
		if(ls)//����
			for(j=h_sel;j>place_h;j--)
				write_hsel(j,hsel_addr); //����������д�뻺����
		else  //��Ļ
			write_hsel(h_sel,hsel_addr);
	}else{//����
		if(ls)//����
			for(j=h_sel;j<screen_h+place_h;j++)
				write_hsel(j,hsel_addr); 
		else //��Ļ
			write_hsel(h_sel,hsel_addr);
	} 
	pArg->parameter++;
}

/**************************************************************************
** �������� �� clear
** �������� �� �����������ĳһ��
** ��ڲ��� �� ��len���ֽ�
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void clear(unsigned short len)
{
	unsigned char j;
  	for(j=pArg->place_h;j<pArg->screen_h+pArg->place_h;j++){
	 	leddata[j*SCREEN_LEN+len]=0;
	}
}

/**************************************************************************
** �������� �� pull_left_right_screen
** �������� �� ����,������Ļ	/��,������
** ��ڲ��� �� ��Ļ�ķ���ch����Ļ������ı�־ls
** ���ز��� �� ��
** ����˵�� �� ch=1���󣬷�֮���ң�ls=1Ϊ���䣬��֮Ϊ��Ļ
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void pull_left_right_screen(unsigned char ch,unsigned char ls) //ch=1������Ļ
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
	param=pArg->parameter;//��Ļ������Ĳ�������
	if(ls){//����
		if((pArg->area_hb_flag&0x01) || hb.flag){//����
			if(pArg->place_w==0){//�����ʼ��λ��Ϊ��Ļ�����
				bit_l=1;
				if(param==0)
					clear(0);//�����0���ֽڵ�һ��
			}
			if(len==pArg->place_w+screen_w){//�����ʼλ��Ϊ��Ļ���ұ�
				bit_r=1;
				if(param==0)
					clear(SCREEN_LEN-1);//������һ���ֽ����ڵ���
			}
		}
	}
	if(param==pArg->screen_w*8){//�����Ļ��������ĵ㹻һ��
		pArg->cnt++;	    //�����Լ�
		pArg->parameter=0;//�����Ĵ�����0		
		pArg->st=1;       //��Ŀ����ͣ����־
		
			pArg->info_content_addr+=screen_w*screen_h*offset;//��Ŀ�����ŵ��׵�ַ
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//�������Ϊ���ݵ����������ҷ��������
			pArg->cnt=0;     //��������0
			pArg->info_end=1;//��Ŀ����
		}
		return;  //����
	}
	if(!pArg->sj){//������������
		if(param==0){clr_buf();}//�����Ļ�ĵ�0�У������buf����
	}	 
	if(ch){	//����
		place_w=pArg->place_w+screen_w-1-param/8; //��Ļ���������ʼλ��Ϊ���Ҷ�
		flash_addr=pArg->info_content_addr+screen_w-1-param/8; //��Ŀ����ʼ��ַ 
		r_sel=getbit(7-param%8);//ÿ���ֽڴ���������ʾ
		if(ls){//����
			pos=place_w;//��ʼλ��
			_pos=pArg->place_w+bit_l;//����λ��Ϊ��Ļλ�õ���ʼλ�õ���һ���ֽڵ�λ��
		}
	}else{//����
		place_w=pArg->place_w+param/8;
		flash_addr=pArg->info_content_addr+param/8;//��Ŀ����ʼ��ַ
		r_sel=getbit(param%8);//ÿ���ֽڴ���������ʾ
		if(ls){ //����
			pos=pArg->place_w+screen_w-bit_r;	 //��ʼλ��Ϊ��Ŀ��ʾ�����λ�õ���һ���ֽڵ�λ��
			_pos=place_w+1;//����λ��Ϊ��Ŀ����λ�õ�ǰһ���ֽڵ�λ
		}
	} 
	size_green=len*OFFSET_GREEN;//��ɫ���ݵĸ���
	#ifdef CONFIG_SEVEN_COLOR
		size_blue=len*OFFSET_BLUE;//��ɫ���ݵĸ���
	#endif
	for(j=place_h;j<screen_h+place_h;j++){
		temp=j*len;
		//��ɫ����
		d=flash_read(flash_addr);
		leddata[temp+place_w] &= ~r_sel;
		leddata[temp+place_w] |= d&r_sel;
		if(ls){//����
			for(i=_pos;i<pos;i++)
				leddata[temp+i] = ((d&r_sel)?0xff:0); //��ʾ����һλ����Ϊ1ʱ����ֵΪ0xFF����֮��ֵΪ0    
		}
		GREEN_BEGIN //��ɫ����
			 d=flash_read(flash_addr+GREEN_DATA);
			 leddata[temp+size_green+place_w] &= ~r_sel;
			 leddata[temp+size_green+place_w] |= d&r_sel;
			 if(ls){//����
				 for(i=_pos;i<pos;i++)
					leddata[temp+size_green+i] = ((d&r_sel)?0xff:0);	 
			 }
		GREEN_END 
		
		#ifdef CONFIG_SEVEN_COLOR //��ɫ����
			d=flash_read(flash_addr+BLUE_DATA);
			leddata[temp+size_blue+place_w] &= ~r_sel;
			leddata[temp+size_blue+place_w] |= d&r_sel;
			if(ls){
				for(i=place_w+1;i<place_w+screen_w-pos;i++)
					leddata[temp+size_blue+i] = ((d&r_sel)?0xff:0);  
			}
		#endif	
		flash_addr+=pArg->screen_w; //��ֵ֮�󣬵�ַ��������֮�����һ�е�����
	} 
	pArg->parameter++;//ÿ����ʾ֮����Ļ��������Ĵ�����һ
}

/**************************************************************************
** �������� �� closed_down_screen
** �������� �� ���±պ�
** ��ڲ��� �� �պϵķ���ch
** ���ز��� �� ��
** ����˵�� �� ch=1���±պϣ�����r2Ϊ1Ϊ�ϰ�������֮Ϊ�°���
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void closed_down_screen(unsigned char ch)  //ch=1���±պ�
{
	unsigned char r2=1,place_h=pArg->place_h,screen_h=pArg->screen_h;
	unsigned short r1,screen_w=pArg->screen_w,param=pArg->parameter;
	unsigned long hsel_addr;
	if(param==pArg->screen_h){//�պϲ����Ĵ���Ϊ����
		pArg->cnt++;	//�����Լ�1
		pArg->st=1;	  //ͣ����־��1 
		pArg->parameter=0;//ÿ����ʾ�����������0
		pArg->info_content_addr+=screen_w*screen_h*offset;//��Ŀ��ŵĵ�ַΪ��һ����Ŀ��ŵ��׵�ַ
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//�������Ϊ���ݵ����������ҷ��������
			pArg->cnt=0;      //������0
			pArg->info_end=1; //��Ŀ����
		}
		return; //����
	}
	if(param==(ch?0:screen_h/2))clr_buf(); //���±պ�ʱ����0�β���ʱ���建������ 
	//���䣺ch=1��Ϊ���±պϣ���ʱÿ����Ҫ�����Ĵ���Ϊ���ߵ�һ�� 
	 
	
	display:		
	if(r2)r1=place_h+screen_h-param-1;//�°�����β�е�λ��
	else r1=param+place_h;//�ϰ���������λ��
	flash_addr=pArg->info_content_addr+screen_w*(r1-place_h);//��ɫ���ݵ���ʼλ��
	hsel_addr=flash_addr+GREEN_DATA;//��ɫ���ݵ���ʼλ��
	write_hsel(r1,hsel_addr);	//��r1��д����
	
	if(r2){//����°�����ʾ��֮�󣬽�r2��0����ʾ�ϰ���
		r2=0;goto display;
	}
	pArg->parameter++;  //ÿ����ʾ����ʾ������1 
}

/**************************************************************************
** �������� �� down_on_screen
** �������� �� ���¶Կ�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void down_on_screen() 
{
	if(pArg->parameter==0)pArg->parameter=pArg->screen_h/2;			
	closed_down_screen(0);
}

/**************************************************************************
** �������� �� left_right_close
** �������� �� ���ұպ�
** ��ڲ��� �� �պϵı�־ch
** ���ز��� �� ��
** ����˵�� �� ch=0�����ұպϣ�ch=1�����ҶԿ�
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void left_right_close(unsigned char ch)	//ch=1 ���ҶԿ�	
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
	unsigned short DATA=pArg->screen_w*screen_h; //ÿ����ɫ�����ݵĸ���					  	

	if(pArg->parameter==(ch?screen_w:screen_w/2)){ //ch=0�����ұպϣ���������screen_w/2ʱ��˵��������һ������ 
		//���䣺ch=1�����ҶԿ�����ʱ��Ҫ��������ʼֵΪscreen_w/2����ֵֹΪscreen_w  	
	  pArg->cnt++;        //������1
		pArg->parameter = 0;//������0	 	
		pArg->st=1;         //ͣ����־��1
		
 		pArg->info_content_addr+=pArg->screen_w*screen_h*offset; //��Ŀ�ĵ����ŵ�ַָ����һ����Ŀ���׵�ַ
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//�����Ŀ������Ϊ���ݵ����������ҷ��������
			pArg->cnt=0;     //��������0
			pArg->info_end=1;//��Ŀ������־��λ   
		}
		return;//����
	}
	if(pArg->parameter==(ch?screen_w/2:0)){clr_buf();}//ch=1,Ϊ���ҶԿ�����ʼʱλ��Ϊ�м䣬��ʱ�������
	                                                   //ch=0,Ϊ���ұպϣ���ʼλ��Ϊ0�У���ʱ�������
	
	display:
	if(r2){	//r2=1Ϊ�Ұ���������
		place_w=pArg->place_w+pArg->screen_w-pArg->parameter/8-1;//�Ұ������ұߵ�λ��											 
		flash_addr=pArg->info_content_addr+pArg->screen_w-1-pArg->parameter/8;
		r_sel=getbit(7-pArg->parameter%8);	//ÿ�β�����λ��������r_sel���ӵ͵���λ
	}else{
		place_w=pArg->place_w+pArg->parameter/8;//���������ߵ�λ��
		flash_addr=pArg->info_content_addr+pArg->parameter/8;
		r_sel=getbit(pArg->parameter%8);//ÿ�β�����λ��������r_sel���Ӹߵ���λ
	}
	size_green=OFFSET_GREEN*len+place_w;
	#ifdef CONFIG_SEVEN_COLOR
		size_blue=OFFSET_BLUE*len+place_w;
	#endif
	for(j=place_h;j<screen_h+place_h;j++){
		temp=j*len;
		//��ɫ����
		d=flash_read(flash_addr);
		leddata[place_w+temp] |= d&r_sel;
		GREEN_BEGIN//��ɫ����
			d=flash_read(flash_addr+DATA);
			leddata[temp+size_green] |= d&r_sel;
		GREEN_END
		#ifdef CONFIG_SEVEN_COLOR //��ɫ����
			d=flash_read(flash_addr+2*DATA);
			leddata[temp+size_blue] |= d&r_sel;
		#endif
		flash_addr+=pArg->screen_w; 	 
	} 
	if(r2){//�Ұ�����������ʾ��֮�󣬽�r2��0��Ȼ����ʾ�����������
		r2=0; goto display;	
	}
	pArg->parameter++;	 //ÿ�β���֮�󣬲����Լ�1 	 						
}

/**************************************************************************
** �������� �� left_right_open
** �������� �� ���ұպ�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void left_right_open()
{
	 if(pArg->parameter==0){
		  pArg->parameter = pArg->screen_w*8/2;//���ұպϵ���ʼ��Ϊ�����м�
	 }
	 left_right_close(1);
}

/**************************************************************************
** �������� �� V_blinds
** �������� �� ��ֱ��Ҷ
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
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
	
	if(pArg->parameter==8){//����Ϊ8˵��һ����Ϣ��Ҷ��
		pArg->cnt++;         //������1
		pArg->parameter=0;   //���Ĳ�����0
		pArg->st=1;          //ͣ��ʱ���־
		
 		pArg->info_content_addr+=screen_w*screen_h*offset;//��Ŀ�ĵ����ŵ�ַָ����һ����Ŀ���׵�ַ
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){  //�����Ŀ������Ϊ���ݵ����������ҷ��������
			pArg->cnt=0;      //������0
			pArg->info_end=1; //��Ŀ������־
		}
		return; //����
	}
	if(pArg->parameter==0){ //�������Ϊ0
		clr_buf(); //����������0
	}
	r2=screen_w;
	while(r2--){  //��ֱ��Ҷ��Ҷ��
		i=place_w+n;//��ֱ��Ҷ��ʼ��λ�ã�n�ǰ�Ҷ�Ĵ���			
		flash_addr=pArg->info_content_addr+n;//��ʾ�����ݵĵ�����׵�ַ
		n++; n%=screen_w;//�����ʾ����������һ�����������Ҷ
		r1=getbit(pArg->parameter%8);//�˴���ʾ����һλ
		for(j=place_h;j<screen_h+place_h;j++){//��ʾ��ʼ�����꿪ʼ
			temp=j*len;
			//��ɫ����
			d=flash_read(flash_addr);//����������Ϣ
			leddata[i+temp] |= d&r1; //��ʾ�����ĳһλ
			
			GREEN_BEGIN  //��ɫ����
				d=flash_read(flash_addr+GREEN_DATA);
				leddata[i+temp+size_green] |= d&r1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				d=flash_read(flash_addr+BLUE_DATA);
				leddata[i+temp+size_blue] |= d&r1;
			#endif
			flash_addr+=screen_w;  //ÿ�ΰ�Ҷ������һ��
		}
	}
	pArg->parameter++;  //���Ĳ����Լ�1
}

/**************************************************************************
** �������� �� H_blinds
** �������� �� ˮƽ��Ҷ
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
void H_blinds()//ˮƽ��Ҷ
{
	unsigned char j;
 	unsigned long hsel_addr;
	unsigned short screen_w=pArg->screen_w;
	unsigned char place_h=pArg->place_h,screen_h=pArg->screen_h;
	unsigned char param=pArg->parameter;//���Ĳ���
	if(param==8){ //���Ĳ���Ϊ8ʱ˵��һ�����ݰ�Ҷ���
		pArg->cnt++;//ÿ��ʾ��һ�����ݣ���ʾ�������Լ�1
		pArg->st=1; //��Ŀͣ��ʱ��
		pArg->parameter=0;//ÿ��ʾ��һ�������ݣ����Ĳ�����0
 
		pArg->info_content_addr+=screen_w*screen_h*offset;//��Ŀ�ĵ����ŵ�ַ
		if((pArg->cnt==pArg->info_data_h)&&(!pArg->sj)){//�����Ŀ������Ϊ���ݵ����������ҷ��������
			pArg->cnt=0;     //������0
			pArg->info_end=1;//��Ŀ������־
		}
		return; //����
	}
	if(param==0){clr_buf();}   //ÿ�ΰ�Ҷǰ������������0
	for(j=place_h+param;;j+=8){
		if(j>place_h+screen_h-1)break;//����кŴ������һ�У�������forѭ��
	
		flash_addr=pArg->info_content_addr+screen_w*(j-place_h);//��ɫ���ݵĵ�ַ
		hsel_addr=flash_addr+GREEN_DATA;//��ɫ���ݵĵ�ַ
		write_hsel(j,hsel_addr);//����j������д�뻺��
	}
	pArg->parameter++;	//���Ĳ����Լ�1	
}

/**************************************************************************
** �������� �� up_move
** �������� �� �����ƶ�
** ��ڲ��� �� ���Ƶı�־r2
** ���ز��� �� ��
** ����˵�� �� r2=0 ���ƣ���֮ ��ͣ��
** �޸���־ �� 2014��04��11���޸�	ZZW	
**************************************************************************/
void up_move(unsigned char r2)//�����ƶ�	 r2=0 ����
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
	
	if(pArg->sj){//�������
		DATA=screen_w*screen_h;//��ɫ���������
		data_w=screen_w;
	}else{  //���������
	 	DATA=pArg->info_data_w*pArg->info_data_h; //��ɫ���������
		data_w=pArg->info_data_w;
	}

	r=0x7f;_r=~r;//��໨�ߴ���ʱ��
	l=0xfe;_l=~l;	 	//�Ҳ໨�ߴ���ʱ��
	if(pArg->area_hb_flag&0x01){//�з�������(���߿�)
		ucStep=1;isBk = 1; //ZZW 14.4.11
		place_h=place_h+1;   //��ʼ��λ�ú���һλ
		screen_h=screen_h-2; //���������У�����������
	}
	if(hb.flag){  //�л���
		isHb = 1;
		if(place_w==0){//�����Ŀ��ʾ����ʼλ��Ϊ���
			r=0xff>>(hb.high+1);
		    _r=~r;
		}
		if(len==(place_w+screen_w)){//�������Ϊ��Ŀĩ��
			l=0xff<<(hb.high+1);
			_l=~l;
		}
	}		   
	if(r2){   //������
		if((param + ucStep) == pArg->screen_h*(pArg->cnt+1)){//һ���������ƶ���
			pArg->cnt++; //�����Լ�
			pArg->st=1;  //ÿ�������ƶ���֮��ͣ��һ��
			return;
		}	 
	}
	if(pArg->sj){ //�������
		if(param==screen_h){//���ŵ����һ��
			pArg->st=1;         //û��ͣ��
			pArg->parameter=0;  //ÿ�������ƶ��꽫��������0
			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//��Ŀ�ĵ�ַָ����һ�����ݵ���ʼ��ַ
 			return;
		}		
	}
	else if((param+ucStep)==pArg->info_data_h){//��������ţ����ƶ�����������
		pArg->parameter=0;  //����������0
		pArg->info_end=1;   //��Ŀ������־
		pArg->st=1;         //ͣ����־
		pArg->cnt=0;        //������0
		return;
	}


	for(j=place_h;j<screen_h+place_h-1;j++){//��ʾ�Ľ�Ŀ����ʼλ�ÿ�ʼ����(screen_h-1)��,ÿ����������дһ�е�����
	  if ( isBk || isHb )// ZZW 14.4.11
		{	
			//��ɫ����
			temp=j*len;
			save=leddata[temp+place_w]&_r;    //��໨�ߵ�λ��
			__save=leddata[temp+ps_len-1]&=_l;//�Ҳ໨�ߵ�λ��
			
			GREEN_BEGIN //��ɫ����
				temp=j*len+size_green;
				save1=leddata[temp+place_w]&_r;
				__save1=leddata[temp+ps_len-1]&=_l;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR  //��ɫ����
				temp=j*len+size_blue;
				save2=leddata[temp+place_w]&_r;
				__save2=leddata[temp+ps_len-1]&=_l;
			#endif
		}	
		
		for(i=place_w;i<ps_len;i++){//ÿ�����ݴ���ʼλ�ÿ�ʼ������Ϊps_len=screen_w+place_w
			//��ɫ����
			 temp= j*len;
			 leddata[i+temp]=leddata[i+temp+len];//����j�����ݴ��뻺����,�������ϱ�һ������������
			 
			 GREEN_BEGIN  //��ɫ����
				temp= j*len+size_green;
			 	leddata[i+temp]=leddata[i+temp+len];   
			 GREEN_END
			
			 #ifdef CONFIG_SEVEN_COLOR //��ɫ����
			 	temp=j*len+size_blue;
			 	leddata[i+temp]=leddata[i+temp+len];
			 #endif
		}
			if ( isBk || isHb )// ZZW 14.4.11
			{	
				//��ɫ����
				temp=j*len+place_w; //����ָ����е�����ߵ�һ���ֽ�
				leddata[temp]&=r;   //�����ݵ����λ 0x7f;
				leddata[temp]|=save;//���ݼ��ϻ���
				temp=j*len+ps_len-1;//����ָ����е����ұߵ�һ���ֽ�
				leddata[temp]&=l;   //�����ݵ����λ 0xfe;
				leddata[temp]|=__save;//���ݼ��ϻ���
				
				GREEN_BEGIN //��ɫ����
					temp=j*len+size_green+place_w;
					leddata[temp]&=r;
					leddata[temp]|=save1;
					temp=j*len+size_green+ps_len-1;
					leddata[temp]&=l;
					leddata[temp]|=__save1;
				GREEN_END
				
				#ifdef CONFIG_SEVEN_COLOR //��ɫ����
					temp=j*len+size_blue+place_w;
					leddata[temp]&=r;
					leddata[temp]|=save2;
					temp=j*len+size_blue+ps_len-1;
					leddata[temp]&=l;
					leddata[temp]|=__save2;
				#endif
		}
	}
	flash_addr=pArg->info_content_addr+param*data_w;//��Ŀ����һ�е����ݵ�λ��
	hsel_addr=flash_addr+DATA;//��ɫ���ݵ�λ��
	r1=screen_h+place_h-1;    //�к�
	write_hsel(r1,hsel_addr); //����������д�뻺��
	pArg->parameter++;
}

/**************************************************************************
** �������� �� down_move
** �������� �� �����ƶ�
** ��ڲ��� �� ���Ƶı�־r2
** ���ز��� �� ��
** ����˵�� �� r2=0 ���ƣ���֮ ��ͣ��
** �޸���־ �� 2014��04��11���޸�	ZZW
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

	if(pArg->sj){  //�������
		DATA=screen_w*screen_h; //��ɫ���������
		data_h=screen_h;//���ݵĸ�
		data_w=screen_w;//���ݵĳ�
	}else{//���������
		DATA=pArg->info_data_w*pArg->info_data_h;//��ɫ���������
		data_h=pArg->info_data_h;//���ݵĸ�
		data_w=pArg->info_data_w;//���ݵĳ�
	}
	r=0x7f;_r=~r; //��໨�ߴ���ʱ��
	l=0xfe;_l=~l; //�Ҳ໨�ߴ���ʱ��
	if(pArg->area_hb_flag&0x01){//�з�������(���߿�)
		ucStep=1;isBk = 1; // ZZW 14.4.11
		place_h=place_h+1;        //��ʼ��λ�ú���һλ
		screen_h=screen_h-2;	    //���������У�����������
	}
	if(hb.flag){ //�л���
		isHb = 1;
		if(place_w==0){  //�����Ŀ��ʾ����ʼλ��Ϊ���
			r=0xff>>(hb.high+1);
			_r=~r;
		}
		if(len==(screen_w+place_w)){ //�������Ϊ��Ŀĩ��
			l=0xff<<(hb.high+1);
			_l=~l;
		}			
	}
	ps_len=place_w+screen_w; //��Ŀ��ʾ���յ�λ��
 	if(r2){  //������
		if((param + ucStep) == pArg->screen_h*(pArg->cnt+1)){//һ���������ƶ���
			pArg->cnt++;  //�����Լ�
			pArg->st=1; 	//ÿ�������ƶ���֮��ͣ��һ��
			return;
		}
	}
	if(pArg->sj){ //�������
		if(param==screen_h){//���ŵ����һ��
			pArg->st=1;       //ÿ��ͣ��
			pArg->parameter=0;//ÿ�������ƶ��꽫��������0
 			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//��Ŀ�ĵ�ַָ����һ�����ݵ���ʼ��ַ
			return;
		} 	
	}
	else if((param+ucStep)==pArg->info_data_h){   //��������ţ����ƶ����������� 
		pArg->parameter=0;//����������0
		pArg->info_end=1; //��Ŀ������־
		pArg->st=1;       //ͣ����־
		pArg->cnt=0;      //������0
		return;	
	}


	for(j=place_h+screen_h-1;j>place_h;j--){//��ʾ�Ľ�Ŀ�����һ��λ�ÿ�ʼ����(screen_h-1)��,ÿ����������дһ�е�����
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//��ɫ����
			temp=j*len;//��j�еĵ�0������
			save=leddata[temp+place_w]&_r;   //��໨�ߵ�λ�� _r=0x80;
			__save=leddata[temp+ps_len-1]&_l;//�Ҳ໨�ߵ�λ�� _l=0x01;
			
			GREEN_BEGIN //��ɫ����
				temp=j*len+size_green;
				save1=leddata[temp+place_w]&_r;
				__save1=leddata[temp+ps_len-1]&_l;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				temp=j*len+size_blue;
				save2=leddata[temp+place_w]&_r;
				__save2=leddata[temp+ps_len-1]&_l;
			#endif
		}	
		
		for(i=place_w;i<ps_len;i++){//ÿ�����ݴ���ʼλ�ÿ�ʼ������Ϊps_len=screen_w+place_w
			//��ɫ����
			temp=j*len;
			leddata[i+temp]=leddata[i+(j-1)*len];//����j�����ݴ��뻺����,�������±�һ������������
			
			GREEN_BEGIN //��ɫ����
			leddata[i+temp+size_green]=leddata[i+(j-1)*len+size_green];
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
			leddata[i+temp+size_blue]=leddata[i+(j-1)*len+size_blue];
			#endif
		}
		if ( isBk || isHb )// ZZW 14.4.11
		{
			//��ɫ����
			temp=j*len+place_w;//����ָ����е�����ߵ�һ���ֽ�
			leddata[temp]&=r;    //�����ݵ����λ r=0x7f
			leddata[temp]|=save; //���ݼ��ϻ���
			temp=j*len+ps_len-1; //����ָ����е����ұߵ�һ���ֽ�
			leddata[temp]&=l;    //�����ݵ����λ l=0xfe
			leddata[temp]|=__save;//���ݼ��ϻ���
			
			GREEN_BEGIN //��ɫ����
				temp=j*len+size_green+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save1;
				temp=j*len+size_green+ps_len-1;
				leddata[temp]&=l;//0xfe;
				leddata[temp]|=__save1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				temp=j*len+size_blue+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save2;
				temp=j*len+size_blue+ps_len-1;
				leddata[temp]&=l;//0xfe;
				leddata[temp]|=__save2;
			#endif
		}	
	}

	flash_addr=pArg->info_content_addr+(data_h-1-param)*data_w; //��Ŀ����һ�е����ݵ�λ��
	hsel_addr=flash_addr+DATA; //��ɫ���ݵ�λ�� 
	r1=place_h;  //�к�  
	write_hsel(r1,hsel_addr);	 //����������д�뻺��
	pArg->parameter++;
}

/**************************************************************************
** �������� �� left_move
** �������� �� �����ƶ�
** ��ڲ��� �� ���Ƶı�־r2
** ���ز��� �� ��
** ����˵�� �� r2=0 ���ƣ���֮ ��ͣ��
** �޸���־ �� 2014��04��11���޸�	ZZW	
**************************************************************************/
void left_move(unsigned char r2) //r2=0 ����
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
	
	if(pArg->sj){ //�������
		DATA=screen_w*screen_h; //��ɫ���������
		data_w=screen_w;
	}else{ //���������
		DATA=pArg->info_data_w*pArg->info_data_h; //��ɫ���������
		data_w=pArg->info_data_w;
	}
	r=0x7f;_r=0x80; //��໨�ߴ���ʱ��
	l=0xfe;_l=0x01; //�Ҳ໨�ߴ���ʱ��
	bit=0x01; // ZZW 14.4.11
	if(pArg->area_hb_flag&0x01){ //�з�������(���߿�)
		ucStep = 1; 
		//isBk = 1;
		isBk = 0; 							//=======================================
		//bit = 0x02;// ZZW 14.4.11==========================================
		bit = 0x01;
		place_h=pArg->place_h+1;   //��ʼ��λ�ú���һλ
		//place_h=pArg->place_h; 
		screen_h=pArg->screen_h-2; //���������У�����������
	}

	ps_len=place_w+screen_w;   //���ݵ����ұߵ�λ��
	if(hb.flag){ //�л���
		//isHb = 1; // ZZW 14.4.11
		isHb = 0;
		bit=0x02;
		//bit=0x02; // ZZW 14.4.11================================================================
		if((len==(screen_w+place_w))&&((hb.high+1)<8)){  //�������ڽ�Ŀ��ʾ�����ұߵ�λ�ã����һ��ߵĸ߶�С��7
			l=0xff<<(hb.high+1);  //�Ҳ໨�ߴ���ʱ��
			//l=0xff<<(hb.high); 
			_l=~l;
			bit=1<<(hb.high+1);			
		}else if((len == (screen_w+place_w))&&(hb.high==7)){//�������ڽ�Ŀ��ʾ�����ұߵ�λ�ã����һ��ߵĸ߶ȵ���7
			ps_len=place_w+screen_w-1;
		}
		if((place_w==0)&&((hb.high+1)<8)){//��ʼλ��Ϊ����ߣ����һ��ߵĸ߶�С��7
			r=0xff>>(hb.high+1); //��໨�ߴ���ʱ��
			//r=0xff>>(hb.high); 
			_r=~r;			
		}else if((place_w==0)&&(hb.high==7)){//��ʼλ��Ϊ����ߣ����һ��ߵĸ߶ȵ���7
			place_w+=1;//��ʼλ���ƶ�һ���ֽ�
		}
	} 

	if(r2){ //������
		if((param + ucStep) == screen_w*8*(pArg->cnt+1)){ //һ���������ƶ���
			pArg->cnt++; //�����Լ�
			pArg->st=1;	 //ÿ�������ƶ���֮��ͣ��һ��
			return;
		}
	}
 
	if(pArg->sj){ //�������
		if(param==screen_w*8){//���ŵ����һ��
			pArg->st=1;          //ÿ��ͣ��
			pArg->parameter=0;    //ÿ�������ƶ��꽫��������0
 			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//��Ŀ�ĵ�ַָ����һ�����ݵ���ʼ��ַ
			return;
		}
	}
	else if(param + ucStep == pArg->info_data_w*8){ 	
		 pArg->parameter=0; //����������0
		 pArg->info_end=1;  //��Ϣ������־
		 pArg->st = 0;
			/* ADD BY ZZW 2013.5.7 */
		 if ( ((pro_num > 1) && (pArg->info_number > 1)) || (pArg->info_standtime > 0) )
		 {
				pArg->st = 1;
		 } 
		 pArg->info_c_speed = 0; /* ADD BY ZZW 2013.5.3 */
		 pArg->cnt=0;       //������0
		 return;
	}
	for(j=place_h;j<screen_h+place_h;j++){//��ʾ�Ľ�Ŀ����ʼλ�ÿ�ʼ����screen_h��,ÿ����������дһ�е�����
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//��ɫ����
			temp=j*len; //��j�еĵ�0������
			save=leddata[temp+place_w]&_r;//��໨�ߵ�λ�� r=0x7f;_r=0x80;

			GREEN_BEGIN	//��ɫ����											
				save1=leddata[temp+size_green+place_w]&_r;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				save2=leddata[temp+size_blue+place_w]&_r;
			#endif
		}
		for(i=place_w;i<ps_len-1;i++){//ÿ�����ݴ���ʼλ�ÿ�ʼ������Ϊps_len=screen_w+place_w ��ǰһ���ֽ�
			//��ɫ����
			temp=i+j*len;
			d1  = leddata[temp]<<1;   //ÿ�����������ƶ�һλ
			d2 = leddata[temp+1]&0x80;//��һ�����ݵ����λȡ������������һ�������ƶ�֮��
			d2>>=7;
			leddata[temp] = d1 | d2;
 
			GREEN_BEGIN //��ɫ����
			temp=i+j*len+size_green;
			d1  = leddata[temp]<<1;
			d2 = leddata[temp+1]&0x80;
			d2>>=7;
			leddata[temp] = d1 | d2;
 			GREEN_END	
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				temp=i+j*len+size_blue;
				d1  = leddata[temp]<<1;
				d2 = leddata[temp+1]&0x80;
				d2>>=7;
				leddata[temp] = d1 | d2;
 		 #endif
		}
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//��ɫ����
			temp=j*len+place_w;   //����ָ����е�����ߵ�һ���ֽ�
			leddata[temp]&=r;     //�����ݵ����λ r=0x7f
			leddata[temp]|=save;  //���ݼ��ϻ���
			
			GREEN_BEGIN //��ɫ����
				temp=j*len+size_green+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				temp=j*len+size_blue+place_w;
				leddata[temp]&=r;//0x7f;
				leddata[temp]|=save2;
			#endif
		}	
	}
 
	flash_addr=pArg->info_content_addr+isBk*data_w+param/8; //
	r1=getbit(param%8);	//�ƶ������ݵĵڼ�λ								 
	for(j=place_h;j<screen_h+place_h;j++){//ÿ�����ݴ���ʼλ�ÿ�ʼ������Ϊscreen_h
	 //��ɫ����	
		temp=j*len+ps_len-1;      //��j�е����һ���ֽڵ���
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			save=(leddata[temp]&_l);	//�Ҳ໨�ߵ�λ�õ���
			leddata[temp]&=l;         //�����ݵ����λ l=0xfe
			leddata[temp] = ((leddata[temp]<<1) | save);//���ݼ��ϻ���	
		}
		else
		{
			leddata[temp] <<= 1;
		}
		d=flash_read(flash_addr);
		if(d&r1){ //����ƶ�����һλ����Ϊ1
			leddata[temp]|= bit; 
		}
		
		GREEN_BEGIN //��ɫ����
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
		
		#ifdef CONFIG_BLUE_COLOR //��ɫ����
			temp=j*len+size_blue+ps_len-1;
			save=leddata[temp]&_l;
			leddata[temp]&=l;
			leddata[temp]=(leddata[temp]<<1)|save;
			d=flash_read(flash_addr+2*DATA);
			if(d&r1)leddata[temp] |= bit;
			//if(d&r1)leddata[temp] |= 0x01;
		#endif	
		flash_addr+=data_w; //ÿ��д������֮�󣬵�ַָ����һ�����ݵ���ʼλ��
	}
	pArg->parameter++; 
}

/**************************************************************************
** �������� �� right_move
** �������� �� �����ƶ�
** ��ڲ��� �� ���Ƶı�־r2
** ���ز��� �� ��
** ����˵�� �� r2=0 ���ƣ���֮ ��ͣ��
** �޸���־ �� 2014��04��11���޸�	ZZW	
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
	
	if(pArg->sj){ //�������
		DATA=screen_w*screen_h;//��ɫ���������
		data_w=screen_w;
	}else{//���������
		DATA=pArg->info_data_w*pArg->info_data_h;//��ɫ���������
		data_w=pArg->info_data_w;
	}
	l=0xfe;_l=0x01; //�Ҳ໨�ߴ���ʱ��
	r=0x7f;_r=0x80; //��໨�ߴ���ʱ��
	bit=0x80; // ZZW 14.4.11
	if(pArg->area_hb_flag&0x01){ //�з�������(���߿�)
		ucStep=1;isBk = 1; bit = 0x40;// ZZW 14.4.11=========================================================
		place_h=pArg->place_h+1;   //��ʼ��λ�ú���һλ
		screen_h=pArg->screen_h-2; //���������У�����������
	}

	ps_len=screen_w+place_w;//���ݵ����ұߵ�λ��
	if(hb.flag){    //�л���
		isHb = 1;// ZZW 14.4.11
		bit=0x40; // ZZW 14.4.11
		if((place_w==0)&&((hb.high+1)<8)){//�������ڽ�Ŀ��ʾ������ߵ�λ�ã����һ��ߵĸ߶�С��7
			r=0xff>>(hb.high+1); //��໨�ߴ���ʱ��
			_r=~r;
			bit=0x80>>(hb.high+1);			
		}else if((place_w==0)&&(hb.high==7)){//�������ڽ�Ŀ��ʾ������ߵ�λ�ã����һ��ߵĸ߶ȵ���7
			place_w+=1;  //��Ŀ����ʼ��ַ�����ƶ�һλ
		}
		if((len==(screen_w+pArg->place_w))&&((hb.high+1)<8)){ //��ʼλ��Ϊ���ұߣ����һ��ߵĸ߶�С��7
			l=0xff<<(hb.high+1); //�Ҳ໨�ߴ���ʱ��
			_l=~l;	
		}else if((len==(screen_w+pArg->place_w))&&(hb.high==7)){//��ʼλ��Ϊ���ұߣ����һ��ߵĸ߶ȵ���7
			ps_len=screen_w+pArg->place_w-1;//��Ļ����ֹ��ַΪ�����ұߵ����ڶ�λ
		}
	}
	

	if(r2){//������
		if((pArg->parameter+ucStep)==screen_w*8*(pArg->cnt+1)){ //һ���������ƶ��� 
			pArg->cnt++;//�����Լ�
			pArg->st=1; //ÿ�������ƶ���֮��ͣ��һ��
			return;
		}
	}
 	if(pArg->sj){ //�������
		if(pArg->parameter==screen_w*8){//���ŵ����һ��
			pArg->st=1;        //ÿ��ͣ��
			pArg->parameter=0; //ÿ�������ƶ��꽫��������0
			pArg->info_content_addr+=screen_w*pArg->screen_h*offset;//��Ŀ�ĵ�ַָ����һ�����ݵ���ʼ��ַ
			return;
		}
	}
	else if((pArg->parameter+ucStep)==pArg->info_data_w*8){//��������ţ����ƶ�����������
		pArg->parameter=0;  //����������0
		pArg->info_end=1;   //��Ŀ������־ 
		pArg->st = 0;         //ͣ����־
		 if ( ((pro_num > 1) && (pArg->info_number > 1)) || (pArg->info_standtime > 0) )
		 {
				pArg->st = 1;
		 } 			
		pArg->info_c_speed = 0; /* ADD BY ZZW 2013.5.3 */
		pArg->cnt=0;        //������0
		return;	 
	}

	for(j=place_h;j<screen_h+place_h;j++){//��ʾ�Ľ�Ŀ����ʼλ�ÿ�ʼ����screen_h��,ÿ����������дһ�е�����
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//��ɫ����
			temp=j*len; //��j�еĵ�0������
			save=leddata[temp+ps_len-1]&_l;//�Ҳ໨�ߵ�λ�� _l=0x01;
			
			GREEN_BEGIN //��ɫ����
				save1=leddata[temp+size_green+ps_len-1]&_l;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				save2=leddata[temp+size_blue+ps_len-1]&_l;
			#endif	
		}
		for(i=ps_len-1;i>place_w;i--){//ÿ�����ݴ�����ĩ�˿�ʼ������Ϊps_len=screen_w+place_w �������ݿ�ʼ��λ�ý���
			//��ɫ����
			temp=j*len;	
			d1=leddata[i+temp]>>1; //ÿ�����������ƶ�һλ
			d2=	leddata[i-1+temp]&0x01;//ǰһ�����ݵ����λȡ���������������ƶ�֮������λ
			d2<<=7;
			leddata[i+temp]=d1 | d2;
			
			GREEN_BEGIN  //��ɫ����
				temp=j*len+size_green;
				d1=leddata[i+temp]>>1;
				d2=	leddata[i-1+temp]&0x01;
				d2<<=7;
				leddata[i+temp]=d1 | d2;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				temp=j*len+size_blue;
				d1=leddata[i+temp]>>1;
				d2=	leddata[i-1+temp]&0x01;
				d2<<=7;
				leddata[i+temp]=d1 | d2;	
			#endif
		}
		if ( isBk || isHb )// ZZW 14.4.11
		{	
			//��ɫ����
			temp=j*len+ps_len;//����ָ����е����ұߵ�һ���ֽ�
			leddata[temp-1]&=l;   //�����ݵ����λ l=0xfe
			leddata[temp-1]|=save;//���ݼ��ϻ���
			
			GREEN_BEGIN //��ɫ����
				temp=j*len+size_green+ps_len;
				leddata[temp-1]&=l;//0xfe;
				leddata[temp-1]|=save1;
			GREEN_END
			
			#ifdef CONFIG_SEVEN_COLOR //��ɫ����
				temp=j*len+size_blue+ps_len;
				leddata[temp-1]&=l;//0xfe;
				leddata[temp-1]|=save2;
			#endif
		}	
	}
	flash_addr=pArg->info_content_addr+isBk*data_w+(data_w-pArg->parameter/8-1);
	r1=getbit(7-pArg->parameter%8); //�ƶ������ݵĵڼ�λ
	for(j=place_h;j<screen_h+place_h;j++){//ÿ�����ݴ���ʼλ�ÿ�ʼ������Ϊscreen_h
	 //��ɫ����	
		temp= place_w+j*len;//��j�еĵ�һ���ֽڵ���
		if ( isBk || isHb )
		{	
			save=leddata[temp]&_r; //��໨�ߵ�λ�õ��� _r=0x80;
			leddata[temp]&=r;      //�����ݵ����λ r=0x7f
			leddata[temp] = ((leddata[temp]>>1)|save);//���ݼ��ϻ���
		}
		else
		{
			leddata[temp] >>= 1;
		}		
		d=flash_read(flash_addr);	 
		if(d&r1){ //����ƶ�����һλ����Ϊ1
			leddata[temp] |= bit;//0x40; 
		 }
		 
		GREEN_BEGIN //��ɫ����
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
		 
		#ifdef CONFIG_BLUE_COLOR  //��ɫ����
			temp=place_w+j*len+size_blue;
			save=leddata[temp]&_r;
			leddata[temp]&=r;
			leddata[temp]=(leddata[temp]>>1)|save;
			d=flash_read(flash_addr+2*DATA);
			if(d&r1)leddata[temp]|=bit;
		#endif
		flash_addr+=data_w; //ÿ��д������֮�󣬵�ַָ����һ�����ݵ���ʼλ
	}
	pArg->parameter++; 
}
 
/**************************************************************************
** �������� �� datadeal_move
** �������� �� ��������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void datadeal_move()
	{ 
	unsigned short i; 
	unsigned short scr_len=SCREEN_LEN,screen_w=pArg->screen_w;
	unsigned char place_h=pArg->place_h,place_w=pArg->place_w,screen_h=pArg->screen_h;
	unsigned short j,data_h=pArg->info_data_h,DATA=screen_w*screen_h;
 
	if(pArg->area_hb_flag&0x01){//��������(���߿�)
		place_h=place_h+1;   //��Ŀ��ʼ��λ�������ƶ�һ�У������߿���
		screen_h=screen_h-2; //��Ŀ�ĸ߶ȼ�2
		i=1;
	}
	
	flash_addr=pArg->info_content_addr+pArg->parameter*screen_w*pArg->screen_h*offset;//��ǰ��������ʾ�����ݵ��׵�ַ
	pArg->parameter++;   //ÿ��ʾһ�����ݣ��������Լ�1
	if(pArg->parameter ==data_h){   //��ʾ��һ������
		pArg->parameter=0;  //ÿ��ʾ��һ�����ݣ���������0
		pArg->st=1;         //ͣ����־��ÿ��ͣ��һ��
		pArg->info_end=1;   //��Ŀ������־ 
		if(pArg->style==4)pArg->end=1; //��������������
	}
	//��ɫ����	
	flash_read(flash_addr);	 
	for(j=place_h;j<screen_h+place_h;j++){
 		for(i=place_w;i<place_w+screen_w;i++){
		leddata[i+j*scr_len]=srs();//�������ĵ�������д��leddata��

 		} 
 	}
	GREEN_BEGIN //��ɫ����
	flash_read(flash_addr+DATA);
	for(j=place_h;j<screen_h+place_h;j++){
 	 for(i=place_w;i<place_w+screen_w;i++){
	   leddata[i+(j+OFFSET_GREEN)*scr_len]=srs();
 	 }
 	}
	GREEN_END
	#ifdef CONFIG_BLUE_COLOR //��ɫ����
	flash_read(flash_addr+2*DATA);
	for(j=place_h;j<screen_h+place_h;j++){
 	 for(i=place_w;i<place_w+screen_w;i++){
	 	leddata[i+(j+OFFSET_BLUE)*scr_len]=srs();
 	 }
 	}
	#endif 
}

/**************************************************************************
** �������� �� datadeal_word
** �������� �� �ı�ͼƬ����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void datadeal_word() 
{
 if(pArg->info_mode>24){
 	datadeal_move();
 }else{
	 switch(pArg->info_mode){
	  case 1: 
	  	still_display();     	  //��ֹ��ʾ 
	   	break;
	  case 3: 
	  	pull_up_down_screen(1,0); //������Ļ
	   	break;
	  case 4:
	  	 pull_up_down_screen(0,0); //������Ļ
	   	break;
	  case 5: 
	  	pull_left_right_screen(1,0);     //������Ļ
	   	break;
	  case 6:
	  	pull_left_right_screen(0,0);     //������Ļ
	   	break;
	  case 7: 
	  	closed_down_screen(1);   //���±պ�
	   	break;
	  case 8: 
	  	down_on_screen();       //���¶Կ�
	   	break;
	  case 9: 
	  	left_right_close(0);      //���ұպ�
	   	break;
	  case 10: 
	  	left_right_open();       //���ҶԿ�
	   	break;
	  case 11: 
	  	V_blinds();    		  //��ֱ��Ҷ
	  	break;
	  case 12:
	  	H_blinds();			  //ˮƽ��Ҷ				  
	   	break;
	
	  case 13: 
	  	if(pArg->sj){up_move(0);break;}
	  	else{up_move(1);break;}			   //�����ƶ�
	   
	  case 14: 
	  	if(pArg->sj){
			down_move(0);
			break;
		}else{
			down_move(1); 
			break;
		}          //�����ƶ�
	   
	  case 15: 
	  	if(pArg->sj){
			left_move(0);
			break;
		}else{
			left_move(1);
			break;
		}			  //�����ƶ�
	   
	  case 16: 
	  	if(pArg->sj){
			right_move(0);
			break;
		}else{
			right_move(1);
			break;
		}		  //�����ƶ�
	   
	  case 17: 
	  	up_move(0);			  //��������
	   	break;
	  case 18: 
	  	down_move(0);		  //��������
	   	break;
	  case 19: 
	  	left_move(0);  //��������		  
	   	break;
	  case 20: 
	  	right_move(0);		  //��������
	  	break;
	  case 21: 
	  	pull_up_down_screen(1,1);   //��������
	   	break;
	  case 22: 
	  	pull_up_down_screen(0,1);  //��������   
	   	break;
	  case 23: 
	  	pull_left_right_screen(0,1);//��������	 
	  	break;
	  case 24: 
	  	pull_left_right_screen(1,1);//��������	 
	  	break;
	  default:  
	   break;
	 }
  }
 
}

/**************************************************************************
** �������� �� srs
** �������� �� ��flash�е�����
** ��ڲ��� �� ��
** ���ز��� �� ������flash�е�����
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
unsigned char srs()
{
	unsigned char i=spdats;	 //����*(u16*)0x4001300cָ��ļĴ����е�����
	spdelay;				 
	return i;
}

/**************************************************************************
** �������� �� PutPixel
** �������� �� ��һ����
** ��ڲ��� �� ��Ҫ���ĵ��λ�ã�������x��������y
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void PutPixel(unsigned short x,unsigned short y)    // x<1024  y<64
{
	unsigned char i;
	register unsigned short len=SCREEN_LEN;
	register unsigned short size_green=len*OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		register unsigned short size_blue=len*OFFSET_BLUE;
	#endif
	if(x>1024||y>64)return;//���Ƴ��ߣ�������1024��߳���64ʱ�����ء�
	i=x%8;     //���ֽ���
	i=0x80>>i; //��д��λ��
	if(pArg->info_mode&0x01)leddata[y*len+x/8]|=i;//��ɫ���ݣ���y�еĵ�xλ��1
	if(pArg->info_mode&0x02)leddata[y*len+size_green+x/8]|=i;//��ɫ����
	#ifdef CONFIG_SEVEN_COLOR//��ɫ����
		if(pArg->info_mode&0x04)leddata[y*len+size_blue+x/8]|=i;
	#endif				
}

/**************************************************************************
** �������� �� printf_line
** �������� �� ��ֱ��
** ��ڲ��� �� ��ʼλ�õ�����(x1,y1),�յ�λ�õ�����(x2,y2)
** ���ز��� �� ��
** ����˵�� �� �㷨�����ؼӹ��еĲ岹���е����ȽϷ�����
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void printf_line(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
       int dx,dy,e;
       dx = x2 -x1;  //ֱ����x�����ϵ�ӳ��
       dy = y2 -y1;  //ֱ����y�����ϵ�ӳ��
       if(dx >= 0)   //ֱ����x��������ӳ��
       {
              if(dy >= 0)  //ֱ����y��������ӳ��
              {
                     if(dx>=dy) // 1/8 octant
                     {
                            e=dy-dx/2; //�����ж�x��y��������ı�־��e>0��y�����������֮��x�������
                            while(x1<=x2)
                            {
                                   PutPixel(x1,y1);
                                   if(e>0){y1+=1;e-=dx;}//ÿ��һ���㣬��x��y�����1��ͬʱ����e��ֵ		
                                   x1+=1; //ÿ����һ���㣬x����ʼλ��������һλ
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
** �������� �� show_second
** �������� �� ��ʾ����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void show_second()
{
	unsigned char i,j,r5,r6,n=0;
	unsigned short r1,r2,r3,r4,x;
 	r1=pArg->place_w; 	   	//����x
	r3=pArg->screen_w; 				
 	if(pArg->area_hb_flag&0x01){//��������(�߿�)
		r2=pArg->place_h+1; //��ʼ����y
		r4=pArg->screen_h-2;//��Ŀ��ʾ�ĸ߶�
		n=1; //���߸߶�
	}else{	//�ޱ߿�		
		r2=pArg->place_h;  //��ʼ����y		 
		r4=pArg->screen_h; //��Ŀ��ʾ�ĸ߶�
		n=0; //���߸߶�
	}
	//��ɫ����
	r5=n;
	for(i=r2;i<r2+r4;i++,r5++){
		for(j=r1,r6=0;j<r1+r3;j++,r6++){
			leddata[j+i*SCREEN_LEN]=sim_buf_red[r5*16+r6];		 	
		}
	}
	GREEN_BEGIN //��ɫ����
		r5=n;
		for(i=r2;i<r2+r4;i++,r5++){
			for(j=r1,r6=0;j<r1+r3;j++,r6++){
				leddata[j+(i+OFFSET_GREEN)*SCREEN_LEN]=sim_buf_green[r5*16+r6];		 	
			}
		}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR //��ɫ����
		r5=n;
		for(i=r2;i<r2+r4;i++,r5++){
			for(j=r1,r6=0;j<r1+r3;j++,r6++){
				leddata[j+(i+OFFSET_BLUE)*SCREEN_LEN]=sim_buf_blue[r5*16+r6];		 	
			}
		}
	#endif
		
	i=time32;//��ȡ��ʱ����ֵ
	flash_addr=pArg->info_data_w+i*4;//��ʱ�����ŵĵ�ַ 
	flash_read(flash_addr);
 	x=pArg->place_w*8;//����(�������)
	j=pArg->place_h;  //����
	r1=srs()+x;
	r2=srs()+j;
	r3=srs()+x;
	r4=srs()+j;
	printf_line(r1,r2,r3,r4);//������ʱ����
	pArg->st=1;	 //ͣ����־
	pArg->end=1; //��������		
}

/**************************************************************************
** �������� �� datadeal_sim
** �������� �� ģ��ʱ�Ӵ���
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
void datadeal_sim()
{
	unsigned char h,m,n,i,d,color_ch,asc_vir,hz_vir,asc_size,hz_size,week_w,week_h,j,fg;
	unsigned char r1,r2,r3,r4,width;
	unsigned short green_data; //��ɫ���ݵĸ���
	#ifdef CONFIG_BLUE_DATA
		unsigned short blue_data;//��ɫ���ݵĸ���
	#endif
	
 	memset(sim_buf_red,0,1024);   //��������0
	memset(sim_buf_green,0,1024);
	#ifdef CONFIG_SEVEN_COLOR
		memset(sim_buf_blue,0,1024);
	#endif

	if(pArg->is_init_sim){ //ģ��ʱ�ӳ�ʼֵ   
		pArg->is_init_sim=0;
		flash_addr=	pArg->info_content_addr;
		read_buf(34,buf,flash_addr);
		sim.addr=(unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
		sim.h_color = buf[4];    //ʱ����ɫ
		sim.m_color = buf[5];    //������ɫ
	    sim.s_color = buf[6];  //������ɫ
	    sim.date_flag = buf[7];//���ڱ�־
	    sim.asc_w = buf[8];    //�ַ���
	    sim.asc_h = buf[9];    //�ַ���
	    sim.hz_w = buf[10];    //���ֳ�
	    sim.hz_h = buf[11];    //���ָ�
		sim.format_addr = (unsigned long)buf[13]<<16 | (unsigned long)buf[14]<<8 | (unsigned long)buf[15];//��ʽ��ַ
	    sim.asc_addr = (unsigned long)buf[17]<<16 | (unsigned long)buf[18]<<8 | (unsigned long)buf[19]; //�ַ���ַ
	    sim.hz_addr  = (unsigned long)buf[21]<<16 | (unsigned long)buf[22]<<8 | (unsigned long)buf[23]; //���ֵ�ַ
	    sim.date_place_x = (unsigned short)buf[24]<<8 | (unsigned short)buf[25]; //����λ�ú�����
	    sim.date_place_y = (unsigned short)buf[26]<<8 | (unsigned short)buf[27]; //����λ��������
	    sim.date_color = buf[28];//������ɫ
	    sim.week_place_x = (unsigned short)buf[29]<<8 | (unsigned short)buf[30]; //����λ�ú����� 
	    sim.week_place_y = (unsigned short)buf[31]<<8 | (unsigned short)buf[32]; //����λ��������
	    sim.week_color = buf[33];//������ɫ
		sim.week_place_x /= 8;
	  }
		if(sim.asc_w%8) asc_vir = sim.asc_w/8+1;
	    else asc_vir = sim.asc_w/8; //�ַ����ȵ��ֽ���
	    if(sim.hz_w%8) hz_vir = sim.hz_w/8+1;
	    else hz_vir = sim.hz_w/8;    //���ֵĳ��ȵ��ֽ���
	    asc_size = asc_vir*sim.asc_h;//�ַ��ĸ���
	    hz_size = hz_vir*sim.hz_h;   //���ֵĸ���
	 
	/********************************ʱ��*****************************************/
	flash_addr=sim.addr;
	h=time32>>16;  //ʱ
	if(h>12)h-=12;  //������ʾ����12����
	m=time32>>8;    //��
	h=h*5+m/12;     //ʱ����ʾ��λ��
	flash_addr+=h*4;
	read_buf(4,buf,flash_addr);
	flash_addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
	
	flash_read(flash_addr);	 //ʱ��
	r1=srs()/8;	 //����
	r2=srs();
	r3=srs()/8;	 //����
	r4=srs();
	if(sim.h_color&0x01){ //��ɫ
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){ 
				sim_buf_red[16*m+h]|=srs();	 
			}
		}
	}
	if(sim.h_color&0x02){ //˫ɫ
		flash_read(flash_addr+4);
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){
				sim_buf_green[16*m+h]|=srs();	
			}
		}	
	}
	#ifdef CONFIG_SEVEN_COLOR
		if(sim.h_color&0x04){ //��ɫ
			flash_read(flash_addr+4);
			for(m=r2;m<r2+r4;m++){
				for(h=r1;h<r1+r3;h++){
					sim_buf_blue[16*m+h]|=srs();	
				}
			}		
		}
	#endif
   /********************************����******************************/
	flash_addr=sim.addr;
	h=time32>>8;         //��
	flash_addr+=240+h*4;
	read_buf(4,buf,flash_addr);
	flash_addr = (unsigned long)buf[1]<<16 | (unsigned long)buf[2]<<8 | (unsigned long)buf[3];
	flash_read(flash_addr);	 //����
	r1=srs()/8;	  //����
	r2=srs();
	r3=srs()/8;	  //����
	r4=srs();
	if(sim.m_color&0x01){  //��ɫ
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){
				sim_buf_red[16*m+h]|=srs();
			}
		}
	}
	if(sim.m_color&0x02){  //˫ɫ
		flash_read(flash_addr+4);
		for(m=r2;m<r2+r4;m++){
			for(h=r1;h<r1+r3;h++){
				sim_buf_green[16*m+h]|=srs();
			}
		}		
	}
	#ifdef CONFIG_SEVEN_COLOR
		if(sim.m_color&0x04){  //��ɫ
			flash_read(flash_addr+4);
			for(m=r2;m<r2+r4;m++){
				for(h=r1;h<r1+r3;h++){
					sim_buf_blue[16*m+h]|=srs();
				}
			}		
		}
	#endif
	/*************************************����/��Ӱ**************************************/
    flash_addr=sim.addr;
	flash_addr+=240*3;
	flash_read(flash_addr);

	r3=pArg->screen_w; //���ĳ��ȣ��ֽ�����
	r4=pArg->screen_h; //���ĸ߶�
	for(m=0;m<r4;m++){
		for(h=0;h<r3;h++){
			sim_buf_red[16*m+h]|=srs();
		}
	}
	//GREEN_BEGIN
		green_data=pArg->screen_w*pArg->screen_h;//��ɫ���ݵĸ���
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
	/****************************ģ��ʱ������***********************************/
    width=sim.date_place_x;	 //����λ�õĺ����꼴���ڵĳ�
	if(sim.date_flag&0x01){	   //��ʾ����
		flash_addr=sim.format_addr; //��ʽ��ַ
		n=flash_read(flash_addr);   //��ʽ
		read_buf(n,buf,flash_addr+1);
		
		r2=n; 
		for(i=2;i<n;i++){
			if((buf[i]==0x6e)||(buf[i]==0x79)||(buf[i]==0x72)){//����������
				for(r1=i;r1<n;r1++)buf[r1]=buf[r1+1];  
				r2--;
			}
		}	
		i=0; scr_info.fg3=0;
		if(buf[1]==0x4d)scr_info.fg3=1;	 //�ַ���
		if(buf[2]==0x59){		//�ַ���
			flash_addr=sim.asc_addr+2*asc_size;	 
			color_ch=sim.date_color; //������ɫ
			__dig_write(width,sim.date_place_y,sim.asc_w,sim.asc_h,color_ch);
		    flash_addr = sim.asc_addr;//�ַ���ַ
			__dig_write(width+sim.asc_w,sim.date_place_y,sim.asc_w,sim.asc_h,color_ch);
			i+=2; width+=2*sim.asc_w;
		}  
	  	fg=0;
		scr_info.fg2=0;
		for(;i<r2;i++){
			d=asc_check(buf[i],fg);//fg=0��ȡ��λ fg=1��ȡ��λ
			if(scr_info.fg1){ //scr_info.fg1=1���� ??????
				 flash_addr=sim.hz_addr+d*hz_size;	fg=0;
				 if(sim.date_flag&0x08)color_ch=sim.week_color;//������ɫ
				 else color_ch=sim.date_color;//������ɫ  
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
	if(sim.date_flag&0x02){	 //��ʾ����
		i=flash_read(sim.format_addr);//�������ڵĸ�ʽ����
		flash_addr=sim.format_addr+1+i;
		i=flash_read(flash_addr); //����ʱ��ĸ�ʽ����
		flash_addr=	flash_addr+1+i;  //����������ʼ��ַ
		flash_read(flash_addr);  
		week_w=srs(); week_w /=8; //���ڵĳ� 
		week_h=srs();             //���ڵĸ�
		d=date32>>16; d-=1;       //��ȡ���ڵ�ֵ
		flash_addr=flash_addr+2+d*week_w*week_h;
		if(sim.week_color&0x01){ //��ɫ
			flash_read(flash_addr);
			for(j=sim.week_place_y;j<sim.week_place_y+week_h;j++){
				for(i=sim.week_place_x;i<sim.week_place_x+week_w;i++){
					sim_buf_red[j*16+i]|=srs();
				} 
			}
	    }
		if(sim.week_color&0x02){  //˫ɫ
			flash_read(flash_addr);
			for(j=sim.week_place_y;j<sim.week_place_y+week_h;j++){
				for(i=sim.week_place_x;i<sim.week_place_x+week_w;i++){
					sim_buf_green[j*16+i]|=srs();
				}
			}		
		}
		#ifdef CONFIG_SEVEN_COLOR
			if(sim.week_color&0x04){  //��ɫ
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
	pArg->info_mode=sim.s_color;      //����������ɫ
	pArg->info_data_w=sim.addr+240*2;	//���������ַ 						//��ʾ���룬�������������ַ 
} 

/**************************************************************************
** �������� �� datadeal_digi
** �������� �� ����ʱ�Ӵ���(���롢�֡�ʱ�ֱ����)
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
void datadeal_digi()
{
	unsigned char r1,r3;
	unsigned short pos;
	pArg->st=1;  
	pArg->end=1;	 
	pos=pArg->miao_pos; //������ı�־
	r1=0;	r3=0;
	if((pArg->miao_pos&0xff)==0xaa){ //û����
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
				pos=pArg->fen_pos;   //�ֵı�־
				r1=8;r3=0x33;
				continue;
			}
		}else{
			if((time32&0xffff)==0){
				pos=pArg->info_data_w; //ʱ��λ��
				r1=16;r3=0x55;
				continue;
			}
		} 
		break;
	}
	if(time32==0)dig_clock_deal();  
}
 
/**************************************************************************
** �������� �� datn_deal
** �������� �� ũ����ʾ����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
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
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//�¸�λ
  }
	r1+=width;
	flash_addr=addr+size*vir_width*high;
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//�µ�λ
	r1+=width;
	flash_addr=addr;
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//��
	r1+=width;
	if(((datn16&0xff)/10==0)||((datn16&0xff)==10))i=1; 
	else if((datn16&0xff)/10==1)i=11; 
	else if((datn16&0xff)==30)i=4; 
	else i=12; 
	flash_addr=addr+i*vir_width*high;
	dig_write(r1,pArg->place_h,width,high,datn_color); 		//�ո�λ
	r1+=width;
	i=(datn16&0xff)%10+1; 
	if((datn16&0xff)%10==0)i=11;  
	flash_addr=addr+i*vir_width*high;
	dig_write(r1,pArg->place_h,width,high,datn_color);	    //�� 
	
	pArg->st=1;  
	pArg->end=1; 
}

/**************************************************************************
** �������� �� pro_end
** �������� �� ��Ŀ����
** ��ڲ��� �� ��
** ���ز��� �� ������־����ʾ���������1�����򷵻�0
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
extern unsigned char pro_end1;
unsigned char pro_end()
{
	unsigned char i,temp=0,__temp;
	for(i=0;i<area_num;i++){ //�����ĸ���    
		if(area[i].end){    //��������
			temp |= (1<<i);   //������������temp�Ķ�Ӧλ��1
			pArg->info_c=0;   //����������0
			info_init(pArg);
		} 
	}
	__temp=(0xff>>(8-area_num));//�ӵ�λ��ʼ��Ϊ1˵���з�����0λ˵��û�з���
	if(temp == __temp)pro_end1=1;
	if((temp == __temp) && ((pro_num > 1) || ((pro_flag&0x03) == 0x01)) || ((pro_flag&0x03)==0x01) && (__didi == scr_info.text)){
		scr_info.st=1;//ͣ����־
		return 1;
	}
	
	else return 0;	
}  

/**************************************************************************
** �������� �� sj
** �������� �� �������
** ��ڲ��� �� ��
** ���ز��� �� ������ŵ�ֵ�����´β��ŵ����з�ʽ
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
unsigned char sj()
{
	unsigned char cnt=0,n;
	n=pArg->info_mode;   //���з�ʽ
	while(1){
 		pArg->temp=(time_sj+area_c+cnt)%25+3; //������ŵ�ֵ
 		cnt++;	
		if((pArg->temp<25)&&(pArg->temp!=n))break;//������ֵ�������25�����Ҳ������ϴ����з�ʽ��������whileѭ��
	}
	return pArg->temp;
}

/**************************************************************************
** �������� �� info_delay
** �������� �� ��Ϣ��ʱ	
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� ͣ��ʱ�䵽,Ϊ�ﵽ���ؾ��⣬��Ϣ���п�ѭ������
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void info_delay()
{
	unsigned char i,n;
	unsigned long r1;
	r1=pArg->screen_w*pArg->screen_h; //���ĵ���
 
	if(pArg->style==4)n=30;//����Ƕ���������ʱʱ����Ϊ30
	else n=12;  //�Ƕ���������ʱ��Ϊ12
	while(r1--){
		i=n;
		while(i--);	
	}
		
}

/**************************************************************************
** �������� �� datadeal
** �������� �� ���ݴ���	
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
unsigned char didi=0;  //�˱����ڱ�������time3������ʹ��
void datadeal()
{

	 static unsigned char cnt=0;
	 for(area_c=0;area_c<area_num;area_c++){//�������ֱ����
	  pArg=&area[area_c]; //ָ��ָ���������ʼλ��
	  if(pArg->style>1)scr_info.text |= (1<<area_c);
	  switch(pArg->style){
	   case 1: 		//�ı���ͼƬ����
/*****************20131230��ӣ���ֹ���Ŀ������Ϣͣ��ʱ��Ϊ0�����л�**************/
            if(pArg->info_c_stand==0){
                if(pArg->info_end){        //��ǰ��Ϣ����           
					if(pArg->info_c==(pArg->info_number-1)){  //��ǰ���������һ����Ϣ����
						pArg->info_c=0;  //������Ϣ��������
						pArg->end=1;	    //��ǰ��������
						if(pro_end())return;  //��ǰ��Ŀ����
					}else{	   
						pArg->info_c++; //��Ϣ��������1
						pArg->info_c %= pArg->info_number;
						info_init(pArg);		   //��ȡ������Ϣ����
					}
               }
           }
/*****************20131230��ӣ���ֹ���Ŀ������Ϣͣ��ʱ��Ϊ0�����л�**************/
		if(pArg->st){			//����ͣ��
			//info_delay();   //ͣ��ʱ,Ϊ�ﵽ���ؾ��⣬��Ϣ������ʱ  //ʹ�����������ʱ�� ��ͣ�ٵ�����Ӱ�������
			if(pArg->st_end){     //ͣ��ʱ�䵽	���������ʱ����Щ��־����			
				pArg->st=0;
                pArg->st_end=0;//���ͣ����־�����������־  &&(pArg->info_end==1)
				if(pArg->sj){ //�������
					pArg->info_mode=sj(); //����ģʽΪ���ȡ��ֵ
					pArg->sj_cnt++;       //������ż����Լ�1
					if(pArg->sj_cnt==pArg->info_data_h){//������ŵļ���Ϊ���ݵĸߣ�˵��һ����������ʾ��
						pArg->sj_cnt=0;     //��������ŵļ�������0
						pArg->info_end=1;   //��Ŀ������־
					}
				}
				if(pArg->info_end){ //��Ŀ����
					if(pArg->info_c==(pArg->info_number-1)){  //��ǰ���������һ����Ϣ����
						pArg->info_c=0;  //������Ϣ��������
						pArg->end=1;	    //��ǰ��������
						if(pro_end())return;  //��ǰ��Ŀ����
						else {goto end;} 
					}else{	   
						pArg->info_c++; //��Ϣ��������1
						pArg->info_c %= pArg->info_number;
						info_init(pArg);		   //��ȡ������Ϣ����
						goto end; 
					}
				}				
			}else {goto end;} 
		}else
		{
		   	if(pArg->info_c_speed){goto end;} 		//�ٶȷ�ֵ
			pArg->info_c_speed=pArg->info_speed;    //��ǰ�ٶ���ֵΪ0ʱ����ֵΪ��Ŀ���ٶ�
	    	datadeal_word(); //�����ı�ͼƬ������
            
          
		}		  
	    break;
		
	   case 2: 				   //ģ��ʱ��
	   	if(pArg->is_init_sim)datadeal_sim();   //sim_init�ڷ�����ʼ��ʱ��1		
	    if(!((sec_flag>>area_c)&0x01))goto end;// Ϊ�˱߿���ʾ����
		sec_flag &= ~(0x01<<area_c);
		if(0x00==(time32&0xff))datadeal_sim();    //���ﵽһ���Ӻ󣬵���ģ��ʱ�䴦��
		show_second();	  //��һ����֮�ڣ�ֻ���ø�������
	    break;
		 
	   case 3: 					  //����ʱ��
	    if(pArg->is_init_dig)dig_clock_deal();  //is_init_dig�ڷ�����ʼ��ʱ��1	
	    if(pArg->miao_pos==0xee)goto end;       //����
	    if(!((sec_flag>>area_c)&0x01))goto end; 
		sec_flag &= ~(0x01<<area_c);
		datadeal_digi();	   //ÿ�붼Ҫ����		
	    break;
		 
	   case 4: 	 //����
	    if(pArg->info_c_speed)goto end;     //�ٶ���ֵ��Ϊ0ʱ��һֱ��ʾ
	    pArg->info_c_speed=pArg->info_speed;//�ٶ���ֵΪ0ʱ����ֵΪ�ٶ�ֵ 
	    datadeal_move();	  
	    break;
		 
	   case 5: 	 //��ʱ
			if(pArg->is_init_count)time_count();
	    if(!((sec_flag>>area_c)&0x01))goto end;
	    sec_flag &= ~(0x01<<area_c);
		time_count();  
	    break;
		 
	   case 6:    //ũ��
	    if(pArg->is_init_datn)datn_deal(); 
		break;
	   default:break;
	  }	
	  end: 
	  if(pArg->area_hb_flag&0x01){//��������(�߿�)
		if(pArg->speed_hb_c)continue;
		pArg->speed_hb_c=10;
		area_hb(pArg,pArg->area_hb_flag);
	  }
	 } 
	 if((__didi == scr_info.text) && (didi>5)){	  
	 	didi = 0;
	 	pro_end();
	 }

	 if(hb.flag){  //���� 
	 	if(scr_info.is_init_hb)hb_deal();//���ߵĳ�ʼֵΪ1ʱ����ʾ����
		if(hb.move_way==0)hb_deal();	   //���߾�ֹ��ʾ
		else if(hb.move_way>0){
			if(hb.c_speed)return;      //��ǰ�ٶ���ֵ��Ϊ0ʱ����һֱ��ʾ
			hb.c_speed=hb.move_speed;	 //��ǰ�ٶ���ֵΪ0ʱ�����¸�ֵ
			if(hb.move_way==1){	  //˳ʱ���ƶ�
				hb_move_deal(1);
			} 
			else if(hb.move_way==2){  //��ʱ���ƶ�
				hb_move_deal(0);
			}else if(hb.move_way==3){ //��˸��ʾ
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
0xFF,0x80,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x20,0x00,0x00,0x00,0x00};/*"��",0*/


unsigned char month[]=
{0x00,0x00,0x00,0x04,0x02,0x00,0x07,0xFF,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x07,0xFE,
0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x06,0x02,0x00,0x04,0x02,0x00,0x07,0xFE,0x00,0x04,0x02,0x00,0x04,
0x02,0x00,0x04,0x02,0x00,0x08,0x02,0x00,0x08,0x02,0x00,0x10,0x0E,0x00,0x60,0x04,0x00,0x00,0x00,0x00};/*"��",0*/



unsigned char xing[]=

{0x00,0x00,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x08,0x01,
0x00,0x0F,0xFF,0x00,0x08,0x40,0x00,0x04,0x60,0x00,0x08,0x61,0x80,0x0F,0xFE,0x00,0x10,0x60,0x00,0x20,
0x62,0x00,0x47,0xFC,0x00,0x00,0x60,0x00,0x00,0x60,0x00,0x7F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00};/*"��",0*/

unsigned char qi[]=


{0x00,0x00,0x00,0x08,0x80,0x00,0x08,0x8F,0xE0,0x08,0xA8,0x40,0x3F,0xF8,0x40,0x08,0x88,0x40,0x08,0x88,
0x40,0x0F,0x8F,0xC0,0x08,0x88,0x40,0x0F,0x88,0x40,0x08,0x88,0x40,0x08,0x8F,0xC0,0x7F,0xE8,0x40,0x09,
0x08,0x40,0x0C,0x98,0x40,0x18,0x50,0x40,0x10,0x60,0x40,0x20,0x41,0xC0,0x00,0x80,0x80,0x00,0x00,0x00};/*"��",0*/



 unsigned char shi[]=


{0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x02,0x00,0x22,0x02,0x00,0x3E,0x02,0x00,0x22,0x02,0x40,0x23,0xFF,
0x80,0x22,0x02,0x00,0x22,0x02,0x00,0x3E,0x62,0x00,0x22,0x32,0x00,0x22,0x32,0x00,0x22,0x02,0x00,0x22,
0x02,0x00,0x3E,0x02,0x00,0x22,0x02,0x00,0x20,0x02,0x00,0x00,0x0E,0x00,0x00,0x04,0x00,0x00,0x00,0x00};/*"ʱ",0*/



unsigned char fen[]=


{0x00,0x00,0x00,0x01,0x18,0x00,0x01,0x90,0x00,0x03,0x08,0x00,0x02,0x08,0x00,0x06,0x04,0x00,0x04,0x02,
0x00,0x08,0x03,0x00,0x10,0x05,0xC0,0x2F,0xFE,0x40,0x40,0x84,0x00,0x00,0x84,0x00,0x01,0x84,0x00,0x01,
0x04,0x00,0x01,0x04,0x00,0x02,0x04,0x00,0x04,0x04,0x00,0x08,0x38,0x00,0x30,0x10,0x00,0x00,0x00,0x00};/*"��",0*/


unsigned char miao[]=



{0x00,0x00,0x00,0x01,0x06,0x00,0x0F,0x04,0x00,0x34,0x04,0x00,0x04,0x04,0x00,0x04,0x35,0x80,0x05,0xA4,
0x40,0x3E,0x24,0x60,0x0C,0x44,0x00,0x0F,0x44,0x00,0x15,0x44,0xC0,0x14,0x84,0x80,0x24,0x05,0x80,0x44,
0x03,0x00,0x04,0x06,0x00,0x04,0x0C,0x00,0x04,0x10,0x00,0x04,0x60,0x00,0x03,0x80,0x00,0x00,0x00,0x00};/*"��",0*/


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
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"һ",0*/



0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x0F,0xFE,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x3F,0xFF,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",0*/



0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x01,0xC0,0x1F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x0F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x7F,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",0*/


0x00,0x00,0x00,0x00,0x00,0x80,0x1F,0xFF,0x80,0x11,0x98,0x80,0x11,0x98,0x80,0x11,0x18,0x80,0x11,0x18 ,
 0x80,0x11,0x18,0x80,0x11,0x18,0x80,0x11,0x18,0x80,0x11,0x18,0x80,0x12,0x18,0x80,0x12,0x0F,0x80,0x14 ,
 0x00,0x80,0x18,0x00,0x80,0x10,0x00,0x80,0x1F,0xFF,0x80,0x10,0x00,0x80,0x00,0x00,0x80,0x00,0x00,0x00,/*"��",0*/




0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x80,0x1F,0xFE,0x00,0x00,0x40,0x00,0x00,0xC0,0x00,0x00,0x80,
0x00,0x00,0x80,0x00,0x00,0x82,0x00,0x0F,0xFE,0x00,0x00,0x82,0x00,0x00,0x82,0x00,0x01,0x02,0x00,0x01,
0x02,0x00,0x01,0x02,0x00,0x01,0x02,0x00,0x01,0x02,0x40,0x7F,0xFF,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",0*/

0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x40,0x00,0x00,0x60,0x00,0x00,0x20,0x00,0x00,0x00,0x40,0x7F,0xFF,
 0xE0,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x88,0x00,0x03,0x04,0x00,0x02,0x06,0x00,0x06,0x03,0x00,0x04,
 0x01,0x00,0x08,0x01,0x80,0x10,0x00,0xC0,0x10,0x00,0xC0,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*"��",0*/


0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,
0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x08,
0x01,0x00,0x08,0x01,0x00,0x08,0x01,0x00,0x0F,0xFF,0x00,0x08,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00/*"��",0*/

};
unsigned char move_num=0;
unsigned char time_num=0;
unsigned char K=0;
// void fun_time_asc(unsigned char *p)
// {
// 	unsigned char i,j,k;
// 	unsigned int temp;
// 	for(i=0;i<32;i++)				//��
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
	for(i=0;i<20;i++)				//��
	//for(i=4;i<24;i++)				//��
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
	for(i=0;i<20;i++)				//��
	//for(i=4;i<24;i++)				//��
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
		if(pArg->info_c_speed){goto end;} 		//�ٶȷ�ֵ
			pArg->info_c_speed=pArg->info_speed;    //��ǰ�ٶ���ֵΪ0ʱ����ֵΪ��Ŀ���ٶ�
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
				case 16:fun_time_zi(&shi[0]);break; //ʱ
				case 17:fun_time_asc(&shuzi[(r5/10)*40]);break; //5
				case 18:fun_time_asc(&shuzi[(r5%10)*40]);break; //5
				case 19:fun_time_zi(&fen[0]);break; //��
				case 20:fun_time_asc(&shuzi[(r6/10)*40]);break; //5
				case 21:fun_time_asc(&shuzi[(r6%10)*40]);break; //5
				case 22:fun_time_zi(&miao[0]);break; //��
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
** �������� �� USART1_IRQHandler
** �������� �� ����1�жϽ���
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
extern unsigned char wifi_init_flag;                    //wifi ��ʼ����־λ�ϵ����� 0������ 1���������� 2��



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
	USART1->CR1&=0Xffdf;//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//���ж�
	if(wifi_init_flag!=1)
	{
		for(i=0;i<6;i++){//�����յ���ǰ�����Ƿ���ȷ
		 if(buf1[i]!=ur()){
		goto out; //������յ��Ĵ����������ж�
		 }
		}

    i=ur(); //����������
	switch(i){
		
		case(0x07):goto set_time;        //Уʱ
		case(0x08):goto screen_on;       //����
		case(0x09):goto screen_off;      //�ػ�
		case(0x0a):goto set_bright;      //�ֶ���������
		
		
		case(0x61):goto interval_Pattern;  //�����������=========================================================================	 
		case(0x62):goto disp_Pattern;      //����============================================================================		
		case(0x63):goto time_on_off; 
		case(0x64):goto set_bright_bg;    //��������======================================================================
					
			
		case(0x0c):goto set_bright_timer;//��ʱ����
		case(0x0d):goto time_sw;         //��ʱ���ػ�
	
		case(0x78):goto flash_write;     //���ս�Ŀ���ⲿflash
		case 0x7c: goto SW;              //�޸Ĳ�����
		case 0x7d: goto rate;            //�޸Ĳ���������
		case(0x82):goto config_info;     //��ȡ���ƿ�������
		case(0x83):goto vision;          //��ȡ�汾��
  	case(0x84):goto s_high;          //��ȡ�汾��

		case(0x86):goto set_rcc;        //���õ�Ƶ

		case(0xab):goto flash_read;     //��flash������
		case(0xFE):goto read_Time;	//��ȡʱ��   ///TEMP

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

rate:   //�޸Ĳ���������
	i=ur();
	if((i>0)&&(i<4)){
		temp=i;
		ut(0xa5);ut(0x5a);
		sw=1;	
	}
	goto out;
	
SW:   //�޸Ĳ�����
	if(sw){  
	  sw=0;
		flash_read_write(1); 						//0x14 ��ſ�����
		sim_buf_red[0x1e]=temp;
		flash_read_write(0);
		ut(0x88);
		r1=0x800000;
		while(r1--); //��ʱ
		SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	}
	goto out;
	
set_rcc:  //���õ�Ƶ
	i=ur();
	rcc_set(i);
	ut(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	goto out;
	
set_bright_timer:  //����ʵʱ����
	for(i=6;i<18;i++)buf[i]=ur();
	timer_bright_set();
	goto out;
	

	
config_info:  //��ȡ���ƿ�������
	flash_addr=8;
	flash_read(flash_addr);
	for(i=0;i<9;i++){
	ut(SPI1->DR);
	spisd;
	}
	goto vision;

set_bright:	 									//�ֶ�����
	i=0xff-ur(); 
	if(i==0)i=16;
	flash_read_write(1); 						//0x29 �������ֵ
	sim_buf_red[BRIGHT_WORD_ADDR]=i;
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ
	goto out;
	
set_bright_bg:	 									//��������===============================================================
	i=ur(); 
	flash_read_write(1); 						//
	sim_buf_red[BRIGHT_BG_ADDR]=i;    
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ
	goto out;
	
interval_Pattern:	 									//�������===============================================================
	//i=ur(); 
	flash_read_write(1); 						//TIME_ONOFF_ADDR
	if(sim_buf_red[INTERVAL_PATTERN_ADDR])	sim_buf_red[INTERVAL_PATTERN_ADDR]=0x00;
	else sim_buf_red[INTERVAL_PATTERN_ADDR]=0x01;
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ
	goto out;			


time_on_off:	 									//ʱ�俪��===============================================================
	//i=ur(); 
	flash_read_write(1); 						//TIME_ONOFF_ADDR
	if(sim_buf_red[TIME_ONOFF_ADDR])	sim_buf_red[TIME_ONOFF_ADDR]=0x00;
	else sim_buf_red[TIME_ONOFF_ADDR]=0x01;
	flash_read_write(0);
	ut(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ
	goto out;			
	
disp_Pattern:	 									//DISP_PATTERN_ADDR  �������Կ���===============================================================
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
 	while(r1--); //��ʱ
  	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ
	goto out;	
	
screen_on:  //����
	scr_info.of=0;
	scr_info.st=1;
	scr_onoff=0;
// 		flash_read_write(1);    
// 		sim_buf_red[0x38] = 0xff;
// 		flash_read_write(0);	
		
	ut(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	goto out;
	
screen_off:  //�ػ�
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
	

	
vision:  //��ȡ���ͺͰ汾��
	ut(card); 
	ut((u8)(VISION>>8));
	ut((u8)VISION);
	goto out; 
	
	s_high:  //��ȡ���ͺͰ汾��
	
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
		


flash_write: //����Ŀ����flash uiFileSum = 0, uiSum = 0
	ur();
	clr_screen(); //����
	//ledplay_denglongHei(0x00);
 	flash_chip_erase(); //����flash
	flash_addr=0;
	if(flash_read(0)!=0xff){ //�ж��Ƿ������
	flash_byte_write(0x0);
	flash_chip_erase(); //�ٴβ���
	}
	flash_read_write(1);//��
	cnt=0xffff;
	while(cnt--);
	//2013.10.30 wwd *clr03
	ut(0x03);   //�����꣬����0x03

	cnt=0;
	dwTmpCnt = GetCnt();
	dwTmpCnt &= 0xfffffff;	   								//�ȴ�Լ30��
	r1 = dwTmpCnt;
     while((USART1->SR&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//ι��
	 	if(--r1==0)
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//��λ
	}
	cnt=USART1->DR;
	cnt<<=8;
	cnt|=ur();
	cnt<<=8;
	cnt|=ur();   //����Ŀ�Ĵ�С��ֵ��cnt
	cnt += 8;// 	cnt -= 3; 2013.10.30 wwd ɾ������ȫ�����ֽ�
//	cnt += 20;// 	20��0xff 2013.10.30 wwd ɾ������ȫ�����ֽ�
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
		r1=dwTmpCnt;	   								//�ȴ�Լ30��
    while((USART1->SR&0x20)==0)//while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{IWDG_Feed();//ι��
			if ( --r1 == 0 )
			{	
				SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//��λ
			}	
		}
		ucTmp = USART1->DR;
		flash_byte_write( ucTmp ); //�����յ������ݴ����ⲿflash��
		flash_addr++;	
		IWDG_Feed();//ι��
	 if(cnt>0)u_getsum+=ucTmp;
	 else if(cnt==0)sums=ucTmp;	 
	}

	r1=0xffff;
	while(r1--); //��ʱ
	if(sums==u_getsum)
	{
		flash_addr=0;
		flash_byte_write(0x88);
		flash_read_write(0);//д
		ut(0x88);    //����0x88
		r1=0x800000;
		while(r1--); //��ʱ
	}
	
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ 
	goto out;
	
flash_read:   //��ȡflash������
	r1=ur();   //��ȡ flash���ݵĸ���
	i=ur();    //��ȡ�ڲ�flash���ⲿflash�ı�־
	switch(i){
	case 0xfe:
		r1=r1*256;
		while(r1--)ut(*p++);
		break;
 
	default:
		cnt=r1;
		cnt<<=8;
		cnt|=i; //��ȡ���ݵĸ���
 		flash_addr=ur();
		flash_addr<<=8;
		flash_addr|=ur();
		flash_addr<<=8;
		flash_addr|=ur();	 //��ȡ�����ݵĵ�ַ	
     	i=flash_read(flash_addr);
		while(cnt--){
		ut(i);
		spisd;
		i=SPI1->DR;
		}
		OFF_CE;		
	}
	goto out;
	
set_time:  //Уʱ   
	ut(0x88);  // ����λ������0x88
	for(i=0;i<8;i++)
	buf[i]=ur();    //��ʵʱ���ꡢ�¡��ա����ڡ�ʱ���֡������buf����
	settime(buf,0); //��ʵʱʱ�������ת��Ϊ��ֵд��RTC�ļ�������
	for(i=0;i<8;i++)
	{
		area[i].is_init_sim=1;
		area[i].is_init_dig=1;
	}
	scr_info.st=1;
	pro_c=0;
	ut(0x88); //��ɲ���֮�󣬸���λ������0x88
// 	if(scr_info.d_l)SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	
	goto out;
	
time_sw: //��ʱ���ػ�
	ut(0x88);
	flash_read_write(1);
	cnt=0;
	for(i=0;i<9;i++)buf[i]=ur(); //��ʱ�����buf��
	if((buf[2]|(buf[3]<<8))==0){	  
	sim_buf_red[0x28]=0xff;
	flash_read_write(0);
	scr_info.of=0; //����
	goto out;
	}
	for(i=0;i<9;i++)sim_buf_red[i+0x20]=buf[i];
	flash_read_write(0);
	goto out;
 set_wifi_key:                   //����Wi-Fi�ȵ�����
		if(mm_set_en) {								//�ϵ�40S�� ��������WIFI����
			flash_read_write(1);            
			i=ur();
								 //�洢���볤��С��19�ֽ��Ҵ���8���ֽ�
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
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ 
			}   
		}
    goto out;
    
set_wifi_ssid:
		if(mm_set_en) {								//�ϵ�40S�� ��������WIFI����
				flash_read_write(1);            
				sim_buf_red[0x56]=ur();
				i=sim_buf_red[0x56];            //�洢���볤��С��19�ֽ��Ҵ���8���ֽ�
				if(i>0&&i<20)
				{
						z=0x57;                         
						for(;i>0;i--)                   
						{
							 sim_buf_red[z]=ur();
							 z++;
						}
						sim_buf_red[0x55]=0xab;             //��־λ������λ����Ҫ��������ģ�������ô�λ,��IAP���������
						flash_read_write(0);
						ut(0x88);
						 r1=0xcfffff;
								while(r1--);
						SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ 
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
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//���ж�
USART1->CR1|=0X20;//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}			
#endif

/**************************************************************************
** �������� �� TIM2_IRQHandler
** �������� �� ��ʱ��2�ж�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
// void TIM2_IRQHandler(void)
// { 												
//     TIM2->SR&=0xfffe;//TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 	//���TIM2���жϴ�����λ
// 	if(scr_info.of)return; //����ǹػ�״̬���򷵻�
// 	(*led_play)(); //��ʾ               
// }

void TIM2_IRQHandler(void)
{ 												
    TIM2->SR&=0xfffe;//TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 	//���TIM2���жϴ�����λ
	if(scr_info.of)return; //����ǹػ�״̬���򷵻�
	//(*led_play)(); //��ʾ               
}

/**************************************************************************
** �������� �� TIM4_IRQHandler
** �������� �� ��ʱ��4�жϹ�OE
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
// void TIM4_IRQHandler(void)
// { 			
// 	TIM4->SR&=0xfffe;			//���TIM4���жϴ�����λ
// 	TIM4->CR1&=0xFE;			//�ض�ʱ��TIM4										
// //     TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update); 	//���TIM4���жϴ�����λ
// // 	TIM_Cmd(TIM4, DISABLE);//ʹ�ܶ�ʱ��4
// 	SET_OE(!scr_info.OE); //��OE
// 	if(scr_info.scan==4)
// 		GPIOA->ODR |= 0x0f;	//t08�ӿڵ�����˫ɫ����	   
// }

void TIM4_IRQHandler(void)
{ 			
	TIM4->SR&=0xfffe;			//���TIM4���жϴ�����λ
	TIM4->CR1&=0xFE;			//�ض�ʱ��TIM4										
 
}

/*************************************************************************/
#elif ConMod2

/**************************************************************************
** �������� �� USART1_IRQHandler
** �������� �� ����1�жϽ���
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��07���޸�	
**************************************************************************/
extern unsigned char wifi_init_flag;                    //wifi ��ʼ����־λ�ϵ����� 0������ 1���������� 2��
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
		{//�����յ���ǰ�����Ƿ���ȷ
		 if(buf1[i]!=usart_get())
			 return; //������յ��Ĵ����������ж�
		 
		}

    i=usart_get(); //����������
	switch(i){
	case(0x07):goto set_time;        //Уʱ
	case(0x08):goto screen_on;       //����
	case(0x09):goto screen_off;      //�ػ�
	case(0x0a):goto set_bright;      //�ֶ���������
		
	case(0x61):goto time_on_off;      //ʱ�俪��=========================================================================	 
	case(0x62):goto disp_Pattern;      //����============================================================================		
	case(0x63):goto time_on_off; 
	case(0x64):goto set_bright_bg;    //��������======================================================================
		
	case(0x0c):goto set_bright_timer;//��ʱ����
	case(0x0d):goto time_sw;         //��ʱ���ػ�		
	case(0x70):goto flash_iap;			//IAP update
	case(0x78):goto flash_write;     //���ս�Ŀ���ⲿflash
	case 0x7c: goto SW;              //�޸Ĳ�����
	case 0x7d: goto rate;            //�޸Ĳ���������
	case(0x82):goto config_info;     //��ȡ���ƿ�������
	case(0x83):goto vision;          //��ȡ�汾��
 	case(0x84):goto s_high;          //��ȡ�汾��
		/*2013.3.20ע�ͣ�ȥ���������ƹ���  modify by zzw*/
//	case(0x85):goto date_limt;
	case(0x86):goto set_rcc;        //���õ�Ƶ
//	case(0x98):goto init;           //��ʼ��
// 	case(0x99):goto reset_ram;      //��λram
	case(0xab):goto flash_read;     //��flash������
	case(0xFE):goto sendtime;	//��ȡʱ��
//	case(0xad):goto rst; //��λ
    case(0x77):goto set_wifi_key;
    case(0x80): goto set_wifi_ssid;
	default:return;
	}
sendtime:			//����ʱ�䵽ģ��
	usart_send(0x2A);usart_send(0x74);usart_send(0x69);usart_send(0x6D);usart_send(0x65);usart_send(0x3A);
	for(i=0;i<14;i++)
		usart_send(ledtime[i]);
	usart_send(0x04);
	return;
rate:   //�޸Ĳ���������
	i=usart_get();
	if((i>0)&&(i<4)){
		temp=i;
		usart_send(0xa5);usart_send(0x5a);
		sw=1;	
	}
	return;
	
SW:   //�޸Ĳ�����
	if(sw){  
	  sw=0;
		flash_read_write(1); 						//0x14 ��ſ�����
		sim_buf_red[0x1e]=temp;
		flash_read_write(0);
		usart_send(0x88);
		r1=0x800000;
		while(r1--); //��ʱ
		SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	}
	return;
	
set_rcc:  //���õ�Ƶ
	i=usart_get();
	rcc_set(i);
	usart_send(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	return;
	
set_bright_timer:  //����ʵʱ����
	for(i=6;i<18;i++)buf[i]=usart_get();
	timer_bright_set();
	return;
	
//init:  //��ʼ��
//// 	flash_deal();
//// 	scr_info.lk=0;
//	FLASH_Unlock();	 	//FLASH
////	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	
//	FLASH_ErasePage(0x0800FC00);
//	flash_chip_erase(); //����flash
//	usart_send(0x88);
//	r1=0x800000;
//	while(r1--); //��ʱ
//	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
//	goto out;
	
// reset_ram:
// 	PWR_BackupAccessCmd(ENABLE);
// 	BKP_WriteBackupRegister(BKP_DR8, 0);
// 	BKP_WriteBackupRegister(BKP_DR9, 0);
// 	BKP_WriteBackupRegister(BKP_DR10, 0);
// 	goto out;
	
config_info:  //��ȡ���ƿ�������
	flash_addr=8;
	flash_read(flash_addr);
	for(i=0;i<9;i++){
	usart_send(SPI1->DR);
	spisd;
	}
	return;
	
set_bright:	 									//�ֶ�����
	i=0xff-usart_get(); 
	if(i==0)i=16;
	flash_read_write(1); 						//0x29 �������ֵ
	sim_buf_red[0x29]=i;
	flash_read_write(0);
	usart_send(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
 	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;			//��λ
	return;
	
screen_on:  //����
	scr_info.of=0;
	scr_info.st=1;
	usart_send(0x88);
	r1=0x800000;
	while(r1--); //��ʱ
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
	return;
	
screen_off:  //�ػ�
	usart_send(0x88);
	if(scr_info.d_l)return;
	clr_screen();
	scr_info.of=1;
	scr_info.st=1;
	GPIOA->ODR |= 0x0f; 
	return;
	
vision:  //��ȡ���ͺͰ汾��
	usart_send(card); 
	usart_send((u8)(VISION>>8));
	usart_send((u8)VISION);
	return; 
	
	s_high:  //��ȡ���ͺͰ汾��
	
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
		
	flash_iap:   //IAP����
	usart_get();
	clr_screen(); //����
 	flash_chip_erase(); //����flash
	flash_addr=0;
	if(flash_read(0)!=0xff){ //�ж��Ƿ������
	flash_byte_write(0x0);
	flash_chip_erase(); //�ٴβ���
	}
	
	flash_read_write(1);//��
	cnt=0xffff;
	while(cnt--);
	
	sim_buf_red[0x70]=0x00;
	sim_buf_red[0x71]=0x01;
	flash_read_write(0);//д

	usart_send(0x04);   //�����꣬����0x04
	//2013.7.29 wwd *clr04
// 	usart_send(0x2A);usart_send(0x63);usart_send(0x6C);
// 	usart_send(0x72);usart_send(0x30);usart_send(0x34); 
		cnt=0;
	 r1=0x8ffffff;	   								//�ȴ�Լ30S
     while((USART_SR()&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//ι��
	 	if(--r1==0)
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//��λ
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
	sim_buf_red[0x72]=cnt&0xff;   //����Ŀ�Ĵ�С��ֵ��cnt
	cnt+=1;
	flash_addr=0;
	sums=u_getsum=0;
	while(cnt--){
	 r1=0x8ffffff;	   								//�ȴ�Լ30S
     while((USART_SR()&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//ι��
	 if(--r1==0)
		 SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//��λ
	 }
	 ucTmp=USART_DR();
	 flash_byte_write(ucTmp); //�����յ������ݴ����ⲿflash��
	 flash_addr++;
	 IWDG_Feed();//ι��
	 if(cnt>0)u_getsum+=ucTmp;
	 else if(cnt==0)sums=ucTmp;
	 
	}
	r1=0xffff;
	while(r1--); //��ʱ
	if(sums==u_getsum)
	{
		usart_send(0x88);    //����0x88
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
		
	flash_read_write(0);//д	
	
	r1=0x800000;
	while(r1--); //��ʱ
	
	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ 
	return; 
	
flash_write: //����Ŀ����flash uiFileSum = 0, uiSum = 0
	usart_get();
	clr_screen(); //����
 	flash_chip_erase(); //����flash
	flash_addr=0;
	if(flash_read(0)!=0xff){ //�ж��Ƿ������
	flash_byte_write(0x0);
	flash_chip_erase(); //�ٴβ���
	}
	flash_read_write(1);//��
	cnt=0xffff;
	while(cnt--);
	//2013.10.30 wwd *clr03
	usart_send(0x03);   //�����꣬����0x03
// 			usart_send(0x2A);usart_send(0x63);usart_send(0x6C);
// 		usart_send(0x72);usart_send(0x30);usart_send(0x33); 
	
	cnt=0;
	dwTmpCnt = GetCnt();
	dwTmpCnt &= 0xfffffff;	   								//�ȴ�Լ30��
	r1 = dwTmpCnt;
     while((USART_SR()&0x20)==0)//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==RESET)
	{IWDG_Feed();//ι��
	 	if(--r1==0)
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//��λ
	}
	cnt=USART_DR();
	cnt<<=8;
	cnt|=usart_get();
	cnt<<=8;
	cnt|=usart_get();   //����Ŀ�Ĵ�С��ֵ��cnt
	cnt += 8;// 	cnt -= 3; 2013.10.30 wwd ɾ������ȫ�����ֽ�
//	cnt += 20;// 	20��0xff 2013.10.30 wwd ɾ������ȫ�����ֽ�
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
		r1=dwTmpCnt;	   								//�ȴ�Լ30��
    while((USART_SR()&0x20)==0)//while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
		{IWDG_Feed();//ι��
			if ( --r1 == 0 )
			{	
				SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	 	//��λ
			}	
		}
		ucTmp = USART_DR();
		flash_byte_write( ucTmp ); //�����յ������ݴ����ⲿflash��
		flash_addr++;	
		IWDG_Feed();//ι��
	 if(cnt>0)u_getsum+=ucTmp;
	 else if(cnt==0)sums=ucTmp;	 
	}

	r1=0xffff;
	while(r1--); //��ʱ
	if(sums==u_getsum)
	{
		flash_addr=0;
		flash_byte_write(0x88);
		flash_read_write(0);//д
		usart_send(0x88);    //����0x88
		r1=0x800000;
		while(r1--); //��ʱ
	}

	SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ 
	return; 
	
flash_read:   //��ȡflash������
	r1=usart_get();   //��ȡ flash���ݵĸ���
	i=usart_get();    //��ȡ�ڲ�flash���ⲿflash�ı�־
	switch(i){
	case 0xfe:
		r1=r1*256;
		while(r1--)usart_send(*p++);
		break;
 
	default:
		cnt=r1;
		cnt<<=8;
		cnt|=i; //��ȡ���ݵĸ���
 		flash_addr=usart_get();
		flash_addr<<=8;
		flash_addr|=usart_get();
		flash_addr<<=8;
		flash_addr|=usart_get();	 //��ȡ�����ݵĵ�ַ	
     	i=flash_read(flash_addr);
		while(cnt--){
		usart_send(i);
		spisd;
		i=SPI1->DR;
		}
		OFF_CE;		
	}
	return; 
	
set_time:  //Уʱ   
// 	usart_send(0x88);  // ����λ������0x88
	for(i=0;i<8;i++)
	buf[i]=usart_get();    //��ʵʱ���ꡢ�¡��ա����ڡ�ʱ���֡������buf����
	settime(buf,0); //��ʵʱʱ�������ת��Ϊ��ֵд��RTC�ļ�������
	for(i=0;i<8;i++)
	{
		area[i].is_init_sim=1;
		area[i].is_init_dig=1;
	}
	scr_info.st=1;
	pro_c=0;
	usart_send(0x88); //��ɲ���֮�󣬸���λ������0x88
// 	if(scr_info.d_l)SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;	
	return; 
	
time_sw: //��ʱ���ػ�
	usart_send(0x88);
	flash_read_write(1);
	cnt=0;
	for(i=0;i<9;i++)buf[i]=usart_get(); //��ʱ�����buf��
	if((buf[2]|(buf[3]<<8))==0){	  
	sim_buf_red[0x28]=0xff;
	flash_read_write(0);
	scr_info.of=0; //����
	return; 
	}
	for(i=0;i<9;i++)sim_buf_red[i+0x20]=buf[i];
	flash_read_write(0);

	return; 
 set_wifi_key:                   //����Wi-Fi�ȵ�����
    flash_read_write(1);            
    i=usart_get();
               //�洢���볤��С��19�ֽ��Ҵ���8���ֽ�
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
		
    SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ 
    }   
	return; 
    
set_wifi_ssid:
      flash_read_write(1);            
    sim_buf_red[0x56]=usart_get();
    i=sim_buf_red[0x56];            //�洢���볤��С��19�ֽ��Ҵ���8���ֽ�
    if(i>0&&i<20)
    {
    z=0x57;                         
    for(;i>0;i--)                   
    {
       sim_buf_red[z]=usart_get();
       z++;
    }
    sim_buf_red[0x55]=0xab;             //��־λ������λ����Ҫ��������ģ�������ô�λ,��IAP���������
    flash_read_write(0);
    usart_send(0x88);
     r1=0xcfffff;
     while(r1--);
		
    SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ 
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
	USART1->CR1&=0Xffdf;//USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);//���ж�
	usart_num=1;
	USART_config();
	USART1->CR1|=0X20;//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}			

/**************************************************************************
** �������� �� USART2_IRQHandler
** �������� �� ����1�жϽ���
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� wwd��2015��05��15���޸�	
**************************************************************************/
void USART2_IRQHandler(void)   					
{
	USART2->CR1&=0Xffdf;//USART_ITConfig(USART2, USART_IT_RXNE, DISABLE);//���ж�
	usart_num=2;
	USART_config();
	USART2->CR1|=0X20;//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}			

/**************************************************************************
** �������� �� TIM2_IRQHandler
** �������� �� ��ʱ��2�ж�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
void TIM2_IRQHandler(void)
{ 												
    TIM2->SR&=0xfffe;//TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 	//���TIM2���жϴ�����λ
	if(scr_info.of || (scr_info.scan ==6))return; //����ǹػ�״̬���򷵻�
	(*led_play)(); //��ʾ               
}

/**************************************************************************
** �������� �� TIM4_IRQHandler
** �������� �� ��ʱ��4�жϹ�OE
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
void TIM4_IRQHandler(void)
{ 			
	TIM4->SR&=0xfffe;			//���TIM4���жϴ�����λ
	TIM4->CR1&=0xFE;			//�ض�ʱ��TIM4										
//     TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update); 	//���TIM4���жϴ�����λ
// 	TIM_Cmd(TIM4, DISABLE);//ʹ�ܶ�ʱ��4
	SET_OE(!scr_info.OE); //��OE
	if(scr_info.scan==4)
		GPIOB->BSRR=0xf; //����ѡ	//t08�ӿڵ�����˫ɫ����	   
}

#endif 


/**************************************************************************
** �������� �� TIM3_IRQHandler
** �������� �� ��ʱ��3�ж�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
void TIM3_IRQHandler(void)
{ 	
	unsigned char i;											
    TIM3->SR&=0xfffe;//TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update); 	//���TIM3���жϴ�����λ
	
	if ( g_wShowVersionCnt < 400 ) 
	{	
		g_wShowVersionCnt ++;
	}
	
	if(scr_info.of)return; //����ǹػ�״̬���򷵻�
	ticks--; 

	for(i=0;i<area_num;i++){
		if(area[i].speed_hb_c)area[i].speed_hb_c--; //���ߵ��ٶȣ�ÿ�μ�һ  
		if(!area[i].info_c_speed)continue;//������ߵ��ٶ�ֵ����0������������ѭ��
	    area[i].info_c_speed--;
	}

	if(hb.c_speed)hb.c_speed--;
	if(ticks==0){
		didi++;
		ticks=200; 
		time_sj++; 	
		
		/* 2013.3.20ȥ����Ŀ����һ������ modify by zzw */
		//if(scr_info.lk)return; 
		/*******************end*************************/

		for(i=0;i<area_num;i++){
			if(area[i].info_c_stand==0){ //��������Ϣͣ��ʱ�䵽
			 	//area[i].st=0;			 //������Ϣͣ����־����
			  	area[i].st_end=1;	 //����ͣ��ʱ�䵽���
			 	area[i].info_c_stand=area[i].info_standtime;  //��װͣ��ʱ��
			 	continue;
			}
			if(area[i].st)area[i].info_c_stand--;  //ͣ��ʱ��ÿ�μ�һ
		}	  
		if(scr_info.s_t){//��������
			if(show_timer)show_timer--;
			else {scr_info.s_t=0;scr_info.st=1;} 
		}
	}            
}

/**************************************************************************
** �������� �� time_sw
** �������� �� ��ʱ���ػ�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��08���޸�	
**************************************************************************/
void time_sw() 							//��ʱ���ػ�
{
	unsigned char flag,*p=(u8*)(0x0800FC20);
	unsigned short time=time32>>8;
	unsigned short r1,r2,r3,r4;
	flag=*(unsigned char*)(0x0800FC28);//�ػ���־��ŵ�λ��
	if(flag==0xff)return;	 //���û�����ö�ʱ���ػ����򷵻� 
	flag=~flag;     //������ö�ʱ���ػ����򽫱�־ȡ��
	r1=*p<<8|*(p+1);    //��һ�ο���ʱ��(ʱ��)
	r2=*(p+3)|*(p+2)<<8;//��һ�ιػ�ʱ��(ʱ��)
	r3=*(p+5)|*(p+4)<<8;//�ڶ��ο���ʱ��(ʱ��)
	r4=*(p+7)|*(p+6)<<8;//�ڶ��ιػ�ʱ��(ʱ��)
	if(time == r1){     //�����ǰ��ʱ�䵽����ʱ��
		scr_info.st = 1;  //ͣ��ʱ���־��λ
		scr_info.of = flag >> 3; //���ÿ��ػ���־
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}else if(time == r2){//�����ǰ��ʱ�䵽�ػ�ʱ��
		scr_info.st = 1;   //ͣ��ʱ���־��λ
		scr_info.of = flag >> 2; //���ÿ��ػ���־
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}else if(time == r3){ //�����ǰ��ʱ�䵽����ʱ��
		scr_info.st = 1;    //ͣ��ʱ���־��λ
		scr_info.of = flag >> 1; //���ÿ��ػ���־
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}else if(time == r4){//�����ǰ��ʱ�䵽����ʱ��
		scr_info.st = 1;   //ͣ��ʱ���־��λ
		scr_info.of = flag;      //���ÿ��ػ���־ 
		if ( scr_info.of )//15.3.20
	{
		flash_read_write(1);
		sim_buf_red[0x38] = 0xa0;
		flash_read_write(0);
	}
	}	
}

/**************************************************************************
** �������� �� RTC_IRQHandler
** �������� �� RTC���ж�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
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
	if(r2<3)//�ж���ֵ���������һ���ӣ����ֵ��һ
	{
		r3=(r1%3600)/60;	//��
		r4=(r1%86400)/3600;				//ʱ
		time32=(r4<<16)|(r3<<8)|r2;
		
	if(r4>=24){    //ʱֵ����24��
			if ( scr_info.of )//15.3.20
			{
				flash_read_write(1);
				sim_buf_red[0x38] = 0xa0;
				flash_read_write(0);
			}	
								
		SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;//��λ
	}
	time_sw(); //��ʱ���ػ�
	timer_bright_check();//��ʱ�������
	
	
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

const u8 scanbuf[4][32]={//��ʼɨ�� 
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
** �������� �� EXTI15_10_IRQHandler
** �������� �� ��������,u���ж�
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
void EXTI15_10_IRQHandler(void)
{
	u8 type=0;//��������
	u8 n;
	u16 i, lh;
	u8 ch = 0;
	u32 time = 400, dwTime;	
	
// 		if (EXTI_GetITStatus(EXTI_Line13) != RESET){	 //���԰���
// 		EXTI_ClearITPendingBit(EXTI_Line13);
			if (EXTI->PR&(1<<13)){
			EXTI->PR=1<<13;
				
 		delay(100);
		/* ȥ�� */
		dwTime = 100000;//300000
		while ( dwTime --> 6 )
		{
			/* �����ʱ��������а��������������˳��ж� */
			if ( GPIOA->IDR & (1 << 13) )  
			{
				return;
			}
		}

		
		/* �ȴ������ɿ� */
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
		{	//����һ��
			for(i = 0; i < lh; i++){
				memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
				n = ((n << 1) | (n & 0x80) >> 7);
			}	
			n = ((n << 1) | (n & 0x80) >> 7);//����
		}
		else if(ch == 3)
		{//��������
			for(i = 0; i < lh; i++){
				memset(&leddata[i*SCREEN_LEN], n, SCREEN_LEN);
				n = ((n >> 1) | (n & 0x01) << 7);
			}	
			n = ((n >> 1) | (n & 0x01) << 7);	//����		
		} 
		else if(ch == 1) 
		{//���ڶ���
			memset(leddata, 0, 8*1024);//ȫ��
		}
		else if(ch == 2) 
		{//��������
			memset(leddata, 0xff, 8*1024);//ȫ��
		}
			
		if(--time < 5){//��ʱ��λ
			SoftReset();//*((u32 *)0xE000ED0C) = 0x05fa0004;
		}
		delay(500000);

		if (!(GPIOA->IDR & (1 << 13))) {
			delay(1000);//��ʱȥ��
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
** �������� �� show_debug
** �������� �� ��ʾ������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��01��10���޸�	
**************************************************************************/
// extern const unsigned char card_debug[3][32];  

//void show_debug() 	//��ʾ������
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
** �������� �� show
** �������� �� ��ʾ3B��
** ��ڲ��� �� ȡģ����ĵ�index�����飬д��ĸ���n��д�뻺������λ��pos
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
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
** �������� �� show_info
** �������� �� ��ʾ����
** ��ڲ��� �� ���Ͳ���ch
** ���ز��� �� ��
** ����˵�� �� ch=1��ʾ���ͣ�������ʾʱ��
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void show_info(unsigned char ch)	//��ʾ����
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
		 
		 
		 
#elif XB_W4_1024  		  //�����ӵ�
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
		       show(cardNum[i],1,2+i); // ��ʾ 
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
//			show(7,1,7);//��ʾS
//		else if(card==0x52 || card==0x54)
//			show(8,1,7);//��ʾH
//		else if(card==0x55)
//			show(9,1,7);//��ʾN
//		else if(card==0x65)
//			show(9,1,7);//��ʾW
	}
// 	RTC_Configuration();
// 	while(1){
//		IWDG_Feed();//ι��
		show((time32&0xff)/10,1,0);//��ʾʱ��-��(ʮλ)
		show((time32&0xff)%10,1,1);//��ʾʱ��-��(��λ)
//		if (scr_info.scan==3)led_play();///14.07.18
//		LEDShowVersion((time32&0xff)/10, 1, -1);
//		LEDShowVersion((time32&0xff)%10, 1, 0);
//	}	  
}

/**************************************************************************
** �������� �� show_info
** �������� �� ��ʾ����
** ��ڲ��� �� ���Ͳ���ch
** ���ز��� �� ��
** ����˵�� �� ch=1��ʾ���ͣ�������ʾʱ��
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void show_err(unsigned char ch)	//��ʾ����
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
//			show((time32&0xff)/10,1,1);//��ʾʱ��-��(ʮλ)
//			show((time32&0xff)%10,1,2);//��ʾʱ��-��(��λ)
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
		IWDG_Feed();//ι��
		if ((scr_info.scan!=1)&&(scr_info.scan!=4)&&(scr_info.scan!=5)){led_play();}////14.7.18
//			while(1)
//			{
//				IWDG_Feed();//ι��
//				if(ch==5)
//				{
//					show((time32&0xff)/10,1,1);//��ʾʱ��-��(ʮλ)
//					show((time32&0xff)%10,1,2);//��ʾʱ��-��(��λ)
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
** �������� �� flash_check
** �������� �� ���flash��������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void flash_check()
{
  unsigned char j,d;
//   unsigned short time;
  u8 f_sum,sum;
//	u8 a_data=0;//��Ŀʱ�����У���־
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
//		flash_chip_erase(); //����flash
//// 		flash_addr=0;
//// 		if(flash_read(flash_addr)!=0xff){//�ж��Ƿ������
//// 			flash_byte_write(0x0);
//// 			flash_chip_erase(); //�ٴβ���
//// 		}
////  					
//// 		flash_addr=1;
//// 		for(j=1;j<7;j++){ //flash�ĵ�1����ַ��ʼд��1~7
//// 			flash_byte_write(j);
//// 			flash_byte_write(j);//��ֹ��һ��ûд��ȥ
//// 			flash_addr++;
//// 			time=1000;
//// 			while(time--);//��ʱ	
//// 		}
//// 		for(j=1;j<7;j++){
//// 			if(flash_read(j)!=j)//�������ݣ�������������ݺ�д������ݲ�ͬ������forѭ��
//// 				break;
//// 		}
//		/* ���FLASHΪ�վ�Ĭ��˫ɫ64�����;2013.3.15�޸� */
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
//				show(0,1,2+j); //�����ĵ�(2+j)��λ����ʾR
//		}
//		
//		ut(0x2A);ut(0x74);ut(0x69);ut(0x6D);ut(0x65);ut(0x3A);
//		ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);
//		ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);ut(0);
//		ut(0x04);
//		show_info(0); //��ʾʱ��													
//	}
	flash_addr=2;//a_data=0;
	d=flash_read(0);  //��ȡflash���׵�ַ�е�����
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
	d=flash_read(flash_addr); //��ȡflash�е�ַ2������
	if((d!=card)&&(d!=0)) //���Ͳ���
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
////		show_info(1); //��ʾ����	
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
** �������� �� rcc_set
** �������� �� ���ò�����
** ��ڲ��� �� д�����ʵı�־
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
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
** �������� �� date_limt
** �������� �� ����ʹ����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
// void date_limt()
// {
// 	/*
//  	bkp8�洢�û���
//  	bkp9�洢����ʱ��
//  	bkp10�洢��ǰʱ��
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
// 	if(r2<=r1||r2==0xffff){	 						//�뵱ǰ���ڱȽ�
// 	sim_buf_red[0x2e]=r1;
// 	sim_buf_red[0x2f]=r1>>8;
// 	flash_read_write(0); 							//���浱ǰ���ڵ�ֵ
// 	}
// 	else scr_info.d_l=1;							//���趨���ڱȽ�
// 	r2=sim_buf_red[0x2c]|(sim_buf_red[0x2d]<<8);
// 	if(r2<=r1)scr_info.d_l=1; 
// 		
// }

// /**************************************************************************
// ** �������� �� date_limt_set
// ** �������� �� ����ʹ��������
// ** ��ڲ��� �� ��
// ** ���ز��� �� ��
// ** ����˵�� �� 
// ** �޸���־ �� ������ ��2013��03��14���޸�	
// **************************************************************************/
// /*  00 01 02 03     �û���0a 0b  ��������0c 0d  ��ǰ����0e 0f */
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
// 	if(r1!=r2)return;			//�û��������򷵻�
// 	if(buf[14]==0){				//�����򷵻�
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
** �������� �� timer_bright_check
** �������� �� ��ⶨʱ����
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void timer_bright_check()
{
  unsigned char *p=(u8*)(0x0800FC05);
  unsigned short time,time2,i;
  if(*p!=0x88)return;
  p++;	time=time32>>8; //ʱ��ķ�
  for(i=0;i<4;i++){
  time2=*p;
  time2<<=8;
  p++;
  time2|=*p;
  p++;
  if(time==time2){  //���������ȵ�ʱ�䵽

	bright=0xff-*p;
	if(bright==0)bright=16;
	init_t4(bright);  //��ʱ��14����	 		
	flash_read_write(1); 						//0x29 �������ֵ
	sim_buf_red[0x29]=bright;
	flash_read_write(0);			
	break ;
 }
 p++;
 }	
}

/************************************************************************************************************
��������: 
*************************************************************************************************************/
/*�ڴ��ַ
 *0x00-0x03 
 *0x06-0x12 ������Ϣ
 *0x18		Ƶ�� 0x38-0x14
 *0x20-0x2f ������Ϣ
 */

/**************************************************************************
** �������� �� timer_bright_set
** �������� �� ��ʱ��������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��14���޸�	
**************************************************************************/
void timer_bright_set()										//��ʱ����					
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
// ** �������� �� id_read
// ** �������� �� ��CPU��id��
// ** ��ڲ��� �� ��
// ** ���ز��� �� cpu��id��
// ** ����˵�� �� 
// ** �޸���־ �� ������ ��2013��03��14���޸�	
// **************************************************************************/
// unsigned long id_read()
// {
// 	u32 CpuID[3];
// 	u32 Lock_Code;
//  													 	//��ȡCPUΨһID
//  	CpuID[0]=*(vu32*)(0x1ffff7e8);
//  	CpuID[1]=*(vu32*)(0x1ffff7ec);
//  	CpuID[2]=*(vu32*)(0x1ffff7f0);
//  
//  	Lock_Code=(CpuID[0]>>3)+(CpuID[1]>>1)+(CpuID[2]>>2);//�����㷨,�ܼ򵥵ļ����㷨
// 	return Lock_Code;
// }

// /**************************************************************************
// ** �������� �� id_check
// ** �������� �� ���id��
// ** ��ڲ��� �� ��
// ** ���ز��� �� ��
// ** ����˵�� �� 
// ** �޸���־ �� ������ ��2013��03��14���޸�	
// **************************************************************************/
// void id_check()
// {
// 	u32 id,r1;
// 	scr_info.lk=0; //������־��ʼֵΪ0
// 	id=id_read();

// 	r1=*(vu32*)(0x0800FC00);
// 	if(id!=r1)scr_info.lk=1;//���id�Ų���ȷ����������־��λ
// } 

// /**************************************************************************
// ** �������� �� flash_deal
// ** �������� �� ��id�Ŵ����ڲ�flash��
// ** ��ڲ��� �� ��
// ** ���ز��� �� ��
// ** ����˵�� �� 
// ** �޸���־ �� ������ ��2013��03��14���޸�	
// **************************************************************************/
// void flash_deal()
// {
// 	typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
// 	u32 StartAddr=0xF800;
// 	u32 id;
// 	FLASH_Unlock();		 //FLASH����

//   FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	//���־λ

// 	id=id_read();   //��ȡid��

// 	FLASH_ErasePage(StartAddr);//�����ڲ�flash��StartAddr��ʼ��ҳ
// 	FLASH_ErasePage(StartAddr+FLASH_PAGE_SIZE);//�����ڲ�flash��(StartAddr+FLASH_PAGE_SIZE)��ʼ��ҳ
// 	FLASH_ProgramWord(StartAddr, id);//��id��д���ڲ�flash��StartAddr��ַλ
// 	FLASH_ProgramWord(StartAddr+FLASH_PAGE_SIZE, id);//��id��д���ڲ�flash��(StartAddr+FLASH_PAGE_SIZE)�ĵ�ַλ
// }






