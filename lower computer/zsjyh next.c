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

//参数设置
unsigned char datachar[3];
unsigned char room_temp, temperature;
unsigned char flag=0, flag1=1, rece_flag=0;
unsigned char i=0;
unsigned char x1=10, x2=10, x3=10, x4=10;//分别对应数码管的四位
unsigned int rece_cout;
unsigned char temp;
unsigned char Interger, Decimals;
unsigned char cc;
//参数设置

void UART0_Init(void);
void delay(unsigned int x);
void Timer3_Init(int counts);
void Receive_data(void);
void Send_data(unsigned int sign, unsigned char data1, unsigned char data2);
unsigned char Sendchar(unsigned int sign1);
unsigned char ADC_get(void);
unsigned int D_Temperature(unsigned char x);
void display_temp(unsigned int x);
void temp_adc_display(void);
void change_temp(void);
void temp_dac_change(void);
void Launch(unsigned char Data[3]);


void UART0_Init(void)
{
    CKCON = 0x00; 		
    SCON0 = 0x50;
    TMOD = 0x21;
    TH1 = 0xFD;
    TL1 = 0xFD;
	EA=1;
    //ES0 = 1;//UART0 中断开启
    TR1 = 1;//启动定时器T1
    TI0 = 1;
    TR0 = 1;
}//串口初始化

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

void Receive_data(void)
{
    
     rece_cout=6;
    while(rece_cout)
    {
        while(RI0 == 0);
        temp = SBUF0;
        switch(rece_cout)
        {
            case 6:
                if (temp == 'C') rece_cout = 5;
                RI0=0;
                break;
            case 5:
                if (temp == 'O') rece_cout = 4;
                RI0=0;
                break;
            case 4:
                if (temp == 'M') rece_cout = 3;
                RI0=0;
                break;
            case 3:
                datachar[0] = temp;
                rece_cout--;
                RI0=0;
                break;
            case 2:
                datachar[1] = temp;
                rece_cout--;
                RI0=0;
                break;
            case 1:
                datachar[2] = temp;
                rece_cout--;
                RI0=0;
                break;
            default:
                break; 
        }
    }
    rece_flag = 1;
}//接受数据

void Launch(unsigned char Data[3])
{   
    
		cc=Data[0];
		cc=cc&0xF0;
    if ((cc == 0xA0) && (rece_flag == 1))
    {
        unsigned char choice = Data[0];
        unsigned char opera, Dec, data1, data2;
        switch(choice)
				{
				case 0xA0 :
                Interger = Data[1];
                Decimals = Data[2];
                temperature = Interger+Decimals/100;
                flag = 1;
                break;
			case 0xA1:  
                temperature++;
                flag = 1;
                break;
            case 0xA2 :
                temperature--;
                flag = 1;
                break;
            case 0xA3 :
                if(flag1 == 0) flag1 = 1;
                else flag1 = 0;
                
                break;
            case 0xA6 :
                opera = Data[1];
                DAC = opera;
				flag=0;
                break;
            case 0xA8 :
                temp_adc_display();
                Dec = 0x00;
								if (room_temp>99) room_temp=99;
                Send_data(8, room_temp, Dec);
                break;
            case 0xAE :
                if(flag1 == 0) data1 = 0xFF;
                else data1 = 0x7F;
                data2 = 0xFF;
                Send_data(14, data1, data2);
                break;
            default:
                break;
        }
        rece_flag = 0;
    }

}//判断功能

void Send_data(unsigned int sign, unsigned char data1, unsigned char data2)
{
    unsigned int send_cout=6;
    REN0 = 0;
    while(send_cout > 0)
    {
        switch(send_cout)
        {
            case 6 :
                SBUF0 = 'C';
                send_cout--;
                break;
            case 5 :
                SBUF0 = 'O';
                send_cout--;
                break;
            case 4 :
                SBUF0 = 'M';
                send_cout--;
                break;
            case 3 :
                SBUF0 = Sendchar(sign);
                send_cout--;
                break;
            case 2 :
                SBUF0 = data1;
                send_cout--;
                break;
            case 1 :
                SBUF0 = data2;
                send_cout--;
                break;
        }
        while(TI0 == 0);
        TI0 = 0;
    }
     REN0 = 1;
}//发送数据

unsigned char Sendchar(unsigned int sign1)
{   
    unsigned char scha;
    switch(sign1)
    {
        case 8:
            scha = 0xA8;
            break;
        case 10:
            scha = 0xAA;
            break;
        case 11:
            scha = 0xAB;
            break;
        case 14:
            scha = 0xAE;
            break;
    }
    return scha;
}

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
void temp_adc_display(void)
{
	unsigned char a;
	unsigned int b;
	a = ADC_get();
	b = D_Temperature(a);
	room_temp = b;
	display_temp(b);
}//温度数据采集显示
void display_temp(unsigned int x)
{
	x1 = x/10;
	x2 = (x-x1*10);
}//显示温度

void change_temp(void)
{
	temp_adc_display();
	if(temperature > room_temp)
	{
	
		DAC = 0xC0;
	}
	else
	{
		DAC = 0x60;
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
		DAC = 0x7A; 
	}
}//温度改变调度函数

void main(void)
{
	Init_Device();
	Timer3_Init(100);  
	UART0_Init();
	DAC=0x7A;
	while(1)
	{   
	temp_adc_display();
    Receive_data();
	Launch(datachar);
	temp_dac_change();
	}
}