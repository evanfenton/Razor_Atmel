/***********************************************************************************************************************
* File: eief1-pcb-01.h                                                                
* 
* Description:
* This file provides header information for the eief1-pcb-01 board.
***********************************************************************************************************************/

#ifndef __EIEF1
#define __EIEF1

/***********************************************************************************************************************
Type Definitions
***********************************************************************************************************************/


/***********************************************************************************************************************
* Constants
***********************************************************************************************************************/
#define OSC_VALUE                 (u32)12000000
#define MAINCK                    OSC_VALUE
#define MULA                      (u32)7
#define DIVA                      (u32)1
#define PLLACK_VALUE              (u32)(MAINCK * (MULA + 1)) / DIVA         /* 96 MHz */
#define CPU_DIVIDER               (u32)2
#define CCLK_VALUE                PLLACK_VALUE / CPU_DIVIDER                /* 48 MHz */
#define MCK                       CCLK_VALUE
#define PERIPHERAL_DIVIDER        (u32)1
#define PCLK_VALUE                CCLK_VALUE / PERIPHERAL_DIVIDER           /* 48 MHz */
#define SYSTICK_DIVIDER           (u32)8


#define SYSTICK_COUNT             (u32)(0.001 * (CCLK_VALUE / SYSTICK_DIVIDER) )
/* To get 1 ms tick, need SYSTICK_COUNT to be 0.001 * SysTick Clock.  
Should be 6000 for 48MHz CCLK. */

#define RTC_INT_TIME              (u16)2            /* Half-seconds for RTC interrupt */
#define RTC_STOP_INT_TIME         (u16)2            /* Half-seconds for RTC interrupt */

#define OSC_STARTUP_TIMOUT        (u32)1000000      /* Timeout for oscillator to start up */
#define PLL_LOCK_TIMEOUT          (u32)4000000      /* Timeout for PLL0 to lock */


/***********************************************************************************************************************
* Macros
***********************************************************************************************************************/
#define WATCHDOG_BONE()     (AT91C_BASE_WDTC->WDTC_WDCR = WDT_CR_FEED)    /* Resets the watch dog countdown timer*/
#define HEARTBEAT_ON()      (AT91C_BASE_PIOA->PIO_CODR = PA_31_HEARTBEAT) /* Turns on Heartbeat LED */
#define HEARTBEAT_OFF()     (AT91C_BASE_PIOA->PIO_SODR = PA_31_HEARTBEAT) /* Turns off Heartbeat LED */

#define SD_CARD_INSERTED()  (!(AT91C_BASE_PIOA->PIO_PDSR & PA_02_SD_DETECT))   /* SD detect switch asserted */


/***********************************************************************************************************************
* Function Declarations
***********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public Functions */
/*--------------------------------------------------------------------------------------------------------------------*/
void PWMAudioSetFrequency(u32 u32Channel_, u16 u16Frequency_);
void PWMAudioOn(u32 u32Channel_);
void PWMAudioOff(u32 u32Channel_);


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected Functions */
/*--------------------------------------------------------------------------------------------------------------------*/
void ClockSetup(void);
void RealTimeClockSetup(void);
void SysTickSetup(void);
void SystemSleep(void);
void WatchDogSetup(void);
void GpioSetup(void);

void PWMSetupAudio(void);


/***********************************************************************************************************************
Perihperal Setup Initializations

Bookmarks:
@@@@@ Clock, Power Control, Systick and Watchdog setup values
!!!!! GPIO pin names
##### GPIO initial setup values
$$$$$ PWM and Timer setup values

***********************************************************************************************************************/

/***********************************************************************************************************************
@@@@@ Clock, Systick and Power Control setup values
***********************************************************************************************************************/
#define PMC_PCER_INIT (u32)0x27FCED73
/*
    31 [0] Reserved
    30 [0] "
    29 [1] AT91C_ID_UDPHS  USB Device High Speed clock enabled
    28 [0] AT91C_ID_HDMA   HDMA

    27 [0] AT91C_ID_ADC    10-bit ADC Controller (ADC) not enabled
    26 [1] AT91C_ID_ADC12B 12-bit ADC Controller (ADC12B) clock enabled
    25 [1] AT91C_ID_PWMC   Pulse Width Modulation Controller clock enabled
    24 [1] AT91C_ID_TC2    Timer Counter 2 clock enabled

    23 [1] AT91C_ID_TC1    Timer Counter 1 clock enabled
    22 [1] AT91C_ID_TC0    Timer Counter 0 clock enabled
    21 [1] AT91C_ID_SSC0   Serial Synchronous Controller 0 clock enabled
    20 [1] AT91C_ID_SPI0   Serial Peripheral Interface clock enabled

    19 [1] AT91C_ID_TWI1   TWI 1 clock enabled
    18 [1] AT91C_ID_TWI0   TWI 0 clock enabled
    17 [0] AT91C_ID_MCI0   Multimedia Card Interface not enabled
    16 [0] AT91C_ID_US3    USART 3 not enabled

    15 [1] AT91C_ID_US2    USART 2 clock enabled
    14 [1] AT91C_ID_US1    USART 1 clock enabled
    13 [1] AT91C_ID_US0    USART 0 clock enabled
    12 [0] AT91C_ID_PIOC   Parallel IO Controller C  not enabled

    11 [1] AT91C_ID_PIOB   Parallel IO Controller B clock enabled
    10 [1] AT91C_ID_PIOA   Parallel IO Controller A clock enabled
    09 [0] AT91C_ID_HSMC4  HSMC4 not enabled
    08 [1] AT91C_ID_DBGU   DBGU (standalone UART) clock enabled

    07 [0] AT91C_ID_EFC1   EFC1 not enabled
    06 [1] AT91C_ID_EFC0   EFC0 clock enabled
    05 [1] AT91C_ID_PMC    PMC clock enabled
    04 [1] AT91C_ID_WDG    WATCHDOG TIMER clock enabled

    03 [0] AT91C_ID_RTT    REAL TIME TIMER not enabled
    02 [0] AT91C_ID_RTC    REAL TIME CLOCK not enabled
    01 [1] AT91C_ID_RSTC   RESET CONTROLLER clock enabled
    00 [1] AT91C_ID_SUPC   SUPPLY CONTROLLER clock enabled
*/


#define MOR_KEY      (u32)(0x37 << 16)
#define PMC_MOR_INIT (u32)0x0037F019
/*
    31 [0] Reserved
    30 [0] "
    29 [0] "
    28 [0] "

    27 [0] "
    26 [0] "
    25 [0] CFDEN clock failure detector disabled
    24 [0] MOSCSEL internal main oscillator selected for now

    23 [0] KEY 0x37
    22 [0] "
    21 [1] "
    20 [1] "

    19 [0] "
    18 [1] "
    17 [1] "
    16 [1] "

    15 [1] MOSCXTST main crystal start-up time (based on 8 x the slow clock frequency)
    14 [1] "
    13 [1] "
    12 [1] "

    11 [0] "
    10 [0] "
    09 [0] "
    08 [0] "

    07 [0] Reserved
    06 [0] MOSCRCF 8MHz
    05 [0] "
    04 [1] "

    03 [1] MOSCRCEN main on-chip RC osc is on for now
    02 [0] WAITMODE disabled
    01 [0] MOSCXTBY main crystal osc is not bypassed
    00 [1] MOSCXTEN main crystal osc is enabled
*/


#define PMC_MCKR_INIT (u32)(0x00002011)
/*
    31 [0] Reserved
    30 [0] "
    29 [0] "
    28 [0] "

    27 [0] "
    26 [0] "
    25 [0] "
    24 [0] "

    23 [0] "
    22 [0] "
    21 [0] "
    20 [0] "

    19 [0] "
    18 [0] "
    17 [0] "
    16 [0] "

    15 [0] "
    14 [0] "
    13 [1] UPLLDIV UPLL divided by 2 
    12 [0] Reserved 

    11 [0] "
    10 [0] "
    09 [0] "
    08 [0] "

    07 [0] "
    06 [0] PRES processor clock prescaler selected clock divided by 2
    05 [0] 
    04 [1] 

    03 [0] 
    02 [0] 
    01 [0] CSS Master clock selection MAIN_CLK for now
    00 [1] 
*/

#define PMC_MCKR_PLLA (u32)(0x00002012)


/* The PLL DIVA and MULA coefficients set the PLLA output frequency.
The PLLA input frequency must be between 8 and 16 MHz so with 12MHz clock, DIVA must be 1.
Since we want PLLACK at 96 MHz:
(96 MHZ * DIVA / 12 MHz) - 1 = MULA = 7
*/

#define PMC_PLAAR_INIT (u32)0x20073F01
/* Bit Set Description
    31 [0] Reserved
    30 [0] "
    29 [1] Always 1
    28 [0] "

    27 [0] "
    26 [0] MULA
    25 [0] "
    24 [0] "


    23 [0] "
    22 [0] "
    21 [0] "
    20 [0] "

    19 [0] "
    18 [1] "
    17 [1] "
    16 [1] "

    15 [0] Reserved
    14 [0] "
    13 [1] PLLACOUNT
    12 [1] "

    11 [1] "
    10 [1] "
    09 [1] "
    08 [1] "

    07 [0] DIVA
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] "
    02 [0] "
    01 [0] "
    00 [1] "
*/


#define SLEEP_MODE_STATUS_CLEAR (u32)0xfffff0ff


#define SYSTICK_CTRL_INIT (u32)0x00000003
/* Bit Set Description
    31:20 Reserved 

    19 [0] Reserved
    18 [0] "
    17 [0] "
    16 [0] Countflag (read only)

    15 [0] Reserved
    14 [0] "
    13 [0] "
    12 [0] "

    11 [0] "
    10 [0] "
    09 [0] "
    08 [0] "

    07 [0] "
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] "
    02 [0] Clock source is CPU clock / 8
    01 [1] System tick interrupt on 
    00 [1] System tick is enabled 
*/


/* Watch Dog Values
The watchdog oscillator is on the internal 32k RC with a 128 prescaler = 3.9ms / tick.  For a minimum 5 second watchdog timeout, the watchdog
counter must be set at 1280. */

#define WDT_CR_FEED  (u32)0xA5000001
/*
    31 [1] Key
    30 [0] " 
    29 [1] "
    28 [0] "

    27 [0] "
    26 [1] "
    25 [0] "
    24 [1] "

    23 - 04 [0] Reserved

    03 [0] Reserved
    02 [0] "
    01 [0] "
    00 [1] WDRSTT Restart watchdog
*/


