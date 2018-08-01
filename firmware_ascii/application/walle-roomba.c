/**********************************************************************************************************************
File: walle-roomba.c                                                                

----------------------------------------------------------------------------------------------------------------------

Description:
This file has code for making the enGENIUS camp robot drive around like a roomba using
an ultrasonic sensor

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void WR_Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void WR_RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_WR_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32WR_Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "WR_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type WR_StateMachine;            /* The state machine function pointer */
static u32 WR_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: WR_Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void WR_Initialize(void)
{
  WR_StateMachine = WR_SM_Idle;
 
} /* end WR_Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function WR_RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void WR_RunActiveState(void)
{
  WR_StateMachine();

} /* end WR_RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/





/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void WR_SM_Idle(void)
{
  if(IsButtonPressed(BUTTON0)){
    ANTRCC_R_SignalOn(TRIG);
    LedOn(PURPLE);
  }
  else{
    ANTRCC_R_SignalOff(TRIG);
    LedOff(PURPLE);
  }
  
} /* end WR_SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void WR_SM_Error(void)          
{
  
} /* end WR_SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
