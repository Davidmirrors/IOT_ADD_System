/***********
//@Davidmirrors
//7/10/2023
//21/10/2023
//�̵�����ʼ��
//GB2312
//.....
************/

#include "relay.h"

void RELAY_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʹ��GPIODʱ��

  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIO
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_0 | GPIO_Pin_1);//PD2/PD3/PD0/PD1  ���ø�

}
