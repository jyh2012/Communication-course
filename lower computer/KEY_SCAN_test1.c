#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#include "Init_Device.c"
#define    LED1         XBYTE[0x0000]
#define    LED2         XBYTE[0x0001]
#define    LED3         XBYTE[0x0002]
#define    LED4         XBYTE[0x0003]
#define    KEY_WR      	XBYTE[0x0004]
#define    KEY_RD      	XBYTE[0x0005]
#define    linescan1   	0xfe
#define    linescan2   	0xfd
#define    linescan3   	0xfb
#define    linescan4   	0xf7

unsigned char num[]={0xC0,0xF9,0xA4,0xB0,0x99,
                     0x92,0x82,0xF8,0x80,0x90};
void main(void)  
{
  unsigned  char l_val,r_val,r_state,temp,conter,x;
  LED1=LED2=LED3=LED4=0xff;
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
			r_state=KEY_RD;
    }
  }
}