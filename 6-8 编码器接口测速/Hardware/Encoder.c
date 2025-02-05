#include "stm32f10x.h"                  // Device header


void Encoder_Init(void)
{
	// 开启时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// 开启GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化GPIO
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 配置时钟源
	TIM_InternalClockConfig(TIM3);
	
	// 初始化时钟
	TIM_TimeBaseInitTypeDef TIM_TimBaseInitStructure;
	TIM_TimBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimBaseInitStructure.TIM_Period = 65536 - 1;			// ARR
	TIM_TimBaseInitStructure.TIM_Prescaler = 1 - 1; 			// PSC
	TIM_TimBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimBaseInitStructure);
	
	// 初始化输入捕获
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; // 通道2
	TIM_ICInitStructure.TIM_ICFilter = 0xF; // 滤波
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// 后面两个参数都会覆盖掉TIM_ICInitStructure.TIM_ICSelection，用这两个参数进行初始化也可以
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_Cmd(TIM3, ENABLE);
	
}

int16_t Encoder_Get(void)
{
	return TIM_GetCounter(TIM3);
}

int16_t Encoder_GetSpeed(void)
{
	int16_t Temp; 
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp;
}


