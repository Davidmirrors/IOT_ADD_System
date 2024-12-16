/***********
//@Davidmirrors
//7/10/2023
//20/3/2024
//主函数main
//功能：
// 1.
//GB2312
//.....
************/
/**
  ******************************************************************************
  * 开发板:野火 STM32 F429 开发板
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
						函数声明
*************************************/

static void AppTaskCreate(void);/* 用于创建任务 */

static void LED_Task(void* pvParameters);/* LED_Task任务实现 */
static void KEY_Task(void* pvParameters);/* KEY_Task任务实现 */
static void READ_DHT11_Task(void* parameter);/* READ_DHT11_Task任务实现 */
static void LCD_Task(void* pvParameters);/* LCD_Task任务实现 */

static void HARDWARE_Init(void);					//硬件设备初始化函数声明
static void LCD_RunTest(void);						//LCD初始化背光兼测试

/****************************************
								任务句柄
****************************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* 创建任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;/* LED任务句柄 */
static TaskHandle_t KEY_Task_Handle = NULL;/* KEY任务句柄 */
static TaskHandle_t READ_DHT11_Task_Handle = NULL;/* DHT11任务句柄 */
static TaskHandle_t LCD_Task_Handle = NULL;/* LCD任务句柄 */

static QueueHandle_t xQueueDHT11_Data_Handle;

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	
	BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
	
	HARDWARE_Init();
	LCD_RunTest();
	
	xQueueDHT11_Data_Handle = xQueueCreate(4,sizeof(int));
	if(xQueueDHT11_Data_Handle == NULL)
	{
		printf("Can't create DHT11_Data");
	}
	/* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
	
	printf("EXIT ERROR!");
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
//	Wifi_Init();
//	printf("ESP8266_Config		OK!\r\n");
//  /* 创建LED_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* 任务入口函数 */
//                        (const char*    )"LED_Task",/* 任务名字 */
//                        (uint16_t       )512,   /* 任务栈大小 */
//                        (void*          )NULL,	/* 任务入口函数参数 */
//                        (UBaseType_t    )2,	    /* 任务的优先级 */
//                        (TaskHandle_t*  )&LED_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建LED_Task任务成功!\r\n");
//  /* 创建KEY_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )KEY_Task,  /* 任务入口函数 */
//                        (const char*    )"KEY_Task",/* 任务名字 */
//                        (uint16_t       )512,  /* 任务栈大小 */
//                        (void*          )NULL,/* 任务入口函数参数 */
//                        (UBaseType_t    )3, /* 任务的优先级 */
//                        (TaskHandle_t*  )&KEY_Task_Handle);/* 任务控制块指针 */ 
//  if(pdPASS == xReturn)
//    printf("创建KEY_Task任务成功!\r\n");
		
	/* 创建READ_DHT11_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )READ_DHT11_Task,  /* 任务入口函数 */
//                        (const char*    )"READ_DHT11_Task",/* 任务名字 */
//                        (uint16_t       )512,  /* 任务栈大小 */
//                        (void*          )NULL,/* 任务入口函数参数 */
//                        (UBaseType_t    )4, /* 任务的优先级 */
//                        (TaskHandle_t*  )&READ_DHT11_Task_Handle);/* 任务控制块指针 */ 
//  if(pdPASS == xReturn)
//    printf("创建READ_DHT11_Task任务成功!\r\n");
//	
//	/* 创建LCD_Task任务 */
//  xReturn = xTaskCreate((TaskFunction_t )LCD_Task, /* 任务入口函数 */
//                        (const char*    )"LCD_Task",/* 任务名字 */
//                        (uint16_t       )512,   /* 任务栈大小 */
//                        (void*          )NULL,	/* 任务入口函数参数 */
//                        (UBaseType_t    )1,	    /* 任务的优先级 */
//                        (TaskHandle_t*  )&LCD_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建LCD_Task任务成功!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}

/**********************************************************************
  * @ 函数名  ： READ_DHT11_Task
  * @ 功能说明： READ_DHT11_Task任务主体读取DHT11
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void READ_DHT11_Task(void* parameter)
{
	static DHT11_Data_TypeDef DHT11_Data;
//	static uint8_t dis_buf[1024];
	 while(1)
	{
		/*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/
		if( Read_DHT11 ( & DHT11_Data ) == SUCCESS)
		{
			printf("\r\n读取DHT11成功!\r\n\r\n湿度为%d.%d %%RH ，温度为 %d.%d℃ \r\n",\
			DHT11_Data.humi_int,DHT11_Data.humi_deci,DHT11_Data.temp_int,DHT11_Data.temp_deci);
      
			xQueueSend(xQueueDHT11_Data_Handle,&DHT11_Data,portMAX_DELAY);

		}		
		else
		{
		  printf("\r\nRead DHT11 ERROR!\r\n\r\n");
		}
    vTaskDelay(2000);   /* 延时2000个tick */
	} 
}

