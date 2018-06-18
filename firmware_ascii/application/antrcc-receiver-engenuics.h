/**********************************************************************************************************************
File: antrcc-receiver-engenuics.h                                                                

----------------------------------------------------------------------------------------------------------------------

Description:
Header file for antrcc-receiver-engenuics.c  

**********************************************************************************************************************/

#ifndef __ANTRCC_R_H
#define __ANTRCC_R_H

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


#define DIR_REV                         PA_12_BLADE_UPOMI /* TX */
#define DIR_FWD                         PA_11_BLADE_UPIMO /* RX */

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


#endif /* __ANTRCC_R_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
