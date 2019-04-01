#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define    LED1      XBYTE[0x0000]
#define    LED2      XBYTE[0x0001]
#define    LED3      XBYTE[0x0002]
#define    LED4      XBYTE[0x0003]
#include "Init_Device.c"
unsigned char num[]={0xC0,0xF9,0xA4,0xB0,0x99,
                 				0x92,0x82,0xF8,0x80,0x90,0x7f};
void delay(uint x)
{ 
  unsigned long i;
	for(i=0;i<1000*x;++i) i=i; 
}

void main(void)
{  
  unsigned  char i;
  Init_Device();        
  LED1=LED2=LED3=LED4=0xff;
  while(1)
  {  
		for(i=0;i<11;++i)
    { 
	    LED1=num[i]; 
      LED2=num[i];
			LED3=num[i]; 
      LED4=num[i];
      delay(60); 
    }     
  }
}
