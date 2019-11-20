#include "tim_driver.h"


/***********************************************************************
**函数功能：定时器2初始化配置，产生2ms中断一次的计时
**计	算：计数器的时钟频率CK_CNT = f(CK_PSC)/(PCS[15:0]+1)
**			计数器的计时频率CK_CNT = 48MHz/(4799+1) = 10KHz
**			单次计数时间 	T(CNT) = 1/CK_CNT = 100us
**			定时器溢出时间	Tout = ((CNT[15:0] + 1)*[PSC[15:0]+1])/Tclk
**			定时器溢出时间	Tout = ((9999 + 1)*(7199 + 1))/72MHz = 1s
**********************************************************************/
static void TIM3_Initconfig(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Prescaler = 4799;
	TIM_TimeBaseInitStruct.TIM_Period = 19;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

static void TIM_NVIC_Initconfig(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel= TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPriority = 3;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


void TIM_Init(void)
{
	TIM_NVIC_Initconfig();
	TIM3_Initconfig();
}


