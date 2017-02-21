#include "led.h"
#include "stmlib.h"
//#include <string.h>
unsigned char at_start[3]="+++";

unsigned char WiFi_AT_AT[3]="AT+";
// unsigned char WiFi_AT_WSMAC[6]="WSMAC\x0d";
unsigned char WiFi_AT_WMODE[9]="WMODE=AP\x0d";

unsigned char WiFi_AT_NETP[35]="NETP=TCP,Server,25622,192.168.47.1\x0d";

unsigned char WiFi_AT_WAKEY[17]="WAKEY=WPAPSK,AES,";
unsigned char WiFi_WAKEY_CONFIG2[9]="12345678\x0d";
#ifdef RHX_AP
//unsigned char WiFi_AT_WAP[14]="WAP=11BGN,RHX-";
unsigned char WiFi_AT_WAP[14]="WAP=11BGN,BMW-";

#elif JS_AP
unsigned char WiFi_AT_WAP[13]="WAP=11BGN,JS-";
#elif XGTD_AP
unsigned char WiFi_AT_WAP[15]="WAP=11BGN,XGTD-";
#endif
unsigned char WiFi_WAP_CONFIG2[6]=",Auto\x0d";

unsigned char WiFi_AT_LANN[32]="LANN=192.168.47.1,255.255.255.0\x0d";

unsigned char WiFi_AT_DHCPGW[20]="DHCPGW=192.168.47.1\x0d";

unsigned char WiFi_AT_Z[5]="AT+Z\x0d";   //reset
unsigned char timer1_flag=0;
unsigned char wifi_init_flag;           //上电启动 0；配置 1；正常工作 2；
unsigned char start_wifi_init=0,start_wifi_config=0;   //上电直接配制
u8 wifi_config=0,sendflag=0;	
u8 wifiinit=0;	

extern unsigned char usart_buf[200];
extern unsigned char buf_num;
unsigned char config_tim=0;
unsigned char config_step=0;
extern unsigned char sim_buf_red[]; 
unsigned char wifi_mac[6];

void send_at_start(void)
{
    send_string(at_start,sizeof(at_start));
}


// unsigned char WiFi_AT_WAKEY[]="WAKEY=WPAPSK,AES,";
// unsigned char WiFi_WAKEY_CONFIG2[]="12345678";
void send_at_WAKEY(void)
{
	u8 len;
	len=*(unsigned char*)0x0800FC40;
	if((len>0)&(len<20))
	{
		sendflag=1;
	    send_string(WiFi_AT_AT,sizeof(WiFi_AT_AT));
	    send_string(WiFi_AT_WAKEY,sizeof(WiFi_AT_WAKEY));
		send_string((unsigned char*)0x0800FC41,len);
		#ifdef ConMod1
		ut(0x0d);
		#elif ConMod2
		ut2(0x0d);
		#endif
	 }
	 else sendflag=0;
    
}
// unsigned char WiFi_AT_WAP[]="WAP=11BGN,RHX-";
// unsigned char WiFi_WAP_CONFIG2[]="Auto";
void send_at_WAP(void)
{
	u8 len;
	len=*(unsigned char*)0x0800FC56;
	if((len>0)&(len<20)) 
	{
		sendflag=3;
		send_string(WiFi_AT_AT,sizeof(WiFi_AT_AT));
		send_string(WiFi_AT_WAP,sizeof(WiFi_AT_WAP));
		send_string((unsigned char*)0x0800FC57,len);
		send_string(WiFi_WAP_CONFIG2,sizeof(WiFi_WAP_CONFIG2)); 
	}
	 else if(sendflag==1) 
		 sendflag=2;
	 else sendflag=0;
}

void send_at_reset(void)
{
//	unsigned int i;
//	send_string(WiFi_AT_Z,sizeof(WiFi_AT_Z));
//	i=0xfffff;
//	while(i--);
	send_string(WiFi_AT_Z,sizeof(WiFi_AT_Z));
}