#define WDT_MR_INIT      (u32)0x1FFF0500
/*
    31 [0] Reserved
    30 [0] "
    29 [0] WDIDLEHLT Watchdog runs when system is idle
    28 [1] WDDBGHLT Watchdog stops in debug state

    27 [1] WDD Watchdog delta value (allow resets any time)
    26 [1] "
    25 [1] "
    24 [1] "

    23 [1] "
    22 [1] "
    21 [1] "
    20 [1] "

    19 [1] "
    18 [1] "
    17 [1] "
    16 [1] "

    15 [0] WDDIS Watchdog is enabled
    14 [0] WDRPROC watchdog reset processor off
    13 [0] WDRSTEN Watchdog reset enable off
    12 [0] WDFIEN Watchdog fault interrupt enable off

    11 [0] WDV Watchdog counter value: 0x500 = 1280 x (128 x 1/32768) = 5 seconds
    10 [1] "
    09 [0] "
    08 [1] "

    07 [0] "
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] "
    02 [0] "
    01 [0] "
    00 [0] "
*/


/***********************************************************************************************************************
!!!!! GPIO pin names
***********************************************************************************************************************/
/* Hardware Definition for PCB EIEF1-PCB-01 */

/* Port A bit positions */
#define PA_31_HEARTBEAT          (u32)0x80000000
#define PA_30_AN_DEMO            (u32)0x40000000
#define PA_29_BUZZER2            (u32)0x20000000
#define PA_28_BUZZER1            (u32)0x10000000
#define PA_27_CLOCK_OUT          (u32)0x08000000
#define PA_26_ANT_PWR_EN         (u32)0x04000000 
#define PA_25_ANT_USPI2_SCK      (u32)0x02000000
#define PA_24_SD_USPI1_SCK       (u32)0x01000000
#define PA_23_ANT_USPI2_MOSI     (u32)0x00800000
#define PA_22_ANT_USPI2_MISO     (u32)0x00400000 
#define PA_21_SD_USPI1_MISO      (u32)0x00200000
#define PA_20_SD_USPI1_MOSI      (u32)0x00100000
#define PA_19_DEBUG_U0_PIMO      (u32)0x00080000
#define PA_18_DEBUG_U0_POMI      (u32)0x00040000
#define PA_17_BUTTON0            (u32)0x00020000
#define PA_16_BLADE_CS           (u32)0x00010000
#define PA_15_BLADE_SCK          (u32)0x00008000
#define PA_14_BLADE_MOSI         (u32)0x00004000
#define PA_13_BLADE_MISO         (u32)0x00002000

#define PA_12_BLADE_UPOMI        (u32)0x00001000
#define PA_11_BLADE_UPIMO        (u32)0x00000800
#define PA_10_I2C_SCL            (u32)0x00000400
#define PA_09_I2C_SDA            (u32)0x00000200
#define PA_08_SD_CS_MCDA3        (u32)0x00000100
#define PA_07_HSMCI_MCDA2        (u32)0x00000080
#define PA_06_HSMCI_MCDA1        (u32)0x00000040
#define PA_05_HSMCI_MCDA0        (u32)0x00000020
#define PA_04_HSMCI_MCCDA        (u32)0x00000010
#define PA_03_HSMCI_MCCK         (u32)0x00000008
#define PA_02_SD_DETECT          (u32)0x00000004
#define PA_01_SD_WP              (u32)0x00000002 
#define PA_00_TP54               (u32)0x00000001 


/* Port B bit positions */
#define PB_31_                  (u32)0x80000000
#define PB_30_                  (u32)0x40000000
#define PB_29_                  (u32)0x20000000
#define PB_28_                  (u32)0x10000000
#define PB_27_                  (u32)0x08000000
#define PB_26_                  (u32)0x04000000
#define PB_25_                  (u32)0x02000000
#define PB_24_ANT_SRDY          (u32)0x01000000
#define PB_23_ANT_MRDY          (u32)0x00800000
#define PB_22_ANT_USPI2_CS      (u32)0x00400000
#define PB_21_ANT_RESET         (u32)0x00200000
#define PB_20_LED_RED           (u32)0x00100000
#define PB_19_LED_GRN           (u32)0x00080000
#define PB_18_LED_BLU           (u32)0x00040000
#define PB_17_LED_YLW           (u32)0x00020000
#define PB_16_LED_CYN           (u32)0x00010000
#define PB_15_LED_ORG           (u32)0x00008000
#define PB_14_LED_PRP           (u32)0x00004000
#define PB_13_LED_WHT           (u32)0x00002000
#define PB_12_LCD_BL_BLU        (u32)0x00001000
#define PB_11_LCD_BL_GRN        (u32)0x00000800
#define PB_10_LCD_BL_RED        (u32)0x00000400
#define PB_09_LCD_RST           (u32)0x00000200
#define PB_08_TP62              (u32)0x00000100 
#define PB_07_TP60              (u32)0x00000080
#define PB_06_TP58              (u32)0x00000040
#define PB_05_TP56              (u32)0x00000020
#define PB_04_BLADE_AN1         (u32)0x00000010
#define PB_03_BLADE_AN0         (u32)0x00000008
#define PB_02_BUTTON3           (u32)0x00000004
#define PB_01_BUTTON2           (u32)0x00000002
#define PB_00_BUTTON1           (u32)0x00000001


#define ADC12_POTENTIOMETER     ADC12_CH1
#define ADC12_BLADE_AN0         ADC12_CH2
#define ADC12_BLADE_AN1         ADC12_CH3
      

/***********************************************************************************************************************
##### GPIO setup values
***********************************************************************************************************************/

/* PIO Enable Register: 
0: No effect
1: Pin is controlled by PIO
*/
#define PIOA_PER_INIT (u32)0x84031807
/* 
    31 [1] PA_31_HEARTBEAT PIO control enabled
    30 [0] PA_30_AN_DEMO PIO control not enabled
    29 [0] PA_29_BUZZER2 PIO control not enabled
    28 [0] PA_28_BUZZER1 PIO control not enabled

    27 [0] PA_27_CLOCK_OUT PIO control not enabled
    26 [1] PA_26_ANT_PWR_EN PIO control enabled
    25 [0] PA_25_ANT_USPI2_SCK PIO control not enabled
    24 [0] PA_24_SD_USPI1_SCK PIO control not enabled

    23 [0] PA_23_ANT_USPI2_MOSI PIO control not enabled
    22 [0] PA_22_ANT_USPI2_MISO PIO control not enabled
    21 [0] PA_21_SD_USPI1_MISO PIO control not enabled
    20 [0] PA_20_SD_USPI1_MOSI PIO control not enabled

    19 [0] PA_19_DEBUG_U0_PIMO PIO control not enabled
    18 [0] PA_18_DEBUG_U0_POMI PIO control not enabled
    17 [1] PA_17_BUTTON0 PIO control enabled
    16 [1] PA_16_BLADE_CS PIO control enabled

    15 [0] PA_15_BLADE_SCK PIO control not enabled
    14 [0] PA_14_BLADE_MOSI PIO control not enabled
    13 [0] PA_13_BLADE_MISO PIO control not enabled

    12 [1] PA_12_BLADE_UPOMI PIO control enabled


    11 [1] PA_11_BLADE_UPIMO PIO control enabled

    10 [0] PA_10_I2C_SCL PIO control not enabled
    09 [0] PA_09_I2C_SDA PIO control not enabled
    08 [0] PA_08_SD_CS_MCDA3 PIO control not enabled

    07 [0] PA_07_HSMCI_MCDA2 PIO control not enabled
    06 [0] PA_06_HSMCI_MCDA1 PIO control not enabled
    05 [0] PA_05_HSMCI_MCDA0 PIO control not enabled
    04 [0] PA_04_HSMCI_MCCDA PIO control not enabled

    03 [0] PA_03_HSMCI_MCCK PIO control not enabled
    02 [1] PA_02_SD_DETECT PIO control enabled
    01 [1] PA_01_SD_WP PIO control enabled
    00 [1] PA_00_TP54 PIO control enabled
*/

#define PIOB_PER_INIT (u32)0x01BFFF57
/*
    31 [0] PB_31_ PIO control not enabled
    30 [0] PB_30_ PIO control not enabled
    29 [0] PB_29_ PIO control not enabled
    28 [0] PB_28_ PIO control not enabled

    27 [0] PB_27_ PIO control not enabled
    26 [0] PB_26_ PIO control not enabled
    25 [0] PB_25_ PIO control not enabled
    24 [1] PB_24_ANT_SRDY PIO control enabled

    23 [1] PB_23_ANT_MRDY PIO control enabled
    22 [0] PB_22_ANT_USPI2_CS PIO control not enabled
    21 [1] PB_21_ANT_RESET PIO control enabled
    20 [1] PB_20_LED_RED PIO control enabled

    19 [1] PB_19_LED_GRN PIO control enabled
    18 [1] PB_18_LED_BLU PIO control enabled
    17 [1] PB_17_LED_YLW PIO control enabled
    16 [1] PB_16_LED_CYN PIO control enabled

    15 [1] PB_15_LED_ORG PIO control enabled
    14 [1] PB_14_LED_PRP PIO control enabled
    13 [1] PB_13_LED_WHT PIO control enabled
    12 [1] PB_12_LCD_BL_BLU PIO control enabled

    11 [1] PB_11_LCD_BL_GRN PIO control enabled
    10 [1] PB_10_LCD_BL_RED PIO control enabled
    09 [1] PB_09_LCD_RST PIO control enabled
    08 [1] PB_08_TP62 PIO control enabled

    07 [1] PB_07_TP60 PIO control enabled
    06 [0] PB_06_TP58 PIO control not enabled
    05 [1] PB_05_TP56 PIO control enabled
    04 [0] PB_04_BLADE_AN1 PIO control not enabled

    03 [0] PB_03_BLADE_AN0 PIO control not enabled
    02 [1] PB_02_BUTTON3 PIO control enabled
    01 [1] PB_01_BUTTON2 PIO control enabled
    00 [1] PB_00_BUTTON1 PIO control enabled
*/


