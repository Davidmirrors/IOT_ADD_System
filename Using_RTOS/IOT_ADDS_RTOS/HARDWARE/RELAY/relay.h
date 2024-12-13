/***********
//@Davidmirrors
//7/10/2023
//21/10/2023
//继电器三目判别
//GB2312
//.....
************/

#ifndef _RELAY_H_
#define	_RELAY_H_

#include "stm32f4xx.h"

#define RELAY0(x)  (x)?( GPIO_ResetBits(GPIOD, GPIO_Pin_0) ) : ( GPIO_SetBits(GPIOD, GPIO_Pin_0) )
#define RELAY1(x)  (x)?( GPIO_ResetBits(GPIOD, GPIO_Pin_1) ) : ( GPIO_SetBits(GPIOD, GPIO_Pin_1) )
#define RELAY2(x)  (x)?( GPIO_ResetBits(GPIOD, GPIO_Pin_2) ) : ( GPIO_SetBits(GPIOD, GPIO_Pin_2) )
#define RELAY3(x)  (x)?( GPIO_ResetBits(GPIOD, GPIO_Pin_3) ) : ( GPIO_SetBits(GPIOD, GPIO_Pin_3) )

void RELAY_GPIO_Config(void);

#endif /* _RELAY_H_ */
