#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#include <stdarg.h>
#include "bsp_usart.h"




// 重定向printf函数
// 重定向printf函数
//int fputc(int ch, FILE *f)
//{
//	uint8_t temp[1]={ch};
//	USART_SendData(USART1, temp[0]);
//	return 0;
//}
//重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
	/* 发送一个字节数据到串口 */
	USART_SendData(USART1, (uint8_t) ch);

	/* 等待发送完毕 */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

	
//	/* 发送一个字节数据到串口 */
//	USART_SendData(USART2, (uint8_t) ch);

//	/* 等待发送完毕 */
//	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	
	return (ch);
}


void Uart1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* Configure USART1 PA10 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART1 PA9 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	/* Configure USARTy */
	USART_Init(USART1, &USART_InitStructure);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	
	/* Enable the USART1 */
	USART_Cmd(USART1, ENABLE);
}

 
uint8_t Rxdata;
uint8_t RxBuffer1[256];
__IO uint8_t RxCounter1;

void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		
		/* Read one byte from the receive data register */
		Rxdata = USART_ReceiveData(USART1);
		RxBuffer1[RxCounter1++] = Rxdata;
	}  
	
	/* Write one byte to the transmit data register */
	USART_SendData(USART1, Rxdata);
}


void Uart2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure the NVIC Preemption Priority Bits */  
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	/* Configure USART2 PA3 Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* Configure USART2 PA2 Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	/* Configure USART2 */
	USART_Init(USART2, &USART_InitStructure);
	
	/* Enable the USART2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable USART2 Receive and Transmit interrupts */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	/* Enable the USART2 */
	USART_Cmd(USART2, ENABLE);
}

unsigned char Rxdata2;
char RxBuffer2[Max_BUF_LEN];
unsigned int Rx_Flag = 0;

void USART2_IRQHandler(void)
{

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		
		/* Read one byte from the receive data register */
//		Rxdata2 = USART_ReceiveData(USART2);
//		RxBuffer2[RxCounter2++] = Rxdata2;
		
		RxBuffer2[Rx_Flag] = USART_ReceiveData(USART2);
		Rx_Flag++;
		
		if(RxBuffer2[Rx_Flag-1] == 0x0a || Rx_Flag == Max_BUF_LEN) 
		{
			if(RxBuffer2[0] == 'A')
			{
//				printf("%s\r\n",RxBuffer2);
				Rx_Flag = 0;
			}
			else
			{
				memset(RxBuffer2, 0, sizeof(RxBuffer2));
				Rx_Flag = 0;
			}
		}
		
//		printf("%c",Rxdata2);

	}

//	printf("%s",RxBuffer2);
//		memset(RxBuffer2, 0, sizeof(RxBuffer2));
	
	/* Write one byte to the transmit data register */
//	USART_SendData(USART2, Rxdata2);
}

uint8_t TxBuffer2[Max_BUF_LEN];

//串口2,printf 函数
//确保一次发送数据不超过USART2_MAX_SEND_LEN字节
void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)TxBuffer2,fmt,ap);
	va_end(ap);
	i = strlen((const char*)TxBuffer2);		//此次发送数据的长度
	for(j=0;j<i;j++)							//循环发送数据
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
		USART_SendData(USART2,TxBuffer2[j]); 
	}
	memset(TxBuffer2, 0, sizeof(TxBuffer2));
}































///**
//  ******************************************************************************
//  * @file    bsp_usart.c
//  * @version V1.0
//  * @date    2013-xx-xx
//  * @brief   调试用的printf串口，重定向printf到串口
//  ******************************************************************************
//  * @attention
//  *
//  * 实验平台:野火 F103-MINI STM32 开发板 
//  * 论坛    :http://www.firebbs.cn
//  * 淘宝    :https://fire-stm32.taobao.com
//  *
//  ******************************************************************************
//  */ 

//#include "stdio.h"
//#include "stdlib.h"
//#include <stdarg.h>
//#include <string.h>

//#include "bsp_usart.h"


// /**
//  * @brief  USART GPIO 配置,工作参数配置
//  * @param  无
//  * @retval 无
//  */
//void USART1_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	// 打开串口GPIO的时钟
//	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
//	
//	// 打开串口外设的时钟
//	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

//	// 将USART Tx的GPIO配置为推挽复用模式
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

//  // 将USART Rx的GPIO配置为浮空输入模式
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
//	
//	// 配置串口的工作参数
//	// 配置波特率
//	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
//	// 配置 针数据字长
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	// 配置停止位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	// 配置校验位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;
//	// 配置硬件流控制
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	// 配置工作模式，收发一起
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	// 完成串口的初始化配置
//	USART_Init(DEBUG_USARTx, &USART_InitStructure);

//	// 使能串口
//	USART_Cmd(DEBUG_USARTx, ENABLE);	    
//}


/////重定向c库函数printf到串口，重定向后可使用printf函数
//int fputc(int ch, FILE *f)
//{
//		/* 发送一个字节数据到串口 */
//		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
//		
//		/* 等待发送完毕 */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

