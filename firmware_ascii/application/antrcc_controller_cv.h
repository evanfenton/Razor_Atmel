/**********************************************************************************************************************
File: user_app1.h                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app1 as a template:
1. Follow the instructions at the top of user_app1.c
2. Use ctrl-h to find and replace all instances of "user_app1" with "yournewtaskname"
3. Use ctrl-h to find and replace all instances of "ANTRCC_C" with "YourNewTaskName"
4. Use ctrl-h to find and replace all instances of "USER_APP1" with "YOUR_NEW_TASK_NAME"
5. Add #include yournewtaskname.h" to configuration.h
6. Add/update any special configurations required in configuration.h (e.g. peripheral assignment and setup values)
7. Delete this text (between the dashed lines)
----------------------------------------------------------------------------------------------------------------------

Description:
Header file for user_app1.c

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
#define ANT_DEVICEID_LO_ANTRCC         (u8)0x21              // Device # Low byte
#define ANT_DEVICEID_HI_ANTRCC         (u8)0x00              // Device # High byte
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
void ANTRCC_CInitialize(void);
void ANTRCC_CRunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
/* ANT Functions */
static void ANTRCC_C_AntInit(void);
static void ANTRCC_C_AntMasterConfig(void);
static void ANTRCC_C_AntSendDirections(void);

void SendDirections(void);

/* Engenuics Standard Functions */
static void ANTRCC_C_Forward(void);
static void ANTRCC_C_Backward(void);
static void ANTRCC_C_ForwardLeft(void);
static void ANTRCC_C_BackwardLeft(void);
static void ANTRCC_C_LeftTurn(void);
static void ANTRCC_C_RightTurn(void);
static void ANTRCC_C_ForwardRight(void);
static void ANTRCC_C_BackwardRight(void);
static void ANTRCC_C_Stalled(void);

/* CV Functions */
void Forward(void);
void Backward(void);
void Left(void);
void Right(void);
void Stop(void);
void ForwardLeft(void);
void ForwardRight(void);
void BackwardLeft(void);
void BackwardRight(void);


/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void ANTRCC_CSM_SendData(void);    
static void ANTRCC_CSM_ANT_ChannelAssign(void);
static void ANTRCC_CSM_Error(void);         
static void ANTRCC_CSM_MasterConfig(void);


#endif /* __ANTRCC_C_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