void buf_clr(void)
{
    unsigned char i;
    buf_num=0;
    for(i=0;i<200;i++)
    {
        usart_buf[i]=0;     
    }
}




volatile unsigned char init_finish=0;

u8 ap_name[13];

u32 get_mcu_id(void)
{
		u32 ID;	    

                                             
	 ID=  (((*( u8 *)( 0x1FFFF7E8 ))) & 0xfF) ^(*(u8*)0x1FFFF7E9)^(*(u8*)0x1FFFF7EA)^(*(u8*)0x1FFFF7EB);

		ID<<=8;
 
		ID|=  (((*( u8 *)( 0x1FFFF7EC ))) & 0xfF) ^(*(u8*)0x1FFFF7ED)^(*(u8*)0x1FFFF7EE)^(*(u8*)0x1FFFF7EF);

	 ID<<=8;
	 ID|=  (((*( u8 *)( 0x1FFFF7F0 ))) & 0xfF) ^(*(u8*)0x1FFFF7F1)^(*(u8*)0x1FFFF7F2)^(*(u8*)0x1FFFF7F3);
									


return  ID;



}

#ifdef RHX_AP
void Get_SerialNum(void)
{
  u32 a;
  char aa;
  
	a=get_mcu_id();
		
// 	ap_name[0]='R';
// 	ap_name[1]='H';
// 	ap_name[2]='X';
// 	ap_name[3]='-';
	strcpy((char*)ap_name,"BMW-");
	strncpy((char*)&ap_name[4],WIFI_SSID_DEV,2);//WIFI_SSID_DEV WifiSsidFun(WifiSwitch(WIFI_SSID_DEV))
	 switch (((a>>20)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>20)&0x0f)+'0';
			 break;
        }

  }
	ap_name[6]=aa;
	
		 switch (((a>>16)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>16)&0x0f)+'0';
			 break;
        }

  }
	ap_name[7]=aa;
	
			 switch (((a>>12)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>12)&0x0f)+'0';
			 break;
        }

  }
	ap_name[8]=aa;
	
	
			 switch (((a>>8)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>8)&0x0f)+'0';
			 break;
        }

  }
	ap_name[9]=aa;
	
		 switch (((a>>4)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>4)&0x0f)+'0';
			 break;
        }

  }
	ap_name[10]=aa;
	
		 switch (((a)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a)&0x0f)+'0';
			 break;
        }

  }
	ap_name[11]=aa;
}
#elif  XGTD_AP

void Get_SerialNum(void)
{
  u32 a;
  char aa;
  
	a=get_mcu_id();
	

	ap_name[0]='X';
	ap_name[1]='G';
	ap_name[2]='T';
	ap_name[3]='D';
	ap_name[4]='-';
	
	 switch (((a>>20)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>20)&0x0f)+'0';
			 break;
        }

  }
	ap_name[5]=aa;
	
		 switch (((a>>16)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>16)&0x0f)+'0';
			 break;
        }

  }
	ap_name[6]=aa;
	
			 switch (((a>>12)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>12)&0x0f)+'0';
			 break;
        }

  }
	ap_name[7]=aa;
	
	
			 switch (((a>>8)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>8)&0x0f)+'0';
			 break;
        }

  }
	ap_name[8]=aa;
	
		 switch (((a>>4)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>4)&0x0f)+'0';
			 break;
        }

  }
	ap_name[9]=aa;
	
		 switch (((a)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a)&0x0f)+'0';
			 break;
        }

  }
	ap_name[10]=aa;
}

#elif  JS_AP

