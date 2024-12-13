#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#include <stdarg.h>
#include "bsp_usart.h"




// �ض���printf����
// �ض���printf����
//int fputc(int ch, FILE *f)
//{
//	uint8_t temp[1]={ch};
//	USART_SendData(USART1, temp[0]);
//	return 0;
//}
//�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
	/* ����һ���ֽ����ݵ����� */
	USART_SendData(USART1, (uint8_t) ch);

	/* �ȴ�������� */
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		

	
//	/* ����һ���ֽ����ݵ����� */
//	USART_SendData(USART2, (uint8_t) ch);

//	/* �ȴ�������� */
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

//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char* fmt,...)  
{  
	u16 i,j; 
	va_list ap; 
	va_start(ap,fmt);
	vsprintf((char*)TxBuffer2,fmt,ap);
	va_end(ap);
	i = strlen((const char*)TxBuffer2);		//�˴η������ݵĳ���
	for(j=0;j<i;j++)							//ѭ����������
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
		USART_SendData(USART2,TxBuffer2[j]); 
	}
	memset(TxBuffer2, 0, sizeof(TxBuffer2));
}































///**
//  ******************************************************************************
//  * @file    bsp_usart.c
//  * @version V1.0
//  * @date    2013-xx-xx
//  * @brief   �����õ�printf���ڣ��ض���printf������
//  ******************************************************************************
//  * @attention
//  *
//  * ʵ��ƽ̨:Ұ�� F103-MINI STM32 ������ 
//  * ��̳    :http://www.firebbs.cn
//  * �Ա�    :https://fire-stm32.taobao.com
//  *
//  ******************************************************************************
//  */ 

//#include "stdio.h"
//#include "stdlib.h"
//#include <stdarg.h>
//#include <string.h>

//#include "bsp_usart.h"


// /**
//  * @brief  USART GPIO ����,������������
//  * @param  ��
//  * @retval ��
//  */
//void USART1_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	// �򿪴���GPIO��ʱ��
//	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
//	
//	// �򿪴��������ʱ��
//	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

//	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

//  // ��USART Rx��GPIO����Ϊ��������ģʽ
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
//	
//	// ���ô��ڵĹ�������
//	// ���ò�����
//	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
//	// ���� �������ֳ�
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	// ����ֹͣλ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	// ����У��λ
//	USART_InitStructure.USART_Parity = USART_Parity_No ;
//	// ����Ӳ��������
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	// ���ù���ģʽ���շ�һ��
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	// ��ɴ��ڵĳ�ʼ������
//	USART_Init(DEBUG_USARTx, &USART_InitStructure);

//	// ʹ�ܴ���
//	USART_Cmd(DEBUG_USARTx, ENABLE);	    
//}


/////�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
//int fputc(int ch, FILE *f)
//{
//		/* ����һ���ֽ����ݵ����� */
//		USART_SendData(DEBUG_USARTx, (uint8_t) ch);
//		
//		/* �ȴ�������� */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);		
//	
//		return (ch);
//}

/////�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
//int fgetc(FILE *f)
//{
//		/* �ȴ������������� */
//		while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET);

//		return (int)USART_ReceiveData(DEBUG_USARTx);
//}


//static void NVIC_Configuration(void)
//{
//  NVIC_InitTypeDef NVIC_InitStructure;
//  
//  /* Ƕ�������жϿ�������ѡ�� */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//  
//  /* ����USARTΪ�ж�Դ */
//  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART2_IRQ;
//  /* �������ȼ�*/
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  /* �����ȼ� */
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  /* ʹ���ж� */
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  /* ��ʼ������NVIC */
//  NVIC_Init(&NVIC_InitStructure);
//}

//void USART2_Config(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;

//	// �򿪴���GPIO��ʱ��
//	DEBUG_USART2_GPIO_APBxClkCmd(DEBUG_USART2_GPIO_CLK, ENABLE);
//	
//	// �򿪴��������ʱ��
//	DEBUG_USAR2_APBxClkCmd(DEBUG_USART2_CLK, ENABLE);

//	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_TX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(DEBUG_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

