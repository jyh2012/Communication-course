
#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define   CS1      XBYTE[0x2000]
#define   TIMER   0x10
#include "Init_Device.c"
void delay(void);
void main(void)
{ 
  unsigned char x;
  Init_Device();
	while(1)
	{ 
		CS1=x; 
		delay(); 
		x=CS1; 
		delay(); 
	}
}
void delay(void)
{ 
    uint i;
    for(i=0;i<TIMER;i++) i=i;
}