/**********************************************************************
  * @ 函数名  ： LCD_Task
  * @ 功能说明： LCD_Task任务主体LCD显示
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LCD_Task(void* parameter)
{	
	BaseType_t xReturn = pdTRUE;
	
	static uint8_t dis_buf[1024];
	typedef struct
	{
		uint8_t  humi_int;		//湿度的整数部分
		uint8_t  humi_deci;	 	//湿度的小数部分
		uint8_t  temp_int;	 	//温度的整数部分
		uint8_t  temp_deci;	 	//温度的小数部分
		uint8_t  check_sum;	 	//校验和
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
			printf("LCD 显示温湿度写入完成\r\n\r\n");
		}
		else
		{
			LCD_ClearLine(LINE(4));
			LCD_ClearLine(LINE(5));
			LCD_SetTextColor(LCD_COLOR_RED);
			LCD_DisplayStringLine_EN_CH(LINE(2),(uint8_t* )"DHT11 initialization failed!");
			LCD_DisplayStringLine_EN_CH(LINE(3),(uint8_t* )"Please check the connection!");	
			printf("LCD 显示温湿度传感器错误信息写入完成\r\n\r\n");			
		}
	
		vTaskDelay(2000);   /* 延时2000个tick */
  }
}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LED_Task(void* parameter)
{	
  while (1)
  {
    LED1_ON;
    printf("LED_Task Running,LED1_ON\r\n");
    vTaskDelay(500);   /* 延时500个tick */
    
    LED1_OFF;     
    printf("LED_Task Running,LED1_OFF\r\n");
    vTaskDelay(500);   /* 延时500个tick */
  }
}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void KEY_Task(void* parameter)
{	
  while (1)
  {
    if( Key_Scan(KEY1_GPIO_PORT,KEY1_PIN) == KEY_ON )
    {/* K1 被按下 */
//      printf("挂起LED任务！\n");
//      vTaskSuspend(LED_Task_Handle);/* 挂起LED任务 */
//      printf("挂起LED任务成功！\n");
    } 
    if( Key_Scan(KEY2_GPIO_PORT,KEY2_PIN) == KEY_ON )
    {/* K2 被按下 */
//      printf("恢复LED任务！\n");
//      vTaskResume(LED_Task_Handle);/* 恢复LED任务！ */
//      printf("恢复LED任务成功！\n");
    }
    vTaskDelay(20);/* 延时20个tick */
  }
}

/************************
  * @ 函数名  ： LCD_RunTest
  * @ 功能说明： LCD_RunTest测试函数
  * @ 参数    ：   
  * @ 返回值  ： 无
*************************/
static void LCD_RunTest(void)
{
	/*把背景层刷黑色*/
  LCD_SetLayer(LCD_BACKGROUND_LAYER);  
	LCD_Clear(LCD_COLOR_BLACK);
	
  /*初始化后默认使用前景层*/
	LCD_SetLayer(LCD_FOREGROUND_LAYER); 
	/*默认设置不透明	，该函数参数为不透明度，范围 0-0xff ，0为全透明，0xff为不透明*/
  LCD_SetTransparency(0xFF);
  LCD_SetColors(LCD_COLOR_BLUE,LCD_COLOR_BLACK);

	LCD_Clear(LCD_COLOR_BLACK); 

	/*经过LCD_SetLayer(LCD_FOREGROUND_LAYER)函数后，
	以下液晶操作都在前景层刷新，除非重新调用过LCD_SetLayer函数设置背景层*/
	
	LCD_DisplayStringLine_EN_CH(LINE(1),(uint8_t* )"DHT11 temperature & humidity detect demo");
	
}

/***************************
  * @ 函数名  ： HARDWARE_Init
  * @ 功能说明： HARDWARE_Init硬件设备初始化
  * @ 参数    ：   
  * @ 返回值  ： 无
***************************/
static void HARDWARE_Init(void)				//硬件设备初始化函数放在这里
{
/*
* STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
* 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
* 都统一用这个优先级分组，千万不要再分组，切忌。
*/
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* 串口初始化	*/
	Debug_USART_Config();
  printf("USART_Config		OK!\r\n");
	Debug_USART3_Config();
  printf("USART3_Config		OK!\r\n");
	
	/* LED 初始化 */
	LED_GPIO_Config();
	printf("LED_Config		OK!\r\n");

  /* 按键初始化	*/
  Key_GPIO_Config();
	printf("KEY_Config		OK!\r\n");

	/* 初始化系统定时器 */
	SysTick_Init(); 
	printf("SYSTICK_Config		OK!\r\n");

	/*初始化液晶屏*/
  LCD_Init();
  LCD_LayerInit();
  LTDC_Cmd(ENABLE);
	printf("LCD_Config		OK!\r\n");
		
	/* DHT11初始化 */
	DHT11_GPIO_Config();
	printf("DHT11_Config		OK!\r\n");
	
	/* ESP8266-01s初始化 */
	Wifi_Init();
	printf("ESP8266_Config		OK!\r\n");
	
	printf("ALL_Config		READY!\r\n\r\n\r\n");
	
}

/*********************************************END OF FILE**********************/

