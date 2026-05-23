#include "MPU6050.h"
#include "i2c.h"
#include "FreeRTOS.h"
#include "task.h"

/* 在指定MPU6050地址的指定寄存器地址写入数据Data，该API可以传入一个数组来连续传递两个值而不扰乱时序 只需设置SIZE的值即可 */
void MPU6050_WriteReg(uint8_t Address,uint8_t Data){
	uint8_t Adr_Data[2];
	Adr_Data[0]=Address;
	Adr_Data[1]=Data;
HAL_I2C_Master_Transmit(&hi2c2,MPU6050_AD0D_SLAVER_ARRD<<1,Adr_Data,2,1000);
}

/* 在指定MPU6050地址的指定寄存器地址读取值 具体流程为:1.先写入指定地址 2.再直接读 不用再指定具体寄存器地址了 只要这个两个API连着就行 */
uint8_t MPU6050_ReadByte(uint8_t Address){
	uint8_t Data;
	if(HAL_I2C_Master_Transmit(&hi2c2,MPU6050_AD0D_SLAVER_ARRD<<1,&Address,1,1000)!=HAL_OK){
	return 0;
	}
	if(HAL_I2C_Master_Receive(&hi2c2,MPU6050_AD0D_SLAVER_ARRD<<1,&Data,1,1000)!=HAL_OK){
	return 0;
	}
	else{
	return Data;
	}
}

/* 在指定MPU6050地址的指定寄存器读取一个Buffer,与上一个注释原理大差不差 */
HAL_StatusTypeDef MPU6050_ReadBuffer(uint8_t Address,uint8_t *Buffer,uint8_t Size){
	if(HAL_I2C_Master_Transmit(&hi2c2,MPU6050_AD0D_SLAVER_ARRD<<1,&Address,1,1000)!=HAL_OK){
	return HAL_ERROR;
	}
	if(HAL_I2C_Master_Receive(&hi2c2,MPU6050_AD0D_SLAVER_ARRD<<1,Buffer,Size,1000)!=HAL_OK){
	return HAL_ERROR;
	}
	else 
		return HAL_OK;		
}

uint8_t MPU6050_GetID(void){
return MPU6050_ReadByte(MPU6050_WHO_AM_I<<1);
}


