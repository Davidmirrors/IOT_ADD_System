#ifndef __SOILTURE_H_
#define __SOILTURE_H_	
#include "bsp_SysTick.h"  
 							   
void Soiladc_Init(void); 				//ADC通道初始化
u16  Get_Soiladc(u8 ch); 				//获得某个通道值 
u16 Get_Soiladc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
#endif 















