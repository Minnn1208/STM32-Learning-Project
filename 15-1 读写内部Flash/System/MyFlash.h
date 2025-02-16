#ifndef __MYFLASH_H
#define __MYFLASH_H

uint32_t MyFlash_ReadWorld(uint32_t Address);
uint16_t MyFlash_ReadHalfWorld(uint32_t Address);
uint8_t MyFlash_ReadByte(uint32_t Address);
void MyFlash_EraseAllPages(void);
void MyFlash_ErasePage(uint32_t Address);
void MyFlash_ProgramWorld(uint32_t Address, uint32_t Data);
void MyFlash_ProgramHalfWorld(uint32_t Address, uint16_t Data);


#endif
