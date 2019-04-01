#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define    LED1         XBYTE[0x0000]
#define    LED2         XBYTE[0x0001]
#define    LED3         XBYTE[0x0002]
#define    LED4         XBYTE[0x0003]
#include "Init_Device.c"

unsigned char num[]={0xC0,0xF9,0xA4,0xB0,0x99,
                       0x92,0x82,0xF8,0x80,0x90};
void main(void)  
{
 	unsigned  char i,j,a,temp;
 	Init_Device();
  LED1=LED2=LED3=LED4=0xff;
	while(1) 			 	
  {				
   	for(i=4;i<8;i++) 		 	
   	{    	
      a=XBYTE[i];		
      a=~a&0x1f;		 	
      for(j=0,temp=1;j<5;j++)
     	{				
       	if(a&temp)
       	{
          LED4=num[j+1];
					LED3=num[i-3];
				}
    		temp=temp<<1;            
 	    }
    }
  }
}