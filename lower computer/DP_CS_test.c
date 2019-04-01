#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define    LED1      XBYTE[0x0000]
#define    LED2      XBYTE[0x0001]
#define    LED3      XBYTE[0x0002]
#define    LED4      XBYTE[0x0003]
#include "Init_Device.c"

void delay(uint x)
{ 
  unsigned long i;
	for(i=0;i<1000*x;++i) i=i; 
}

void main(void)
{  
  unsigned  char i;
  Init_Device();
	while(1)
	{        
  	LED1=LED2=LED3=LED4=0xff;
		delay(0);
	}
}
