#ifndef __SOILTURE_H_
#define __SOILTURE_H_	
#include "bsp_SysTick.h"  
 							   
void Soiladc_Init(void); 				//ADCͨ����ʼ��
u16  Get_Soiladc(u8 ch); 				//���ĳ��ͨ��ֵ 
u16 Get_Soiladc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
#endif 















