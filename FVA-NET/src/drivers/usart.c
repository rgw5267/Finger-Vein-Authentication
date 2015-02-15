#include "stm32f10x.h"
/*******************************************************************************
** 函数名称:  
** 功能描述:  
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART1_SendByte(u16 Data)
{ 
   while (!(USART1->SR & USART_FLAG_TXE));
   USART1->DR = (Data & (uint16_t)0x01FF);	 
}
/*******************************************************************************
** 函数名称:  
** 功能描述:  
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART1Write(u8* data,u16 len)
{
	u16 i;
	for (i=0; i<len; i++){
		USART1_SendByte(data[i]);
	}	
}

/*******************************************************************************
** 函数名称:  
** 功能描述:  
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART2_SendByte(u16 Data)
{ 
   while (!(USART2->SR & USART_FLAG_TXE));
   USART2->DR = (Data & (uint16_t)0x01FF);	 
}
/*******************************************************************************
** 函数名称:  
** 功能描述:  
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART2Write(u8* data,u16 len)
{
	u16 i;
	for (i=0; i<len; i++){
		USART2_SendByte(data[i]);
	}
}
/*******************************************************************************
** 函数名称:  
** 功能描述:  
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART3_SendByte(u16 Data)
{
   while (!(USART3->SR & USART_FLAG_TXE));
   USART3->DR = (Data & (uint16_t)0x01FF);	 
}
/*******************************************************************************
** 函数名称:  
** 功能描述:  
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART3Write(u8* data,u16 len)
{
	u16 i;
	for (i=0; i<len; i++){
		USART3_SendByte(data[i]);
	}		
}
/*******************************************************************************
** 函数名称:  
** 功能描述:  H2E串口
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART1_Configuration(void)
{ 
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 

  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 57600;
  USART_InitStructure.USART_WordLength = USART_WordLength_9b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_Odd;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure); 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
//  USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  USART_Cmd(USART1, ENABLE);
}
/*******************************************************************************
** 函数名称:  
** 功能描述:  POE串口
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART2_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE);
    /*
    *  USART2_TX -> PA2 , USART2_RX ->	PA3
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_9b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Odd;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);
//    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}
/*******************************************************************************
** 函数名称:  
** 功能描述:  Debug串口
** 输入参数:  
** 输出参数:  
** 返回值  :  
** 修改记录:  
*******************************************************************************/
void USART3_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB ,ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3, ENABLE);
    /*
    *  USART3_TX -> PB10 , USART3_RX ->	PB11
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);
//    USART_ITConfig(USART3, USART_IT_TXE, ENABLE);
    USART_Cmd(USART3, ENABLE);
}
