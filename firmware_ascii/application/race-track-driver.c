/**********************************************************************************************************************
File: race-track-driver.c                                                                

----------------------------------------------------------------------------------------------------------------------

Description:
This file has code for making the enGENIUS camp robot drive around the race track 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void RT_DriverInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void RT_DriverRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_RT_Driver"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32RT_DriverFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "RT_Driver_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type RT_Driver_StateMachine;            /* The state machine function pointer */
static u32 RT_Driver_u32Timeout;                      /* Timeout counter used across states */

static u8 RT_Driver_Directions [32] = {0,5,3,1,3,1,3,1,3,1,4,1,4,1,6,1,5,4,1,3,1,3,1,3,1,6,1,5,1,3,1,0};

static u32 RT_Driver_Times [32] = 
{3000,
 FWD_15M,
 TURN_90,
 FWD_15M,
 TURN_30,
 FWD_1M,
 TURN_90,
 FWD_05M,
 TURN_30,
 FWD_05M,
 TURN_90,
 FWD_05M,
 TURN_30,
 FWD_15M,
 FWD_05M,
 FWD_05M,
 FWD_05M,
 TURN_90,
 FWD_1M,
 TURN_30,
 FWD_1M,
 TURN_90,
 FWD_05M,
 TURN_90,
 FWD_05M,
 FWD_05M,
 FWD_15M,
 FWD_05M,
 FWD_15M,
 TURN_90,
 FWD_2M,
 1
 };

static u32 RT_Driver_Timer;

static u8 RT_Driver_Index;

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
Function: RT_DriverInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void RT_DriverInitialize(void)
{
  
  RT_Driver_Index = 0;
  RT_Driver_Timer = 0;
  
  RT_Driver_StateMachine = RT_DriverSM_Idle;
 
} /* end RT_DriverInitialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function RT_DriverRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void RT_DriverRunActiveState(void)
{
  RT_Driver_StateMachine();

} /* end RT_DriverRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/





/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void RT_DriverSM_Idle(void)
{
  
  if(RT_Driver_Timer < RT_Driver_Times[RT_Driver_Index])
  {
    
    if(RT_Driver_Directions[RT_Driver_Index] == 1)
    {
      ANTRCC_R_Forward();
    }
    else if(RT_Driver_Directions[RT_Driver_Index] == 2)
    {
      ANTRCC_R_Backward();
    }
    else if(RT_Driver_Directions[RT_Driver_Index] == 3)
    {
      ANTRCC_R_LeftTurn();
    }
    else if(RT_Driver_Directions[RT_Driver_Index] == 4)
    {
      ANTRCC_R_RightTurn();
    }
    else if(RT_Driver_Directions[RT_Driver_Index] == 5)
    {
      ANTRCC_R_ForwardLeft();
    }
    else if(RT_Driver_Directions[RT_Driver_Index] == 6)
    {
      ANTRCC_R_ForwardRight();
    }
    else if(RT_Driver_Directions[RT_Driver_Index] == 7)
    {
      ANTRCC_R_BackwardLeft();
    }
    else if(RT_Driver_Directions[RT_Driver_Index] == 8)
    {
      ANTRCC_R_BackwardRight();
    }
    else{
      Stop();
    }
    
    RT_Driver_Timer++;
    
  }
  else{
    
    RT_Driver_Index++;
    RT_Driver_Timer= 0;
    
  }
  
  
} /* end RT_DriverSM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void RT_DriverSM_Error(void)          
{
  
} /* end RT_DriverSM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
