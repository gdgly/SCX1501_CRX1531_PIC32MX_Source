
/*********************************************************************
 *
 *                  Boot Loader Simple Application
 *
 *********************************************************************
 * FileName:        main.c
 * Dependencies:
 * Processor:       PIC32
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology, Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the ?ompany? for its PIC32 Microcontroller is intended
 * and supplied to you, the Company? customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN ?S IS?CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 *
 * $Id: $
 * $Name: x.x $
 *
 **********************************************************************/

/*
Boot Loader Example Application
This project shows an application that works with the boot loader.  The application is almost 
same as a standard c program for pic32.  The  only difference is that this project use an updated procdef.ld linker file.  
This linker file sets the code to reside wholly in the program flash area and not use the Boot block area.  

Code and debug this application as any other standard application using real ice. just make sure this procdefs.ld is in your project folder.

When you are ready to deploy your app, program the PIC32  with the boot loader firmware, and then program your application 
to the PIC32 using the boot loader.  Now the PIC32 has both the boot loader and your application.

Note: make sure the boot loader and your application, both use the same fuse settings.  This is because the boot loader does not reprogram the fuse settings to the PIC.

*/
 
#include <stdlib.h>
#include <plib.h>
#include "initial.h"		// ³õÊ¼»¯
#include "adf7021.h"
#include "ID_Decode.h"
#include "Timers.h"
#include "Uart.h"
#include "EEPROM.h"
#include "pcf8563.h"

#if   (((__PIC32_FEATURE_SET__ >= 100) && (__PIC32_FEATURE_SET__ <= 299)))
    #define __PIC32MX1XX_2XX__
#elif (((__PIC32_FEATURE_SET__ >= 300) && (__PIC32_FEATURE_SET__ <= 799)))
    #define __PIC32MX3XX_7XX__
#else
    #error("Controller not supported")
#endif


// *****************************************************************************
// *****************************************************************************
// Device Configuration Bits (Runs from Aux Flash)
// *****************************************************************************
// *****************************************************************************
// Configuring the Device Configuration Registers
// 80Mhz Core/Periph, Pri Osc w/PLL, Write protect Boot Flash
#pragma config UPLLEN   = ON        // USB PLL Enabled
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#pragma config BWP = OFF

#pragma config JTAGEN = OFF
#pragma config FCKSM    = CSDCMD   // Clock Switching & Fail Safe Monitor: Clock Switching Disabled, Clock Monitoring Disabled
#pragma config OSCIOFNC = OFF      // CLKO Enable: Disabled
#pragma config IESO     = OFF      // Internal/External Switch-over: Disabled
#pragma config FSOSCEN  = OFF      // Secondary Oscillator Enable: Disabled
#if defined(TRANSPORT_LAYER_ETH)
	#pragma config FMIIEN = OFF, FETHIO = OFF	// external PHY in RMII/alternate configuration
#endif

#if defined(__PIC32MX1XX_2XX__)
    // For PIC32MX1xx, PIC32MX2xx devices there are jumpers on PIM to choose from PGx1/PGx2.
    #pragma config ICESEL = ICS_PGx1    // ICE pins configured on PGx1 (PGx2 is multiplexed with USB D+ and D- pins).
    // For PIC32MX1xx, PIC32MX2xx devices the output divisor is set to 2 to produce max 40MHz clock.
    #if defined(__32MX230F064D__)
        #pragma config FPLLODIV = DIV_2         // PLL Output Divider: Divide by 2  SYSCLK=40M
        #pragma config WDTPS = PS128 // WDT timeout period = 1ms
    #endif

    #if defined(__32MX210F016D__)
        #pragma config FPLLODIV = DIV_4 //DIV_4         // PLL Output Divider: Divide by 8    SYSCLK=10M
        #pragma config WDTPS = PS128 // WDT timeout period = 1ms
    #endif
#elif defined(__PIC32MX3XX_7XX__)
    // For PIC32MX3xx, PIC32MX4xx, PIC32MX5xx, PIC32MX6xx and PIC32MX7xx
    // devices the ICE connection is on PGx2. .
    #pragma config ICESEL = ICS_PGx2    // ICE pins configured on PGx2, Boot write protect OFF.
    //For PIC32MX3xx, PIC32MX4xx, PIC32MX5xx, PIC32MX6xx and PIC32MX7xx devices,
    //the output divisor is set to 1 to produce max 80MHz clock.
    #pragma config FPLLODIV = DIV_1         // PLL Output Divider: Divide by 1
#endif


#if defined(__PIC32MX1XX_2XX__)
    #define SYS_FREQ 				(40000000L)
#else
    #define SYS_FREQ 				(80000000L)
#endif
#define TOGGLES_PER_SEC			18
#define CORE_TICK_RATE	       (SYS_FREQ/2/TOGGLES_PER_SEC)

////////////////////////////////////////////////////////////

int main(void)
{
   DDPCONbits.JTAGEN = 0; // Disable JTAG
    // WDT timeout period is set in the Device Configuration (WDTPS)
    EnableWDT(); // Enable the WDT
	//unsigned int pb_clock;
   SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

//    OpenCoreTimer(CORE_TICK_RATE);
//
//    mConfigIntCoreTimer((CT_INT_ON | CT_INT_PRIOR_2 | CT_INT_SUB_PRIOR_0));

    VHF_GPIO_INIT();
    //ID_Decode_Initial_CNx();
    ID_Decode_Initial_INT();
    timer2_Init();
    Uart1_Init();
    INTEnableSystemMultiVectoredInt();

    all_Erase_EEPROM();
    ID_EEPROM_Initial();
    dd_set_ADF7021_Power_on();
    FLAG_HA_L_signal=1;
    FLAG_HA_ERR_signal=1;

#if defined(__Product_PIC32MX2_Receiver__)
    Receiver_LED_OUT=1;    
    for(time_3sec=0;time_3sec<1000;time_3sec++){
    Delay100us(10);
    ClearWDT(); // Service the WDT
    }
    time_3sec=10;
    Receiver_LED_OUT=0;
#endif

    TIME_EMC=10;
    while(1)
    {
        //if(time_3sec==0){time_3sec=10;Receiver_LED_TX=!Receiver_LED_TX;}

        ClearWDT(); // Service the WDT
        ADF7021_change_TXorRX();
        ID_Decode_IDCheck();
        ID_Decode_OUT();
        Freq_Scanning();
        ID_learn();

     }
	return 0;
}

////////////////////////////////////////////////////////////
// Core Timer Interrupts
//
void __ISR(_CORE_TIMER_VECTOR, ipl2) CoreTimerHandler(void)
{
    // clear the interrupt flag
    mCTClearIntFlag();
    // update the period
    UpdateCoreTimer(CORE_TICK_RATE);
}
