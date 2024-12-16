/*********************************************************************************************/
【*】程序简介
-开发板品牌：野火
-芯片: STM32 F429IGT6
-FreeRTOS版本：9.0.0
-ST固件库版本：2.5.0

原项目搭建的硬件平台已留校供学弟学妹参考，后续更新使用本人初学stm32所使用的野火f429开发板进行移植开发；
为节约开发时间，项目代码主要借鉴于野火f429开发板配套源码进行移植/增加/删减/修改相应的功能需求；
同时进行Free RTOS移植,并将原裸机工程的功能进行移植与拓展，有优化会持续更新。
本工程仅用于学习交流使用！
如有侵权请联系邮箱：david.mirror@foxmail.com
常用署名 Davidmirror 与 Davidmirrors

已实现功能:
	1.LED灯
	2.KEY按键
	3.EXTI中断
	4.DHT11
	5..USART1串口调试/USART3_ESP8266-01S串口AT通信（已启用连接服务器，暂未优化根据回参发送指令）
	6.LCD英文字库，FLASH_SPI
	7.LCD显存SDRAM_FMC
	8.LCD显示
	9.

暂未解决的bug:
	1.USART3接收由电脑串口助手发送的消息会发生乱码，但是接收其他单片机与WIFI模块的数据正常。
	  USART3暂时仅用于接收WIFI模块回参和透传数据，没有影响到主要功能，等有空再用逻辑分析仪抓虫。
	2.

/*********************************************************************************************/
【*】 引脚分配
LED:
	R_LED<--->PH10
	G_LED<--->PH11
	B_LED<--->PH12
	
	LED4<--->PD11
	
KEY:
	KEY1/KEY_UP<--->PA0
	KEY2<--->PC13

EXTI:
	EXTI0_IRQn_GPIOA_Line0<--->PA0
	EXTI15_10_IRQn_GPIOC_Line13<--->PC13

DHT11:
	DHT11<--->PE2
	
USART:
	USART1_RX<--->PA10
	USART1_TX<--->PA9
	
	USART3_RX<--->PB11
	USART3_TX<--->PB10
	
FLASH_SPI:
	SCK<--->PF7
	MISO<--->PF8
	MOSI<--->PF9
	CS<--->PF6
	
SDRAM_FMC:
	FMC_A0<--->PF0				FMC_D0<--->PD14				FMC_CS<--->PH6
	FMC_A1<--->PF1				FMC_D1<--->PD15				
	FMC_A2<--->PF2				FMC_D2<--->PD0				FMC_BA0<--->PG4
	FMC_A3<--->PF3				FMC_D3<--->PD1				FMC_BA1<--->PG5
	FMC_A4<--->PF4				FMC_D4<--->PE7				
	FMC_A5<--->PF5				FMC_D5<--->PE8				FMC_WE<--->PC0
	FMC_A6<--->PF12				FMC_D6<--->PE9				
	FMC_A7<--->PF13				FMC_D7<--->PE10				FMC_RAS<--->PF11
	FMC_A8<--->PF14				FMC_D8<--->PE11				FMC_CAS<--->PG15
	FMC_A9<--->PF15				FMC_D9<--->PE12				FMC_CLK<--->PG8
	FMC_A10<--->PG0				FMC_D10<--->PE13			FMC_CKE<--->PH7	
	FMC_A11<--->PG1				FMC_D11<--->PE14				
								FMC_D12<--->PE15			FMC_UDQM<--->PE1
								FMC_D13<--->PD8				FMC_LDQM<--->PE0
								FMC_D14<--->PD9				
								FMC_D15<--->PD10			
								
LCD:
	RGB888						RGB565
	LTDC_R0<--->PH2				LTDC_R0<--->PH2	
	LTDC_R1<--->PH3				LTDC_R1<--->PH3	
	LTDC_R2<--->PH8				LTDC_R2<--->PH8	
	LTDC_R3<--->PB0				LTDC_R3<--->PB0	
	LTDC_R4<--->PA11			LTDC_R4<--->PA11
	LTDC_R5<--->PA12			LTDC_R5<--->PA12
	LTDC_R6<--->PB1				LTDC_R6<--->PB1	
	LTDC_R7<--->PG6				LTDC_R7<--->PG6
	
	LTDC_G0<--->PE5				LTDC_G0<--->PE5	
	LTDC_G1<--->PE6				LTDC_G1<--->PE6	
	LTDC_G2<--->PH13			LTDC_G2<--->PH13
	LTDC_G3<--->PG10			LTDC_G3<--->PG10
	LTDC_G4<--->PH15			LTDC_G4<--->PH15
	LTDC_G5<--->PI0				LTDC_G5<--->PI0
	LTDC_G6<--->PC7				LTDC_G6<--->PC7
	LTDC_G7<--->PI2				LTDC_G7<--->PI2
	
	LTDC_B0<--->PE4				LTDC_B0<--->PE4
	LTDC_B1<--->PG12			LTDC_B1<--->PG12
	LTDC_B2<--->PD6				LTDC_B2<--->PD6
	LTDC_B3<--->PG11			LTDC_B3<--->PG11
	LTDC_B4<--->PI4				LTDC_B4<--->PI4
	LTDC_B5<--->PA3				LTDC_B5<--->PA3
	LTDC_B6<--->PB8				LTDC_B6<--->PB8
	LTDC_B7<--->PB9				LTDC_B7<--->PB9
	
	LTDC_CLK<--->PG7			LTDC_CLK<--->PG7
	LTDC_HSYNC<--->PI10			LTDC_HSYNC<--->PI10
	LTDC_VSYNC<--->PI9			LTDC_VSYNC<--->PI9
	LTDC_DE<--->PF10			LTDC_DE<--->PF10
	LTDC_DISP<--->PD4			LTDC_DISP<--->PD4
	LTDC_BL<--->PD7				LTDC_BL<--->PD7	
	

/*********************************************************************************************/

【*】 时钟

A.晶振：
-外部高速晶振：25MHz
-RTC晶振：32.768KHz

B.各总线运行时钟：
-系统时钟 = SYCCLK = AHB1 = 180MHz
-APB2 = 90MHz 
-APB1 = 45MHz

C.浮点运算单元：
  不使用
