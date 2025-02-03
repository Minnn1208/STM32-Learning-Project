#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	
	OLED_ShowString(1, 1, "LZX is my son."); // 展示字符串
	OLED_ShowChar(1, 16, 'Z'); // 展示字符
	OLED_ShowNum(2, 1, 12345, 5); // 展示数字
	OLED_ShowSignedNum(2, 6, -66, 2); // 展示有符号数字
	OLED_ShowHexNum(3, 1, 0xAA555, 4); // 展示16进制数
	OLED_ShowBinNum(4, 1, 0xaa55, 16); // 展示二进制数
	
	while(1)
	{
		
	}

}
