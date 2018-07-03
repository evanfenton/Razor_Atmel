/**********************************************************************************************************************
File: antrcc_receiver_cv.c                                                                

Description:
A file for enGENUIS camp participants to write their RC Car Receiver code
(code to take controller commands and send corresponding signals to the motor driver)

THE FUNCTIONS YOU WILL NEED TO CALL TO MAKE THE ROBOT WORK ARE SHOWN BELOW

SIGNAL VARIABLES HAVE ALSO BEEN DEFINED FOR YOU SO MAKE SURE TO PROVIDE THE
NECESSARY ARGUMENTS TO THE FUNCTIONS BEING CALLED

***********************************************************************************************************************------------------------------------------------------------------------------------------------------------------------

----------------------------------------------
Direction Functions To Be Completed By Campers
----------------------------------------------

void Forward();
void Backward();
void Left();
void Right();
void Stalled();

----------------
Signal Functions
----------------

void SignalOn( int pin );
void SignalOff( int pin );

------------------------------
Signal Variables (of type int)
------------------------------

DIR_REV
DIR_FWD
EN_LEFT
EN_RIGHT

---------------
State Functions
---------------

void Receiver_Active();

void Receiver_Stalled();
void Receiver_CommInterupt();
void ANT_Startup();
void ANT_ChannelAssign();

----------------------
Initializing Functions
----------------------

void Receiver_Initialize();
void ANT_Initialize();

---------------
Other Functions
---------------

void ANT_ReadMessage();

void Color_LCD( bool red, bool green, bool blue );

void Backup_Beep();

void Forward_Left();
void Forward_Right();
void Backward_Left();
void Backward_Right();



CAMP ENGENIUS STUDENTS MAY IGNORE THE FOLLOWING VARIABLE DEFINITIONS


SKIP TO LINE 148



**********************************************************************************************************************/

#include "configuration.h"

/**********************************************************************************************************************
Variable Definitions 
**********************************************************************************************************************/


/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_Receiver"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ReceiverFlags;                       /* Global state flags */
volatile fnCode_type Receiver_StateMachine;            /* The state machine function pointer */

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
Variable names shall start with "Receiver_" and be declared as static.
***********************************************************************************************************************/

static u32 Receiver_u32Timeout;                       /* Timeout counter used across states */

static u32 Receiver_u32BackupTimer;


static u8 u8DirectionMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA };

static u8 FL_counter;
static u8 FR_counter;
static u8 BL_counter;
static u8 BR_counter;

AntAssignChannelInfoType Receiver_sChannelInfo;




/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Direction Functions To Be Completed By Campers                                                                                                    */
/*--------------------------------------------------------------------------------------------------------------------*/

static void Forward(void)
{
  
  // INSERT CODE HERE
  
}

static void Backward(void)
{
  
  // AND HERE
  
}

static void Left(void)
{
  
  // AND HERE
  
}

static void Right(void)
{
  
  // AND HERE
  
}



/*--------------------------------------------------------------------------------------------------------------------*/
/* Signal Functions                                                                                              */
/*--------------------------------------------------------------------------------------------------------------------*/

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

/*--------------------------------------------------------------------------------------------------------------------*/
/* State Functions                                                                                              */
/*--------------------------------------------------------------------------------------------------------------------*/

static void Receiver_Active(void)
{
  
  // FORWARD AND LEFT (WHITE)
  if(u8DirectionMsg[0] == 0xFF && u8DirectionMsg[2] == 0xFF)
  {
    Color_LCD(TRUE,TRUE,TRUE);
    Forward_Left();
  }
  // FORWARD AND RIGHT (WHITE)
  else if(u8DirectionMsg[0] == 0xFF && u8DirectionMsg[3] == 0xFF)
  {
    Color_LCD(TRUE,TRUE,TRUE);
    Forward_Right();
  }
  // FORWARD (GREEN)
  else if(u8DirectionMsg[0] == 0xFF)
  {
    Color_LCD(FALSE,TRUE,FALSE);
    Forward();
  }
  
  // BACKWARD AND LEFT (WHITE)
  else if(u8DirectionMsg[1] == 0xFF && u8DirectionMsg[2] == 0xFF)
  {
    Color_LCD(TRUE,TRUE,TRUE);
    Backward_Left();
  }
  // BACKWARD AND RIGHT (WHITE)
  else if(u8DirectionMsg[1] == 0xFF && u8DirectionMsg[3] == 0xFF)
  {
    Color_LCD(TRUE,TRUE,TRUE);
    Backward_Right();
  }
  // BACKWARD (YELLOW)
  else if(u8DirectionMsg[1] == 0xFF)
  {
    Color_LCD(TRUE,TRUE,FALSE);
    Backward();
  }
  
  // LEFT TURN (PURPLE)
  else if(u8DirectionMsg[2] == 0xFF)
  {
    Color_LCD(TRUE,FALSE,TRUE);
    Left();
  }
  // RIGHT TURN (BLUE)
  else if(u8DirectionMsg[3] == 0xFF)
  {
    Color_LCD(FALSE,FALSE,TRUE);
    Right();
  }
  
  // STALLED (RED)
  else
  {
    Receiver_StateMachine = Receiver_Stalled;
  }
  
  ANT_ReadMessage();
  
}

