#include "delay.h"
#include "sys.h"
//���ʹ��OS,����������ͷ�ļ�����ucosΪ��������.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//֧��OSʱ��ʹ��	  
#endif

// STM32F407������
//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���(֧��OS)
//����delay_us,delay_ms

static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��os��,����ÿ�����ĵ�ms��
	
#if SYSTEM_SUPPORT_OS							//���SYSTEM_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS).
//��delay_us/delay_ms��Ҫ֧��OS��ʱ����Ҫ������OS��صĺ궨��ͺ�����֧��
//������3���궨��:
//    delay_osrunning:���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
//delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
// delay_osintnesting:���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
//Ȼ����3������:
//  delay_osschedlock:��������OS�������,��ֹ����
//delay_osschedunlock:���ڽ���OS�������,���¿�������
//    delay_ostimedly:����OS��ʱ,���������������.

//�����̽���UCOSII��UCOSIII��֧��,����OS,�����вο�����ֲ
//֧��UCOSII
#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD������,˵��Ҫ֧��UCOSII				
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OS_TICKS_PER_SEC	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNesting		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif

//֧��UCOSIII
#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD������,˵��Ҫ֧��UCOSIII	
#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���
#endif


//us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
void delay_osschedlock(void)
{
#ifdef CPU_CFG_CRITICAL_METHOD   			//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedLock(&err);						//UCOSIII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#else										//����UCOSII
	OSSchedLock();							//UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ
#endif
}

//us����ʱʱ,�ָ��������
void delay_osschedunlock(void)
{	
#ifdef CPU_CFG_CRITICAL_METHOD   			//ʹ��UCOSIII
	OS_ERR err; 
	OSSchedUnlock(&err);					//UCOSIII�ķ�ʽ,�ָ�����
#else										//����UCOSII
	OSSchedUnlock();						//UCOSII�ķ�ʽ,�ָ�����
#endif
}

//����OS�Դ�����ʱ������ʱ
//ticks:��ʱ�Ľ�����
void delay_ostimedly(u32 ticks)
{
#ifdef CPU_CFG_CRITICAL_METHOD
	OS_ERR err; 
	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);//UCOSIII��ʱ��������ģʽ
#else
	OSTimeDly(ticks);						//UCOSII��ʱ
#endif 
}
 
//systick�жϷ�����,ʹ��OSʱ�õ�
void SysTick_Handler(void)
{	
	if(delay_osrunning==1)					//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();						//�����ж�
		OSTimeTick();       				//����ucos��ʱ�ӷ������               
		OSIntExit();       	 				//���������л����ж�
	}
}
#endif
			   
//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(u8 SYSCLK)
{
#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
	u32 reload;
#endif
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
	reload=SYSCLK/8;						//ÿ���ӵļ������� ��λΪM	   
	reload*=1000000/delay_ostickspersec;	//����delay_ostickspersec�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����	
	fac_ms=1000/delay_ostickspersec;		//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; 	//����SYSTICK    
#else
	fac_ms=(u16)fac_us*1000;				//��OS��,����ÿ��ms��Ҫ��systickʱ����   
#endif
}								    

