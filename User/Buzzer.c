#include "Buzzer.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "OLED.h"
void Buzzer_ON(void){
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
OLED_ShowString(1,1,"Buzzer_ON");

}
void Buzzer_OFF(void){
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);
OLED_ShowString(2,1,"Buzzer_OFF");
}

void Buzzer_Task(void *params){
	while(1){
	Buzzer_ON();
	Buzzer_OFF();
	OLED_ShowString(1,1,"         ");
	OLED_ShowString(2,1,"         ");
	vTaskDelay(300);
	}
}


