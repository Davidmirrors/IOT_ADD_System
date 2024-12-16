/***********
//@Davidmirrors
//7/10/2023
//20/3/2024
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

#include <string.h>

/************************************
						��������
*************************************/

static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void* pvParameters);/* LED_Task����ʵ�� */
static void KEY_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void READ_DHT11_Task(void* parameter);/* READ_DHT11_Task����ʵ�� */
static void LCD_Task(void* pvParameters);/* LCD_Task����ʵ�� */

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
static TaskHandle_t LCD_Task_Handle = NULL;/* LCD������ */

static QueueHandle_t xQueueDHT11_Data_Handle;

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
//  xReturn = xTaskCreate((TaskFunction_t )READ_DHT11_Task,  /* ������ں��� */
//                        (const char*    )"READ_DHT11_Task",/* �������� */
//                        (uint16_t       )512,  /* ����ջ��С */
//                        (void*          )NULL,/* ������ں������� */
//                        (UBaseType_t    )4, /* ��������ȼ� */
//                        (TaskHandle_t*  )&READ_DHT11_Task_Handle);/* ������ƿ�ָ�� */ 
//  if(pdPASS == xReturn)
//    printf("����READ_DHT11_Task����ɹ�!\r\n");
//	
//	/* ����LCD_Task���� */
//  xReturn = xTaskCreate((TaskFunction_t )LCD_Task, /* ������ں��� */
//                        (const char*    )"LCD_Task",/* �������� */
//                        (uint16_t       )512,   /* ����ջ��С */
//                        (void*          )NULL,	/* ������ں������� */
//                        (UBaseType_t    )1,	    /* ��������ȼ� */
//                        (TaskHandle_t*  )&LCD_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("����LCD_Task����ɹ�!\r\n");
  
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
			printf("\r\n��ȡDHT11�ɹ�!\r\n\r\nʪ��Ϊ%d.%d %%RH ���¶�Ϊ %d.%d�� \r\n",\
			DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
      
			xQueueSend(xQueueDHT11_Data_Handle,&DHT11_Data,portMAX_DELAY);

		}		
		else
		{
		  printf("\r\nRead DHT11 ERROR!\r\n\r\n");
		}
    vTaskDelay(2000);   /* ��ʱ2000��tick */
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
	BaseType_t xReturn = pdTRUE;
	
	static uint8_t dis_buf[1024];
	typedef struct
	{
		uint8_t  humi_int;		//ʪ�ȵ���������
		uint8_t  humi_deci;	 	//ʪ�ȵ�С������
		uint8_t  temp_int;	 	//�¶ȵ���������
		uint8_t  temp_deci;	 	//�¶ȵ�С������
		uint8_t  check_sum;	 	//У���
	}DHT11_Data2_TypeDef;
	
	static DHT11_Data2_TypeDef DHT11_Data2;
	
  while (1)
  {
		xReturn = xQueueReceive(xQueueDHT11_Data_Handle,&DHT11_Data2,portMAX_DELAY);
		
		if(pdPASS == xReturn)
		{
			LCD_ClearLine(LINE(2));
			LCD_ClearLine(LINE(3));
			LCD_ClearLine(LINE(4));
			LCD_ClearLine(LINE(5));
		
			sprintf((char*)dis_buf,"humidity: %2d.%2d %%RH ",DHT11_Data2.humi_int,DHT11_Data2.humi_deci);
			LCD_DisplayStringLine_EN_CH(LINE(4),dis_buf);
			
			sprintf((char*)dis_buf,"temperature: %2d.%2d degree Celsius ",DHT11_Data2.temp_int,DHT11_Data2.temp_deci);
			LCD_DisplayStringLine_EN_CH(LINE(5),dis_buf);
			printf("LCD ��ʾ��ʪ��д�����\r\n\r\n");
		}
		else
		{
			LCD_ClearLine(LINE(4));
			LCD_ClearLine(LINE(5));
			LCD_SetTextColor(LCD_COLOR_RED);
			LCD_DisplayStringLine_EN_CH(LINE(2),(uint8_t* )"DHT11 initialization failed!");
			LCD_DisplayStringLine_EN_CH(LINE(3),(uint8_t* )"Please check the connection!");	
			printf("LCD ��ʾ��ʪ�ȴ�����������Ϣд�����\r\n\r\n");			
		}
	
		vTaskDelay(2000);   /* ��ʱ2000��tick */
  }
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
	
	LCD_DisplayStringLine_EN_CH(LINE(1),(uint8_t* )"DHT11 temperature & humidity detect demo");
	
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
	
	/* ESP8266-01s��ʼ�� */
	Wifi_Init();
	printf("ESP8266_Config		OK!\r\n");
	
	printf("ALL_Config		READY!\r\n\r\n\r\n");
	
}

/*********************************************END OF FILE**********************/

