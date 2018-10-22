
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

void HA_uart_send_APP(void);

#define BaudRate 64
#if defined(__Product_PIC32MX2_WIFI__)
const UINT8 wifi_uart[6]={0xBB,0x00,0x06,0x80,0x00,0x00};
const UINT8 HA_uart_open[5]={79,80,69,78,32};
const UINT8 HA_uart_close[5]={67,76,79,83,69};
const UINT8 HA_uart_err[5]={69,82,82,32,32};
#endif

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
//    if(UART_DATA_buffer[2]==0x06){
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
            else if(UART_DATA_buffer[6]==0x00){
                if(UART_DATA_cnt>=10){
                    UART_DATA_cnt=0;
                    FLAG_UART_0xBB=0;
                }
            }
//    }
//    else if(UART_DATA_buffer[2]==0x20){
//        if((UART_DATA_buffer[4]==0x01)||(UART_DATA_buffer[5]==0x01))FLAG_HA_Emial=1;
//        else FLAG_HA_Emial=0;
//    }
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
            ID_data.IDB[3]=0x00;
            //UART1_DATA[11];
            Control_code=UART1_DATA[12];
            eeprom_IDcheck_UART();
            if(FLAG_IDCheck_OK==1){
                if(Control_code==0x00){FLAG_HA_Inquiry=1;DATA_Packet_Control_0=0x00;}    //表示APP查询
                FLAG_IDCheck_OK=0;
                FLAG_UART_ok=1;}
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

void HA_uart_send(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT8 i,j,h,l;
    UINT16 m;
    UINT32 h0;

    Read_Time(number_time);
    for(i=1;i<7;i++){
        if(i!=4){
            h=number_time[i];
            if(i<=3)j=22-(i-1)*3;
            else j=22-(i-2)*3;
            l=h&0x0F;
            HA_uart[j+1]=l+0x30;
            h=h&0xF0;
            h=h>>4;
            HA_uart[j]=h+0x30;
        }
    }
    h0=DATA_Packet_ID;
    for(i=8;i>0;i--){
        h=h0%10;
        HA_uart[27+i]=h+0x30;
        h0=h0/10;
    }
    for(i=0;i<5;i++){
        if(DATA_Packet_Control==0x81)HA_uart[37+i]=HA_uart_open[i];
        else if(DATA_Packet_Control==0x82)HA_uart[37+i]=HA_uart_close[i];  
        else if(DATA_Packet_Control==0x83)HA_uart[37+i]=HA_uart_err[i];
    }
    m=0;
    for(i=8;i<43;i++)m=m+HA_uart[i];
    HA_uart[43]=m%256;
    HA_uart[44]=m/256;

//    UART_send_count=0;
//    FLAG_HA_Emial=1;
//    while((FLAG_HA_Emial==1)&&(UART_send_count<=3))
//    {
        for(i=0;i<45;i++){
            U1TXREG=HA_uart[i];
            if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
        }
       Delay100us(300);
//        UART_send_count++;
//        Delay100us(10000);
//    }


   HA_uart_send_APP();
    
 #endif
}

void HA_uart_send_APP(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    uni_rom_id b0;
    UINT8 i;
    UINT16 m;

    b0.IDL=DATA_Packet_ID;
    HA_uart_app[8]=b0.IDB[0];
    HA_uart_app[9]=b0.IDB[1];
    HA_uart_app[10]=b0.IDB[2];
    if(DATA_Packet_Control==0x81)HA_uart_app[11]=01;
    else if(DATA_Packet_Control==0x82)HA_uart_app[11]=02;
    else if(DATA_Packet_Control==0x83)HA_uart_app[11]=03;
    m=0;
    for(i=8;i<13;i++)m=m+HA_uart_app[i];
    HA_uart_app[13]=m%256;
    HA_uart_app[14]=m/256;

    for(i=0;i<15;i++){
        U1TXREG=HA_uart_app[i];
        if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
    }
 #endif
}