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
	LED1=LED2=LED3=LED4=0xff;
  while(1)              	
  {
   	for(i=0;i<8;i++)      	
   	{
	    switch(i)		
	    {
	     	case 0:	LED1=0xfe; LED2=0xfe; LED3=0xfe; LED4=0xfe;
			    			delay(50);  break;

 	      case 1: LED1=0xfd; LED2=0xfd; LED3=0xfd; LED4=0xfd;
			    			delay(50);  break;
								 	
				case 2: LED1=0xfb; LED2=0xfb; LED3=0xfb; LED4=0xfb;
			    			delay(50);  break;

 	      case 3: LED1=0xf7; LED2=0xf7; LED3=0xf7; LED4=0xf7; 
			    			delay(50);  break;
								 
				case 4: LED1=0xef; LED2=0xef; LED3=0xef; LED4=0xef; 
			    			delay(50);  break;

 	      case 5: LED1=0xdf; LED2=0xdf; LED3=0xdf; LED4=0xdf; 
			    			delay(50);  break;
								 	     
				case 6: LED1=0xbf; LED2=0xbf; LED3=0xbf; LED4=0xbf; 
			    			delay(50);  break;

 	      case 7: LED1=0x7f; LED2=0x7f; LED3=0x7f; LED4=0x7f; 
			    			delay(50);  break; 	     
	    }
    }
  }
}