void Get_SerialNum(void)
{
  u32 a;
  char aa;
  
	a=get_mcu_id();
	

// 	ap_name[0]='X';
// 	ap_name[1]='G';
// 	ap_name[2]='T';
// 	ap_name[3]='D';
// 	ap_name[4]='-';

	ap_name[0]='J';
	ap_name[1]='S';
	ap_name[2]='-';
	 switch (((a>>20)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>20)&0x0f)+'0';
			 break;
        }

  }
	ap_name[3]=aa;
	
		 switch (((a>>16)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>16)&0x0f)+'0';
			 break;
        }

  }
	ap_name[4]=aa;
	
			 switch (((a>>12)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>12)&0x0f)+'0';
			 break;
        }

  }
	ap_name[5]=aa;
	
	
			 switch (((a>>8)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>8)&0x0f)+'0';
			 break;
        }

  }
	ap_name[6]=aa;
	
		 switch (((a>>4)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a>>4)&0x0f)+'0';
			 break;
        }

  }
	ap_name[7]=aa;
	
		 switch (((a)&0x0f))
	 {

			 case 10:
			 {
       aa='a';
			 break;
        }
					 case 11:
			 {
       aa='a';
			 break;
        }
					 case 12:
			 {
       aa='a';
			 break;
        }
					 case 13:
			 {
       aa='a';
			 break;
        }
			 case 14:
			 {
       aa='a';
			 break;
        }
					 case 15:
			 {
       aa='a';
			 break;
        }
		default:
			 {
       aa=((a)&0x0f)+'0';
			 break;
       }

  }
	ap_name[8]=aa;
}

#endif

