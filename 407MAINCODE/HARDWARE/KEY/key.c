/***********
//@Davidmirrors
//7/10/2023
//21/10/2023
//功能：
// 1.按键初始化
//GB2312
//.....
************/

#include "key.h"
#include "delay.h" 

//按键初始化函数
void KEYUP_GPIO_Config(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
}

void KEY01_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
 

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

}

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，WKUP按下 对应K1按键

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY_UP==1)return 1;
	}else if(KEY_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
}

u8 KEY_Scan0(u8 mode)
{	 
	static u8 key_0=1;//按键按松开标志
	if(mode)key_0=1;  //支持连按		  
	if(key_0&&(KEY_0==1))
	{
		delay_ms(10);//去抖动 
		key_0=0;
		if(KEY_0==1)return 1;
	}else if(KEY_0==0)key_0=1; 	    
 	return 0;// 无按键按下
}
u8 KEY_Scan1(u8 mode)
{	 
	static u8 key_1=1;//按键按松开标志
	if(mode)key_1=1;  //支持连按		  
	if(key_1&&(KEY_1==1))
	{
		delay_ms(10);//去抖动 
		key_1=0;
		if(KEY_1==1)return 1;
	}else if(KEY_1==0)key_1=1; 	    
 	return 0;// 无按键按下
}




















