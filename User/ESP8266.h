#ifndef __ESP8266_h
#define __ESP8266_h

#include "main.h"

#define ESP8266_STA_MODE 1
#define ESP8266_AP_MODE 2
#define ESP8266_AP_STA_MODE 3

HAL_StatusTypeDef ESP8266_Wait_Receive(void);
void ESP8266_Clean(void);
HAL_StatusTypeDef ESP8266_Send_Command(char *Command,char * Ret);
HAL_StatusTypeDef ESP8266_Send_Command_For_Get_IP(char *Command,char * Ret);
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
HAL_StatusTypeDef ESP8266_init(void);							
#endif
