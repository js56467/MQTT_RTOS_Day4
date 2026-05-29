#include "LED.h"
#include "gpio.h"
#include "Timer_My.h"
#include "FreeRTOS.h"
#include "task.h"
#include "OLED.h"
#include "MPU6050.h"
#include <stdint.h>
#include "event_groups.h"
extern EventGroupHandle_t g_xEventGroup_Light_Wake;
void LED_OFF(void){
xEventGroupWaitBits(g_xEventGroup_Light_Wake,1<<2,pdTRUE,pdTRUE,portMAX_DELAY);
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);
	
}
void LED_ON(void){
xEventGroupWaitBits(g_xEventGroup_Light_Wake,1<<0,pdTRUE,pdTRUE,portMAX_DELAY);
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_RESET);
}
void LED_Task(void *Params){
	while(1){
//	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
	LED_ON();
	LED_OFF();	
	}
}
