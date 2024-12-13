
#include <string.h>
#include "stdio.h"
#include "stdlib.h"
#include <stdarg.h>
 
#include "stm32f10x.h"
#include "bsp_i2c_gpio.h"
#include "bsp_usart.h"
#include "bsp_led.h" 
#include "OLED_I2C.h"
#include "bsp_SysTick.h"
#include "esp8266.h"

extern const unsigned char BMP1[];
void Oled_Inspect(void);
void Oled_Check(void);
void GPIO_Config(void);
void Oled_Set(void);
void WIFIlink_DEcode(void);
void Oled_Show(void);
	
char STA[20] = "";
	
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

int i,k,l;
float SORH;
	
 /**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{	
	GPIO_Config();
	Oled_Set();
	
	
		while(1)
	{
		for(i = 0; i < strlen(RxBuffer2) ; i++)
		{
			STA[i] = RxBuffer2[i];
//			printf("%c\r\n",STA[i]);
			if(STA[i] == 'E') break;
		}
		printf("%s\r\n",STA);
		printf("%s\r\n",RxBuffer2);
		
		if(STA[0] == 'A' && STA[strlen(STA)-1] == 'E')
		{
			
			HI[0] = STA[1];
			HI[1] = STA[2];
			HD[0] = STA[3];
			
			TI[0] = STA[4];
			TI[1] = STA[5];
			TD[0] = STA[6];
			
			for(k = 0; k < strlen(STA) ; k++)
			{
				if(STA[k] == 'N')
				{
					for(l = 0; l <  5; l++)
					{
						if(STA[k + l + 1] == 'D') break;
						SO[l] = STA[k + l + 1];
					}
				}
				if(STA[k] == 'D')
				{
					SM[0] = STA[k + 1];
					NHL[0] = STA[k + 2];
					NH[0] = STA[k + 3];
					NL[0] = STA[k + 4];
					FA[0] = STA[k + 5];
					if(STA[k] == 'E') break;
				}
			}
//			printf("\r\n %s.%s \r\n",HI,HD);
//			printf("\r\n %s.%s \r\n",TI,TD);
//			printf("\r\n %s \r\n",SO);
//			printf("\r\n %s \r\n",SM);
//			printf("\r\n %s \r\n",NHL);
//			printf("\r\n %s \r\n",NH);
//			printf("\r\n %s \r\n",NL);
//			printf("\r\n %s \r\n",FA);
			
		}
		Oled_Show();
		Delay_s(2);
//		Oled_Inspect();
//		u2_printf("0\n");
	}
}

void Oled_Inspect(void)
{
	unsigned char i; 
	
	OLED_Fill(0xFF);//ȫ������
	Delay_s(2);
	
	OLED_Fill(0x00);//ȫ����
	Delay_s(2);
	
	for(i=0;i<4;i++)
	{
		OLED_ShowCN(22+i*16,0,i);									//������ʾ����
	}
	Delay_s(2);
	OLED_ShowStr(0,3,(unsigned char*)"Wildfire Tech",1);				//����6*8�ַ�
	OLED_ShowStr(0,4,(unsigned char*)"Hello wildfire",2);				//����8*16�ַ�
	Delay_s(200);
	OLED_CLS();//����
	OLED_OFF();//����OLED����
	Delay_s(2);
	OLED_ON();//����OLED���ߺ���
	OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
	Delay_s(2);
}

void Oled_Check(void)
{
	/*OLED������*/
	  if(OLED_Test()==0)
		{          
			/* û�м�⵽EEPROM */
			printf("\r\n û�м�⵽OLED��\r\n");
			LED1_ON;//D4��	
		}
		else
		{
			printf("\r\n OLED���ɹ�,OLED���ڹ�����\r\n");
			LED2_ON;//D5��
		}
		OLED_Fill(0xFF);//ȫ������
		Delay_s(2);
		
		OLED_Fill(0x00);//ȫ����
		Delay_s(2);
}

void GPIO_Config(void)
{
	/*��ʼ��LED������*/	
	  LED_GPIO_Config();     /* LED��ʼ�� */
	
//		USART1_Config();       /* ���ڳ�ʼ�� */
//		USART2_Config();       /* ���ڳ�ʼ�� */
	
		Uart1_GPIO_Config();
		Uart2_GPIO_Config();
	
	  SysTick_Init();        /*��ʼ���ӳٺ���*/
		i2c_CfgGpio();				 /*I2C���ߵ�GPIO��ʼ��*/
	  OLED_Init();					 /* OLED��ʼ�� */
		Oled_Check();
		Wifi_Init();
		printf("\r\n �����豸׼������ \r\n");
}

void Oled_Set(void)
{
	OLED_ShowStr(24,0,(unsigned char*) "Testing System",1);
	
	OLED_ShowStr(0,2,(unsigned char*) "TEMP",1);
	OLED_ShowStr(64,2,(unsigned char*) " RH",1);
	
	OLED_ShowStr(0,4,(unsigned char*) "SORH",1);
	OLED_ShowStr(64,4,(unsigned char*) " LV",1);
	
	OLED_ShowStr(0,6,(unsigned char*) "RELAY1",1);
	OLED_ShowStr(64,6,(unsigned char*) "RELAY2",1);
	
	OLED_ShowStr(0,7,(unsigned char*) "RELAY3",1);
	OLED_ShowStr(64,7,(unsigned char*) "RELAY4",1);
}

