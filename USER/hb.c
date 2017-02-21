#include "hb.h"
#include "led.h"
#include "stmlib.h"

//����ȫ�ֱ���
#ifdef CONFIG_SEVEN_COLOR
	unsigned char hb_lblue[70],hb_rblue[70]; //���������ɫ���ݻ������������Ҳ���ɫ���ݻ�����
#endif
unsigned char hb_l[70],hb_lgreen[70]; //��������ɫ���ݻ����������������ɫ���ݻ�����
unsigned char hb_r[70],hb_rgreen[70]; //�����Ҳ��ɫ���ݻ������������Ҳ���ɫ���ݻ�����

//����ṹ�����
struct hbStruct hb; //�������Խṹ

//�����ⲿ����
extern struct ScrStruct scr_info; //�����Խṹ
extern unsigned long flash_addr,pro_addr;//flash�ĵ�ַ����Ŀ��ŵ�ַ
extern unsigned short screen_len; //��Ŀ�ĳ���(�ֽ���)
extern unsigned char offset_green;//��ɫ���ݵ�ƫ����
extern unsigned char area_c,pro_c,area_num;//��������������Ŀ����������������
extern unsigned char leddata[8*1024]; //��ʾ��������
extern unsigned char buf[40];
extern unsigned char dis_length;//��Ŀ�ĳ���(�ֽ���)

/**************************************************************************
** �������� �� hb_way
** �������� �� �����ƶ�����
** ��ڲ��� �� �ƶ������־ch���ƶ��Ĵ���n
** ���ز��� �� ���ߵ�ȡֵ
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��12���޸�	
**************************************************************************/
unsigned short hb_way(unsigned char ch,unsigned char n)	//ch=1˳ʱ��
{
	unsigned char up=0,down=0;
	if((ch&0x06)==0x04){ //˳ʱ��
		switch(n){ //�ƶ��Ĵ���n
			case 0:	up=0x0f; down=0xf0;	
				break;
			case 1:	up=0x87; down=0xe1;
				break;
			case 2:	up=0xc3; down=0xc3;
				break;
			case 3:	up=0xe1; down=0x87;
				break;
			case 4:	up=0xf0; down=0x0f;
				break;
			case 5:	up=0x78; down=0x1e;
				break;
			case 6:	up=0x3c; down=0x3c;
				break;
			case 7:	up=0x1e; down=0x78;
				break;
			default: break;
		}	
	}else if((ch&0x06)==0x02){ // ��ʱ��
		switch(n){
			case 0:	up=0x0f; down=0xf0;
				break;
			case 1:	up=0x1e; down=0x78;
				break;
			case 2:	up=0x3c; down=0x3c;
				break;
			case 3:	up=0x78; down=0x1e;
				break;
			case 4:	up=0xf0; down=0x0f;
				break;
			case 5:	up=0xe1; down=0x87;
				break;
			case 6:	up=0xc3; down=0xc3;
				break;
			case 7:	up=0x87; down=0xe1;
				break;
			default: break;
		}
	}
	return ((unsigned short)up<<8)|(unsigned short)down;
}

