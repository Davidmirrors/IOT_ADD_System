/***********
//@Davidmirrors
//8/3/2024
//9/3/2024
//功能：
// 1.非接触式液位传感器初始化
//GB2312
//.....
************/

#include "ncllds.h"
#include "delay.h" 

//按键初始化函数
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

//非接触式液位传感器处理函数
//返回非接触式液位传感器值
//mode:0,不支持连续读取;1,支持连续读取;
//0，没有任何非接触式液位传感器触发
//1，非接触式液位传感器High触发 对应K1按键

u8 NCLLDS_High_Scan(u8 mode)
{	 
	static u8 ncllds_high=1;//按键按松开标志
	if(mode)ncllds_high=1;  //支持连按		  
	if(ncllds_high&&(NCLLDS_High==1))
	{
		delay_ms(10);//去抖动 
		ncllds_high=0;
		if(NCLLDS_High==1)return 1;
	}else if(NCLLDS_High==0)ncllds_high=1; 	    
 	return 0;// 无按键按下
}

u8 NCLLDS_Low_Scan(u8 mode)
{	 
	static u8 ncllds_low=1;//按键按松开标志
	if(mode)ncllds_low=1;  //支持连按		  
	if(ncllds_low&&(NCLLDS_Low==1))
	{
		delay_ms(10);//去抖动 
		ncllds_low=0;
		if(NCLLDS_Low==1)return 1;
	}else if(NCLLDS_Low==0)ncllds_low=1; 	    
 	return 0;// 无按键按下
}




















