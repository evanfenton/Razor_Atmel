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

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp1_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp1_StateMachine;            /* The state machine function pointer */
static u32 UserApp1_u32Timeout = 0;                      /* Timeout counter used across states */
/*
static u8 u8StalledMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     0x00 };
static u8 u8ForwardMsg[ANT_DATA_BYTES]= { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,     0x01 };
static u8 u8BackwardMsg[ANT_DATA_BYTES]= { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,    0x02 };
static u8 u8LeftTurnMsg[ANT_DATA_BYTES]= { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,    0x03 };
static u8 u8RightTurnMsg[ANT_DATA_BYTES]= { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,   0x04 };
*/

static u8 u8DirectionMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0xA5, 0x00, 0x00, 0x00 };
/* last byte= 0x00 for STALLED
              0x01 for FORWARD
              0x02 for BACKWARD
              0x03 for LEFT
              0x04 for RIGHT */

/*
static u8 u8ForwardLCDMsg[]= "Going Forward";
static u8 u8BackwardLCDMsg[]= "Going Backward";
static u8 u8LeftLCDMsg[]= "Turning Left";
static u8 u8RightLCDMsg[]= "Turning Right";
*/

/******GLOBALS******/
static u8 au8DriverName[12] = {'N', 'A', 'M', 'E'};
static u8 u8CursorPosition = 8;
static bool bEdit = TRUE;
static bool bLoad = TRUE;
static u32 u32Timer = 0;

AntAssignChannelInfoType sChannelInfo;


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
  
  for(u32 u32I = 0; u32I < 10000; u32I++);
  
  LCDMessage(LINE1_START_ADDR, "Press Button 0 to");
  LCDMessage(LINE2_START_ADDR, "connect to RC car");
  
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
static void LEDLoading(void)
{
  while(u32Timer < 350000)
  {
    u32Timer++;
    if(u32Timer < 50000)
    {
      LedOn(RED);
    }
    if(u32Timer > 50000 && u32Timer < 100000)
    {
      LedOn(ORANGE);
    }
    if(u32Timer > 100000 && u32Timer < 150000)
    {
      LedOn(YELLOW);
    }
    if(u32Timer > 150000 && u32Timer < 220000)
    {
      LedOn(GREEN);
    }
    if(u32Timer > 220000 && u32Timer < 280000)
    {
      LedOn(BLUE);
      LedOff(RED);
      LedOff(ORANGE);
      LedOff(YELLOW);
      LedOff(GREEN);
    }
    if(u32Timer > 28000)
    {
      LedOff(BLUE);
      bLoad = FALSE;
    }
  }
}


//CALL BEFORE ENTERING IDLE STATE
static void UserApp1LCDInit(void)
{
  static u8* au8NameEdit = "^     v    >/CLR  OK";
  
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 u32I = 0; u32I < 10000; u32I++);
  
  LCDMessage(0x00, "Driver:");
  LCDMessage(0x08, au8DriverName);
  LCDMessage(LINE2_START_ADDR, au8NameEdit);
  
  for(u32 u32I = 0; u32I < 10000; u32I++);
  
  LCDCommand(LCD_DISPLAY_CMD | LCD_DISPLAY_ON | LCD_DISPLAY_CURSOR | LCD_DISPLAY_BLINK);
  LCDCommand(LCD_ADDRESS_CMD | u8CursorPosition);
}
//CALL IN IDLE STATE
static void UserApp1NameEdit(void)
{ 
  static bool bUpdateMessage = FALSE;
  static u8 u8Index = 0;
  static u8* au8Controls = "^     v      <     >";
  
  //SCROLL UP
  if(WasButtonPressed(BUTTON0) && au8DriverName[u8Index] > 'A')
  {
    ButtonAcknowledge(BUTTON0);
    au8DriverName[u8Index]--;
    bUpdateMessage = TRUE;
  }
  //SCROLL DOWN
  if(WasButtonPressed(BUTTON1) && au8DriverName[u8Index] < 'Z')
  {
    ButtonAcknowledge(BUTTON1);
    au8DriverName[u8Index]++;
    bUpdateMessage = TRUE;
  }
  //GET NEW LETTER
  if(WasButtonPressed(BUTTON2) && u8Index < 11)
  {
    ButtonAcknowledge(BUTTON2);
    u8Index++;
    if(au8DriverName[u8Index] < 'A' || au8DriverName[u8Index] > 'Z')
    {
      bUpdateMessage = TRUE;
      au8DriverName[u8Index] = 'A';
    }
    u8CursorPosition++;
    LCDCommand(LCD_ADDRESS_CMD | u8CursorPosition);
  }
  //CLEAR
  if(IsButtonHeld(BUTTON2, 500) && u8Index > 0)
  {
    au8DriverName[0] = 'A';
    for(u8 u8i= 1; u8i < 11; u8i++)
    {
      au8DriverName[u8i] = ' ';
    }
    LCDClearChars(8, 12); 
    u8Index = 0;
    u8CursorPosition = 8;
  }
  //CONFIRM
  if(WasButtonPressed(BUTTON3) || u8Index == 12)
  {
    ButtonAcknowledge(BUTTON3);
    LCDCommand(LCD_DISPLAY_CMD | LCD_DISPLAY_ON);
    LCDCommand(LCD_CLEAR_CMD);
    
    for(u32 u32I = 0; u32I < 10000; u32I++);
    
    LCDMessage(LINE1_START_ADDR, "Driving:");
    LCDMessage(LINE1_START_ADDR+9, au8DriverName);
    LCDMessage(LINE2_START_ADDR, au8Controls);
    
    /******Change some boolean to stop calling the function*******/
    bEdit = FALSE;
  }
  //ONLY UPDATE LCD IF MESSAGE CHANGES
  if(bUpdateMessage)
  {
    LCDMessage(0x08, au8DriverName);
    
    for(u32 u32I = 0; u32I < 10000; u32I++);
    
    LCDCommand(LCD_ADDRESS_CMD | u8CursorPosition);
    bUpdateMessage = FALSE;
  }
}

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
  
  /**************************************/
  sChannelInfo.AntFlags            = 1;
  /**************************************/
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
}

