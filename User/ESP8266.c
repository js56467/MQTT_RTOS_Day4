#include "ESP8266.h"
#include "main.h"
#include "usart.h"
/* 定义全局缓存区,用来保存接收到的数据 */
static uint8_t g_uart_Rx_Buf[128];
/* 定义计数值,用来计收到了多少个数据 */
static uint8_t ESP8266_Cnt=0;

/* 回调函数,接收到数据触发中断 */
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	uint8_t Data=0;
	/* 防止接收过大溢出 */
	if(ESP8266_Cnt>sizeof(g_uart_Rx_Buf)){
		ESP8266_Cnt=0;
	}
	/* 判断接收完成 */
  if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET){
	  /* 存入数据给Data */
	  HAL_UART_Receive_IT(&huart2,&Data,1);
	  /* 给全局缓冲区存一份,Cnt++表示记的数 */
	  g_uart_Rx_Buf[ESP8266_Cnt++]=Data;
  }
}