#include "ESP8266.h"
#include "usart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "UART_Task.h"
#include "string.h"
#include "Timer_My.h"
#include "queue.h"
#include "DHT11.h"

#define WIFI_SSID   "iQOO12"
#define WIFI_PWD    "258258258"
#define TCP_SERVER_IP "mqtts.heclouds.com"
#define TCP_SERVER_PORT "1883"
#define Device_Name    "D1"
#define Device_ID      "0BQEYg770u"
#define Token          "version=2018-10-31&res=products%2F0BQEYg770u%2Fdevices%2FD1&et=1812008750&method=md5&sign=%2Fn95KGYJ%2Fek9%2BV3MD5VYRQ%3D%3D"
#define My_ID           "5" 
#define Params_Tem     "temp"
#define Params_Humi     "humi"

//#define OneNET_Internet_Address   "mqtts.heclouds.com"
/* ? */

#define Tem  				25
#define RECEIVE_ID          "222"

/* 定义全局缓存区,用来保存接收到的数据 */
static uint8_t g_uart_Rx_Buf[256];
/* 定义计数值,用来计收到了多少个数据 */
static uint8_t ESP8266_Cnt=0;
/* 定义上一个计数值,判断是否接收完成 */
static uint8_t ESP8266_PreCnt=0;
/* 收到的数据存放到这个变量里面 */
static uint8_t Data;
/* 表示即将上报的数据长度 */
static uint16_t Number_Message=0;
/* 队列句柄,用来读队列,当写队列时,同步数据到OneNET上 */
extern QueueHandle_t g_QueueMQTT;


