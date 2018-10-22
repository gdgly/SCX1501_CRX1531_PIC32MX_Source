
/***********************************************************************/
/*  FILE        :uart.c                                               */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化
#include "pcf8563.h"

#define BaudRate 64
const UINT8 wifi_uart[6]={0xBB,0x00,0x06,0x80,0x00,0x00};

void Uart1_Init(void)
{
#if defined(__Product_PIC32MX2_WIFI__)
    RPA0R=1;         //Set RPA0-->U1TX
    U1RXR=4;         //Set U1RX-->RPB2
        
    U1BRG = BaudRate; //Set Baud rate
    U1STA = 0;
    U1MODEbits.ON = 1; //Enable UART for 8-bit data
    //no parity, 1 Stop bit
    U1STAbits.URXEN= 1; //Enable Transmit and Receive
    U1STAbits.UTXEN= 1;

    IPC8bits.U1IP = 3; // Set priority level=1
    IPC8bits.U1IS = 3; // Set Subpriority level=1
    // Can be done in a single operation by assigning PC2SET = 0x0000000D
    IFS1bits.U1RXIF = 0; // Clear the timer interrupt status flag
    IEC1bits.U1RXIE = 1; // Enable timer interrupts
#endif
}
#if defined(__Product_PIC32MX2_WIFI__)
void __ISR(_UART_1_VECTOR,ipl3)Uart1Handler(void)
{
    UART_DATA_buffer[UART_DATA_cnt] = U1RXREG;
    if((FLAG_UART_0xBB==0)&&(UART_DATA_cnt>=1)){
        if((UART_DATA_buffer[UART_DATA_cnt-1]==0xBB)&&(UART_DATA_buffer[UART_DATA_cnt]==0x00)){TIME_UART=8;UART_DATA_cnt=1;UART_DATA_buffer[0]=0xBB;UART_DATA_buffer[1]=0x00;FLAG_UART_0xBB=1;}
    }
    UART_DATA_cnt++;
    if(UART_DATA_buffer[6]==0x05){
        if(UART_DATA_cnt>=15){
            UART_DATA_cnt=0;
            FLAG_UART_0xBB=0;
            for(UART_DATA_i=0;UART_DATA_i<15;UART_DATA_i++)UART1_DATA[UART_DATA_i]=UART_DATA_buffer[UART_DATA_i];
            FLAG_UART_R=1;
        }
    }
    else if(UART_DATA_buffer[6]==0x08){
        if(UART_DATA_cnt>=18){
            UART_DATA_cnt=0;
            FLAG_UART_0xBB=0;
            for(UART_DATA_i=0;UART_DATA_i<18;UART_DATA_i++)UART1_DATA[UART_DATA_i]=UART_DATA_buffer[UART_DATA_i];
            FLAG_UART_R=1;
        }
    }
    if((UART_DATA_cnt>=18)||((UART_DATA_cnt>2)&&(TIME_UART==0))){UART_DATA_cnt=0;FLAG_UART_0xBB=0;}
    IFS1bits.U1RXIF = 0;
}
#endif
void UART_Decode(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT8 i;
    UINT16 m=0;
    UINT16 n=0;
    uni_rom_id y;
    for(i=0;i<6;i++){
        if(UART1_DATA[i]==wifi_uart[i]);
        else {FLAG_UART_ok=0;return;}
    }
    if((UART_DATA_buffer[6]==0x05)&&(UART_DATA_buffer[7]==0x00)){
        for(i=8;i<13;i++)  m+=UART1_DATA[i];
        n=UART1_DATA[13]+UART1_DATA[14]*256;
        if(m==n){
            ID_data.IDB[0]=UART1_DATA[8];
            ID_data.IDB[1]=UART1_DATA[9];
            ID_data.IDB[2]=UART1_DATA[10];
            ID_data.IDB[3]=UART1_DATA[11];
            Control_code=UART1_DATA[12];
            eeprom_IDcheck_UART();
            if(FLAG_IDCheck_OK==1){FLAG_IDCheck_OK=0;FLAG_UART_ok=1;}
        }
        else FLAG_UART_ok=0;
    }
    else if((UART_DATA_buffer[6]==0x08)&&(UART_DATA_buffer[7]==0x00)){
        for(i=8;i<16;i++)  m+=UART1_DATA[i];
        n=UART1_DATA[16]+UART1_DATA[17]*256;
        if(m==n){
            if(UART_DATA_buffer[8]==0x00)Set_Time(&UART1_DATA[9]);       //==0x00  写时钟
            else if(UART_DATA_buffer[8]==0x01){                //==0x01  读时钟
                Read_Time(number_time);
                U1TXREG=0xBB;
                U1TXREG=0x00;
                U1TXREG=0x07;
                U1TXREG=0x00;
                U1TXREG=0x00;
                U1TXREG=0x00;
                U1TXREG=0x08;
                U1TXREG=0x00;
                Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                U1TXREG=0x01;
                m=0;
                for(i=0;i<7;i++){U1TXREG=number_time[i];m=m+number_time[i];}
                Delay100us(10);//延时2.1mS以上，缓冲区是8级FIFO
                m=m+0x01;
                U1TXREG=m%256;
                U1TXREG=m/256;
            }
        }
    }
#endif
}