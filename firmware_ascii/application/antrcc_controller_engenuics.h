/**********************************************************************************************************************
File: antrcc_controller_engenuics.h                                                                

----------------------------------------------------------------------------------------------------------------------

Description:
Header file for antrcc-receiver-engenuics.c  

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
void ANTRCC_C_Initialize(void);

void ANTRCC_C_RunActiveState(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/
static void ANTRCC_C_AntInitialize(void);

static void ANTRCC_C_AntSendMessage(void);

static void ANTRCC_C_MessageBytes(bool Forward, bool Backward, bool Left, bool Right);


static void ANTRCC_C_Forward(void);

static void ANTRCC_C_Backward(void);

static void ANTRCC_C_LeftTurn(void);

static void ANTRCC_C_RightTurn(void);

static void ANTRCC_C_Stalled(void);


static void ANTRCC_C_ForwardLeft(void);

static void ANTRCC_C_ForwardRight(void);

static void ANTRCC_C_BackwardLeft(void);

static void ANTRCC_C_BackwardRight(void);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void ANTRCC_CSM_RobotActive(void);

static void ANTRCC_CSM_Error(void);

static void ANTRCC_CSM_AntChannelAssign(void);

static void ANTRCC_CSM_AntStartup(void);

#endif /* __ANTRCC_C_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
