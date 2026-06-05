#ifndef __ESP8266_h
#define __ESP8266_h

#include "main.h"

#define ESP8266_STA_MODE 1
#define ESP8266_AP_MODE 2
#define ESP8266_AP_STA_MODE 3



HAL_StatusTypeDef ESP8266_Wait_Receive(void);
void ESP8266_Clean(void);
HAL_StatusTypeDef ESP8266_Send_Command(char *Command,char * Ret);
HAL_StatusTypeDef ESP8266_Send_Command_Get_IP(char *Command,char * Ret);
HAL_StatusTypeDef ESP8266_AT_Test(void);
HAL_StatusTypeDef ESP8266_SW_Reset(void);
HAL_StatusTypeDef ESP8266_Set_Mode(uint8_t Mode);
void ESP8266_Task(void *Params);
HAL_StatusTypeDef ESP8266_Restore(void);
HAL_StatusTypeDef ESP8266_ATE0_Config(void);
HAL_StatusTypeDef ESP8266_Single_Connection(void);
HAL_StatusTypeDef ESP8266_Join_AP(char * SSID,char * PWD);
HAL_StatusTypeDef ESP8266_Get_IP(char * Buf);
HAL_StatusTypeDef ESP8266_Connect_TCP_Server(char *Server_IP,char *Server_Port);
HAL_StatusTypeDef ESP8266_Enter_Transparent(void);
HAL_StatusTypeDef ESP8266_Out_Transparent(void);
HAL_StatusTypeDef ESP8266_Link_Device(char *Device_Name,char *Device_ID,char *Token);
HAL_StatusTypeDef ESP8266_init(void);	
HAL_StatusTypeDef ESP8266_Link_Device_Status(char *device_name,char *device_iD,char *token);
HAL_StatusTypeDef ESP8266_TCP_MQTT_Link(char *internet_Address,char * mqtt_Port);
HAL_StatusTypeDef ESP8266_Subscribe(char * device_id,char * device_name);
HAL_StatusTypeDef ESP8266_Send_Data_OneNET_CIP(char * device_id,char * device_name,char *Number_Message,uint8_t Tem);
HAL_StatusTypeDef ESP8266_Send_Data(char * my_id,char *params_tem,int tem_data,char *params_humi,int hum_data);
HAL_StatusTypeDef ESP8266_MQTT_ACK(char * device_id,char * device_name,char * receive_id);
HAL_StatusTypeDef ESP8266_MQTT_Give_Data(void);
#endif
