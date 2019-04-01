#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#define   DP1   XBYTE[0x7000]
#define   DP2   XBYTE[0x7100]
#define   DP3   XBYTE[0x7200]
#define   DP4   XBYTE[0x7300]
#define   TIMER 0x8000
#include "Init_Device.c"
unsigned char table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};
void display(unsigned char x,unsigned char y)
{
 	DP3=table[x];
 	DP4=table[y];
}     
   
void main(void)
{ 
	unsigned  char l_val,r_val,r_state,temp,conter,x,c;
    int key;
    DP1=DP2=DP3=DP4=0xff;
    Init_Device();
	while(1)
	{  
     	key=0x0004;
        for(x=1;x<5;x++,key++)
      	{  
         	c=XBYTE[key];
            if(r_state=~c&0x1f)
            {
				for(conter=1,temp=0x01;conter<6;++conter,temp=temp<<1)
                if((r_state&temp)!=0)  
                {
					r_val=conter;
                  	l_val=x;
                 	display(l_val,r_val);
				}
           	}
      	}
  	}
}