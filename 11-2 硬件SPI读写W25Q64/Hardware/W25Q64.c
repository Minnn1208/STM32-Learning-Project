#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

//初始化
void W25Q64_Init(void)
{
	MySPI_Init();
}

//获取设备ID
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	// 起始信号
	MySPI_Start();
	// 发送指令
	MySPI_SwapByte(W25Q64_JEDEC_ID);
	
	// 交换数据
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	*DID <<= 8;
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	
	// 停止信号
	MySPI_Stop();
}

// 写使能
void W25Q64_WriteEnable(void)
{
	MySPI_Start();
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);
	MySPI_Stop();
}


// 等待忙状态
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout = 100000;
	
	// 起始信号
	MySPI_Start();
	// 发送指令
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);
	while((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)
	{
		Timeout --;
		if(Timeout <= 0)
			break;
	}
	
	// 结束信号
	MySPI_Stop();
}

// 页编程，Count不能超过256，uint8_t最大是255 所以不使用
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArry, uint16_t Count)
{
	uint16_t i;
	
	if(Count > 256)
	{
		// 串口输出报错或者输出错误日志
		return;
	}
	
	W25Q64_WriteEnable();
	
	// 起始信号
	MySPI_Start();
	// 发送指令
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);
	
	// 发数据给寄存器指定地址
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	for(i = 0; i < Count; i ++)
	{
		// 发送数据
		MySPI_SwapByte(DataArry[i]);
	}
	// 结束信号
	MySPI_Stop();
	
	W25Q64_WaitBusy();

}

// 扇区擦除
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();
	
	// 起始信号
	MySPI_Start();
	
	// 发送指令
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);
	// 发数据给寄存器指定地址
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	
	// 结束信号
	MySPI_Stop();
	W25Q64_WaitBusy();
}

// 读数据
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArry, uint32_t Count)
{
	uint32_t i;
	
	// 起始信号
	MySPI_Start();
	
	// 发送指令
	MySPI_SwapByte(W25Q64_READ_DATA);
	
	// 发数据给寄存器指定地址
	MySPI_SwapByte(Address >> 16);
	MySPI_SwapByte(Address >> 8);
	MySPI_SwapByte(Address);
	for(i = 0; i < Count; i ++)
	{
		DataArry[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();
	
}


