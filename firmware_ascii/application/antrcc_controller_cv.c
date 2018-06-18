/**********************************************************************************************************************
File: antrcc_controller_cv.c      

Description:
A file for enGENIUS camp participants to write their RC Car Controller code
(code for responding to user input and sending the appropriate messages to the receiver)

YOU WILL BE WRITING THE EVENT HANDLING LOGIC FOR THE CONTROLLER

IN OUR CASE THE "EVENT" IS THE USERS INPUT WHICH IS GOING TO BE BUTTON PRESSING

BELOW IS A REFERENCE TO THE FUNCTIONS YOU'LL NEED TO CALL 

WHEN YOU ARE READY TO START GO TO LINE 121

***********************************************************************************************************************

--------------------------------
Function To Be Edited By Campers
--------------------------------

void Controller_Active();

---------------------------
Direction Message Functions
---------------------------

void Forward();
void Backward();
void Left();
void Right();
void Stalled();

void Forward_Left();
void Forward_Right();
void Backward_Left();
void Backward_Right();

---------------
State Functions
---------------

void Controller_Active();
void Controller_Error();
void ANT_Startup();
void ANT_ChannelAssign();

----------------------
Initializing Functions
----------------------

void Controller_Initialize();
void ANT_Initialize();

---------------
Other Functions
---------------

void ANT_SendDirection();



CAMP STUDENTS MAY IGNORE THE FOLLOWING VARIABLE DEFINITIONS


SKIP TO LINE ____



***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_Controller"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ControllerFlags;                       /* Global state flags */

volatile fnCode_type Controller_StateMachine;            /* The state machine function pointer */


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
Variable names shall start with "Controller_" and be declared as static.
***********************************************************************************************************************/

static u32 Controller_u32Timeout = 0;                      /* Timeout counter used across states */

static u8 u8DirectionMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static AntAssignChannelInfoType Controller_sChannelInfo;



/***********************************************************************************************************************
Function Definitions
***********************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Function To Be Edited By Campers                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/


static void Controller_Active(void)
{
  if(IsButtonPressed(BUTTON0) && IsButtonPressed(BUTTON2))
  {
    Forward_Left();
  }
  else if(IsButtonPressed(BUTTON0) && IsButtonPressed(BUTTON3))
  {
    Forward_Right();
  }
  else if(IsButtonPressed(BUTTON0))
  {
    Forward();
  }
  else if(IsButtonPressed(BUTTON1) && IsButtonPressed(BUTTON2))
  {
    Backward_Left();
  }
  else if(IsButtonPressed(BUTTON1) && IsButtonPressed(BUTTON3))
  {
    Backward_Right();
  }
  else if(IsButtonPressed(BUTTON1))
  {
    Backward();
  }
  else if(IsButtonPressed(BUTTON2))
  {
    Left();
  }
  else if(IsButtonPressed(BUTTON3))
  {
    Right();
  }
  else
  {
    Stalled();
  }
  
  ANT_SendDirection();
  
} /* end Controller_Active() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Direction Message Functions                                                                                              */
/*--------------------------------------------------------------------------------------------------------------------*/


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
  
} /* end Forward() */


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
  
} /* end Backward() */


static void Left(void)
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
  
} /* end Left() */


static void Right(void)
{
  LedOn(BLUE);
  LedOff(YELLOW);
  LedOff(GREEN);
  LedOff(RED);
  LedOff(PURPLE);
  u8DirectionMsg[0]= 0x00;
  u8DirectionMsg[1]= 0x00;
  u8DirectionMsg[2]= 0x00;
  u8DirectionMsg[3]= 0xFF;
  
} /* end Right() */


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
  
} /* end Stalled() */


static void Forward_Left(void)
{
  LedOn(GREEN);
  LedOff(RED);
  LedOff(YELLOW);
  LedOn(PURPLE);
  LedOff(BLUE);
  u8DirectionMsg[0]= 0xFF;
  u8DirectionMsg[1]= 0x00;
  u8DirectionMsg[2]= 0xFF;
  u8DirectionMsg[3]= 0x00;
  
} /* end Forward_Left() */