extern struct DHT11SendData SData;
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
/* 因为getip发完命令还要用缓冲区,所以我单独把clean移到外面 */
HAL_StatusTypeDef ESP8266_Send_Command_Get_IP(char *Command,char * Ret){
	uint8_t Time_Out=250;
	HAL_UART_Transmit(&huart2,(uint8_t *)Command,strlen(Command),300);
	vTaskDelay(10);
    while(Time_Out--){
	if(ESP8266_Wait_Receive()==HAL_OK){
		if(strstr((const char *)g_uart_Rx_Buf,Ret)!=NULL){
		/* 清除上一个数据帧 */
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
return ESP8266_Send_Command("ATE0\r\n","OK");
}


void ESP8266_Task(void *Params){
	
	/* ESP8266接收使能 */
	HAL_UART_Receive_IT(&huart2,&Data,1);
	ESP8266_init();
	ESP8266_MQTT_Give_Data_Init();
	while(1){
		/* 等待烧录器到了我再看看传入数据的流程是怎样的,此处已经将温湿度的值传入到这个结构体Buffer中了 */
	  xQueueReceive(g_QueueMQTT,&SData,portMAX_DELAY);
		/* 关闭调度器 */
		//taskENTER_CRITICAL();
	  ESP8266_Send_Data_To_OneNET();
		/* 打开调度器 */
		//taskEXIT_CRITICAL();
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
if(ESP8266_Send_Command_Get_IP("AT+CIFSR\r\n","STAIP")==HAL_OK){
	Sta=strstr((char *)g_uart_Rx_Buf,"\"");
	End=strstr(Sta+1,"\"");
	*End='\0';
	sprintf(Buf,"%s",Sta+1);
	ESP8266_Clean();
	return HAL_OK;
  }else{
  return HAL_ERROR;
  }
}

/* 连接TCP服务器 */
HAL_StatusTypeDef ESP8266_Connect_TCP_Server(char *Server_IP,char *Server_Port){
	char cmd[64];
	sprintf(cmd,"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",Server_IP,Server_Port);
	 return ESP8266_Send_Command(cmd,"CO");
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


/* OneNET服务器上的设备授权 */
HAL_StatusTypeDef ESP8266_Link_Device_Status(char *device_name,char *device_iD,char *token){
	char   cmd[256]; //我去,本来写128以为够了调试半天,发现128确实小了
	sprintf(cmd,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",device_name,device_iD,token);
    return ESP8266_Send_Command(cmd,"OK");
}

/* TCP+MQTT握手,正式连上OneNET云平台,OneNET设备从离线变为在线 */
HAL_StatusTypeDef ESP8266_TCP_MQTT_Link(char *internet_Address,char * mqtt_Port){
    char cmd[128];
	sprintf(cmd,"AT+MQTTCONN=0,\"%s\",%s,1\r\n",internet_Address,mqtt_Port);
	return ESP8266_Send_Command(cmd,"OK");
}

/* 订阅属性上报回复主题,能收到平台确认消息 */
HAL_StatusTypeDef ESP8266_Subscribe(char * device_id,char * device_name){
    char cmd[128];
	sprintf(cmd,"AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/post/reply\",0\r\n",device_id,device_name);
	return ESP8266_Send_Command(cmd,"OK");
}


/* 计算下一个命令长度为多少的函数 */
HAL_StatusTypeDef ESP8266_Count_CmdLen(char * my_id,char *params_tem,int tem_data,char *params_humi,int hum_data){
	char cmd[128];
	sprintf(cmd,"{\"id\":\"%s\",\"params\":{\"%s\":{\"value\":%d},\"%s\":{\"value\":%d}}}\r\n",my_id,params_tem,tem_data,params_humi,hum_data);
	 Number_Message=strlen(cmd);
	return HAL_OK;
}

/* 设备上报属性数据,上传数据到OneNET云,第一步进入透传模式 */
HAL_StatusTypeDef ESP8266_Send_Data_OneNET_CIP(char * device_id,char * device_name,uint16_t number_message){
    char cmd[128];
	sprintf(cmd,"AT+MQTTPUBRAW=0,\"$sys/%s/%s/thing/property/post\",%d,0,0\r\n",device_id,device_name,Number_Message);
	return ESP8266_Send_Command(cmd,"OK");
}



/* 上报指定数据,上报完退出透传模式 */
HAL_StatusTypeDef ESP8266_Send_Data(char * my_id,char *params_tem,int tem_data,char *params_humi,int hum_data){
	char cmd[128];
	sprintf(cmd,"{\"id\":\"%s\",\"params\":{\"%s\":{\"value\":%d},\"%s\":{\"value\":%d}}}\r\n",my_id,params_tem,tem_data,params_humi,hum_data);
	return ESP8266_Send_Command(cmd,"OK");
}


/* 订阅平台下发控制指令主题,订阅这条指令后串口自动下发控制指令主题 */
HAL_StatusTypeDef ESP8266_OneNet_Command(char * device_id,char * device_name){
	char cmd[128];
	sprintf(cmd,"AT+MQTTSUB=0,\"$sys/%s/%s/thing/property/set\",0\r\n",device_id,device_name);
	return ESP8266_Send_Command(cmd,"OK");
}

/* 收到云端指令后,回复应答给平台 */
HAL_StatusTypeDef ESP8266_MQTT_ACK(char * device_id,char * device_name,char * receive_id){
    char cmd[128];
	sprintf("AT+MQTTPUB=0,\"$sys/%s/%s/thing/property/set_reply\"\r\n",device_id,device_name,receive_id);
	return ESP8266_Send_Command(cmd,"OK");
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
//	while(ESP8266_Out_Transparent())
//        vTaskDelay(500); 
	
	printf("1.RESTORE\r\n");
	while(ESP8266_Restore())
	    vTaskDelay(300);
	
	printf("2.AT\r\n");
	while(ESP8266_AT_Test())
         vTaskDelay(300);
	
	printf("3.RST\r\n");
    while(ESP8266_SW_Reset())
         vTaskDelay(300);
	
	printf("4.CWMODE=1\r\n");
	while(ESP8266_Set_Mode(ESP8266_STA_MODE))
         vTaskDelay(300);
	
//	printf("5.AT+CIPMUX\r\n");  //设置单路连接模式，透传只能使用此模式
//	while(ESP8266_Single_Connection())
//        vTaskDelay(500);
	
	printf("5.CWJAP\r\n");      //连接WIFI
	while(ESP8266_Join_AP(WIFI_SSID, WIFI_PWD))
         vTaskDelay(300);
    
//    printf("7.CIFSR\r\n");
//    while(ESP8266_Get_IP(ip_buf))
//       vTaskDelay(500);

   // printf("ESP8266 IP: %s\r\n", ip_buf);
	
//	printf("8.CIPSTART\r\n");
//    while(ESP8266_Connect_TCP_Server(TCP_SERVER_IP, TCP_SERVER_PORT))
//       vTaskDelay(500);
    
//    printf("9.CIPMODE\r\n");
//    while(ESP8266_Out_Transparent())
   //     vTaskDelay(500);
    
    printf("ESP8266_Init OK\r\n");
    return HAL_OK;
}

/* 此处是在用AT指令连接wifi以后用mqtt协议连接上OneNET的初始化步骤 */
HAL_StatusTypeDef ESP8266_MQTT_Give_Data_Init(void){

	printf("1.MQTTStatus\r\n");
	while(ESP8266_Link_Device_Status(Device_Name,Device_ID,Token));
	vTaskDelay(500);
	
	printf("2.MQTTConnct\r\n");
	while(ESP8266_TCP_MQTT_Link(TCP_SERVER_IP, TCP_SERVER_PORT));
	vTaskDelay(500);
	
//	printf("2.5 CountLen\r\n");
//	while(ESP8266_Count_CmdLen(My_ID,Params_Tem,SData.dht11_Hum,Params_Humi,SData.dht11_Tem));
//	vTaskDelay(500);
	
	printf("3.Sub_Up\r\n");
	while(ESP8266_Subscribe(Device_ID,Device_Name));
	vTaskDelay(500);
	
//	printf("4.MQTT_Data_UP_CIP\r\n");
//	while(ESP8266_Send_Data_OneNET_CIP(Device_ID,Device_Name,Number_Message));
//	vTaskDelay(500);
	
//	printf("5.MQTT_Data_UP\r\n");
//	while(ESP8266_Send_Data(My_ID,Params_Tem,SData.dht11_Hum,Params_Humi,SData.dht11_Tem));
//	vTaskDelay(500);
	
	printf("4.Sub_Dowm\r\n");
	while(ESP8266_OneNet_Command(Device_ID,Device_Name));
	vTaskDelay(500);
	
//	printf("6.ESP8266_ACK\r\n");
//	while(ESP8266_MQTT_ACK(Device_ID,Device_Name,RECEIVE_ID));
//	vTaskDelay(500);
	
	printf(" MQTT_SendData_Init  OK\r\n");
	return HAL_OK;
}

HAL_StatusTypeDef ESP8266_Send_Data_To_OneNET(void){
	printf("1.CountLen\r\n");
	while(ESP8266_Count_CmdLen(My_ID,Params_Tem,SData.dht11_Hum,Params_Humi,SData.dht11_Tem));
	vTaskDelay(500);
	
	printf("2.MQTT_Data_UP_CIP\r\n");
	while(ESP8266_Send_Data_OneNET_CIP(Device_ID,Device_Name,Number_Message));
	vTaskDelay(500);
	
	printf("3.MQTT_Data_UP\r\n");
	while(ESP8266_Send_Data(My_ID,Params_Tem,SData.dht11_Hum,Params_Humi,SData.dht11_Tem));
	vTaskDelay(500);
	
	return HAL_OK;
}

