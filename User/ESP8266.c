#include "ESP8266.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "UART_Task.h"
#include "string.h"
#include "Timer_My.h"

#define WIFI_SSID   "iQOO12"
#define WIFI_PWD    "258258258"
#define TCP_SERVER_IP "172.25.246.114"
#define TCP_SERVER_PORT "8000"

/* 定义全局缓存区,用来保存接收到的数据 */
static uint8_t g_uart_Rx_Buf[256];
/* 定义计数值,用来计收到了多少个数据 */
static uint8_t ESP8266_Cnt=0;
/* 定义上一个计数值,判断是否接收完成 */
static uint8_t ESP8266_PreCnt=0;
/* 收到的数据存放到这个变量里面 */
static uint8_t Data;

/* 回调函数,接收到数据触发中断 */
 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)	 
{
	if(huart -> Instance==USART2){
	  /* 给全局缓冲区存一份,Cnt++表示记的数 */
	g_uart_Rx_Buf[ESP8266_Cnt++]=Data;
	//printf("收到一字节:%02x\r\n",Data);
		/* 使能下一个数据接收 */
	HAL_UART_Receive_IT(&huart2,&Data,1);
   }

}

/* 将全局计数值和设置的值进行比较,先赋值若相等说明没有数据进来了 */
HAL_StatusTypeDef ESP8266_Wait_Receive(void){
	
	/* 判断收没收到数据的情况 */
  if(ESP8266_Cnt==0){
  printf("ESP8266_Cnt=0\r\n");
  return HAL_ERROR;
  }
  ESP8266_PreCnt=ESP8266_Cnt;
  vTaskDelay(10);
  if(ESP8266_PreCnt==ESP8266_Cnt){
	//printf("ESP8266_Receive_OK\r\n");
  return HAL_OK;
  }
  return HAL_ERROR;
   }
	
/* 在判断完一个数据帧以后就要进行清除 */
void ESP8266_Clean(void){
	memset(g_uart_Rx_Buf,0,sizeof(g_uart_Rx_Buf));
	ESP8266_Cnt=0;
	ESP8266_PreCnt=0;
//	memset(Data,0,sizeof(Data));
	Data=0;
}

