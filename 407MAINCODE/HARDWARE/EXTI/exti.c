/***********
//@Davidmirrors
//7/10/2023
//9/3/2024
//�ⲿ�ж�
//�жϷ�����
//GB2312
//.....
************/

#include "exti.h"
#include "delay.h" 
#include "usart.h"
#include "led.h" 
#include "key.h"
#include "relay.h"
#include "OLED_I2C.h"
#include "ncllds.h"


//�ⲿ�ж�0�������
int a = 0;
int NCLLDS_HL_FLAG = 0;
int KEYUP_FLAG = 0;
int KEY0_FLAG = 0;
int KEY1_FLAG = 0;
int NCLLDS_H_FLAG = 0;
int NCLLDS_L_FLAG = 0;
int SoilRH_FLAG = 0;
int Fan_FLAG = 0;
void EXTI0_IRQHandler(void)
{
	delay_ms(10);	//����
	if(KEY_UP==1)	//��ӦK1����
	{
		static int flag = 1;
		
		printf("input:KEY_UP\r\n");
		LED2(1);
		delay_ms(1);
		LED2(0);
		
		flag = !flag;
		
//		RELAY0(flag);
//		RELAY1(flag);
//		RELAY2(flag);
//		RELAY3(flag);
		
		if(!flag)
		{
//			OLED_ShowStr(40,6,(unsigned char*) "ON.",1);
//			OLED_ShowStr(104,6,(unsigned char*) "ON.",1);
//			OLED_ShowStr(40,7,(unsigned char*) "ON.",1);
//			OLED_ShowStr(104,7,(unsigned char*) "ON.",1);

//			printf(" RELAY0/1 ON\r\n");
			Fan_FLAG = 1;
			RELAY0(Fan_FLAG);
			RELAY1(Fan_FLAG);
			KEY1_FLAG = 1;
			
		}
		else
		{
//			OLED_ShowStr(40,6,(unsigned char*) "OFF",1);
//			OLED_ShowStr(104,6,(unsigned char*) "OFF",1);
//			OLED_ShowStr(40,7,(unsigned char*) "OFF",1);
//			OLED_ShowStr(104,7,(unsigned char*) "OFF",1);

//			printf(" RELAY0/1 OFF\r\n");
			Fan_FLAG = 0;
			RELAY0(Fan_FLAG);
			RELAY1(Fan_FLAG);
			KEY1_FLAG = 0;
		}
		

	}		 
	 EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ 
}	

//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
//	delay_ms(10);	//����
//	if(NCLLDS_High_Scan(0)==0)	 
//	{
//		printf("\r\ninput:NCLLDS_H_FLAG\r\n");

//		printf("NCLLDS_H_FLAG ON\r\n");
//		NCLLDS_H_FLAG = 1;
//		printf("NCLLDS_H_FLAG %d\r\n",NCLLDS_H_FLAG);
//	}
//	else
//	{
//		printf("NCLLDS_H_FLAG OFF\r\n");
//		NCLLDS_H_FLAG = 0;
//		printf("NCLLDS_H_FLAG %d\r\n",NCLLDS_H_FLAG);
//	}

//	if(NCLLDS_H_FLAG == 1 && NCLLDS_H_FLAG == 1)
//	{
//		RELAY2(1);
//		printf("RELAY2 ON\r\n");
//		NCLLDS_HL_FLAG = 1;
//		printf("NCLLDS_HL_FLAG %d\r\n",NCLLDS_HL_FLAG);
//		
//		LED2(1);
//		delay_ms(1);
//		LED2(0);
//	}
//	else{}


	EXTI_ClearITPendingBit(EXTI_Line1);//���LINE1�ϵ��жϱ�־λ 
}

//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
//	delay_ms(10);	//����
//	if(NCLLDS_Low_Scan(0)==0)	 
//	{
//		printf("\r\ninput:NCLLDS_L_FLAG\r\n");
//		
//		printf("NCLLDS_L_FLAG ON\r\n");
//		NCLLDS_L_FLAG = 1;
//		printf("NCLLDS_L_FLAG %d\r\n",NCLLDS_L_FLAG);
//	}
//	else
//	{
//		printf("NCLLDS_L_FLAG OFF\r\n");
//		NCLLDS_L_FLAG = 0;
//		printf("NCLLDS_L_FLAG %d\r\n",NCLLDS_L_FLAG);
//	}