unsigned char channel_num=0;
void SET_ESP8266_WAKEY(void)
{

//          char tab[50]="AT+CWSAP=\"RHX-";
//	char tab2[50]="AT+CWSAP=\"RHX-";
	u8 len,len1;
	
	len= *(volatile unsigned char*)0x0800FC40;//密码
	len1= *(volatile unsigned char*)0x0800FC56; //名字
	channel_num=*(volatile  unsigned char*)0x0800FC18;//通道号
	if((channel_num<1)||(channel_num>9))
	{
     channel_num=2+'0';
   }
	 else
	 {
	 channel_num+='0';
	 }
	
	if(((len>0)&(len<20))&&((len1>0)&(len1<20)))
	 {
	    
		 #ifdef RHX_AP
				//send_string("AT+CWSAP=\"RHX-",14);
				send_string("AT+CWSAP=\"BMW-",14);					//ZDB   智多邦
				send_string(WIFI_SSID_DEV,2);
			#elif JS_AP
				send_string("AT+CWSAP=\"JS-",13);
		  #elif XGTD_AP
				send_string("AT+CWSAP=\"XGTD-",15);
		  #endif 
	    send_string((unsigned char*)0x0800FC57,len1); //ssid
	    send_string("\",\"",3);
	    send_string((unsigned char*) 0x0800FC41,len);	//code
		  send_string("\",",2);
		  send_string(&channel_num,1);
		 
		  
		 // send_string(",3\r\n",4);
		  send_string(",3,",3);
		 #ifdef RHX_AP
				send_string(&ap_name[6],6);
		  #elif XGTD_AP
				send_string(&ap_name[5],6);
			#elif JS_AP
				send_string(&ap_name[3],6);
		  #endif 
		  
		  send_string("\r\n",2);
		 
// 	    send_string("\",2,3\r\n",7);		 
//	  ut(0x0d); "\r\n"
//	  ut(0x0a);
	  
	 }
	 else if((len1>0)&(len1<20))//名字正确 但密码不对
	 {
			#ifdef RHX_AP
				send_string("AT+CWSAP=\"BMW-",14);
				send_string(WIFI_SSID_DEV,2);
		  #elif XGTD_AP
				send_string("AT+CWSAP=\"XGTD-",15);
			#elif JS_AP
				send_string("AT+CWSAP=\"JS-",13);
		  #endif 
	    send_string((unsigned char*)0x0800FC57,len1); //ssid
// 	    send_string("\",\"12345678\",2,3\r\n",18);	

       send_string("\",\"12345678\",",13);
		   send_string(&channel_num,1);
		   //send_string(",3\r\n",4);
		   send_string(",3,",3);
		  #ifdef RHX_AP
				send_string(&ap_name[6],6);
		  #elif XGTD_AP
				send_string(&ap_name[5],6);
		 #elif JS_AP
				send_string(&ap_name[3],6);
		  #endif 
		  send_string("\r\n",2);
		 
	 }
	 else if((len>0)&(len<20))//密码正确 但名字不对
	 {
	 
// 	    send_string("AT+CWSAP=\"RHX-AP\",\"",19);
		  send_string("AT+CWSAP=\"",10);
		 #ifdef RHX_AP
				send_string(ap_name,12);
		  #elif XGTD_AP
				send_string(ap_name,11);
		 #elif JS_AP
				send_string(ap_name,9);
		  #endif 
		  
		  send_string("\",\"",3);
	    send_string((unsigned char*) 0x0800FC41,len);	//code
	 
		  send_string("\",",2);
		  send_string(&channel_num,1);
		 // send_string(",3\r\n",4);	
       send_string(",3,",3);
		  #ifdef RHX_AP
				send_string(&ap_name[6],6);
		  #elif XGTD_AP
				send_string(&ap_name[5],6);
			#elif JS_AP
				send_string(&ap_name[3],6);	
		  #endif 
		  send_string("\r\n",2);		 
	 }
	 else
	 {
	 
	 
// 	    send_string("AT+CWSAP=\"RHX-AP\",\"12345678\",2,3\r\n",34);
// 	 send_string("AT+CWSAP=\"RHX-AP1\",\"12345678\",2,3\r\n",35); 

		 
			if(*(volatile unsigned char*)0x0800FC55 == 0xab)      //模块密码、ssid重新配置标志位  
			{		

				#ifdef RHX_AP
				//send_string("AT+CWSAP=\"RHX-AP1\",\"12345678\",2,3,",34);
				send_string("AT+CWSAP=\"BMW-AP1\",\"12345678\",2,3,",34);
				send_string(&ap_name[6],6);//mac
		  #elif XGTD_AP
				send_string("AT+CWSAP=\"XGTD-AP1\",\"12345678\",2,3,",35);
				send_string(&ap_name[5],6);//mac
			#elif JS_AP
				send_string("AT+CWSAP=\"JS-AP1\",\"12345678\",2,3,",33);
				send_string(&ap_name[3],6);//mac	
		  #endif  
				
		     send_string("\r\n",2);	
				
			   flash_read_write(1); 				
				sim_buf_red[0x40]=8; //KEY
				sim_buf_red[0X41]='1';sim_buf_red[0X42]='2';sim_buf_red[0X43]='3';sim_buf_red[0X44]='4';
				sim_buf_red[0X45]='5';sim_buf_red[0X46]='6';sim_buf_red[0X47]='7';sim_buf_red[0X48]='8';
				sim_buf_red[0x56]=3;  //AP
				sim_buf_red[0x57]='A';sim_buf_red[0x58]='P';sim_buf_red[0x59]='1';
				flash_read_write(0);	

			}
			else
			{
			 send_string("AT+CWSAP=\"",10);
		  
				#ifdef RHX_AP
				send_string(ap_name,12);
		  #elif XGTD_AP
				send_string(ap_name,11);
				#elif JS_AP
				send_string(ap_name,9);
		  #endif 
		  send_string("\",\"12345678\",",13);
		 
// 		 send_string("AT+CWSAP=\"RHX-AP1\",\"12345678\",",30); 
	   send_string(&channel_num,1);
// 		 send_string(",3\r\n",4);
		 
		   send_string(",3,",3);
		  #ifdef RHX_AP
				send_string(&ap_name[6],6);
		  #elif XGTD_AP
				send_string(&ap_name[5],6);
			#elif JS_AP
				send_string(&ap_name[3],6);	
		  #endif 
		  send_string("\r\n",2);
			
			
			}
		 
		 
		 
 }
 

	
}

