#include "Buzzer.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "OLED.h"
#include "semphr.h"
/* 外部信号量句柄 */
extern SemaphoreHandle_t AATSemaphoreHandle;
void Buzzer_ON(void){

HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
//OLED_ShowString(1,1,"Buz_ON");

}
void Buzzer_OFF(void){
	
	
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
//OLED_ShowString(2,1,"Bu_OFF");
}

void Buzzer_Task(void *params){
	while(1){
	/* 获得信号量 */
	xSemaphoreTake(AATSemaphoreHandle,portMAX_DELAY);
	Buzzer_ON();
	vTaskDelay(300);
	Buzzer_OFF();
	}
}


