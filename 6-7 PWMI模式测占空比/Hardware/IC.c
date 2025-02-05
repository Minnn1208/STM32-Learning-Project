#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	// 开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// 开启GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 配置时钟源
	TIM_InternalClockConfig(TIM3);
	
	// 初始化时钟
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
	TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimBaseInitStructure.TIM_Period = 65536 - 1;			// ARR
	TIM_TimBaseInitStructure.TIM_Prescaler = 72 - 1; 			// PSC
	TIM_TimBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseInitStructure);
	
	// 初始化输入捕获
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 直连通道
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_PWMIConfig(TIM3, &TIM_ICInitStructure); // 和下面的注释代码块效果相等
	
	/*
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; // 通道2
	TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 上升沿捕获
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; // 分频
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_IndirectTI; // 交叉通道
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	*/
	
	// 配置触发源
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	//配置从模式
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	
	TIM_Cmd(TIM3, ENABLE);
}

uint32_t IC_GetFreq(void)
{
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

uint32_t IC_GetDuty(void)
{
	return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
