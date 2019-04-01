#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#include "Init_Device.c"
#define    LED1         XBYTE[0x0003]
#define    LED2         XBYTE[0x0002]
#define    LED3         XBYTE[0x0001]
#define    LED4         XBYTE[0x0000]
#define    KEY_WR      	XBYTE[0x0004]
#define    KEY_RD      	XBYTE[0x0005]
#define    linescan1   	0xfe
#define    linescan2   	0xfd
#define    linescan3   	0xfb
#define    linescan4   	0xf7

unsigned char data table1[]={0x50,0xF9,0x4A,0x49,0xE1,0x45,0x44,0xD9,0x40,0x41,0xbf};
unsigned char data table2[]={0x0c,0xAF,0xC8,0x8a,0x2b,0x1a,0x18,0x8F,0x08,0x0a,0xf7};
unsigned char data table3[]={0x44,0xF5,0x86,0x85,0x35,0x0d,0x0c,0xE5,0x04,0x05,0xfb};
unsigned char data table4[]={0x0c,0xEE,0x58,0x4A,0xAA,0x0B,0x09,0x6E,0x08,0x0a,0xf7}; 

void main(void)  
{
  unsigned  char l_val,r_val,r_state,temp,conter,x;
  Init_Device();
	LED1=LED2=LED3=LED4=0xff;
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
			if(r_state=((~KEY_RD)&0x1f))
			{
      	for(r_val=1,temp=1;r_val<6;r_val++)
     		{				
       		if(r_state&temp)
       		{
          	LED4=table4[r_val];
						LED3=table3[l_val];
					}
    			temp=temp<<1;            
 	    	}
			}
    }
  }
}