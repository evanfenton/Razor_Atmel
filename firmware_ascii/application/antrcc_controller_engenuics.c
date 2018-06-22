/**********************************************************************************************************************
File: antrcc-receiver-engenuics.c                                                                

----------------------------------------------------------------------------------------------------------------------

Description:
This is the official code for the enGENIUS ANT RC Car following Engenuics standards

------------------------------------------------------------------------------------------------------------------------
API:

ANT
LEDS
BUTTONS

Public functions:
----------------


Protected System functions:
--------------------------

void ANTRCC_C_Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void ANTRCC_C_RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


Private Application functions:
-----------------------------

static void ANTRCC_C_AntInitialize(void)
Runs required ANT initializaton for the task. Should be called once from ANT startup state.

static void ANTRCC_C_AntSendMessage(void)
Sends a message with direction instructions to the receiver on RC car.

static void ANTRCC_C_MessageBytes(bool Forward, bool Backward, bool Left, bool Right)
Changes the message bytes to 0xFF or 0x00 depending on the arguments.


static void ANTRCC_C_Forward(void)
Adjusts proper bytes of the direction message and LEDs to signal forward.

static void ANTRCC_C_Backward(void)
Adjusts proper bytes of the direction message and LEDs to signal backward.

static void ANTRCC_C_LeftTurn(void)
Adjusts proper bytes of the direction message and LEDs to signal left turn.

static void ANTRCC_C_RightTurn(void)
Adjusts proper bytes of the direction message and LEDs to signal right turn.

static void ANTRCC_C_Stalled(void)
Adjusts proper bytes of the direction message and LEDs to signal stall.


static void ANTRCC_C_ForwardLeft(void)
Adjusts proper bytes of the direction message and LEDs to signal gradual left turn.

static void ANTRCC_C_ForwardRight(void)
Adjusts proper bytes of the direction message and LEDs to signal gradual right turn.

static void ANTRCC_C_BackwardLeft(void)
Adjusts proper bytes of the direction message and LEDs to signal backward left turn.

static void ANTRCC_C_BackwardRight(void)
Adjusts proper bytes of the direction message and LEDs to signal backward right turn.


State Machine functions:
-----------------------

static void ANTRCC_CSM_RobotActive(void)
When user is giving input with the buttons.

static void ANTRCC_CSM_Error(void)
When errors occur with ANT or any other part of the application.

static void ANTRCC_CSM_AntChannelAssign(void)
Assigning channel and finalizing connection to robot.

static void ANTRCC_CSM_AntStartup(void)
Tests for user button press to start application.

**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_ANTRCC_C_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ANTRCC_C_Flags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */

/* From ANT API */
extern u32 G_u32AntApiCurrentMessageTimeStamp;                           
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;            
extern u8 G_au8AntApiCurrentMessageBytes[ANT_APPLICATION_MESSAGE_BYTES];  
extern AntExtendedDataType G_sAntApiCurrentMessageExtData;                

/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "ANTRCC_C_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type ANTRCC_C_StateMachine;                  /* The state machine function pointer */

static u32 ANTRCC_C_u32Timeout = 0;                        /* Timeout counter used across states */

static u8 ANTRCC_C_u8DirectionMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA };

/* ANTRCC_C_u8DirectionMsg[0] = FORWARD SIGNAL BYTE
   ANTRCC_C_u8DirectionMsg[1] = BACKWARD SIGNAL BYTE
   ANTRCC_C_u8DirectionMsg[2] = LEFT TURN SIGNAL BYTE
   ANTRCC_C_u8DirectionMsg[3] = RIGHT TURN SIGNAL BYTE
*/