/**************************************************************************
** �������� �� area_hb
** �������� �� ��������
** ��ڲ��� �� ���ߵĲ����ṹ��p�������ƶ���־inf
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��12���޸�	
**************************************************************************/
void area_hb(struct AreaStruct *p,unsigned char inf)
{  
	unsigned char j,n=0,up,down;
	register unsigned short scr_len=SCREEN_LEN;
	unsigned char place_h,place_w,screen_h;
	unsigned char r,l,bit_r,bit_l;
	unsigned short screen_w,i,temp;
	static unsigned char cnt_hb=0,cs=0;	
	place_h=p->place_h;    //��Ŀ��ʼλ�õ�������
	screen_h=p->screen_h;  //��Ļ�ĸ�
	place_w=p->place_w;    //��Ŀ��ʼλ�õĺ�����
	screen_w=p->screen_w;  //��Ļ�ĳ�
	if((inf&0x06)==0){up=0x0f;down=0xf0;}
	else{	 
		temp=hb_way(inf,cnt_hb);
		up=temp>>8;down=temp;
		cs++;
		if(cs>=area_num){
			cs=0;
			cnt_hb++;cnt_hb%=8;
		}
	}
	bit_r=0x80; //��໨�����ʱ��
	r=~bit_r;
	bit_l=0x01; //�Ҳ໨�����ʱ��
	l=~bit_l;

	if(hb.flag){ //���߱�־
		if(place_w==0){
			bit_r=0x80>>hb.high;
			r=~bit_r;			
		}
		if(scr_len==place_w+screen_w){
			bit_l=0x01<<hb.high;
			l=~bit_l;		
		}
	}

	GREEN_BEGIN
		if(inf&0x20){
			for(n=0,j=place_h;j<screen_h+place_h;j++,n++,n%=8){
				leddata[(j+OFFSET_GREEN)*scr_len+place_w] &= r;//0x7f;
				if(down&(0x80>>n))leddata[(j+OFFSET_GREEN)*scr_len+place_w] |= bit_r;//0x80;

				leddata[(j+OFFSET_GREEN)*scr_len+place_w+screen_w-1] &= l;//0xfe;
				if(up&(0x80>>n))leddata[(j+OFFSET_GREEN)*scr_len+place_w+screen_w-1] |= bit_l;//0x01;  
			}
		}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR
		if(inf&0x40){
			for(n=0,j=place_h;j<screen_h+place_h;j++,n++,n%=8){
				leddata[(j+OFFSET_BLUE)*scr_len+place_w] &= 0x7f;
				if(down&(0x80>>n))leddata[(j+OFFSET_BLUE)*scr_len+place_w] |= 0x80;
				leddata[(j+OFFSET_BLUE)*scr_len+place_w+screen_w-1] &= 0xfe;
				if(up&(0x80>>n))leddata[(j+OFFSET_BLUE)*scr_len+place_w+screen_w-1] |= 0x01;  
			}
		}
	#endif

	if(inf&0x10){  //��ɫ
		for(i=place_w;i<place_w+screen_w;i++){ 
			leddata[place_h*scr_len+i] = up;
			leddata[(place_h+screen_h-1)*scr_len+i] = down;
		}
		if(hb.high){  //������ߵĸ߶ȷ�0
			if(!place_w){
				l=0xff>>hb.high;
				leddata[place_h*scr_len] &= l;
				leddata[(place_h+screen_h-1)*scr_len] &= l;
			}
			if(scr_len==place_w+screen_w){
				r = 0xff<<hb.high;
				leddata[(place_h+1)*scr_len-1] &= r;
				leddata[(place_h+screen_h)*scr_len-1] &= r;	
			}
		}
	}
	GREEN_BEGIN
		if(inf&0x20){  //˫ɫ     
			for(i=place_w;i<place_w+screen_w;i++){ 
				leddata[(place_h+OFFSET_GREEN)*scr_len+i] = up;
				leddata[(OFFSET_GREEN+place_h+screen_h-1)*scr_len+i] = down;
			}
			if(hb.high){  //������ߵĸ߶ȷ�0
				if(!place_w){
					l=0xff>>hb.high;
					leddata[(place_h+OFFSET_GREEN)*scr_len] &= l;
					leddata[(place_h+OFFSET_GREEN+screen_h-1)*scr_len] &= l;
				}
				if(scr_len==place_w+screen_w){
					r = 0xff<<hb.high;
					leddata[(place_h+OFFSET_GREEN+1)*scr_len-1] &= r;
					leddata[(place_h+OFFSET_GREEN+screen_h)*scr_len-1] &= r;	
				}
			}
		}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR
		if(inf&0x40){ //��ɫ
			for(i=place_w;i<place_w+screen_w;i++){ 
				leddata[(place_h+OFFSET_BLUE)*scr_len+i] = up;
				leddata[(OFFSET_BLUE+place_h+screen_h-1)*scr_len+i] = down;
			}
			if(hb.high){ //������ߵĸ߶ȷ�0
				if(!place_w){
					l=0xff>>hb.high;
					leddata[(place_h+OFFSET_BLUE)*scr_len] &= l;
					leddata[(place_h+OFFSET_BLUE+screen_h-1)*scr_len] &= l;
				}
				if(scr_len==place_w+screen_w){
					r = 0xff<<hb.high;
					leddata[(place_h+OFFSET_BLUE+1)*scr_len-1] &= r;
					leddata[(place_h+OFFSET_BLUE+screen_h)*scr_len-1] &= r;	
				}
			}
		}
	  #endif
}