void WIFIlink_DEcode(void)
{	
	static int i,k,l;


/****************
			����
*****************/
	
	for(i = 0; i < strlen(RxBuffer2) ; i++)
	{
		STA[i] = RxBuffer2[i];
//			printf("%c\r\n",STA[i]);
		if(STA[i] == 'E') break;
	}

	if(STA[0] == 'A' && RxBuffer2[strlen(STA)-1] == 'E')
	{
		
		HI[0] = STA[1];
		HI[1] = STA[2];
		HD[0] = STA[3];
		
		TI[0] = STA[4];
		TI[1] = STA[5];
		TD[0] = STA[6];
		
		for(k = 0; k < strlen(STA) ; k++)
		{
			if(STA[k] == 'N')
			{
				for(l = 0; l <  5; l++)
				{
					if(STA[k + l + 1] == 'D') break;
					SO[l] = STA[k + l + 1];
				}
			}
			if(STA[k] == 'D')
			{
				SM[0] = STA[k + 1];
				NHL[0] = STA[k + 2];
				NH[0] = STA[k + 3];
				NL[0] = STA[k + 4];
				FA[0] = STA[k + 5];
				if(STA[k] == 'E') break;
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

//			memset(TI, 0, sizeof(TI));
//			memset(TD, 0, sizeof(TD));
//			memset(HI, 0, sizeof(HI));
//			memset(HD, 0, sizeof(HD));	//�ͷ��ڴ��ֹ���
//			memset(SO, 0, sizeof(SO));
//			memset(SM, 0, sizeof(SM));
//			memset(NHL, 0, sizeof(NHL));
//			memset(NH, 0, sizeof(NH));
//			memset(NL, 0, sizeof(NL));
//			memset(FA, 0, sizeof(FA));
	}
		
}

void Oled_Show(void)
{
	OLED_ShowStr(30,2,(unsigned char*)TI,1);
	OLED_ShowStr(43,2,(unsigned char*) ".",1);
	
	OLED_ShowStr(48,2,(unsigned char*)TD,1);
	
	OLED_ShowStr(88,2,(unsigned char*) HI,1);
	OLED_ShowStr(101,2,(unsigned char*) ".",1);
	
	OLED_ShowStr(106,2,(unsigned char*)HD,1);
	
	OLED_ShowStr(30,4,(unsigned char*) SO,1);
	
	if(FA[0] == '0')
	{
		OLED_ShowStr(40,6,(unsigned char*) "ON.",1);
		OLED_ShowStr(104,6,(unsigned char*) "ON.",1);

		printf("\r\n ����״̬:ON\r\n");

		printf(" RELAY0 ON\r\n");
		printf(" RELAY1 ON\r\n");
	}
	if(FA[0] == '1')
	{
		OLED_ShowStr(40,6,(unsigned char*) "OFF",1);
		OLED_ShowStr(104,6,(unsigned char*) "OFF",1);

		printf("\r\n ����״̬:OFF\r\n");
		
		printf(" RELAY0 OFF\r\n");
		printf(" RELAY1 OFF\r\n");
	}
	
	/*****************/
	
	if(NHL[0] == '0')
	{
		OLED_ShowStr(104,7,(unsigned char*) "ON.",1);
		OLED_ShowStr(88,4,(unsigned char*) "Low  ",1);
		printf(" RELAY2 ON\r\n");
	}
	if(NHL[0] == '1')
	{
		OLED_ShowStr(104,7,(unsigned char*) "OFF",1);
		OLED_ShowStr(88,4,(unsigned char*) "Fall ",1);
		printf(" RELAY2 OFF\r\n");
	}
	if(NL[0] == '1' && NH[0] == '1')
	{
		printf("\r\n ��ˮ��״̬:Fall\r\n");
	}
	if(NL[0] == '0' && NH[0] == '0')
	{
		printf("\r\n ��ˮ��״̬:Low\r\n");
	}
	if(NL[0] == '1' && NH[0] == '0')
	{
		OLED_ShowStr(88,4,(unsigned char*) "Ample",1);
		printf("\r\n ��ˮ��״̬:Ample\r\n");
	}
	if(NL[0] == '0' && NH[0] == '1')
	{
		OLED_ShowStr(88,4,(unsigned char*) "ERROR",1);
		printf("\r\n ��ˮ�ش���������\r\n");
	}
	
	SORH = atof(SO);

	if(SORH <= 40 )
	{
		printf("\r\n ����ʪ�ȹ���\r\n");
	}
	if(SORH >= 75)
	{
		printf("\r\n ����ʪ�ȸ�\r\n");
	}
	if(SORH > 40 && SORH < 75)
	{
		printf("\r\n ����ʪ������\r\n");
	}
	
	if(SM[0] == '0')
	{
		OLED_ShowStr(40,7,(unsigned char*) "ON.",1);
		printf(" RELAY3 ON\r\n");
	}
	if(SM[0] == '1')
	{
		OLED_ShowStr(40,7,(unsigned char*) "OFF",1);
		printf(" RELAY3 OFF\r\n");
	}
}


/*********************************************END OF FILE**********************/
