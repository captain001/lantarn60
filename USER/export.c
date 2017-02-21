#include "export.h"
#include "led.h"
#include "stmlib.h"
#include "hb.h"



#define PORT_R1_HIGH()    	GPIOB->BSRR=0x8000 
#define PORT_R1_LOW()    		GPIOB->BRR =0x8000
#define PORT_CLK_HIGH()    	GPIOB->BSRR=0x80 
#define PORT_CLK_LOW()    	GPIOB->BRR =0x80



#define FP_DA_HI()    	GPIOB->BSRR=0x8000 
#define FP_DA_LOW()    	GPIOB->BRR =0x8000

#define FP_SCK_HI()    	GPIOB->BSRR=0x80 
#define FP_SCK_LOW()    GPIOB->BRR =0x80

#define FP_LT_HI()    	GPIOA->BSRR=0x100 
#define FP_LT_LOW()     GPIOA->BRR =0x100


#define DT_SCK_HI()    	GPIOA->BSRR=0x01 
#define DT_SCK_LOW()    GPIOA->BRR =0x01

#define DT_LT_HI()    	GPIOA->BSRR=0x02 
#define DT_LT_LOW()    	GPIOA->BRR =0x02


//����ȫ�ֱ���
unsigned char dis_length,ex_row=0,h=16,hx=0; //��Ŀ�ĳ���(�ֽ���)����ѡ����������ѡ��ʼֵ
unsigned short usBright = 0;

unsigned char	disp_nbytes;

const u8 scan8[8]={12,8,12,8,4,0,4,0};
const u8 scan8temp[8]={0,0,1,1,0,0,1,1};
//�����ⲿ����
extern unsigned char bright;//����
extern unsigned short screen_len;   //��Ŀ�ĳ���(�ֽ���)
extern unsigned char offset_green;  
extern struct ScrStruct  scr_info;  
extern struct hbStruct hb;          
extern unsigned char leddata[8*1024];
unsigned char jiade[16]={0,0,0,1,8,0,8,1,0,2,0,3,8,2,8,3};
extern unsigned char scan_play;

//===================================================
void delay_nop(u32 i)      
{
	while(i) i--;
}

void  FP_SCK()							//CP  FP-SCK0
{	
	u8 i=30;
	GPIOB->BSRR=0x80;			//CP  FP-SCK0	
	while(i--);
	GPIOB->BRR=0x80;
}

void  FP_LT()							//LC  FP-LT0
{	
	u8 i=30;
	GPIOA->BSRR=0x100;			//LC  FP-LT0	
	while(i--);
	GPIOA->BRR=0x100;
}

static void  DT_SCK()			//A  DT-SCK1
{	
	u8 i=30;
	GPIOA->BSRR=0x1;				//A  DT-SCK1
	while(i--);
	GPIOA->BRR=0x1;
}
static void  DT_LT()			//B  DT-LT1			
{	
	u8 i=30;
	GPIOA->BSRR=0x2;				//B  DT-LT1	
	while(i--);
	GPIOA->BRR=0x2;
}

// void export_s32(unsigned long red)
// {
// 	register unsigned short d;	
// 	register volatile unsigned long *pOut,s;
// 	pOut=( volatile unsigned long *)0x40010c10;

// 		d =(red&0x00008000)>>1;
// 		SHIFT_SHEL_S32
// 		SHIFT_SHEL_S32
// 		SHIFT_SHEL_S32
// 		SHIFT_SHEL_S32
// 		SHIFT_SHEL_S32
// 		SHIFT_SHEL_S32
// 		SHIFT_SHEL_S32

// 	

// 	d|=(red&0x00000001)<<15;
// 		*pOut=d|0xFF800000;

// 		red>>=1;
// 		d = red&0x01;
// 		d |= 0x80;
// 		*pOut=0x80|d;
// }
/******************************************************/
   //�������鵥Ԫ�����ʾ
/******************************************************/
void send_dis_data_12_32(u8 red)
{
    u8 temp1;
  	temp1=red;
	
	//PORT_G1_HIGH(); 
	//PORT_G2_HIGH(); 
	//PORT_R1_LOW(); 
	
		if((temp1&0x80)==0x80) PORT_R1_HIGH();	  
		else  PORT_R1_LOW();
		PORT_CLK_LOW(); 	 //��λbit0	
    //delay_nop(1)  ;
		PORT_CLK_HIGH(); 
	
		if((temp1&0x40)==0x40) PORT_R1_HIGH();
		else PORT_R1_LOW(); 
		PORT_CLK_LOW(); 	 //��λbit1
    //delay_nop(1)  ;
		PORT_CLK_HIGH(); 
	
		if((temp1&0x20)==0x20) PORT_R1_HIGH();
		else PORT_R1_LOW();
		PORT_CLK_LOW();//��λbit2
    //delay_nop(1)  ;
		PORT_CLK_HIGH(); 
	
		if((temp1&0x10)==0x10) 	PORT_R1_HIGH();	 
		else PORT_R1_LOW();
		PORT_CLK_LOW(); 
    //delay_nop(1)  ;
    PORT_CLK_HIGH();
	
		if((temp1&0x08)==0x08)  PORT_R1_HIGH();
		else PORT_R1_LOW(); 
		PORT_CLK_LOW();	
    //delay_nop(1);
		PORT_CLK_HIGH(); 
	
		if((temp1&0x04)==0x04) PORT_R1_HIGH();
		else  PORT_R1_LOW(); 
		PORT_CLK_LOW(); 
    //delay_nop(1);
		PORT_CLK_HIGH();
	
		if((temp1&0x02)==0x02)  PORT_R1_HIGH();	
		else PORT_R1_LOW();
		PORT_CLK_LOW(); 
    //delay_nop(1);
    PORT_CLK_HIGH();
	
    PORT_CLK_LOW(); 
		if((temp1&0x01)==0x01) PORT_R1_HIGH(); 
		else 	PORT_R1_LOW();
		PORT_CLK_LOW();  
    //delay_nop(1);
    PORT_CLK_HIGH();

    //delay_nop(1);	
}