/**************************************************************************
** �������� �� hb_deal
** �������� �� ���ߴ���
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��12���޸�	
**************************************************************************/
void hb_deal()
{
	unsigned char d,j,r1,r2,size_green=OFFSET_GREEN;
	unsigned short len,i;
	register unsigned short scr_len = SCREEN_LEN;
	unsigned long addr,addr_green;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned char size_blue=OFFSET_BLUE;
		unsigned long addr_blue;
		unsigned short blue_len=scr_len*size_blue;
	#endif
	register unsigned short temp;
	unsigned short green_len=scr_len*size_green;
	unsigned char r,l; 
	r=0xff>>hb.high; //���ݻ��߸߶����û����Ҳ�
	l=0xff<<hb.high; //���ݻ��߸߶����û������

	scr_info.is_init_hb=0; 
 	r1=scr_info.high-hb.high;
	addr=hb.addr;
	if(8*hb.width<=r1-hb.high)r2=8*hb.width;
	else r2=r1-hb.high;
	addr_green=hb.addr+hb.width*hb.high*2+16*hb.width;
	#ifdef CONFIG_SEVEN_COLOR
		addr_blue=addr_green+hb.width*hb.high*2+16*hb.width;
	#endif
/*********************************************************************************/
 	flash_read(addr+hb.width*hb.high*2);		//��
	for(i=hb.high;i<r2+hb.high;i++){ 	
		hb_l[i] = d = srs();	  
		temp=i*scr_len;
		leddata[temp] &= r;
		leddata[temp] |= d;  
	}
	GREEN_BEGIN
 		flash_read(addr_green+hb.width*hb.high*2);		//��
		for(i=hb.high;i<r2+hb.high;i++){ 	
			hb_lgreen[i] = d = srs();	  
			temp=i*scr_len+green_len;
			leddata[temp] &= r;
			leddata[temp] |= d;  
		}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR
 		flash_read(addr_blue+hb.width*hb.high*2);		//��
		for(i=hb.high;i<r2+hb.high;i++){ 	
			hb_lblue[i] = d = srs();	  
			temp=i*scr_len+blue_len;
			leddata[temp] &= r;
			leddata[temp] |= d;  
		}
	#endif

 	flash_read(addr+hb.width*hb.high*2+8*hb.width);			//��
	for(i=hb.high;i<r2+hb.high;i++){ 
		hb_r[i] = d = srs();	 
		temp=(i+1)*scr_len-1;
		leddata[temp] &= l;
		leddata[temp] |= d;
	}		  
	GREEN_BEGIN
 		flash_read(addr_green+hb.width*hb.high*2+8*hb.width);	 //��
		for(i=hb.high;i<r2+hb.high;i++){ 
			hb_rgreen[i] = d = srs();	  
			temp=(i+1)*scr_len+green_len-1; 
			leddata[temp] &= l;
			leddata[temp] |= d;
		}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR	
 		flash_read(addr_blue+hb.width*hb.high*2+8*hb.width);	//��
		for(i=hb.high;i<r2+hb.high;i++){ 
			hb_rblue[i] = d = srs();	  
			temp=(i+1)*scr_len+blue_len-1;
			leddata[temp] &= l;
			leddata[temp] |= d;
		}
	#endif			
	for(i=hb.high;i<8*(hb.width)+hb.high;i++){
		for(j=8*hb.width;j<r1;j+=8*hb.width){ 
			if((j+i)>=64)break;//2014.05.13 ����64��Խ��
			temp=(j+i)*scr_len;
			leddata[temp]&=r;
			leddata[temp]|= hb_l[i];  
			hb_l[j+i]=hb_l[i];
			temp=(j+i+1)*scr_len-1;
			leddata[temp]&=l;
			leddata[temp]|=hb_r[i];  
			hb_r[j+i]=hb_r[i];
			GREEN_BEGIN
				temp=(j+i)*scr_len+green_len;
				leddata[temp]&=r;
				leddata[temp]|= hb_lgreen[i];  
				hb_lgreen[j+i]=hb_lgreen[i];
				temp=(j+i+1)*scr_len+green_len-1;
				leddata[temp]&=l;
				leddata[temp]|=hb_rgreen[i];  
				hb_rgreen[j+i]=hb_rgreen[i];				
			GREEN_END
			#ifdef CONFIG_SEVEN_COLOR
				temp=(j+i)*scr_len+blue_len;
				leddata[temp]&=r;
				leddata[temp]|= hb_lblue[i];   
				hb_lblue[j+i]=hb_lblue[i];
				temp=(j+i+1)*scr_len+blue_len-1;
				leddata[temp]&=l;
				leddata[temp]|=hb_rblue[i];  
				hb_rblue[j+i]=hb_rblue[i];				
			#endif 
		}
	} 
/***********************************************************************************/
  		
	flash_read(addr);
    for(j=0;j<hb.high;j++) 					//��
		for(i=0;i<hb.width;i++){   
			leddata[j*scr_len+i]=srs();
		}
	GREEN_BEGIN
 		flash_read(addr_green);
	    for(j=0;j<hb.high;j++) 					//��
			for(i=0;i<hb.width;i++){  
				leddata[j*scr_len+green_len+i]=srs();	  
			}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR
 		flash_read(addr_blue);
	    for(j=0;j<hb.high;j++) 					//��
			for(i=0;i<hb.width;i++){
				leddata[j*scr_len+blue_len+i]=srs();
			}
	#endif

 	flash_read(addr+hb.width*hb.high);						   //��
	temp=r1*scr_len;
	for(j=0;j<hb.high;j++)
		for(i=0;i<hb.width;i++){
			leddata[j*scr_len+temp+i]=srs();	
		} 
	GREEN_BEGIN
 		flash_read(addr_green+hb.width*hb.high);						   //��
		temp=r1*scr_len+green_len;
		for(j=0;j<hb.high;j++)
			for(i=0;i<hb.width;i++){
				leddata[j*scr_len+temp+i]=srs();	
			}
	GREEN_END
	#ifdef CONFIG_SEVEN_COLOR
 		flash_read(addr_blue+hb.width*hb.high);
		temp=r1*scr_len+blue_len;						   //��
		for(j=0;j<hb.high;j++)
			for(i=0;i<hb.width;i++){
				leddata[j*scr_len+temp+i]=srs();	
			}
	#endif

	len=scr_len/hb.width;
   	for(j=0;j<hb.high;j++)
   		for(i=1;i<len;i++)
			for(r2=0;r2<hb.width;r2++){	
				leddata[j*scr_len+i*hb.width+r2]=leddata[j*scr_len+r2];
				leddata[(j+r1)*scr_len+i*hb.width+r2]=leddata[(j+r1)*scr_len+r2];
			}
	GREEN_BEGIN	   
		temp=green_len;
	  	for(j=0;j<hb.high;j++)
	   		for(i=1;i<len;i++)
				for(r2=0;r2<hb.width;r2++){	
					leddata[j*scr_len+i*hb.width+temp+r2]=leddata[j*scr_len+temp+r2];
					leddata[(j+r1)*scr_len+temp+i*hb.width+r2]=leddata[(j+r1)*scr_len+temp+r2];
				}
	GREEN_END
	#ifdef CONIFG_SEVEN_COLOR
		temp=blue_len;
	  	for(j=0;j<hb.high;j++)
	   		for(i=1;i<len;i++)
				for(r2=0;r2<hb.width;r2++){	
					leddata[j*scr_len+temp+i*hb.width+r2]=leddata[j*scr_len+temp+r2];
					leddata[(j+r1)*scr_len+temp+i*hb.width+r2]=leddata[(j+r1)*scr_len+temp+r2];
				}
	#endif 
/*********************************************************************************/						 			  
}
 
