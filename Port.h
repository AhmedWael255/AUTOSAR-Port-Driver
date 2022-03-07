 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ahmed Wael
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR */
#define PORT_VENDOR_ID    (1000U)

/* Port Module Id */
#define PORT_MODULE_ID    (120U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION              (1U)
#define PORT_SW_MINOR_VERSION              (0U)
#define PORT_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_AR_RELEASE_PATCH_VERSION     (3U)

/* Standard AUTOSAR types */
#include "Std_Types.h"  

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif
   
 /*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

/* Non AUTOSAR files */
#include "Common_Macros.h"
   
   /******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/

/* Service ID for Port Initialization */
#define Port_Init_SID                   (uint8)0x00
   
/*Service ID for Port Pin Direction*/
#define Port_SetPinDirection_SID        (uint8)0x01
   
/*Service ID for Port Refresh Direction*/   
#define Port_RefreshPortDirection_SID   (uint8)0x02
   
/*Service ID for Port Version Info*/   
#define Port_GetVersionInfo_SID         (uint8)0x03
   
/*Service ID for Port Pin Mode*/
#define Port_SetPinMode_SID             (uint8)0x04
 
   
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/

/* Type definition for Port_PinType used by the PORT APIs */
typedef uint8 Port_PinType;

/* Type definition for Port_PinModeType used by the PORT APIs */
typedef uint8 Port_PinModeType;

/*Type definition for Port_PinInitValue used by the PORT APIs*/
typedef enum 
{
  PORT_Pin_LOGIC_LOW,
  PORT_PIN_LOGIC_HIGH,
}Port_PinInitValue;

/*Type definition for Port_PinDirectionType used by the PORT APIs*/
typedef enum  
{
  PORT_PIN_IN,
  PORT_PIN_OUT,
}Port_PinDirectionType;

/*Type definition for PORT_PinPullResistor used by the PORT APIs*/
typedef enum
{
  PORT_PIN_OFF,
  PORT_PIN_PUN,
  PORT_PIN_PDN,
}PORT_PinPullResistor;

typedef enum
{
  No_Change,
  Change,
}Port_PinChange;

/*Type definition for Port_PinInitMode used by the PORT APIs*/
typedef enum
{
  PORT_PIN_MODE_ADC,
  PORT_PIN_MODE_ALT1,
  PORT_PIN_MODE_ALT2,
  PORT_PIN_MODE_ALT3,
  PORT_PIN_MODE_ALT4,
  PORT_PIN_MODE_ALT5,
  PORT_PIN_MODE_ALT6,
  PORT_PIN_MODE_ALT7,
  PORT_PIN_MODE_ALT8,
  PORT_PIN_MODE_ALT9,
  PORT_PIN_MODE_GPIO,
  
}Port_PinInitMode;

/*Type definition Structure to Configure all the Port Pins used by PORT APIs*/
typedef struct 
{
  uint8 Port_Num;
  uint8 Pin_Num;
  Port_PinDirectionType Direction;
  Port_PinChange Pin_Change_Direction;
  Port_PinInitMode Pin_Mode;
  Port_PinChange Pin_Change_Mode;
  Port_PinInitValue Init_Value;
  PORT_PinPullResistor Pull_Resistor;
  
}Pin_Config;

typedef struct
{
    Pin_Config Pin[PORT_CONFIGURED_PINS];
    
}Port_ConfigType;

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/

/* Port_Init API service called with NULL pointer parameter */
#define PORT_E_PARAM_CONFIG             (uint8)0x10

   /* DET code to report Invalid Port */
#define PORT_E_PARAM_INVALID_PIN_ID    (uint8)0x14
   
   /*DET code to report Null pointer parameter*/
#define PORT_E_PARAM_POINTER            (uint8)0x15
   
   /*DET code to report Unchangable Mode*/
#define PORT_E_MODE_UNCHANGEABLE        (uint8)0x20
   
   /*DET code to report Unchangable Direction*/
#define PORT_E_DIRECTION_UNCHANGEABLE   (uint8)0x23
   
   /*DET code to report Invalid Pin*/
#define PORT_E_PARAM_PIN                (uint8)0x25       

/*
 * API service used without module initialization is reported using following
 * error code (Not exist in AUTOSAR 4.0.3 Port SWS Document.
 */
#define PORT_E_UNINIT                   (uint8)0xF0

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/*Initializes the Port Driver module*/
void Port_Init( const Port_ConfigType* ConfigPtr );

#if (Port_Set_Pin_Direction_Api == STD_ON)
/*Sets the port pin direction*/
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction );
#endif

/*Port_RefreshPortDirection shall refresh the direction of all configured ports to the configured direction (PortPinDirection)*/
void Port_RefreshPortDirection( void );

#if (PORT_VERSION_INFO_API == STD_ON)
/*Port_GetVersionInfo shall return the version information of this module*/
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
#endif

/*Port_SetPinMode shall set the port pin mode of the referenced pin during runtime*/
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode );

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_PinConfiguration;



#endif /* PORT_H */