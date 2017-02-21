#ifndef __EXPORT_H__
#define __EXPORT_H__

 


#define SHIFT_SHEL_S32	d|=(red&0x00000001)<<15;  			 \
												*pOut=d|0xFF800000;						  \
												red>>=1;							 \
												d =(red&0x00008000)>>1; 			 \
												*pOut=0x80;
												


struct all_scall_struct 
{
	
  unsigned char  all_scall_color;  //0 单色，1双色
	unsigned char  all_scall_screen_high;//这个表示是几个屏高。1,2,3,4,

			
//根据高度，单双色选择合适的显示函数。
    void (*put_pix)(unsigned long red,unsigned long green);
 		void (*gen_data)(unsigned short temp,unsigned short temp1);
};

volatile extern unsigned char row11;        // 是几分之几扫
volatile extern unsigned char Singe_Flag; 	//这个标示 是全屏还是 半屏显示；   01 全屏  55上半屏  aa下半屏
volatile extern unsigned char circle_style; //折行：  1 以字节为单位   1:表示8个点一折行
volatile extern unsigned char circle_num;  //这个是双折屏的参数
volatile extern unsigned char face_style;  //这个  0表示 08接口   1表示  12接口
volatile extern unsigned char scan81[50];
volatile extern unsigned char scan8temp1[50];

extern unsigned short usBright;
extern char flag_enter;
extern struct all_scall_struct all_scall_face;



#endif
