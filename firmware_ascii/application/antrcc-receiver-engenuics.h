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


/* DEVICE ID MUST BE CHANGED FOR EACH CONTROLLER-RECEIVER PAIR 
   
   DEFAULT IS 0x1234
*/
#define ANT_DEVICEID_LO_USERAPP         (u8)0x34              // Device # Low byte
#define ANT_DEVICEID_HI_USERAPP         (u8)0x12              // Device # High byte


#define ANT_DEVICE_TYPE_USERAPP         (u8)1                 // 1 - 255
#define ANT_TRANSMISSION_TYPE_USERAPP   (u8)1                 // 1-127; MSB is pairing
#define ANT_FREQUENCY_USERAPP           (u8)50                // 2400MHz + 0 - 99 MHz
#define ANT_TX_POWER_USERAPP            RADIO_TX_POWER_4DBM   // Max tx power


/* signal names */

#define DIR_REV                         PA_12_BLADE_UPOMI /* TX */
#define DIR_FWD                         PA_11_BLADE_UPIMO /* RX */

#define EN_LEFT                         PA_13_BLADE_MISO /* MISO */
#define EN_RIGHT                        PA_14_BLADE_MOSI /* MOSI */


/* durations for combination directions */

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

void ANTRCC_R_Initialize(void);

void ANTRCC_R_RunActiveState(void);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

static void ANTRCC_R_AntInitialize(void);

static void ANTRCC_R_AntReadMessage(void);

static void ANTRCC_R_Forward(void);

static void ANTRCC_R_Backward(void);

static void ANTRCC_R_LeftTurn(void);

static void ANTRCC_R_RightTurn(void);


static void ANTRCC_R_ForwardLeft(void);

static void ANTRCC_R_ForwardRight(void);

static void ANTRCC_R_BackwardLeft(void);

static void ANTRCC_R_BackwardRight(void);


static void ANTRCC_R_ColorLCD(bool Red, bool Green, bool Blue);


static void ANTRCC_R_SignalOn(u32 pin);

static void ANTRCC_R_SignalOff(u32 pin);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/

static void ANTRCC_RSM_RobotActive(void);

static void ANTRCC_RSM_RobotStalled(void);

static void ANTRCC_RSM_Error(void);

static void ANTRCC_RSM_AntChannelAssign(void);

static void ANTRCC_RSM_AntStartup(void);

#endif /* __ANTRCC_R_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
