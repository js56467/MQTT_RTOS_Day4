#ifndef __MPU6050_h__
#define __MPU6050_h__

#define MPU6050_AD0D_SLAVER_ARRD   0x68					//MPU6050姿态传感器AD0下拉时的地址
#define MPU6050_AD0U_SLAVER_ARRD   0x69					//MPU6050姿态传感器AD0上拉时的地址
#include "main.h"

#define MPU6050_WHO_AM_I              0x75        // MPU6050设备ID寄存器地址
#define MPU6050_SMPLRT_DIV            0           // 采样率分频，默认8000Hz
#define MPU6050_DLPF_CFG              0           // 数字低通滤波配置
#define MPU6050_GYRO_OUT              0x43        // 陀螺仪数据寄存器地址
#define MPU6050_ACC_OUT               0x3B        // 加速度数据寄存器地址

#define MPU6050_ID                   0x68         // MPU6050默认设备ID

void MPU6050_WriteReg(uint8_t Address,uint8_t Data);
uint8_t MPU6050_ReadByte(uint8_t Address);
HAL_StatusTypeDef MPU6050_ReadBuffer(uint8_t Address,uint8_t *Buffer,uint8_t Size);
uint8_t MPU6050_GetID(void);
#endif
