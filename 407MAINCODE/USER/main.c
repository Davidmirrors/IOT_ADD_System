/***********
//@Davidmirrors
//7/10/2023
//20/3/2024
//������
//���ܣ�
// 1.
//GB2312
//.....
************/

#include "stdio.h"
#include "stdlib.h"

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "relay.h"
#include "bsp_dht11.h"
#include "OLED_I2C.h"
#include "mtf01.h"
#include "Soil_moisture.h"
#include "ncllds.h"
#include "esp8266.h"
#include "timer.h"


/****ȫ������****/
extern const unsigned char BMP1[];

extern int NCLLDS_HL_FLAG;
extern int NCLLDS_H_FLAG;
extern int NCLLDS_L_FLAG;

extern int SoilRH_FLAG;
extern int Fan_FLAG;

extern int KEYUP_FLAG;
extern int KEY0_FLAG;
extern int KEY1_FLAG;

extern int WIFI_Mode_FLAG;

/*****��������*****/
void uart_output(void);
void uart_DHT11_output(void);
void oled_on(void);
void GPIO_Config(void);
void OLED_Dateout(void);
void SoilADC_Dateout(void);
void WIFIlink_Encode(int ENCOND_FLAG,int DECOND_FLAG);

/*****�ṹ������*****/
DHT11_Data_TypeDef DHT11_Data; //DHT11��ؽṹ��

/*****��������*****/
u16 	adcx;
float temp,SORH;

char	Sorh[10],Sorh_motor[10];
char	NCLLDS_HL[10],NCLLDS_H[10],NCLLDS_L[10];
char	FAN[10];
char 	DHT_humii[10],DHT_humid[10],DHT_tempi[10],DHT_tempd[10];

char 	REEncode[20];

/*****������*****/
int main(void)
{ 
	
	GPIO_Config();
	oled_on();
 	TIM3_Int_Init(10000-1,8400-1);	//��ʱ��ʱ��84M����Ƶϵ��8400������84M/8400=10Khz�ļ���Ƶ�ʣ�����50000��Ϊ5000ms 
	
	while(1)
	{
		
/****************
		��ʪ���ж�
*****************/
		
		if(DHT11_Data.temp_int >= 28 || DHT11_Data.humi_int >= 80)
		{
			Fan_FLAG = 0;
			snprintf(FAN,10,"%d",Fan_FLAG);
			RELAY0(Fan_FLAG);
			RELAY1(Fan_FLAG);
		}
		if(DHT11_Data.temp_int < 26 && DHT11_Data.humi_int < 60)
		{
			Fan_FLAG = 1;
			snprintf(FAN,10,"%d",Fan_FLAG);
			RELAY0(Fan_FLAG);
			RELAY1(Fan_FLAG);
		}
		
/****************
		ˮλ�ж�
*****************/

		if(NCLLDS_High == 1 && NCLLDS_Low == 1)			//��ˮλ
		{
			NCLLDS_H_FLAG = 0;
			NCLLDS_L_FLAG = 0;
			snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
			snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);
			if(Fan_FLAG != 0)
			{
				NCLLDS_HL_FLAG = 0;
				snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
				snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);
				snprintf(NCLLDS_HL,10,"%d",NCLLDS_HL_FLAG);
				RELAY2(NCLLDS_HL_FLAG);
			}
			if(Fan_FLAG == 0)
			{
				NCLLDS_HL_FLAG = 1;
				snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
				snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);
				snprintf(NCLLDS_HL,10,"%d",NCLLDS_HL_FLAG);
				
			}
		
		}
		
		if(NCLLDS_High == 0 && NCLLDS_Low == 0)			//��ˮλ
		{

			NCLLDS_H_FLAG = 1;
			NCLLDS_L_FLAG = 1;
			NCLLDS_HL_FLAG = 1;
			snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
			snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);
			snprintf(NCLLDS_HL,10,"%d",NCLLDS_HL_FLAG);
			RELAY2(NCLLDS_HL_FLAG);
		
		}
		
		if(NCLLDS_High == 0 && NCLLDS_Low == 1)			//����
		{
			NCLLDS_H_FLAG = 0;
			NCLLDS_L_FLAG = 1;
			snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
			snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);
			
		}
		if(NCLLDS_High == 1 && NCLLDS_Low == 0)			//����
		{
			NCLLDS_H_FLAG = 1;
			NCLLDS_L_FLAG = 0;
			snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
			snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);;
			
		}
		if (Fan_FLAG == 0)
		{
			NCLLDS_HL_FLAG = 1;
			snprintf(NCLLDS_HL,10,"%d",NCLLDS_HL_FLAG);
			RELAY2(NCLLDS_HL_FLAG);
			
			SoilRH_FLAG = 1;
			snprintf(Sorh_motor,10,"%d",SoilRH_FLAG);
			RELAY3(SoilRH_FLAG);
			
		}
		