/* PIO Disable Register: 
0: No effect
1: Pin is controlled by corresponding peripheral
*/
#define PIOA_PDR_INIT (u32)0x7BFCE7F8
/* 
    31 [0] PA_31_HEARTBEAT not controlled by peripheral
    30 [1] PA_30_AN_DEMO controlled by peripheral
    29 [1] PA_29_BUZZER2 controlled by peripheral
    28 [1] PA_28_BUZZER1 controlled by peripheral

    27 [1] PA_27_CLOCK_OUT controlled by peripheral
    26 [0] PA_26_ANT_PWR_EN not controlled by peripheral
    25 [1] PA_25_ANT_USPI2_SCK controlled by peripheral
    24 [1] PA_24_SD_USPI1_SCK controlled by peripheral

    23 [1] PA_23_ANT_USPI2_MOSI controlled by peripheral
    22 [1] PA_22_ANT_USPI2_MISO controlled by peripheral
    21 [1] PA_21_SD_USPI1_MISO controlled by peripheral
    20 [1] PA_20_SD_USPI1_MOSI controlled by peripheral

    19 [1] PA_19_DEBUG_U0_PIMO controlled by peripheral
    18 [1] PA_18_DEBUG_U0_POMI controlled by peripheral
    17 [0] PA_17_BUTTON0 not controlled by peripheral
    16 [0] PA_16_BLADE_CS not controlled by peripheral

    15 [1] PA_15_BLADE_SCK controlled by peripheral
    14 [1] PA_14_BLADE_MOSI controlled by peripheral
    13 [1] PA_13_BLADE_MISO controlled by peripheral

    12 [0] PA_12_BLADE_UPOMI not controlled by peripheral


    11 [0] PA_11_BLADE_UPIMO not controlled by peripheral

    10 [1] PA_10_I2C_SCL controlled by peripheral
    09 [1] PA_09_I2C_SDA controlled by peripheral
    08 [1] PA_08_SD_CS_MCDA3 controlled by peripheral

    07 [1] PA_07_HSMCI_MCDA2 controlled by peripheral
    06 [1] PA_06_HSMCI_MCDA1 controlled by peripheral
    05 [1] PA_05_HSMCI_MCDA0 controlled by peripheral
    04 [1] PA_04_HSMCI_MCCDA controlled by peripheral

    03 [1] PA_03_HSMCI_MCCK controlled by peripheral
    02 [0] PA_02_SD_DETECT not controlled by peripheral
    01 [0] PA_01_SD_WP not controlled by peripheral
    00 [0] PA_00_TP54 not controlled by peripheral
*/

#define PIOB_PDR_INIT (u32)0x00400058
/*
    31 [0] PB_31_ 
    30 [0] PB_30_ 
    29 [0] PB_29_ 
    28 [0] PB_28_ 

    27 [0] PB_27_ 
    26 [0] PB_26_ 
    25 [0] PB_25_ 
    24 [0] PB_24_ANT_SRDY not controlled by peripheral

    23 [0] PB_23_ANT_MRDY not controlled by peripheral
    22 [1] PB_22_ANT_USPI2_CS controlled by peripheral
    21 [0] PB_21_ANT_RESET not controlled by peripheral
    20 [0] PB_20_LED_RED not controlled by peripheral

    19 [0] PB_19_LED_GRN not controlled by peripheral
    18 [0] PB_18_LED_BLU not controlled by peripheral
    17 [0] PB_17_LED_YLW not controlled by peripheral
    16 [0] PB_16_LED_CYN not controlled by peripheral

    15 [0] PB_15_LED_ORG not controlled by peripheral
    14 [0] PB_14_LED_PRP not controlled by peripheral
    13 [0] PB_13_LED_WHT not controlled by peripheral
    12 [0] PB_12_LCD_BL_BLU not controlled by peripheral

    11 [0] PB_11_LCD_BL_GRN not controlled by peripheral
    10 [0] PB_10_LCD_BL_RED not controlled by peripheral
    09 [0] PB_09_LCD_RST not controlled by peripheral
    08 [0] PB_08_TP62 not controlled by peripheral

    07 [0] PB_07_TP60 not controlled by peripheral
    06 [1] PB_06_TP58 controlled by peripheral
    05 [0] PB_05_TP56 not controlled by peripheral
    04 [1] PB_04_BLADE_AN1 controlled by peripheral

    03 [1] PB_03_BLADE_AN0 controlled by peripheral
    02 [0] PB_02_BUTTON3 not controlled by peripheral
    01 [0] PB_01_BUTTON2 not controlled by peripheral
    00 [0] PB_00_BUTTON1 not controlled by peripheral
*/

/* PIO Controller Output Enable Register
Configures the pin as an output or input.
0: No effect
1: Enables the output on the I/O line
*/
#define PIOA_OER_INIT (u32)0xBF55DFF9
/* 
    31 [1] PA_31_HEARTBEAT output enabled
    30 [0] PA_30_AN_DEMO input
    29 [1] PA_29_BUZZER2 output enabled
    28 [1] PA_28_BUZZER1 output enabled

    27 [1] PA_27_CLOCK_OUT output enabled
    26 [1] PA_26_ANT_PWR_EN output enabled
    25 [1] PA_25_ANT_USPI2_SCK output enabled
    24 [1] PA_24_SD_USPI1_SCK output enabled

    23 [0] PA_23_ANT_USPI2_MOSI input
    22 [1] PA_22_ANT_USPI2_MISO output enabled
    21 [0] PA_21_SD_USPI1_MISO input input
    20 [1] PA_20_SD_USPI1_MOSI output enabled

    19 [0] PA_19_DEBUG_U0_PIMO input
    18 [1] PA_18_DEBUG_U0_POMI output enabled
    17 [0] PA_17_BUTTON0 input
    16 [1] PA_16_BLADE_CS output enabled

    15 [1] PA_15_BLADE_SCK output enabled
    14 [1] PA_14_BLADE_MOSI output enabled
    13 [0] PA_13_BLADE_MISO input

    12 [1] PA_12_BLADE_UPOMI output enabled

    11 [1] PA_11_BLADE_UPIMO output enabled

    10 [1] PA_10_I2C_SCL output enabled
    09 [1] PA_09_I2C_SDA output enabled
    08 [1] PA_08_SD_CS_MCDA3 output enabled

    07 [1] PA_07_HSMCI_MCDA2 output enabled
    06 [1] PA_06_HSMCI_MCDA1 output enabled
    05 [1] PA_05_HSMCI_MCDA0 output enabled
    04 [1] PA_04_HSMCI_MCCDA output enabled

    03 [1] PA_03_HSMCI_MCCK output enabled
    02 [0] PA_02_SD_DETECT input
    01 [0] PA_01_SD_WP input
    00 [1] PA_00_TP54 output enabled
*/

#define PIOB_OER_INIT (u32)0x01BFFFE0
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [1] PB_24_ANT_SRDY output enabled

    23 [1] PB_23_ANT_MRDY output enabled
    22 [0] PB_22_ANT_USPI2_CS
    21 [1] PB_21_ANT_RESET output enabled
    20 [1] PB_20_LED_RED output enabled

    19 [1] PB_19_LED_GRN output enabled
    18 [1] PB_18_LED_BLU output enabled
    17 [1] PB_17_LED_YLW output enabled
    16 [1] PB_16_LED_CYN output enabled

    15 [1] PB_15_LED_ORG output enabled
    14 [1] PB_14_LED_PRP output enabled
    13 [1] PB_13_LED_WHT output enabled
    12 [1] PB_12_LCD_BL_BLU output enabled

    11 [1] PB_11_LCD_BL_GRN output enabled
    10 [1] PB_10_LCD_BL_RED output enabled
    09 [1] PB_09_LCD_RST output enabled
    08 [1] PB_08_TP62 output enabled

    07 [1] PB_07_TP60 output enabled
    06 [1] PB_06_TP58 output enabled
    05 [1] PB_05_TP56 output enabled
    04 [0] PB_04_BLADE_AN1 input

    03 [0] PB_03_BLADE_AN0 input
    02 [0] PB_02_BUTTON3 input
    01 [0] PB_01_BUTTON2 input
    00 [0] PB_00_BUTTON1 input
*/

/* PIO Controller Output Disable Register
0: No effect
1: Disables the output on the I/O line.
*/
#define PIOA_ODR_INIT (u32)0x40AA2006
/* 
    31 [0] PA_31_HEARTBEAT output 
    30 [1] PA_30_AN_DEMO input
    29 [0] PA_29_BUZZER2 output 
    28 [0] PA_28_BUZZER1 output 

    27 [0] PA_27_CLOCK_OUT output 
    26 [0] PA_26_ANT_PWR_EN output 
    25 [0] PA_25_ANT_USPI2_SCK output 
    24 [0] PA_24_SD_USPI1_SCK output 

    23 [1] PA_23_ANT_USPI2_MOSI input
    22 [0] PA_22_ANT_USPI2_MISO output 
    21 [1] PA_21_SD_USPI1_MISO input 
    20 [0] PA_20_SD_USPI1_MOSI output 

    19 [1] PA_19_DEBUG_U0_PIMO input
    18 [0] PA_18_DEBUG_U0_POMI output 
    17 [1] PA_17_BUTTON0 input
    16 [0] PA_16_BLADE_CS output 

    15 [0] PA_15_BLADE_SCK output 
    14 [0] PA_14_BLADE_MOSI output
    13 [1] PA_13_BLADE_MISO input

    12 [0] PA_12_BLADE_UPOMI output 

    11 [0] PA_11_BLADE_UPIMO output

    10 [0] PA_10_I2C_SCL output 
    09 [0] PA_09_I2C_SDA input 
    08 [0] PA_08_SD_CS_MCDA3 output 

    07 [0] PA_07_HSMCI_MCDA2 output 
    06 [0] PA_06_HSMCI_MCDA1 output 
    05 [0] PA_05_HSMCI_MCDA0 output 
    04 [0] PA_04_HSMCI_MCCDA output 

    03 [0] PA_03_HSMCI_MCCK output 
    02 [1] PA_02_SD_DETECT input
    01 [1] PA_01_SD_WP input
    00 [0] PA_00_TP54 output 
*/

#define PIOB_ODR_INIT (u32)0x0040001F
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY output

    23 [0] PB_23_ANT_MRDY output
    22 [1] PB_22_ANT_USPI2_CS input
    21 [0] PB_21_ANT_RESET output 
    20 [0] PB_20_LED_RED output 

    19 [0] PB_19_LED_GRN output 
    18 [0] PB_18_LED_BLU output 
    17 [0] PB_17_LED_YLW output 
    16 [0] PB_16_LED_CYN output 

    15 [0] PB_15_LED_ORG output 
    14 [0] PB_14_LED_PRP output 
    13 [0] PB_13_LED_WHT output 
    12 [0] PB_12_LCD_BL_BLU output 

    11 [0] PB_11_LCD_BL_GRN output 
    10 [0] PB_10_LCD_BL_RED output 
    09 [0] PB_09_LCD_RST output 
    08 [0] PB_08_TP62 output 

    07 [0] PB_07_TP60 output 
    06 [0] PB_06_TP58 output 
    05 [0] PB_05_TP56 output 
    04 [1] PB_04_BLADE_AN1 input

    03 [1] PB_03_BLADE_AN0 input
    02 [1] PB_02_BUTTON3 input
    01 [1] PB_01_BUTTON2 input
    00 [1] PB_00_BUTTON1 input
