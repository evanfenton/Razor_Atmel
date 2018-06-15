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

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

extern u32 G_u32AntApiCurrentMessageTimeStamp;                            // From ant_api.c
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;            // From ant_api.c
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  // From ant_api.c
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                // From ant_api.c

AntAssignChannelInfoType sChannelInfo;
/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout;                      /* Timeout counter used across states */
static u8 u8DirectionMsg[ANT_DATA_BYTES]= { 0x01, 0x00, 0x00, 0x00, 0xA5, 0x00, 0x00, 0x00 };
/* last byte= 0x00 for STALLED
              0x01 for FORWARD
              0x02 for BACKWARD
              0x03 for LEFT
              0x04 for RIGHT */

static u8 FL_counter;
static u8 FR_counter;
static u8 BL_counter;
static u8 BR_counter;




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
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 u32i = 0; u32i < 10000; u32i++);
  
  LCDMessage(LINE1_START_ADDR, "Press Button 0 to");
  LCDMessage(LINE2_START_ADDR, "connect controller");
  
  FL_counter= 0;
  FR_counter= 0;
  BL_counter= 0;
  BR_counter= 0;
  
  UserApp1_StateMachine = UserApp1SM_Master_or_Slave;
 
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

/* ANT FUNCTIONS */
static void AntInit(void)
{
  sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;
  sChannelInfo.AntNetwork          = ANT_NETWORK_DEFAULT;
  sChannelInfo.AntFlags            = 1;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
}

static void AntSlaveConfig(void)
{
  AntInit();
  sChannelInfo.AntChannelType = CHANNEL_TYPE_SLAVE;
  if(AntAssignChannel(&sChannelInfo))
  {
    UserApp1_StateMachine = UserApp1SM_ANT_ChannelAssign;
  }
  else
  {
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

static void AntReadMessage(void)
{
  static u32 u32Timeout = 0;
  if( AntReadAppMessageBuffer() )
  {
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      LedOn(GREEN);
      LedOff(RED);
      LCDCommand(LCD_CLEAR_CMD);
      u32Timeout = 0;
      for(u8 i = 0; i < ANT_DATA_BYTES; i++)
      {
        u8DirectionMsg[i] = G_au8AntApiCurrentMessageBytes[i];
      }
    }
  }
  u32Timeout++;
  LedOff(GREEN);
  
  if(u32Timeout == 60000)
  {
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LedBlink(RED, LED_2HZ);
    LCDMessage(LINE1_START_ADDR, "Lost Connection");
    UserApp1_StateMachine = UserApp1SM_Error;
  }  
}

/* DIRECTION FUNCTIONS */

static void Forward(void) // element 0 GREEN
{
  SignalOff(INPUT_TOP);
  SignalOn(INPUT_BOT);
  SignalOn(EN_LEFT);
  SignalOn(EN_RIGHT);
}

static void Backward(void) // element 1 YELLOW
{
  SignalOn(INPUT_TOP);
  SignalOff(INPUT_BOT);
  SignalOn(EN_LEFT);
  SignalOn(EN_RIGHT);
}

static void Left(void) // element 2 PURPLE
{
  SignalOff(INPUT_TOP);
  SignalOn(INPUT_BOT);
  SignalOff(EN_LEFT);
  SignalOn(EN_RIGHT);
}

static void Right(void) // element 3 BLUE
{
  SignalOff(INPUT_TOP);
  SignalOn(INPUT_BOT);
  SignalOn(EN_LEFT);
  SignalOff(EN_RIGHT);
}

static void Stalled(void) //no element RED
{
  SignalOff(INPUT_TOP);
  SignalOff(INPUT_BOT);
  SignalOff(EN_LEFT);
  SignalOff(EN_RIGHT);
}


/* COMBINATION FUNCTIONS */

static void Forward_Left(void)
{
  FL_counter++;
  
  if(FL_counter < FIRST_DIR_RATE)
  {
    Forward();
  }
  else if(FL_counter < SEC_DIR_RATE)
  {
    Left();
  }
  else
  {
    FL_counter= 0;
  }
  
}

static void Forward_Right(void)
{
  
  FR_counter++;
  
  if(FR_counter < FIRST_DIR_RATE)
  {
    Forward();
  }
  else if(FR_counter < SEC_DIR_RATE)
  {
    Right();
  }
  else
  {
    FR_counter= 0;
  }
  
}

static void Backward_Left(void)
{
  
  BL_counter++;
  
  if(BL_counter < FIRST_DIR_RATE)
  {
    Backward();
  }
  else if(BL_counter < SEC_DIR_RATE)
  {
    Backward();
    SignalOff(EN_LEFT);
  }
  else
  {
    BL_counter= 0;
  }
  
}

static void Backward_Right(void)
{
  
  BR_counter++;
  
  if(BR_counter < FIRST_DIR_RATE)
  {
    Backward();
  }
  else if(BR_counter < SEC_DIR_RATE)
  {
    Backward();
    SignalOff(EN_RIGHT);
  }
  else
  {
    BR_counter= 0;
  }
}

static void CombinationLCD(void)
{
  //WHITE
  LedOn(LCD_RED);
  LedOn(LCD_GREEN);
  LedOn(LCD_BLUE);
}


/* PIN SIGNAL FUNCTIONS */

static void SignalOn(u32 pin)
{ 
  u32 *pu32ToggleGPIO= (u32*)(&(AT91C_BASE_PIOA->PIO_SODR));
  *pu32ToggleGPIO = pin;
  
}

static void SignalOff(u32 pin)
{  
  u32 *pu32ToggleGPIO= (u32*)(&(AT91C_BASE_PIOA->PIO_CODR));
  *pu32ToggleGPIO = pin;
}



/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
static void UserApp1SM_Master_or_Slave(void)
{
  LedOn(RED);
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    AntSlaveConfig();
  }
  
}


