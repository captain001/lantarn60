#ifndef __HB_H__
#define __HB_H__
#include "led.h"

//���»������� 
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

//���»�������
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


//��໨�����ƣ��Ҳ໨������
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

//��໨�����ƣ��Ҳ໨������
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

struct hbStruct{ 		//���߽ṹ
				unsigned long addr;      //���ݻ���ַ
				unsigned char style;     //������ʽ
				unsigned char color;     //��ɫ
				unsigned char flag;      //�л��߱�־
				unsigned char width;     //��λ�����Ļ���ʵ�ʿ��
				unsigned char high;      //���߸߶�
				unsigned char move_way;  //�ƶ���ʽ
				unsigned char move_speed;//�ƶ��ٶ�
				unsigned char c_speed;   //��ǰ�ٶ�
				unsigned char move_step; //����
				unsigned char cge;

				unsigned char vir_w;     //���������������
	};
/***********************************��������**********************************************/
void hb_deal(void);
void hb_move_deal(unsigned char ch); 
void clear_hb(void); 
void area_hb(struct AreaStruct *p,unsigned char inf);	

#endif 
