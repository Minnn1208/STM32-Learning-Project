#include "stm32f10x.h"                  // Device header
#include "MyFlash.h"
#define STORE_START_ADDRESS 0x08000C00
#define STORE_COUNT 512

uint16_t Store_Data[512];

void Store_Init(void)
{
	if(MyFlash_ReadHalfWorld(STORE_START_ADDRESS) != 0xA5A5)
	{
		MyFlash_ErasePage(STORE_START_ADDRESS);
		MyFlash_ProgramHalfWorld(STORE_START_ADDRESS, 0xA5A5);
		
		for(uint16_t i = 1; i < STORE_COUNT; i ++)
		{
			MyFlash_ProgramHalfWorld(STORE_START_ADDRESS + i * 2, 0x0000);
		}
	}
	
	for(uint16_t i = 0; i < STORE_COUNT; i ++)
	{
		Store_Data[i] = MyFlash_ReadHalfWorld(STORE_START_ADDRESS + i * 2);
	}
}

void Store_Save(void)
{
	MyFlash_ErasePage(STORE_START_ADDRESS);
	
	for(uint16_t i = 0; i < STORE_COUNT; i ++)
	{
		MyFlash_ProgramHalfWorld(STORE_START_ADDRESS + i * 2, Store_Data[i]);
	}
}

void Store_Clear(void)
{
	for(uint16_t i = 0; i < STORE_COUNT; i ++)
	{
		Store_Data[i] = 0;
	}
	
	Store_Save();
}