char comper_str(void)
{
  unsigned char a;
	
	for(a=0;a<200;a++)
	{
    
		if(usart_buf[a]=='O')
		{
       if(usart_buf[a+1]=='K')
			 {

         return 1;
       }
   
		}
		

  }

   return 0;

}





void TIM1_UP_IRQHandler(void)
{
   
    
 

	
#ifdef	WIFI_CARD_8266
 
	static unsigned char num_error=0,error_flage=0,enter_num=0;
	 TIM1->SR&=0xfe;
	
	
	
	
	if(init_finish==0)
{	
	if(wifi_init_flag==0)
	{
			if(timer1_flag<3)            //开机等待4S等待wifi模块启动     
			{
				timer1_flag++; 
				goto OUT;
			}
			else 
			{
			      num_error++;
				    config_tim=0;

			      wifi_init_flag=1;  //串口开始接收数据到	usart_buf 中
			}
	}


	if(num_error>4)
	{
	  wifi_init_flag=0;// 开始串口接收节目
			wifiinit=3;   //如果是按键初始化则退出
	  //////// //失能定时器1
	 init_finish=0x55;  //这个标志是  当初始化完成后不再进入 
	}
	else if(error_flage!=num_error)
	{
	  error_flage=num_error;
		send_string("+++",3);  //退出透传		
	}

	enter_num++;


// 	if((enter_num%4==0)||(buf_num>2))
	{
	 enter_num=1;
	 switch (config_step)
	 {
	   case 0:
	   {
	   	buf_clr();	             //清空	 usart_buf
	    SET_ESP8266_WAKEY();//配制WIFI名字密码	  
			config_step=1;
			config_tim=0;
		  break;
	   
	   }
	   case 1:
	   {
// 	   	if(strstr((char*)usart_buf,"OK"))
			if(comper_str())
			{
				buf_clr();
				wifi_init_flag=0;//重新上电等待
				timer1_flag=0;
				
				send_string("AT+RST\r\n",8);        //复位		   			
				config_tim=0;
				config_step=2;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>4) //如果4s内没有发现返回OK
				{			   
					 wifi_init_flag=0;
					 timer1_flag=0;

					 config_tim=0;
// 					 config_step=0;
					send_string("+++",3);///////
						 config_step=0;
					 buf_clr();//这个不能发的太快 
// 						send_string("AT+RST\r\n",8); //15.3.14
					      
					
				}
			}
		break;
	   
	   }



	  case 2:
	  {
	  
			buf_clr();
			send_string("AT+CIPSERVER=1,25622\r\n",22);		
			config_step=3;
			config_tim=0;

		break;
	   
	   }

	  
	   case 3:
	   {
// 	   	if((strstr((char*)usart_buf,"OK")))
		if(comper_str())
		{
			buf_clr();			
			wifi_init_flag=0;//开始接收节目
			wifiinit=3;  //如果是按键初始化则退出
			init_finish=0x55;  //退出  
			
			config_step=0;
			config_tim=0;


			
			channel_num-='0';
      channel_num++;

			flash_read_write(1);
			
		sim_buf_red[0x55] = 0x00; //消除复位重新配制
		sim_buf_red[0x18]=channel_num;//更换通道号
	
	
			flash_read_write(0);
			
			
			//////// //失能定时器1
		 
		}
		else 
		{
			config_tim++;                                  //错误次数累加
			if(config_tim>4) //识别模块////3   15.3.25
			{
			   
			   config_tim=0;
			   //SoftReset();  //单片机复位
			    wifi_init_flag=0;
			   timer1_flag=0;
			   config_step=2;
				//init_finish=0x55;
			   send_string("AT+RST\r\n",8);        //复位
			  //break; //15.3.14
			}
		
		}
		break;
	   
	   }

	 }
	
  }	

	
	 
	 
}


