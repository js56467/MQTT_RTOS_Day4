#include "ESP8266.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "UART_Task.h"
#include "string.h"
#include "Timer_My.h"
/* 定义全局缓存区,用来保存接收到的数据 */
static uint8_t g_uart_Rx_Buf[128];
/* 定义计数值,用来计收到了多少个数据 */
static uint8_t ESP8266_Cnt=0;
/* 定义上一个计数值,判断是否接收完成 */
static uint8_t ESP8266_PreCnt=0;
/* 回调函数,接收到数据触发中断 */
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)	 
{
	if(huart==&huart2){
	/* 判断接收完成 */
//	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE)!=RESET){
	uint8_t Data=0;
	/* 防止接收过大溢出 */
	if(ESP8266_Cnt>sizeof(g_uart_Rx_Buf)-1){
		ESP8266_Cnt=0;
	}
	  /* 存入数据给Data */
	  HAL_UART_Receive(&huart2,&Data,1,0);
	  /* 给全局缓冲区存一份,Cnt++表示记的数 */
	  g_uart_Rx_Buf[ESP8266_Cnt++]=Data;
	 HAL_UART_Receive_IT(&huart2,&Data,1);
   }
//	HAL_UART_IRQHandler(&huart2);
//}
}

/* 将全局计数值和设置的值进行比较,先赋值若相等说明没有数据进来了 */
HAL_StatusTypeDef ESP8266_Wait_Receive(void){
	/* 判断收没收到数据的情况 */
  if(ESP8266_Cnt==0){
	 printf("ESP8266_Cnt=0\r\n");
  return HAL_ERROR;
  }
  if(ESP8266_PreCnt==ESP8266_Cnt){
//	printf("ESP8266_Receive_OK\r\n");
  return HAL_OK;
  }
  ESP8266_PreCnt=ESP8266_Cnt;
  return HAL_ERROR;
}

/* 在判断完一个数据帧以后就要进行清除 */
void ESP8266_Clean(void){
memset(g_uart_Rx_Buf,0,sizeof(g_uart_Rx_Buf));
ESP8266_Cnt=0;
ESP8266_PreCnt=0;
}

/* 给串口发送一个字符串,再根据他返回来的值判断有没有我想要的字符串比如"OK",有说明成功了,现在是要编程实现AT指令 */
HAL_StatusTypeDef ESP8266_Send_Command(char *Command,char * Ret){
	/* while循环超时时间 */
	uint8_t Time_Out=250;

	/* 给串口发送一个cmd指令,目的是通过里面本就烧好的AT固件库所返还的指令里面看有没有我要的Ret */
	HAL_UART_Transmit(&huart2,(uint8_t *)Command,strlen(Command),200);
    while(Time_Out--){
	/* 等待发送完成 */
	if(ESP8266_Wait_Receive()==HAL_OK){
		g_uart_Rx_Buf[ESP8266_Cnt]='\0';
		/* 如果有,说明成功发送了 */
		if(strstr((const char *)g_uart_Rx_Buf,Ret)==NULL){
		printf("收到数据%s\r\n",g_uart_Rx_Buf);
		/* 清除上一个数据帧 */
		ESP8266_Clean();
		return HAL_OK;
		}
	}
	vTaskDelay(10);
  }
return HAL_ERROR;
}
/* 实现AT测试指令 */
HAL_StatusTypeDef ESP8266_AT_Test(void){
return ESP8266_Send_Command("AT\r\n","OK");
}

HAL_StatusTypeDef ESP8266_SW_Reset(void){
return ESP8266_Send_Command("AT+RST\r\n","OK");
}

HAL_StatusTypeDef ESP8266_Set_Mode(uint8_t Mode){
	
  switch(Mode){
	  case ESP8266_STA_MODE:return ESP8266_Send_Command("AT+CWMODE=1\r\n","OK");
	  case ESP8266_AP_MODE:return ESP8266_Send_Command("AT+CWMODE=2\r\n","OK");
	  case ESP8266_AP_STA_MODE:return ESP8266_Send_Command("AT+CWMODE=3\r\n","OK");
	  
	  default: return HAL_ERROR;
  }
}

void ESP8266_Task(void *Params){
	/* ESP8266接收使能 */
	uint8_t PreData=0;
	HAL_UART_Receive_IT(&huart2,&PreData,1);
	while(1){
	if(ESP8266_Send_Command("AT\r\n","OK")==HAL_OK){
	printf("AT_Tesk_OK\r\n");
	}
	else{
	printf("AT_Tesk_Fail\r\n)");
	}
 }
}









