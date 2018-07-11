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
BUZZERS

Public functions:
----------------


Protected System functions:
--------------------------

void ANTRCC_R_Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void ANTRCC_R_RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


Private Application functions:
-----------------------------

static void ANTRCC_R_AntInitialize(void)
Runs required ANT initializaton for the task. Should be called once from ANT startup state.

static void ANTRCC_R_AntReadMessage(void)
Reads an incoming message from the controller.

static void ANTRCC_R_Forward(void)
Adjusts proper signals for forward motion of robot.

static void ANTRCC_R_Backward(void)
Adjusts proper signals for backward motion of robot.

static void ANTRCC_R_LeftTurn(void)
Adjusts proper signals for left turn of robot.

static void ANTRCC_R_RightTurn(void)
Adjusts proper signals for right turn of robot.


static void ANTRCC_R_ForwardLeft(void)
Adjusts proper signals for more gradual left turn of robot using PWM concept.

static void ANTRCC_R_ForwardRight(void)
Adjusts proper signals for more gradual right turn of robot using PWM concept.

static void ANTRCC_R_BackwardLeft(void)
Adjusts proper signals for a backward left turn of robot using PWM concept.

static void ANTRCC_R_BackwardRight(void)
Adjusts proper signals for a backward right turn of robot using PWM concept.


static void ANTRCC_R_ColorLCD(bool Red, bool Green, bool Blue)
Changes LCD color with the provided RGB value.


static void ANTRCC_R_SignalOn(u32 pin)
Turns the provided signal/pin ON (use the pre-defined signal names in header)

static void ANTRCC_R_SignalOff(u32 pin)
Turns the provided signal/pin OFF (use the pre-defined signal names in header)



State Machine functions:
-----------------------

static void ANTRCC_RSM_RobotActive(void)
When user is giving input and robot is moving.

static void ANTRCC_RSM_RobotStalled(void)
When user is not giving input and robot is stopped.

static void ANTRCC_RSM_Error(void)
When receiver application error occurs

static void ANTRCC_RSM_AntChannelAssign(void)
Assigning channel and finalizing connection to controller.

static void ANTRCC_RSM_AntStartup(void)
Tests for user button press to start application.

**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_ANTRCC_R_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ANTRCC_R_Flags;                       /* Global state flags */


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
Variable names shall start with "ANTRCC_R_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type ANTRCC_R_StateMachine;            /* The state machine function pointer */
static u32 ANTRCC_R_u32Timeout;                      /* Timeout counter used across states */

static u8 ANTRCC_R_u8DirectionMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA };

/* ANTRCC_R_u8DirectionMsg[0] = FORWARD SIGNAL BYTE
   ANTRCC_R_u8DirectionMsg[1] = BACKWARD SIGNAL BYTE
   ANTRCC_R_u8DirectionMsg[2] = LEFT TURN SIGNAL BYTE
   ANTRCC_R_u8DirectionMsg[3] = RIGHT TURN SIGNAL BYTE
*/

static u8 ANTRCC_R_ComboCounter;

