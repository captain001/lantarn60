#ifndef __EXPORT_H__
#define __EXPORT_H__

 


#define SHIFT_SHEL_S32	d|=(red&0x00000001)<<15;  			 \
												*pOut=d|0xFF800000;						  \
												red>>=1;							 \
												d =(red&0x00008000)>>1; 			 \
												*pOut=0x80;
												


struct all_scall_struct 
{
	
  unsigned char  all_scall_color;  //0 ��ɫ��1˫ɫ
	unsigned char  all_scall_screen_high;//�����ʾ�Ǽ������ߡ�1,2,3,4,

			
//���ݸ߶ȣ���˫ɫѡ����ʵ���ʾ������
    void (*put_pix)(unsigned long red,unsigned long green);
 		void (*gen_data)(unsigned short temp,unsigned short temp1);
};

volatile extern unsigned char row11;        // �Ǽ���֮��ɨ
volatile extern unsigned char Singe_Flag; 	//�����ʾ ��ȫ������ ������ʾ��   01 ȫ��  55�ϰ���  aa�°���
volatile extern unsigned char circle_style; //���У�  1 ���ֽ�Ϊ��λ   1:��ʾ8����һ����
volatile extern unsigned char circle_num;  //�����˫�����Ĳ���
volatile extern unsigned char face_style;  //���  0��ʾ 08�ӿ�   1��ʾ  12�ӿ�
volatile extern unsigned char scan81[50];
volatile extern unsigned char scan8temp1[50];

extern unsigned short usBright;
extern char flag_enter;
extern struct all_scall_struct all_scall_face;



#endif
