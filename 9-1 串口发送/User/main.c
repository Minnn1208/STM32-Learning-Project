#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	Serial_Init();
	
	OLED_ShowString(1, 1, "Hello World."); // 展示字符串
	// Serial_SendByte(0x41);
	
	// uint8_t My_Array[] = {0x42, 0x43, 0x44, 0x45};
	// Serial_SendArray(My_Array, 4);
	// Serial_SendString("\r\nHelloWorld!\r\n");
	// Serial_SendNumber(123456, 6);
	
	// printf("Num=%d\r\n", 666);
	// char String[100];
	// sprintf(String, "Num=%d\r\n", 666);
	// Serial_SendString(String);
	
	// Serial_Printf("Num=%d\r\n", 666);
	Serial_Printf("李智炫是我儿子");
	while(1)
	{
		
	}

}
