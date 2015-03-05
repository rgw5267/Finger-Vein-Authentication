#include "stm32f10x.h"
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configure GPIO Pin
* Input          : None
* Output         : None
* Return         : None
* Attention		 : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  EXTI_InitTypeDef EXTI_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB , ENABLE); 
    
      /**
      *  GPIOA
	  *  GPIO_Pin4 - speaker_driver
      *  GPIO_Pin5 - led_green
      *  GPIO_Pin6 - led_red
	  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; ; 
	  GPIO_Init(GPIOA, &GPIO_InitStructure);
					 
      /**
      *  GPIOB
	  *  GPIO_Pin12 - Touch_out2
      *  GPIO_Pin13 - pass_driver
      *  GPIO_Pin14 - Touch_out1
      *  GPIO_Pin15 - Busy
	  */
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; ; 
	  GPIO_Init(GPIOB, &GPIO_InitStructure);
      
      // Configure EXTI Line13 to generate an interrupt on falling edge


      EXTI_InitStructure.EXTI_Line = EXTI_Line13;
      EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
      EXTI_InitStructure.EXTI_LineCmd = ENABLE;
      EXTI_Init(&EXTI_InitStructure);
      GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
}
