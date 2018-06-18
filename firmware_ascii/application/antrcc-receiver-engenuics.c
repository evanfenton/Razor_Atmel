/**********************************************************************************************************************
File: antrcc-receiver-engenuics.c                                                                

----------------------------------------------------------------------------------------------------------------------

Description:
This is the official code for the enGENIUS ANT RC Car following Engenuics standards

------------------------------------------------------------------------------------------------------------------------
API:

ANT
LEDS
BUTTONS
BUZZERS

Public functions:


Protected System functions:

void ANTRCC_R_Initialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void ANTRCC_R_RunActiveState(void)
Runs current task state.  Should only be called once in main loop.


Private Application functions:

static void ANTRCC_R_AntInitialize(void)
Runs required ANT initializaton for the task. Should be called once from ANT startup state.

static void ANTRCC_R_AntReadMessage(void)
Reads an incoming message from the controller.

static void ANTRCC_R_Forward(void)
Adjusts proper signals for forward motion of robot.

static void ANTRCC_R_Backward(void)
Adjusts proper signals for backward motion of robot.

static void ANTRCC_R_LeftTurn(void)
Adjusts proper signals for left turn of robot.

static void ANTRCC_R_RightTurn(void)
Adjusts proper signals for right turn of robot.





State Machine functions:

static void ANTRCC_RSM_RobotActive(void)
When user is giving input and robot is moving.

static void ANTRCC_RSM_RobotStalled(void)
When user is not giving input and robot is stopped.

static void ANTRCC_RSM_AntCommInterupt(void)
When receiver has been disconnected from a controller for a certain amount of time.

static void ANTRCC_RSM_AntChannelAssign(void)
Assigning channel and finalizing connection to controller.

static void ANTRCC_RSM_AntStartup(void)
Tests for user button press to start application.

**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_ANTRCC_R_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32ANTRCC_R_Flags;                       /* Global state flags */


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
Variable names shall start with "ANTRCC_R_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type ANTRCC_R_StateMachine;            /* The state machine function pointer */
static u32 ANTRCC_R_u32Timeout;                      /* Timeout counter used across states */

static u8 ANTRCC_R_u8DirectionMsg[ANT_DATA_BYTES]= { 0x00, 0x00, 0x00, 0x00, 0xAA, 0xAA, 0xAA, 0xAA };

/* ANTRCC_R_u8DirectionMsg[0] = FORWARD SIGNAL BYTE
   ANTRCC_R_u8DirectionMsg[1] = BACKWARD SIGNAL BYTE
   ANTRCC_R_u8DirectionMsg[2] = LEFT TURN SIGNAL BYTE
   ANTRCC_R_u8DirectionMsg[3] = RIGHT TURN SIGNAL BYTE
*/

//static u8 ANTRCC_R_FL_counter;
//static u8 ANTRCC_R_FR_counter;
//static u8 ANTRCC_R_BL_counter;
//static u8 ANTRCC_R_BR_counter;

static u8 ANTRCC_R_ComboCounter;

static AntAssignChannelInfoType ANTRCC_R_sChannelInfo;


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/