//	if(NCLLDS_L_FLAG == 0 && NCLLDS_H_FLAG == 0)
//	{
//		RELAY2(0);
//		printf("RELAY2 OFF\r\n");
//		NCLLDS_HL_FLAG = 0;
//		printf("NCLLDS_HL_FLAG %d\r\n",NCLLDS_HL_FLAG);
//		
//		LED2(1);
//		delay_ms(1);
//		LED2(0);
//	}
//	else{}

	EXTI_ClearITPendingBit(EXTI_Line2);//���LINE2�ϵ��жϱ�־λ 
}

//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	static int flag = 1;
	
	delay_ms(10);	//����
	if(KEY_1==0)	 
	{
		printf("input:KEY_1\r\n");
		LED2(1);
		delay_ms(1);
		LED2(0);
		
		flag = !flag;

		if(!flag)
		{
//			OLED_ShowStr(40,6,(unsigned char*) "ON.",1);
//			OLED_ShowStr(104,6,(unsigned char*) "ON.",1);
//			OLED_ShowStr(40,7,(unsigned char*) "ON.",1);
//			OLED_ShowStr(104,7,(unsigned char*) "ON.",1);
//			printf("RELAY2 ON\r\n");
			NCLLDS_HL_FLAG = 1;
			RELAY2(NCLLDS_HL_FLAG);
			KEY1_FLAG = 1;
			
		}
		else
		{
//			OLED_ShowStr(40,6,(unsigned char*) "OFF",1);
//			OLED_ShowStr(104,6,(unsigned char*) "OFF",1);
//			OLED_ShowStr(40,7,(unsigned char*) "OFF",1);
//			OLED_ShowStr(104,7,(unsigned char*) "OFF",1);
//			printf("RELAY2 OFF\r\n");
			NCLLDS_HL_FLAG = 0;
			RELAY2(NCLLDS_HL_FLAG);
			KEY1_FLAG = 0;
		}
		
		
//		RELAY0(0);
//		RELAY1(1);
//		RELAY2(0);
//		RELAY3(1);
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
}

//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	static int flag = 1;
	
	delay_ms(10);	//����
	if(KEY_0==0)	 
	{				 
		printf("input:KEY_0\r\n");
		LED2(1);
		delay_ms(1);
		LED2(0);
		
		flag = !flag;
		
		if(!flag)
		{
//			OLED_ShowStr(40,6,(unsigned char*) "ON.",1);
//			OLED_ShowStr(104,6,(unsigned char*) "ON.",1);
//			OLED_ShowStr(40,7,(unsigned char*) "ON.",1);
//			OLED_ShowStr(104,7,(unsigned char*) "ON.",1);
//			printf("RELAY3 ON\r\n");
			SoilRH_FLAG = 1;
			RELAY3(SoilRH_FLAG);
			KEY0_FLAG = 1;
			
		}
		else
		{
//			OLED_ShowStr(40,6,(unsigned char*) "OFF",1);
//			OLED_ShowStr(104,6,(unsigned char*) "OFF",1);
//			OLED_ShowStr(40,7,(unsigned char*) "OFF",1);
//			OLED_ShowStr(104,7,(unsigned char*) "OFF",1);
//			printf("RELAY3 OFF\r\n");
			SoilRH_FLAG = 0;
			RELAY3(SoilRH_FLAG);
			KEY0_FLAG = 0;
			
		}
			
//		RELAY0(1);
//		RELAY1(0);
//		RELAY2(1);
//		RELAY3(0);
		
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line4);//���LINE4�ϵ��жϱ�־λ  
}
	   
//�ⲿ�жϳ�ʼ������
//��ʼ��PE2~4,PA0Ϊ�ж�����.
void EXTIX_Init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	
	KEYUP_GPIO_Config();				//IO��ʼ��
	KEY01_GPIO_Config();
	RELAY_GPIO_Config();				//�̵�����ʼ��
	Ncllds12HL_GPIO_Config();		//�ǽӴ�ʽҺλ��������ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);//PE1 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 ���ӵ��ж���2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 ���ӵ��ж���3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 ���ӵ��ж���4
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 ���ӵ��ж���0--��ӦKUP����
	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource2);//PC2 ���ӵ��ж���2,,
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource3);//PC3 ���ӵ��ж���3,,
	
  /* ����EXTI_Line0 */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE0
  EXTI_Init(&EXTI_InitStructure);//����
	
////	/* ����EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2 | EXTI_Line3 | EXTI_Line4 ;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//�ж���ʹ��
  EXTI_Init(&EXTI_InitStructure);//����
//// 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ⲿ�ж�0
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//��ռ���ȼ�0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
////
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
////
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ⲿ�ж�2
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//��ռ���ȼ�3
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
////	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//�ⲿ�ж�3
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//��ռ���ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//�ⲿ�ж�4
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//��ռ���ȼ�1
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//�����ȼ�2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);//����
	
	
}












