/***********
//@Davidmirrors
//8/3/2024
//9/3/2024
//���ܣ�
// 1.�ǽӴ�ʽҺλ��������ʼ��
//GB2312
//.....
************/

#include "ncllds.h"
#include "delay.h" 

//������ʼ������
void Ncllds12HL_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

}

//�ǽӴ�ʽҺλ������������
//���طǽӴ�ʽҺλ������ֵ
//mode:0,��֧��������ȡ;1,֧��������ȡ;
//0��û���κηǽӴ�ʽҺλ����������
//1���ǽӴ�ʽҺλ������High���� ��ӦK1����

u8 NCLLDS_High_Scan(u8 mode)
{	 
	static u8 ncllds_high=1;//�������ɿ���־
	if(mode)ncllds_high=1;  //֧������		  
	if(ncllds_high&&(NCLLDS_High==1))
	{
		delay_ms(10);//ȥ���� 
		ncllds_high=0;
		if(NCLLDS_High==1)return 1;
	}else if(NCLLDS_High==0)ncllds_high=1; 	    
 	return 0;// �ް�������
}

u8 NCLLDS_Low_Scan(u8 mode)
{	 
	static u8 ncllds_low=1;//�������ɿ���־
	if(mode)ncllds_low=1;  //֧������		  
	if(ncllds_low&&(NCLLDS_Low==1))
	{
		delay_ms(10);//ȥ���� 
		ncllds_low=0;
		if(NCLLDS_Low==1)return 1;
	}else if(NCLLDS_Low==0)ncllds_low=1; 	    
 	return 0;// �ް�������
}




