static AntAssignChannelInfoType ANTRCC_R_sChannelInfo;


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
Function: ANTRCC_R_Initialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void ANTRCC_R_Initialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 u32i = 0; u32i < 10000; u32i++);
  
  LCDMessage(LINE1_START_ADDR, "Press Button 0 to");
  LCDMessage(LINE2_START_ADDR, "connect controller");
  
  ANTRCC_R_ComboCounter= 0;
  
  ANTRCC_R_StateMachine = ANTRCC_RSM_AntStartup;
  
} /* end ANTRCC_R_Initialize() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_RunActiveState

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void ANTRCC_R_RunActiveState(void)
{
  ANTRCC_R_StateMachine();
  
} /* end ANTRCC_R_RunActiveState() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_AntInitialize

Description:
Initializes the Channel info for the applications ANT usage

Requires:
  -

Promises:
  - initialize the channel info to the parameters defined in the header file
  - set the device as a slave node
*/
static void ANTRCC_R_AntInitialize(void)
{
  
  ANTRCC_R_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  ANTRCC_R_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  ANTRCC_R_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  ANTRCC_R_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  ANTRCC_R_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  ANTRCC_R_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  ANTRCC_R_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  ANTRCC_R_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  ANTRCC_R_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;
  ANTRCC_R_sChannelInfo.AntNetwork          = ANT_NETWORK_DEFAULT;
  
  ANTRCC_R_sChannelInfo.AntChannelType      = CHANNEL_TYPE_SLAVE;
  
  ANTRCC_R_sChannelInfo.AntFlags            = 1;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    ANTRCC_R_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
} /* end ANTRCC_R_AntInitialize() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_Forward

Description:
Adjusts signals being sent to motor driver to make the robot move forward

Requires:
  - 

Promises:
  - send signals to make motors spin forward
  - LCD is GREEN
*/
void ANTRCC_R_Forward(void)
{
  ANTRCC_R_ColorLCD(FALSE,TRUE,FALSE);
  
  ANTRCC_R_SignalOff(DIR_REV);
  ANTRCC_R_SignalOn(DIR_FWD);
  ANTRCC_R_SignalOn(EN_LEFT);
  ANTRCC_R_SignalOn(EN_RIGHT);
  
} /* end ANTRCC_R_Forward() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_Backward

Description:
Adjusts signals being sent to motor driver to make the robot move backward

Requires:
  - 

Promises:
  - send signals to make motors spin backward
  - LCD is YELLOW
*/
void ANTRCC_R_Backward(void)
{
  ANTRCC_R_ColorLCD(TRUE,TRUE,FALSE);
  
  ANTRCC_R_SignalOn(DIR_REV);
  ANTRCC_R_SignalOff(DIR_FWD);
  ANTRCC_R_SignalOn(EN_LEFT);
  ANTRCC_R_SignalOn(EN_RIGHT);
  
} /* end ANTRCC_R_Backward() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_LeftTurn

Description:
Adjusts signals being sent to motor driver to make the robot turn left

Requires:
  - 

Promises:
  - send signals to make right side motor spin forward
  - LCD is PURPLE
*/
void ANTRCC_R_LeftTurn(void)
{
  ANTRCC_R_ColorLCD(TRUE,FALSE,TRUE);
  
  ANTRCC_R_SignalOff(DIR_REV);
  ANTRCC_R_SignalOn(DIR_FWD);
  ANTRCC_R_SignalOff(EN_LEFT);
  ANTRCC_R_SignalOn(EN_RIGHT);
  
} /* end ANTRCC_R_LeftTurn() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_RightTurn

Description:
Adjusts signals being sent to motor driver to make the robot turn right

Requires:
  -

Promises:
  - send signals to make left side motor spin forward
  - LCD is BLUE
*/
void ANTRCC_R_RightTurn(void)
{
  ANTRCC_R_ColorLCD(FALSE,FALSE,TRUE);
  
  ANTRCC_R_SignalOff(DIR_REV);
  ANTRCC_R_SignalOn(DIR_FWD);
  ANTRCC_R_SignalOn(EN_LEFT);
  ANTRCC_R_SignalOff(EN_RIGHT);
  
} /* end ANTRCC_R_RightTurn() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_ForwardLeft

Description:
Adjusts signals being sent to motor driver to make the robot move forward and left
for a more gradual turn

Requires:
  - 

Promises:
  - alternate the motor driver signals between forward and left
  - LCD is WHITE
*/
void ANTRCC_R_ForwardLeft(void)
{
  ANTRCC_R_ComboCounter++;
  
  if(ANTRCC_R_ComboCounter < FIRST_DIR_RATE)
  {
    ANTRCC_R_Forward();
  }
  else if(ANTRCC_R_ComboCounter < SEC_DIR_RATE)
  {
    ANTRCC_R_LeftTurn();
  }
  else
  {
    ANTRCC_R_ComboCounter= 0;
  }
  
  ANTRCC_R_ColorLCD(TRUE,TRUE,TRUE);
  
} /* end ANTRCC_R_ForwardLeft() */



/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_ForwardRight

Description:
Adjusts signals being sent to motor driver to make the robot move forward and right
for a more gradual turn

Requires:
  - 

Promises:
  - alternate the motor driver signals between forward and right
  - LCD is WHITE
