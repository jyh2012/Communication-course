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
unsigned char num[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x7f};
unsigned char i=0;
unsigned char x1,x2,x3,x4;
void delay(unsigned int x);
void Timer3_Init (int counts);
void display(unsigned char x,unsigned char y);
void main(void)
{  
  	unsigned char a;
  	unsigned int b;
		unsigned  char r_state,temp,x,c,cc;
		unsigned char l_val=0,r_val=0;
    int key=0,conter=0;
	Init_Device();
	Timer3_Init(50);
	EA=1;
	LED1=1;
	LED2=1;
	LED3=0;
	LED4=1;

	while(1)
	{
		for(x=1;x<5;x++)
		{
			c=0;
			switch(x)
			{
				case 1:
					c=XBYTE[0x0000];
					break;
				case 2:
					c=XBYTE[0x0100];
					break;
				case 3:
					c=XBYTE[0x0800];
					break;
				case 4:
					c=XBYTE[0x0900];
					break;
				default:
					c=0;
					break;
			}
			if(r_state=~c&0x1F)
      {
						for(conter=1,temp=0x01;conter<6;++conter,temp=temp<<1)
							{
                	if((r_state&temp)!=0)  
                	{
										x4=conter;
                  	x3=x;
										//display(r_val,l_val);
									}
							}
      }
		}
	}
}
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
			P3=num[x1];
			LED1=1;//
			LED2=1;
			LED3=1;
			LED4=1;
			break;	
		case 2:
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=1;
			P3=(num[x2]& 0x7F);
			LED1=1;
			LED2=1;//
			LED3=1;
			LED4=1;
			break;	
		case 3:
			LED1=1;
			LED2=1;
			LED3=1;
			LED4=1;
			P3=num[x3] ;
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
			P3=num[x4];
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
void display(unsigned char x,unsigned char y)
{
		x4=x;
		x3=y;
}