OUT:	

if(init_finish==0x55)
{
  TIM1->CR1&=0xFE;    //失能定时器1
}

#elif WIFI_CARD_5350
	 TIM1->SR&=0xfe;
	
	if(start_wifi_init)
   {
    if(wifi_init_flag==0)
		{
			if(timer1_flag<15)            //开机等待10S等待wifi模块启动     
			{
				timer1_flag++ ; 
				return;
			}
			else 
			{
				wifi_init_flag=1;//这个是串口接收标志
			}
		}

	if(config_step==0)
		{
			buf_clr();
			send_at_start();
			config_step=1;
		}
	else if(config_step==1)
	{
				if(strchr((char*)usart_buf,'a'))
				{
					buf_clr();
					#ifdef ConMod1
					ut('a');//进入配置   
					#elif ConMod2
					ut2('a');
					#endif
					config_step=2;
					config_tim=0;
				}
				else 
				{
					config_tim++;								   //错误次数累加
					if(config_tim>5) //识别模块
					{
				start_wifi_init=0;
				start_wifi_config=1;
				wifiinit=1;
				//timer1_flag=0;
				config_step=0;	
				config_tim=0;
				//wifi_init_flag=0;		
				wifi_config=1;	
					}
					 else 
					 {
						buf_clr();
						send_at_start();
					 }
				 }
			}
		else if(config_step==2)
		{
			if(strstr((char*)usart_buf,"ok"))
			{
				buf_clr();
				//send_at_WAKEY();//配置密码
				send_string("AT+WAP\r",7);
				config_step=3;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }
		}

		else if(config_step==3)
		{
			#ifdef RHX_AP
					if( (strstr((char*)usart_buf,"BMW-"))   )//无配置信息
			#elif JS_AP
					if( (strstr((char*)usart_buf,"JS-"))   )//无配置信息
		  #elif XGTD_AP
					if( (strstr((char*)usart_buf,"XGTD-"))   )//无配置信息
		  #endif 	
			{
				
				buf_clr();
				//send_at_WAP();//配置配置名字
				send_string("AT+ENTM\r",8);
				config_step=4;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
// 				config_step=0;config_tim=0;timer1_flag=0;
// 				wifi_init_flag=0;send_at_reset();
					
						send_string("AT+ENTM\r",8);	
				start_wifi_init=0;
				start_wifi_config=1;
				wifiinit=1;
				//timer1_flag=0;
				config_step=0;	
				config_tim=0;
				//wifi_init_flag=0;		
				wifi_config=1;	
					
					
				}
			 }

		}
			else if(config_step==4)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();

				start_wifi_init=0;
				config_tim=0;

				 wifi_init_flag=5;
				//wifiinit=3;
				//////// //失能定时器1
				TIM1->CR1&=0xFE;  

				
				//配置tcp服务端模式
				//send_string("AT+NETP=TCP,SERVER,25622,192.168.47.1\r",38);
				//config_step=5;
				//config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		


          return;
	}
	
	if(wifiinit)//初始化wifi
	{
		if(wifi_init_flag==0)
		{
			if(timer1_flag<10)            //开机等待10S等待wifi模块启动     
			{
				timer1_flag++ ; 
				return;
			}
			else 
			{
				wifi_init_flag=1;
			}
		}
        
	
		if(config_step==0)
		{
			buf_clr();
			send_at_start();
			config_step=1;
		}
		else if(config_step==1)
		{
			if(strchr((char*)usart_buf,'a'))
			{
				buf_clr();
				#ifdef ConMod1
				ut('a');//进入配置
				#elif ConMod2
				ut2('a');//进入配置
				#endif 
				config_step=2;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>4) //识别模块
				{
					if(wifiinit==1)//115200
					{
						#ifdef ConMod1
						uart_init(57600);//转换为57600来判断
						#elif ConMod2
						uart2_init(57600);//转换为57600来判断
						#endif
						wifiinit=2;
                     config_tim=0;
						config_step=0;//57600 baud
					}
					else// if(wifiinit==2)
					{
	if(start_wifi_config)
				{
					wifiinit=0;
					config_tim=0;
					start_wifi_config=0;
                    wifi_init_flag=5;
					#ifdef ConMod1
				    uart_init(115200);//转换为57600来判断
					#elif ConMod2
					  uart2_init(115200);//转换为57600来判断
					#endif
				}
				else
				{
						wifiinit=3;//57600 识别失败退出
}
						
						//////// //失能定时器1
						TIM1->CR1&=0xFE; 
					}
				}
				 else 
				 {
				 	buf_clr();
				 	send_at_start();
				 }
			 }
		}
		else if(config_step==2)
		{
			if(strstr((char*)usart_buf,"ok"))
			{
				buf_clr();
				//send_at_WAKEY();//配置密码

				if(start_wifi_config)
				{
                 send_at_WAKEY();//配置密码
                 if(sendflag==0)
                 {send_string("AT+WAKEY=WPAPSK,AES,12345678\r",29);}
               
				}
				else
				{
				send_string("AT+WAKEY=WPAPSK,AES,12345678\r",29);
}
				config_step=3;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }
		}
		else if(config_step==3)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();
				//send_at_WAP();//配置配置名字
	if(start_wifi_config)
				{
            send_at_WAP();//配置配置名字
					 if(sendflag!=3)
						{
								#ifdef RHX_AP
								 //send_string("AT+WAP=11BGN,RHX-AP,Auto\r",25);
								send_string("AT+WAP=11BGN,BMW-AP,Auto\r",25);
								#elif JS_AP
								 send_string("AT+WAP=11BGN,JS-AP,Auto\r",24);
								#elif XGTD_AP
								 send_string("AT+WAP=11BGN,XGTD-AP,Auto\r",26);
								#endif

				     }
				}
				else
				{
					#ifdef RHX_AP
					 //send_string("AT+WAP=11BGN,RHX-AP,Auto\r",25);
					send_string("AT+WAP=11BGN,BMW-AP,Auto\r",25);
					#elif JS_AP
					 send_string("AT+WAP=11BGN,JS-AP,Auto\r",24);
					#elif XGTD_AP
					 send_string("AT+WAP=11BGN,XGTD-AP,Auto\r",26);
					#endif
				}
				config_step=4;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		
		else if(config_step==4)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();
				//配置tcp服务端模式
				send_string("AT+NETP=TCP,SERVER,25622,192.168.47.1\r",38);
				config_step=5;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		else if(config_step==5)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();
				//配置lan口的ip
				send_string("AT+LANN=192.168.47.1,255.255.255.0\r",35);
				config_step=6;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		else if(config_step==6)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();
				//配置波特率
				send_string("AT+UART=115200,8,1,None,NFC\r",28);
				config_step=7;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		
		else if(config_step==7)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();
				//配置网页登陆名及密码
				//send_string("AT+WEBU=RHXAP,RHXAP\r",20);
				send_string("AT+WEBU=BMWAP,BMWAP\r",20);
				config_step=8;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		else if(config_step==8)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();
				//关闭初始化管脚
				send_string("AT+FRLDEN=off\r",14);
				config_step=9;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		else if(config_step==9)
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				
				buf_clr();
				//配置成功重启模块
				send_at_reset();//send_string("AT+FRLDEN=off\r",14);
				config_step=10;
				config_tim=0;
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}
		else if(config_step==10)//配置成功重启模块
		{
			if( (strstr((char*)usart_buf,"ok"))   )//无配置信息
			{
				buf_clr();
	if(start_wifi_config)
				{
					wifiinit=0;
					config_tim=0;
					start_wifi_config=0;
                    wifi_init_flag=5;

         #ifdef ConMod1
					uart_init(115200);//转换为57600来判断
				 #elif ConMod2
					uart2_init(115200);//转换为57600来判断
					#endif
				}
				else
				{
				config_tim=0;
				wifiinit=3;
				//////// //失能定时器1
}
				TIM1->CR1&=0xFE;   
			}
			else 
			{
				config_tim++;                                  //错误次数累加
				if(config_tim>10) //识别模块
				{
				config_step=0;config_tim=0;timer1_flag=0;
				wifi_init_flag=0;send_at_reset();
				}
			 }

		}

		return;//初始化时，不能执行之后操作
	}
    if(wifi_init_flag==0)
    {
        if(timer1_flag<6)            //开机等待14S等待wifi模块启动     
        timer1_flag++ ;              
        else 
        {
            wifi_init_flag=1;
            timer1_flag=0;
        }
    }
    if(wifi_init_flag==1)
    {
			
		

				if(config_step==0)
				{
					send_at_start();
					config_step=1;
				}
				else if(config_step==1)
				{
					if(strchr((char*)usart_buf,'a'))
					{
						//识别模块成功
						if(wifi_config==0)// have config
						{
							wifi_init_flag=2;
						}
						else
						{
							buf_clr();
							#ifdef ConMod1
							ut('a');//进入配置
							#elif ConMod2
							ut2('a');//进入配置
							#endif
							config_step=2;config_tim=0;
						}
					}
					else 
					{
						config_tim++;                                  //错误次数累加
						if(config_tim>2) //识别模块
							{wifi_init_flag=3;}
						 else 
						 {
						 	buf_clr();
						 	send_at_start();
						 }
					 }
				}
				else if(config_step==2)
				{
					if(strstr((char*)usart_buf,"ok"))
					{
						buf_clr();send_at_WAKEY();//配置密码
						config_step=3;config_tim=0;
					}
					else 
					{
						config_tim++;                                  //错误次数累加
						if(config_tim>10) //识别模块
						{
						config_step=0;config_tim=0;timer1_flag=0;
						wifi_init_flag=0;send_at_reset();
						}
					 }
	
				}
				else if(config_step==3)
				{
					if( (strstr((char*)usart_buf,"ok")) || (sendflag==0) )//无配置信息
					{
						//flash_read_write(1);
						//sim_buf_red[0x40] = 0;//key
						//flash_read_write(0);
						buf_clr();send_at_WAP();//配置配置名字
						config_step=4;config_tim=0;
					}
					else 
					{
						config_tim++;                                  //错误次数累加
						if(config_tim>10) //识别模块
						{
						config_step=0;config_tim=0;timer1_flag=0;
						wifi_init_flag=0;send_at_reset();
						}
					 }
	
				}
				else if(config_step==4)
				{
					if( (strstr((char*)usart_buf,"ok")) || (sendflag!=3) )//无配置信息
					{
						//flash_read_write(1);
						//sim_buf_red[0x56] = 0;//wap
						//flash_read_write(0);
						buf_clr();
						config_step=5;config_tim=0;wifi_init_flag=2;
						
						 if(sendflag)
						 {
							 send_at_reset();
						 }
					}
					else 
					{
						config_tim++;                                  //错误次数累加
						if(config_tim>10) //识别模块
						{
						config_step=0;config_tim=0;timer1_flag=0;
						wifi_init_flag=0;send_at_reset();
						}
					 }
	
				}

				
				
    }
	 if(wifi_init_flag==3)
	 {
		 TIM1->CR1&=0xFE;    //失能定时器1
	 }
	 
	 if(wifi_init_flag==2)
	 {
			flash_read_write(1);
			sim_buf_red[0x55] = 0xA0;
			flash_read_write(0);
			TIM1->CR1&=0xFE;    //失能定时器1
	 }
	 

	 
#endif	 
	 
	 
//    out:;
}


