#include "C8051F020.h"
#include "absacc.h"
#include "data_define_demo3.c"
#include "Init_Device.c"

void  delay(void)									//��ʱ����
{
  uint  i;												//����ѭ�����Ʊ���
  for(i=0;i<TIMER;i++) i=i; 			//�ղ���ѭ���ȴ�
}

void main(void)
{ 
  uchar  a;
  long int b;
  Init_Device();									//�����豸���ú���
	DPH=DPL=DPZ=DPN=0xFF;								//���ʮλ�����ʾ 
	while(1)												//��ѭ��
  {
    ADC = 0;											//����A/Dת��
  	delay();											//������ʱ�����ȴ�ת������
  	a = ADC;											//��ȡA/Dת�����
    b = a;
  	b = b*1000/255;								//��������10�����¶���ֵ
		if(b>999) b = 999;
    a = b/100;										//��ȡ�¶���ֵ��λ
  	DPH = table2[a]; 	 						//�¶���ֵ��λ��DPH
  	a=(b/10)%10;									//��ȡ�¶���ֵʮλ
  	a=table3[a]&0xFB;							//DPL��������С����
  	DPL=a;												//�¶���ֵʮλ��DPL
  	a=b%10;												//��ȡ�¶���ֵ��λ
  	DPZ=table4[a];								//�¶���ֵ��λ��DPZ
    for(a=0;a<200;a++) delay;			//�ȴ�0.5��
  }
}