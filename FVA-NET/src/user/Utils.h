#ifndef __UTILS_H__
#define __UTILS_H__

#include "usart.h"


void CheckFrame(void);
u8 ReadTouchOut(void);

extern void TimeOut(u16 TIM_Period, FunctionalState NewState);

extern void Delay(u32 n);

#endif
