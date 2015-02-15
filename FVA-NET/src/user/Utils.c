#include "stm32f10x.h"
#include "Utils.h"


vu32 mTimeOut;		/*超时时间，毫秒*/

u8 RxBuf[1100];		/*接收数据缓冲*/
u32 Rx1Flag;			/*接收数据完成标志*/
u32 Rx2Flag;			/*接收数据完成标志*/
u32 count; 		/*接收数据个数*/



u32	SensorID;

//PB12 Touch2
//PB14 Touch1
u8 ReadTouchOut()
{
    u8 ReadTouchout1Value, ReadTouchout2Value;
    ReadTouchout1Value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14);
    ReadTouchout2Value = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12);
    
    return (~(ReadTouchout1Value)&0x01) | ((~ReadTouchout2Value)&0x01) << 1;
}

void CheckFrame()
{
    u8 CmdReadTouchOut[4];
    
    if(Rx1Flag == 1)
    {
        Rx1Flag = 0;
        USART2Write(RxBuf, count);
        memset(RxBuf, 0, 1100);
        count = 0;
    }
    if(Rx2Flag == 1)
    {
        Rx2Flag = 0;
        
        if(RxBuf[0] == 0x31)
        {
            CmdReadTouchOut[0] = 0x00;
            CmdReadTouchOut[1] = 0x00;
            CmdReadTouchOut[2] = 0x01;
            CmdReadTouchOut[3] = ReadTouchOut();
            USART2Write(CmdReadTouchOut, 4);
        }
        else
        {
            USART1Write(RxBuf, count);
            memset(RxBuf, 0, 1100);
        }
        count = 0;
        
    }
}
/**
  * @brief 1毫秒分频  
  * @param  None
  * @retval None
  */
void TimeOut(u16 TIM_Period, FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
		TIM_DeInit(TIM3);
		TIM_TimeBaseStructure.TIM_Period = TIM_Period;		 			/* 自动重装载寄存器周期的值(计数值) */
																	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
		TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* 时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1) */
		TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 			/* 采样分频 */
		TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 		/* 向上计数模式 */
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
		TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
		TIM_ITConfig(TIM3,TIM_IT_Update,NewState);
		TIM_Cmd(TIM3, NewState);											/* 开启时钟 */

	}
 	else
	{
		TIM_ITConfig(TIM3,TIM_IT_Update,NewState);
		TIM_Cmd(TIM3, NewState);											/* 开启时钟 */	
	}
}
/**
  * @brief  异或校验
  * @param  帧，长度
  * @retval 校验真假
  */
u8 CheckXOR(u8 *u8Data, u8 Length)
{
	u8 CheckLen;
	u8 XORVal;
	u32 i;
	XORVal = 0;
	CheckLen = Length - 1;
	for(i = 0; i < CheckLen; i++)
	{
		XORVal = XORVal ^ u8Data[i];
	}
	return XORVal == u8Data[Length - 1] ? 1 : 0 ;
	
}