static void UserApp1SM_ANT_ChannelAssign(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    UserApp1_StateMachine = UserApp1SM_Idle;
  }
  UserApp1_u32Timeout++;
  if(UserApp1_u32Timeout == 5000)
  {
    LedBlink(RED, LED_2HZ);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
} /* end UserApp1SM_ChannelAssign() */



/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for ??? */
static void UserApp1SM_Idle(void)
{
  //FORWARD AND LEFT
  if(u8DirectionMsg[0] == 0xFF && u8DirectionMsg[2] == 0xFF)
  {
    CombinationLCD();
    Forward_Left();
  }
  //FORWARD AND RIGHT
  else if(u8DirectionMsg[0] == 0xFF && u8DirectionMsg[3] == 0xFF)
  {
    CombinationLCD();
    Forward_Right();
  }
  //FORWARD
  else if(u8DirectionMsg[0] == 0xFF)
  {
    LedOn(LCD_GREEN);
    LedOff(LCD_RED);
    LedOff(LCD_BLUE);
    Forward();
  }
  
  //BACKWARD AND LEFT
  else if(u8DirectionMsg[1] == 0xFF && u8DirectionMsg[2] == 0xFF)
  {
    CombinationLCD();
    Backward_Left();
  }
  //BACKWARD AND RIGHT
  else if(u8DirectionMsg[1] == 0xFF && u8DirectionMsg[3] == 0xFF)
  {
    CombinationLCD();
    Backward_Right();
  }
  //BACKWARD
  else if(u8DirectionMsg[1] == 0xFF)
  {
    LedOn(LCD_RED);
    LedOn(LCD_GREEN);
    LedOff(LCD_BLUE);
    Backward();
  }
  
  //LEFT TURN
  else if(u8DirectionMsg[2] == 0xFF)
  {
    LedOn(LCD_BLUE);
    LedOn(LCD_RED);
    LedOff(LCD_GREEN);
    Left();
  }
  //RIGHT TURN
  else if(u8DirectionMsg[3] == 0xFF)
  {
    LedOff(LCD_GREEN);
    LedOff(LCD_RED);
    LedOn(LCD_BLUE);
    Right();
  }
  else
  {
    //STALLED
    LedOff(LCD_GREEN);
    LedOff(LCD_BLUE);
    LedOn(LCD_RED);
    Stalled();
  }
  
  AntReadMessage();
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
