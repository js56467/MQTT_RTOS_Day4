#ifndef __DHT11_h__
#define __DHT11_h__
struct DHT11Data {
   int8_t humi_int;        // 湿度整数部分
   int8_t humi_deci;       // 湿度小数部分
   int8_t temp_int;        // 温度整数部分
   int8_t temp_deci;       // 温度小数部分
   int8_t check_sum;       // 校验和           
};
void SelectGPIOMode_DHT11(uint16_t GPIO_Mode , uint16_t GPIO_Pull);
int8_t DHT11_ReadByte(void);
uint8_t DHT11_ReadData(int *Hum,int *Tem);
void DHT11_Task(void *Params);

#endif
