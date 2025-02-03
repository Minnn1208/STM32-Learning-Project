#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "CountSensor.h"


int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	/*CountSensor初始化*/
	CountSensor_Init();  
	
	OLED_ShowString(1, 1, "There are ");
	OLED_ShowString(1, 13, "LZXs");
	
	while(1)
	{
		OLED_ShowNum(1, 11, Get_CounterSensor_Count(), 1);
		
	}

}