static AntAssignChannelInfoType ANTRCC_C_sChannelInfo;


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
Function: ANTRCC_C_Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void ANTRCC_C_Initialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 u32I = 0; u32I < 10000; u32I++);
  
  LCDMessage(LINE1_START_ADDR, "Press Button 0 to");
  LCDMessage(LINE2_START_ADDR, "connect to RC car");
  
  ANTRCC_C_StateMachine = ANTRCC_CSM_AntStartup;
  
} /* end ANTRCC_C_Initialize() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_RunActiveState

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void ANTRCC_C_RunActiveState(void)
{
  ANTRCC_C_StateMachine();
  
} /* end ANTRCC_C_RunActiveState() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_AntInitialize

Description:
Initializes the Channel info for the applications ANT usage

Requires:
  -

Promises:
  - initialize the channel info to the parameters defined in the header file
  - set the device as a slave node
*/
static void ANTRCC_C_AntInitialize(void)
{
  ANTRCC_C_sChannelInfo.AntChannel          = ANT_CHANNEL_ANTRCC;
  ANTRCC_C_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_ANTRCC;
  ANTRCC_C_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_ANTRCC;
  ANTRCC_C_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_ANTRCC;
  ANTRCC_C_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_ANTRCC;
  ANTRCC_C_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_ANTRCC;
  ANTRCC_C_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_ANTRCC;
  ANTRCC_C_sChannelInfo.AntFrequency        = ANT_FREQUENCY_ANTRCC;
  ANTRCC_C_sChannelInfo.AntTxPower          = ANT_TX_POWER_ANTRCC;
  ANTRCC_C_sChannelInfo.AntNetwork          = ANT_NETWORK_DEFAULT;
  
  ANTRCC_C_sChannelInfo.AntChannelType      = CHANNEL_TYPE_MASTER;
  
  ANTRCC_C_sChannelInfo.AntFlags            = 1;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    ANTRCC_C_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
}


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_AntSendMessage

Description:
Sends the next direction message to the receiver on the RC Car

Requires:
  - connection to a slave node (the receiver)

Promises:
  - send the latest direction message stored in ANTRCC_C_u8DirectionMsg

*/
static void ANTRCC_C_AntSendMessage(void)
{
  if( AntReadAppMessageBuffer() )
  {
    AntQueueBroadcastMessage(ANT_CHANNEL_ANTRCC, ANTRCC_C_u8DirectionMsg);
  }

}


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_MessageBytes

Description:
Reads the current direction message sent by a controller

Requires:
  - four arguments indicating which bytes to change to 0xFF or 0x00
  - arguments do not conflict (e.g. robot cannot go forward and backward at the same time)

Promises:
  - change the direction message bytes depending on the truth values provided

*/
static void ANTRCC_C_MessageBytes(bool Forward, bool Backward, bool Left, bool Right)
{
  
  if(Forward)
  {
    ANTRCC_C_u8DirectionMsg[0]= 0xFF;
  }
  else
  {
    ANTRCC_C_u8DirectionMsg[0]= 0x00;
  }
  
  if(Backward)
  {
    ANTRCC_C_u8DirectionMsg[1]= 0xFF;
  }
  else
  {
    ANTRCC_C_u8DirectionMsg[1]= 0x00;
  }
  
  if(Left)
  {
    ANTRCC_C_u8DirectionMsg[2]= 0xFF;
  }
  else
  {
    ANTRCC_C_u8DirectionMsg[2]= 0x00;
  }
  
  if(Right)
  {
    ANTRCC_C_u8DirectionMsg[3]= 0xFF;
  }
  else
  {
    ANTRCC_C_u8DirectionMsg[3]= 0x00;
  }
}


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_Forward

Description:
Adjusts proper bytes of the direction message and LEDs to signal forward.

Requires:
  - 

Promises:
  - call message byte function with right arguments to send forward message
  - GREEN LED is ON and others are OFF