/* 给串口发送一个字符串,再根据他返回来的值判断有没有我想要的字符串比如"OK",有说明成功了,现在是要编程实现AT指令 */
HAL_StatusTypeDef ESP8266_Send_Command(char *Command,char * Ret){
	/* while循环超时时间 */
	uint8_t Time_Out=250;
	/* 给ESP8266发送一个cmd指令,目的是通过里面本就烧好的AT固件库所返还的指令里面看有没有我要的Ret */
	HAL_UART_Transmit(&huart2,(uint8_t *)Command,strlen(Command),300);
	/* 此处延时一会是为了等待中断收完数据 */
	vTaskDelay(10);
		//printf("收到字符串%s",g_uart_Rx_Buf);
    while(Time_Out--){
	/* 等待发送完成 */
	if(ESP8266_Wait_Receive()==HAL_OK){
		//g_uart_Rx_Buf[ESP8266_Cnt]='\0';
		/* 如果有,说明成功发送了 */
		if(strstr((const char *)g_uart_Rx_Buf,Ret)!=NULL){
		//printf("收到数据%s\r\n",g_uart_Rx_Buf);
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

/* 软件复位 */
HAL_StatusTypeDef ESP8266_SW_Reset(void){
return ESP8266_Send_Command("AT+RST\r\n","OK");
}

/* 恢复出厂设置 */
HAL_StatusTypeDef ESP8266_Restore(void){
return ESP8266_Send_Command("AT+RESTORE\r\n","OK");
}

HAL_StatusTypeDef ESP8266_Set_Mode(uint8_t Mode){
	
  switch(Mode){
	  case ESP8266_STA_MODE:return ESP8266_Send_Command("AT+CWMODE=1\r\n","OK");
	  case ESP8266_AP_MODE:return ESP8266_Send_Command("AT+CWMODE=2\r\n","OK");
	  case ESP8266_AP_STA_MODE:return ESP8266_Send_Command("AT+CWMODE=3\r\n","OK");
	  
	  default: return HAL_ERROR;
  }
}

/* 关闭回显模式 */
HAL_StatusTypeDef ESP8266_ATE0_Config(void){
return ESP8266_Send_Command("AT+ATE0\r\n","OK");
}


void ESP8266_Task(void *Params){
	
	/* ESP8266接收使能 */
	HAL_UART_Receive_IT(&huart2,&Data,1);
	ESP8266_init();
	while(1){
////	if((ESP8266_Send_Command("AT\r\n","OK"))==HAL_OK){
////	vTaskDelay(500);
////	printf("AT_Tesk_OK\r\n");
////	}
////	else{
////	printf("AT_Tesk_Fail\r\n");
//	if((ESP8266_Set_Mode(1)==HAL_OK)){
//	vTaskDelay(500);
//	printf("AT_Tesk_OK\r\n");
//	}
//	else{
//	printf("AT_Tesk_Fail\r\n");
//	   }
//	ESP8266_Clean();
     }
}
/* 单路链接 */
HAL_StatusTypeDef ESP8266_Single_Connection(void){
	ESP8266_Send_Command("AT+CIPMUX=0\r\n","OK");
	return HAL_OK;
}


/* 连接WIFI */
HAL_StatusTypeDef ESP8266_Join_AP(char * SSID,char * PWD){
	char cmd[64];
	sprintf(cmd,"AT+CWJAP=\"%s\",\"%s\"\r\n", SSID, PWD);
	return ESP8266_Send_Command(cmd,"WIFI GOT IP");
}

/* 得到ESP8266的IP地址 */
HAL_StatusTypeDef ESP8266_Get_IP(char * Buf){
	char *Sta;
	char *End;
if(ESP8266_Send_Command("AT+CIFSR\r\n","STAIP")==HAL_OK){
	Sta=strstr((char *)g_uart_Rx_Buf,"\"");
	End=strstr(Sta+1,"\"");
	*End='\0';
	sprintf(Buf,"%s",Sta+1);
	return HAL_OK;
  }else{
  return HAL_ERROR;
  }
}
/* 连接TCP服务器 */
HAL_StatusTypeDef ESP8266_Connect_TCP_Server(char *Server_IP,char *Server_Port){
	char cmd[64];
	sprintf(cmd,"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",Server_IP,Server_Port);
	return ESP8266_Send_Command(cmd,"CONNECT");
}
/* 断开TCP服务器 */
HAL_StatusTypeDef ESP8266_Disconnect_TCP_Server(void){
return ESP8266_Send_Command("AT+CIPCLOSE\r\n"," ");
}

/* 开启CIP透传(单链接生效) */
HAL_StatusTypeDef ESP8266_Enter_Transparent(void){
uint8_t Ret;
	Ret=ESP8266_Send_Command("AT+CIPMODE=1\r\n","OK");
	Ret+=ESP8266_Send_Command("AT+CIPSEND\r\n",">");
if(Ret==0){
return HAL_OK;
  }else{
  return HAL_ERROR;
  }
}

/* 退出CIP透传 */
HAL_StatusTypeDef ESP8266_Out_Transparent(void){
return ESP8266_Send_Command("+++","");
}


/**
 * @brief       ESP8266初始化
 * @param       baudrate: ESP8266 UART通讯波特率
 * @retval      ESP8266_EOK  : ESP8266初始化成功，函数执行成功
 *              ESP8266_ERROR: ESP8266初始化失败，函数执行失败
 */
HAL_StatusTypeDef ESP8266_init(void)
{
    char ip_buf[16];
    /* 让WIFI退出透传模式 */
	while(ESP8266_Out_Transparent())
        vTaskDelay(500);

	printf("1.AT\r\n");
	while(ESP8266_AT_Test())
        vTaskDelay(500);
	
	printf("2.RST\r\n");
    while(ESP8266_SW_Reset())
        vTaskDelay(500);
	while(ESP8266_Disconnect_TCP_Server())
        vTaskDelay(500);
	
	printf("3.CWMODE\r\n");
	while(ESP8266_Set_Mode(ESP8266_STA_MODE))
        vTaskDelay(500);
	
	printf("4.AT+CIPMUX\r\n");  //设置单路连接模式，透传只能使用此模式
	while(ESP8266_Single_Connection())
        vTaskDelay(500);
	
	printf("5.CWJAP\r\n");      //连接WIFI
	while(ESP8266_Join_AP(WIFI_SSID, WIFI_PWD))
        vTaskDelay(1000);
    
    printf("6.CIFSR\r\n");
    while(ESP8266_Get_IP(ip_buf))
        vTaskDelay(500);

    printf("ESP8266 IP: %s\r\n", ip_buf);
	printf("7.CIPSTART\r\n");
    while(ESP8266_Connect_TCP_Server(TCP_SERVER_IP, TCP_SERVER_PORT))
        vTaskDelay(500);
    
    printf("8.CIPMODE\r\n");
    while(ESP8266_Out_Transparent())
        vTaskDelay(500);
    
    printf("ESP8266_Init OK\r\n");
    return HAL_OK;
}

    
//void ESP8266_STA_Mode_Init