*/
void ANTRCC_R_ForwardRight(void)
{
  ANTRCC_R_ComboCounter++;
  
  if(ANTRCC_R_ComboCounter < FIRST_DIR_RATE)
  {
    ANTRCC_R_Forward();
  }
  else if(ANTRCC_R_ComboCounter < SEC_DIR_RATE)
  {
    ANTRCC_R_RightTurn();
  }
  else
  {
    ANTRCC_R_ComboCounter= 0;
  }
  
  ANTRCC_R_ColorLCD(TRUE,TRUE,TRUE);
  
} /* end ANTRCC_R_ForwardRight() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_BackwardLeft

Description:
Adjusts signals being sent to motor driver to make the robot do a backward left turn

Requires:
  -

Promises:
  - alternate the motor driver signals between backward and backward with left motor off
  - LCD is WHITE
*/
void ANTRCC_R_BackwardLeft(void)
{
  ANTRCC_R_ComboCounter++;
  
  if(ANTRCC_R_ComboCounter < FIRST_DIR_RATE)
  {
    ANTRCC_R_Backward();
  }
  else if(ANTRCC_R_ComboCounter < SEC_DIR_RATE)
  {
    ANTRCC_R_Backward();
    ANTRCC_R_SignalOff(EN_LEFT);
  }
  else
  {
    ANTRCC_R_ComboCounter= 0;
  }
  
  ANTRCC_R_ColorLCD(TRUE,TRUE,TRUE);
  
} /* end ANTRCC_R_BackwardLeft() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_BackwardRight

Description:
Adjusts signals being sent to motor driver to make the robot do a backward right turn

Requires:
  - 

Promises:
  - alternate the motor driver signals between backward and backward with right motor off
  - LCD is WHITE
*/
void ANTRCC_R_BackwardRight(void)
{
  ANTRCC_R_ComboCounter++;
  
  if(ANTRCC_R_ComboCounter < FIRST_DIR_RATE)
  {
    ANTRCC_R_Backward();
  }
  else if(ANTRCC_R_ComboCounter < SEC_DIR_RATE)
  {
    ANTRCC_R_Backward();
    ANTRCC_R_SignalOff(EN_RIGHT);
  }
  else
  {
    ANTRCC_R_ComboCounter= 0;
  }
  
  ANTRCC_R_ColorLCD(TRUE,TRUE,TRUE);
  
} /* end ANTRCC_R_BackwardRight() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_AntReadMessage

Description:
Reads the current direction message sent by a controller

Requires:
  - connection to a master node (the controller)

Promises:
  - read a message from ANT api and copy it into ANTRCC_R_u8DirectionMsg
  - go to a communication interupt state if the controller has been disconnected
*/
static void ANTRCC_R_AntReadMessage(void)
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
        ANTRCC_R_u8DirectionMsg[i] = G_au8AntApiCurrentMessageBytes[i];
      }
    }
  }
  u32Timeout++;
  LedOff(GREEN);
  
  if(u32Timeout == 60000)
  {
    LedBlink(RED, LED_2HZ);
  
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LCDMessage(LINE1_START_ADDR, "Lost Connection");
    
    ANTRCC_R_StateMachine = ANTRCC_RSM_Error;
  }  
  
} /* end ANTRCC_R_AntReadMessage() */



/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_ColorLCD

Description:
Changes the color of the LCD to show the driver that commands are going through

Requires:
  - three arguments for RED, GREEN, and BLUE values (YES/TRUE or NO/FALSE)

Promises:
  - change the LCD color depending on the combination of RGB values provided
*/
static void ANTRCC_R_ColorLCD(bool Red, bool Green, bool Blue)
{
  
  // LCD RED
  if(Red){
    LedOn(LCD_RED);
  }
  else{
    LedOff(LCD_RED);
  }
  
  // LCD GREEN
  if(Green){
    LedOn(LCD_GREEN);
  }
  else{
    LedOff(LCD_GREEN);
  }
  
  // LCD BLUE
  if(Blue){
    LedOn(LCD_BLUE);
  }
  else{
    LedOff(LCD_BLUE);
  }
  
} /* end ANTRCC_R_ColorLCD(bool, bool , bool) */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_SignalOn

Description:
Turn the specified pin/signal ON

Requires:
  - pin provided is ACTIVE HIGH
  - pin settings are correctly configured in eief1-pcb-01.h

Promises:
  - pin signal set to HIGH (~5V)
*/
static void ANTRCC_R_SignalOn(u32 pin)
{
  u32 *pu32ToggleGPIO= (u32*)(&(AT91C_BASE_PIOA->PIO_SODR));
  *pu32ToggleGPIO = pin;
  
} /* end ANTRCC_R_SignalOn(u32) */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_R_SignalOff

Description:
Turn the specified pin/signal OFF

Requires:
  - pin provided is ACTIVE HIGH
  - pin settings are correctly configured in eief1-pcb-01.h

Promises:
  - pin signal set to LOW (~0V)
*/
static void ANTRCC_R_SignalOff(u32 pin)
{
  u32 *pu32ToggleGPIO= (u32*)(&(AT91C_BASE_PIOA->PIO_CODR));
  *pu32ToggleGPIO = pin;
  
} /* end ANTRCC_R_SignalOff() */



/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_RSM_RobotActive

Description:
Robot is currently receiving direction messages from the controller

Requires:
  - ANT setup successful

Promises:
  - call different direction functions based on bytes of the message
  - go to stalled state if all message bytes are 0x00
