/***********
//@Davidmirrors
//8/3/2024
//9/3/2024
//�ǽӴ�ʽҺλ������
//GB2312
//.....
************/

#ifndef __NCCLLDS_H_
#define __NCCLLDS_H_ 
#include "bsp_SysTick.h"  

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/

#define NCLLDS_High 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)	//PE1
#define NCLLDS_Low	 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)	//PE2

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define NCLLDS_High 	PEin(1)		//PE1
*/


//#define NCLLDS_HIGH_PRES   1

void Ncllds12HL_GPIO_Config(void);	//IO��ʼ��

u8 NCLLDS_High_Scan(u8);  		//����ɨ�躯��	
u8 NCLLDS_Low_Scan(u8);  		//����ɨ�躯��	

#endif
