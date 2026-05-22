/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "UART_Task.h"
#include "LightSensor.h"
#include "OLED.h"
#include "DHT11.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* 创建一个检查各个任务占有CPU资源任务所需要的内存 */
static signed char pcWriteBuffer[200];
/* 创建串口任务栈大小 用于静态创建任务 */
static StackType_t  g_pucStackofUARTTask[128];
/* 创建光敏任务栈大小，用于静态创建任务 */
static StackType_t g_pucStackofLightSensorTask[128];
/* 创建温湿度任务栈大小，用于创建静态任务 */
static StackType_t g_pucStackofDHT11Task[128];
/* 创建串口任务的TCB结构体 用于静态创建任务 */
static StaticTask_t g_TCBofUARTTask;
/* 创建串口任务的TCB结构体，用于创建静态任务 */
static StaticTask_t g_TCBofLightSensorTask;
/* 创建温湿度传感器任务的TCB结构体,用于创建静态任务 */
static StaticTask_t g_TCBofDHT11Task;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */


/* 创建串口任务句柄 */
static TaskHandle_t UARTTaskHandle;
/* 创建光敏传感任务句柄 */
static TaskHandle_t LightSensorHandle;
/* 创建DHT11温湿度传感器任务句柄 */
static TaskHandle_t DHT11Handle;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{

   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
	vTaskList(pcWriteBuffer);
	printf("%s\r\n",pcWriteBuffer);
}
/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  
  
  /* add threads, ... */
  /* 创建串口打印任务 */
  UARTTaskHandle=xTaskCreateStatic(UART_Task,"PrintTask",128,NULL,osPriorityNormal,g_pucStackofUARTTask,&g_TCBofUARTTask);
  
  /* 创建光敏传感器任务 */
  LightSensorHandle=xTaskCreateStatic(LightSensor_Task,"LightSensorTask",128,NULL,osPriorityNormal,g_pucStackofLightSensorTask,&g_TCBofLightSensorTask);
  
  /* 创建温湿度传感任务 */
  DHT11Handle=xTaskCreateStatic(DHT11_Task,"DHT11Rask",128,NULL,osPriorityNormal,g_pucStackofDHT11Task,&g_TCBofDHT11Task);
  
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

