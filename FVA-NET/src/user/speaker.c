#include <stm32f10x.h>
#include "Utils.h"

extern vu32 mTimeOut;
void speaker(u16 loudtime_ms)
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
    GPIO_SetBits(GPIOA, GPIO_Pin_4);
    TimeOut(loudtime_ms, ENABLE);
    while(mTimeOut != 1);
    mTimeOut = 0;
    TimeOut(loudtime_ms, DISABLE);
    GPIO_ResetBits(GPIOA, GPIO_Pin_4);
}
