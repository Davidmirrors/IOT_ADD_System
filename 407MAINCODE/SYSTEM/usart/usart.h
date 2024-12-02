#ifndef __USART_H_
#define __USART_H_
#include "stdio.h"
#include <stdarg.h>
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 

//STM32������
//����1/2��ʼ��		   

////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  		//�����������ֽ��� 200
#define EN_USART1_RX 			1			//ʹ�ܣ�1��/��ֹ��0������1����

#define USART2_REC_LEN  		200  		//�����������ֽ��� 200
#define EN_USART2_RX 			1			//ʹ�ܣ�1��/��ֹ��0������2����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; 	//���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         			//����״̬���	

extern u8 MTF01_Date;						//MTF01�������ݽ���	

extern u8  USART2_RX_BUF[USART2_REC_LEN]; 	//���ջ���,���USART2_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART2_RX_STA;         			//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void uart2_init(u32 bound);
void u2_printf(char* fmt,...);

#endif //__USART_H_