*/

/* PIO Controller Input Filter Enable Register
0: No effect
1: Enables the input glitch filter on the I/O line.
*/
#define PIOA_IFER_INIT (u32)0x00000000
/* 
    31 [0] PA_31_HEARTBEAT no glitch filter
    30 [0] PA_30_AN_DEMO no glitch filter
    29 [0] PA_29_BUZZER2 no glitch filter
    28 [0] PA_28_BUZZER1 no glitch filter

    27 [0] PA_27_CLOCK_OUT no glitch filter
    26 [0] PA_26_ANT_PWR_EN no glitch filter
    25 [0] PA_25_ANT_USPI2_SCK no glitch filter
    24 [0] PA_24_SD_USPI1_SCK no glitch filter

    23 [0] PA_23_ANT_USPI2_MOSI no glitch filter
    22 [0] PA_22_ANT_USPI2_MISO no glitch filter
    21 [0] PA_21_SD_USPI1_MISO no glitch filter
    20 [0] PA_20_SD_USPI1_MOSI no glitch filter

    19 [0] PA_19_DEBUG_U0_PIMO no glitch filter
    18 [0] PA_18_DEBUG_U0_POMI no glitch filter
    17 [0] PA_17_BUTTON0 no glitch filter
    16 [0] PA_16_BLADE_CS no glitch filter

    15 [0] PA_15_BLADE_SCK no glitch filter
    14 [0] PA_14_BLADE_MOSI no glitch filter
    13 [0] PA_13_BLADE_MISO no glitch filter
      
    12 [0] PA_12_BLADE_UPOMI no glitch filter

    11 [0] PA_11_BLADE_UPIMO no glitch filter
      
    10 [0] PA_10_I2C_SCL no glitch filter
    09 [0] PA_09_I2C_SDA no glitch filter
    08 [0] PA_08_SD_CS_MCDA3 no glitch filter

    07 [0] PA_07_HSMCI_MCDA2 no glitch filter
    06 [0] PA_06_HSMCI_MCDA1 no glitch filter
    05 [0] PA_05_HSMCI_MCDA0 no glitch filter
    04 [0] PA_04_HSMCI_MCCDA no glitch filter

    03 [0] PA_03_HSMCI_MCCK no glitch filter
    02 [0] PA_02_SD_DETECT no glitch filter
    01 [0] PA_01_SD_WP no glitch filter
    00 [0] PA_00_TP54 no glitch filter
*/

#define PIOB_IFER_INIT (u32)0x00000000
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY no glitch filter

    23 [0] PB_23_ANT_MRDY no glitch filter
    22 [0] PB_22_ANT_USPI2_CS no glitch filter
    21 [0] PB_21_ANT_RESET no glitch filter
    20 [0] PB_20_LED_RED no glitch filter

    19 [0] PB_19_LED_GRN no glitch filter
    18 [0] PB_18_LED_BLU no glitch filter
    17 [0] PB_17_LED_YLW no glitch filter
    16 [0] PB_16_LED_CYN no glitch filter

    15 [0] PB_15_LED_ORG no glitch filter
    14 [0] PB_14_LED_PRP no glitch filter
    13 [0] PB_13_LED_WHT no glitch filter
    12 [0] PB_12_LCD_BL_BLU no glitch filter

    11 [0] PB_11_LCD_BL_GRN no glitch filter
    10 [0] PB_10_LCD_BL_RED no glitch filter
    09 [0] PB_09_LCD_RST no glitch filter
    08 [0] PB_08_TP62 no glitch filter

    07 [0] PB_07_TP60 no glitch filter
    06 [0] PB_06_TP58 no glitch filter
    05 [0] PB_05_TP56 no glitch filter
    04 [0] PB_04_BLADE_AN1 no glitch filter

    03 [0] PB_03_BLADE_AN0 no glitch filter
    02 [0] PB_02_BUTTON3 no glitch filter
    01 [0] PB_01_BUTTON2 no glitch filter
    00 [0] PB_00_BUTTON1 no glitch filter
*/

/* PIO Controller Input Filter Disable Register
0: No effect
1: Disables the input glitch filter on the I/O line.
*/
#define PIOA_IFDR_INIT (u32)0x00000000
/* 
    31 [0] PA_31_HEARTBEAT no input filter
    30 [0] PA_30_AN_DEMO no input filter
    29 [0] PA_29_BUZZER2 no input filter
    28 [0] PA_28_BUZZER1 no input filter

    27 [0] PA_27_CLOCK_OUT no input filter
    26 [0] PA_26_ANT_PWR_EN no input filter
    25 [0] PA_25_ANT_USPI2_SCK no input filter
    24 [0] PA_24_SD_USPI1_SCK no input filter

    23 [0] PA_23_ANT_USPI2_MOSI no input filter
    22 [0] PA_22_ANT_USPI2_MISO no input filter
    21 [0] PA_21_SD_USPI1_MISO no input filter
    20 [0] PA_20_SD_USPI1_MOSI no input filter

    19 [0] PA_19_DEBUG_U0_PIMO no input filter
    18 [0] PA_18_DEBUG_U0_POMI no input filter
    17 [0] PA_17_BUTTON0 no input filter
    16 [0] PA_16_BLADE_CS no input filter

    15 [0] PA_15_BLADE_SCK no input filter
    14 [0] PA_14_BLADE_MOSI no input filter
    13 [0] PA_13_BLADE_MISO no input filter
      
    12 [0] PA_12_BLADE_UPOMI no input filter

    11 [0] PA_11_BLADE_UPIMO no input filter
      
    10 [0] PA_10_I2C_SCL no input filter
    09 [0] PA_09_I2C_SDA no input filter
    08 [0] PA_08_SD_CS_MCDA3 no input filter

    07 [0] PA_07_HSMCI_MCDA2 no input filter
    06 [0] PA_06_HSMCI_MCDA1 no input filter
    05 [0] PA_05_HSMCI_MCDA0 no input filter
    04 [0] PA_04_HSMCI_MCCDA no input filter

    03 [0] PA_03_HSMCI_MCCK no input filter
    02 [0] PA_02_SD_DETECT no input filter
    01 [0] PA_01_SD_WP no input filter
    00 [0] PA_00_TP54 no input filter
*/

#define PIOB_IFDR_INIT (u32)0x00000000
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY no input filter

    23 [0] PB_23_ANT_MRDY no input filter
    22 [0] PB_22_ANT_USPI2_CS no input filter
    21 [0] PB_21_ANT_RESET no input filter
    20 [0] PB_20_LED_RED no input filter

    19 [0] PB_19_LED_GRN no input filter
    18 [0] PB_18_LED_BLU no input filter
    17 [0] PB_17_LED_YLW no input filter
    16 [0] PB_16_LED_CYN no input filter

    15 [0] PB_15_LED_ORG no input filter
    14 [0] PB_14_LED_PRP no input filter
    13 [0] PB_13_LED_WHT no input filter
    12 [0] PB_12_LCD_BL_BLU no input filter

    11 [0] PB_11_LCD_BL_GRN no input filter
    10 [0] PB_10_LCD_BL_RED no input filter
    09 [0] PB_09_LCD_RST no input filter
    08 [0] PB_08_TP62 no input filter

    07 [0] PB_07_TP60 no input filter
    06 [0] PB_06_TP58 no input filter
    05 [0] PB_05_TP56 no input filter
    04 [0] PB_04_BLADE_AN1 no input filter

    03 [0] PB_03_BLADE_AN0 no input filter
    02 [0] PB_02_BUTTON3 no input filter
    01 [0] PB_01_BUTTON2 no input filter
    00 [0] PB_00_BUTTON1 no input filter
*/

/* PIO Controller Set Output Data Register
Default start-up IO values are held here.
0: No effect
1: Sets the data to be driven on the I/O line.
*/
#define PIOA_SODR_INIT (u32)0x88011000
/* 
    31 [1] PA_31_HEARTBEAT output high 
    30 [0] PA_30_AN_DEMO N/A
    29 [0] PA_29_BUZZER2 output low
    28 [0] PA_28_BUZZER1 output low

    27 [1] PA_27_CLOCK_OUT output high
    26 [0] PA_26_ANT_PWR_EN output high Z
    25 [0] PA_25_ANT_USPI2_SCK N/A
    24 [0] PA_24_SD_USPI1_SCK N/A

    23 [0] PA_23_ANT_USPI2_MOSI N/A
    22 [0] PA_22_ANT_USPI2_MISO N/A
    21 [0] PA_21_SD_USPI1_MISO N/A
    20 [0] PA_20_SD_USPI1_MOSI N/A

    19 [0] PA_19_DEBUG_U0_PIMO N/A
    18 [0] PA_18_DEBUG_U0_POMI N/A
    17 [0] PA_17_BUTTON0 N/A
    16 [1] PA_16_BLADE_CS output high

    15 [0] PA_15_BLADE_SCK N/A
    14 [0] PA_14_BLADE_MOSI N/A
    13 [0] PA_13_BLADE_MISO N/A

    12 [1] PA_12_BLADE_UPOMI output high


    11 [1] PA_11_BLADE_UPIMO output high

    10 [0] PA_10_I2C_SCL N/A 
    09 [0] PA_09_I2C_SDA N/A
    08 [0] PA_08_SD_CS_MCDA3 N/A

    07 [0] PA_07_HSMCI_MCDA2 N/A
    06 [0] PA_06_HSMCI_MCDA1 N/A
    05 [0] PA_05_HSMCI_MCDA0 N/A
    04 [0] PA_04_HSMCI_MCCDA N/A

    03 [0] PA_03_HSMCI_MCCK N/A
    02 [0] PA_02_SD_DETECT N/A
    01 [0] PA_01_SD_WP N/A
    00 [0] PA_00_TP54 N/A
*/

