
#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define    CS0  XBYTE[0x0055]
#define    CS1  XBYTE[0x20AA]
#define    CS2  XBYTE[0x4055]
#define    CS3  XBYTE[0x60AA]
#define    CS4  XBYTE[0x8055]
#define    CS5  XBYTE[0xA0AA]
#include "Init_Device.c"
void main(void)
{ 
	uchar a;
	Init_Device();
	while(1)  
  {
		CS0=0x55; a=CS1; CS2=0xaa; a=CS3; CS4=0x55; a=CS5;
	}
		
}
