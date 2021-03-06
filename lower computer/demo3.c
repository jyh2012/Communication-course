#include "C8051F020.h"
#include "absacc.h"
#include "data_define_demo3.c"
#include "Init_Device.c"

void  delay(void)									//延时函数
{
  uint  i;												//定义循环控制变量
  for(i=0;i<TIMER;i++) i=i ; 			//空操作循环等待
}

void  INIT(void)									//初始化函数
{ 
  Init_Device();									//调用设备配置函数 
  DPH = 0xFF;											//清除十位随机显示 
  DPL = 0xFF;											//清除个位随机显示 
  DPZ = 0xFF;											//清除小数位随机显示
	DPN = 0xFF;											//清除未用位随机显示 
}


uchar ADCacc(void)								//数据采集函数
{
  uchar  x;												//定义变量存放结果
  ADC = 0;												//启动A/D转换
  delay();												//调用延时函数等待转换结束
  x = ADC;												//读取A/D转换结果
  return x;												//返回温度数据采集结果
}

uint  Temperature(uchar x)				//温度计算函数
{
  long int  y;										//定义变量存放计算结果
	y=x;
  y = y*1000/255;									//计算扩大10倍的温度数值
	if(y>999) y=999;
  return y;												//返回扩大10倍的温度数值
}

void display(uint x)							//温度显示函数
{
  uchar  y;												//定义变量存放计算结果
  y = x/100;											//提取温度数值百位
  DPH = table2[y]; 	 							//温度数值百位送DPH
  y = (x/10)%10;									//提取温度数值十位
  y = table3[y]&0xFB;							//DPL数据增加小数点
  DPL = y;												//温度数值十位送DPL
  y = x%10;												//提取温度数值个位
  DPZ = table4[y];								//温度数值个位送DPZ
}

void main(void)
{ 
  uchar  a;
  uint   b;
  INIT();													//设备初始化
  while(1)												//主循环
  {
    a = ADCacc();									//温度数据采集
    b = Temperature(a);						//温度数值计算
    display(b);	  								//温度数值显示
    for(a=0;a<20;a++) delay;			//等待0.5秒
  }
}