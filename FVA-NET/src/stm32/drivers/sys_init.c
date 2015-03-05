#include <stm32f10x.h>
#include "sys_init.h"

void TIM2_Configuration(u16 TIM_Period, FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		TIM_DeInit(TIM2);
		TIM_TimeBaseStructure.TIM_Period = TIM_Period;		 			/* 自动重装载寄存器周期的值(计数值) */
																	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
		TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* 时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1) */
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* 采样分频 */
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* 向上计数模式 */
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
		TIM_ITConfig(TIM2,TIM_IT_Update,NewState);
		TIM_Cmd(TIM2, NewState);											/* 开启时钟 */

	}
 	else
    {
		TIM_ITConfig(TIM2,TIM_IT_Update,NewState);
		TIM_Cmd(TIM2, NewState);											/* 开启时钟 */	
	}
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	                                                            
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   //使能或者失能指定的 IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //设置了成员 NVIC_IRQChannel中的先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //设置了成员 NVIC_IRQChannel中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;   //使能或者失能指定的 IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //设置了成员 NVIC_IRQChannel中的先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //设置了成员 NVIC_IRQChannel中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;   //使能或者失能指定的 IRQ通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //设置了成员 NVIC_IRQChannel中的先占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;           //设置了成员 NVIC_IRQChannel中的从优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void sys_init(void)
{
	GPIO_Configuration();
	NVIC_Configuration();
	TIM2_Configuration(1000, ENABLE);
	USART1_Configuration();
	USART2_Configuration();
	USART3_Configuration();
}
