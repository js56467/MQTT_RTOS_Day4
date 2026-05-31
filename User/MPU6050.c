#include "MPU6050.h"
#include "i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "UART_Task.h"
#include "OLED.h"
#include "semphr.h"
#include "event_groups.h"
/* 外部信号量句柄 */
extern  SemaphoreHandle_t AATSemaphoreHandle;
/* 创建一个互斥量 */
extern SemaphoreHandle_t MPU6050_I2C_Mutex;
/* 外部事件组句柄 */
 extern EventGroupHandle_t g_xEventGroup_Light_Wake;
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

/* 获取MPU6050的ID号,只要在指定位置读一个字节即可 */
uint8_t MPU6050_GetID(void){
return MPU6050_ReadByte(MPU6050_WHO_AM_I);
}

/* 指定位置写一个Buffer */
void MPU6050_WriteBuffer(uint8_t Address,uint8_t *Buffer,uint8_t Size){
	uint8_t Data[Size+1],i;
	Data[0]=Address;
	for(i=0;i<Size;i++){
	Data[i+1]=Buffer[i];
	}
	memcpy(&Data[1],Buffer,Size);
HAL_I2C_Master_Transmit(&hi2c2,MPU6050_AD0D_SLAVER_ARRD<<1,Data,Size+1,1000);
}

/* 初始化MPU6050，通过写寄存器来控制和唤醒寄存器 */
HAL_StatusTypeDef MPU6050_Init(void){
	vTaskDelay(100);
	uint8_t IDNumber=0x68,MPU6050_Flag=0;
	if(IDNumber==MPU6050_GetID()){
	//printf("MPU6050已经接收到ID号\r\n");
	MPU6050_Flag=1;
	}else {
	//printf("未检测到ID号,请检查链接\r\n");
	return HAL_ERROR;
	}
	if(MPU6050_Flag==1){
	//printf("检测到MPU6050ID号%d",IDNumber);
    MPU6050_WriteReg(MPU6050_RA_PWR_MGMT_1, 0x00);    // 解除休眠
    MPU6050_WriteReg(MPU6050_RA_SMPLRT_DIV, 0x07);    // 设置采样率为1kHz/(7+1)=125Hz
    MPU6050_WriteReg(MPU6050_RA_ACCEL_CONFIG, 0x00);  // 加速度计2G模式
	MPU6050_Flag=0;
	return HAL_OK;
	}
	return HAL_ERROR;
}

HAL_StatusTypeDef MPU6050_ReadAcc(int16_t *Acc){
	uint8_t Data[6];
if(MPU6050_ReadBuffer(MPU6050_ACC_OUT,Data,6)==HAL_OK){
	/* X方向加速度 */
	Acc[0]=(int16_t)(Data[1]|(Data[0]<<8));
	/* Y方向加速度 */
	Acc[1]=(int16_t)(Data[3]|(Data[2]<<8));
	/* Z方向加速度 */
	Acc[2]=(int16_t)(Data[5]|(Data[4]<<8));
	return HAL_OK;
  }
else{
	//OLED_ShowString(4,1,"ReadAcc_Error");
	return HAL_ERROR;
	}
}

/* 为解决mpu6050与oled抢占资源的问题，现在引入一个互斥量 */
void MPU6050_GiveI2cMutex(void){
xSemaphoreGive(MPU6050_I2C_Mutex);

}
	
/* 获得互斥量 */
void MPU6050_GetI2cMutex(void){
xSemaphoreTake(MPU6050_I2C_Mutex,portMAX_DELAY);
}


void MPU6050_Task(void *Params){
	int16_t Acc[3];
if(MPU6050_Init()==HAL_OK){
while(1){
  MPU6050_GetI2cMutex();
  MPU6050_ReadAcc(Acc);
  OLED_ShowSignedNum(1,8,Acc[0],5);
  OLED_ShowSignedNum(2,8,Acc[1],5);
  OLED_ShowSignedNum(3,8,Acc[2],5);
  MPU6050_GiveI2cMutex();
  vTaskDelay(300);
  if(Acc[0]>=10000 || Acc[0]<=-10000 || Acc[1]>=10000 || Acc[1]<=-10000 || Acc[2]<10000){
	  xSemaphoreGive(AATSemaphoreHandle);
	  //xEventGroupSetBits(g_xEventGroup_Light_Wake,1<<4);
       }
     }
   }
 }





