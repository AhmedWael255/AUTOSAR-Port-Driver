 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Ahmed Wael
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and PORT Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif
   
STATIC const Port_ConfigType* Port_PinConfigPtr = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non-Reentrant
* Parameters (in): ConfigPtr - Pointer to configuration set.
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: -Initialize ALL ports and port pins with the configuration set pointed to by the parameter ConfigPtr:
*              -Initialize all configured resources
************************************************************************************/

void Port_Init( const Port_ConfigType* ConfigPtr )
{
  
    #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == ConfigPtr)
	{
		Det_ReportError(PORT_MODULE_ID,
                                PORT_INSTANCE_ID,
                                Port_Init_SID,
                                PORT_E_PARAM_CONFIG);
	}
	else
        {
          /* Do Nothing */
        }
#endif
        
          Port_Status = PORT_INITIALIZED;
          Port_PinConfigPtr = ConfigPtr;
            
          for(Port_PinType idx = PIN_MIN_NUMBER; idx < PORT_CONFIGURED_PINS; idx++)
          {
            volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
            volatile uint32 delay = 0;
            
           switch(Port_PinConfigPtr->Pin[idx].Port_Num)
           {
               case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                        break;
               case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                        break;
               case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                        break;
               case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                        break;
               case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                        break;
               case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                        break;
           }
           
           /* Enable clock for PORT and allow time for clock to start*/
		SYSCTL_REGCGC2_REG |= (1<<Port_PinConfigPtr->Pin[idx].Port_Num);
		delay = SYSCTL_REGCGC2_REG;
           
           if( ((Port_PinConfigPtr->Pin[idx].Port_Num == 3) && (Port_PinConfigPtr->Pin[idx].Pin_Num == 7)) || ((Port_PinConfigPtr->Pin[idx].Port_Num == 5) && (Port_PinConfigPtr->Pin[idx].Pin_Num == 0)) ) /* PD7 or PF0 */
           {
              *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , ConfigPtr->Pin[idx].Pin_Num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            }
          else if( (Port_PinConfigPtr->Pin[idx].Port_Num == 2) && (Port_PinConfigPtr->Pin[idx].Pin_Num <= 3) ) /* PC0 to PC3 */
          {
              /* Do Nothing ...  this is the JTAG pins */
              continue;
          }
          else
          {
              /* Do Nothing ... No need to unlock the commit register for this pin */
          }
          
           
           /*Configure the Mode of the Pin*/
              switch(Port_PinConfigPtr->Pin[idx].Pin_Mode)
              {
              case PORT_PIN_MODE_ADC:
                
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);      /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);     /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);             /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000F << (Port_PinConfigPtr->Pin[idx].Pin_Num * 4));             /* Set the PMCx bits for this pin */
              break;
                
                case PORT_PIN_MODE_ALT1:
                case PORT_PIN_MODE_ALT2:
                case PORT_PIN_MODE_ALT3:
                case PORT_PIN_MODE_ALT4:
                case PORT_PIN_MODE_ALT5:
                case PORT_PIN_MODE_ALT6:
                case PORT_PIN_MODE_ALT7:
                case PORT_PIN_MODE_ALT8:
                case PORT_PIN_MODE_ALT9:
                  
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);               /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PinConfigPtr->Pin[idx].Pin_Num * 4));     /* Clear the PMCx bits for this pin */
                break;
                
                case PORT_PIN_MODE_GPIO:
                  
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PinConfigPtr->Pin[idx].Pin_Num * 4));     /* Clear the PMCx bits for this pin */
                    
                break;
                
                if(Port_PinConfigPtr->Pin[idx].Direction == PORT_PIN_OUT)
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                   
                  if(Port_PinConfigPtr->Pin[idx].Init_Value == STD_HIGH)
                  {
                      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                  }
                  else
                  {
                      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                  }
                 }
                else if(Port_PinConfigPtr->Pin[idx].Direction == PORT_PIN_IN)
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
                     
                    if(Port_PinConfigPtr->Pin[idx].Pull_Resistor == PORT_PIN_PUN)
                    {
                        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                    }
                    else if(Port_PinConfigPtr->Pin[idx].Pull_Resistor == PORT_PIN_PDN)
                    {
                       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);     /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                    }
                    else
                    {
                        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);     /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);   /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                    }
                }
                else
                {
                    /* Do Nothing */
                }
                
              }
          }

}


