/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "UserApp1" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

**********************************************************************************************************************/

#ifndef __USER_APP1_H
#define __USER_APP1_H

/**********************************************************************************************************************
Type Definitions
**********************************************************************************************************************/


/**********************************************************************************************************************
Constants / Definitions
**********************************************************************************************************************/

/* Required constants for ANT channel configuration */
#define ANT_CHANNEL_USERAPP             ANT_CHANNEL_0         // Channel 0 - 7
#define ANT_CHANNEL_PERIOD_LO_USERAPP   (u8)0x66              // LO; 0x0001 - 0x7fff
#define ANT_CHANNEL_PERIOD_HI_USERAPP   (u8)0x06              // HI; 0x0001 - 0x7fff
#define ANT_DEVICEID_LO_USERAPP         (u8)0x21              // Device # Low byte
#define ANT_DEVICEID_HI_USERAPP         (u8)0x00              // Device # High byte
#define ANT_DEVICE_TYPE_USERAPP         (u8)1                 // 1 - 255
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)1                 // 1-127; MSB is pairing
#define ANT_FREQUENCY_USERAPP           (u8)50                // 2400MHz + 0 - 99 MHz
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM   // Max tx power


#define INPUT_TOP                       PA_12_BLADE_UPOMI /* TX */
#define INPUT_BOT                       PA_11_BLADE_UPIMO /* RX */

#define EN_LEFT                         PA_13_BLADE_MISO /* MISO */
#define EN_RIGHT                        PA_14_BLADE_MOSI /* MOSI */

#define FIRST_DIR_RATE                  (u8)30
#define SEC_DIR_RATE                    (u8)50

/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void UserApp1Initialize(void);
void UserApp1RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void AntInit(void);
static void AntSlaveConfig(void);
static void AntReadMessage(void);

/* Directions */
static void Forward(void);
static void Backward(void);
static void Left(void);
static void Right(void);
static void Stalled(void);

/* Combinations */
static void Forward_Left(void);
static void Forward_Right(void);

static void Backward_Left(void);
static void Backward_Right(void);

static void CombinationLCD(void);

/* Pin Signals */
static void SignalOn(u32 pin);
static void SignalOff(u32 pin);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void UserApp1SM_Idle(void);    
static void UserApp1SM_ANT_ChannelAssign(void);
static void UserApp1SM_Error(void);         
static void UserApp1SM_Master_or_Slave(void);


#endif /* __USER_APP1_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
