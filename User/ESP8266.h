#ifndef __ESP8266_h
#define __ESP8266_h

#include "main.h"

#define ESP8266_STA_MODE 1
#define ESP8266_AP_MODE 2
#define ESP8266_AP_STA_MODE 3

HAL_StatusTypeDef ESP8266_Wait_Receive(void);
void ESP8266_Clean(void);
HAL_StatusTypeDef ESP8266_Send_Command(char *Command,char * Ret);
HAL_StatusTypeDef ESP8266_AT_Test(void);
HAL_StatusTypeDef ESP8266_SW_Reset(void);
HAL_StatusTypeDef ESP8266_Set_Mode(uint8_t Mode);
void ESP8266_Task(void *Params);

#endif
