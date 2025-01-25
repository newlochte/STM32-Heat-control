/**
 ******************************************************************************
 * @file		  : pwm_device.h
 * @author  	: AW		Adrian.Wojcik@put.poznan.pl
 * @version 	: 1.3.0
 * @date     : Nov 27, 2022
 * @brief    : Electric pwm_device components driver
 *
 ******************************************************************************
 */
#ifndef INC_PWM_DEVICE_H_
#define INC_PWM_DEVICE_H_

/* Public includes -----------------------------------------------------------*/
#include "dio.h"
#include "pwm.h"

/* Public typedef ------------------------------------------------------------*/
typedef enum
{
  PWM_DEVICE_ON_LOW,
  PWM_DEVICE_ON_HIGH
} PWM_DEVICE_ActiveState_TypeDef;
typedef enum
{
  PWM_DEVICE_OFF = 0,
  PWM_DEVICE_ON = 1
} PWM_DEVICE_DIO_State_TypeDef;

/**
 * @brief Digital output (On/Off) PWM_DEVICE handle structure definition
 */
typedef struct
{
  DIO_Handle_TypeDef Output;
  PWM_DEVICE_ActiveState_TypeDef ActiveState;
} PWM_DEVICE_DIO_Handle_TypeDef;

/**
 * @brief PWM output (0.-100.%) PWM_DEVICE handle structure definition
 */
typedef struct
{
  PWM_Handle_TypeDef Output;
  PWM_DEVICE_ActiveState_TypeDef ActiveState;
} PWM_DEVICE_PWM_Handle_TypeDef;

/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/
/**
 * @brief Digital output (On/Off) pwm_device handle structure initialization
 * @param[in] USER_NAME    : Pin user label set up in CubeMX (.ioc file)
 * @param[in] ACTIVE_STATE : Active state (polarity) of PWM_DEVICE
 *       This parameter can be one of the following values:
 *            @arg PWM_DEVICE_ON_LOW  : Pwm_device turns on if output state is 0
 *            @arg PWM_DEVICE_ON_HIGH : Pwm_device turns on if output state is 1
 */
#define PWM_DEVICE_DIO_INIT_HANDLE(USER_NAME, ACTIVE_STATE) \
  {                                                         \
      .Output = DIO_INIT_HANDLE(USER_NAME),                 \
      .ActiveState = ACTIVE_STATE}

/**
 * @brief @brief PWM output (0.-100.%) pwm_device handle structure initialization
 * @param[in] TIMER        : TIM handle
 *       This parameter can be one of the following values:
 *            @arg &htim1  : TIM1 (Advanced control timer, 6 channels)
 *            @arg &htim2  : TIM2 (General purpose timer, 4 channels)
 *            @arg &htim3  : TIM3 (General purpose timer, 4 channels)
 *            @arg &htim4  : TIM4 (General purpose timer, 4 channels)
 *            @arg &htim5  : TIM5 (General purpose timer, 4 channels)
 *            @arg &htim8  : TIM8 (Advanced control timer, 6 channels)
 *            @arg &htim9  : TIM9 (General purpose timer, 2 channels)
 *            @arg &htim10 : TIM10 (General purpose timer, 1 channel)
 *            @arg &htim11 : TIM11 (General purpose timer, 1 channel)
 *            @arg &htim12 : TIM12 (General purpose timer, 2 channels)
 *            @arg &htim13 : TIM13 (General purpose timer, 1 channel)
 *            @arg &htim14 : TIM14 (General purpose timer, 1 channel)
 * @param[in] CHANNEL      : TIM Channel
 *       This parameter can be one of the following values:
 *            @arg TIM_CHANNEL_1: TIM Channel 1 selected
 *            @arg TIM_CHANNEL_2: TIM Channel 2 selected
 *            @arg TIM_CHANNEL_3: TIM Channel 3 selected
 *            @arg TIM_CHANNEL_4: TIM Channel 4 selected
 *            @arg TIM_CHANNEL_5: TIM Channel 5 selected
 *            @arg TIM_CHANNEL_6: TIM Channel 6 selected
 * @param[in] ACTIVE_STATE : Active state (polarity) of PWM_DEVICE
 *       This parameter can be one of the following values:
 *            @arg PWM_DEVICE_ON_LOW  : Pwm_device turns on if output state is 0
 *            @arg PWM_DEVICE_ON_HIGH : Pwm_device turns on if output state is 1
 */
#define PWM_DEVICE_PWM_INIT_HANDLE(TIMER, CHANNEL, ACTIVE_STATE) \
  {                                                              \
      .Output = PWM_INIT_HANDLE((TIMER), CHANNEL),               \
      .ActiveState = ACTIVE_STATE}

/* Public variables ----------------------------------------------------------*/

/* Public function prototypes ------------------------------------------------*/

/**
 * @brief Turns pwm_device on
 * @param[in] hhtr		: Pwm_device DIO handler
 * @retval None
 */
void PWM_DEVICE_DIO_On(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr);

/**
 * @brief Turns pwm_device off
 * @param[in] hhtr		: Pwm_device DIO handler
 * @retval None
 */
void PWM_DEVICE_DIO_Off(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr);

/**
 * @brief Toggles pwm_device state
 * @param[in] hhtr		: Pwm_device DIO handler
 * @retval None
 */
void PWM_DEVICE_DIO_Toggle(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr);

/**
 * @brief Writes given pwm_device state
 * @param[in] hhtr		: Pwm_device DIO handler
 * @param[in] state 	: Pwm_device state (PWM_DEVICE_OFF or PWM_DEVICE_ON)
 * @retval None
 */
void PWM_DEVICE_DIO_Write(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr, PWM_DEVICE_DIO_State_TypeDef state);

/**
 * @brief Reads pwm_device state
 * @param[in] hhtr		: Pwm_device DIO handler
 * @retval	Pwm_device state (PWM_DEVICE_OFF or PWM_DEVICE_ON)
 */
PWM_DEVICE_DIO_State_TypeDef PWM_DEVICE_DIO_Read(const PWM_DEVICE_DIO_Handle_TypeDef *hhtr);

/**
 * @brief Initialize PWM pwm_device control
 * @param[in/out] hhtr   : Pwm_device PWM handler
 * @retval None
 */
void PWM_DEVICE_PWM_Init(PWM_DEVICE_PWM_Handle_TypeDef *hhtr);

/**
 * @brief Write PWM duty cycle
 * @param[in/out] hhtr   : Pwm_device PWM handler
 * @param[in]     duty   : PWM duty cycle in percents (0. - 100.)
 * @retval None
 */
void PWM_DEVICE_PWM_WriteDuty(PWM_DEVICE_PWM_Handle_TypeDef *hhtr, float duty);

/**
 * @brief Set PWM duty cycle
 * @param[in] hhtr   : Pwm_device PWM handler
 * @retval PWM duty cycle in percents (0. - 100.)
 */
float PWM_DEVICE_PWM_ReadDuty(const PWM_DEVICE_PWM_Handle_TypeDef *hhtr);

#endif /* INC_PWM_DEVICE_H_ */
