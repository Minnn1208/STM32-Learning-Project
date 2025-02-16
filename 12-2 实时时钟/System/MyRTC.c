#include "stm32f10x.h"                  // Device header
#include <time.h>

uint16_t MyRTC_Time[] = {2023, 1, 1, 23, 59, 55};

void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_data;
	
	time_data.tm_year = MyRTC_Time[0] - 1900;
	time_data.tm_mon = MyRTC_Time[1] - 1;
	time_data.tm_mday = MyRTC_Time[2];
	time_data.tm_hour = MyRTC_Time[3];
	time_data.tm_min = MyRTC_Time[4];
	time_data.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_data) - 8 * 60 * 60;
	
	RTC_SetCounter(time_cnt);
	RTC_WaitForLastTask();
}

void MyRCT_ReadTime(void)
{
	time_t time_cnt;
	struct tm *time_data;
	
	time_cnt = RTC_GetCounter() + 8 * 60 * 60;
	time_data = localtime(&time_cnt);
	
	MyRTC_Time[0] = time_data->tm_year + 1900;
	MyRTC_Time[1] = time_data->tm_mon + 1;
	MyRTC_Time[2] = time_data->tm_mday;
	MyRTC_Time[3] = time_data->tm_hour;
	MyRTC_Time[4] = time_data->tm_min;
	MyRTC_Time[5] = time_data->tm_sec;
}


void MyRTC_Init(void)
{
	/*初始化BKP*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP, ENABLE);
	/*PWR使能BKP*/
	PWR_BackupAccessCmd(ENABLE);
	// RCC_LSICmd(ENABLE);
	
	if(BKP_ReadBackupRegister(BKP_DR1 != 0xA5A5))
	{
		/*配置并使能RCC时钟*/
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != SET);
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
		
		// while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != SET);
		// RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
		// RCC_RTCCLKCmd(ENABLE);
		
		/*进行时钟同步*/ 
		RTC_WaitForSynchro();
		RTC_WaitForLastTask(); // 隔开写入操作，写入关于RTC的东西最好都带一行代码
		
		
		RTC_SetPrescaler(40000  - 1);
		RTC_WaitForLastTask();
		
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
	}
	else
	{
		RTC_WaitForSynchro();
		RTC_WaitForLastTask();
	}
	
	
}