#if SYSTEM_SUPPORT_OS 						//�����Ҫ֧��OS.
//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=21)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	delay_osschedlock();					//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};
	delay_osschedunlock();					//�ָ�OS����											    
}  
//��ʱnms
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(u16 nms)
{	
	if(delay_osrunning&&delay_osintnesting==0)//���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������)	    
	{		 
		if(nms>=fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			delay_ostimedly(nms/fac_ms);	//OS��ʱ
		}
		nms%=fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((u32)(nms*1000));				//��ͨ��ʽ��ʱ
}
#else  //����ucosʱ
//��ʱnus
//nusΪҪ��ʱ��us��.	
//ע��:nus��ֵ,��Ҫ����798915us(���ֵ��2^24/fac_us@fac_us=21)
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//ʱ�����	  		 
	SysTick->VAL=0x00;        				//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //�رռ�����
	SysTick->VAL =0X00;       				//��ռ����� 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��168M������,nms<=798ms 
void delay_xms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           			//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //��ʼ���� 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //�رռ�����
	SysTick->VAL =0X00;     		  		//��ռ�����	  	    
} 
//��ʱnms 
//nms:0~65535
void delay_ms(u16 nms)
{	 	 
	u8 repeat=nms/540;						//������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
											//���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
#endif

#if USE_DWT_DELAY


#define  DWT_CR      *(__IO uint32_t *)0xE0001000
#define  DWT_CYCCNT  *(__IO uint32_t *)0xE0001004
#define  DEM_CR      *(__IO uint32_t *)0xE000EDFC


#define  DEM_CR_TRCENA                   (1 << 24)
#define  DWT_CR_CYCCNTENA                (1 <<  0)


/**
  * @brief  ��ʼ��ʱ���
  * @param  ��
  * @retval ��
  * @note   ʹ����ʱ����ǰ��������ñ�����
  */
void CPU_TS_TmrInit(void)
{
    /* ʹ��DWT���� */
    DEM_CR |= (uint32_t)DEM_CR_TRCENA;                

    /* DWT CYCCNT�Ĵ���������0 */
    DWT_CYCCNT = (uint32_t)0u;

    /* ʹ��Cortex-M DWT CYCCNT�Ĵ��� */
    DWT_CR |= (uint32_t)DWT_CR_CYCCNTENA;
}

/**
  * @brief  ��ȡ��ǰʱ���
  * @param  ��
  * @retval ��ǰʱ�������DWT_CYCCNT�Ĵ�����ֵ
  */
uint32_t CPU_TS_TmrRd(void)
{        
  return ((uint32_t)DWT_CYCCNT);
}

///**
//  * @brief  ��ȡ��ǰʱ���
//  * @param  ��
//  * @retval ��ǰʱ�������DWT_CYCCNT�Ĵ�����ֵ
//	* 				�˴���HAL���滻HAL_GetTick����������os
//  */
//uint32_t HAL_GetTick(void)
//{        
//  return ((uint32_t)DWT_CYCCNT*1000/SysClockFreq);
//}

/**
  * @brief  ����CPU���ڲ�����ʵ�־�ȷ��ʱ��32λ������
  * @param  us : �ӳٳ��ȣ���λ1 us
  * @retval ��
  * @note   ʹ�ñ�����ǰ�����ȵ���CPU_TS_TmrInit����ʹ�ܼ�������
            ��ʹ�ܺ�CPU_TS_INIT_IN_DELAY_FUNCTION
            �����ʱֵΪ8�룬��8*1000*1000
  */
void CPU_TS_Tmr_Delay_US(__IO uint32_t us)
{
  uint32_t ticks;
  uint32_t told,tnow,tcnt=0;

  /* �ں����ڲ���ʼ��ʱ����Ĵ����� */  
#if (CPU_TS_INIT_IN_DELAY_FUNCTION)  
  /* ��ʼ��ʱ��������� */
  CPU_TS_TmrInit();
#endif
  
  ticks = us * (GET_CPU_ClkFreq() / 1000000);  /* ��Ҫ�Ľ����� */      
  tcnt = 0;
  told = (uint32_t)CPU_TS_TmrRd();         /* �ս���ʱ�ļ�����ֵ */

  while(1)
  {
    tnow = (uint32_t)CPU_TS_TmrRd();  
    if(tnow != told)
    { 
        /* 32λ�������ǵ��������� */    
      if(tnow > told)
      {
        tcnt += tnow - told;  
      }
      /* ����װ�� */
      else 
      {
        tcnt += UINT32_MAX - told + tnow; 
      } 
      
      told = tnow;

      /*ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳� */
      if(tcnt >= ticks)break;
    }  
  }
}

#endif
			 



































