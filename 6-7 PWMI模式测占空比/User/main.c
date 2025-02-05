#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"


int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	PWM_Init();
	IC_Init();
	
	OLED_ShowString(1, 1, "Freq:00000HZ");
	OLED_ShowString(2, 1, "Duty:000%");
	
	PWM_SetPrescaler(720 - 1);
	PWM_SetCompare1(50);

	while(1)
	{
		OLED_ShowNum(1, 6, IC_GetFreq(), 5);
		OLED_ShowNum(1, 7, IC_GetDuty(), 3);
	}

}
