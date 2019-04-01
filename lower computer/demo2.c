#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#include "Init_Device.c"
#define    ADC      XBYTE[0x2000]
#define    DP0      XBYTE[0x0003]
#define    DP1      XBYTE[0x0002]
#define    DP2      XBYTE[0x0001]
#define    DP3      XBYTE[0x0000]
#define    TIMER    0x8000
unsigned char data table1[]={0x50,0xF9,0x4A,0x49,0xE1,0x45,0x04,0xD9,0x40,0x41};
unsigned char data table2[]={0x0c,0xAF,0xC8,0x8a,0x2b,0x1a,0x18,0x8F,0x08,0x0a};
unsigned char data table3[]={0x44,0xF5,0x86,0x85,0x35,0x0d,0x0c,0xE5,0x04,0x05};
unsigned char data table4[]={0x0c,0xEE,0x58,0x4A,0xAA,0x0B,0x09,0x6E,0x08,0x0a};
void delay(void);
void display(unsigned char x,unsigned char y);
void main(void)
{ 
	unsigned char x=0,y=0,z=0,num=0;
  Init_Device();
	DP0=DP1=DP2=DP3=0xff;
 	while(1)
  	{ 
   		ADC=x; 
    	delay(); 
   		x=ADC;
   		x=(x*100)/256;  
     	y=x%10;num=2;display(num,y);
       	z=x/10;num=1;display(num,z);
   	}
}

void display(unsigned char x,unsigned char y)
{
 	if (x==1) DP1=table2[y];
 	else      DP2=table3[y];
}     
   
void delay(void)
{
  	unsigned int i;
  	for(i=0;i<TIMER;++i);
}
