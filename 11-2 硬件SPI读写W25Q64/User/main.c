#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "W25Q64.h"

uint8_t MID, i;
uint16_t DID;

uint8_t ArraryWrite[] = {0x01, 0x02, 0x03, 0x04};
uint8_t ArraryRead[4];

int main(void)
{
	/*OLED初始化*/
	OLED_Init();
	W25Q64_Init();
	
	W25Q64_ReadID(&MID, &DID);
	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	
	OLED_ShowHexNum(1, 5, MID, 2);
	OLED_ShowHexNum(1, 12, DID, 4);
	
	W25Q64_SectorErase(0x000000);
	W25Q64_PageProgram(0x000000, ArraryWrite, 4);
	W25Q64_ReadData(0x000000, ArraryRead, 4);
	
	for(i = 0; i < 4; i ++)
	{
		OLED_ShowHexNum(2, 3 + 3 * i, ArraryWrite[i], 2);
		OLED_ShowHexNum(3, 3 + 3 * i, ArraryRead[i], 2);
	}
	
	
	
	while(1)
	{
		
	}

}
