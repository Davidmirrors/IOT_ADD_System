//头文件是有固定的格式的（相当于简历有固定模板）
#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"
#include <stdio.h>

#define Max_BUF_LEN 20

//函数声明
void Uart1_GPIO_Config(void);
void Uart2_GPIO_Config(void);
void u2_printf(char* fmt,...);
extern char RxBuffer2[Max_BUF_LEN]; 	//接收缓冲
extern unsigned char TxBuffer2[Max_BUF_LEN]; 	//接收缓冲

#endif /* __UART_H */


//#ifndef __USART_H
//#define	__USART_H

//#include "stm32f10x.h"
//#include <stdio.h>

//#define USART2_REC_LEN  		200  		//定义最大接收字节数 200
//#define EN_USART2_RX 			1			//使能（1）/禁止（0）串口2接收

//extern u8  USART2_TX_BUF[USART2_REC_LEN]; 	//接收缓冲,最大USART2_REC_LEN个字节.末字节为换行符 
//extern u8  USART2_RX_BUF[USART2_REC_LEN]; 	//接收缓冲,最大USART2_REC_LEN个字节.末字节为换行符 
//extern u16 USART2_RX_STA;         			//接收状态标记	

///** 
//  * 串口宏定义，不同的串口挂载的总线和IO不一样，移植时需要修改这几个宏
//	* 1-修改总线时钟的宏，uart1挂载到apb2总线，其他uart挂载到apb1总线
//	* 2-修改GPIO的宏
//  */
//	
//// 串口1-USART1
//#define  DEBUG_USARTx                   USART1
//#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
//#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
//#define  DEBUG_USART_BAUDRATE           115200

//// USART GPIO 引脚宏定义
//#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
//#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
//#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

//#define  DEBUG_USART_IRQ                USART1_IRQn
//#define  DEBUG_USART_IRQHandler         USART1_IRQHandler


//// 串口2-USART2
//#define  DEBUG_USART2                    USART2
//#define  DEBUG_USART2_CLK                RCC_APB1Periph_USART2
//#define  DEBUG_USAR2_APBxClkCmd          RCC_APB1PeriphClockCmd
//#define  DEBUG_USART2_BAUDRATE           115200

//// USART2 GPIO 引脚宏定义
//#define  DEBUG_USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
//#define  DEBUG_USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
//    
//#define  DEBUG_USART2_TX_GPIO_PORT       GPIOA   
//#define  DEBUG_USART2_TX_GPIO_PIN        GPIO_Pin_2
//#define  DEBUG_USART2_RX_GPIO_PORT       GPIOA
//#define  DEBUG_USART2_RX_GPIO_PIN        GPIO_Pin_3

//#define  DEBUG_USART2_IRQ                USART2_IRQn
//#define  DEBUG_USART2_IRQHandler         USART2_IRQHandler

//// 串口3-USART3
////#define  DEBUG_USARTx                   USART3
////#define  DEBUG_USART_CLK                RCC_APB1Periph_USART3
////#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
////#define  DEBUG_USART_BAUDRATE           115200

////// USART GPIO 引脚宏定义
////#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
////#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
////    
////#define  DEBUG_USART_TX_GPIO_PORT         GPIOB   
////#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_10
////#define  DEBUG_USART_RX_GPIO_PORT       GPIOB
////#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

////#define  DEBUG_USART_IRQ                USART3_IRQn
////#define  DEBUG_USART_IRQHandler         USART3_IRQHandler

//// 串口4-UART4
////#define  DEBUG_USARTx                   UART4
////#define  DEBUG_USART_CLK                RCC_APB1Periph_UART4
////#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
////#define  DEBUG_USART_BAUDRATE           115200

////// USART GPIO 引脚宏定义
////#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC)
////#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
////    
////#define  DEBUG_USART_TX_GPIO_PORT         GPIOC   
////#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_10
////#define  DEBUG_USART_RX_GPIO_PORT       GPIOC
////#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_11

////#define  DEBUG_USART_IRQ                UART4_IRQn
////#define  DEBUG_USART_IRQHandler         UART4_IRQHandler


//// 串口5-UART5
////#define  DEBUG_USARTx                   UART5
////#define  DEBUG_USART_CLK                RCC_APB1Periph_UART5
////#define  DEBUG_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
////#define  DEBUG_USART_BAUDRATE           115200

////// USART GPIO 引脚宏定义
////#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD)
////#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
////    
////#define  DEBUG_USART_TX_GPIO_PORT         GPIOC   
////#define  DEBUG_USART_TX_GPIO_PIN          GPIO_Pin_12
////#define  DEBUG_USART_RX_GPIO_PORT       GPIOD
////#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_2

////#define  DEBUG_USART_IRQ                UART5_IRQn
////#define  DEBUG_USART_IRQHandler         UART5_IRQHandler


//void USART1_Config(void);
//void USART2_Config(void);
////void DEBUG_USART2_IRQHandler(void);
//void u2_printf(char* fmt,...);
//void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
//void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
//void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);

//#endif /* __USART_H */
