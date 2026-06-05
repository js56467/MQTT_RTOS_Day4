#include "gpio.h"
#include "DHT11.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Timer_My.h"
#include "OLED.h"
#include "MPU6050.h"
#include "queue.h"
#define xQueueLenMQTT 10
#define DHT11_DATA_Pin GPIO_PIN_4
#define DHT11_DATA_GPIO_Port GPIOA
/* 创建任务句柄,此处用队列是代替用全局变量来"跳跃"不同函数的情况 */
QueueHandle_t g_QueueMQTT;
struct DHT11Data RData;
struct DHT11SendData SData;
/* 创建一个Buffer,用于静态创建队列 */
static uint8_t g_pucQueueStorageBufferMQTT[xQueueLenMQTT*sizeof(&SData)];
/* 创建一个结构体,用于静态创建队列,保存数据 */
static StaticQueue_t g_pxQueueBufferMQTT;

int g_DHT11_Tem=0,g_DHT11_Hum=0;
/* 选择GPIO模式，为DHT11主从机的切换做准备 */
void SelectGPIOMode_DHT11(uint16_t GPIO_Mode , uint16_t GPIO_Pull){
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_Mode;
  GPIO_InitStruct.Pull = GPIO_Pull;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* 根据读到的数据写字节 */
int8_t DHT11_ReadByte(void){
	
	/* 根据从机读取的信号来写入Value值 */
	int8_t i=0,Value=0x00;
	for(i=0;i<8;i++){
	while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==GPIO_PIN_RESET);
	udelay(40);
	if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==GPIO_PIN_SET){
	   Value|=(1<<(7-i));
	  }
	  else {
	  //Value|=(0<<(7-i));
	  }
	  while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==SET);
	}
	return Value;
}

/* 完整流程 */
uint8_t DHT11_ReadData(void){
	/* 静态创建队列 */
	g_QueueMQTT=xQueueCreateStatic(xQueueLenMQTT,sizeof(&SData),g_pucQueueStorageBufferMQTT,&g_pxQueueBufferMQTT);
	 /* 初始化DHT11数据引脚 */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	
/* 主机发送特定信号 */
SelectGPIOMode_DHT11(GPIO_MODE_OUTPUT_PP,GPIO_NOPULL);
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_RESET);
mdelay(20);
HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
udelay(30);
/* 变为从机模式 */
SelectGPIOMode_DHT11(GPIO_MODE_INPUT,GPIO_PULLUP);
/* 设置变量表示超时退出 */
uint32_t ReTry=0;
/* 等待拉低表示响应 */
while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==GPIO_PIN_SET){
	
	udelay(1);
	if(ReTry<100){
	ReTry++;
	}else{

//	OLED_ShowString(4,1,"		");
//	OLED_ShowString(4,1,"1ERROR");
	return 0;
	}
}
/* 等待拉高表示准备发数据 */
ReTry=0;
while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==GPIO_PIN_RESET){
	udelay(1);
	if(ReTry<100){
	ReTry++;
	}else{
//	OLED_ShowString(4,1,"		");
//	OLED_ShowString(4,1,"2ERROR");
	return 0;
	}
	
}
/* 再次拉低表示开始发数据 */
ReTry=0;
while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4)==GPIO_PIN_SET){
	udelay(1);
	if(ReTry<100){
	ReTry++;
	}else{
//	OLED_ShowString(4,1,"		");
//	OLED_ShowString(4,1,"3ERROR");
	return 0;
	}
}

/* 开始读数据 */
RData.humi_int=DHT11_ReadByte();
RData.humi_deci=DHT11_ReadByte();
RData.temp_int=DHT11_ReadByte();
RData.temp_deci=DHT11_ReadByte();
RData.check_sum=DHT11_ReadByte();
/* 开始校验 */
uint16_t sum=RData.humi_int+RData.humi_deci+RData.temp_int+RData.temp_deci;
if(RData.check_sum==sum){
	
	/* 解引用指针 改传入参数的地址里面的数据 */
	//*hum=RData.humi_int;
	//*tem=RData.temp_int;
	SData.dht11_Hum=RData.humi_int;
	SData.dht11_Tem=RData.temp_int;
  return 1;
  }else{
	  MPU6050_GetI2cMutex();
	  /* 表示传入了数据但和校验位不对 */
	OLED_ShowString(1,1,"SBF");
	  MPU6050_GiveI2cMutex();
	return 0;
  }
} 

void DHT11_Task(void *Params){

	OLED_ShowString(3,1,"Hum:  ");
	OLED_ShowString(4,1,"Tem:  ");
	while(1){
	/* 关闭调度器,对于DHT11对时序要求精度高的硬件,防止时序进行时被打断,此时关闭调度器打断TICK中断 */
	taskENTER_CRITICAL();
	DHT11_ReadData();
	/* 打开调度器,注意！在开关调度器保护的资源里面,比如这里的ReadData中不可调用任务阻塞式API!!! */
	taskEXIT_CRITICAL();
	/* 获得互斥量,防止多任务抢占i2c资源 */
    MPU6050_GetI2cMutex();
	OLED_ShowNum(3,5,SData.dht11_Hum,2);
	OLED_ShowNum(4,5,SData.dht11_Tem,2);
	/* 给出互斥量 */
	MPU6050_GiveI2cMutex();
	/* 将得到的数据写入队列 */
	 xQueueSend(g_QueueMQTT,&SData,portMAX_DELAY);
	vTaskDelay(100);
	}
  }