static void Forward_Right(void)
{
  LedOn(GREEN);
  LedOff(RED);
  LedOff(YELLOW);
  LedOff(PURPLE);
  LedOn(BLUE);
  u8DirectionMsg[0]= 0xFF;
  u8DirectionMsg[1]= 0x00;
  u8DirectionMsg[2]= 0x00;
  u8DirectionMsg[3]= 0xFF;
  
} /* end Forward_Right() */


static void Backward_Left(void)
{
  LedOff(GREEN);
  LedOff(RED);
  LedOn(YELLOW);
  LedOn(PURPLE);
  LedOff(BLUE);
  u8DirectionMsg[0]= 0x00;
  u8DirectionMsg[1]= 0xFF;
  u8DirectionMsg[2]= 0xFF;
  u8DirectionMsg[3]= 0x00;
  
} /* end Backward_Left() */


static void Backward_Right(void)
{
  LedOff(GREEN);
  LedOff(RED);
  LedOn(YELLOW);
  LedOff(PURPLE);
  LedOn(BLUE);
  u8DirectionMsg[0]= 0x00;
  u8DirectionMsg[1]= 0xFF;
  u8DirectionMsg[2]= 0x00;
  u8DirectionMsg[3]= 0xFF;
  
} /* end Backward_Right() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* State Functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/


static void Controller_Error(void)          
{
  
  LedBlink(RED, LED_2HZ);
  LCDCommand(LCD_CLEAR_CMD);
  for(u32 u32I = 0; u32I < 10000; u32I++);
  LCDMessage(LINE1_START_ADDR, "Error!");
  
} /* end Controller_Error() */


static void ANT_Startup(void)
{
  
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    ANT_Initialize();
    
    if(AntAssignChannel(&Controller_sChannelInfo))
    {
      Controller_StateMachine = ANT_ChannelAssign;
    }
    else
    {
      Controller_StateMachine = Controller_Error;
    }
    
  }
  
} /* end ANT_Startup() */


static void ANT_ChannelAssign(void)
{
  
  if(AntRadioStatusChannel(ANT_CHANNEL_ANTRCC) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_ANTRCC);
    
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LCDMessage(LINE2_START_ADDR, "^     v      <     >");  
    
    Controller_StateMachine = Controller_Active;
  }
  
  Controller_u32Timeout++;
  
  if(Controller_u32Timeout == 5000)
  {
    Controller_StateMachine = Controller_Error;
  }
  
} /* end ANT_ChannelAssign() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Initializing Functions                                                                                             */
/*--------------------------------------------------------------------------------------------------------------------*/


void Controller_Initialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 u32I = 0; u32I < 10000; u32I++);
  
  LCDMessage(LINE1_START_ADDR, "Press Button 0 to");
  LCDMessage(LINE2_START_ADDR, "connect to RC car");
  
  Controller_StateMachine = ANT_Startup;
  
} /* end Controller_Initialize() */


static void ANT_Initialize(void)
{
  
  Controller_sChannelInfo.AntChannel          = ANT_CHANNEL_ANTRCC;
  Controller_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_ANTRCC;
  Controller_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_ANTRCC;
  Controller_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_ANTRCC;
  Controller_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_ANTRCC;
  Controller_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_ANTRCC;
  Controller_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_ANTRCC;
  Controller_sChannelInfo.AntFrequency        = ANT_FREQUENCY_ANTRCC;
  Controller_sChannelInfo.AntTxPower          = ANT_TX_POWER_ANTRCC;
  Controller_sChannelInfo.AntNetwork          = ANT_NETWORK_DEFAULT;
  
  Controller_sChannelInfo.AntChannelType      = CHANNEL_TYPE_MASTER;
  
  Controller_sChannelInfo.AntFlags            = 1;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    Controller_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
} /* end ANT_Initialize() */



/*--------------------------------------------------------------------------------------------------------------------*/
/* Other Functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/


static void ANT_SendDirection(void)
{
  if( AntReadAppMessageBuffer() )
  {
    AntQueueBroadcastMessage(ANT_CHANNEL_ANTRCC, u8DirectionMsg);
  }
  
} /* end ANT_SendDirection() */




/***********************************************************************************************************************
End of File
***********************************************************************************************************************/

