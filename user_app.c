/*!*********************************************************************************************************************
@file user_app.c                                                                
@brief User's tasks / applications are written here.  This description
should be replaced by something specific to the task.

------------------------------------------------------------------------------------------------------------------------
GLOBALS
- NONE

CONSTANTS
- NONE

TYPES
- NONE

PUBLIC FUNCTIONS
- NONE

PROTECTED FUNCTIONS
- void UserApp1Initialize(void)
- void UserApp1Run(void)


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_<type>UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u8 G_u8UserAppFlags;                             /*!< @brief Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemTime1ms;                   /*!< @brief From main.c */
extern volatile u32 G_u32SystemTime1s;                    /*!< @brief From main.c */
extern volatile u32 G_u32SystemFlags;                     /*!< @brief From main.c */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_<type>" and be declared as static.
***********************************************************************************************************************/


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/*! @publicsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void TimeXus(u16 u16TimeXus_)

@brief
Sets Timer0 to count u16Microseconds_

Requires:
- Timer0 configured such that each timer tick is 1 microsecond

Promises:
- Pre-loads TMR0H:L to clock out desired period
- TMR0IF cleared
- Timer0 enabled

*/
void TimeXus(u16 u16TimeXus_)
{
  u16 u16Temp = 65535;
  
  /* Handle edge case */
  if(u16TimeXus_ == 0)
  {
      PIR3bits.TMR0IF = 1;
      return;
  }
  
  /* Disable the timer during config */
  T0CON0bits.EN = 0;
  
  /* Preload TMR0H and TMR0L based on u16TimeXus */
  u16Temp -= u16TimeXus_;
  TMR0H = (u8)( (u16Temp >> 8) & 0x00FF);
  TMR0L = (u8)( u16Temp & 0x00FF);
   
  /* Clear TMR0IF and enable Timer 0 */
  PIR3bits.TMR0IF = 0;
  T0CON0bits.EN = 1;
  
} /* end TimeXus() */


/*--------------------------------------------------------------------------------------------------------------------*/
/*! @protectedsection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/

/*!--------------------------------------------------------------------------------------------------------------------
@fn void UserAppInitialize(void)

@brief
Initializes the application's variables.

Should only be called once in main init section.

Requires:
- NONE

Promises:
- Start with RA6:0 low

*/
void UserAppInitialize(void)
{
    /* LED initialization */
    LATA &= 0xC0;
    
    /* Timer0 control register initialization to turn timer on, asynch mode, 16-bit
     * Fosc/4, 1:x prescaler, 1:1 postscaler  */
    T0CON0 = 0x90; // b'10010000'
    T0CON1 = 0x54; // b'01010100'

} /* end UserAppInitialize() */

  
/*!----------------------------------------------------------------------------------------------------------------------
@fn void UserAppRun(void)

@brief Application code that runs once per system loop

Requires:
- 

Promises:
- 

*/
void UserAppRun(void)
{
  u8 u8Temp;
  static u16 u16SecondCounter = 0;
  static u8 au8Pattern[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x02};
  static u8 u8PatternIndex = 0;

  u16SecondCounter++;
  if(u16SecondCounter == 100)
  {
    u16SecondCounter = 0;
    
    /* Update the LEDs */
   u8Temp = LATA;
   u8Temp &= 0xC0;
   u8Temp |= au8Pattern[u8PatternIndex];
   LATA = u8Temp;

   /* Update the pattern counter and wrap if at end of array */
   u8PatternIndex++;
   if(u8PatternIndex == sizeof(au8Pattern))
   {
     u8PatternIndex = 0;
   }
  }

} /* end UserAppRun() */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
