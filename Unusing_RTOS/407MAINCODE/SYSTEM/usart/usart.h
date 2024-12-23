#ifndef __USART_H_
#define __USART_H_
#include "stdio.h"
#include <stdarg.h>
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 

//STM32开发板
//串口1/2初始化		   

////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  		//定义最大接收字节数 200
#define EN_USART1_RX 			1			//使能（1）/禁止（0）串口1接收

#define USART2_REC_LEN  		200  		//定义最大接收字节数 200
#define EN_USART2_RX 			1			//使能（1）/禁止（0）串口2接收
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; 	//接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         			//接收状态标记	

extern u8 MTF01_Date;						//MTF01接收数据解析	

extern u8  USART2_RX_BUF[USART2_REC_LEN]; 	//接收缓冲,最大USART2_REC_LEN个字节.末字节为换行符 
extern u16 USART2_RX_STA;         			//接收状态标记	
//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void uart2_init(u32 bound);
void u2_printf(char* fmt,...);

#endif //__USART_H_


