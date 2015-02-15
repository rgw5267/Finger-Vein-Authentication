#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include <string.h>

extern void USART1_Configuration(void);
extern void USART2_Configuration(void);
extern void USART3_Configuration(void);

extern void USART1_SendByte(u16 dat);
extern void USART1Write(u8* data,u16 len);

extern void USART2_SendByte(u16 dat);
extern void USART2Write(u8* data,u16 len);

extern void USART3_SendByte(u16 dat);
extern void USART3Write(u8* data,u16 len);


#define DIR485_H  GPIOC->BSRR=1<<1
#define DIR485_L  GPIOC->BRR=1<<1

#endif
