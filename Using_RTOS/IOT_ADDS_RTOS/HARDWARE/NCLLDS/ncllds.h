/***********
//@Davidmirrors
//8/3/2024
//9/3/2024
//非接触式液位传感器
//GB2312
//.....
************/

#ifndef __NCCLLDS_H_
#define __NCCLLDS_H_ 
#include "bsp_SysTick.h"  

/*下面的方式是通过直接操作库函数方式读取IO*/

#define NCLLDS_High 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)	//PE1
#define NCLLDS_Low	 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)	//PE2

/*下面方式是通过位带操作方式读取IO*/
/*
#define NCLLDS_High 	PEin(1)		//PE1
*/


//#define NCLLDS_HIGH_PRES   1

void Ncllds12HL_GPIO_Config(void);	//IO初始化

u8 NCLLDS_High_Scan(u8);  		//按键扫描函数	
u8 NCLLDS_Low_Scan(u8);  		//按键扫描函数	

#endif
