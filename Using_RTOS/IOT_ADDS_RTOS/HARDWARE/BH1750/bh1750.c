
//#include "bh1750.h"
//#include "delay.h"

//void IIC_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	I2C_InitTypeDef  I2C_InitStructure;
//	//��ʼ��ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
//	//����IO�ڹ���
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 |GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;//IICģʽ
//	I2C_InitStructure.I2C_OwnAddress1 = 0x0A;//��Ϊ�ӻ���ַ
//	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;//��ַ��λ��
//	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;//ѡ����Ӧ���ź�
//	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_16_9;//���øߵ͵�ƽ����
//	I2C_InitStructure.I2C_ClockSpeed = 200000;//����ʱ��
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
//	//��ʼ�ź�
//	I2C_GenerateSTART(I2C2,ENABLE);
//    while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
//	
//	//�����豸��ַ+д�ź�
//	I2C_Send7bitAddress(I2C2,SlaveAddress,I2C_Direction_Transmitter);
//	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
//	
//	//�ڲ��Ĵ�����ַ
//	I2C_SendData(I2C2,REG_Address);
//	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
//	//����ֹͣ�ź�
//	I2C_GenerateSTOP(I2C2,ENABLE);               
//}
// 
////*********************************************************
////
////��������BH1750�ڲ�����
////
////*********************************************************
//BYTE    BUF[8];                         //�������ݻ�����   
//int     dis_data;                       //����   	
//void Multiple_read_BH1750(void)
//{   
//	u8 i;
//	while(I2C_GetFlagStatus(I2C2,I2C_FLAG_BUSY));
//	//��ʼ�ź�
//	I2C_GenerateSTART(I2C2,ENABLE);
//    while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);  
//	//�����豸��ַ+���ź�
//	I2C_Send7bitAddress(I2C2,SlaveAddress,I2C_Direction_Receiver);
//	while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
//   
//	for (i=0; i<3-1; i++)                      //������ȡ2����ַ���ݣ��洢��BUF
//    {
//		while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
//		BUF[i] = I2C_ReceiveData(I2C2);
//    }
//    I2C_AcknowledgeConfig(I2C2,DISABLE);
//    while(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
//	BUF[i] = I2C_ReceiveData(I2C2);
//	
//	 //ֹͣ�ź�
//	I2C_GenerateSTOP(I2C2,ENABLE);
//	
//	I2C_AcknowledgeConfig(I2C2,ENABLE);                        
//    delay_ms(5);
//}
// 
////��ʼ��BH1750��������Ҫ��ο�pdf�����޸�****
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
//	Multiple_read_BH1750();		 //�����������ݣ��洢��BUF��
//	
//	dis_data=BUF[0];
//	dis_data=(dis_data<<8)+BUF[1];//�ϳ����ݣ�����������
//	
//	BH1750_Value=(float)dis_data/1.20;
//	
//}
