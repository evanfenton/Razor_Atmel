/**********************************************************************************************************************
File: antrcc_controller_cv.c      

Description:
A file for enGENIUS camp participants to write their RC Car Controller code
(code for responding to user input and sending the appropriate messages to the receiver)

YOU WILL BE WRITING THE EVENT HANDLING LOGIC FOR THE CONTROLLER

IN OUR CASE THE "EVENT" IS THE USERS INPUT WHICH IS GOING TO BE BUTTON PRESSING

BELOW IS A REFERENCE TO THE FUNCTIONS YOU'LL NEED TO CALL 

WHEN YOU ARE READY TO START GO TO LINE ___

***********************************************************************************************************************

--------------------------------
Function To Be Edited By Campers
--------------------------------

void Controller


***********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_ANTRCC_C"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ANTRCC_CFlags;                       /* Global state flags */

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
static fnCode_type ANTRCC_C_StateMachine;            /* The state machine function pointer */

AntAssignChannelInfoType sChannelInfo;
static u32 ANTRCC_C_u32Timeout = 0;                      /* Timeout counter used across states */
static u8 u8DirectionMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

/*--------------------------------------------------------------------------------------------------------------------*/
/* Functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/* ANT FUNCTIONS */
static void ANTRCC_C_AntInit(void)
{
  sChannelInfo.AntChannel          = ANT_CHANNEL_ANTRCC;
  sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_ANTRCC;
  sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_ANTRCC;
  sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_ANTRCC;
  sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_ANTRCC;
  sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_ANTRCC;
  sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_ANTRCC;
  sChannelInfo.AntFrequency        = ANT_FREQUENCY_ANTRCC;
  sChannelInfo.AntTxPower          = ANT_TX_POWER_ANTRCC;
  sChannelInfo.AntNetwork          = ANT_NETWORK_DEFAULT;
  sChannelInfo.AntFlags            = 1;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
}

static void ANTRCC_C_AntMasterConfig(void)
{
  ANTRCC_C_AntInit();
  sChannelInfo.AntChannelType = CHANNEL_TYPE_MASTER;
  if(AntAssignChannel(&sChannelInfo))
  {
    ANTRCC_C_u32Timeout++;
    ANTRCC_C_StateMachine = ANTRCC_CSM_ANT_ChannelAssign;
    
    if(ANTRCC_C_u32Timeout == 5000)
    {
      LedBlink(RED, LED_4HZ);
      ANTRCC_C_StateMachine = ANTRCC_CSM_Error;
    }
  }
  else
  {
    LedOn(RED);
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 u32I = 0; u32I < 10000; u32I++);
    LCDMessage(LINE1_START_ADDR, "Failed Master");
    ANTRCC_C_StateMachine = ANTRCC_CSM_Error;
  }
}

static void ANTRCC_C_AntSendDirections(void)
{
  if( AntReadAppMessageBuffer() )
  {
    AntQueueBroadcastMessage(ANT_CHANNEL_ANTRCC, u8DirectionMsg);
  }
}

/* DIRECTION FUNCTIONS */

static void ANTRCC_C_Forward(void)
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

static void ANTRCC_C_Backward(void)
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

static void ANTRCC_C_ForwardLeft(void)
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
}

static void ANTRCC_C_BackwardLeft(void)
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
}

static void ANTRCC_C_LeftTurn(void)
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

static void ANTRCC_C_RightTurn(void)
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
}

static void ANTRCC_C_ForwardRight(void)
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
}

static void ANTRCC_C_BackwardRight(void)
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
}

static void ANTRCC_C_Stalled(void)
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

void Forward(void)
{
  ANTRCC_C_Forward();
}

void Backward(void)
{
  ANTRCC_C_Backward();
}

void Left(void)
{
  ANTRCC_C_LeftTurn();
}

void Right(void)
{
  ANTRCC_C_RightTurn();
}

void ForwardLeft(void)
{
  ANTRCC_C_ForwardLeft();
}

void ForwardRight(void)
{
  ANTRCC_C_ForwardRight();
}

void BackwardLeft(void)
{
  ANTRCC_C_BackwardLeft();
}

void BackwardRight(void)
{
  ANTRCC_C_BackwardRight();
}

void Stop(void)
{
  ANTRCC_C_Stalled();
}

void SendDirections(void)
{
  ANTRCC_C_AntSendDirections();
}

/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CInitialize */
void ANTRCC_CInitialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 u32I = 0; u32I < 10000; u32I++);
  
  LCDMessage(LINE1_START_ADDR, "Press Button 0 to");
  LCDMessage(LINE2_START_ADDR, "connect to RC car");
  
  ANTRCC_C_StateMachine = ANTRCC_CSM_MasterConfig;
} /* end ANTRCC_CInitialize() */

/*----------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CRunActiveState() */
void ANTRCC_CRunActiveState(void)
{
  ANTRCC_C_StateMachine();
} /* end ANTRCC_CRunActiveState */

static void ANTRCC_CSM_MasterConfig(void)
{
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    ANTRCC_C_AntMasterConfig();
  }
}/* end ANTRCC_CSM_MasterConfig() */

/*----------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CSM_ANT_ChannelAssign() */
static void ANTRCC_CSM_ANT_ChannelAssign(void)
{
  if(AntRadioStatusChannel(ANT_CHANNEL_ANTRCC) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_ANTRCC);
    LCDCommand(LCD_CLEAR_CMD);
    for(u32 i = 0; i < 10000; i++);
    LCDMessage(LINE2_START_ADDR, "^     v      <     >");  
    ANTRCC_C_StateMachine = ANTRCC_CSM_SendData;
  }
  ANTRCC_C_u32Timeout++;
  if(ANTRCC_C_u32Timeout == 5000)
  {
    LedBlink(RED, LED_2HZ);
    ANTRCC_C_StateMachine = ANTRCC_CSM_Error;
  }
} /* end ANTRCC_CSM_ChannelAssign() */

/*----------------------------------------------------------------------------------------------------------------------
Function: ANTRCC_CSM_Error() */
static void ANTRCC_CSM_Error(void)          
{
  
} /* end ANTRCC_CSM_Error() */

/*----------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------

This is the portion of Code for the students to complete

------------------------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------
----------------------------------------------------------------------------------------------------------------------*/
static void ANTRCC_CSM_SendData(void)
{
  if(IsButtonPressed(BUTTON0) && IsButtonPressed(BUTTON2))
  {
    ForwardLeft();
  }
  else if(IsButtonPressed(BUTTON0) && IsButtonPressed(BUTTON3))
  {
    ForwardRight();
  }
  else if(IsButtonPressed(BUTTON0))
  {
    Forward();
  }
  else if(IsButtonPressed(BUTTON1) && IsButtonPressed(BUTTON2))
  {
    BackwardLeft();
  }
  else if(IsButtonPressed(BUTTON1) && IsButtonPressed(BUTTON3))
  {
    BackwardRight();
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
    Stop();
  }
  SendDirections();
} /* end ANTRCC_CSM_Idle() */
     
/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/