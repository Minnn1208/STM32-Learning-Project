#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

#define MPU6050_ADDRESS			0xD0

/*
void MPU6050_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout = 1000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if(Timeout <= 0)
		{
			break;
		}
	}
}
*/

uint8_t MPU6050_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
	uint32_t Timeout = 1000;
	while(I2C_CheckEvent(I2Cx, I2C_EVENT) != SUCCESS)
	{
		Timeout --;
		if(Timeout <= 0)
		{
			return 0;
		}
	}
	return 1;
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	
	
	// 生成起始条件
	I2C_GenerateSTART(I2C2, ENABLE);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != 1)
	{
		return;
	}
	
	// 查找从机，这里可以改成多加两个参数
	// 一个参数指定发7位地址还是10位地址
	// 另外一个参数指定外设
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != 1)
	{
		return;
	}
	
	
	// 指定地址
	I2C_SendData(I2C2, RegAddress);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTING) != 1)
	{
		return;
	}
	
	// 发送数据
	I2C_SendData(I2C2, Data);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != 1)
	{
		return;
	}
	
	// 生成停止位
	I2C_GenerateSTOP(I2C2, ENABLE);
}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	// 生成起始条件
	I2C_GenerateSTART(I2C2, ENABLE);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != 1)
	{
		return 0;
	}
	
	// 查找从机
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Transmitter);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != 1)
	{
		return 0;
	}
	
	// 指定地址
	I2C_SendData(I2C2, RegAddress);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED) != 1)
	{
		return 0;
	}
	
	// 生成起始条件
	I2C_GenerateSTART(I2C2, ENABLE);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT) != 1)
	{
		return 0;
	}
	
	// 指定地址
	I2C_Send7bitAddress(I2C2, MPU6050_ADDRESS, I2C_Direction_Receiver);
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != 1)
	{
		return 0;
	}
	
	// 最后一位数据前
	I2C_AcknowledgeConfig(I2C2, DISABLE);
	// 生成停止位
	I2C_GenerateSTOP(I2C2, ENABLE);
	
	// while(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != SUCCESS);
	if(MPU6050_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED) != 1)
	{
		return 0;
	}
	Data = I2C_ReceiveData(I2C2);
	
	// 重置应答
	I2C_AcknowledgeConfig(I2C2, ENABLE);
	
	return Data;
}

void MPU6050_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_Initstructure);
	
	I2C_InitTypeDef I2C_InitStructure;
	I2C_StructInit(&I2C_InitStructure);
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	I2C_Init(I2C2, &I2C_InitStructure);
	
	I2C_Cmd(I2C2, ENABLE);
	
	MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
	MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
	MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
	MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
	MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);
	MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);
}

uint8_t MPU6050_GetID(void)
{
	return MPU6050_ReadReg(MPU6050_WHO_AM_I);		//返回WHO_AM_I寄存器的值
}



void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
											int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
	uint8_t DataH, DataL;
	
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
	*AccX = (DataH) << 8 | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
	*AccY = (DataH) << 8 | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
	*AccZ = (DataH) << 8 | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
	*GyroX = (DataH) << 8 | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
	*GyroY = (DataH) << 8 | DataL;
	
	DataH = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
	DataL = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
	*GyroZ = (DataH) << 8 | DataL;
}

