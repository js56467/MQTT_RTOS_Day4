#include "UART_Task.h"
#include "stdio.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
extern  char pcWriteBuffer[200];

int fputc(int ch,FILE *F){
	HAL_UART_Transmit(&huart1,(uint8_t *)&ch,1,HAL_MAX_DELAY);
	return ch;
}

void UART_Task(void *params){
	while(1){
printf("我已准备就绪，可以发送数据\r\n");
	vTaskList(pcWriteBuffer);
	printf("%s\r\n",pcWriteBuffer);
vTaskDelay(1000);
	}
}