/****************
		����ʪ���ж�
*****************/
		if(Fan_FLAG != 0 && NCLLDS_HL_FLAG != 0)
		{
			if(temp <= 40 )															//ʪ�ȵ�
			{
				SoilRH_FLAG = 0;
				snprintf(Sorh_motor,10,"%d",SoilRH_FLAG);
				RELAY3(SoilRH_FLAG);
			}
			if(temp >= 70)															//ʪ�ȸ�
			{
				SoilRH_FLAG = 1;
				snprintf(Sorh_motor,10,"%d",SoilRH_FLAG);
				RELAY3(SoilRH_FLAG);
			}
		}
		if(Fan_FLAG == 0 || NCLLDS_HL_FLAG == 0)
		{
			SoilRH_FLAG = 1;
			snprintf(Sorh_motor,10,"%d",SoilRH_FLAG);
			RELAY3(SoilRH_FLAG);

		}
		
	}

}

void uart_output(void)
{
	static u32 t=0;
	printf("t:%d\r\n",t);	//��ӡ������ʾ��������
	t++;
	LED3(1);
	delay_ms(1);
	LED3(0);	
	delay_ms(999);	  //ÿ��1s��ӡһ��
}

void uart_DHT11_output()
{
	static u32 t=0;
//	static char DHT_humii[10],DHT_humid[10],DHT_tempi[10],DHT_tempd[10];
	if( Read_DHT11 ( & DHT11_Data ) == SUCCESS)
		{
			printf("\r\n Read DHT11 OK!\r\n\r\n Dampness %d.%d��RH ��Temperature %d.%d�� \r\n\r\n",\
			DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
			
			snprintf(DHT_tempi,10,"%d",DHT11_Data.temp_int);
			snprintf(DHT_tempd,10,"%d",DHT11_Data.temp_deci);	
			snprintf(DHT_humii,10,"%d",DHT11_Data.humi_int);
			snprintf(DHT_humid,10,"%d",DHT11_Data.humi_deci);
			
			LED3(1);
			delay_ms(1);
			LED3(0);	
		}		
		else
		{
		  printf("\r\n Read DHT11 ERROR!\r\n");
			LED3(1);
			delay_ms(1);
			LED3(0);	
		}
    delay_ms(1);
		printf(" DHT��ȡ���� t:%d\r\n",t);	//��ӡ������ʾ��������
		t++;
		LED3(1);
		delay_ms(1);
		LED3(0);
//		delay_ms(997);

//		}
}

void GPIO_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init(168);    //��ʼ����ʱ����
//	SysTick_Init();   	//��ʼ���ӳٺ���
	
	uart_init(115200); 	//���ڳ�ʼ�� 
	printf("UART1�豸��ʼ��: 	OK !\r\n");
	
	uart2_init(115200); 	//���ڳ�ʼ�� 
	printf("UART2�豸��ʼ��:	OK !\r\n");
	
	LED_GPIO_Config();	//��ʼ��LED�˿�
	printf("LED�豸��ʼ��: 		OK !\r\n");
	
	RELAY_GPIO_Config();//�̵�����ʼ��	
	printf("RELAY�豸��ʼ��: 	OK !\r\n");
	
	EXTIX_Init();       //��ʼ���ⲿ�ж�����
	printf("EXTIX��ʼ��: 		OK !\r\n");
	
	DHT11_GPIO_Config();//��ʪ�ȴ�������ʼ��
	printf("DHT11�豸��ʼ��: 	OK !\r\n");
	
	I2C_Configuration();//����CPU��Ӳ��I2C
	printf("IIC�豸��ʼ��: 		OK !\r\n");
	
	Soiladc_Init();         //��ʼ��ADC
	printf("ADC�豸��ʼ��: 		OK !\r\n");
	
	OLED_Init();    //��ʼ��OLED
	OLED_Fill(0x00);//ȫ����
	printf("OLED�豸��ʼ��: 	OK !\r\n");
	
	BEEP_GPIO_Config();
	printf("BEEP�豸��ʼ��: 	OK !\r\n");
	
	Wifi_Init();
	printf("WIFI�豸��ʼ��: 	OK !\r\n");
	
//	#if WIFI_AP
//	Wifi_Init();									//����WiFi��AP������ģʽ����
//	#else
//		while( Esp8266_Init());							//����WiFi��STA����ģʽ����
//	#endif
	
	LED2(1);					    //�ȵ���LED
	LED3(1);					    //�ȵ���LED
	delay_ms(50);					//��Ƭ�������źŵ��Լ�
	LED2(0);					    //Ϩ��LED
	LED3(0);					    //Ϩ��LED
	printf("LED�豸�Լ�: 		OK !\r\n");
	
	printf("�����豸��ʼ��: 	OK !\r\n\n");
}

