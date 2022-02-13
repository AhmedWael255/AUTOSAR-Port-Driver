 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ahmed Wael Hamed
 ******************************************************************************/

#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION                       (1U)
#define PORT_CFG_SW_MINOR_VERSION                       (0U)
#define PORT_CFG_SW_PATCH_VERSION                       (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION               (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION               (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION               (3U)
   
/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                           (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                           (STD_OFF)

/* Pre-compile option for Pin Direction Info API */
#define Port_SET_PIN_DIRECTION_API                      (STD_ON)

#define PORT_PIN_MODE_CHANGEABLE                        (FALSE)

/*NUmber of Pins in the MCU*/
#define PORT_CONFIGURED_PINS                            (43U)

/*The First Pin*/
#define PIN_MIN_NUMBER                                  (0U)

/*TM4C Ports*/
#define PORT_PORTA                                      (0U)   
#define PORT_PORTB                                      (1U) 
#define PORT_PORTC                                      (2U) 
#define PORT_PORTD                                      (3U) 
#define PORT_PORTE                                      (4U) 
#define PORT_PORTF                                      (5U)

/*TM4C Pins*/
#define PORT_PIN0                                       (0U)
#define PORT_PIN1                                       (1U)
#define PORT_PIN2                                       (2U)
#define PORT_PIN3                                       (3U)
#define PORT_PIN4                                       (4U)
#define PORT_PIN5                                       (5U)
#define PORT_PIN6                                       (6U)
#define PORT_PIN7                                       (7U)

#endif