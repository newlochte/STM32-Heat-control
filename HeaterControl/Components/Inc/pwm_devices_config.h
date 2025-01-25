/**
 ******************************************************************************
 * @file		  : heater_config.h
 * @author  	: AW		Adrian.Wojcik@put.poznan.pl
 * @version 	: 1.3.0
 * @date     : Nov 27, 2022
 * @brief   	: Electric heater components configuration file
 *
 ******************************************************************************
 */
#ifndef INC_HEATER_CONFIG_H_
#define INC_HEATER_CONFIG_H_

/* Public includes -----------------------------------------------------------*/
#include "pwm_device.h"

/* Public typedef ------------------------------------------------------------*/

/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
extern PWM_DEVICE_PWM_Handle_TypeDef heater;
extern PWM_DEVICE_PWM_Handle_TypeDef cooler;

/* Public function prototypes ------------------------------------------------*/

#endif /* INC_HEATER_CONFIG_H_ */
