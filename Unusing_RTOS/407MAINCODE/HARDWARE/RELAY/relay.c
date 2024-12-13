/***********
//@Davidmirrors
//7/10/2023
//21/10/2023
//继电器初始化
//GB2312
//.....
************/

#include "relay.h"

void RELAY_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOD时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//2MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_ResetBits(GPIOD,GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_0 | GPIO_Pin_1);//PD2/PD3/PD0/PD1  设置高

}