//  // ��USART Rx��GPIO����Ϊ��������ģʽ
//	GPIO_InitStructure.GPIO_Pin = DEBUG_USART2_RX_GPIO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init(DEBUG_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
//	
//	// ���ô��ڵĹ�������
//	// ���ò�����
//	USART_InitStructure.USART_BaudRate = DEBUG_USART2_BAUDRATE;
//	// ���� �������ֳ�
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	// ����ֹͣλ
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	// ����У��λ
//	USART_InitStructure.USART_Parity = USART_Parity_No ;
//	// ����Ӳ��������
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	// ���ù���ģʽ���շ�һ��
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	// ��ɴ��ڵĳ�ʼ������
//	USART_Init(DEBUG_USART2, &USART_InitStructure);
//	
//	// �����ж����ȼ�����
//	NVIC_Configuration();
//	
//	// ʹ�ܴ��ڽ����ж�
//	USART_ITConfig(DEBUG_USART2, USART_IT_RXNE, ENABLE);	

//	// ʹ�ܴ���
//	USART_Cmd(DEBUG_USART2, ENABLE);	    
//}

////uint8_t Rxdata2;
////uint8_t RxBuffer2[256];
////__IO uint8_t RxCounter2;

//u8 USART2_TX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//u16 USART2_RX_STA=0;       //����״̬���	

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

////����2,printf ����
////ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
//void u2_printf(char* fmt,...)  
//{  
//	u16 i,j; 
//	va_list ap; 
//	va_start(ap,fmt);
//	vsprintf((char*)USART2_TX_BUF,fmt,ap);
//	va_end(ap);
//	i = strlen((const char*)USART2_TX_BUF);		//�˴η������ݵĳ���
//	for(j=0;j<i;j++)							//ѭ����������
//	{
//		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //ѭ������,ֱ���������   
//		USART_SendData(USART2,USART2_TX_BUF[j]); 
//	}
//}

//// �����жϷ�����
//void DEBUG_USART2_IRQHandler(void)
//{
//  uint8_t ucTemp;
//	if(USART_GetITStatus(DEBUG_USART2,USART_IT_RXNE)!=RESET)
//	{		
//		ucTemp = USART_ReceiveData(DEBUG_USART2);
////    USART_SendData(DEBUG_USART2,ucTemp);    
//		if((USART2_RX_STA&0x8000)==0)//����δ���
//		{
//			if(USART2_RX_STA&0x4000)//���յ���0x0d
//			{
//				if(ucTemp!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
//				else USART2_RX_STA|=0x8000;	//���������
//			}
//			else //��û�յ�0X0D
//			{	
//				if(ucTemp==0x0d)USART2_RX_STA|=0x4000;
//				else
//				{
//					USART2_RX_BUF[USART2_RX_STA&0X3FFF] = ucTemp ;
//					USART2_RX_STA++;
//					if(USART2_RX_STA>(USART2_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
//				}		 
//			}
//		}
//		
//	}
//		printf("%s 22",USART2_RX_BUF);
//		memset(USART2_RX_BUF, 0, sizeof(USART2_RX_BUF));

//}

///*****************  ����һ���ַ� **********************/
//void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
//{
//	/* ����һ���ֽ����ݵ�USART */
//	USART_SendData(pUSARTx,ch);
//		
//	/* �ȴ��������ݼĴ���Ϊ�� */
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
//}

///*****************  �����ַ��� **********************/
//void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
//{
//	unsigned int k=0;
//  do 
//  {
//      Usart_SendByte( pUSARTx, *(str + k) );
//      k++;
//  } while(*(str + k)!='\0');
//  
//  /* �ȴ�������� */
//  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
//  {}
//}

///*****************  ����һ��16λ�� **********************/
//void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
//{
//	uint8_t temp_h, temp_l;
//	
//	/* ȡ���߰�λ */
//	temp_h = (ch&0XFF00)>>8;
//	/* ȡ���Ͱ�λ */
//	temp_l = ch&0XFF;
//	
//	/* ���͸߰�λ */
//	USART_SendData(pUSARTx,temp_h);	
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
//	
//	/* ���͵Ͱ�λ */
//	USART_SendData(pUSARTx,temp_l);	
//	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
//}
