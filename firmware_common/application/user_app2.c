/**********************************************************************************************************************
File: user_app2.c                                                                

Description:
This is a user_app2.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserApp2Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserApp2RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_UserApp2"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserApp2Flags;                       /* Global state flags */
volatile u32 G_u32UserApp2Timer;                       /* for timing restart */

/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp2_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp2_StateMachine;            /* The state machine function pointer */
//static u32 UserApp2_u32Timeout;                      /* Timeout counter used across states */

static u8 simonSequence[MAX_PATTERN_SIZE];           /* for keeping pattern (edit max in header)*/
static bool addingTo;                                /* when adding to the sequence= TRUE */
static bool showingSeq;                              /* when showing sequence = TRUE */
static u32 showIndex;                                /* for showing pattern */
static u32 respIndex;                                /* for getting reponse */
static u32 seqSize;                                  /* keeps current size of pattern */
static bool startState;                              /* for restarting */

static u8 line1Msg[] = "Watch and Repeat";
static u8 line2Msg[] = "B     G     Y      R";

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
Function: UserApp2Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserApp2Initialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, line1Msg);
  LCDMessage(LINE2_START_ADDR, line2Msg);
  
  LedNumberType initSeq[]= {BLUE, GREEN, YELLOW, RED};
  
  for(u8 n=0; n<5; n++){
    
    for(u8 i=0; i<4; i++){
    
      LedOn(initSeq[i]);
      for(u32 j = 0; j < 1000000; j++);
      LedOff(initSeq[i]);
    
    }
  }
  
  /* initialize all values to -1 */
  for(u32 i=0; i<100; i++){
    
    simonSequence[i]= -1;
  }
  
  
  showingSeq = FALSE;
  addingTo = FALSE;
  showIndex = 0;
  respIndex = 0;
  seqSize = 0;
  startState = TRUE;
  G_u32UserApp2Timer = 0;
  LedOn(ORANGE);
  
  
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp2_StateMachine = UserApp2SM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp2_StateMachine = UserApp2SM_FailedInit;
  }

} /* end UserApp2Initialize() */

  
/*----------------------------------------------------------------------------------------------------------------------
Function UserApp2RunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserApp2RunActiveState(void)
{
  UserApp2_StateMachine();

} /* end UserApp2RunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

static u8 getNewEntry(void)
{
  /* semi-random number from 0-3 */
  return (G_u32SystemTime1ms%4);
  
}


static void showPatternEntry( u8 entry )
{
  if(entry == 0){
    LedOn(BLUE);
  }
  else if(entry == 1){
    LedOn(GREEN);
  }
  else if(entry == 2){
    LedOn(YELLOW);
  }
  else if(entry == 3){
    LedOn(RED);
  }
  
  for(u32 j = 0; j < 3000000; j++);
  LedOff(BLUE);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(RED);
  
}


static bool responseCorrect(u8 entry)
{
  bool correct= FALSE;
  
  if(entry == 0 && WasButtonPressed(BUTTON0))
  {
    correct= TRUE;
  }
  else if(entry == 1 && WasButtonPressed(BUTTON1))
  {
    correct= TRUE;
  }
  else if(entry == 2 && WasButtonPressed(BUTTON2))
  {
    correct= TRUE;
  }
  else if(entry == 3 && WasButtonPressed(BUTTON3))
  {
    correct= TRUE;
  }
  
  ButtonAcknowledge(BUTTON0);
  ButtonAcknowledge(BUTTON1);
  ButtonAcknowledge(BUTTON2);
  ButtonAcknowledge(BUTTON3);
  
  return correct;
}


static bool responseGiven(void)
{
  bool reponse= FALSE;
  
  if(WasButtonPressed(BUTTON0)){
    LedOn(BLUE);
    reponse= TRUE;
    for(u32 j = 0; j < 2500000; j++);
  }
  else if(WasButtonPressed(BUTTON1)){
    LedOn(GREEN);
    reponse= TRUE;
    for(u32 j = 0; j < 2500000; j++);
  }
  else if(WasButtonPressed(BUTTON2)){
    LedOn(YELLOW);
    reponse= TRUE;
    for(u32 j = 0; j < 2500000; j++);
  }
  else if(WasButtonPressed(BUTTON3)){
    LedOn(RED);
    reponse= TRUE;
    for(u32 j = 0; j < 2500000; j++);
  }
 
  LedOff(BLUE);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(RED);
  
  return reponse;
}
                     

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp2SM_Idle(void)
{
  
  if(startState){
    
    if(G_u32UserApp2Timer >= 3000){
      addingTo = TRUE;
      startState = FALSE;
    }
    else{
      G_u32UserApp2Timer++;
    }
    
  }
  else if(addingTo)
  {
    LedOff(ORANGE);
    simonSequence[seqSize] = getNewEntry();
    seqSize++;
    showingSeq = TRUE;
    addingTo = FALSE;
  }
  else if(showingSeq)
  {
    
    for(u32 j = 0; j < 2000000; j++);
    showPatternEntry(simonSequence[showIndex]);
    showIndex++;
      
    if(showIndex == seqSize){
       showingSeq= FALSE;
       showIndex = 0;
    }
  }
  else
  {
    
    if(responseGiven()){
        
      if(responseCorrect(simonSequence[respIndex]))
      {
        respIndex++;
      }
      else
      {
        restart();
        startState = TRUE;
      }
      
    }
      
    if(respIndex == seqSize){
      addingTo= TRUE;
      respIndex = 0;
    }
  }
  
} /* end UserApp2SM_Idle() */
     
#if 0
/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp2SM_Error(void)          
{
  
} /* end UserApp2SM_Error() */
#endif


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserApp2SM_FailedInit(void)          
{
    
} /* end UserApp2SM_FailedInit() */


static void restart(void)
{
  
  showingSeq = FALSE;
  addingTo = FALSE;
  showIndex = 0;
  respIndex = 0;
  seqSize = 0;
  
  LedOn(ORANGE);
  
  G_u32UserApp2Timer = 0;
  
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
