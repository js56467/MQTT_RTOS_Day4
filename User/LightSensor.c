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
static uint16_t g_Data[5];
extern int fputc(int ch,FILE *F);
static uint16_t Aver_Data=0;
void LightSensor_Task(void* params){
/* 开启ADC校验 */
HAL_ADCEx_Calibration_Start(&hadc1);
	while(1){
	/* 开启ADC读取数据和DMA转换，DMA转化完数据立马进中断 */
		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)g_Data,5); //DMA传满5个数据进入中断，如果用单次扫描只有1个数据，就只有第五次有数据
		vTaskDelay(500);	
		/* 判断是否转化完成 */
		//printf("接收到了光敏传感器数据:%d\r\n",Aver_Data);
		OLED_ShowNum(2,1,Aver_Data,4);
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
