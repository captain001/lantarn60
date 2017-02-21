#ifndef __CLOCK_H__
#define __CLOCK_H__
#include "stmlib.h"
 
void dig_clock_deal(void);
unsigned char asc_check(unsigned char tmp,unsigned char n);
void dig_write(unsigned short x,unsigned char y,unsigned char w,unsigned char h,unsigned char color);
void __dig_write(unsigned char x,unsigned char y,unsigned char w,unsigned char h,unsigned char color);
void time_count(void);
unsigned char time_switch(u8 i);
void datn_deal(void);
unsigned char show_timers(void);
#endif
