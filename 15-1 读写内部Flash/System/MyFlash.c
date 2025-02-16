#include "stm32f10x.h"                  // Device header


uint32_t MyFlash_ReadWorld(uint32_t Address)
{
	return *((__IO uint32_t*)(Address));
}


uint16_t MyFlash_ReadHalfWorld(uint32_t Address)
{
	return *((__IO uint16_t*)(Address));
}

uint8_t MyFlash_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t*)(Address));
}

void MyFlash_EraseAllPages(void)
{
	FLASH_Unlock();
	FLASH_EraseAllPages();
	FLASH_Lock();
}

void MyFlash_ErasePage(uint32_t Address)
{
	FLASH_Unlock();
	FLASH_ErasePage(Address);
	FLASH_Lock();
}

void MyFlash_ProgramWorld(uint32_t Address, uint32_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramWord(Address, Data);
	FLASH_Lock();
}

void MyFlash_ProgramHalfWorld(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();
	FLASH_ProgramHalfWord(Address, Data);
	FLASH_Lock();
}


