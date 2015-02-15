#include <stm32f10x.h>

void led_red(u8 on)
{
    if(on == 1)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_6);
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
    }
    else if(on ==0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        GPIO_SetBits(GPIOA, GPIO_Pin_6);
    }
    
}

void led_green(u8 on)
{
    if(on == 1)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        GPIO_SetBits(GPIOA, GPIO_Pin_6);
    }
    else if(on == 0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        GPIO_SetBits(GPIOA, GPIO_Pin_6);
    }
}

void led_yellow(u8 on)
{
    if(on == 1)
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        GPIO_ResetBits(GPIOA, GPIO_Pin_6);
    }
    else if(on == 0)
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        GPIO_SetBits(GPIOA, GPIO_Pin_6);
    }
}