struct all_scall_struct all_scall_face;

volatile unsigned char row11=0;        // �Ǽ���֮��ɨ
volatile unsigned char Singe_Flag=0; //�����ʾ ��ȫ������ ������ʾ��   01 ȫ��  55�ϰ���  aa�°���
volatile unsigned char circle_style=0; //���У�  1 ���ֽ�Ϊ��λ   1:��ʾ8����һ����
volatile unsigned char circle_num=0;  //�����˫�����Ĳ���
volatile unsigned char face_style=0;  //���  0��ʾ 08�ӿ�   1��ʾ  12�ӿ�
volatile unsigned char scan81[50];
volatile unsigned char scan8temp1[50];

unsigned long red,green; 

void ledplay_denglong()
{
	unsigned char row,i,len;
	//unsigned short scr_len=SCREEN_LEN;
	unsigned short scr_len=13;
	unsigned long red;  
	unsigned short temp;	//������������24us ת��+д��IO������57.9us

		for (row = 0; row < 32; row ++)
		{
			//for(len=0;len<scr_len;len++)	 
			//for(len=(scr_len-1);len>0;len--)
			for(len=scr_len-1,i=0;i<scr_len;i++,len--)	 
			{  						
					temp=(31-row)*scr_len+(scr_len-len-1);	 

					red=(u32)leddata[temp];
 					if(!scr_info.data){ 			//������ݼ���Ϊ�����ԣ��������ȡ��
 						red=~red;}
					send_dis_data_12_32(~red);
			}
			FP_LT();
			DT_SCK();
	}
	DT_LT();
}

// void ledplay_denglong_bg()
// {
// 	unsigned char row,i,len;
// 	//unsigned short scr_len=SCREEN_LEN;
// 	unsigned short scr_len=13;
// 	unsigned long red;  
// 	unsigned short temp;	//������������24us ת��+д��IO������57.9us

// 		for (row = 0; row < 32; row ++)
// 		{
// 			for(len=0;len<scr_len;len++)	 
// 			{  						
// 					temp=(31-row)*scr_len+(scr_len-len-1);	 

// 					red=(u32)leddata[temp];
//  					if(!scr_info.data){ 			//������ݼ���Ϊ�����ԣ��������ȡ��
//  						red=~red;}
// 					send_dis_data_12_32((~red));
// 			}
// 			FP_LT();
// 			DT_SCK();
// 	}
// 	DT_LT();
// }
/*
void ledplay_denglong()
{
	unsigned char row,i,len;
	unsigned short scr_len=SCREEN_LEN;
	unsigned long red,green;  
	unsigned short temp;	//������������24us ת��+д��IO������57.9us
	unsigned char		disp_n=0;
	SET_OE(scr_info.OE);
	//scr_info.high
	disp_nbytes++;
	if(disp_nbytes>2)disp_nbytes=0;
	
	//disp_n=disp_nbytes*3;
	
		for (row = 0; row < 24; row ++)
		{
			
			for(len=0;len<scr_len;len++)	 
			{  						//24us
					 temp=(23-row)*scr_len+(scr_len-len-1);	 

					red=(u32)leddata[temp];
 					 if(!scr_info.data){ //������ݼ���Ϊ�����ԣ��������ȡ��
 						red=~red;
						//green=~green;
					 } 
					
					 export_s32(red);
			}
			
			HL_LC();
			i=30;//20; 2014.08.19 wwd
			while(i--);	 
			HL_LCA();			

	}
	HL_LCB();
}*/

void ledplay_denglongHei(unsigned char data)
{
	unsigned char row,len;
	//unsigned short scr_len=SCREEN_LEN;
	unsigned short scr_len=13;
	//unsigned long red;  
//	unsigned short temp;	//������������24us ת��+д��IO������57.9us

		for (row = 0; row < 32; row ++)
		{

			for(len=0;len<scr_len;len++)	 
			{  						
					//red=data;
					//if(!scr_info.data){ 			//������ݼ���Ϊ�����ԣ��������ȡ��
 					//red=~red;}
					send_dis_data_12_32(data);
			}
			
			FP_LT();
			DT_SCK();
	}
	
	DT_LT();

}
