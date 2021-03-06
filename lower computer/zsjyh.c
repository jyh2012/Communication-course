#include "C8051F020.h"
#include "absacc.h"
#include "data_define.c"
#include "Init_Device.c"

#define   CS0      XBYTE[0x0000]
#define   ADC      XBYTE[0x2000]
#define   DAC      XBYTE[0x4000]
#define   CS3      XBYTE[0x6000]

sbit    LED1=P1^0;
sbit    LED2=P1^1;
sbit    LED3=P1^2;
sbit    LED4=P1^3;

sfr16 TMR3RL = 0x92;
sfr16 TMR3 = 0x94;

unsigned char num[] = {0xC0,0xF9,0xA4,0xB0,0x99,
                 				0x92,0x82,0xF8,0x80,0x90,0xff};
//数码管
unsigned char keys[4][4] = {	{0,1,2,3},
								{4,5,6,7},
								{8,9,10,11},
								{12,13,14,15}};
//按键表
unsigned char x1=10,x2=10,x3=10,x4=10;//分别对应数码管的四位
unsigned char flag=0,flag1=1,room_temp,temperature=0,A_sign = 0, B_sign = 0, i=0,ones,tens;
unsigned char VAL138,VAL244;
void delay(unsigned int x);
void Timer3_Init(int counts);
unsigned char ADC_get(void);
unsigned int D_Temperature(unsigned char x);
void display_temp(unsigned int x);
void temp_adc_display(void);
void Key(void);
int key_shake(void);
void judge_key(unsigned char x,unsigned char y);
void key_get(void);
void set_temp(unsigned char x);
void change_temp(void);
void temp_dac_change(void);

	unsigned int bb=0;//
void delay(unsigned int x)
{
    unsigned long i;
    for(i=0;i<x;i++);
}//延时函数
void Timer3_Init(int counts)
{
    TMR3CN = 0x00;
    TMR3RL = -counts;
    TMR3 = 0xFFFF;
    EIE2 |= 0x01;
    TMR3CN |= 0x04;
}//T3时钟初始化函数
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
			P3=num[x3];
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
}//时钟T3中断
unsigned char ADC_get(void)
{
    unsigned char xx;
	ADC = 0;
	delay(1000);
	xx = ADC;
	return xx;
}//获取温度函数
unsigned int D_Temperature(unsigned char x)
{
	long int y;
	y = x;
	y = (y*100)/255;
	if (y>99)	y=99;
	return y;
}//温度数字化处理
void display_temp(unsigned int x)
{
	x1 = x/10;
	x2 = (x-x1*10);
}//显示温度
void temp_adc_display(void)
{
	unsigned char a;
	unsigned int b;
	a = ADC_get();
	b = D_Temperature(a);
	room_temp = b;
	display_temp(b);
}//温度数据采集显示
void Key(void)
{
	unsigned int code addr16[] = {0x0000, 0x0100, 0x0800, 0x0900};
	unsigned char val_138, val_244, state_244, temp, c;
	unsigned int hex;
	for(val_138=1;val_138<5;val_138++)
	{
		hex = addr16[val_138-1];
		c = XBYTE[hex];
		state_244 = (~c)&0x0F;
		if(state_244!=0)
		{
			for(val_244=1,temp=1;val_244<5;val_244++,temp=temp<<1)
			{
				if((state_244&temp)!=0)
				{
					VAL138 = val_138;
					VAL244 = val_244;
				}
			}
		}
	}
}//按键读取
int key_shake(void)
{
	unsigned char l_138,r_244;
	Key();
	l_138 = VAL138;
	r_244 = VAL244;
	delay(20);
	Key();
	if((l_138 == VAL138)&&(r_244 == VAL244))	return 1;
	else	return 0;
}//按键防抖
void judge_key(unsigned char x,unsigned char y)
{
	unsigned int sign=0;
	sign = keys[x-1][y-1];
	if((sign>=0) && (sign<=9))
	{
		if(A_sign == 1) 
		{
			tens = sign;
			x3 = tens;
			A_sign = 0;
		}
		else if(B_sign == 1) 
		{
			ones = sign;
			x4 = ones;
			B_sign = 0;
		}
	}
	else
	{
		switch(sign)
		{
			case 10:
				A_sign = 1;
				B_sign = 0;
				break;
			case 11:
				A_sign = 0;
				B_sign = 1;
				break;
			case 12:
				A_sign = 0;
				B_sign = 0;
				bb = tens*10+ones;
				set_temp(bb);
				break;
			case 14:
				flag = 0;
				flag1 = 1;
				break;
			case 15:
				flag1=0;
				break;
			default:
				break;
		}
	}
}//按键判断
void key_get(void)
{
	if(key_shake())
	{
		Key();
		judge_key(VAL138, VAL244);
	}
}//按键获取总函数
void set_temp(unsigned char x)
{
	temperature = x;
	flag = 1;
}//温度标识设置

void change_temp(void)
{
	temp_adc_display();
	if(temperature > room_temp)
	{
	
		DAC = 0xFF;
	}
	else
	{
		DAC = 0x00;
	}
}//改变温度

void temp_dac_change(void)
{
	if(flag1==0)
	{
		if(flag==1)
		{
			change_temp();
		}
	}
	else
	{
		DAC = 0x80; 
	}
}//温度改变调度函数


void main(void)
{
	Init_Device();
	Timer3_Init(100);
	EA=1;
	while(1)
	{
		temp_adc_display();
		temp_dac_change();
		key_get();
	}
}