/***********************************************************************/
/*  FILE        :ID_Decode.c                                           */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化
#include "adf7021.h"
#include "Timers.h"


void ID_Decode_function(void);
void  Freq_Scanning(void);
void ID_code_function(void);

void ID_Decode_Initial_INT(void)
{
    /*The following code example will set INT4 to trigger on a high-to-low
    transition edge.The CPU must be set up for either multi or single vector
    interrupts to handle external interrupts*/
    //    /* Input Pins Group 1 */
    IEC0bits.CTIE =0;
    IEC0bits.CS0IE=0;
    IEC0bits.CS1IE=0;
    
    INT2Rbits.INT2R=7;   //INT4-->RCP5
    IEC0bits.INT2IE = 0; // disable INT3
    INTCONbits.INT2EP = 1; // clear the bit for falling edge trigger
    IPC2bits.INT2IP= 6; // Set priority level=6
    IPC2bits.INT2IS = 3; // Set Subpriority level=3
    IFS0bits.INT2IF = 0; // clear the interrupt flag
    IEC0bits.INT2IE = 1; // enable INT3
}
void __ISR(_EXTERNAL_2_VECTOR, ipl6) ExternalHandler(void)
{
    asm ("di");
    if(FLAG_APP_RX==1) 
        ID_Decode_function();
    else ID_code_function();
    //Receiver_LED_TX=!Receiver_LED_TX;//测试，测试完后需要删除
    IFS0bits.INT2IF = 0;
    asm ("ei");
}
void ID_code_function(void)
{
    if(PCB_debug_num0==1)ADF7021_DATA_tx=!ADF7021_DATA_tx;
}
void ID_Decode_function(void)
{
    UINT16 DATA_Packet_Syn_bak=0;
     switch (rxphase){
        case 0:
                DATA_Packet_Syn=DATA_Packet_Syn<<1;
                if(ADF7021_DATA_rx)DATA_Packet_Syn+=1;
                //if(DATA_Packet_Syn==0x55555555){rxphase=1;TIMER18ms=65;DATA_Packet_Syn=0;DATA_Packet_Head=0;break;}
                //Receiver_LED_OUT=ADF7021_DATA_rx;//测试，测试完后需要删除
                if(TIMER18ms==0){
                                 DATA_Packet_Syn_bak=DATA_Packet_Syn&0x0000FFFF;
                                 if((DATA_Packet_Syn_bak==0x5555)||(DATA_Packet_Syn_bak==0xAAAA));
                                 else FLAG_Receiver_Scanning=1;
                                }
                //if(DATA_Packet_Syn==0x55555555){rxphase=1;TIMER18ms=65;DATA_Packet_Syn=0;DATA_Packet_Head=0;}
                if((DATA_Packet_Syn&0xFFFFFFFF)==0x55555555){rxphase=1;TIMER18ms=65;DATA_Packet_Syn=0;DATA_Packet_Head=0;}
                break;
	case 1:
                //Receiver_LED_TX=0;//测试，测试完后需要删除
                //if(TIMER18ms==0)rxphase=0;  //Scanning  测试
                DATA_Packet_Head=DATA_Packet_Head<<1;
                if(ADF7021_DATA_rx)DATA_Packet_Head+=1;
                //DATA_Packet_Head=DATA_Packet_Head&0x0000FFFF;
                if(DATA_Packet_Head==0x5515){rxphase=2;DATA_Packet_Syn=0;DATA_Packet_Head=0;DATA_Packet_Code_i=0;}
		break;
        case 2:
                DATA_Packet_Code_g=DATA_Packet_Code_i/32;
                DATA_Packet_Code[DATA_Packet_Code_g]=DATA_Packet_Code[DATA_Packet_Code_g]<<1;
                if(ADF7021_DATA_rx)DATA_Packet_Code[DATA_Packet_Code_g]+=1;
                DATA_Packet_Code_i++;
                if(DATA_Packet_Code_i>=96)rxphase=3;
                //Receiver_LED_TX=0;//测试，测试完后需要删除
                break;
        case 3:
                //Receiver_LED_TX=0;//测试，测试完后需要删除
                FLAG_Receiver_IDCheck=1;
                rxphase=0;
                DATA_Packet_Syn=0;
                TIMER18ms=0;
                break;
        default:
               break;
	}
}
void ID_Decode_IDCheck(void)
{
    UINT32 data_in;
    UINT16 data_out;
    UINT16 data_NRZ[3];
    UINT i,j;
    if(FLAG_Receiver_IDCheck)
    {
        FLAG_Receiver_IDCheck=0;
        for(i=0;i<3;i++)
        {
            data_in=DATA_Packet_Code[i];
            data_out=0;
            data_in=data_in>>1;
            for(j=0;j<16;j++)
            {
                data_out=data_out<<1;
                if(data_in&0x00000001)data_out+=1;
                data_in=data_in>>2;
            }
           data_NRZ[i] =data_out;
        }
        if(data_NRZ[2]==data_NRZ[0]+data_NRZ[1])
        {
            FLAG_Receiver_OK=1;
            DATA_Packet_ID=(data_NRZ[1]&0x00FF)*65536+data_NRZ[0];
            if(EEPROM_Receiver_ID==DATA_Packet_ID)
            {
                #if defined(__Product_PIC32MX2_Receiver__)
                    Receiver_LED_OUT=1;           //测试，测试完后需要删除
                #endif
                #if defined(__Product_PIC32MX2_WIFI__)
                   WIFI_LED_TX=1;
                #endif
                TIMER1s=1000;
            }
        }
        else FLAG_Receiver_OK=0;     
    }
}


