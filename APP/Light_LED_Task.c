#include "Light_LED_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "LightSensor.h"
#include "LED.h"
#include "event_groups.h"
/* 创建光敏任务栈大小，用于静态创建任务 */
static StackType_t g_pucStackofLightSensorTask[128];
/* 创建灯光闪烁任务栈大小，用于创建静态任务 */
static StackType_t g_pucStackofLEDTask[128];
/* 创建光敏传感任务句柄 */
 TaskHandle_t LightSensorHandle;
/* 创建LED闪烁任务句柄 */
 TaskHandle_t LEDHandle;
/* 创建LED闪烁任务的TCB结构体,用于创建静态任务 */
static StaticTask_t g_TCBofLEDTask;
/* 创建光敏任务的TCB结构体，用于创建静态任务 */
static StaticTask_t g_TCBofLightSensorTask;

void Light_LED_Task(void *params){
	
/* 创建光敏传感器任务 */
  LightSensorHandle=xTaskCreateStatic(LightSensor_Task,"LightSensorTask",100,NULL,osPriorityNormal,g_pucStackofLightSensorTask,&g_TCBofLightSensorTask);
  
/* 创建灯光闪烁任务 */
  LEDHandle=xTaskCreateStatic(LED_Task,"LEDTask",50,NULL,osPriorityNormal,g_pucStackofLEDTask,&g_TCBofLEDTask);
	while(1){
		
	}
}
  
  