/************************************************************************************
* Service Name: Port_SetPinDirection    
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): -Pin Port    Pin ID number
                   -Direction   Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: -Sets the port pin direction
************************************************************************************/

#if (Port_Set_Pin_Direction_Api == STD_ON)
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction )
{
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (Port_Status == PORT_NOT_INITIALIZED)
	{
            Det_ReportError(PORT_MODULE_ID,
                            PORT_INSTANCE_ID,
                            Port_SetPinDirection_SID,
                            PORT_E_UNINIT);
	}
        
	else
        {
          /* Do Nothing..... */
        }
        
        /* check if the the Pin is Valid */
        if(Pin >= PORT_CONFIGURED_PINS || Pin < PIN_MIN_NUMBER)
        {
            Det_ReportError(PORT_MODULE_ID,
                            PORT_INSTANCE_ID,
                            Port_SetPinDirection_SID,
                            PORT_E_PARAM_INVALID_PIN_ID);
        }
        
        else
        {
          /* Do Nothing */
        }
        
        /* check if the Pin Direction is Unchangeable or not */
        if(Port_PinConfigPtr->Pin[Pin].Pin_Change_Direction == No_Change)
        {
          Det_ReportError(PORT_MODULE_ID,
                          PORT_INSTANCE_ID,
                          Port_SetPinDirection_SID,
                          PORT_E_DIRECTION_UNCHANGEABLE);
          return;
        }
        
        else
        {
          /* Do Nothing */
        }

#endif
        
          volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
       

         
          switch(Port_PinConfigPtr->Pin[Pin].Port_Num)
          {
               case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                        break;
               case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                        break;
               case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                        break;
               case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                        break;
               case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                        break;
               case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                        break;
          }

          if(Direction == PORT_PIN_OUT)
          {
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */                 
          }
                           
          else if(Direction == PORT_PIN_IN)
          {
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
          }
          
          else
          {
            /* Do Nothing */
          }    
        }
}
#endif


/************************************************************************************
* Service Name: Port_RefreshPortDirection    
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: -Refreshes port direction.
************************************************************************************/

void Port_RefreshPortDirection( void )
{
      #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
      if (Port_Status == PORT_NOT_INITIALIZED)
      {
        Det_ReportError(PORT_MODULE_ID,
                        PORT_INSTANCE_ID,
                        Port_RefreshPortDirection_SID,
                        PORT_E_UNINIT);
      }
      else
      {
        /* Do Nothing */
      }
        
#endif
      
        for(Port_PinType idx = PIN_MIN_NUMBER; idx < PORT_CONFIGURED_PINS; idx++)
          {
            volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
            
           switch(Port_PinConfigPtr->Pin[idx].Port_Num)
           {
               case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                        break;
               case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                        break;
               case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                        break;
               case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                        break;
               case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                        break;
               case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                        break;
           }
           
             if(Port_PinConfigPtr->Pin[idx].Pin_Change_Direction == Change)
             {
               /*Do Nothing*/
             }
             else
             {
                if(Port_PinConfigPtr->Pin[idx].Direction == PORT_PIN_OUT)
                {
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
                }
                else if(Port_PinConfigPtr->Pin[idx].Direction == PORT_PIN_IN)
                {
                   CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , Port_PinConfigPtr->Pin[idx].Pin_Num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */    
                }
             }
           
          }
      
        
}

