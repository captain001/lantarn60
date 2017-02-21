#include "clock.h"
#include "led.h"
#include "hb.h"

//����ȫ�ֱ���������ڣ����������գ�ʱ�䣺ʱ����
unsigned long date32 ,time32; //������ڡ�ʱ��

//�����ⲿ����
extern unsigned long flash_addr;  //flash�ĵ�ַ
extern unsigned short screen_len; //����(���ֽ�)
extern unsigned char offset_green;//��ɫ���ݵ�ƫ����
extern unsigned char area_num;    //������

//�����ⲿ�ṹ�����
extern struct ScrStruct  scr_info;//�����Խṹ
extern struct AreaStruct *pArg;   //�������Խṹ

extern unsigned char area_c,pro_c;//��������������Ŀ������

//�����ⲿ�������
extern unsigned char buf[40];
extern unsigned char leddata[8*1024];    //��ʾ��������
extern unsigned char sim_buf_red[1024];  //��ɫ������ 
extern unsigned char sim_buf_green[1024];//��ɫ������
extern unsigned char sim_buf_blue[1024]; //��ɫ������

/**************************************************************************
** �������� �� __dig_write
** �������� �� ģ��ʱ��д����
** ��ڲ��� �� ������x��������y������w���߶�h����ɫcolor
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
void __dig_write(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned char color)
{ 
  unsigned char j,i,d,zs,ys,wzs,wys;
  zs=x/8; ys=x%8;  wys=w%8;
  if(wys>0)wzs=w/8+1;
  else wzs=w/8;
	
  flash_read(flash_addr);
  if(color&0x01){ //��ɫ
	  for(j=y;j<y+h;j++){
	   for(i=zs;i<zs+wzs;i++){
	    d=srs();
	    sim_buf_red[j*16+i] |= d>>ys; 
		sim_buf_red[j*16+i+1] |= d<<(8-ys);
	   } 
	  }
   }

  flash_read(flash_addr);
  if(color&0x02){ //˫ɫ
	  for(j=y;j<y+h;j++){
	   for(i=zs;i<zs+wzs;i++){
	    d=srs();
	    sim_buf_green[j*16+i] |= d>>ys; 
		sim_buf_green[j*16+i+1] |= d<<(8-ys);
	   } 
	  }
   }
 #ifdef CONFIG_SEVEN_COLOR  //��ɫ
  	  flash_read(flash_addr);
	  if(color&0x04){
		  for(j=y;j<y+h;j++){ 
		   for(i=zs;i<zs+wzs;i++){
		    d=srs();
		    sim_buf_blue[j*16+i] |= d>>ys; 
			sim_buf_blue[j*16+i+1] |= d<<(8-ys);
		   } 
		  }
	   } 
 #endif
}

/**************************************************************************
** �������� �� dig_write
** �������� �� ����ʱ��д����
** ��ڲ��� �� ������x��������y������w���߶�h����ɫcolor
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
void dig_write(unsigned short x,unsigned char y,unsigned char w,unsigned char h,unsigned char color)
{ 
  
  unsigned char j,i,d,zs,ys,wzs,wys,ch=0;
  unsigned char xwz,xwy,save=0,save1=0;
  unsigned short scr_len = SCREEN_LEN;
  unsigned short temp,size_green = scr_len*OFFSET_GREEN;
  #ifdef CONFIG_SEVEN_COLOR
  	size_blue = scr_len*OFFSET_BLUE;
  #endif
  zs=x/8; 
  ys=x%8;  
  wys=w%8;
  xwz=(x+w)/8;
  xwy=(x+w)%8;
  if(wys>0)wzs=w/8+1;
  else wzs=w/8;

  if((8-ys)>=w)ch=1;
  if(color&0x01){ //��ɫ
  	flash_read(flash_addr);
	  for(j=y;j<y+h;j++){
	   if((x+w)%8>0){
	   	save = leddata[j*scr_len+xwz] & (0xff>>xwy);
		save1 = leddata[j*scr_len+xwz+1] & (0xff<<(8-xwy));
	   }	
	   for(i=zs;i<zs+wzs;i++){	
	    d=srs();
		temp = j*scr_len+i; 
		leddata[temp] &= 0xff<<(8-ys);
		leddata[temp] |= d>>ys; 
		if(!ch){
			leddata[temp+1] &= 0xff>>ys;
			leddata[temp+1] |= d<<(8-ys);
		}
	   }
	   if((x+w)%8>0){
	   	leddata[j*scr_len+xwz] |= save;
		leddata[j*scr_len+xwz+1] |= save1; 
	   }
	  }
   }
  if(color&0x02){ //˫ɫ
  	flash_read(flash_addr);
	  for(j=y;j<y+h;j++){
	   if((x+w)%8>0){
	   	save = leddata[j*scr_len+size_green+xwz] & (0xff>>xwy);
		save1 = leddata[j*scr_len+size_green+xwz+1] & (0xff<<(8-xwy));
	   }
	   for(i=zs;i<zs+wzs;i++){
	    d=srs();
		temp = j*scr_len+size_green+i;
		leddata[temp] &= 0xff<<(8-ys);
	    leddata[temp] |= d>>ys; 
		if(!ch){
			leddata[temp+1] &= 0xff>>ys;
			leddata[temp+1] |= d<<(8-ys);
		}
	   }
	   if((x+w)%8>0){
	   	leddata[j*scr_len+size_green+xwz] |= save;
		leddata[j*scr_len+size_green+xwz+1] |= save1; 
	   }	    
	  }
   }
 #ifdef CONFIG_SEVEN_COLOR
  	  flash_read(flash_addr);
	  if(color&0x04){ //��ɫ
		  for(j=y;j<y+h;j++){ 
		   if((x+w)%8>0){
		   	save = leddata[j*scr_len+size_blue+xwz] & (0xff>>xwy);
			save1 = leddata[j*scr_len+size_blue+xwz+1] & (0xff<<(8-xwy));
		   }
		   for(i=zs;i<zs+wzs;i++){
		    d=srs();
			temp = j*scr_len+size_blue+i;
			leddata[temp] &= 0xff<<(8-ys);
		    leddata[temp] |= d>>ys; 
			if(!ch){
				leddata[temp+1] &= 0xff>>ys;
				leddata[temp+1] |= d<<(8-ys);
			}
		   } 
		   if((x+w)%8>0){
		   	leddata[j*scr_len+size_blue+xwz] |= save;
			leddata[j*scr_len+size_blue+xwz+1] |= save1; 
		   }
		  }
	   } 
 #endif
}

/**************************************************************************
** �������� �� asc_check
** �������� �� ʱ����ʾ���ж�
** ��ڲ��� �� ����ʱ��ı��tmp����ʾ�ĸ�8λ����8λ�ı��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
unsigned char asc_check(unsigned char tmp,unsigned char n)
{
	unsigned char d=0;
	switch(tmp){	  
 	case(0x59)://asc��
		d=	n?(date32>>24)%10:(date32>>24)/10;
		if(scr_info.fg3){if(n){d=0;scr_info.fg3=0;}else d=2;}
		goto asc;
	case(0x4d)://asc��
		d=  n?((date32>>8)&0xff)%10:((date32>>8)&0xff)/10;
		goto asc; 
	case(0x44)://asc��
		d=  n?(date32&0xff)%10:(date32&0xff)/10;
		goto asc;
	case(0x48)://ascʱ
		d=  n?((time32>>16)&0xff)%10:((time32>>16)&0xff)/10;
		goto asc;
	case(0x6d)://asc��
		d=  n?((time32>>8)&0xff)%10:((time32>>8)&0xff)/10;
		goto asc;
	case(0x73)://asc��
		d=  n?(time32&0xff)%10:(time32&0xff)/10;
		goto asc;
	case(0x2f):// /
	case(0x2e):// .
	case(0x2d):// -
		scr_info.fg2=1;
		d= 10;
		goto asc;
	case(0x3a)://:
		scr_info.fg2=1;
		d= 11;
		goto asc;
	case(0x6e)://������
		d= 0;
		goto hz;
	case(0x79)://������
		d= 1;
		goto hz;
	case(0x72)://������
		d= 2;
		goto hz;
 
	case(0x7a)://����ʱ
		d= 3;
		goto hz;
	case(0x66)://���ַ�
		d= 4;
		goto hz;
	case(0x53)://������
		d= 5;
		goto hz;
 
	default:break;
	}
asc:
	scr_info.fg1=0;
	return d;
hz:
	scr_info.fg1=1;
	return d;		
}

/**************************************************************************
** �������� �� dig_clock_deal
** �������� �� ����ʱ�Ӵ���
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
void dig_clock_deal()
{
	unsigned char len,n,d,i,asc_vir,hz_vir,fg,color_ch=0,week_w,week_h;
	unsigned short width;
	struct DigiStruct{
					unsigned char show_flag;   //��ʾ���
					unsigned short asc_w;      //�ַ�����
					unsigned short asc_h;      //�ַ��߶�
					unsigned short hz_w;       //���ֳ���
					unsigned short hz_h;       //���ָ߶�
					unsigned long format_addr; //��ʽ��ַ
					unsigned long asc_addr;    //�ַ��ֿ��ַ
					unsigned long hz_addr;     //�����ֿ��ַ
					unsigned char color_format;	 //��ɫ��ʽ
					unsigned char date_color;  //������ɫ
					unsigned char week_color;  //������ɫ
					unsigned char time_color;  //ʱ����ɫ
					unsigned short date_place_w;//���ڳ���
					unsigned short date_place_h;//���ڸ߶�
					unsigned short week_place_w;//���ڳ���
					unsigned short week_place_h;//���ڸ߶�
					unsigned short time_place_w;//ʱ�䳤��
					unsigned short time_place_h;//ʱ��߶�
					unsigned short hz_size;     //���ִ�С
					unsigned short asc_size;    //�ַ���С
	}dig; 
	pArg->is_init_dig=0;	
	flash_addr=pArg->info_content_addr;
	read_buf(38,buf,flash_addr);
	dig.show_flag = buf[0];
	dig.asc_w = (unsigned short)buf[1]<<8 | (unsigned short)buf[2];
	dig.asc_h = (unsigned short)buf[3]<<8 | (unsigned short)buf[4];
	dig.hz_w  = (unsigned short)buf[5]<<8 | (unsigned short)buf[6];
	dig.hz_h  = (unsigned short)buf[7]<<8 | (unsigned short)buf[8];
	dig.format_addr = (unsigned long)buf[10]<<16 | (unsigned long)buf[11]<<8 | (unsigned long)buf[12];
	dig.asc_addr = (unsigned long)buf[14]<<16 | (unsigned long)buf[15]<<8 | (unsigned long)buf[16];
	dig.hz_addr  = (unsigned long)buf[18]<<16 | (unsigned long)buf[19]<<8 | (unsigned long)buf[20];
	dig.color_format = buf[21];
	dig.date_color = buf[22];
	dig.week_color = buf[23];
	dig.time_color = buf[24];	
	dig.date_place_w = (unsigned short)buf[25]<<8 | (unsigned short)buf[26];
	dig.date_place_h = (unsigned short)buf[27]<<8 | (unsigned short)buf[28];
	dig.week_place_w = (unsigned short)buf[29]<<8 | (unsigned short)buf[30];
	dig.week_place_h = (unsigned short)buf[31]<<8 | (unsigned short)buf[32];
	dig.time_place_w = (unsigned short)buf[33]<<8 | (unsigned short)buf[34];
	dig.time_place_h = (unsigned short)buf[35]<<8 | (unsigned short)buf[36];

	dig.date_place_h += pArg->place_h;
	dig.week_place_h += pArg->place_h;
	dig.time_place_h += pArg->place_h;
	if(dig.color_format==0){
		dig.color_format=1;  //��ɫ��ʽ
		dig.date_color=dig.week_color=dig.time_color=0x01;
	}
 	if(dig.asc_w%8) asc_vir = dig.asc_w/8+1;
	else asc_vir = dig.asc_w/8; //�ַ����ȣ��ֽ�����
	if(dig.hz_w%8) hz_vir = dig.hz_w/8+1;
	else hz_vir = dig.hz_w/8;   //���ֳ��ȣ��ֽ�����
	dig.asc_size = asc_vir*dig.asc_h;	//�ַ���С	
	dig.hz_size = hz_vir*dig.hz_h;  //���ִ�С
 	flash_addr = dig.format_addr;
	n=flash_read(dig.format_addr);  //���ڸ�ʽ����
	read_buf(n,buf,flash_addr+1);
	
	width=pArg->place_w*8+dig.date_place_w;
	if(dig.show_flag&0x01) //�����ڲ���   
	{
		len=n;
		for(i=2;i<n;i++){
			if((buf[i]==0x6e)||(buf[i]==0x79)||(buf[i]==0x72)){//����������
				for(d=i;d<n;d++)buf[d]=buf[d+1];  len--;
			}
		}	   
		i=0; scr_info.fg3=0;
		if(buf[1]==0x4d)scr_info.fg3=1;	 //�ַ���
		if(buf[2]==0x59){	//�ַ���
		    flash_addr=dig.asc_addr+2*dig.asc_size;		
 			if(1==dig.color_format || 2==dig.color_format)color_ch=dig.date_color;
		    dig_write(width,dig.date_place_h,dig.asc_w,dig.asc_h,color_ch);	  
		    flash_addr = dig.asc_addr; 
		    dig_write(width+dig.asc_w,dig.date_place_h,dig.asc_w,dig.asc_h,color_ch);
		    i+=2; width+=2*dig.asc_w;
		} 
		
		fg=0; scr_info.fg2=0;
		for(;i<len;i++){
			d=asc_check(buf[i],fg);//fg=0��ȡ��λ fg=1��ȡ��λ
			if(scr_info.fg1){ //scr_info.fg1=1���� ??????
				flash_addr=dig.hz_addr+d*dig.hz_size;	fg=0;
				if(1==dig.color_format)color_ch=dig.date_color;//������ɫ
				if(2==dig.color_format)color_ch=dig.week_color;//������ɫ
			}else{
				flash_addr=dig.asc_addr+d*dig.asc_size;
				if(1==dig.color_format || 2==dig.color_format)color_ch=dig.date_color;
				if(!scr_info.fg2) fg=!fg;
				else scr_info.fg2=0;
			}
			dig_write(width,dig.date_place_h,scr_info.fg1?dig.hz_w:dig.asc_w,scr_info.fg1?dig.hz_h:dig.asc_h,color_ch);  
			width+=scr_info.fg1?dig.hz_w:dig.asc_w;
		}
	} 
	if(dig.show_flag&0x04){	  //�����ڲ���
		width=pArg->place_w*8+dig.week_place_w;
		i=flash_read(dig.format_addr);//�������ڵĸ�ʽ����
		flash_addr=dig.format_addr+1+i;
		i=flash_read(flash_addr); //����ʱ��ĸ�ʽ����
		flash_addr=	flash_addr+1+i;  //����������ʼ��ַ
		flash_read(flash_addr);  
		week_w=srs(); week_w /=8; //���ڵ���ʾ���ȣ��ֽ�����
		week_h=srs();             //���ڵ���ʾ�߶�
		d=date32>>16; d-=1;
		flash_addr=flash_addr+2+d*week_w*week_h;
		color_ch=dig.week_color;	
		dig_write(width,dig.week_place_h,week_w*8,week_h,color_ch);
	}
	pArg->miao_pos = 0xee;  //����
	if(dig.show_flag&0x02){	 //��ʱ�����
		width=pArg->place_w*8+dig.time_place_w;
		flash_addr = dig.format_addr+1+n;
		n=flash_read(flash_addr);//ʱ���ʽ�ĸ���
		read_buf(n,buf,flash_addr+1);
		
		pArg->info_data_w = width;  //���ڳ���
		pArg->info_data_h = dig.time_place_h;//���ڸ߶�
		
		for(i=0;i<15;i++)
			if(buf[i]==0x6d)break;//������ַ��֣�������forѭ��
		if(buf[i-1]!=0x7a)pArg->fen_pos=3*dig.asc_w+width;
		else pArg->fen_pos=2*dig.asc_w+dig.hz_w+width;
 		for(i=0;i<15;i++)
			if(buf[i]==0x73)break; //���Ϊ�ַ��룬������forѭ��
		if(i!=15){ //i��Ϊ15����˵������
			if(buf[i-1]!=0x66)pArg->miao_pos=pArg->fen_pos+3*dig.asc_w;
			else pArg->miao_pos=pArg->fen_pos+2*dig.asc_w+dig.hz_w;
 		 }
		 else pArg->miao_pos=0xaa;				//û����
	
		 len=n;
		 for(i=2;i<n;i++){
			 if((buf[i]==0x7a)||(buf[i]==0x66)||(buf[i]==0x53)){
				 for(d=i;d<n;d++)buf[d]=buf[d+1];  len--;
			 }
		 }		 
		 fg=0; scr_info.fg2=0;
		 for(i=0;i<len;i++){
			d=asc_check(buf[i],fg);
			if(scr_info.fg1){ //����
				flash_addr=dig.hz_addr+d*dig.hz_size; fg=0;
				if(1==dig.color_format)color_ch=dig.time_color;
				if(2==dig.color_format)color_ch=dig.week_color;
			}else{
				flash_addr=dig.asc_addr+d*dig.asc_size;
				if(1==dig.color_format)color_ch=dig.time_color; //ʱ����ɫ
				if(2==dig.color_format)color_ch=dig.date_color; //������ɫ
				pArg->info_exit_mode=color_ch;//���������ɫ
				if(!scr_info.fg2)fg=!fg;
				else scr_info.fg2=0; 
			} 
			dig_write(width,dig.time_place_h,scr_info.fg1?dig.hz_w:dig.asc_w,scr_info.fg1?dig.hz_h:dig.asc_h,color_ch);
			width+=scr_info.fg1?dig.hz_w:dig.asc_w;
		}
	}
	pArg->asc_addr = dig.asc_addr;  //�����ַ���ַ
	pArg->parameter = (unsigned long)dig.asc_h<<16 | (unsigned long)dig.asc_w; //�����ַ��ĳ���
	pArg->asc_size = dig.asc_size;	 //�����ַ���С

	pArg->st=1;  
	pArg->end=1;
}

/**************************************************************************
** �������� �� time_count
** �������� �� ��ʱ
** ��ڲ��� �� ��
** ���ز��� �� ��
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
void time_count()
{
	  unsigned char h,m,s,i,len=0; 
	  unsigned long time,addr=0;
	  unsigned short temp,width;
	  struct CntStruct{
	  				 unsigned char info_format; 
					   unsigned char asc_w;	     
					   unsigned char asc_h;      
					   unsigned char title_color; 
					   unsigned char time_color;  
					   unsigned char title_place_w;
					   unsigned char title_place_h;
					   unsigned char time_place_w; 
					   unsigned char time_place_h; 
					   unsigned short title_w;     
					   unsigned short day_w;       
					   unsigned short h_w;         
					   unsigned short m_w;         
					   unsigned short s_w;        
					   volatile unsigned long addr;
					   unsigned char asc_vir;      
					   unsigned char title_vir;   
					   unsigned char day_vir;      
					   unsigned char h_vir;        
					   unsigned char m_vir;        
					   unsigned char s_vir;       
	  }cnt;	
	  pArg->is_init_count=0; 
	  flash_addr=pArg->info_content_addr;  
	  read_buf(29,buf,flash_addr);
	  cnt.info_format = buf[0];	  
	  cnt.asc_w = buf[2];	    
	  cnt.asc_h = buf[4];	    
	  cnt.title_color = buf[5];	   
	  cnt.time_color = buf[6];	  
	  cnt.title_place_w = buf[8];   
	  cnt.title_place_h = buf[10];	
	  cnt.time_place_w = buf[12];	  
	  cnt.time_place_h = buf[14];
	  cnt.title_w    = (unsigned short)buf[15]<<8 | (unsigned short)buf[16];  //00 00
	  cnt.day_w = (unsigned short)buf[17]<<8 | (unsigned short)buf[18];	 // 10
	  cnt.h_w = (unsigned short)buf[19]<<8 | (unsigned short)buf[20];	// 10
	  cnt.m_w  = (unsigned short)buf[21]<<8 | (unsigned short)buf[22];	// 10
	  cnt.s_w = (unsigned short)buf[23]<<8 | (unsigned short)buf[24];	 // 10
	  cnt.addr = (unsigned long)buf[26]<<16 | (unsigned long)buf[27]<<8 |(unsigned long)buf[28];
	  cnt.title_place_h+=pArg->place_h;
	  cnt.time_place_h+=pArg->place_h;

	  if(cnt.asc_w%8>0)cnt.asc_vir = cnt.asc_w/8+1;
	  else cnt.asc_vir = cnt.asc_w/8; //�ַ�����ֽ���
	  if(cnt.title_w%8>0)cnt.title_vir = cnt.title_w/8+1;
	  else cnt.title_vir = cnt.title_w/8; //�������ֽ���
	  
	  if(cnt.day_w%8>0)cnt.day_vir = cnt.day_w/8+1;
	  else cnt.day_vir = cnt.day_w/8;  //�����ֽ��� 
	  if(cnt.h_w%8>0)cnt.h_vir = cnt.h_w/8+1;
	  else cnt.h_vir = cnt.h_w/8;      //ʱ����ֽ���
	  if(cnt.m_w%8>0)cnt.m_vir = cnt.m_w/8+1;
	  else cnt.m_vir = cnt.m_w/8;      //�ֿ���ֽ���
	  if(cnt.s_w%8>0)cnt.s_vir = cnt.s_w/8+1;
	  else cnt.s_vir = cnt.s_w/8;     //�����ֽ���
	
	  flash_addr=pArg->info_content_addr+29;  
	  read_buf(8,buf,flash_addr); 		
	  time = settime(buf,1);					  
	  pArg->info_data_h = time;	
 	  if(time > RTC_GetCounter()) time -= RTC_GetCounter();
	  else 
	  	time = RTC_GetCounter()-time;//ʱ��Ĳ�ֵ
 
	  pArg->info_data_w = (time/(24*3600))%1000;//��     
	  time %= 24*3600;
	  h = time/3600; 	//ʱ
 	  m = (time%3600)/60;//��
 	  s = (time%3600)%60;	//��  
	
	  width = pArg->place_w*8+cnt.title_place_w;
	  if(cnt.info_format&0x20){	//��ʾ����
		   flash_addr = cnt.addr+10*cnt.asc_vir*cnt.asc_h;
		   dig_write(width,cnt.title_place_h,cnt.title_w,cnt.asc_h,cnt.title_color);
 	  }		
	  if(cnt.info_format&0x10){	 //��ʾ��λ
		   	buf[0]=pArg->info_data_w/100;
			buf[1]=(pArg->info_data_w%100)/10;
			buf[2]=(pArg->info_data_w%100)%10;
			buf[3]=0x80;											//��
			buf[4]=h/10;
			buf[5]=h%10;
			buf[6]=0x81;											//ʱ
			buf[7]=m/10;
			buf[8]=m%10;
			buf[9]=0x82;											//��
			buf[10]=s/10;
			buf[11]=s%10; 
			buf[12]=0x83;											//��
			if(!buf[0])buf[0]=0x99;
			if( ( !buf[1] ) && ( buf[0] == 0x99 ) )buf[1]=0x99; //zzw 12.6.8
 	   }else{
			buf[0]=pArg->info_data_w/100;      //��
			buf[1]=(pArg->info_data_w%100)/10;
			buf[2]=(pArg->info_data_w%100)%10;
			buf[3]=0xaa;     
			buf[4]=h/10;    //ʱ
			buf[5]=h%10;
			buf[6]=0xaa;
			buf[7]=m/10;   //�� 
			buf[8]=m%10;
			buf[9]=0xaa;
			buf[10]=s/10;   //��
			buf[11]=s%10;
			buf[12]=0xaa;
			if(!buf[0])buf[0]=0x99;
			if( ( !buf[1] ) && ( buf[0] == 0x99 ) )buf[1]=0x99; //zzw 12.6.8
 	   }
	   if(cnt.title_place_h==cnt.time_place_h)temp=pArg->screen_w*8-cnt.title_w;
	   else temp=pArg->screen_w*8;

	   if(temp%8>0)temp=temp/8+1;
	   else temp/=8;
 	   if(cnt.info_format&0x01)len=13;
	   else if(cnt.info_format&0x02)len=10;
	   else if(cnt.info_format&0x04)len=7;
	   else if(cnt.info_format&0x08)len=4; 
	   width = pArg->place_w*8+cnt.time_place_w;
	   for(i=0;i<len;i++){
			if(buf[i]==0x99){ 
				width+=cnt.asc_w;
				continue; 
			}
			else if((buf[i]&0xf0)==0x80){
				if(buf[i]==0x80){
					flash_addr = cnt.addr+10*cnt.asc_vir*cnt.asc_h+cnt.title_vir*cnt.asc_h;
					addr=flash_addr;
					dig_write(width,cnt.time_place_h,cnt.day_w,cnt.asc_h,cnt.time_color);
					width+=cnt.day_w;
				}
				else if(buf[i]==0x81){
					addr+=cnt.day_vir*cnt.asc_h;
 					flash_addr = addr;
					dig_write(width,cnt.time_place_h,cnt.h_w,cnt.asc_h,cnt.time_color);
					width+=cnt.h_w;
				}
				else if(buf[i]==0x82){
 					addr+= cnt.h_vir*cnt.asc_h;
					flash_addr=addr;
					dig_write(width,cnt.time_place_h,cnt.m_w,cnt.asc_h,cnt.time_color);
					width+=cnt.m_w;
				}
				else if(buf[i]==0x83){
					addr+= cnt.m_vir*cnt.asc_h;
					flash_addr=addr; 
					dig_write(width,cnt.time_place_h,cnt.s_w,cnt.asc_h,cnt.time_color);
				}
			}
			else if(buf[i]!=0xaa){
				flash_addr = cnt.addr+buf[i]*cnt.asc_h*cnt.asc_vir;
				dig_write(width,cnt.time_place_h,cnt.asc_w,cnt.asc_h,cnt.time_color);
				width += cnt.asc_w;
			}
		}
	 

		pArg->st=1;  
		pArg->end=1;
} 

/**************************************************************************
** �������� �� show_timers
** �������� �� ��ʱ����
** ��ڲ��� �� ��
** ���ز��� �� ����1˵�������ţ�����0˵������
** ����˵�� �� 
** �޸���־ �� 2013.3.18 ������	
**************************************************************************/
unsigned char show_timers()     
{
	u8 i;
	u32 date1,date2,time1,time2;
	 
	read_buf(16,buf,flash_addr);
 	date1=date2=0;
	for(i=1;i<5;i++){
	date1<<=8;
	date1|=buf[i];
	}
	for(i=5;i<9;i++){
	date2<<=8;
	date2|=buf[i];
	}
	time1=((date32&0xff000000)>>8)|(date32&0xffff)|0x14000000;
	if((time1<date1)||(time1>date2))return 1;
	buf[0]=buf[0]>>1;
	i=date32>>16;
	if(((buf[0]<<i)&0x80)==0)return 1;
	time1=time2=0;
	for(i=9;i<12;i++){
	time1<<=8;
	time1|=buf[i];
	}
	for(i=12;i<15;i++){
	time2<<=8;
	time2|=buf[i];
	}
	if((time32<time1)||(time32>time2))return 1;
	return 0;
}
