/***********
//@Davidmirrors
//7/10/2023
//20/3/2024
//主函数
//功能：
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


/****全局声明****/
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

/*****函数声明*****/
void uart_output(void);
void uart_DHT11_output(void);
void oled_on(void);
void GPIO_Config(void);
void OLED_Dateout(void);
void SoilADC_Dateout(void);
void WIFIlink_Encode(int ENCOND_FLAG,int DECOND_FLAG);

/*****结构体声明*****/
DHT11_Data_TypeDef DHT11_Data; //DHT11相关结构体

/*****变量定义*****/
u16 	adcx;
float temp,SORH;

char	Sorh[10],Sorh_motor[10];
char	NCLLDS_HL[10],NCLLDS_H[10],NCLLDS_L[10];
char	FAN[10];
char 	DHT_humii[10],DHT_humid[10],DHT_tempi[10],DHT_tempd[10];

char 	REEncode[20];

/*****主函数*****/
int main(void)
{ 
	
	GPIO_Config();
	oled_on();
 	TIM3_Int_Init(10000-1,8400-1);	//定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数50000次为5000ms 
	
	while(1)
	{
		
/****************
		温湿度判断
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
		水位判断
*****************/

		if(NCLLDS_High == 1 && NCLLDS_Low == 1)			//低水位
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
		
		if(NCLLDS_High == 0 && NCLLDS_Low == 0)			//高水位
		{

			NCLLDS_H_FLAG = 1;
			NCLLDS_L_FLAG = 1;
			NCLLDS_HL_FLAG = 1;
			snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
			snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);
			snprintf(NCLLDS_HL,10,"%d",NCLLDS_HL_FLAG);
			RELAY2(NCLLDS_HL_FLAG);
		
		}
		
		if(NCLLDS_High == 0 && NCLLDS_Low == 1)			//充足
		{
			NCLLDS_H_FLAG = 0;
			NCLLDS_L_FLAG = 1;
			snprintf(NCLLDS_H,10,"%d",NCLLDS_H_FLAG);
			snprintf(NCLLDS_L,10,"%d",NCLLDS_L_FLAG);
			
		}
		if(NCLLDS_High == 1 && NCLLDS_Low == 0)			//错误
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
		土壤湿度判断
*****************/
		if(Fan_FLAG != 0 && NCLLDS_HL_FLAG != 0)
		{
			if(temp <= 40 )															//湿度低
			{
				SoilRH_FLAG = 0;
				snprintf(Sorh_motor,10,"%d",SoilRH_FLAG);
				RELAY3(SoilRH_FLAG);
			}
			if(temp >= 70)															//湿度高
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
	printf("t:%d\r\n",t);	//打印数字提示程序运行
	t++;
	LED3(1);
	delay_ms(1);
	LED3(0);	
	delay_ms(999);	  //每隔1s打印一次
}

void uart_DHT11_output()
{
	static u32 t=0;
//	static char DHT_humii[10],DHT_humid[10],DHT_tempi[10],DHT_tempd[10];
	if( Read_DHT11 ( & DHT11_Data ) == SUCCESS)
		{
			printf("\r\n Read DHT11 OK!\r\n\r\n Dampness %d.%d％RH ，Temperature %d.%d℃ \r\n\r\n",\
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
		printf(" DHT读取次数 t:%d\r\n",t);	//打印数字提示程序运行
		t++;
		LED3(1);
		delay_ms(1);
		LED3(0);
//		delay_ms(997);

//		}
}

void GPIO_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);    //初始化延时函数
//	SysTick_Init();   	//初始化延迟函数
	
	uart_init(115200); 	//串口初始化 
	printf("UART1设备初始化: 	OK !\r\n");
	
	uart2_init(115200); 	//串口初始化 
	printf("UART2设备初始化:	OK !\r\n");
	
	LED_GPIO_Config();	//初始化LED端口
	printf("LED设备初始化: 		OK !\r\n");
	
	RELAY_GPIO_Config();//继电器初始化	
	printf("RELAY设备初始化: 	OK !\r\n");
	
	EXTIX_Init();       //初始化外部中断输入
	printf("EXTIX初始化: 		OK !\r\n");
	
	DHT11_GPIO_Config();//温湿度传感器初始化
	printf("DHT11设备初始化: 	OK !\r\n");
	
	I2C_Configuration();//配置CPU的硬件I2C
	printf("IIC设备初始化: 		OK !\r\n");
	
	Soiladc_Init();         //初始化ADC
	printf("ADC设备初始化: 		OK !\r\n");
	
	OLED_Init();    //初始化OLED
	OLED_Fill(0x00);//全屏灭
	printf("OLED设备初始化: 	OK !\r\n");
	
	BEEP_GPIO_Config();
	printf("BEEP设备初始化: 	OK !\r\n");
	
	Wifi_Init();
	printf("WIFI设备初始化: 	OK !\r\n");
	
//	#if WIFI_AP
//	Wifi_Init();									//配置WiFi以AP局域网模式工作
//	#else
//		while( Esp8266_Init());							//配置WiFi以STA联网模式工作
//	#endif
	
	LED2(1);					    //先点亮LED
	LED3(1);					    //先点亮LED
	delay_ms(50);					//单片机启动信号灯自检
	LED2(0);					    //熄灭LED
	LED3(0);					    //熄灭LED
	printf("LED设备自检: 		OK !\r\n");
	
	printf("所有设备初始化: 	OK !\r\n\n");
}

void oled_on(void)
{
	OLED_Fill(0xFF);//全屏点亮
	delay_ms(1000);		// 1s
	OLED_Fill(0x00);//全屏灭
	delay_ms(1000);		// 1s
	OLED_ShowCN(16,2,4);//开
	delay_ms(1000);		// 1s
	OLED_ShowCN(48,2,5);//机
	delay_ms(1000);		// 1s
	OLED_ShowCN(80,2,6);//中
	delay_ms(1000);		// 1s
	OLED_Fill(0x00);//全屏灭
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

//		OLED_Fill(0xFF);//全屏点亮
//		delay_ms(1000);		// 1s
//		
//		OLED_Fill(0x00);//全屏灭
//		delay_ms(1000);		// 1s
//		for(i = 0;i<4;i++)
//		{
//			OLED_ShowCN(22+i*16,0,i);									//测试显示中文
//		}
//		delay_ms(1000);		// 2s
//		OLED_ShowStr(0,3,(unsigned char*)"Wildfire Tech",1);				//测试6*8字符
//		OLED_ShowStr(0,4,(unsigned char*)"Hello wildfire",2);				//测试8*16字符
//		delay_ms(1000);;		// 2*100=200s
//		OLED_CLS();//清屏
//		OLED_OFF();//测试OLED休眠
//		delay_ms(1000);;		// 2s
//		OLED_ON();//测试OLED休眠后唤醒
//		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
//		delay_ms(1000);;		// 2*100=200s

//		if(0==t%300)
//		{
//			OLED_Fill(0x00);//全屏灭
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

		printf("\r\n 风扇状态:ON\r\n");

		printf(" RELAY0 ON\r\n");
		printf(" RELAY1 ON\r\n");
	}
	if(Fan_FLAG == 1)
	{
		OLED_ShowStr(40,6,(unsigned char*) "OFF",1);
		OLED_ShowStr(104,6,(unsigned char*) "OFF",1);

		printf("\r\n 风扇状态:OFF\r\n");
		
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
		printf("\r\n 蓄水池状态:Low\r\n");
	}
	if(NCLLDS_L_FLAG == 1 && NCLLDS_H_FLAG == 1)
	{
		OLED_ShowStr(88,4,(unsigned char*) "Fall ",1);
		printf("\r\n 蓄水池状态:Fall\r\n");
	}
	if(NCLLDS_L_FLAG == 0 && NCLLDS_H_FLAG == 1)
	{
		OLED_ShowStr(88,4,(unsigned char*) "Ample",1);
		printf("\r\n 蓄水池状态:Ample\r\n");
	}
	if(NCLLDS_L_FLAG == 1 && NCLLDS_H_FLAG == 0)
	{
		OLED_ShowStr(88,4,(unsigned char*) "ERROR",1);
		printf("\r\n 蓄水池传感器错误！\r\n");
	}
	
	if(temp <= 40 )
	{
		printf("\r\n 土壤湿度过低\r\n");
	}
	if(temp >= 75)
	{
		printf("\r\n 土壤湿度高\r\n");
	}
	if(temp > 40 && temp < 75)
	{
		printf("\r\n 土壤湿度适宜\r\n");
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
	adcx=Get_Soiladc_Average(ADC_Channel_5,20);//获取通道5的转换值，20次取平均
//	printf("\r\n %d\r\n",adcx);
	temp=(3.4-(float)adcx*(3.3/4096))*55;          //获取计算后的带小数的实际电压值，比如3.1111
	printf("\r\n 土壤湿度:%.1f\r\n",temp);
	
	sprintf(Sorh,"%.1f",temp);
	
}

void WIFIlink_Encode(int ENCOND_FLAG,int DECOND_FLAG)	//编码兼解码 ENCOND_FLAG为编码后内容，DECOND_FLAG为解码内容 传参0为关闭解码调试 传参1为开启解码调试 
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
		编码
*****************/

	strcat(Encode,DHT_humii);
	strcat(Encode,DHT_humid);
	strcat(Encode,DHT_tempi);
	strcat(Encode,DHT_tempd);
	
	strcat(Encode,str1);
	strcat(Encode,Sorh);
	strcat(Encode,str2);
	
	strcat(Encode,Sorh_motor);	//1 关闭
	strcat(Encode,NCLLDS_HL);		//1 关闭
	strcat(Encode,NCLLDS_H);		//1 有
	strcat(Encode,NCLLDS_L);		//1 有
	strcat(Encode,FAN);					//1 关闭
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
		printf("\r\n 发送数据为:%s\r\n",REEncode);
		printf("\r\n\r\n\r\n\r\n");
	}
		
	memset(Encode, 0, sizeof(Encode));	//释放内存防止溢出
	memset(str0, 0, sizeof(str0));
	memset(str1, 0, sizeof(str1));
	memset(str2, 0, sizeof(str2));
	
/****************
			解码
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

			memset(TEMP, 0, sizeof(TEMP));	//释放内存防止溢出
			memset(TI, 0, sizeof(TI));
			memset(TD, 0, sizeof(TD));
			memset(HI, 0, sizeof(HI));
			memset(HD, 0, sizeof(HD));	//释放内存防止溢出
			memset(SO, 0, sizeof(SO));
			memset(SM, 0, sizeof(SM));
			memset(NHL, 0, sizeof(NHL));
			memset(NH, 0, sizeof(NH));
			memset(NL, 0, sizeof(NL));
			memset(FA, 0, sizeof(FA));
		}
		
	}
	
}



