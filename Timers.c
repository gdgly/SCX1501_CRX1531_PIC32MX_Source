
/***********************************************************************/
/*  FILE        :Timers.c                                           */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化


void timer2_Init(void)
{
/*This code example enables the Timer2 interrupts, loads the Timer2 period
register, and starts the timer.
When a Timer2 period match interrupt occurs, the interrupt service routine must clear
the Timer2 interrupt status flag in software.*/

    T2CONbits.ON = 0; // Stop the timer and clear the control register,
    T2CONbits.TCKPS = 7; // prescaler at 1:256,internal clock source    1/(20MHz/256)==12.8us     1/(40MHz/256)==6.4us    1/(10MHz/256)==25.6us
    //T2CONbits.TCKPS = 6; // prescaler at 1:64,internal clock source    1/(5MHz/64)==12.8us
    TMR2 = 0x0; // Clear the timer register
    #if defined(__Product_PIC32MX2_Receiver__)
       //PR2 = 39;//0xFFFF; // Load the period register    25.6us*39==1ms    //SYSCLK=10M
       PR2 = 78;//0xFFFF; // Load the period register    12.8us*78==1ms    //SYSCLK=20M
       //PR2 = 78;  //0xFFFF; // Load the period register       12.8us*78==1ms    //SYSCLK=5M
    #endif
    #if defined(__Product_PIC32MX2_WIFI__)
       PR2 = 156;//0xFFFF; // Load the period register    6.4us*156==1ms       //SYSCLK=40M
    #endif
    IPC2bits.T2IP = 1; // Set priority level=1
    IPC2bits.T2IS = 1; // Set Subpriority level=1
    // Can be done in a single operation by assigning PC2SET = 0x0000000D
    IFS0bits.T2IF = 0; // Clear the timer interrupt status flag
    IEC0bits.T2IE = 1; // Enable timer interrupts

    T2CONbits.ON = 1; // Start the timer
}


/*This code example demonstrates a simple interrupt service routine for Timer
interrupts.The user’s code at this ISR handler should perform any application
specific operations and must clear the corresponding Timer interrupt status flag
before exiting.*/
void __ISR(_TIMER_2_VECTOR,ipl1)Timer1Handler(void)
{
/*... perform application specific operations in response to the interrupt*/
    //Receiver_LED_OUT=!Receiver_LED_OUT;    ////测试，测试完后需要删除
    //if(FLAG_APP_TX==1)ID_code_function();
    //ADF7021_DATA_tx=!ADF7021_DATA_tx;
    if(TIMER1s)--TIMER1s;
    if(TIMER300ms)--TIMER300ms;
    if(TIMER18ms)--TIMER18ms;
    if(TIMER250ms_STOP)--TIMER250ms_STOP;
    if (TIME_10ms)--TIME_10ms;
    else{                            // 10mS FLAG
	  TIME_10ms = 10;
	  FG_10ms = 1;
    }
    IFS0bits.T2IF = 0; // Be sure to clear the Timer1 interrupt status
}



