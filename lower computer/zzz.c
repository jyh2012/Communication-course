#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define   CS1      XBYTE[0x2000]
sbit    LED1=P1^0;
sbit    LED2=P1^1;
sbit    LED3=P1^2;
sbit    LED4=P1^3;
sfr16 TMR3RL = 0x92;
sfr16 TMR3 = 0x94;
#include "Init_Device.c"
unsigned char num[]={0xC0,0xF9,0xA4,0xB0,0x99,
                 				0x92,0x82,0xF8,0x80,0x90,0x7f};
unsigned char i=0;
unsigned char x1,x2,x3,x4;
unsigned int temperature; 
void delay(unsigned int x)
{ 
  unsigned long i;
	for(i=0;i<x;++i) i=i; 
}
void Timer3_Init (int counts)
{	
	TMR3CN = 0x00;
	TMR3RL = -counts;
	TMR3 = 0xFFFF;
	EIE2 |= 0x01;
	TMR3CN |= 0x04;
}
void Timer3_ISR (void) interrupt 14
{
	TMR3CN &= ~(0x80);
	i++;
	if(i==5)i=0;
	switch(i)
	{
		case 1:
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=1;
			P3=num[x4];
			LED1=0;
			LED2=1;
			LED3=1;
			LED4=1;
			break;	
		case 2:
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=1;
			P3=(num[x3] & 0x7F);
			LED1=1;
			LED2=0;
			LED3=1;
			LED4=1;
			break;	
		case 3:
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=1;
			P3=num[x2];
			LED1=1;
			LED2=1;
			LED3=0;
			LED4=1;
			break;			
		case 4:
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=1;
			P3=num[x1];
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=0;
			break;
		default:
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=1;
			break;		
	}
}
unsigned char ADCacc(void)
{
	unsigned char xx;
	CS1 = 0;
	delay(16000);
	xx = CS1;
	return xx;
}
unsigned int Temperature(unsigned int x)
{
	unsigned int y;
	y = x;
	y = (y*1000)/255;
	if (y>1000)y=1000;
	return y;
}
void display(unsigned int x)
{
	x1 = x/1000;
	x2 = (x-x1*1000)/100;
	x3 = (x-x1*1000-x2*100)/10;
	x4 = (x-x1*1000-x2*100-x3*10);
}
void main(void)
{  
  	unsigned char a;
  	unsigned int c;
	Init_Device();
	Timer3_Init(500);
	EA=1;
	LED1=0;
	LED2=1;
	LED3=1;
	LED4=1;
	while(1)
	{
		a=ADCacc();
		c=Temperature(a);
		display(c);
	}
}
