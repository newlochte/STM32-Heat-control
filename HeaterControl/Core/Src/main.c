/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
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
#include "main.h"
#include "eth.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "pwm_devices_config.h"
#include "bmp2_config.h"
#include "pid_config.h"
#include <stdio.h>
#include "command_parser.h"
#include "led_config.h"
#include "encoder_config.h"
#include "i2c_lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PRECISION 1000
#define TRANSMIT_TIMEOUT 100
#define ENCODER_RESOLUTION 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint16_t response_len = 0;
uint8_t response_buffer[128];
uint8_t tx_buffer[12];
char lcd_buffer[2][16];
int tmp_int = 0;

float setpoint = 30;

double temp;
uint32_t prev_counter;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim4)
  {
    uint32_t counter = ENC_GetCounter(&henc1);
    // logic is flipped to have right movement incrementE
    if (prev_counter == 100 && counter == 0)
      setpoint--;
    if (prev_counter == 0 && counter == 100)
      setpoint++;
    if (prev_counter < counter)
      setpoint--;
    if (prev_counter > counter)
      setpoint++;

    if (counter != prev_counter)
    {
      tmp_int = PRECISION * setpoint;
      response_len = sprintf((char *)response_buffer, "Setpoint: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      HAL_UART_Transmit(&huart3, response_buffer, response_len, 100);
      prev_counter = counter;
    }
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim5)
  {
    if (BMP2_ReadData(&bmp2dev, NULL, &temp) < 0)
      response_len = sprintf((char *)response_buffer, "Failed to read sensor\r\n");
    else
    {
      tmp_int = PRECISION * temp;
      response_len = sprintf((char *)response_buffer, "Temperature: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
    }
    if (pid.state == PID_AUTO)
    {
      float error = setpoint - temp;
      float signal = PID_calulate_signal(&pid, error);
      if (signal > 0)
      {
        PWM_DEVICE_PWM_WriteDuty(&heater, signal * 100.0 / pid.abs_max_signal);
        PWM_DEVICE_PWM_WriteDuty(&cooler, 0);
      }
      else
      {
        PWM_DEVICE_PWM_WriteDuty(&cooler, -1 * signal * 100.0 / pid.abs_max_signal);
        PWM_DEVICE_PWM_WriteDuty(&heater, 0);
      }
      tmp_int = PRECISION * signal;
      response_len = sprintf((char *)response_buffer, "Signal: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
    }
    LED_DIO_On(&led_green);
    HAL_UART_Transmit(&huart3, response_buffer, response_len, TRANSMIT_TIMEOUT);
    LED_DIO_Off(&led_green);
  }
}

// Recive UART communication
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart3)
  {
    Command_TypeDef command = parse_commmand(tx_buffer, sizeof(tx_buffer));
    switch (command.type)
    {
    case BAD_ARG:
      response_len = sprintf((char *)response_buffer, "Złe polecenie \"%.8s\"\r\n", tx_buffer);
      break;
    case READ_TEMPERATURE:
      tmp_int = PRECISION * temp;
      response_len = sprintf((char *)response_buffer, "Temperature: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      break;
    case READ_FAN_PWM:
      tmp_int = PRECISION * PWM_DEVICE_PWM_ReadDuty(&cooler);
      response_len = sprintf((char *)response_buffer, "Fan duty: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      break;
    case READ_RESISTOR_PWM:
      tmp_int = PRECISION * PWM_DEVICE_PWM_ReadDuty(&heater);
      response_len = sprintf((char *)response_buffer, "Resistor duty: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      break;
    case READ_SETPOINT:
      tmp_int = PRECISION * setpoint;
      response_len = sprintf((char *)response_buffer, "Setpoint: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      break;
    case WRITE_FAN_PWM:
      tmp_int = PRECISION * command.value;
      response_len = sprintf((char *)response_buffer, "Fan duty set to: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      PWM_DEVICE_PWM_WriteDuty(&cooler, command.value);
      break;
    case WRITE_RESISTOR_PWM:
      tmp_int = PRECISION * command.value;
      response_len = sprintf((char *)response_buffer, "Resistor duty set to: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      PWM_DEVICE_PWM_WriteDuty(&heater, command.value);
      break;
    case WRITE_SETPOINT:
      tmp_int = PRECISION * command.value;
      response_len = sprintf((char *)response_buffer, "Setpoint set to: %2u.%03u\r\n", tmp_int / PRECISION, tmp_int % PRECISION);
      setpoint = command.value;
      break;
    case WRITE_PID_STATE:
      response_len = sprintf((char *)response_buffer, "PID state changed to: %2d\r\n", command.value ? PID_AUTO : PID_MANUAL);
      pid.state = command.value ? PID_AUTO : PID_MANUAL;
      break;
    default:
      response_len = sprintf((char *)response_buffer, "Reached Unreachable! Did you forget something?\r\n");
      break;
    }
    LED_DIO_On(&led_green);
    HAL_UART_Transmit(&huart3, response_buffer, response_len, TRANSMIT_TIMEOUT);
    LED_DIO_Off(&led_green);

    HAL_UART_Receive_IT(&huart3, tx_buffer, sizeof(tx_buffer));
  }
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_I2C1_Init();
  MX_USART3_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_SPI4_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */
  BMP2_Init(&bmp2dev);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Encoder_Start_IT(&htim4, TIM_CHANNEL_ALL);
  HAL_TIM_Base_Start_IT(&htim5);
  PWM_DEVICE_PWM_Init(&heater);
  PWM_DEVICE_PWM_Init(&cooler);
  ENC_Init(&henc1);
  HAL_UART_Receive_IT(&huart3, tx_buffer, sizeof(tx_buffer));
  lcd_init();
  lcd_clear();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    LED_DIO_Write(&led_red, (LED_DIO_State_TypeDef)PWM_DEVICE_PWM_ReadDuty(&heater) > 0);
    LED_DIO_Write(&led_blue, (LED_DIO_State_TypeDef)PWM_DEVICE_PWM_ReadDuty(&cooler) > 0);
    tmp_int = PRECISION * setpoint;
    sprintf((char *)lcd_buffer[0], "Setpoint: %2u.%02u", tmp_int / PRECISION, tmp_int % PRECISION);
    tmp_int = PRECISION * temp;
    sprintf((char *)lcd_buffer[1], "Temp: %2u.%03u", tmp_int / PRECISION, tmp_int % PRECISION);
    lcd_send_string_frame(lcd_buffer);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
   */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
