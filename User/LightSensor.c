#include "LightSensor.h"
#include "adc.h"
#include "dma.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stdio.h"
#include "UART_Task.h"
#include "OLED.h"
#include "Timer_My.h"
#include "DHT11.h"
#include "OLED.h"
#include "Light_LED_Task.h"
#include "event_groups.h"
#include "MPU6050.h"
#include "semphr.h"
static uint16_t g_Data[5];
extern int fputc(int ch,FILE *F);
 extern TaskHandle_t LEDHandle;
static uint16_t Aver_Data=0;

/* 外部信号量句柄,表示姿态检测和光照太强都可以触发蜂鸣器任务 */
extern SemaphoreHandle_t AATSemaphoreHandle;
/* 创建任务组句柄 */
 EventGroupHandle_t g_xEventGroup_Light_Wake;
 
void LightSensor_Task(void* params){
/* 开启ADC校验 */
HAL_ADCEx_Calibration_Start(&hadc1);
/* 创建任务组 */
g_xEventGroup_Light_Wake = xEventGroupCreate();
	while(1){
	/* 开启ADC读取数据和DMA转换，DMA转化完数据立马进中断 */
		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)g_Data,5); //DMA传满5个数据进入中断，如果用单次扫描只有1个数据，就只有第五次有数据
		vTaskDelay(100);
		MPU6050_GetI2cMutex();
			/* 测试 */
		OLED_ShowNum(2,1,Aver_Data,4);
		
		MPU6050_GiveI2cMutex();
		if(Aver_Data>2500){
		xEventGroupSetBits(g_xEventGroup_Light_Wake,1<<0);
		}else if (Aver_Data>200){
		xEventGroupSetBits(g_xEventGroup_Light_Wake,1<<2);
		}else if(Aver_Data<200){
		//xEventGroupSetBits(g_xEventGroup_Light_Wake,1<<3);
		/* 蜂鸣器信号量,二者有一个便可响 */
		xSemaphoreGive(AATSemaphoreHandle);
		}
		Aver_Data=0;
		}
  }


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{	/* 采集5次得到的数值记录平均值 */
	uint16_t i,ALL_Data=0;
	for(i=0;i<5;i++){
		ALL_Data+=g_Data[i];
	}
	Aver_Data=ALL_Data/5;
}
