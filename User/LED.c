#include "LED.h"
#include "gpio.h"
#include "Timer_My.h"
#include "FreeRTOS.h"
#include "task.h"
#include "OLED.h"
#include "MPU6050.h"
#include <stdint.h>
void LED_ON(void){
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
}
void LED_OFF(void){
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
}
void LED_Task(void *Params){
	uint8_t My_MPU6050_ID=0;
	while(1){
	
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
	vTaskDelay(350);
//	/* 测试MPU6050ID号 */
//	My_MPU6050_ID=MPU6050_GetID();
//	OLED_ShowNum(1,1,MPU6050_ID,3);
	}
}