#define PIOB_SODR_INIT (u32)0x01BFFE00
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [1] PB_24_ANT_SRDY output high

    23 [1] PB_23_ANT_MRDY output high
    22 [0] PB_22_ANT_USPI2_CS N/A
    21 [1] PB_21_ANT_RESET output high
    20 [1] PB_20_LED_RED output high

    19 [1] PB_19_LED_GRN output high
    18 [1] PB_18_LED_BLU output high
    17 [1] PB_17_LED_YLW output high
    16 [1] PB_16_LED_CYN output high

    15 [1] PB_15_LED_ORG output high
    14 [1] PB_14_LED_PRP output high
    13 [1] PB_13_LED_WHT output high
    12 [1] PB_12_LCD_BL_BLU output high

    11 [1] PB_11_LCD_BL_GRN output high
    10 [1] PB_10_LCD_BL_RED output high
    09 [1] PB_09_LCD_RST output high
    08 [0] PB_08_TP62 N/A

    07 [0] PB_07_TP60 N/A
    06 [0] PB_06_TP58 N/A
    05 [0] PB_05_TP56 N/A
    04 [0] PB_04_BLADE_AN1 N/A

    03 [0] PB_03_BLADE_AN0 N/A
    02 [0] PB_02_BUTTON3 N/A
    01 [0] PB_01_BUTTON2 N/A
    00 [0] PB_00_BUTTON1 N/A
*/

/* PIO Controller Clear Output Data Register
Initial output values are stored here.
0: No effect
1: Clears the data to be driven on the I/O line.
*/
#define PIOA_CODR_INIT (u32)0x30000000
/* 
    31 [0] PA_31_HEARTBEAT output high 
    30 [0] PA_30_AN_DEMO N/A
    29 [1] PA_29_BUZZER2 output low
    28 [1] PA_28_BUZZER1 output low

    27 [0] PA_27_CLOCK_OUT output high
    26 [0] PA_26_ANT_PWR_EN output high Z
    25 [0] PA_25_ANT_USPI2_SCK N/A
    24 [0] PA_24_SD_USPI1_SCK N/A

    23 [0] PA_23_ANT_USPI2_MOSI N/A
    22 [0] PA_22_ANT_USPI2_MISO N/A
    21 [0] PA_21_SD_USPI1_MISO N/A
    20 [0] PA_20_SD_USPI1_MOSI N/A

    19 [0] PA_19_DEBUG_U0_PIMO N/A
    18 [0] PA_18_DEBUG_U0_POMI N/A
    17 [0] PA_17_BUTTON0 N/A
    16 [0] PA_16_BLADE_CS output high

    15 [0] PA_15_BLADE_SCK N/A
    14 [0] PA_14_BLADE_MOSI N/A
    13 [0] PA_13_BLADE_MISO N/A

    12 [0] PA_12_BLADE_UPOMI N/A

    11 [0] PA_11_BLADE_UPIMO N/A

    10 [0] PA_10_I2C_SCL N/A  CHANGED(again)
    09 [0] PA_09_I2C_SDA N/A
    08 [0] PA_08_SD_CS_MCDA3 N/A

    07 [0] PA_07_HSMCI_MCDA2 N/A
    06 [0] PA_06_HSMCI_MCDA1 N/A
    05 [0] PA_05_HSMCI_MCDA0 N/A
    04 [0] PA_04_HSMCI_MCCDA N/A

    03 [0] PA_03_HSMCI_MCCK N/A
    02 [0] PA_02_SD_DETECT N/A
    01 [0] PA_01_SD_WP N/A
    00 [0] PA_00_TP54 N/A
*/

#define PIOB_CODR_INIT (u32)0x00000000
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY output high

    23 [0] PB_23_ANT_MRDY output high
    22 [0] PB_22_ANT_USPI2_CS N/A
    21 [0] PB_21_ANT_RESET output high
    20 [0] PB_20_LED_RED output high

    19 [0] PB_19_LED_GRN output high
    18 [0] PB_18_LED_BLU output high
    17 [0] PB_17_LED_YLW output high
    16 [0] PB_16_LED_CYN output high

    15 [0] PB_15_LED_ORG output high
    14 [0] PB_14_LED_PRP output high
    13 [0] PB_13_LED_WHT output high
    12 [0] PB_12_LCD_BL_BLU output high

    11 [0] PB_11_LCD_BL_GRN output high
    10 [0] PB_10_LCD_BL_RED output high
    09 [0] PB_09_LCD_RST output high
    08 [0] PB_08_TP62 N/A

    07 [0] PB_07_TP60 N/A
    06 [0] PB_06_TP58 N/A
    05 [0] PB_05_TP56 N/A
    04 [0] PB_04_BLADE_AN1 N/A

    03 [0] PB_03_BLADE_AN0 N/A
    02 [0] PB_02_BUTTON3 N/A
    01 [0] PB_01_BUTTON2 N/A
    00 [0] PB_00_BUTTON1 N/A
*/

/* PIO Multi-driver (open drain) Enable Register
0: No effect
1: Enables Multi Drive on the I/O line.
*/
#define PIOA_MDER_INIT (u32)0x04000600
/* 
    31 [0] PA_31_HEARTBEAT
    30 [0] PA_30_AN_DEMO
    29 [0] PA_29_BUZZER2
    28 [0] PA_28_BUZZER1

    27 [0] PA_27_CLOCK_OUT
    26 [1] PA_26_ANT_PWR_EN open drain
    25 [0] PA_25_ANT_USPI2_SCK
    24 [0] PA_24_SD_USPI1_SCK

    23 [0] PA_23_ANT_USPI2_MOSI
    22 [0] PA_22_ANT_USPI2_MISO
    21 [0] PA_21_SD_USPI1_MISO
    20 [0] PA_20_SD_USPI1_MOSI

    19 [0] PA_19_DEBUG_U0_PIMO
    18 [0] PA_18_DEBUG_U0_POMI
    17 [0] PA_17_BUTTON0
    16 [0] PA_16_BLADE_CS

    15 [0] PA_15_BLADE_SCK
    14 [0] PA_14_BLADE_MOSI 
    13 [0] PA_13_BLADE_MISO
      
    12 [0] PA_12_BLADE_UPOMI

    11 [0] PA_11_BLADE_UPIMO
      
    10 [1] PA_10_I2C_SCL open drain
    09 [1] PA_09_I2C_SDA open drain
    08 [0] PA_08_SD_CS_MCDA3

    07 [0] PA_07_HSMCI_MCDA2
    06 [0] PA_06_HSMCI_MCDA1
    05 [0] PA_05_HSMCI_MCDA0
    04 [0] PA_04_HSMCI_MCCDA

    03 [0] PA_03_HSMCI_MCCK
    02 [0] PA_02_SD_DETECT
    01 [0] PA_01_SD_WP
    00 [0] PA_00_TP54
*/

#define PIOB_MDER_INIT (u32)0x00000000
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY

    23 [0] PB_23_ANT_MRDY
    22 [0] PB_22_ANT_USPI2_CS
    21 [0] PB_21_ANT_RESET
    20 [0] PB_20_LED_RED

    19 [0] PB_19_LED_GRN
    18 [0] PB_18_LED_BLU
    17 [0] PB_17_LED_YLW
    16 [0] PB_16_LED_CYN

    15 [0] PB_15_LED_ORG
    14 [0] PB_14_LED_PRP
    13 [0] PB_13_LED_WHT
    12 [0] PB_12_LCD_BL_BLU

    11 [0] PB_11_LCD_BL_GRN
    10 [0] PB_10_LCD_BL_RED
    09 [0] PB_09_LCD_RST
    08 [0] PB_08_TP62

    07 [0] PB_07_TP60
    06 [0] PB_06_TP58
    05 [0] PB_05_TP56
    04 [0] PB_04_BLADE_AN1

    03 [0] PB_03_BLADE_AN0
    02 [0] PB_02_BUTTON3
    01 [0] PB_01_BUTTON2
    00 [0] PB_00_BUTTON1
*/

/* PIO Multi-driver Disable Register
0: No effect
1: Disables Multi Drive on the I/O line.
*/
#define PIOA_MDDR_INIT (u32)0xFBFFE1FF
/* 
    31 [1] PA_31_HEARTBEAT not open drain
    30 [1] PA_30_AN_DEMO not open drain
    29 [1] PA_29_BUZZER2 not open drain
    28 [1] PA_28_BUZZER1 not open drain

    27 [1] PA_27_CLOCK_OUT not open drain
    26 [0] PA_26_ANT_PWR_EN
    25 [1] PA_25_ANT_USPI2_SCK not open drain
    24 [1] PA_24_SD_USPI1_SCK not open drain

    23 [1] PA_23_ANT_USPI2_MOSI not open drain
    22 [1] PA_22_ANT_USPI2_MISO not open drain
    21 [1] PA_21_SD_USPI1_MISO not open drain
    20 [1] PA_20_SD_USPI1_MOSI not open drain

    19 [1] PA_19_DEBUG_U0_PIMO not open drain
    18 [1] PA_18_DEBUG_U0_POMI not open drain
    17 [1] PA_17_BUTTON0 not open drain
    16 [1] PA_16_BLADE_CS not open drain

    15 [1] PA_15_BLADE_SCK not open drain
    14 [1] PA_14_BLADE_MOSI not open drain
    13 [1] PA_13_BLADE_MISO not open drain

    12 [0] PA_12_BLADE_UPOMI 

    11 [0] PA_11_BLADE_UPIMO not open drain

    10 [0] PA_10_I2C_SCL
    09 [0] PA_09_I2C_SDA
    08 [1] PA_08_SD_CS_MCDA3 not open drain

    07 [1] PA_07_HSMCI_MCDA2 not open drain
    06 [1] PA_06_HSMCI_MCDA1 not open drain
    05 [1] PA_05_HSMCI_MCDA0 not open drain
    04 [1] PA_04_HSMCI_MCCDA not open drain

    03 [1] PA_03_HSMCI_MCCK not open drain
    02 [1] PA_02_SD_DETECT not open drain
    01 [1] PA_01_SD_WP not open drain
    00 [1] PA_00_TP54 not open drain
*/