static void AntMasterConfig(void)
{
  AntInit();
  sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
  if(AntAssignChannel(&sChannelInfo))
  {
    UserApp1_u32Timeout++;
    UserApp1_StateMachine = UserApp1SM_ANT_ChannelAssign;
    
    if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
    {
      AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
      UserApp1_StateMachine = UserApp1SM_Idle;
    }
    if(UserApp1_u32Timeout == 5000)
    {
      LedBlink(RED, LED_4HZ);
      UserApp1_StateMachine = UserApp1SM_Error;
    }
  }
  else
  {
    LedOn(RED);
    UserApp1_StateMachine = UserApp1SM_Error;
  }
}

/* DIRECTION FUNCTIONS */

static void Forward(void)
{
  LedOn(GREEN);
  LedOff(RED);
  LedOff(YELLOW);
  LedOff(PURPLE);
  LedOff(BLUE);
  u8DirectionMsg[0]= 0xFF;
  u8DirectionMsg[1]= 0x00;
  u8DirectionMsg[2]= 0x00;
  u8DirectionMsg[3]= 0x00;
}

static void Backward(void)
{
  LedOn(YELLOW);
  LedOff(RED);
  LedOff(GREEN);
  LedOff(PURPLE);
  LedOff(BLUE);
  u8DirectionMsg[0]= 0x00;
  u8DirectionMsg[1]= 0xFF;
  u8DirectionMsg[2]= 0x00;
  u8DirectionMsg[3]= 0x00;
  
}

static void LeftTurn(void)
{
  
  LedOn(PURPLE);
  LedOff(RED);
  LedOff(GREEN);
  LedOff(BLUE);
  LedOff(YELLOW);
  u8DirectionMsg[0]= 0x00;
  u8DirectionMsg[1]= 0x00;
  u8DirectionMsg[2]= 0xFF;
  u8DirectionMsg[3]= 0x00;
}

static void RightTurn(void)
{
  LedOn(BLUE);
  LedOff(ORANGE);
  LedOff(GREEN);
  LedOff(RED);
  LedOff(PURPLE);
  u8DirectionMsg[0]= 0x00;
  u8DirectionMsg[1]= 0x00;
  u8DirectionMsg[2]= 0x00;
  u8DirectionMsg[3]= 0xFF;
}

static void Stalled(void)
{
  LedOff(PURPLE);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(BLUE);
  LedOn(RED);
  u8DirectionMsg[0]= 0x00;
  u8DirectionMsg[1]= 0x00;
  u8DirectionMsg[2]= 0x00;
  u8DirectionMsg[3]= 0x00;
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/
static void UserApp1SM_Master_or_Slave(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    AntMasterConfig();
  }
}

static void UserApp1SM_ANT_ChannelAssign(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    UserApp1LCDInit();
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
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
  if(bEdit)
  {
    if(bLoad)
    {
      LEDLoading();
    }
    else
    {
      UserApp1NameEdit();
    }
  }
  else
  {
    if(IsButtonPressed(BUTTON0))
    {
      Forward();
    }
    else if(IsButtonPressed(BUTTON1))
    {
      Backward();
    }
    else if(IsButtonPressed(BUTTON2))
    {
      LeftTurn();
    }
    else if(IsButtonPressed(BUTTON3))
    {
      RightTurn();
    }
    else
    {
      Stalled();
    }
    if( AntReadAppMessageBuffer() )
    {
      AntQueueBroadcastMessage(ANT_CHANNEL_USERAPP, u8DirectionMsg);
    }
  }
  
} /* end UserApp1SM_Idle() */
    

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserApp1SM_Error(void)          
{
  
} /* end UserApp1SM_Error() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