*/
static void ANTRCC_C_Forward(void)
{
  LedOn(GREEN);
  LedOff(RED);
  LedOff(YELLOW);
  LedOff(PURPLE);
  LedOff(BLUE);
  
  ANTRCC_C_MessageBytes(TRUE, FALSE, FALSE, FALSE);

} /* end ANTRCC_C_Forward() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_Backward

Description:
Adjusts proper bytes of the direction message and LEDs to signal backward.

Requires:
  - 

Promises:
  - call message byte function with right arguments to send backward message
  - YELLOW LED is ON and others are OFF
*/
static void ANTRCC_C_Backward(void)
{
  LedOn(YELLOW);
  LedOff(RED);
  LedOff(GREEN);
  LedOff(PURPLE);
  LedOff(BLUE);
  
  ANTRCC_C_MessageBytes(FALSE, TRUE, FALSE, FALSE);

} /* end ANTRCC_C_Backward() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_LeftTurn

Description:
Adjusts proper bytes of the direction message and LEDs to signal left turn.

Requires:
  - 

Promises:
  - call message byte function with right arguments to send left turn message
  - PURPLE LED is ON and others are OFF
*/
static void ANTRCC_C_LeftTurn(void)
{
  LedOn(PURPLE);
  LedOff(RED);
  LedOff(GREEN);
  LedOff(BLUE);
  LedOff(YELLOW);
  
  ANTRCC_C_MessageBytes(FALSE, FALSE, TRUE, FALSE);

} /* end ANTRCC_C_LeftTurn() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_RightTurn

Description:
Adjusts proper bytes of the direction message and LEDs to signal right turn.

Requires:
  - 

Promises:
  - call message byte function with right arguments to send right turn message
  - BLUE LED is ON and others are OFF
*/
static void ANTRCC_C_RightTurn(void)
{
  LedOn(BLUE);
  LedOff(YELLOW);
  LedOff(GREEN);
  LedOff(RED);
  LedOff(PURPLE);
  
  ANTRCC_C_MessageBytes(FALSE, FALSE, FALSE, TRUE);

} /* end ANTRCC_C_RightTurn() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_Stalled

Description:
Adjusts proper bytes of the direction message and LEDs to signal stall.

Requires:
  - 

Promises:
  - call message byte function with right arguments to send stall message
  - RED LED is ON and others are OFF
*/
static void ANTRCC_C_Stalled(void)
{
  LedOff(PURPLE);
  LedOff(GREEN);
  LedOff(YELLOW);
  LedOff(BLUE);
  LedOn(RED);
  
  ANTRCC_C_MessageBytes(FALSE, FALSE, FALSE, FALSE);

} /* end ANTRCC_C_Stalled() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_ForwardLeft

Description:
Adjusts proper bytes of the direction message and LEDs to signal gradual left turn.


Requires:
  - 

Promises:
  - call message byte function with right arguments to send forward-left message
  - GREEN and PURPLE LEDs are ON and others are OFF
*/
static void ANTRCC_C_ForwardLeft(void)
{
  LedOn(GREEN);
  LedOff(RED);
  LedOff(YELLOW);
  LedOn(PURPLE);
  LedOff(BLUE);
  
  ANTRCC_C_MessageBytes(TRUE, FALSE, TRUE, FALSE);
  
} /* end ANTRCC_C_ForwardLeft() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_ForwardRight

Description:
Adjusts proper bytes of the direction message and LEDs to signal gradual right turn.


Requires:
  - 

Promises:
  - call message byte function with right arguments to send forward-right message
  - GREEN and BLUE LEDs are ON and others are OFF
*/
static void ANTRCC_C_ForwardRight(void)
{
  LedOn(GREEN);
  LedOff(RED);
  LedOff(YELLOW);
  LedOff(PURPLE);
  LedOn(BLUE);
  
  ANTRCC_C_MessageBytes(TRUE, FALSE, FALSE, TRUE);
  
} /* end ANTRCC_C_ForwardRight() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_BackwardLeft

Description:
Adjusts proper bytes of the direction message and LEDs to signal backward left turn.

Requires:
  - 

Promises:
  - call message byte function with right arguments to send backward-left message
  - YELLOW and PURPLE LEDs are ON and others are OFF
*/
static void ANTRCC_C_BackwardLeft(void)
{
  LedOff(GREEN);
  LedOff(RED);
  LedOn(YELLOW);
  LedOn(PURPLE);
  LedOff(BLUE);
  
  ANTRCC_C_MessageBytes(FALSE, TRUE, TRUE, FALSE);

} /* end ANTRCC_C_BackwardLeft() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_C_BackwardRight

Description:
Adjusts proper bytes of the direction message and LEDs to signal backward right turn.

Requires:
  - 

Promises:
  - call message byte function with right arguments to send backward-right message
  - YELLOW and BLUE LEDs are ON and others are OFF
*/
static void ANTRCC_C_BackwardRight(void)
{
  LedOff(GREEN);
  LedOff(RED);
  LedOn(YELLOW);
  LedOff(PURPLE);
  LedOn(BLUE);
  
  ANTRCC_C_MessageBytes(FALSE, TRUE, FALSE, TRUE);

} /* end ANTRCC_C_BackwardRight() */


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CSM_RobotActive

