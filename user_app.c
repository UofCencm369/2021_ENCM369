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
- Start with RA7 high, all else low

*/
void UserAppInitialize(void)
{
    LATA  = 0x80;
 

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
  static u8 u8LedCounter = 0;
  u8 u8Temp;
  u32 u32DelayCounter;
  
  /* Update the counter and check overflow */
  u8LedCounter++;
  if(u8LedCounter == 0x40)
  {
    u8LedCounter = 0;
  }
  
  /* Update the PORTA LEDs being careful not to disturb RA7*/

  /* Option 1: clear the 6 LSBs then OR in the set bits of u8LedCounter.
   * The problem with this, though, is that all of RA0:RA5 go low for a moment
   * albeit a very short moment.  However, if those outputs were being watched,
   * that could potentially cause serious issues. */
#if 0
  LATA &= 0xC0;
  LATA |= u8LedCounter;
#endif
  /* Option 2: read the current values, update the bits of interest, then write them back. 
   * This takes more code and requires a temp variable, but not to worry. */
  u8Temp = PORTA;
  u8Temp &= 0xC0;
  u8Temp |= u8LedCounter;
  LATA = u8Temp;
  
  /* Add the delay
   * 16MHz clock, so 1/16MHz per instruction = 62.5ns / cycle.
   * 250ms / 62.5ns = 4,000,000 cycles.
   * 12 cycles per loop, so need 333,333 */
  u32DelayCounter = 333333;
  while(u32DelayCounter != 0)
  {
    u32DelayCounter--;
  }
  

} /* end UserAppRun */



/*------------------------------------------------------------------------------------------------------------------*/
/*! @privatesection */                                                                                            
/*--------------------------------------------------------------------------------------------------------------------*/





/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
