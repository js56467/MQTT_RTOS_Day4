#include "Attitude_Alarm_Task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "MPU6050.h"
#include "Buzzer.h"
#include "semphr.h"
/* 创建蜂鸣器任务栈大小，用于静态创建任务 */
static StackType_t g_pucStackofBuzzerTask[128];
/* 创建MPU6050任务栈大小，用于创建静态任务 */
static StackType_t g_pucStackofMPU6050Task[128];
/* 创建蜂鸣器任务的TCB结构体 用于静态创建任务 */
static StaticTask_t g_TCBofBuzzerTask;
/* 创建MPU6050任务的TCB结构体,用于创建静态任务 */
static StaticTask_t g_TCBofMPU6050Task;
/* 创建蜂鸣器任务句柄 */
static TaskHandle_t BuzzerHandle;
/* 创建MPU6050任务句柄 */
static TaskHandle_t MPU6050Handle;

/* 创建信号量句柄 */
SemaphoreHandle_t AATSemaphoreHandle;

/* 创建一个互斥量句柄 */
SemaphoreHandle_t MPU6050_I2C_Mutex;



void Attitude_Alarm_Task(void * params){
	/* 创建互斥量 */
    MPU6050_I2C_Mutex = xSemaphoreCreateMutex();
	/* 创建一个信号量 */
	AATSemaphoreHandle=xSemaphoreCreateBinary();
  /* 创建蜂鸣器任务 */
  BuzzerHandle=xTaskCreateStatic(Buzzer_Task,"BuzzerTask",128,NULL,osPriorityNormal,g_pucStackofBuzzerTask,&g_TCBofBuzzerTask);
  /* 创建MPU6050任务 */
  MPU6050Handle=xTaskCreateStatic(MPU6050_Task,"MPU6050_Task",128,NULL,osPriorityNormal,g_pucStackofMPU6050Task,&g_TCBofMPU6050Task);
  while(1){
  
  }
}