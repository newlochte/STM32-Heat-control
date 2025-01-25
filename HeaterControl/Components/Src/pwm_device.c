/**
 ******************************************************************************
 * @file     : pwm_device.c
 * @author   : AW    Adrian.Wojcik@put.poznan.pl
 * @version  : 1.3.0
 * @date     : Nov 27, 2022
 * @brief    : Electric pwm_device components driver
 *
 ******************************************************************************
 */

/* Private includes ----------------------------------------------------------*/
#include "pwm_device.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/
/**
 * @brief Turns pwm_device on
 * @param[in] hhtr   : Pwm_device DIO handler
 * @retval None
 */
void PWM_DEVICE_DIO_On(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr)
{
  PWM_DEVICE_DIO_Write(hhtr, PWM_DEVICE_ON);
}

/**
 * @brief Turns pwm_device off
 * @param[in] hhtr   : Pwm_device DIO handler
 * @retval None
 */
void PWM_DEVICE_DIO_Off(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr)
{
  PWM_DEVICE_DIO_Write(hhtr, PWM_DEVICE_OFF);
}

/**
 * @brief Toggles pwm_device state
 * @param[in] hhtr   : Pwm_device DIO handler
 * @retval None
 */
void PWM_DEVICE_DIO_Toggle(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr)
{
  DIO_Toggle(&(hhtr->Output));
}

/**
 * @brief Writes given pwm_device state
 * @param[in] hhtr   : Pwm_device DIO handler
 * @param[in] state  : Pwm_device state (PWM_DEVICE_OFF or PWM_DEVICE_ON)
 * @retval None
 */
void PWM_DEVICE_DIO_Write(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr, PWM_DEVICE_DIO_State_TypeDef state)
{
  DIO_Write(&(hhtr->Output), (hhtr->ActiveState == PWM_DEVICE_ON_HIGH) ? state : !state);
}

/**
 * @brief Reads pwm_device state
 * @param[in] hhtr   : Pwm_device GPIO handler
 * @retval Pwm_device state (PWM_DEVICE_OFF or PWM_DEVICE_ON)
 */
PWM_DEVICE_DIO_State_TypeDef PWM_DEVICE_DIO_Read(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr)
{
  _Bool state = DIO_Read(&(hhtr->Output));
  return (hhtr->ActiveState == PWM_DEVICE_ON_HIGH) ? state : !state;
}

/**
 * @brief Initialize PWM pwm_device control
 * @param[in] hhtr   : Pwm_device PWM handler
 * @retval None
 */
void PWM_DEVICE_PWM_Init(PWM_DEVICE_PWM_Handle_TypeDef *hhtr)
{
  hhtr->Output.Duty = (hhtr->ActiveState == PWM_DEVICE_ON_HIGH) ? (hhtr->Output.Duty) : (100.0f - hhtr->Output.Duty);
  PWM_Init(&(hhtr->Output));
}

/**
 * @brief Write PWM duty cycle
 * @param[in/out] hhtr   : Pwm_device PWM handler
 * @param[in]     duty   : PWM duty cycle in percents (0. - 100.)
 * @retval None
 */
void PWM_DEVICE_PWM_WriteDuty(PWM_DEVICE_PWM_Handle_TypeDef *hhtr, float duty)
{
  hhtr->Output.Duty = (hhtr->ActiveState == PWM_DEVICE_ON_HIGH) ? (duty) : (100.0f - duty);
  PWM_WriteDuty(&(hhtr->Output), hhtr->Output.Duty);
}

/**
 * @brief Set PWM duty cycle
 * @param[in] hhtr   : Pwm_device PWM handler
 * @retval PWM duty cycle in percents (0. - 100.)
 */
float PWM_DEVICE_PWM_ReadDuty(const PWM_DEVICE_PWM_Handle_TypeDef *hhtr)
{
  return (hhtr->ActiveState == PWM_DEVICE_ON_HIGH) ? (hhtr->Output.Duty) : (100.0f - hhtr->Output.Duty);
}