#define PIOB_MDDR_INIT (u32)0x01FFFFFF
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [1] PB_24_ANT_SRDY not open drain

    23 [1] PB_23_ANT_MRDY not open drain
    22 [1] PB_22_ANT_USPI2_CS not open drain
    21 [1] PB_21_ANT_RESET not open drain
    20 [1] PB_20_LED_RED not open drain

    19 [1] PB_19_LED_GRN not open drain
    18 [1] PB_18_LED_BLU not open drain
    17 [1] PB_17_LED_YLW not open drain
    16 [1] PB_16_LED_CYN not open drain

    15 [1] PB_15_LED_ORG not open drain
    14 [1] PB_14_LED_PRP not open drain
    13 [1] PB_13_LED_WHT not open drain
    12 [1] PB_12_LCD_BL_BLU not open drain

    11 [1] PB_11_LCD_BL_GRN not open drain
    10 [1] PB_10_LCD_BL_RED not open drain
    09 [1] PB_09_LCD_RST not open drain
    08 [1] PB_08_TP62 not open drain

    07 [1] PB_07_TP60 not open drain
    06 [1] PB_06_TP58 not open drain
    05 [1] PB_05_TP56 not open drain
    04 [1] PB_04_BLADE_AN1 not open drain

    03 [1] PB_03_BLADE_AN0 not open drain
    02 [1] PB_02_BUTTON3 not open drain
    01 [1] PB_01_BUTTON2 not open drain
    00 [1] PB_00_BUTTON1 not open drain
*/

/* PIO Pull Up Disable Register
0: No effect
1: Disables the pull up resistor on the I/O line.
*/
#define PIOA_PPUDR_INIT (u32)0xFFFFFFFE
/* 
    31 [1] PA_31_HEARTBEAT no pull-up
    30 [1] PA_30_AN_DEMO no pull-up
    29 [1] PA_29_BUZZER2 no pull-up
    28 [1] PA_28_BUZZER1 no pull-up

    27 [1] PA_27_CLOCK_OUT no pull-up
    26 [1] PA_26_ANT_PWR_EN no pull-up
    25 [1] PA_25_ANT_USPI2_SCK no pull-up
    24 [1] PA_24_SD_USPI1_SCK no pull-up

    23 [1] PA_23_ANT_USPI2_MOSI no pull-up
    22 [1] PA_22_ANT_USPI2_MISO no pull-up
    21 [1] PA_21_SD_USPI1_MISO no pull-up
    20 [1] PA_20_SD_USPI1_MOSI no pull-up

    19 [1] PA_19_DEBUG_U0_PIMO no pull-up
    18 [1] PA_18_DEBUG_U0_POMI no pull-up
    17 [1] PA_17_BUTTON0 no pull-up
    16 [1] PA_16_BLADE_CS no pull-up

    15 [1] PA_15_BLADE_SCK no pull-up
    14 [1] PA_14_BLADE_MOSI no pull-up
    13 [1] PA_13_BLADE_MISO no pull-up

    12 [1] PA_12_BLADE_UPOMI no pull-up

    11 [1] PA_11_BLADE_UPIMO no pull-up

    10 [1] PA_10_I2C_SCL no pull-up
    09 [1] PA_09_I2C_SDA no pull-up
    08 [1] PA_08_SD_CS_MCDA3 no pull-up

    07 [1] PA_07_HSMCI_MCDA2 no pull-up
    06 [1] PA_06_HSMCI_MCDA1 no pull-up
    05 [1] PA_05_HSMCI_MCDA0 no pull-up
    04 [1] PA_04_HSMCI_MCCDA no pull-up

    03 [1] PA_03_HSMCI_MCCK no pull-up
    02 [1] PA_02_SD_DETECT no pull-up
    01 [1] PA_01_SD_WP no pull-up
    00 [0] PA_00_TP54 pull-up enabled
*/

#define PIOB_PPUDR_INIT (u32)0x01FFFE5F
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [1] PB_24_ANT_SRDY no pull-up

    23 [1] PB_23_ANT_MRDY no pull-up
    22 [1] PB_22_ANT_USPI2_CS no pull-up
    21 [1] PB_21_ANT_RESET no pull-up
    20 [1] PB_20_LED_RED no pull-up

    19 [1] PB_19_LED_GRN no pull-up
    18 [1] PB_18_LED_BLU no pull-up
    17 [1] PB_17_LED_YLW no pull-up
    16 [1] PB_16_LED_CYN no pull-up

    15 [1] PB_15_LED_ORG no pull-up
    14 [1] PB_14_LED_PRP no pull-up
    13 [1] PB_13_LED_WHT no pull-up
    12 [1] PB_12_LCD_BL_BLU no pull-up

    11 [1] PB_11_LCD_BL_GRN no pull-up
    10 [1] PB_10_LCD_BL_RED no pull-up
    09 [1] PB_09_LCD_RST no pull-up
    08 [0] PB_08_TP62 pull-up enabled

    07 [0] PB_07_TP60 pull-up enabled
    06 [1] PB_06_TP58 no pull-up enabled
    05 [0] PB_05_TP56 pull-up enabled
    04 [1] PB_04_BLADE_AN1 no pull-up

    03 [1] PB_03_BLADE_AN0 no pull-up
    02 [1] PB_02_BUTTON3 no pull-up
    01 [1] PB_01_BUTTON2 no pull-up
    00 [1] PB_00_BUTTON1 no pull-up
*/

/* PIO Pull Up Enable Register
0: No effect
1: Enables the pull-up resistor on the selected pin
*/
#define PIOA_PPUER_INIT (u32)0x00000001
/*
    31 [0] PA_31_HEARTBEAT no pull-up
    30 [0] PA_30_AN_DEMO no pull-up
    29 [0] PA_29_BUZZER2 no pull-up
    28 [0] PA_28_BUZZER1 no pull-up

    27 [0] PA_27_CLOCK_OUT no pull-up
    26 [0] PA_26_ANT_PWR_EN no pull-up
    25 [0] PA_25_ANT_USPI2_SCK no pull-up
    24 [0] PA_24_SD_USPI1_SCK no pull-up

    23 [0] PA_23_ANT_USPI2_MOSI no pull-up
    22 [0] PA_22_ANT_USPI2_MISO no pull-up
    21 [0] PA_21_SD_USPI1_MISO no pull-up
    20 [0] PA_20_SD_USPI1_MOSI no pull-up

    19 [0] PA_19_DEBUG_U0_PIMO no pull-up
    18 [0] PA_18_DEBUG_U0_POMI no pull-up
    17 [0] PA_17_BUTTON0 no pull-up
    16 [0] PA_16_BLADE_CS no pull-up

    15 [0] PA_15_BLADE_SCK no pull-up
    14 [0] PA_14_BLADE_MOSI no pull-up
    13 [0] PA_13_BLADE_MISO no pull-up

    12 [0] PA_12_BLADE_UPOMI no pull-up

    11 [0] PA_11_BLADE_UPIMO no pull-up

    10 [0] PA_10_I2C_SCL no pull-up
    09 [0] PA_09_I2C_SDA no pull-up
    08 [0] PA_08_SD_CS_MCDA3 no pull-up

    07 [0] PA_07_HSMCI_MCDA2 no pull-up
    06 [0] PA_06_HSMCI_MCDA1 no pull-up
    05 [0] PA_05_HSMCI_MCDA0 no pull-up
    04 [0] PA_04_HSMCI_MCCDA no pull-up

    03 [0] PA_03_HSMCI_MCCK no pull-up
    02 [0] PA_02_SD_DETECT no pull-up
    01 [0] PA_01_SD_WP no pull-up
    00 [1] PA_00_TP54 pull-up enabled
*/

#define PIOB_PPUER_INIT (u32)0x000001C0
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY no pull-up

    23 [0] PB_23_ANT_MRDY no pull-up
    22 [0] PB_22_ANT_USPI2_CS no pull-up
    21 [0] PB_21_ANT_RESET no pull-up
    20 [0] PB_20_LED_RED no pull-up

    19 [0] PB_19_LED_GRN no pull-up
    18 [0] PB_18_LED_BLU no pull-up
    17 [0] PB_17_LED_YLW no pull-up
    16 [0] PB_16_LED_CYN no pull-up

    15 [0] PB_15_LED_ORG no pull-up
    14 [0] PB_14_LED_PRP no pull-up
    13 [0] PB_13_LED_WHT no pull-up
    12 [0] PB_12_LCD_BL_BLU no pull-up

    11 [0] PB_11_LCD_BL_GRN no pull-up
    10 [0] PB_10_LCD_BL_RED no pull-up
    09 [0] PB_09_LCD_RST no pull-up
    08 [1] PB_08_TP62 pull-up enabled

    07 [1] PB_07_TP60 pull-up enabled
    06 [0] PB_06_TP58 no pull-up enabled
    05 [1] PB_05_TP56 pull-up enabled
    04 [0] PB_04_BLADE_AN1 no pull-up

    03 [0] PB_03_BLADE_AN0 no pull-up
    02 [0] PB_02_BUTTON3 no pull-up
    01 [0] PB_01_BUTTON2 no pull-up
    00 [0] PB_00_BUTTON1 no pull-up
*/


/* PIO Peripheral AB Select Register
0: Assigns the I/O line to the Peripheral A function.
1: Assigns the I/O line to the Peripheral B function.
*/
#define PIOA_ABSR_INIT (u32)0x7B000000
/* 
    31 [0] PA_31_HEARTBEAT N/A
    30 [1] PA_30_AN_DEMO PERIPHERAL B
    29 [1] PA_29_BUZZER2 PERIPHERAL B
    28 [1] PA_28_BUZZER1 PERIPHERAL B

    27 [1] PA_27_CLOCK_OUT PERIPHERAL B
    26 [0] PA_26_ANT_PWR_EN N/A
    25 [1] PA_25_ANT_USPI2_SCK PERIPHERAL B
    24 [1] PA_24_SD_USPI1_SCK PERIPHERAL B

    23 [0] PA_23_ANT_USPI2_MOSI PERIPHERAL A
    22 [0] PA_22_ANT_USPI2_MISO PERIPHERAL A
    21 [0] PA_21_SD_USPI1_MISO PERIPHERAL A
    20 [0] PA_20_SD_USPI1_MOSI PERIPHERAL A

    19 [0] PA_19_DEBUG_U0_PIMO PERIPHERAL A
    18 [0] PA_18_DEBUG_U0_POMI PERIPHERAL A
    17 [0] PA_17_BUTTON0 N/A
    16 [0] PA_16_BLADE_CS PERIPHERAL A

    15 [0] PA_15_BLADE_SCK PERIPHERAL A
    14 [0] PA_14_BLADE_MOSI PERIPHERAL A
    13 [0] PA_13_BLADE_MISO PERIPHERAL A

    12 [0] PA_12_BLADE_UPOMI PERIPHERAL A

    11 [0] PA_11_BLADE_UPIMO PERIPHERAL A

    10 [0] PA_10_I2C_SCL PERIPHERAL A
    09 [0] PA_09_I2C_SDA PERIPHERAL A
    08 [0] PA_08_SD_CS_MCDA3 PERIPHERAL A

    07 [0] PA_07_HSMCI_MCDA2 PERIPHERAL A
    06 [0] PA_06_HSMCI_MCDA1 PERIPHERAL A
    05 [0] PA_05_HSMCI_MCDA0 PERIPHERAL A
    04 [0] PA_04_HSMCI_MCCDA PERIPHERAL A

    03 [0] PA_03_HSMCI_MCCK PERIPHERAL A
    02 [0] PA_02_SD_DETECT N/A
    01 [0] PA_01_SD_WP N/A
    00 [0] PA_00_TP54 N/A
*/