/************************************************************************************
* Service Name: Port_GetVersionInfo    
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): Version info -Pointer to where to store the version information of this module.
* Return value: None
* Description: -Refreshes port direction.
************************************************************************************/

#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
      #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
	if (NULL_PTR == versioninfo)
	{
		Det_ReportError(PORT_MODULE_ID,
                                PORT_INSTANCE_ID,
                                Port_GetVersionInfo_SID,
                                PORT_E_PARAM_POINTER);
	}
	else
        {
          /* Do Nothing */
        }
#endif
        
  	/* Copy the vendor Id */
	versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
	/* Copy the module Id */
	versioninfo->moduleID = (uint16)PORT_MODULE_ID;
	/* Copy Software Major Version */
	versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
	/* Copy Software Minor Version */
	versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
	/* Copy Software Patch Version */
	versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
        }
	
}
#endif

/************************************************************************************
* Service Name: Port_SetPinMode    
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): PIN  -Port Pin ID number
                   Mode -New Port Pin mode to be set on port pin.
* Parameters (inout): None
* Parameters (out): Version info -Pointer to where to store the version information of this module.
* Return value: None
* Description: -Sets the port pin mode..
************************************************************************************/

void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )
{
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
	/* check if the input configuration pointer is not a NULL_PTR */
      if (Port_Status == PORT_NOT_INITIALIZED)
      {
        Det_ReportError(PORT_MODULE_ID, 
                        PORT_INSTANCE_ID,
                        Port_RefreshPortDirection_SID,
                        PORT_E_UNINIT);
      }
      
      else
      {
        /* Do Nothing */
      }
      
      
        /* check if the Pin Number is invalid */
      if (Pin < PIN_MIN_NUMBER || Pin > PORT_CONFIGURED_PINS)
      {
        Det_ReportError(PORT_MODULE_ID,
                        PORT_INSTANCE_ID,
                        Port_SetPinMode_SID,
                        PORT_E_PARAM_PIN);
      }
      
      else 
      {
        /* Do Nothing */
      }
      
              /* check if the Pin Mode is Unchangeable or not */
        if(Port_PinConfigPtr->Pin[Pin].Pin_Change_Mode == No_Change)
        {
          Det_ReportError(PORT_MODULE_ID,
                          PORT_INSTANCE_ID,
                          Port_SetPinMode_SID,
                          PORT_E_MODE_UNCHANGEABLE);
          return;
        }
        
        else
        {
          /* Do Nothing */
        }
      
#endif
      
         volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
            
           switch(Port_PinConfigPtr->Pin[Pin].Port_Num)
           {
               case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                        break;
               case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                        break;
               case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                        break;
               case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                        break;
               case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                        break;
               case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                        break;
           }
            switch(Mode)
            {
              case PORT_PIN_MODE_ADC:
                
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);      /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);     /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);             /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                    *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (0x0000000F << (Port_PinConfigPtr->Pin[Pin].Pin_Num * 4));             /* Set the PMCx bits for this pin */
              break;
                
              case PORT_PIN_MODE_ALT1:
              case PORT_PIN_MODE_ALT2:
              case PORT_PIN_MODE_ALT3:
              case PORT_PIN_MODE_ALT4:
              case PORT_PIN_MODE_ALT5:
              case PORT_PIN_MODE_ALT6:
              case PORT_PIN_MODE_ALT7:
              case PORT_PIN_MODE_ALT8:
              case PORT_PIN_MODE_ALT9:
                  
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);               /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                  *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PinConfigPtr->Pin[Pin].Pin_Num * 4));     /* Clear the PMCx bits for this pin */
              break;
                
              case PORT_PIN_MODE_GPIO:
                  
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
                  SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);         /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
                  CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , Port_PinConfigPtr->Pin[Pin].Pin_Num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
                  *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Port_PinConfigPtr->Pin[Pin].Pin_Num * 4));     /* Clear the PMCx bits for this pin */
              break;
            }
           
      
}





