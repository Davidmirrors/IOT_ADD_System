/***********
//@Davidmirrors
//7/10/2023
//21/10/2023
//������
//GB2312
//.....
************/

#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"  

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
#define KEY_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0
#define KEY_0 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)	//PA0
#define KEY_1 	GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PA0

/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/
/*
#define KEY_UP 	PAin(0)		//PA0
*/


//#define WKUP_PRES   1

void KEYUP_GPIO_Config(void);	//IO��ʼ��
void KEY01_GPIO_Config(void);
u8 KEY_Scan(u8);  		//����ɨ�躯��
u8 KEY_Scan0(u8);  		//����ɨ�躯��	
u8 KEY_Scan1(u8);  		//����ɨ�躯��	

#endif
