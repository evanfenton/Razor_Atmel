/**********************************************************************************************************************
File: user_app1.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
 1. Copy both user_app1.c and user_app1.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app1" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app1.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp1Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp1RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp1"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp1Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */
extern volatile u8  G_u8ApplicationIndicator;          /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
//static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */

static u8 UserApp1_enterNameMSG[]= "Enter Your Name     ";
static u8 UserApp1_directionMSG[]= "L     R    SEL   ENT";
static u8 UserApp1_firstABC[]= "ABCDEFGHIJKLM       ";
static u8 UserApp1_secondABC[]= "NOPQRSTUVWXYZ       ";
static u8 UserApp1_nameCheckMSG[]= "Correct?     Y     N";

static u8 UserApp1_userName[]="Bob                 ";

static u8 Userapp1_cursorPosition;
static u8 Userapp1_nameSize;

static u32 UserApp1_NameCheckTimer;                       /* for timing name checking */

static u8 UserApp1_Display1;
static u8 UserApp1_Display2;

static bool UserApp1_Started;

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
Function: UserApp1Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp1Initialize(void)
{
  LedNumberType initSeq[]= {WHITE, PURPLE, BLUE, CYAN, GREEN, YELLOW, ORANGE, RED};
  
  for(u8 i=0; i<8; i++){
    
    LedOn(initSeq[i]);
    for(u32 j = 0; j < 2000000; j++);
    LedOff(initSeq[i]);
    
  }
  LCDCommand(LCD_CLEAR_CMD);
  
  DisplayNameInit();
  
//  LCDCommand(LCD_HOME_CMD);
//  LCDCommand(LCD_DISPLAY_CMD | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR | LCD_DISPLAY_BLINK);
  
  Userapp1_cursorPosition= LINE1_START_ADDR;
 
  Userapp1_nameSize= 0;
  
  UserApp1_Started= FALSE;
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp1_StateMachine = UserApp1SM_Error;
  }

} /* end UserApp1Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp1RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp1RunActiveState(void)
{
  UserApp1_StateMachine();

} /* end UserApp1RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

static void DisplayAlphabet(void)
{
  if(UserApp1_Display1 == 0){
    LCDMessage(LINE1_START_ADDR, UserApp1_firstABC);
    UserApp1_Display1++;
  }
  else{
    LCDMessage(LINE2_START_ADDR, UserApp1_secondABC);
    UserApp1_Display1= 0;
  }
}


static void DisplayNameCheck(void)
{
  if(UserApp1_Display2 == 0){
    LCDMessage(LINE1_START_ADDR, UserApp1_userName);
    UserApp1_Display2++;
  }
  else{
    LCDMessage(LINE2_START_ADDR, UserApp1_nameCheckMSG);
    UserApp1_Display2= 0;
  }
}

static void DisplayNameInit(void)
{
  LCDMessage(LINE1_START_ADDR, UserApp1_enterNameMSG);
  LCDMessage(LINE2_START_ADDR, UserApp1_directionMSG);
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  if(UserApp1_Started){
    
    DisplayAlphabet();
    
    // move cursor LEFT
    if(WasButtonPressed(BUTTON0)){
      ButtonAcknowledge(BUTTON0);
      LedOn(ORANGE);
    }
  
    // move cursor RIGHT
    else if(WasButtonPressed(BUTTON1)){
      ButtonAcknowledge(BUTTON1);
      LedOn(RED);
    }
  
    // SELECT the letter
    else if(WasButtonPressed(BUTTON2)){
      ButtonAcknowledge(BUTTON2);
      LedOn(BLUE);
      
    
    }
  
    // ENTER / FINISHED
    else if(WasButtonPressed(BUTTON3)){
      ButtonAcknowledge(BUTTON3);
      UserApp1_NameCheckTimer= 0;
      LedOn(GREEN);
      UserApp1_StateMachine = UserApp1SM_NameCheck;
    
    }
  }
  else{
    
    if(WasButtonPressed(BUTTON3)){
      ButtonAcknowledge(BUTTON3);
      UserApp1_Started= TRUE;
      
    }
    
  }
  
  
  
  
} /* end UserApp1SM_Idle() */


static void UserApp1SM_NameCheck(void)
{
  
  if(UserApp1_NameCheckTimer >= 4000){
    
    // YES, the name entered is CORRECT
    if(WasButtonPressed(BUTTON2)){
      ButtonAcknowledge(BUTTON2);
      G_u8ApplicationIndicator= 2;
    }
    
    // NO, the name entered is NOT CORRECT
    else if(WasButtonPressed(BUTTON3)){
      ButtonAcknowledge(BUTTON3);
      Userapp1_cursorPosition= LINE1_START_ADDR;
      Userapp1_nameSize= 0;
      
      UserApp1_StateMachine = UserApp1SM_Idle;
    }
  }
  else{
    DisplayNameCheck();
    UserApp1_NameCheckTimer++;
  }
  
} /* end UserApp1SM_NameCheck() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
