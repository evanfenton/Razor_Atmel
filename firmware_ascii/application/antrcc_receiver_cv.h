/**********************************************************************************************************************
File: antrcc_receiver_cv.h                                                                

Description:
Header file for antrcc_receiver_cv.c

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

#define BACKUP_BEEP_FREQ                (u16)1000
#define BACKUP_BEEP_ON                  (u16)600
#define BACKUP_BEEP_OFF                 (u16)1200


/**********************************************************************************************************************
Function Declarations
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
void Receiver_Initialize(void);

static void ANT_Initialize(void);

/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/

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

static void Color_LCD( bool red, bool green, bool blue );
static void ANT_ReadMessage(void);

/* Pin Signals */
static void SignalOn(u32 pin);
static void SignalOff(u32 pin);

static void Backup_Beep(void);

/***********************************************************************************************************************
State Machine Declarations
***********************************************************************************************************************/
static void Receiver_Active(void);
static void Receiver_Stalled(void);
static void ANT_ChannelAssign(void);
static void Receiver_CommInterupt(void);       
static void ANT_Startup(void);


#endif /* __ANTRCC_R_H */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
