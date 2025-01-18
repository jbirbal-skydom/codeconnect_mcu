#include "main.h"

/* Private variables */
UART_HandleTypeDef huart2;

/* Private function prototypes */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
char msg[] = "Hello, UART!\r\n";

/* Custom delay function */
void delay(volatile uint32_t delay_count) {
    while (delay_count--) {
        __asm("NOP");  // No operation, just waste cycles
    }
}

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* MCU Configuration--------------------------------------------------------*/
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();

    MX_USART2_UART_Init();


    /* Infinite loop */
    while (1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

        delay(500000);           // Custom delay 
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /* Initialize the RCC Oscillators */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* Initialize the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{
    // Make sure GPIOA clock is enabled (if not already done in GPIO_Init)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    // Enable USART2 clock
    __HAL_RCC_USART2_CLK_ENABLE();

    // Small delay after clock enable
    volatile uint32_t dummy = RCC->AHB1ENR;
    (void)dummy;

    // Configure UART GPIO pins
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;  // PA2 (TX) and PA3 (RX)
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       // Alternate function - Push pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;  // Set alternate function to USART2
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Verify GPIO configuration for PA2 (TX) and PA3 (RX)
    // Check if pins are in alternate function mode (0b10)
    if ((GPIOA->MODER & (3U << (2 * 2))) != (2U << (2 * 2)) ||
        (GPIOA->MODER & (3U << (3 * 2))) != (2U << (3 * 2))) {
        // Set PA2 and PA3 to alternate function mode
        GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (3 * 2)));
        GPIOA->MODER |= (2U << (2 * 2)) | (2U << (3 * 2));
    }

    // Verify alternate function configuration
    if ((GPIOA->AFR[0] & (0xF << (2 * 4))) != (7U << (2 * 4)) ||
        (GPIOA->AFR[0] & (0xF << (3 * 4))) != (7U << (3 * 4))) {
        // Set alternate function 7 (USART2) for PA2 and PA3
        GPIOA->AFR[0] &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
        GPIOA->AFR[0] |= (7U << (2 * 4)) | (7U << (3 * 4));
    }

    // Configure UART
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // Small delay after clock enable
    volatile uint32_t dummy = RCC->AHB1ENR;
    (void)dummy;
    
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    
    GPIO_InitStruct.Pin = GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    // Verify configuration
    if ((GPIOA->MODER & (3U << (5 * 2))) != (1U << (5 * 2))) {
        GPIOA->MODER &= ~(3U << (5 * 2));
        GPIOA->MODER |= (1U << (5 * 2));
    }
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5) {
        HAL_IncTick();
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}

#ifdef  USE_FULL_ASSERT 
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_failed error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_failed line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number */
}
#endif /* USE_FULL_ASSERT */