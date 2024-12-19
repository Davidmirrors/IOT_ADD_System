#ifndef __BH1750_H_
#define __BH1750_H_	

#include "stm32f4xx.h" 


/**************************I2C参数定义，I2C1或I2C2********************************/
#define BH1750_I2C                         	I2C1
#define BH1750_I2C_CLK                      RCC_APB1Periph_I2C1
#define BH1750_I2C_CLK_INIT									RCC_APB1PeriphClockCmd

#define BH1750_I2C_SCL_PIN                  GPIO_Pin_6                 
#define BH1750_I2C_SCL_GPIO_PORT            GPIOB                       
#define BH1750_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define BH1750_I2C_SCL_SOURCE               GPIO_PinSource6
#define BH1750_I2C_SCL_AF                   GPIO_AF_I2C1

#define BH1750_I2C_SDA_PIN                  GPIO_Pin_7                  
#define BH1750_I2C_SDA_GPIO_PORT            GPIOB                       
#define BH1750_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define BH1750_I2C_SDA_SOURCE               GPIO_PinSource7
#define BH1750_I2C_SDA_AF                   GPIO_AF_I2C1

#define	BH1750_ADDRESS											0x46

/* STM32 I2C 快速模式 */
#define I2C_Speed              400000  //*

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2Cx_OWN_ADDRESS7      0X0A   

/* AT24C01/02每页有8个字节 */
#define I2C_PageSize           8

/* AT24C04/08A/16A每页有16个字节 */
//#define I2C_PageSize           16	

/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))


/*信息输出*/
#define BH1750_DEBUG_ON         0

#define BH1750_INFO(fmt,arg...)           printf("<<-BH1750-INFO->> "fmt"\n",##arg)
#define BH1750_ERROR(fmt,arg...)          printf("<<-BH1750-ERROR->> "fmt"\n",##arg)
#define BH1750_DEBUG(fmt,arg...)          do{\
                                          if(BH1750_DEBUG_ON)\
                                          printf("<<-BH1750-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
																		
void I2C_GPIO_Config(void);
void IIC_BH1750_Write(uint8_t command);
uint32_t IIC_BH1750_Read(void);

#endif
