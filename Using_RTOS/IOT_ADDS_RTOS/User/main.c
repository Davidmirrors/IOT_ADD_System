/***********
//@Davidmirrors
//7/10/2023
//19/12/2024
//������main
//���ܣ�
// 1.
//GB2312
//.....
************/
/**
  ******************************************************************************
  * ������:Ұ�� STM32 F429 ������
  ******************************************************************************
  */
#include "stm32f4xx.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "bsp_SysTick.h"
#include "core_delay.h"  

#include "bsp_led.h"  
#include "bsp_debug_usart.h"
#include "bsp_key.h" 
#include "bsp_exti.h"
#include "bsp_dht11.h"
#include "bsp_sdram.h"
#include "bsp_lcd.h"
#include "esp8266.h"
#include "bh1750.h"

#include <string.h>

/************************************
						��������
*************************************/

static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void* pvParameters);/* LED_Task����ʵ�� */
static void KEY_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void READ_DHT11_Task(void* parameter);/* READ_DHT11_Task����ʵ�� */
static void READ_BH1750_Task(void* parameter);/* READ_DHT11_Task����ʵ�� */
static void LCD_Task(void* pvParameters);/* LCD_Task����ʵ�� */
static void WIFI_SendDate_Task(void* pvParameters);/* WIFI_SendDate_Task����ʵ�� */

static void HARDWARE_Init(void);					//Ӳ���豸��ʼ����������
static void LCD_RunTest(void);						//LCD��ʼ����������

/****************************************
								������
****************************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t LED_Task_Handle = NULL;/* LED������ */
static TaskHandle_t KEY_Task_Handle = NULL;/* KEY������ */
static TaskHandle_t READ_DHT11_Task_Handle = NULL;/* DHT11������ */
static TaskHandle_t READ_BH1750_Task_Handle = NULL;/* BH1750������ */
static TaskHandle_t LCD_Task_Handle = NULL;/* LCD������ */
static TaskHandle_t WIFI_SendDate_Task_Handle = NULL;/* LCD������ */

static QueueHandle_t xQueueDHT11_Data_Handle;/*DHT11��Ϣ����*/
static QueueHandle_t xQueueBH1750_Data_Handle;/*BH1750��Ϣ����*/

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
	
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
	
	HARDWARE_Init();
	LCD_RunTest();
	
	xQueueDHT11_Data_Handle = xQueueCreate(4,sizeof(int));
	if(xQueueDHT11_Data_Handle == NULL)
	{
		printf("Can't create DHT11_Data");
	}
	xQueueBH1750_Data_Handle = xQueueCreate(4,sizeof(int));
	if(xQueueBH1750_Data_Handle == NULL)
	{
		printf("Can't create BH1750_Data");
	}
	
	
	/* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
	
	printf("EXIT ERROR!");
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

  taskENTER_CRITICAL();           //�����ٽ���
//	Wifi_Init();
//	printf("ESP8266_Config		OK!\r\n");
//  /* ����LED_Task���� */
//  xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* ������ں��� */
//                        (const char*    )"LED_Task",/* �������� */
//                        (uint16_t       )512,   /* ����ջ��С */
//                        (void*          )NULL,	/* ������ں������� */
//                        (UBaseType_t    )2,	    /* ��������ȼ� */
//                        (TaskHandle_t*  )&LED_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("����LED_Task����ɹ�!\r\n");
//  /* ����KEY_Task���� */
//  xReturn = xTaskCreate((TaskFunction_t )KEY_Task,  /* ������ں��� */
//                        (const char*    )"KEY_Task",/* �������� */
//                        (uint16_t       )512,  /* ����ջ��С */
//                        (void*          )NULL,/* ������ں������� */
//                        (UBaseType_t    )3, /* ��������ȼ� */
//                        (TaskHandle_t*  )&KEY_Task_Handle);/* ������ƿ�ָ�� */ 
//  if(pdPASS == xReturn)
//    printf("����KEY_Task����ɹ�!\r\n");

	/* ����READ_DHT11_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )WIFI_SendDate_Task,  /* ������ں��� */
                        (const char*    )"WIFI_SendDate_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )6, /* ��������ȼ� */
                        (TaskHandle_t*  )&WIFI_SendDate_Task_Handle);/* ������ƿ�ָ�� */ 
  if(pdPASS == xReturn)
    printf("����WIFI_SendDate_Task_Handle�ɹ�!\r\n");

	/* ����READ_DHT11_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )READ_DHT11_Task,  /* ������ں��� */
                        (const char*    )"READ_DHT11_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )5, /* ��������ȼ� */
                        (TaskHandle_t*  )&READ_DHT11_Task_Handle);/* ������ƿ�ָ�� */ 
  if(pdPASS == xReturn)
    printf("����READ_DHT11_Task����ɹ�!\r\n");
	
	xReturn = xTaskCreate((TaskFunction_t )READ_BH1750_Task,  /* ������ں��� */
                        (const char*    )"READ_BH1750_Task",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )4, /* ��������ȼ� */
                        (TaskHandle_t*  )&READ_BH1750_Task_Handle);/* ������ƿ�ָ�� */ 
  if(pdPASS == xReturn)
    printf("����READ_BH1750_Task����ɹ�!\r\n");
	
	/* ����LCD_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )LCD_Task, /* ������ں��� */
                        (const char*    )"LCD_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LCD_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LCD_Task����ɹ�!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}