void oled_on(void)
{
	OLED_Fill(0xFF);//ȫ������
	delay_ms(1000);		// 1s
	OLED_Fill(0x00);//ȫ����
	delay_ms(1000);		// 1s
	OLED_ShowCN(16,2,4);//��
	delay_ms(1000);		// 1s
	OLED_ShowCN(48,2,5);//��
	delay_ms(1000);		// 1s
	OLED_ShowCN(80,2,6);//��
	delay_ms(1000);		// 1s
	OLED_Fill(0x00);//ȫ����
	delay_ms(1000);		// 1s
	
	OLED_ShowStr(24,0,(unsigned char*) "Testing System",1);
	
	OLED_ShowStr(0,2,(unsigned char*) "TEMP",1);
	OLED_ShowStr(64,2,(unsigned char*) " RH",1);
	
	OLED_ShowStr(0,4,(unsigned char*) "SORH",1);
	OLED_ShowStr(64,4,(unsigned char*) " LV",1);
	
	OLED_ShowStr(0,6,(unsigned char*) "RELAY1",1);
	OLED_ShowStr(64,6,(unsigned char*) "RELAY2",1);
	
	OLED_ShowStr(0,7,(unsigned char*) "RELAY3",1);
	OLED_ShowStr(64,7,(unsigned char*) "RELAY4",1);
	
	
//	OLED_ShowCN(0,2,9);
//	OLED_ShowCN(16,2,8);
//	OLED_ShowCN(64,2,7);
//	OLED_ShowCN(80,2,8);
//	OLED_ShowCN(0,2,9);
//	OLED_ShowCN(0,2,9);

/*

//		OLED_Fill(0xFF);//ȫ������
//		delay_ms(1000);		// 1s
//		
//		OLED_Fill(0x00);//ȫ����
//		delay_ms(1000);		// 1s
//		for(i = 0;i<4;i++)
//		{
//			OLED_ShowCN(22+i*16,0,i);									//������ʾ����
//		}
//		delay_ms(1000);		// 2s
//		OLED_ShowStr(0,3,(unsigned char*)"Wildfire Tech",1);				//����6*8�ַ�
//		OLED_ShowStr(0,4,(unsigned char*)"Hello wildfire",2);				//����8*16�ַ�
//		delay_ms(1000);;		// 2*100=200s
//		OLED_CLS();//����
//		OLED_OFF();//����OLED����
//		delay_ms(1000);;		// 2s
//		OLED_ON();//����OLED���ߺ���
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
//		delay_ms(1000);;		// 2*100=200s

//		if(0==t%300)
//		{
//			OLED_Fill(0x00);//ȫ����
//			OLED_ShowStr(24,0,(unsigned char*) "Testing System",1);
//	
//			OLED_ShowStr(0,5,(unsigned char*) "RELAY1",1);
//			OLED_ShowStr(64,5,(unsigned char*) "RELAY2",1);
//			
//			OLED_ShowStr(0,7,(unsigned char*) "RELAY3",1);
//			OLED_ShowStr(64,7,(unsigned char*) "RELAY4",1);
//			
//			OLED_ShowCN(0,2,9);
//			OLED_ShowCN(16,2,8);
//			OLED_ShowCN(64,2,7);
//			OLED_ShowCN(80,2,8);
//			printf("reload:%d",t%300);

*/

}

