/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
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
#include "app_threadx.h"
#include "tx_api.h"
#include "stm32f4xx_hal.h"  // Main HAL header
#include "stm32f4xx_hal_gpio.h"  // GPIO-specific HAL functions
#include "stm32f4xx_hal_def.h"   // Definitions used by HAL (e.g., HAL_StatusTypeDef)

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
// Thread control block and stack
TX_THREAD led_thread;
UCHAR led_thread_stack[LED_THREAD_STACK_SIZE];


// Thread entry function
void led_thread_entry(ULONG thread_input) {
	debug_message("LED Thread started.\r\n");
    while(1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        debug_message("LED toggled.\r\n");
        // Sleep for 50 ticks (~500ms at default 10 ticks/ms config)
        tx_thread_sleep(50);
    }
}

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  debug_message("App_ThreadX_Init started.\r\n");
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN App_ThreadX_Init */
  (void)byte_pool;
  /* USER CODE END App_ThreadX_Init */
  debug_message("ThreadX byte pool received.\r\n");
  return ret;
}

/**
  * @brief  MX_ThreadX_Init
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */
  /* USER CODE END  Before_Kernel_Start */
  debug_message("ThreadX initialization started.\r\n");
  tx_kernel_enter();
  debug_message("ThreadX initialization completed. This message should NOT appear.\r\n");
  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
