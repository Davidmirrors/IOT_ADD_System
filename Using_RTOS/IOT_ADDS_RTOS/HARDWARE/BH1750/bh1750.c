/***********
//@Davidmirrors
//18/12/2024
//19/12/2024
//BH1750��ȡ
//GB2312
//.....
************/

#include "bh1750.h"

#include "core_delay.h"
#include "bsp_debug_usart.h"	


/**
  * @brief  BH1750_I2C1 I/O����
  * @param  ��
  * @retval ��
  */
void I2C_GPIO_Config(void)
{

  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /*!< BH1750_I2C Periph clock enable */
  BH1750_I2C_CLK_INIT(BH1750_I2C_CLK, ENABLE);
  
  /*!< BH1750_I2C_SCL_GPIO_CLK and BH1750_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHB1PeriphClockCmd(BH1750_I2C_SCL_GPIO_CLK | BH1750_I2C_SDA_GPIO_CLK, ENABLE);

  /*!< GPIO configuration */
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(BH1750_I2C_SCL_GPIO_PORT, BH1750_I2C_SCL_SOURCE, BH1750_I2C_SCL_AF);
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(BH1750_I2C_SDA_GPIO_PORT, BH1750_I2C_SDA_SOURCE, BH1750_I2C_SDA_AF);  
  
  /*!< Configure BH1750_I2C pins: SCL */   
  GPIO_InitStructure.GPIO_Pin = BH1750_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(BH1750_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*!< Configure BH1750_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = BH1750_I2C_SDA_PIN;
  GPIO_Init(BH1750_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
	
	I2C_InitTypeDef  I2C_InitStructure; 

  /*I2C ����*/
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/*�ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ�*/
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
	/*����STM32����ĵ�ַ*/
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7;
	/*�������ݲ���Ӧ��*/
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
	/*I2C��Ѱַģʽ����Ϊ7λ��ַ*/
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/*ͨ������*/
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/*I2C ��ʼ��*/
  I2C_Init(BH1750_I2C, &I2C_InitStructure);
  
	/*ʹ��I2C*/
  I2C_Cmd(BH1750_I2C, ENABLE);  

	/*�����ϵ�ָ��*/
	IIC_BH1750_Write(0x01);
	/*������������ָ��*/
	IIC_BH1750_Write(0x10);
	
}

//BH1750д��
void IIC_BH1750_Write(uint8_t command)
{
	/*���IIC�Ƿ����*/
	while(I2C_GetFlagStatus(BH1750_I2C, I2C_FLAG_BUSY));
	
	/*����һ����ʼ�ź�*/
	I2C_GenerateSTART(BH1750_I2C, ENABLE);
	/*�ȴ�EV5�¼�*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*���͵�ַ���д����*/
	I2C_Send7bitAddress(BH1750_I2C, BH1750_ADDRESS, I2C_Direction_Transmitter);
	/*�ȴ�EV6�¼�*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	/*����ָ��*/
	I2C_SendData(BH1750_I2C, command);
	/*�ȴ�EV8�¼�*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/*���ͽ����ź�*/
	I2C_GenerateSTOP(BH1750_I2C, ENABLE);
}

//BH1750��ȡ
uint32_t IIC_BH1750_Read(void)
{	
	uint32_t Data=0;
	
	/*���IIC�Ƿ����*/
	while(I2C_GetFlagStatus(BH1750_I2C, I2C_FLAG_BUSY));
	
	/*����һ����ʼ�ź�*/
	I2C_GenerateSTART(BH1750_I2C, ENABLE);
	/*�ȴ�EV5�¼�*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*���͵�ַ���д����*/
	I2C_Send7bitAddress(BH1750_I2C, BH1750_ADDRESS, I2C_Direction_Receiver);
	/*�ȴ�EV6�¼�*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	/*�ȴ�EV7�¼�*/
	while(I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
	/*��ȡ���ݵĸ߰�λ*/
	Data=I2C_ReceiveData(BH1750_I2C);
	/*���ư�λ*/
	Data=Data<<8;
	/*���Ϊ��Ӧ���ź�*/
	I2C_AcknowledgeConfig(BH1750_I2C, DISABLE);
	/*���ͽ����ź�*/
	I2C_GenerateSTOP(BH1750_I2C, ENABLE);
	/*�ȴ�EV7�¼�*/
	while(I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
	/*��ȡ���ݵĵͰ�λ*/
	Data|=I2C_ReceiveData(BH1750_I2C);
	
	/*���ΪӦ���ź�*/
	I2C_AcknowledgeConfig(BH1750_I2C, ENABLE);
	
	/*��������*/
	return Data;
}