/**************************************************************************
** �������� �� hb_move_deal
** �������� �� �����ƶ�����
** ��ڲ��� �� �ƶ������־ch
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��12���޸�	
**************************************************************************/
void hb_move_deal(unsigned char ch)//ch=1˳ʱ���ƶ�
{
	unsigned char x,r,_r,l,_l,j; 
	register unsigned char temp,scr_h,d,d2,step=hb.move_step;
	register unsigned short len;
	unsigned short i,offset_g=OFFSET_GREEN;
	#ifdef CONFIG_SEVEN_COLOR
		unsigned short offset_b=OFFSET_BLUE;
	#endif
	len=SCREEN_LEN;
	r=0xff>>hb.high; _r=~r;
	l=0xff<<hb.high; _l=~l;
	scr_h=scr_info.high-hb.high;

	if(ch){ //˳ʱ�� zzw12.30

		HB_SHIFT_RIGHT(0,0)  //�ϻ�������
		GREEN_BEGIN
			HB_SHIFT_RIGHT(0,offset_g)
		GREEN_END
		#ifdef CONFIG_SEVIN_COLOR
			HB_SHIFT_RIGHT(0,offset_b)
		#endif 

		HB_SHIFT_LEFT(scr_h,0)  //�»�������
		GREEN_BEGIN
			HB_SHIFT_LEFT(scr_h,offset_g)
		GREEN_END
		#ifdef CONFIG_SEVEN_COLOR
			HB_SHIFT_LEFT(scr_h,offset_b)
		#endif 
	}else{ 
		HB_SHIFT_RIGHT(scr_h,0)   
		GREEN_BEGIN
			HB_SHIFT_RIGHT(scr_h,offset_g)
		GREEN_END
		#ifdef CONFIG_SEVIN_COLOR
			HB_SHIFT_RIGHT(scr_h,offset_b)
		#endif

		HB_SHIFT_LEFT(0,0)  //�ϻ�������
		GREEN_BEGIN
			HB_SHIFT_LEFT(0,offset_g)
		GREEN_END 
		#ifdef CONFIG_SEVEN_COLOR
			HB_SHIFT_LEFT(0,offset_b)
		#endif
	}

	for(i=hb.high;i<scr_h;i++){
		leddata[i*len] &= r;            //�����������
		leddata[i*len] |= (hb_l[i]&_r); //���������ʾ�Ļ���
		leddata[(i+1)*len-1] &= l;      //�����Ҳ�����
		leddata[(i+1)*len-1] |= (hb_r[i]&_l);//���������ʾ�Ļ���
		GREEN_BEGIN
			leddata[(i+offset_g)*len] &= r;
			leddata[(i+offset_g)*len] |= (hb_lgreen[i]&_r);
			leddata[(i+offset_g+1)*len-1] &= l;
			leddata[(i+offset_g+1)*len-1] |= (hb_rgreen[i]&_l);
		GREEN_END
		#ifdef CONFIG_SEVEN_COLOR
			leddata[(i+offset_b)*len] &= r;
			leddata[(i+offset_b)*len] |= (hb_lblue[i]&_r);
			leddata[(i+offset_b+1)*len-1] &= l;
			leddata[(i+offset_b+1)*len-1] |= (hb_rblue[i]&_l);
	    #endif
	} 
}

