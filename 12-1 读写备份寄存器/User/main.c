#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"

uint16_t ArraryWrite[] = {0x1234, 0x5678};
uint16_t ArraryRead[2] = {0x00};
uint8_t KeyNum = 0;

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	Key_Init();

	OLED_ShowString(1, 1, "W:");
	OLED_ShowString(2, 1, "R:");
	
	// BKP初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	
	while(1)
	{
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1)
		{
			ArraryWrite[0] ++;
			ArraryWrite[1] ++;
			
			// 写入BKP，需要指定寄存器和需要写入的数据
			BKP_WriteBackupRegister(BKP_DR1, ArraryWrite[0]);
			BKP_WriteBackupRegister(BKP_DR2, ArraryWrite[1]);
			
			OLED_ShowHexNum(1, 3, ArraryWrite[0], 4);
			OLED_ShowHexNum(1, 8, ArraryWrite[1], 4);
		}
		// 读取BKP，需要指定读取的寄存器
		ArraryRead[0] = BKP_ReadBackupRegister(BKP_DR1);
		ArraryRead[1] = BKP_ReadBackupRegister(BKP_DR2);
		
		OLED_ShowHexNum(2, 3, ArraryRead[0], 4);
		OLED_ShowHexNum(2, 8, ArraryRead[1], 4);
	}

}
