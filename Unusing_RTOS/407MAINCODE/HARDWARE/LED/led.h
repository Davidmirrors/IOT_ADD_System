/***********
//@Davidmirrors
//7/10/2023
//21/10/2023
//LEDÈýÄ¿ÅÐ±ð
//GB2312
//.....
************/

#ifndef _LED_H_
#define	_LED_H_

#include "stm32f4xx.h"

#define LED2(x)  (x)?( GPIO_ResetBits(GPIOA, GPIO_Pin_6) ) : ( GPIO_SetBits(GPIOA, GPIO_Pin_6) )
#define LED3(x)  (x)?( GPIO_ResetBits(GPIOA, GPIO_Pin_7) ) : ( GPIO_SetBits(GPIOA, GPIO_Pin_7) )
#define beep(x)  (x)?( GPIO_ResetBits(GPIOE, GPIO_Pin_0) ) : ( GPIO_SetBits(GPIOE, GPIO_Pin_0) )

void LED_GPIO_Config(void);
void BEEP_GPIO_Config(void);

#endif /* _LED_H_ */