*/
static void ANTRCC_RSM_RobotActive(void)
{
  
  // FORWARD AND LEFT (WHITE)
  if(ANTRCC_R_u8DirectionMsg[0] == 0xFF && ANTRCC_R_u8DirectionMsg[2] == 0xFF)
  {
    ANTRCC_R_ForwardLeft();
  }
  // FORWARD AND RIGHT (WHITE)
  else if(ANTRCC_R_u8DirectionMsg[0] == 0xFF && ANTRCC_R_u8DirectionMsg[3] == 0xFF)
  {
    ANTRCC_R_ForwardRight();
  }
  // FORWARD (GREEN)
  else if(ANTRCC_R_u8DirectionMsg[0] == 0xFF)
  {
    ANTRCC_R_Forward();
  }
  
  // BACKWARD AND LEFT (WHITE)
  else if(ANTRCC_R_u8DirectionMsg[1] == 0xFF && ANTRCC_R_u8DirectionMsg[2] == 0xFF)
  {
    ANTRCC_R_BackwardLeft();
  }
  // BACKWARD AND RIGHT (WHITE)
  else if(ANTRCC_R_u8DirectionMsg[1] == 0xFF && ANTRCC_R_u8DirectionMsg[3] == 0xFF)
  {
    ANTRCC_R_BackwardRight();
  }
  // BACKWARD (YELLOW)
  else if(ANTRCC_R_u8DirectionMsg[1] == 0xFF)
  {
    ANTRCC_R_Backward();
  }
  
  // LEFT TURN (PURPLE)
  else if(ANTRCC_R_u8DirectionMsg[2] == 0xFF)
  {
    ANTRCC_R_LeftTurn();
  }
  // RIGHT TURN (BLUE)
  else if(ANTRCC_R_u8DirectionMsg[3] == 0xFF)
  {
    ANTRCC_R_RightTurn();
  }
  
  // STALLED (RED)
  else
  {
    ANTRCC_R_StateMachine = ANTRCC_RSM_RobotStalled;
  }
  
  ANTRCC_R_AntReadMessage();
  
} /* end ANTRCC_RSM_RobotActive() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_RSM_RobotStalled

Description:
Robot is stopped and not receiving user input

Requires:
  - 

Promises:
  - Robot is still/stopped
  - LCD is RED
*/
static void ANTRCC_RSM_RobotStalled(void)
{
  
  ANTRCC_R_ColorLCD(TRUE,FALSE,FALSE);
  
  ANTRCC_R_SignalOff(DIR_REV);
  ANTRCC_R_SignalOff(DIR_FWD);
  ANTRCC_R_SignalOff(EN_LEFT);
  ANTRCC_R_SignalOff(EN_RIGHT);
  
  ANTRCC_R_StateMachine = ANTRCC_RSM_RobotActive;
  
  
} /* end ANTRCC_RSM_RobotStalled() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_RSM_Error

Description:
application error occured

Requires:
  - display error message and blink red LED before switching to this state

Promises:
  - 
*/
static void ANTRCC_RSM_Error(void)
{
  // PRESS RESET BUTTON
  
} /* end ANTRCC_RSM_Error() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_RSM_AntChannelAssign

Description:
Waiting to connect to a master/controller

Requires:
  - ANT channel info is setup correctly
  - user has pressed BUTTON0

Promises:
  - go to active robot state if ANT is configured properly
  - go to communication interupt state after a certain amount of time
*/
static void ANTRCC_RSM_AntChannelAssign(void)
{
  
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    ANTRCC_R_StateMachine = ANTRCC_RSM_RobotActive;
  }
  
  ANTRCC_R_u32Timeout++;
  
  if(ANTRCC_R_u32Timeout == 5000)
  {
    LedBlink(RED, LED_2HZ);
  
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LCDMessage(LINE1_START_ADDR, "ANT Not Configured");
    
    ANTRCC_R_StateMachine = ANTRCC_RSM_Error;
  }
  
} /* end ANTRCC_RSM_AntChannelAssign() */


/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_RSM_AntStartup

Description:
Wait for the user to press BUTTON0

Requires:
  -

Promises:
  - calls ANT initializing function when user presses BUTTON0
  - go to channel assign state if initialization is successful
  - go to communication interupt state if unsuccessful
*/
static void ANTRCC_RSM_AntStartup(void)
{
  
  LedOn(RED);
  
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    ANTRCC_R_AntInitialize();
    
    if(AntAssignChannel(&ANTRCC_R_sChannelInfo))
    {
      ANTRCC_R_StateMachine = ANTRCC_RSM_AntChannelAssign;
    }
    else
    {
      LedBlink(RED, LED_2HZ);
  
      LCDCommand(LCD_CLEAR_CMD);
      for(u32 i = 0; i < 10000; i++);
      LCDMessage(LINE1_START_ADDR, "Assign Channel Error");
  
      ANTRCC_R_StateMachine = ANTRCC_RSM_Error;
    }
  }
  
} /* end ANTRCC_RSM_AntStartup() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
