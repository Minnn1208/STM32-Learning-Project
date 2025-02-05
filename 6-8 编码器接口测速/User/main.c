#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Encoder.h"

//uint16_t Num = 0;
int32_t Speed;

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "CNT:");
	OLED_ShowString(1, 1, "Speed:");
	
	while(1)
	{
		// OLED_ShowSignedNum(1, 5, Encoder_Get(), 5);
		// OLED_ShowSignedNum(1, 5, Encoder_GetSpeed(), 5);
		// Delay_ms(1000);
		OLED_ShowSignedNum(1, 7, Speed, 5);
		
	}

}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encoder_GetSpeed();
		// 清除中断标志位
		TIM_ClearFlag(TIM2, TIM_IT_Update);
	}
}