#define PIOB_ABSR_INIT (u32)0x00400018
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY N/A

    23 [0] PB_23_ANT_MRDY N/A
    22 [1] PB_22_ANT_USPI2_CS PERIPHERAL B
    21 [0] PB_21_ANT_RESET N/A
    20 [0] PB_20_LED_RED N/A

    19 [0] PB_19_LED_GRN N/A
    18 [0] PB_18_LED_BLU N/A
    17 [0] PB_17_LED_YLW N/A
    16 [0] PB_16_LED_CYN N/A

    15 [0] PB_15_LED_ORG N/A
    14 [0] PB_14_LED_PRP N/A
    13 [0] PB_13_LED_WHT N/A
    12 [0] PB_12_LCD_BL_BLU N/A

    11 [0] PB_11_LCD_BL_GRN N/A
    10 [0] PB_10_LCD_BL_RED N/A
    09 [0] PB_09_LCD_RST N/A
    08 [0] PB_08_TP62 N/A

    07 [0] PB_07_TP60 N/A
    06 [0] PB_06_TP58 N/A
    05 [0] PB_05_TP56 N/A
    04 [1] PB_04_BLADE_AN1 PERIPHERAL B

    03 [1] PB_03_BLADE_AN0 PERIPHERAL B
    02 [0] PB_02_BUTTON3 N/A
    01 [0] PB_01_BUTTON2 N/A
    00 [0] PB_00_BUTTON1 N/A
*/

/* PIO System Clock Glitch Input Filtering Select Register
0: No effect
1: The Glitch Filter is able to filter glitches with a duration < Tmck/2.
*/
#define PIOA_SCIFSR_INIT (u32)0x00000000
/* 
    31 [0] PA_31_HEARTBEAT
    30 [0] PA_30_AN_DEMO
    29 [0] PA_29_BUZZER2
    28 [0] PA_28_BUZZER1

    27 [0] PA_27_CLOCK_OUT
    26 [0] PA_26_ANT_PWR_EN
    25 [0] PA_25_ANT_USPI2_SCK
    24 [0] PA_24_SD_USPI1_SCK

    23 [0] PA_23_ANT_USPI2_MOSI
    22 [0] PA_22_ANT_USPI2_MISO
    21 [0] PA_21_SD_USPI1_MISO
    20 [0] PA_20_SD_USPI1_MOSI

    19 [0] PA_19_DEBUG_U0_PIMO
    18 [0] PA_18_DEBUG_U0_POMI
    17 [0] PA_17_BUTTON0
    16 [0] PA_16_BLADE_CS

    15 [0] PA_15_BLADE_SCK
    14 [0] PA_14_BLADE_MOSI
    13 [0] PA_13_BLADE_MISO
    12 [0] PA_12_BLADE_UPOMI

    11 [0] PA_11_BLADE_UPIMO
    10 [0] PA_10_I2C_SCL
    09 [0] PA_09_I2C_SDA
    08 [0] PA_08_SD_CS_MCDA3

    07 [0] PA_07_HSMCI_MCDA2
    06 [0] PA_06_HSMCI_MCDA1
    05 [0] PA_05_HSMCI_MCDA0
    04 [0] PA_04_HSMCI_MCCDA

    03 [0] PA_03_HSMCI_MCCK
    02 [0] PA_02_SD_DETECT
    01 [0] PA_01_SD_WP
    00 [0] PA_00_TP54
*/

#define PIOB_SCIFSR_INIT (u32)0x00000000
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY

    23 [0] PB_23_ANT_MRDY
    22 [0] PB_22_ANT_USPI2_CS
    21 [0] PB_21_ANT_RESET
    20 [0] PB_20_LED_RED

    19 [0] PB_19_LED_GRN
    18 [0] PB_18_LED_BLU
    17 [0] PB_17_LED_YLW
    16 [0] PB_16_LED_CYN

    15 [0] PB_15_LED_ORG
    14 [0] PB_14_LED_PRP
    13 [0] PB_13_LED_WHT
    12 [0] PB_12_LCD_BL_BLU

    11 [0] PB_11_LCD_BL_GRN
    10 [0] PB_10_LCD_BL_RED
    09 [0] PB_09_LCD_RST
    08 [0] PB_08_TP62

    07 [0] PB_07_TP60
    06 [0] PB_06_TP58
    05 [0] PB_05_TP56
    04 [0] PB_04_BLADE_AN1

    03 [0] PB_03_BLADE_AN0
    02 [0] PB_02_BUTTON3
    01 [0] PB_01_BUTTON2
    00 [0] PB_00_BUTTON1
*/

/* PIO Debouncing Input Filtering Select Register
0: No effect
1: The Debouncing Filter is able to filter pulses with a duration < Tdiv_slclk/2.
*/
#define PIOA_DIFSR_INIT (u32)0x00000000
/* 
    31 [0] PA_31_HEARTBEAT
    30 [0] PA_30_AN_DEMO
    29 [0] PA_29_BUZZER2
    28 [0] PA_28_BUZZER1

    27 [0] PA_27_CLOCK_OUT
    26 [0] PA_26_ANT_PWR_EN
    25 [0] PA_25_ANT_USPI2_SCK
    24 [0] PA_24_SD_USPI1_SCK

    23 [0] PA_23_ANT_USPI2_MOSI
    22 [0] PA_22_ANT_USPI2_MISO
    21 [0] PA_21_SD_USPI1_MISO
    20 [0] PA_20_SD_USPI1_MOSI

    19 [0] PA_19_DEBUG_U0_PIMO
    18 [0] PA_18_DEBUG_U0_POMI
    17 [0] PA_17_BUTTON0
    16 [0] PA_16_BLADE_CS

    15 [0] PA_15_BLADE_SCK
    14 [0] PA_14_BLADE_MOSI
    13 [0] PA_13_BLADE_MISO

    12 [0] PA_12_BLADE_UPOMI

    11 [0] PA_11_BLADE_UPIMO

    10 [0] PA_10_I2C_SCL
    09 [0] PA_09_I2C_SDA
    08 [0] PA_08_SD_CS_MCDA3

    07 [0] PA_07_HSMCI_MCDA2
    06 [0] PA_06_HSMCI_MCDA1
    05 [0] PA_05_HSMCI_MCDA0
    04 [0] PA_04_HSMCI_MCCDA

    03 [0] PA_03_HSMCI_MCCK
    02 [0] PA_02_SD_DETECT
    01 [0] PA_01_SD_WP
    00 [0] PA_00_TP54
*/

#define PIOB_DIFSR_INIT (u32)0x00000000
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY

    23 [0] PB_23_ANT_MRDY
    22 [0] PB_22_ANT_USPI2_CS
    21 [0] PB_21_ANT_RESET
    20 [0] PB_20_LED_RED

    19 [0] PB_19_LED_GRN
    18 [0] PB_18_LED_BLU
    17 [0] PB_17_LED_YLW
    16 [0] PB_16_LED_CYN

    15 [0] PB_15_LED_ORG
    14 [0] PB_14_LED_PRP
    13 [0] PB_13_LED_WHT
    12 [0] PB_12_LCD_BL_BLU

    11 [0] PB_11_LCD_BL_GRN
    10 [0] PB_10_LCD_BL_RED
    09 [0] PB_09_LCD_RST
    08 [0] PB_08_TP62

    07 [0] PB_07_TP60
    06 [0] PB_06_TP58
    05 [0] PB_05_TP56
    04 [0] PB_04_BLADE_AN1

    03 [0] PB_03_BLADE_AN0
    02 [0] PB_02_BUTTON3
    01 [0] PB_01_BUTTON2
    00 [0] PB_00_BUTTON1
*/

/* PIO Slow Clock Divider Debouncing Register
Tdiv_slclk = 2*(DIV+1)*Tslow_clock. 
*/
#define PIOA_SCDR_INIT (u32)0x00000000
/*
    31 - 16 [0] Reserved

    15 [0] Reserved
    14 [0] "
    13 [0] DIV
    12 [0] "

    11 [0] "
    10 [0] "
    09 [0] "
    08 [0] "

    07 [0] "
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] "
    02 [0] "
    01 [0] "
    00 [0] "
*/

#define PIOB_SCDR_INIT (u32)0x00000000
/*
    31 - 16 [0] Reserved

    15 [0] Reserved
    14 [0] "
    13 [0] DIV
    12 [0] "

    11 [0] "
    10 [0] "
    09 [0] "
    08 [0] "

    07 [0] "
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] "
    02 [0] "
    01 [0] "
    00 [0] "
*/

/* PIO Output Write Enable Register
0: No effect
1: Enables writing PIO_ODSR for the I/O line.
*/
#define PIOA_OWER_INIT (u32)0xB4011800
/* 
    31 [1] PA_31_HEARTBEAT write enabled
    30 [0] PA_30_AN_DEMO
    29 [1] PA_29_BUZZER2 write enabled
    28 [1] PA_28_BUZZER1 write enabled

    27 [0] PA_27_CLOCK_OUT
    26 [1] PA_26_ANT_PWR_EN write enabled
    25 [0] PA_25_ANT_USPI2_SCK
    24 [0] PA_24_SD_USPI1_SCK

    23 [0] PA_23_ANT_USPI2_MOSI
    22 [0] PA_22_ANT_USPI2_MISO
    21 [0] PA_21_SD_USPI1_MISO
    20 [0] PA_20_SD_USPI1_MOSI

    19 [0] PA_19_DEBUG_U0_PIMO
    18 [0] PA_18_DEBUG_U0_POMI
    17 [0] PA_17_BUTTON0
    16 [1] PA_16_BLADE_CS write enabled

    15 [0] PA_15_BLADE_SCK
    14 [0] PA_14_BLADE_MOSI 
    13 [0] PA_13_BLADE_MISO

    12 [1] PA_12_BLADE_UPOMI write enabled

    11 [1] PA_11_BLADE_UPIMO write enabled

    10 [0] PA_10_I2C_SCL 
    09 [0] PA_09_I2C_SDA
    08 [0] PA_08_SD_CS_MCDA3

    07 [0] PA_07_HSMCI_MCDA2
    06 [0] PA_06_HSMCI_MCDA1
    05 [0] PA_05_HSMCI_MCDA0
    04 [0] PA_04_HSMCI_MCCDA

    03 [0] PA_03_HSMCI_MCCK
    02 [0] PA_02_SD_DETECT
    01 [0] PA_01_SD_WP
    00 [0] PA_00_TP54
*/