/**********************************************************************
  * @ ������  �� READ_DHT11_Task
  * @ ����˵���� READ_DHT11_Task���������ȡDHT11
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void READ_DHT11_Task(void* parameter)
{
	static DHT11_Data_TypeDef DHT11_Data;
//	static uint8_t dis_buf[1024];
	 while(1)
	{
		/*����DHT11_Read_TempAndHumidity��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
		if( Read_DHT11 ( & DHT11_Data ) == SUCCESS)
		{
			printf("\r\n��ȡDHT11�ɹ�!\r\nʪ��Ϊ%d.%d %%RH ���¶�Ϊ %d.%d�� \r\n\r\n",\
			DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
      
			xQueueSend(xQueueDHT11_Data_Handle,&DHT11_Data,portMAX_DELAY);//��Ϣ���з���DHT11_Data�ṹ������
		}		
		else
		{
		  printf("\r\nRead DHT11 ERROR!\r\n\r\n");
		}
    vTaskDelay(1000);   /* ��ʱ2000��tick */
	} 
}

/**********************************************************************
  * @ ������  �� READ_DHT11_Task
  * @ ����˵���� READ_DHT11_Task���������ȡDHT11
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void READ_BH1750_Task(void* parameter)
{
	static uint16_t BH1750_Date;

	 while(1)
	{
			
		/*����������*/
		BH1750_Date=IIC_BH1750_Read();
		BH1750_Date=(uint32_t)(BH1750_Date/1.20);
		xQueueSend(xQueueBH1750_Data_Handle,&BH1750_Date,portMAX_DELAY);//��Ϣ���з���BH1750_Data����
		printf("\r\nBH1750��ȡ�ɹ�!\r\n����ǿ��Ϊ:%d Lx\r\n\r\n",BH1750_Date);
		
    vTaskDelay(1000);   /* ��ʱ2000��tick */
	} 
}


/**********************************************************************
  * @ ������  �� LCD_Task
  * @ ����˵���� LCD_Task��������LCD��ʾ
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LCD_Task(void* parameter)
{	
	BaseType_t DHT11Return = pdTRUE;
	BaseType_t BH1750Return = pdTRUE;
	
	static uint8_t dis_buf[1024];
	static uint8_t lig_buf[1024];
	
	typedef struct
	{
		uint8_t  humi_int;		//ʪ�ȵ���������
		uint8_t  humi_deci;	 	//ʪ�ȵ�С������
		uint8_t  temp_int;	 	//�¶ȵ���������
		uint8_t  temp_deci;	 	//�¶ȵ�С������
		uint8_t  check_sum;	 	//У���
	}DHT11_Data2_TypeDef;
	
	static DHT11_Data2_TypeDef DHT11_Data2;
	static uint16_t BH1750_Date2;
	
  while (1)
  {
		DHT11Return = xQueueReceive(xQueueDHT11_Data_Handle,&DHT11_Data2,portMAX_DELAY);
		if(pdPASS == DHT11Return)
		{
			LCD_ClearLine(LINE(2));
			LCD_ClearLine(LINE(3));
			LCD_ClearLine(LINE(4));
			LCD_ClearLine(LINE(5));
		
			sprintf((char*)dis_buf,"Humidity: %2d.%2d %%RH ",DHT11_Data2.humi_int,DHT11_Data2.humi_deci);
			LCD_DisplayStringLine_EN_CH(LINE(4),dis_buf);
			
			sprintf((char*)dis_buf,"Temperature: %2d.%2d degree Celsius ",DHT11_Data2.temp_int,DHT11_Data2.temp_deci);
			LCD_DisplayStringLine_EN_CH(LINE(5),dis_buf);
			printf("\r\nLCD ��ʾ��ʪ��д�����\r\n");
		}
		else
		{
			LCD_ClearLine(LINE(4));
			LCD_ClearLine(LINE(5));
			LCD_SetTextColor(LCD_COLOR_RED);
			LCD_DisplayStringLine_EN_CH(LINE(2),(uint8_t* )"DHT11 initialization failed!");
			LCD_DisplayStringLine_EN_CH(LINE(3),(uint8_t* )"Please check the connection!");	
			printf("LCD ��ʾ��ʪ�ȴ�����������Ϣд�����\r\n");			
		}
		
		BH1750Return = xQueueReceive(xQueueBH1750_Data_Handle,&BH1750_Date2,portMAX_DELAY);
		if(pdPASS == BH1750Return)
		{
			LCD_ClearLine(LINE(6));
			LCD_ClearLine(LINE(7));
		
			sprintf((char*)lig_buf,"Light: %2d Lx ",BH1750_Date2);
			LCD_DisplayStringLine_EN_CH(LINE(6),(uint8_t* )"BH1750:");
			LCD_DisplayStringLine_EN_CH(LINE(7),lig_buf);

			printf("LCD ��ʾ����ǿ��д�����\r\n\r\n");
		}
		else
		{
			LCD_ClearLine(LINE(6));
			LCD_SetTextColor(LCD_COLOR_RED);
			LCD_DisplayStringLine_EN_CH(LINE(6),(uint8_t* )"BH1750 initialization failed!");
			LCD_DisplayStringLine_EN_CH(LINE(7),(uint8_t* )"Please check the connection!");	
			printf("LCD ��ʾ����ǿ�ȴ�����������Ϣд�����\r\n\r\n");			
		}
		vTaskDelay(1000);   /* ��ʱ1000��tick */
  }
}

