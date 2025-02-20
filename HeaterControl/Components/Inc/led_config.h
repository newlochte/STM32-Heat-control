/**
 ******************************************************************************
 * @file		  : led_config.h
 * @author  	: AW		Adrian.Wojcik@put.poznan.pl
 * @version 	: 1.3.1
 * @date     : Nov 27, 2022
 * @brief   	: Single-color LED components configuration file
 *
 ******************************************************************************
 */
#ifndef INC_LED_CONFIG_H_
#define INC_LED_CONFIG_H_

/* Public includes -----------------------------------------------------------*/
#include "led.h"

/* Public typedef ------------------------------------------------------------*/

/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
extern LED_DIO_Handle_TypeDef led_green; //!< Green on-board LED
extern LED_DIO_Handle_TypeDef led_blue;  //!< Blue on-board LED
extern LED_DIO_Handle_TypeDef led_red;   //!< Red on-board LED

/* Public function prototypes ------------------------------------------------*/

#endif /* INC_LED_CONFIG_H_ */
