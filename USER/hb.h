#ifndef __HB_H__
#define __HB_H__
#include "led.h"

//上下花边右移 
#define HB_SHIFT_RIGHT(h,offset) for(x=0;x<step;x++){\
									for(j=h;j<h+hb.high;j++){\
							  		 temp=leddata[(j+offset+1)*len-1];\
									 for(i=len-1;i>0;i--){\
									 	 d=leddata[(j+offset)*len+i]>>1; \
										 d2=leddata[(j+offset)*len+(i-1)]&0x01;	\
										 d2<<=7;\
										 leddata[(j+offset)*len+i]=d|d2; \
									 }\
									 temp<<=7; 	\
									 leddata[(j+offset)*len]=(leddata[(j+offset)*len]>>1)|temp; \
							  	 	}  \
								 }

//上下花边左移
#define HB_SHIFT_LEFT(h,offset)  for(x=0;x<step;x++){\
									for(j=h;j<h+hb.high;j++){\
							 		 temp=leddata[(j+offset)*len];\
									 for(i=0;i<len-1;i++){\
									 	 d=leddata[(j+offset)*len+i]<<1;\
										 d2=leddata[(j+offset)*len+(i+1)]&0x80;\
										 d2>>=7;\
										 leddata[(j+offset)*len+i]=d|d2; \
									 }	\
									 temp>>=7;\
									 leddata[(j+offset+1)*len-1]=(leddata[(j+offset+1)*len-1]<<1)|temp;\
							 	     } \
								   }


//左侧花边上移，右侧花边下移
#define HB_SHIFT_SS(buf_l,buf_r)  for(x=0;x<step;x++){			          \
									temp=buf_l[hb.high];		          \
									for(j=hb.high;j<scr_h-1;j++){	      \
										buf_l[j] = buf_l[j+1];		      \
									 } 							          \
									 buf_l[scr_h-1] = temp;		          \
									 temp=buf_r[scr_h-1];			      \
									 for(j=scr_h-1;j>hb.high;j--){	      \
										buf_r[j] = buf_r[j-1];    	      \
									 }								      \
									 buf_r[hb.high] = temp;	 			  \
								  }	        

//左侧花边下移，右侧花边上移
#define HB_SHIFT_NS(buf_l,buf_r)  for(x=0;x<step;x++){			  		  \
									temp=buf_l[scr_h-1];			      \
									for(j=scr_h-1;j>hb.high;j--){	      \
										buf_l[j] = buf_l[j-1];		      \
									}								      \
									buf_l[hb.high] = temp;			      \
									temp=buf_r[hb.high];				  \
									for(j=hb.high;j<scr_info.high-1;j++){ \
										buf_r[j] = buf_r[j+1];	          \
									}								      \
									buf_r[scr_h-1] = temp;				  \
								 }		   

struct hbStruct{ 		//花边结构
				unsigned long addr;      //内容基地址
				unsigned char style;     //花边样式
				unsigned char color;     //颜色
				unsigned char flag;      //有花边标志
				unsigned char width;     //上位机发的花边实际宽度
				unsigned char high;      //花边高度
				unsigned char move_way;  //移动方式
				unsigned char move_speed;//移动速度
				unsigned char c_speed;   //当前速度
				unsigned char move_step; //步长
				unsigned char cge;

				unsigned char vir_w;     //经过计算的虚拟宽度
	};
/***********************************函数声明**********************************************/
void hb_deal(void);
void hb_move_deal(unsigned char ch); 
void clear_hb(void); 
void area_hb(struct AreaStruct *p,unsigned char inf);	

#endif 