void OLED_Dateout(void)
{
	OLED_ShowStr(30,2,(unsigned char*)DHT_tempi,1);
	OLED_ShowStr(43,2,(unsigned char*) ".",1);
	
	OLED_ShowStr(48,2,(unsigned char*)DHT_tempd,1);
	
	OLED_ShowStr(88,2,(unsigned char*) DHT_humii,1);
	OLED_ShowStr(101,2,(unsigned char*) ".",1);
	
	OLED_ShowStr(106,2,(unsigned char*)DHT_humid,1);
	
	OLED_ShowStr(30,4,(unsigned char*) Sorh,1);
	
	if(Fan_FLAG == 0)
	{
		OLED_ShowStr(40,6,(unsigned char*) "ON.",1);
		OLED_ShowStr(104,6,(unsigned char*) "ON.",1);

		printf("\r\n ����״̬:ON\r\n");

		printf(" RELAY0 ON\r\n");
		printf(" RELAY1 ON\r\n");
	}
	if(Fan_FLAG == 1)
	{
		OLED_ShowStr(40,6,(unsigned char*) "OFF",1);
		OLED_ShowStr(104,6,(unsigned char*) "OFF",1);

		printf("\r\n ����״̬:OFF\r\n");
		
		printf(" RELAY0 OFF\r\n");
		printf(" RELAY1 OFF\r\n");
	}
	
	if(NCLLDS_HL_FLAG == 0)
	{
		OLED_ShowStr(104,7,(unsigned char*) "ON.",1);
		printf(" RELAY2 ON\r\n");
	}
	if(NCLLDS_HL_FLAG == 1)
	{
		OLED_ShowStr(104,7,(unsigned char*) "OFF",1);
		printf(" RELAY2 OFF\r\n");
	}
	if(NCLLDS_L_FLAG == 0 && NCLLDS_H_FLAG == 0)
	{
		OLED_ShowStr(88,4,(unsigned char*) "Low  ",1);
		printf("\r\n ��ˮ��״̬:Low\r\n");
	}
	if(NCLLDS_L_FLAG == 1 && NCLLDS_H_FLAG == 1)
	{
		OLED_ShowStr(88,4,(unsigned char*) "Fall ",1);
		printf("\r\n ��ˮ��״̬:Fall\r\n");
	}
	if(NCLLDS_L_FLAG == 0 && NCLLDS_H_FLAG == 1)
	{
		OLED_ShowStr(88,4,(unsigned char*) "Ample",1);
		printf("\r\n ��ˮ��״̬:Ample\r\n");
	}
	if(NCLLDS_L_FLAG == 1 && NCLLDS_H_FLAG == 0)
	{
		OLED_ShowStr(88,4,(unsigned char*) "ERROR",1);
		printf("\r\n ��ˮ�ش���������\r\n");
	}
	
	if(temp <= 40 )
	{
		printf("\r\n ����ʪ�ȹ���\r\n");
	}
	if(temp >= 75)
	{
		printf("\r\n ����ʪ�ȸ�\r\n");
	}
	if(temp > 40 && temp < 75)
	{
		printf("\r\n ����ʪ������\r\n");
	}
	if(SoilRH_FLAG == 0)
	{
		OLED_ShowStr(40,7,(unsigned char*) "ON.",1);
		printf(" RELAY3 ON\r\n");
	}
	if(SoilRH_FLAG == 1)
	{
		OLED_ShowStr(40,7,(unsigned char*) "OFF",1);
		printf(" RELAY3 OFF\r\n");
	}
	
	printf("\r\n");
	
}