//void timer4_Init(void)
//{
///*This code example enables the Timer2 interrupts, loads the Timer2 period
//register, and starts the timer.
//When a Timer2 period match interrupt occurs, the interrupt service routine must clear
//the Timer2 interrupt status flag in software.*/
//
//    T4CONbits.ON = 0; // Stop the timer and clear the control register,
//    T4CONbits.TCKPS = 7; // prescaler at 1:256,internal clock source    1/(20MHz/256)==12.8us     1/(40MHz/256)==6.4us    1/(10MHz/256)==25.6us
//    //T2CONbits.TCKPS = 6; // prescaler at 1:64,internal clock source    1/(5MHz/64)==12.8us
//    TMR4 = 0x0; // Clear the timer register
//    #if defined(__Product_PIC32MX2_Receiver__)
//       //PR2 = 39;//0xFFFF; // Load the period register    25.6us*39==1ms    //SYSCLK=10M
//       PR4 = 65;//0xFFFF; // Load the period register    12.8us*65==0.832ms    //SYSCLK=20M
//       //PR2 = 78;  //0xFFFF; // Load the period register       12.8us*78==1ms    //SYSCLK=5M
//    #endif
//    #if defined(__Product_PIC32MX2_WIFI__)
//       PR4 = 156;//0xFFFF; // Load the period register    6.4us*156==1ms       //SYSCLK=40M
//    #endif
//    IPC4bits.T4IP = 5; // Set priority level=1
//    IPC4bits.IC4IS = 1; // Set Subpriority level=1
//    // Can be done in a single operation by assigning PC2SET = 0x0000000D
//    IFS0bits.T4IF = 0; // Clear the timer interrupt status flag
//    IEC0bits.T4IE = 1; // Enable timer interrupts
//
//    T4CONbits.ON = 1; // Start the timer
//}
//
//
///*This code example demonstrates a simple interrupt service routine for Timer
//interrupts.The user’s code at this ISR handler should perform any application
//specific operations and must clear the corresponding Timer interrupt status flag
//before exiting.*/
//void __ISR(_TIMER_4_VECTOR,ipl5)Timer4Handler(void)
//{
///*... perform application specific operations in response to the interrupt*/
//    //Receiver_LED_OUT=!Receiver_LED_OUT;    ////测试，测试完后需要删除
//    //if(FLAG_APP_TX==1)ID_code_function();
//    if(FLAG_APP_TX==1)ID_code_function();
//    //ADF7021_CLKOUT=!ADF7021_CLKOUT;//测试，测试完后需要删除
//    IFS0bits.T4IF = 0; // Be sure to clear the Timer1 interrupt status
//}

//void timer2_Init(void)
//{
///*This code example enables the Timer2 interrupts, loads the Timer2 period
//register, and starts the timer.
//When a Timer2 period match interrupt occurs, the interrupt service routine must clear
//the Timer2 interrupt status flag in software.*/
//
//    T2CONbits.ON = 0; // Stop the timer and clear the control register,
//    T2CONbits.TCKPS = 0; // prescaler at 1:256,internal clock source    1/(20MHz/1)==0.05us     1/(40MHz/1)==0.025us
//    TMR2 = 0x0; // Clear the timer register
//    #if defined(__Product_PIC32MX2_Receiver__)
//       PR2 = 16667;//39;//0xFFFF; // Load the period register    0.05us*16667==833.33us    //SYSCLK=20M
//    #endif
//    #if defined(__Product_PIC32MX2_WIFI__)
//       PR2 = 33333;//0xFFFF; // Load the period register    0.025us*33333==833.33us       //SYSCLK=40M
//    #endif
//    IPC2bits.T2IP = 1; // Set priority level=1
//    IPC2bits.T2IS = 1; // Set Subpriority level=1
//    // Can be done in a single operation by assigning PC2SET = 0x0000000D
//    IFS0bits.T2IF = 0; // Clear the timer interrupt status flag
//    IEC0bits.T2IE = 1; // Enable timer interrupts
//
//    T2CONbits.ON = 1; // Start the timer
//}
//
//
///*This code example demonstrates a simple interrupt service routine for Timer
//interrupts.The user’s code at this ISR handler should perform any application
//specific operations and must clear the corresponding Timer interrupt status flag
//before exiting.*/
//void __ISR(_TIMER_2_VECTOR,ipl1)Timer1Handler(void)
//{
///*... perform application specific operations in response to the interrupt*/
//    //Receiver_LED_OUT=!Receiver_LED_OUT;    ////测试，测试完后需要删除
//    if(FLAG_APP_TX==1)ID_code_function();
//    if(TIMER1s)--TIMER1s;
//    if(TIMER300ms)--TIMER300ms;
//    if(TIMER18ms)--TIMER18ms;
//    IFS0bits.T2IF = 0; // Be sure to clear the Timer1 interrupt status
//}