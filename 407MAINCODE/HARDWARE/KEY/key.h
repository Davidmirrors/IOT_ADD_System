/***********
//@Davidmirrors
//7/10/2023
//21/10/2023
//按键宏
//GB2312
//.....
************/

#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"  

/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#define KEY_0 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)	//PA0
#define KEY_1 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PA0

/*下面方式是通过位带操作方式读取IO*/
/*
#define KEY_UP 	PAin(0)		//PA0
*/


//#define WKUP_PRES   1

void KEYUP_GPIO_Config(void);	//IO初始化
void KEY01_GPIO_Config(void);
u8 KEY_Scan(u8);  		//按键扫描函数
u8 KEY_Scan0(u8);  		//按键扫描函数	
u8 KEY_Scan1(u8);  		//按键扫描函数	

#endif