void  Freq_Scanning(void)
{
    if((FLAG_Receiver_Scanning==1)&&(FLAG_APP_RX==1))
    {
        FLAG_Receiver_Scanning=0;
        Freq_Scanning_CH++;
        if(Freq_Scanning_CH>6)Freq_Scanning_CH=1;
        dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
        TIMER18ms=18;

    }
}






void ID_Decode_Initial_CNx(void)
{
     /*The following code example illustrates a Change Notice interrupt configuration for pins
    CNC5(PORTC.RC5).*/
                      /* NOTE:disable vector interrupts prior to configuration */
    CNCONCbits.ON= 1; // Enable Change Notice module
    CNENCbits.CNIEC5 = 1; // Enable individual CN pins CNC5
    //CNPUCbits.CNPUC5= 1; // Enable weak pull ups for pins CN5

    ADF7021_DATA_CLK;             /* read port(s) to clear mismatch on change notice pins */

    IPC8bits.CNIP = 5; // Set priority level=5
    IPC8bits.CNIS = 3; // Set Subpriority level=3
                         // Could have also done this in single
                        // operation by assigning IPC6SET = 0x00170000
    IFS1bits.CNCIF = 0;   // Clear the interrupt flag status bit
    IEC1bits.CNCIE = 1;   // Enable Change Notice interrupts
                        /* re-enable vector interrupts after configuration */
}

/*
The following code example demonstrates a simple interrupt service routine for CN
interrupts.The user’s code at this vector can perform any application specific
operations.The user’s code must read the CN corresponding PORT registers to clear the
mismatch conditions before clearing the CN interrupt status flag.Finally, the CN
interrupt status flag must be cleared before exiting.
*/
void __ISR(_CHANGE_NOTICE_VECTOR, ipl5) ChangeNoticeHandler(void)
{
    UINT8 CNC5_vlaue;
    //... perform application specific operations in response to the interrupt
    //ADF7021_DATA_CLK; // Read PORTC to clear CN1 mismatch condition
    //Receiver_LED_TX= CNC5_vlaue;   //测试，测试完后需要删除
    //Receiver_LED_TX= !Receiver_LED_TX;   //测试，测试完后需要删除
    IFS1bits.CNCIF = 0;// Be sure to clear the CN interrupt status
    // flag before exiting the service routine.
}