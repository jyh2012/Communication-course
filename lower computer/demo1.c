#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#include "Init_Device.c"
#define    ADC      XBYTE[0x2000]
#define    DP1      XBYTE[0x0000]
#define    DP2      XBYTE[0x0001]
#define    DP3      XBYTE[0x0002]
#define    DP4      XBYTE[0x0003]
#define    TIMER    0x100
unsigned char table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void delay(void);
void display(unsigned char x,unsigned char y);
void main(void)
{ 
	unsigned char x,y,z,num;
  Init_Device();
 	DP1=DP2=DP3=DP4=0xff;
  	
	while(1)
 	{ 
   		ADC=x; 
   		delay(); 
   		x=ADC;
   		x=(x*100)/256;  
   		y=x%10;num=1;display(num,y);
   		z=x/10;num=2;display(num,z);
  	}
}

void display(unsigned char x,unsigned char y)
{
 	if (x==1) DP4=table[y];
 	else      DP3=table[y];
}     
   
void delay(void)
{
 	unsigned int i;
 	for(i=0;i<TIMER;++i);
}
