#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define    DP1         XBYTE[0x0000]
#define    DP2         XBYTE[0x0100]
#define    DP3         XBYTE[0x0200]
#define    DP4         XBYTE[0x0300]
#define    KEY_WR      XBYTE[0x0400]
#define    KEY_RD      XBYTE[0x0500]
#define    linescan1   0xfe
#define    linescan2   0xfd
#define    linescan3   0xfb
#define    linescan4   0xf7
#define    TIMER       0x8000
#include "Init_Device.c"
unsigned char table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void display(unsigned char x,unsigned char y);
void main(void)
{
	unsigned  char l_val,r_val,r_state,temp,conter,x;
    DP1=DP2=DP3=DP4=0xff;
    Init_Device();
	while(1)
	{  
    	for(l_val=1;l_val<5;++l_val)
       	{ 
			switch(l_val)
			{
               	case 1:  KEY_WR=linescan1; break;
               	case 2:  KEY_WR=linescan2; break;
               	case 3:  KEY_WR=linescan3; break;
               	default: KEY_WR=linescan4;
            }
               
            if(r_state=~KEY_RD&0x1f)    
            for(conter=1,r_val=1,temp=1;conter<6;++conter,++r_val,temp=temp<<1)
            if((r_state&temp)!=0)  display(l_val,r_val);
      	}
  	}
}
             
void display(unsigned char x,unsigned char y)
{
 	DP1=table[x];
 	DP2=table[y];
}     
   

