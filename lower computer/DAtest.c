#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define   DAC     XBYTE[0x4000]
#define   TIMER   0x100
#include "Init_Device.c"
void delay(void);
void main(void)
{ 
 	unsigned char x;
 	Init_Device();
	while(1)  
	{
	 	DAC = 0x60;
	}
}

void delay(void)
{ 
       unsigned int i;
       for(i=0;i<TIMER;++i) ;
}
