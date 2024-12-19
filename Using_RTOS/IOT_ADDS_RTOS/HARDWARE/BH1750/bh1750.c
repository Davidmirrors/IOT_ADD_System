/***********
//@Davidmirrors
//18/12/2024
//19/12/2024
//BH1750读取
//GB2312
//.....
************/

#include "bh1750.h"

#include "core_delay.h"
#include "bsp_debug_usart.h"	


/**
  * @brief  BH1750_I2C1 I/O配置
  * @param  无
  * @retval 无
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

  /*I2C 配置*/
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/*高电平数据稳定，低电平数据变化 SCL 时钟线的占空比*/
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
	/*设置STM32自身的地址*/
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7;
	/*接收数据产生应答*/
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	 
	/*I2C的寻址模式，设为7位地址*/
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/*通信速率*/
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/*I2C 初始化*/
  I2C_Init(BH1750_I2C, &I2C_InitStructure);
  
	/*使能I2C*/
  I2C_Cmd(BH1750_I2C, ENABLE);  

	/*发送上电指令*/
	IIC_BH1750_Write(0x01);
	/*发送连续测量指令*/
	IIC_BH1750_Write(0x10);
	
}

//BH1750写入
void IIC_BH1750_Write(uint8_t command)
{
	/*检测IIC是否空闲*/
	while(I2C_GetFlagStatus(BH1750_I2C, I2C_FLAG_BUSY));
	
	/*发送一个开始信号*/
	I2C_GenerateSTART(BH1750_I2C, ENABLE);
	/*等待EV5事件*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*发送地址外加写命令*/
	I2C_Send7bitAddress(BH1750_I2C, BH1750_ADDRESS, I2C_Direction_Transmitter);
	/*等待EV6事件*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	/*发送指令*/
	I2C_SendData(BH1750_I2C, command);
	/*等待EV8事件*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/*发送结束信号*/
	I2C_GenerateSTOP(BH1750_I2C, ENABLE);
}

//BH1750读取
uint32_t IIC_BH1750_Read(void)
{	
	uint32_t Data=0;
	
	/*检测IIC是否空闲*/
	while(I2C_GetFlagStatus(BH1750_I2C, I2C_FLAG_BUSY));
	
	/*发送一个开始信号*/
	I2C_GenerateSTART(BH1750_I2C, ENABLE);
	/*等待EV5事件*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	/*发送地址外加写命令*/
	I2C_Send7bitAddress(BH1750_I2C, BH1750_ADDRESS, I2C_Direction_Receiver);
	/*等待EV6事件*/
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	/*等待EV7事件*/
	while(I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
	/*获取数据的高八位*/
	Data=I2C_ReceiveData(BH1750_I2C);
	/*右移八位*/
	Data=Data<<8;
	/*变更为非应答信号*/
	I2C_AcknowledgeConfig(BH1750_I2C, DISABLE);
	/*发送结束信号*/
	I2C_GenerateSTOP(BH1750_I2C, ENABLE);
	/*等待EV7事件*/
	while(I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)==0);
	/*获取数据的低八位*/
	Data|=I2C_ReceiveData(BH1750_I2C);
	
	/*变更为应答信号*/
	I2C_AcknowledgeConfig(BH1750_I2C, ENABLE);
	
	/*返回数据*/
	return Data;
}