#define PIOB_OWER_INIT (u32)0x01FFFE0
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [1] PB_24_ANT_SRDY write enabled

    23 [1] PB_23_ANT_MRDY write enabled
    22 [1] PB_22_ANT_USPI2_CS write enabled
    21 [1] PB_21_ANT_RESET write enabled
    20 [1] PB_20_LED_RED write enabled

    19 [1] PB_19_LED_GRN write enabled
    18 [1] PB_18_LED_BLU write enabled
    17 [1] PB_17_LED_YLW write enabled
    16 [1] PB_16_LED_CYN write enabled

    15 [1] PB_15_LED_ORG write enabled
    14 [1] PB_14_LED_PRP write enabled
    13 [1] PB_13_LED_WHT write enabled
    12 [1] PB_12_LCD_BL_BLU write enabled

    11 [1] PB_11_LCD_BL_GRN write enabled
    10 [1] PB_10_LCD_BL_RED write enabled
    09 [1] PB_09_LCD_RST write enabled
    08 [0] PB_08_TP62

    07 [0] PB_07_TP60
    06 [0] PB_06_TP58
    05 [0] PB_05_TP56
    04 [0] PB_04_BLADE_AN1

    03 [0] PB_03_BLADE_AN0
    02 [0] PB_02_BUTTON3
    01 [0] PB_01_BUTTON2
    00 [0] PB_00_BUTTON1
*/

/* PIO Output Write Disable Register
0: No effect
1: Disables writing PIO_ODSR for the I/O line.
For now, don't worry about explictly disabling any write capability.
*/
#define PIOA_OWDR_INIT (u32)0x000000000
/* 
    31 [0] PA_31_HEARTBEAT
    30 [0] PA_30_AN_DEMO
    29 [0] PA_29_BUZZER2
    28 [0] PA_28_BUZZER1

    27 [0] PA_27_CLOCK_OUT
    26 [0] PA_26_ANT_PWR_EN
    25 [0] PA_25_ANT_USPI2_SCK
    24 [0] PA_24_SD_USPI1_SCK

    23 [0] PA_23_ANT_USPI2_MOSI
    22 [0] PA_22_ANT_USPI2_MISO
    21 [0] PA_21_SD_USPI1_MISO
    20 [0] PA_20_SD_USPI1_MOSI

    19 [0] PA_19_DEBUG_U0_PIMO
    18 [0] PA_18_DEBUG_U0_POMI
    17 [0] PA_17_BUTTON0
    16 [0] PA_16_BLADE_CS

    15 [0] PA_15_BLADE_SCK
    14 [0] PA_14_BLADE_MOSI
    13 [0] PA_13_BLADE_MISO

    12 [0] PA_12_BLADE_UPOMI

    11 [0] PA_11_BLADE_UPIMO

    10 [0] PA_10_I2C_SCL
    09 [0] PA_09_I2C_SDA
    08 [0] PA_08_SD_CS_MCDA3

    07 [0] PA_07_HSMCI_MCDA2
    06 [0] PA_06_HSMCI_MCDA1
    05 [0] PA_05_HSMCI_MCDA0
    04 [0] PA_04_HSMCI_MCCDA

    03 [0] PA_03_HSMCI_MCCK
    02 [0] PA_02_SD_DETECT
    01 [0] PA_01_SD_WP
    00 [0] PA_00_TP54
*/

#define PIOB_OWDR_INIT (u32)0x00000000
/*
    31 [0] PB_31_
    30 [0] PB_30_
    29 [0] PB_29_
    28 [0] PB_28_

    27 [0] PB_27_
    26 [0] PB_26_
    25 [0] PB_25_
    24 [0] PB_24_ANT_SRDY

    23 [0] PB_23_ANT_MRDY
    22 [0] PB_22_ANT_USPI2_CS
    21 [0] PB_21_ANT_RESET
    20 [0] PB_20_LED_RED

    19 [0] PB_19_LED_GRN
    18 [0] PB_18_LED_BLU
    17 [0] PB_17_LED_YLW
    16 [0] PB_16_LED_CYN

    15 [0] PB_15_LED_ORG
    14 [0] PB_14_LED_PRP
    13 [0] PB_13_LED_WHT
    12 [0] PB_12_LCD_BL_BLU

    11 [0] PB_11_LCD_BL_GRN
    10 [0] PB_10_LCD_BL_RED
    09 [0] PB_09_LCD_RST
    08 [0] PB_08_TP62

    07 [0] PB_07_TP60
    06 [0] PB_06_TP58
    05 [0] PB_05_TP56
    04 [0] PB_04_BLADE_AN1

    03 [0] PB_03_BLADE_AN0
    02 [0] PB_02_BUTTON3
    01 [0] PB_01_BUTTON2
    00 [0] PB_00_BUTTON1
*/

/* PIO Write Protect Mode Register PIO_WPMR
Enables the Write Protect if WPKEY corresponds to 0x50494F (�PIO� in ASCII).
Though this is defined in the user guide, there is no definition in the processor header file.
We don't want to lock access to the GPIO registers anyway, so we won't use this for now.
*/
#define PIO_WRITE_ENABLE         (u32)0x50494F00
/*
    31 -08 [0] WPKEY

    07 [0] Reserved
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] "
    02 [0] "
    01 [0] "
    00 [0] WPEN
*/


/***********************************************************************************************************************
$$$$$ PWM and Timer setup values
***********************************************************************************************************************/
#define PWM_CLK_INIT (u32)0x00010001
/*
    31 [0] Reserved
    30 [0] "
    29 [0] "
    28 [0] "

    27 [0] PREB MCK
    26 [0] "
    25 [0] "
    24 [0] "

    23 [0] DIVB = 1 => CLKB is on, no DIVB factor
    22 [0] "
    21 [0] "
    20 [0] "

    19 [0] "
    18 [0] "
    17 [0] "
    16 [1] "

    15 [0] Reserved
    14 [0] "
    13 [0] "
    12 [0] "

    11 [0] PREA MCK
    10 [0] "
    09 [0] "
    08 [0] "

    07 [0] DIVA = 1 => CLKA is on, no DIVA factor
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] "
    02 [0] "
    01 [0] "
    00 [1] "
*/


#define PWM_ENA_INIT (u32)0x00000003
/*
    31 - 4 [0] Reserved

    03 [0] Channel 3 not enabled
    02 [0] Channel 2 not enabled
    01 [1] Channel 1 enabled
    00 [1] Channel 0 enabled
*/

#define PWM_SCM_INIT (u32)0x00000000
/*
    31 [0] Reserved
    30 [0] "
    29 [0] "
    28 [0] "

    27 [0] "
    26 [0] "
    25 [0] "
    24 [0] "

    23 [0] PTRCS
    22 [0] "
    21 [0] "
    20 [0] "

    19 [0] Reserved
    18 [0] "
    17 [0] UPDM
    16 [0] "

    15 [0] Reserved
    14 [0] "
    13 [0] "
    12 [0] "

    11 [0] "
    10 [0] "
    09 [0] "
    08 [0] "

    07 [0] "
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] SYNC3 not synchronous
    02 [0] SYNC2 not synchronous
    01 [0] SYNC1 not synchronous
    00 [0] SYNC0 not synchronous
*/

#define PWM_CMR0_INIT (u32)0x00000003
#define PWM_CMR1_INIT (u32)0x00000003
/*
    31 [0] Reserved
    30 [0] "
    29 [0] "
    28 [0] "

    27 [0] "
    26 [0] "
    25 [0] "
    24 [0] "

    23 [0] "
    22 [0] "
    21 [0] "
    20 [0] "

    19 [0] "
    18 [0] DTLI dead-time low channel output is not inverted
    17 [0] DTHI dead-time high channel output is not inverted
    16 [0] DTE dead-time generator disabled

    15 [0] Reserved
    14 [0] "
    13 [0] "
    12 [0] "

    11 [0] "
    10 [0] CES channel event at end of PWM period
    09 [0] CPOL channel starts low
    08 [0] CALG period is left aligned

    07 [0] Reserved
    06 [0] "
    05 [0] "
    04 [0] "

    03 [0] CPRE clock is MCK/8
    02 [0] "
    01 [1] "
    00 [1] "
*/
#define CPRE_CLCK_SCALE           (u32)8
#define CPRE_CLCK                 CCLK_VALUE / CPRE_CLCK_SCALE

/* To achieve the full range of audio we want from 100Hz to 20kHz, we must be able to set periods
of 10ms to 50us.
10ms at 48MHz clock is 480,000 ticks
50us at 48MHz clock is 2400 ticks
Only 16 bits are available to set the PWM period, so scale the clock by 8:
10ms at 6MHz clock is 60,000 ticks
50us at 6MHz clock is 300 ticks

Set the default period for audio on channel 0 as 1/1kHz
1ms at 6MHz = 6000 (duty = 3000)
Set the default period for audio on channel 1 as 1/4kHz
0.25ms at 6MHz = 1500 (duty = 750)

In general, the period is 6000000 / frequency and duty is always period / 2. 
*/

#define PWM_CPRD0_INIT  (u32)6000
#define PWM_CPRD1_INIT  (u32)1500
#define PWM_CDTY0_INIT  (u32)(PWM_CPRD0_INIT << 1)
#define PWM_CDTY1_INIT  (u32)(PWM_CPRD1_INIT << 1)



/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File */
/*--------------------------------------------------------------------------------------------------------------------*/

#define TEMPLATE_INIT (u32)0x00000000
/*
    31 [0] 
    30 [0] 
    29 [0] 
    28 [0] 

    27 [0] 
    26 [0] 
    25 [0] 
    24 [0] 

    23 [0] 
    22 [0] 
    21 [0] 
    20 [0] 

    19 [0] 
    18 [0] 
    17 [0] 
    16 [0] 

    15 [0] 
    14 [0] 
    13 [0] 
    12 [0] 

    11 [0] 
    10 [0] 
    09 [0] 
    08 [0] 

    07 [0] 
    06 [0] 
    05 [0] 
    04 [0] 

    03 [0] 
    02 [0] 
    01 [0] 
    00 [0] 
*/


#endif /* __EIEF1 */