static void Receiver_Stalled(void)
{
  
  // LCD IS RED
  Color_LCD(TRUE,FALSE,FALSE);
  
  // RC CAR IS STOPPED
  SignalOff(DIR_REV);
  SignalOff(DIR_FWD);
  SignalOff(EN_LEFT);
  SignalOff(EN_RIGHT);
  
  Receiver_StateMachine = Receiver_Active;
  
}

static void Receiver_CommInterupt(void)
{
  
  LCDCommand(LCD_CLEAR_CMD);
  for(u32 i = 0; i < 10000; i++);
  LedBlink(RED, LED_2HZ);
  LCDMessage(LINE1_START_ADDR, "Error");
  
}

static void ANT_Startup(void)
{
  
  LedOn(RED);
  
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    ANT_Initialize();
    
    if(AntAssignChannel(&Receiver_sChannelInfo))
    {
      Receiver_StateMachine = ANT_ChannelAssign;
    }
    else
    {
      Receiver_StateMachine = Receiver_CommInterupt;
    }
  }
  
}

static void ANT_ChannelAssign(void)
{
  
  if(AntRadioStatusChannel(ANT_CHANNEL_USERAPP) == ANT_CONFIGURED)
  {
    AntOpenChannelNumber(ANT_CHANNEL_USERAPP);
    LedOff(GREEN);
    Receiver_StateMachine = Receiver_Active;
  }
  
  Receiver_u32Timeout++;
  
  if(Receiver_u32Timeout == 5000)
  {
    LedBlink(RED, LED_2HZ);
    Receiver_StateMachine = Receiver_CommInterupt;
  }
  
}

/*--------------------------------------------------------------------------------------------------------------------*/
/* Initializing Functions                                                                                              */
/*--------------------------------------------------------------------------------------------------------------------*/

void Receiver_Initialize(void)
{
  LCDCommand(LCD_CLEAR_CMD);
  
  for(u32 u32i = 0; u32i < 10000; u32i++);
  
  LCDMessage(LINE1_START_ADDR, "Press Button 0 to");
  LCDMessage(LINE2_START_ADDR, "connect controller");
  
  FL_counter= 0;
  FR_counter= 0;
  BL_counter= 0;
  BR_counter= 0;
  
  
  Receiver_StateMachine = ANT_Startup;
}

static void ANT_Initialize(void)
{
  
  Receiver_sChannelInfo.AntChannel          = ANT_CHANNEL_USERAPP;
  Receiver_sChannelInfo.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  Receiver_sChannelInfo.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  Receiver_sChannelInfo.AntDeviceIdLo       = ANT_DEVICEID_LO_USERAPP;
  Receiver_sChannelInfo.AntDeviceIdHi       = ANT_DEVICEID_HI_USERAPP;
  Receiver_sChannelInfo.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  Receiver_sChannelInfo.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  Receiver_sChannelInfo.AntFrequency        = ANT_FREQUENCY_USERAPP;
  Receiver_sChannelInfo.AntTxPower          = ANT_TX_POWER_USERAPP;
  Receiver_sChannelInfo.AntNetwork          = ANT_NETWORK_DEFAULT;
  
  Receiver_sChannelInfo.AntChannelType      = CHANNEL_TYPE_SLAVE;
  
  Receiver_sChannelInfo.AntFlags            = 1;
  
  for(u8 i = 0; i < ANT_NETWORK_NUMBER_BYTES; i++)
  {
    Receiver_sChannelInfo.AntNetworkKey[i] = ANT_DEFAULT_NETWORK_KEY;
  }
  
}


/*--------------------------------------------------------------------------------------------------------------------*/
/* Other Functions                                                                                              */
/*--------------------------------------------------------------------------------------------------------------------*/


static void ANT_ReadMessage(void)
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
    Receiver_StateMachine = Receiver_CommInterupt;
  }  
  
}


static void Color_LCD( bool red, bool green, bool blue )
{
  // LCD RED
  if(red){
    LedOn(LCD_RED);
  }
  else{
    LedOff(LCD_RED);
  }
  
  // LCD GREEN
  if(green){
    LedOn(LCD_GREEN);
  }
  else{
    LedOff(LCD_GREEN);
  }
  
  // LCD BLUE
  if(blue){
    LedOn(LCD_BLUE);
  }
  else{
    LedOff(LCD_BLUE);
  }
  
}


static void Backup_Beep(void)
{
  
  Receiver_u32BackupTimer++;
  
  if(Receiver_u32BackupTimer < BACKUP_BEEP_ON)
  {
    PWMAudioOn(BUZZER1);
  }
  else if(Receiver_u32BackupTimer < BACKUP_BEEP_OFF)
  {
    PWMAudioOff(BUZZER1);
  }
  else
  {
    Receiver_u32BackupTimer= 0;
  }
  
}


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



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
