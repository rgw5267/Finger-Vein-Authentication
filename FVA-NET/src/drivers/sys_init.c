#include <stm32f10x.h>
#include "sys_init.h"

void TIM2_Configuration(u16 TIM_Period, FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
		TIM_DeInit(TIM2);
		TIM_TimeBaseStructure.TIM_Period = TIM_Period;		 			/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
																	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
		TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* ʱ��Ԥ��Ƶ��   ���磺ʱ��Ƶ��=72MHZ/(ʱ��Ԥ��Ƶ+1) */
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* ������Ƶ */
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* ���ϼ���ģʽ */
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* �������жϱ�־ */
		TIM_ITConfig(TIM2,TIM_IT_Update,NewState);
		TIM_Cmd(TIM2, NewState);											/* ����ʱ�� */

	}
 	else
    {
		TIM_ITConfig(TIM2,TIM_IT_Update,NewState);
		TIM_Cmd(TIM2, NewState);											/* ����ʱ�� */	
	}
}

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	                                                            
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   //ʹ�ܻ���ʧ��ָ���� IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //�����˳�Ա NVIC_IRQChannel�е���ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;           //�����˳�Ա NVIC_IRQChannel�еĴ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;   //ʹ�ܻ���ʧ��ָ���� IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //�����˳�Ա NVIC_IRQChannel�е���ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;           //�����˳�Ա NVIC_IRQChannel�еĴ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;   //ʹ�ܻ���ʧ��ָ���� IRQͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //�����˳�Ա NVIC_IRQChannel�е���ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;           //�����˳�Ա NVIC_IRQChannel�еĴ����ȼ�
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
