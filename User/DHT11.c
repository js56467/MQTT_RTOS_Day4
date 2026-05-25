#include "gpio.h"
#include "DHT11.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Timer_My.h"
#include "OLED.h"
#define DHT11_DATA_Pin GPIO_PIN_4
#define DHT11_DATA_GPIO_Port GPIOA
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
uint8_t DHT11_ReadData(int *Hum,int *Tem){
//	OLED_ShowString(3,1,"Hum:  ");
//	OLED_ShowString(3,8,"Tem:  ");
	   /* 初始化DHT11数据引脚 */
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);
	struct DHT11Data RData;
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
	OLED_ShowString(4,1,"		");
	OLED_ShowString(4,1,"1ERROR");
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
	OLED_ShowString(4,1,"		");
	OLED_ShowString(4,1,"2ERROR");
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
	OLED_ShowString(4,1,"		");
	OLED_ShowString(4,1,"3ERROR");
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
int16_t sum=RData.humi_int+RData.humi_deci+RData.temp_int+RData.temp_deci;
if(RData.check_sum==sum){
//	//OLED_ShowString(3,1,"Hum:  .  ");
//	OLED_ShowNum(3,5,(int32_t)RData.humi_int,2);
//	//OLED_ShowSignedNum(3,8,(int32_t)RData.humi_deci,2);
//	//OLED_ShowString(3,1,"Tem:  .  ");
//	OLED_ShowNum(3,12,(int32_t)RData.temp_int,2);
//	//OLED_ShowSignedNum(3,13,(int32_t)RData.temp_deci,2);
//	OLED_ShowString(4,8,"        ");	
//	OLED_ShowString(4,1,"Succss");
	/* 解引用指针 改传入参数的地址里面的数据 */
	*Hum=RData.humi_int;
	*Tem=RData.temp_int;
	OLED_ShowString(4,1,"        ");
  return 1;
  }else{
	OLED_ShowString(4,8,"Suc B F");
	return 0;
  }
} 

void DHT11_Task(void *Params){
int Hum=0,Tem=0;
	while(1){
	DHT11_ReadData(&Hum,&Tem);
	OLED_ShowString(3,1,"Hum:  ");
	OLED_ShowString(3,8,"Tem:  ");
	OLED_ShowNum(3,5,(uint32_t)Hum,2);
	OLED_ShowNum(3,12,(uint32_t)Tem,2);
	vTaskDelay(100);
	}
}