/**********************************************************************
  * @ ������  �� WIFI_SendDate_Task
  * @ ����˵���� WIFI_SendDate_Task��������
  * @ ����    �� 
  * @ ����ֵ  �� ��
  ********************************************************************/
static void WIFI_SendDate_Task(void* parameter)
{	
  BaseType_t DHT11Return = pdTRUE;
	BaseType_t BH1750Return = pdTRUE;

	typedef struct
	{
		uint8_t  humi_int;		//ʪ�ȵ���������
		uint8_t  humi_deci;	 	//ʪ�ȵ�С������
		uint8_t  temp_int;	 	//�¶ȵ���������
		uint8_t  temp_deci;	 	//�¶ȵ�С������
		uint8_t  check_sum;	 	//У���
	}DHT11_Data3_TypeDef;
	
	struct SEND_Date
	{
		char	humi_int[10];				//ʪ�ȵ���������
		char	humi_deci[10];			//ʪ�ȵ���������
		char	temp_int[10];	 			//�¶ȵ���������
		char	temp_deci[10];			//�¶ȵ�С������
		char	bh1750_date[10];		//����ǿ��
	}SD;
	
	static DHT11_Data3_TypeDef DHT11_Data3;
	static uint16_t BH1750_Date3;
	
	char		hand[1] = "A";						//֡ͷ
	char		dht11end_flag[1] = "B";		//DHT11������־֡
	char		bh1750end_flag[1] = "C";	//��ǿ������־֡
	char		end[1] = "D";							//֡β
//	char		dot[1] = ".";							//��־֡
	char 		Encode[20] = "";					//����֡
	
	while (1)
  {
		
		DHT11Return = xQueueReceive(xQueueDHT11_Data_Handle,&DHT11_Data3,portMAX_DELAY);
		BH1750Return = xQueueReceive(xQueueBH1750_Data_Handle,&BH1750_Date3,portMAX_DELAY);
		
		if(DHT11Return == pdPASS)
		{	
			snprintf(SD.temp_int,10,"%d",DHT11_Data3.temp_int);
			snprintf(SD.temp_deci,10,"%d",DHT11_Data3.temp_deci);	
			snprintf(SD.humi_int,10,"%d",DHT11_Data3.humi_int);
			snprintf(SD.humi_deci,10,"%d",DHT11_Data3.humi_deci);
		}
		else
		{
			printf("WIFI_SendDate_Task Can't xQueueReceive DHT11_Data");
		}
		if(BH1750Return == pdPASS)
		{
			snprintf(SD.bh1750_date,10,"%d",BH1750_Date3);
		}
		else
		{
			printf("WIFI_SendDate_Task Can't xQueueReceive BH1750_Date");
		}
		strcat(Encode,hand);
		
		strcat(Encode,SD.humi_int);
		strcat(Encode,SD.humi_deci);
		strcat(Encode,SD.temp_int);
		strcat(Encode,SD.temp_deci);
		
		strcat(Encode,dht11end_flag);
		
		strcat(Encode,SD.bh1750_date);
		
		strcat(Encode,bh1750end_flag);
		strcat(Encode,end);
		
		u3_printf("%s",Encode);
		u3_printf("\r\n");
		
		printf("\r\n�ѷ�����Ϣ:%s\r\n",Encode);
		
		memset(Encode, 0, sizeof(Encode));	//�ͷ��ڴ��ֹ���
		memset(SD.humi_int, 0, sizeof(SD.humi_int));
		memset(SD.humi_deci, 0, sizeof(SD.humi_deci));
		memset(SD.temp_int, 0, sizeof(SD.temp_int));
		memset(SD.temp_deci, 0, sizeof(SD.temp_deci));
		memset(SD.bh1750_date, 0, sizeof(SD.bh1750_date));

		vTaskDelay(2000);   /* ��ʱ5000��tick */
  }
	
//֡ͷ	h_i		h_d		t_i		t_d		DHT����֡		b_date	��ǿ����֡	֡β
// A		xx		x			xx		x					B				xxxx				C					D
//
//A = 41 B = 42 C = 43 D = 44 E = 45 F = 46 G = 47 H = 48 I = 49 
//J = 4A K = 4B L = 4C M = 4D N = 4E O = 4F P = 50 Q = 51 R = 52 
//S = 53 T = 54 U = 55 V = 56 W = 57 X = 58 Y = 59 Z = 5A
 
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED_Task(void* parameter)
{	
  while (1)
  {
    LED1_ON;
    printf("LED_Task Running,LED1_ON\r\n");
    vTaskDelay(500);   /* ��ʱ500��tick */
    
    LED1_OFF;     
    printf("LED_Task Running,LED1_OFF\r\n");
    vTaskDelay(500);   /* ��ʱ500��tick */
  }
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void KEY_Task(void* parameter)
{	
  while (1)
  {
    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
    {/* K1 ������ */
//      printf("����LED����\n");
//      vTaskSuspend(LED_Task_Handle);/* ����LED���� */
//      printf("����LED����ɹ���\n");
    } 
    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
    {/* K2 ������ */
//      printf("�ָ�LED����\n");
//      vTaskResume(LED_Task_Handle);/* �ָ�LED���� */
//      printf("�ָ�LED����ɹ���\n");
    }
    vTaskDelay(20);/* ��ʱ20��tick */
  }
}

/************************
  * @ ������  �� LCD_RunTest
  * @ ����˵���� LCD_RunTest���Ժ���
  * @ ����    ��   
  * @ ����ֵ  �� ��
*************************/
static void LCD_RunTest(void)
{
	/*�ѱ�����ˢ��ɫ*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*��ʼ����Ĭ��ʹ��ǰ����*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*Ĭ�����ò�͸��	���ú�������Ϊ��͸���ȣ���Χ 0-0xff ��0Ϊȫ͸����0xffΪ��͸��*/
  LCD_SetTransparency(0xFF);
  LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);

	LCD_Clear(LCD_COLOR_BLACK); 

	/*����LCD_SetLayer(LCD_FOREGROUND_LAYER)������
	����Һ����������ǰ����ˢ�£��������µ��ù�LCD_SetLayer�������ñ�����*/
	
	LCD_DisplayStringLine_EN_CH(LINE(1),(uint8_t* )"Temperature & Humidity & Light Test Demo");
	
}

/***************************
  * @ ������  �� HARDWARE_Init
  * @ ����˵���� HARDWARE_InitӲ���豸��ʼ��
  * @ ����    ��   
  * @ ����ֵ  �� ��
***************************/
static void HARDWARE_Init(void)				//Ӳ���豸��ʼ��������������
{
/*
* STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
* ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
* ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
*/
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* ���ڳ�ʼ��	*/
	Debug_USART_Config();
  printf("USART_Config		OK!\r\n");
	Debug_USART3_Config();
  printf("USART3_Config		OK!\r\n");
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();
	printf("LED_Config		OK!\r\n");

  /* ������ʼ��	*/
  Key_GPIO_Config();
	printf("KEY_Config		OK!\r\n");

	/* ��ʼ��ϵͳ��ʱ�� */
	SysTick_Init(); 
	printf("SYSTICK_Config		OK!\r\n");

	/*��ʼ��Һ����*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	printf("LCD_Config		OK!\r\n");
		
	/* DHT11��ʼ�� */
	DHT11_GPIO_Config();
	printf("DHT11_Config		OK!\r\n");
	
	/* BH1750��ʼ�� */
	I2C_GPIO_Config();
	printf("BH1750_Config		OK!\r\n");
	
	/* ESP8266-01s��ʼ�� */
	Wifi_Init();
	printf("ESP8266_Config		OK!\r\n");
	
	printf("ALL_Config		READY!\r\n\r\n\r\n");
	
}

/*********************************************END OF FILE**********************/