Description:
Controller application is started and communicating with the receiver

Requires:
  - ANT setup successful

Promises:
  - call different direction functions based on buttons being pressed by user

*/
static void ANTRCC_CSM_RobotActive(void)
{
  if(IsButtonPressed(BUTTON0) && IsButtonPressed(BUTTON2))
  {
    ANTRCC_C_ForwardLeft();
  }
  else if(IsButtonPressed(BUTTON0) && IsButtonPressed(BUTTON3))
  {
    ANTRCC_C_ForwardRight();
  }
  else if(IsButtonPressed(BUTTON0))
  {
    ANTRCC_C_Forward();
  }
  else if(IsButtonPressed(BUTTON1) && IsButtonPressed(BUTTON2))
  {
    ANTRCC_C_BackwardLeft();
  }
  else if(IsButtonPressed(BUTTON1) && IsButtonPressed(BUTTON3))
  {
    ANTRCC_C_BackwardRight();
  }
  else if(IsButtonPressed(BUTTON1))
  {
    ANTRCC_C_Backward();
  }
  else if(IsButtonPressed(BUTTON2))
  {
    ANTRCC_C_LeftTurn();
  }
  else if(IsButtonPressed(BUTTON3))
  {
    ANTRCC_C_RightTurn();
  }
  else
  {
    ANTRCC_C_Stalled();
  }
  
  ANTRCC_C_AntSendMessage();
  
} /* end ANTRCC_CSM_RobotActive() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CSM_Error

Description:
Something has gone wrong in the application

Requires:
  - 

Promises:
  - 
*/
static void ANTRCC_CSM_Error(void)
{
  
} /* end ANTRCC_CSM_Error() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CSM_AntChannelAssign

Description:
Waiting to connect to a slave/receiver

Requires:
  - ANT channel info is setup correctly
  - user has pressed BUTTON0

Promises:
  - go to active robot state if ANT is configured properly
  - go to communication interupt state after a certain amount of time with no connection
*/
static void ANTRCC_CSM_AntChannelAssign(void)
{
  
  if(AntRadioStatusChannel(ANT_CHANNEL_ANTRCC) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_ANTRCC);
    
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LCDMessage(LINE2_START_ADDR, "^     v      <     >");  
    
    ANTRCC_C_StateMachine = ANTRCC_CSM_RobotActive;
  }
  
  ANTRCC_C_u32Timeout++;
  
  if(ANTRCC_C_u32Timeout == 5000)
  {
    LedBlink(RED, LED_2HZ);
    
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 u32I = 0; u32I < 10000; u32I++);
    LCDMessage(LINE1_START_ADDR, "ANT Not Configured");
    
    ANTRCC_C_StateMachine = ANTRCC_CSM_Error;
  }
  
} /* end ANTRCC_CSM_AntChannelAssign() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CSM_AntStartup

Description:
Wait for the user to press BUTTON0

Requires:
  -

Promises:
  - calls ANT initializing function when user presses BUTTON0
  - go to channel assign state if initialization is successful
  - go to communication interupt state if unsuccessful
*/
static void ANTRCC_CSM_AntStartup(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    ANTRCC_C_AntInitialize();
    
    if(AntAssignChannel(&ANTRCC_C_sChannelInfo))
    {
      ANTRCC_C_StateMachine = ANTRCC_CSM_AntChannelAssign;
    }
    else
    {
      LedBlink(RED, LED_2HZ);
    
      LCDCommand(LCD_CLEAR_CMD);
      for(u32 u32I = 0; u32I < 10000; u32I++);
      LCDMessage(LINE1_START_ADDR, "Assign Channel Error");
      
      ANTRCC_C_StateMachine = ANTRCC_CSM_Error;
    }
    
  }
  
} /* end ANTRCC_CSM_AntStartup() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/























