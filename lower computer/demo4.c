#include "C8051F020.h"
#include "absacc.h"
#include "data_define_demo3.c"
#include "Init_Device.c"

void  delay(void)									//延时函数
{
  uint  i;												//定义循环控制变量
  for(i=0;i<TIMER;i++) i=i; 			//空操作循环等待
}

void main(void)
{ 
  uchar  a;
  long int b;
  Init_Device();									//调用设备配置函数
	DPH=DPL=DPZ=DPN=0xFF;								//清除十位随机显示 
	while(1)												//主循环
  {
    ADC = 0;											//启动A/D转换
  	delay();											//调用延时函数等待转换结束
  	a = ADC;											//读取A/D转换结果
    b = a;
  	b = b*1000/255;								//计算扩大10倍的温度数值
		if(b>999) b = 999;
    a = b/100;										//提取温度数值百位
  	DPH = table2[a]; 	 						//温度数值百位送DPH
  	a=(b/10)%10;									//提取温度数值十位
  	a=table3[a]&0xFB;							//DPL数据增加小数点
  	DPL=a;												//温度数值十位送DPL
  	a=b%10;												//提取温度数值个位
  	DPZ=table4[a];								//温度数值个位送DPZ
    for(a=0;a<200;a++) delay;			//等待0.5秒
  }
}