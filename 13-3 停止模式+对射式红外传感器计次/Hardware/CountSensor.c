#include "stm32f10x.h"                  // Device header

uint16_t CounterSensor_Count = 0;
/*
* 对射式红外传感器的D0引脚接入PB14
*/
void CountSensor_Init(void)
{
	/*
	* 查阅手册可得，PB14引脚在APB2的GPIOB，所以开启APB2的时钟并初始化PB14
	* EXTI和NVIC不需要开启时钟
	*/ 
	// 使能APB2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// 使能对应的GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// 通过AFIO接通中断设置
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	// EXTI 中断初始化
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_StructInit(&EXTI_InitStruct);
	EXTI_InitStruct.EXTI_Line = EXTI_Line14;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
}

uint16_t Get_CounterSensor_Count(void)
{
	return CounterSensor_Count;
}

void EXTI15_10_IRQHandler(void)
{
	// 判断是否是从我们想要的中断通道进入
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		// 防止数据乱跳
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14))
			CounterSensor_Count ++;
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}

