/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

#include "bridge.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


extern bridge_t Bridge;
extern UART_HandleTypeDef huart4;
extern SPI_HandleTypeDef hspi1;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId UARTHandlerTaskHandle;
osThreadId SPIHandlerTaskHandle;
osMessageQId UARTRxQueueHandle;
osMessageQId SPIRxQueueHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */



/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void StartUARTHandlerTask(void const * argument);
void StartSPIHandlerTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

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

  /* Create the queue(s) */
  /* definition and creation of UARTRxQueue */
  osMessageQDef(UARTRxQueue, 10, msg_t*);
  UARTRxQueueHandle = osMessageCreate(osMessageQ(UARTRxQueue), NULL);

  /* definition and creation of SPIRxQueue */
  osMessageQDef(SPIRxQueue, 10, msg_t*);
  SPIRxQueueHandle = osMessageCreate(osMessageQ(SPIRxQueue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of UARTHandlerTask */
  osThreadDef(UARTHandlerTask, StartUARTHandlerTask, osPriorityNormal, 0, 256);
  UARTHandlerTaskHandle = osThreadCreate(osThread(UARTHandlerTask), NULL);

  /* definition and creation of SPIHandlerTask */
  osThreadDef(SPIHandlerTask, StartSPIHandlerTask, osPriorityNormal, 0, 256);
  SPIHandlerTaskHandle = osThreadCreate(osThread(SPIHandlerTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartUARTHandlerTask */
/**
* @brief Function implementing the UARTHandlerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUARTHandlerTask */
void StartUARTHandlerTask(void const * argument)
{
  /* USER CODE BEGIN StartUARTHandlerTask */


  /* Infinite loop */
  for(;;)
  {
	  if( xQueueReceive(SPIRxQueueHandle, &Bridge.uart_tx_msg, portMAX_DELAY) )
	  {
		  if( HAL_UART_STATE_BUSY_TX != HAL_UART_GetState(&huart4) )
		  {
			  HAL_UART_Transmit_IT(&huart4, Bridge.uart_tx_msg->data, Bridge.uart_tx_msg->size);
		  }
	  }

    osDelay(1);
  }
  /* USER CODE END StartUARTHandlerTask */
}

/* USER CODE BEGIN Header_StartSPIHandlerTask */
/**
* @brief Function implementing the SPIHandlerTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartSPIHandlerTask */
void StartSPIHandlerTask(void const * argument)
{
  /* USER CODE BEGIN StartSPIHandlerTask */
  /* Infinite loop */
  for(;;)
  {
	  //tx empty
	  if(Bridge.spi_tx_msg == Bridge.no_data_msg)
	  {
		  xQueueReceive(UARTRxQueueHandle, &Bridge.spi_tx_msg, portMAX_DELAY);	//get new tx msg
	  }


    osDelay(1);
  }
  /* USER CODE END StartSPIHandlerTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	while(1)
	{

	}
}

//void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	if(&hspi1 == hspi)
	{
		//rx check
		if( '\0' == Bridge.spi_rx_msg->data[Bridge.spi_rx_msg->size] )
		{
			//check if not empty
			if( 0 != Bridge.spi_rx_msg->size )
			{
				//push in queue msg and get new ptr
				xQueueSendFromISR(SPIRxQueueHandle, &Bridge.spi_rx_msg, pdFALSE);
				Bridge.spi_rx_msg = getNewMsgPtr(Bridge.MAX_MSG_LENGHT);
			}
		}
		else
		{
			Bridge.spi_rx_msg->size++;	//read next byte
			if(Bridge.spi_rx_msg->size > Bridge.MAX_MSG_LENGHT) Bridge.spi_rx_msg->size = 0;	//over
		}

		//transmitting real message
		if(Bridge.spi_tx_msg != Bridge.no_data_msg)
		{
			//tx done check
			if( Bridge.spi_tx_msg->tx_index >= Bridge.spi_tx_msg->size )
			{
				freeMsgPtr(Bridge.spi_tx_msg);
				Bridge.spi_tx_msg = Bridge.no_data_msg;
			}
			else
			{
				Bridge.spi_tx_msg->tx_index++;
			}
		}

		//rx tx next byte
		HAL_SPI_TransmitReceive_IT(&hspi1,
								&(Bridge.spi_tx_msg->data[Bridge.spi_tx_msg->tx_index]),
								&(Bridge.spi_rx_msg->data[Bridge.spi_rx_msg->size]),
								1);
	}
}



void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	freeMsgPtr(Bridge.uart_tx_msg);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == &huart4)
	{
		if('\0' == Bridge.uart_rx_msg->data[Bridge.uart_rx_msg->size])
		{
			//check if not empty
			if( 0 != Bridge.uart_rx_msg->size )
			{
				//push in queue msg and get new ptr
				xQueueSendFromISR(UARTRxQueueHandle, &Bridge.uart_rx_msg, pdFALSE);
				Bridge.uart_rx_msg = getNewMsgPtr(Bridge.MAX_MSG_LENGHT);

				//TODO add memory optimization for small message
			}
		}
		else
		{
			Bridge.uart_rx_msg->size++;
			if(Bridge.uart_rx_msg->size > Bridge.MAX_MSG_LENGHT) Bridge.uart_rx_msg->size = 0;	//over
		}

		HAL_UART_Receive_IT(&huart4, &(Bridge.uart_rx_msg->data[Bridge.uart_rx_msg->size]), 1);
	}
}

/* USER CODE END Application */