/////重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
//int fgetc(FILE *f)
//{
//		/* 等待串口输入数据 */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(DEBUG_USARTx);
//}


//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* 嵌套向量中断控制器组选择 */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* 配置USART为中断源 */
//  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART2_IRQ;
//  /* 抢断优先级*/
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  /* 子优先级 */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* 使能中断 */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  /* 初始化配置NVIC */
//  NVIC_Init(&NVIC_InitStructure);
//}

//void USART2_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	// 打开串口GPIO的时钟
//	DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK, ENABLE);
//	
//	// 打开串口外设的时钟
//	DEBUG_USAR2_APBxClkCmd(DEBUG_USART2_CLK, ENABLE);

//	// 将USART Tx的GPIO配置为推挽复用模式
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_TX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

//  // 将USART Rx的GPIO配置为浮空输入模式
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_RX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
//	
//	// 配置串口的工作参数
//	// 配置波特率
//	USART_InitStructure.USART_BaudRate = DEBUG_USART2_BAUDRATE;
//	// 配置 针数据字长
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	// 配置停止位
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	// 配置校验位
//	USART_InitStructure.USART_Parity = USART_Parity_No ;
//	// 配置硬件流控制
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	// 配置工作模式，收发一起
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	// 完成串口的初始化配置
//	USART_Init(DEBUG_USART2, &USART_InitStructure);
//	
//	// 串口中断优先级配置
//	NVIC_Configuration();
//	
//	// 使能串口接收中断
//	USART_ITConfig(DEBUG_USART2, USART_IT_RXNE, ENABLE);	

//	// 使能串口
//	USART_Cmd(DEBUG_USART2, ENABLE);	    
//}

////uint8_t Rxdata2;
////uint8_t RxBuffer2[256];
////__IO uint8_t RxCounter2;

//u8 USART2_TX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//u16 USART2_RX_STA=0;       //接收状态标记	

////void USART2_IRQHandler(void)
////{
////	

////	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
////	{
////		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
////		
////		/* Read one byte from the receive data register */
////		Rxdata2 = USART_ReceiveData(USART2);
////		RxBuffer2[RxCounter2++] = Rxdata2;
////	}  
////	
////	printf("%s x\r\n",RxBuffer2);

////	/* Write one byte to the transmit data register */
//////	USART_SendData(USART2, Rxdata2);
////	
////}

////串口2,printf 函数
////确保一次发送数据不超过USART2_MAX_SEND_LEN字节
//void u2_printf(char* fmt,...)  
//{  
//	u16 i,j; 
//	va_list ap; 
//	va_start(ap,fmt);
//	vsprintf((char*)USART2_TX_BUF,fmt,ap);
//	va_end(ap);
//	i = strlen((const char*)USART2_TX_BUF);		//此次发送数据的长度
//	for(j=0;j<i;j++)							//循环发送数据
//	{
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //循环发送,直到发送完毕   
//		USART_SendData(USART2,USART2_TX_BUF[j]); 
//	}
//}

//// 串口中断服务函数
//void DEBUG_USART2_IRQHandler(void)
//{
//  uint8_t ucTemp;
//	if(USART_GetITStatus(DEBUG_USART2,USART_IT_RXNE)!=RESET)
//	{		
//		ucTemp = USART_ReceiveData(DEBUG_USART2);
////    USART_SendData(DEBUG_USART2,ucTemp);    
//		if((USART2_RX_STA&0x8000)==0)//接收未完成
//		{
//			if(USART2_RX_STA&0x4000)//接收到了0x0d
//			{
//				if(ucTemp!=0x0a)USART2_RX_STA=0;//接收错误,重新开始
//				else USART2_RX_STA|=0x8000;	//接收完成了
//			}
//			else //还没收到0X0D
//			{	
//				if(ucTemp==0x0d)USART2_RX_STA|=0x4000;
//				else
//				{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF] = ucTemp ;
//					USART2_RX_STA++;
//					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//接收数据错误,重新开始接收	  
//				}		 
//			}
//		}
//		
//	}
//		printf("%s 22",USART2_RX_BUF);
//		memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));

//}

///*****************  发送一个字符 **********************/
//void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
//{
//	/* 发送一个字节数据到USART */
//	USART_SendData(pUSARTx,ch);
//		
//	/* 等待发送数据寄存器为空 */
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
//}

///*****************  发送字符串 **********************/
//void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
//{
//	unsigned int k=0;
//  do 
//  {
//      Usart_SendByte( pUSARTx, *(str + k) );
//      k++;
//  } while(*(str + k)!='\0');
//  
//  /* 等待发送完成 */
//  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
//  {}
//}

///*****************  发送一个16位数 **********************/
//void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
//{
//	uint8_t temp_h, temp_l;
//	
//	/* 取出高八位 */
//	temp_h = (ch&0XFF00)>>8;
//	/* 取出低八位 */
//	temp_l = ch&0XFF;
//	
//	/* 发送高八位 */
//	USART_SendData(pUSARTx,temp_h);	
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
//	
//	/* 发送低八位 */
//	USART_SendData(pUSARTx,temp_l);	
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
//}
