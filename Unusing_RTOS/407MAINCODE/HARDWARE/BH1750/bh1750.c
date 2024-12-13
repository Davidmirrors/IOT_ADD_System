
//#include "bh1750.h"
//#include "delay.h"

//void IIC_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	I2C_InitTypeDef  I2C_InitStructure;
//	//初始化时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
//	//配置IO口功能
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//IIC模式
//	I2C_InitStructure.I2C_OwnAddress1 = 0x0A;//作为从机地址
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//地址的位数
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//选择有应答信号
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;//配置高低电平比例
//	I2C_InitStructure.I2C_ClockSpeed = 200000;//配置时钟
//	I2C_Init(I2C2,&I2C_InitStructure);
//	
//	I2C_Cmd(I2C2,ENABLE);
////	I2C_AcknowledgeConfig(I2C2,ENABLE);	
//}

//typedef   unsigned char BYTE;
//typedef   unsigned short WORD;
// 
//void Single_Write_BH1750(u8 REG_Address)
//{
//    while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));
//	//起始信号
//	I2C_GenerateSTART(I2C2,ENABLE);
//    while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
//	
//	//发送设备地址+写信号
//	I2C_Send7bitAddress(I2C2,SlaveAddress,I2C_Direction_Transmitter);
//	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
//	
//	//内部寄存器地址
//	I2C_SendData(I2C2,REG_Address);
//	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
//	//发送停止信号
//	I2C_GenerateSTOP(I2C2,ENABLE);               
//}
// 
////*********************************************************
////
////连续读出BH1750内部数据
////
////*********************************************************
//BYTE    BUF[8];                         //接收数据缓存区   
//int     dis_data;                       //变量   	
//void Multiple_read_BH1750(void)
//{   
//	u8 i;
//	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));
//	//起始信号
//	I2C_GenerateSTART(I2C2,ENABLE);
//    while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);  
//	//发送设备地址+读信号
//	I2C_Send7bitAddress(I2C2,SlaveAddress,I2C_Direction_Receiver);
//	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
//   
//	for (i=0; i<3-1; i++)                      //连续读取2个地址数据，存储中BUF
//    {
//		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
//		BUF[i] = I2C_ReceiveData(I2C2);
//    }
//    I2C_AcknowledgeConfig(I2C2,DISABLE);
//    while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
//	BUF[i] = I2C_ReceiveData(I2C2);
//	
//	 //停止信号
//	I2C_GenerateSTOP(I2C2,ENABLE);
//	
//	I2C_AcknowledgeConfig(I2C2,ENABLE);                        
//    delay_ms(5);
//}
// 
////初始化BH1750，根据需要请参考pdf进行修改****
//void Init_BH1750(void)
//{
//	IIC_Init();
//	Single_Write_BH1750(0x01);
//	Single_Write_BH1750(0x10);   // H- resolution mode
//	delay_ms(180);
//}
// 
//float BH1750_Value;
//void BH1750_Read(void)
//{
//	
//	Multiple_read_BH1750();		 //连续读出数据，存储在BUF中
//	
//	dis_data=BUF[0];
//	dis_data=(dis_data<<8)+BUF[1];//合成数据，即光照数据
//	
//	BH1750_Value=(float)dis_data/1.20;
//	
//}