/**************************************************************************
** �������� �� clear_hb
** �������� �� �������
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� ������ ��2013��03��12���޸�	
**************************************************************************/
void clear_hb()
{ 
	unsigned char i,j,scr_h,r,l;
	unsigned short len=SCREEN_LEN;
	r=0xff>>hb.high; //���ߵ����
	l=0xff<<hb.high; //���ߵ��Ҳ�
	scr_h=scr_info.high-hb.high;
	for(j=0;j<hb.high;j++){
		for(i=0;i<len;i++){
			leddata[j*len+i]=0;
			leddata[(j+scr_h)*len+i]=0;	
			GREEN_BEGIN
				leddata[(j+OFFSET_GREEN)*len+i]=0;
				leddata[(j+OFFSET_GREEN+scr_h)*len+i]=0;
			GREEN_END
			#ifdef CONFIG_SEVEN_COLOR	
				leddata[(j+OFFSET_BLUE)*len+i]=0;
				leddata[(j+OFFSET_BLUE+scr_h)*len+i]=0;
			#endif									
		}
	}
	for(j=hb.high;j<scr_h;j++){
		leddata[j*len]&=r;
		leddata[(j+1)*len-1]&=l;
		GREEN_BEGIN
			leddata[(j+OFFSET_GREEN)*len]&=r;
			leddata[(j+OFFSET_GREEN+1)*len-1]&=l;
		GREEN_END
		#ifdef CONFIG_SEVEN_COLOR	
			leddata[(j+OFFSET_BLUE)*len]&=r;
			leddata[(j+OFFSET_BLUE+1)*len-1]&=l;
		#endif						
	}		 	
}