void SoilADC_Dateout(void)
{
	adcx=Get_Soiladc_Average(ADC_Channel_5,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
//	printf("\r\n %d\r\n",adcx);
	temp=(3.4-(float)adcx*(3.3/4096))*55;          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
	printf("\r\n ����ʪ��:%.1f\r\n",temp);
	
	sprintf(Sorh,"%.1f",temp);
	
}

void WIFIlink_Encode(int ENCOND_FLAG,int DECOND_FLAG)	//�������� ENCOND_FLAGΪ��������ݣ�DECOND_FLAGΪ�������� ����0Ϊ�رս������ ����1Ϊ����������� 
{
	char str0[1] = "E";
	char str1[1] = "N";
	char str2[1] = "D";
	char Encode[20] = "A";
	char TEMP[20];
	
	char TI[10] = "";
	char TD[10] = "";
	char HI[10] = "";
	char HD[10] = "";
	char SO[10] = "";
	char SM[10] = "";
	char NHL[10] = "";
	char NH[10] = "";
	char NL[10] = "";
	char FA[10] = "";
	
	static int i,j,k,l;
	
	int Encond_FLAG = ENCOND_FLAG;	
	int Decond_FLAG = DECOND_FLAG;	
	
	
/****************
		����
*****************/

	strcat(Encode,DHT_humii);
	strcat(Encode,DHT_humid);
	strcat(Encode,DHT_tempi);
	strcat(Encode,DHT_tempd);
	
	strcat(Encode,str1);
	strcat(Encode,Sorh);
	strcat(Encode,str2);
	
	strcat(Encode,Sorh_motor);	//1 �ر�
	strcat(Encode,NCLLDS_HL);		//1 �ر�
	strcat(Encode,NCLLDS_H);		//1 ��
	strcat(Encode,NCLLDS_L);		//1 ��
	strcat(Encode,FAN);					//1 �ر�
	strcat(Encode,str0);
	
	for(i = 0; i < strlen(Encode) ; i++)
	{
		REEncode[i] = Encode[i];
	}
	if(WIFI_Mode_FLAG == 0)
	{
		u2_printf("AT+CIPSEND=0,%d\r\n",strlen(Encode));
		delay_ms(10);
		u2_printf("%s,tcp\r\n",Encode);
//		u2_printf("hello,tcp\r\n");
	}
	if(WIFI_Mode_FLAG == 1)
	{
		u2_printf("%s\n",Encode);
	}
	if (Encond_FLAG)
	{
		printf("\r\n ��������Ϊ:%s\r\n",REEncode);
		printf("\r\n\r\n\r\n\r\n");
	}
		
	memset(Encode, 0, sizeof(Encode));	//�ͷ��ڴ��ֹ���
	memset(str0, 0, sizeof(str0));
	memset(str1, 0, sizeof(str1));
	memset(str2, 0, sizeof(str2));
	
/****************
			����
*****************/
	if (Decond_FLAG)
	{
		for(j = 0; j < strlen(REEncode) ; j++)
		{
			TEMP[j] = REEncode[j];
		}
		if(TEMP[0] == 'A' && TEMP[strlen(TEMP)-1] == 'E')
		{
			
			HI[0] = TEMP[1];
			HI[1] = TEMP[2];
			HD[0] = TEMP[3];
			
			TI[0] = TEMP[4];
			TI[1] = TEMP[5];
			TD[0] = TEMP[6];
			
			for(k = 0; k < strlen(TEMP) ; k++)
			{
				if(TEMP[k] == 'N')
				{
					for(l = 0; l <  5; l++)
					{
						if(TEMP[k + l + 1] == 'D') break;
						SO[l] = TEMP[k + l + 1];
					}
				}
				if(TEMP[k] == 'D')
				{
					SM[0] = TEMP[k + 1];
					NHL[0] = TEMP[k + 2];
					NH[0] = TEMP[k + 3];
					NL[0] = TEMP[k + 4];
					FA[0] = TEMP[k + 5];
					if(TEMP[k] == 'E') break;
				}
			}
				
			printf("\r\n %s.%s \r\n",HI,HD);
			printf("\r\n %s.%s \r\n",TI,TD);
			printf("\r\n %s \r\n",SO);
			printf("\r\n %s \r\n",SM);
			printf("\r\n %s \r\n",NHL);
			printf("\r\n %s \r\n",NH);
			printf("\r\n %s \r\n",NL);
			printf("\r\n %s \r\n",FA);
			
			printf("\r\n\r\n\r\n\r\n");

			memset(TEMP, 0, sizeof(TEMP));	//�ͷ��ڴ��ֹ���
			memset(TI, 0, sizeof(TI));
			memset(TD, 0, sizeof(TD));
			memset(HI, 0, sizeof(HI));
			memset(HD, 0, sizeof(HD));	//�ͷ��ڴ��ֹ���
			memset(SO, 0, sizeof(SO));
			memset(SM, 0, sizeof(SM));
			memset(NHL, 0, sizeof(NHL));
			memset(NH, 0, sizeof(NH));
			memset(NL, 0, sizeof(NL));
			memset(FA, 0, sizeof(FA));
		}
		
	}
	
}



