/**
 ******************************************************************************
 * @file		  : pwm_device_config.c
 * @author  	: AW		Adrian.Wojcik@put.poznan.pl
 * @version 	: 1.3.0
 * @date     : Nov 27, 2022
 * @brief    : Electric pwm_device components configuration file
 *
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "pwm_device.h"
#include "tim.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
PWM_DEVICE_PWM_Handle_TypeDef heater = PWM_DEVICE_PWM_INIT_HANDLE(&htim3, TIM_CHANNEL_1, PWM_DEVICE_ON_HIGH);
PWM_DEVICE_PWM_Handle_TypeDef cooler = PWM_DEVICE_PWM_INIT_HANDLE(&htim3, TIM_CHANNEL_2, PWM_DEVICE_ON_HIGH);

/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
