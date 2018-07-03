/**********************************************************************************************************************
File: antrcc_controller_cv.h      

Description:

Header file for antrcc_controller_cv.c


enGENIUS CAMP LEADERS MAKE SURE DEVICE ID BELOW IS CHANGED FROM 0x1234 AND
IF NEEDED, THE MISSING CODE FOR THE ACTIVITY IS AT THE BOTTOM OF THE FILE


**********************************************************************************************************************/

#ifndef __ANTRCC_C_H
#define __ANTRCC_C_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/

/* Required constants for ANT channel configuration */
#define ANT_CHANNEL_ANTRCC             ANT_CHANNEL_0         // Channel 0 - 7
#define ANT_CHANNEL_PERIOD_LO_ANTRCC   (u8)0x66              // LO; 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_ANTRCC   (u8)0x06              // HI; 0x0001 - 0x7fff


/* enGENIUS setup person should change this device ID to be unique for each group 
   in BOTH controller and receiver code */

#define ANT_DEVICEID_LO_ANTRCC         (u8)0x34              // Device # Low byte
#define ANT_DEVICEID_HI_ANTRCC         (u8)0x12              // Device # High byte


#define ANT_DEVICE_TYPE_ANTRCC         (u8)1                 // 1 - 255
#define ANT_TRANSMISSION_TYPE_ANTRCC   (u8)1                 // 1-127; MSB is pairing
#define ANT_FREQUENCY_ANTRCC           (u8)50                // 2400MHz + 0 - 99 MHz
#define ANT_TX_POWER_ANTRCC            RADIO_TX_POWER_4DBM   // Max tx power



/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

void Controller_Initialize(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

/* ANT Functions */

static void ANT_Initialize(void);
static void ANT_SendDirection(void);


/* Direction Functions */

static void Forward(void);
static void Backward(void);
static void Left(void);
static void Right(void);

static void Stalled(void);

static void Forward_Left(void);
static void Forward_Right(void);
static void Backward_Left(void);
static void Backward_Right(void);


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/

static void Controller_Active(void);
static void Controller_Error(void);
static void ANT_Startup(void);
static void ANT_ChannelAssign(void);


#endif /* __ANTRCC_C_H */




/*     BELOW IS THE MISSING CODE FROM THE "CONTROLLER ACTIVE" FUNCTION 


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


*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
