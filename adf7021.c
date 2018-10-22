
/***********************************************************************/
/*  FILE        :ADF7021.c                                             */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化

void dd_write_7021_reg(unsigned char* reg_bytes)
{
    UINT8 i, j;
    UINT8 byte;
    UINT8 DATA_7021_byte[4];
    for (i=0; i<=3; i++)
    {
        DATA_7021_byte[3-i]=reg_bytes[i];
    }
    ADF7021_SLE = 0;
    //asm ("nop");
    ADF7021_SCLK = 0;


    /* Clock data out MSbit first */
//#ifdef _BIG_ENDIAN_
//    for (i=0; i<=3; i++)
//#else
//    for (i=3; i>=0; i--)
//#endif
    for (i=0; i<=3; i++)
    {
        byte = DATA_7021_byte[i];

        for (j=8; j>0; j--)
        {
            ADF7021_SCLK = 0;
            if (byte & 0x80) ADF7021_SDATA = 1;
            else ADF7021_SDATA = 0;
            //asm ("nop");
            ADF7021_SCLK = 1;
            byte += byte; // left shift 1
        }
        //asm ("nop");
        ADF7021_SCLK = 0;
    }


    /* Strobe the latch */

    ADF7021_SLE = 1;
    //asm ("nop");
    ADF7021_SLE = 1; // Slight pulse extend
    //asm ("nop");
    ADF7021_SDATA = 0;
    //asm ("nop");
    ADF7021_SLE = 0;

    /* All port lines left low */

}

ADF70XX_REG_T dd_read_7021_reg(UINT8 readback_config)
{
    ADF70XX_REG_T register_value;
    UINT8 DATA_7021_byte[4];
    INT8 i, j;
    UINT8 byte;



    /* Write readback and ADC control value */
    register_value.whole_reg = (readback_config & 0x1F) << 4;
    register_value.whole_reg |= 7; // Address the readback setup register

    dd_write_7021_reg(&register_value.byte[0]);

    register_value.whole_reg = 0;


    /* Read back value */

    ADF7021_SDATA = 0;
    ADF7021_SCLK = 0;
    ADF7021_SLE = 1;

   //Clock in first bit and discard 
    ADF7021_SCLK = 1;
    byte = 0; // Slight pulse extend
    ADF7021_SCLK = 0;


    /* Clock in data MSbit first */
  //  for (i=2; i<=3; i++)
    for (i=3; i<=2; i--)
    {
        for (j=8; j>0; j--)
        {
            ADF7021_SCLK = 1;
            byte += byte; // left shift 1
            ADF7021_SCLK = 0;

          if (ADF7021_SREAD) byte |= 1;
        }
        DATA_7021_byte[i] = byte;

		Delayus(1);	//wait for a bit time
	    		
	}//for i=2 : 3;

	ADF7021_SCLK = 1;
	ADF7021_SCLK = 0;

/*
	ADF7021_SCLK = 1;
	ADF7021_SCLK = 0;
*/
    ADF7021_SLE = 0;
    // All port lines left low
    DATA_7021_byte[0]=0;
    DATA_7021_byte[1]=0;
    for (i=0; i<=3; i++)
    {
        register_value.byte[i] =DATA_7021_byte[i];
    }
    return register_value;
}

const unsigned char gain_correction[] =
    { 2*86, 0, 0, 0, 2*58, 2*38, 2*24, 0, 
	0, 0, 0, 0, 0, 0, 0, 0 }; // 7021

UINT8 dd_read_rssi_7021_reg(UINT8 readback_config)
{
    ADF70XX_REG_T register_value;
    UINT8 DATA_7021_byte[4];
    INT8 i, j;
    UINT8 byte;
    UINT16 num,num1;

    /* Write readback and ADC control value */
    register_value.whole_reg = (readback_config & 0x1F) << 4;
    register_value.whole_reg |= 7; // Address the readback setup register

    dd_write_7021_reg(&register_value.byte[0]);

    register_value.whole_reg = 0;


    /* Read back value */

    ADF7021_SDATA = 0;
    ADF7021_SCLK = 0;
    ADF7021_SLE = 1;

   //Clock in first bit and discard 
    ADF7021_SCLK = 1;
    byte = 0; // Slight pulse extend
    ADF7021_SCLK = 0;


    /* Clock in data MSbit first */
    for (i=2; i<=3; i++)
  //  for (i=3; i<=2; i--)
    {
        for (j=8; j>0; j--)
        {
            ADF7021_SCLK = 1;
            byte += byte; // left shift 1
            ADF7021_SCLK = 0;

          if (ADF7021_SREAD) byte |= 1;
        }
        DATA_7021_byte[i] = byte;

		Delayus(1);	//wait for a bit time
	    		
	}//for i=2 : 3;

	ADF7021_SCLK = 1;
	ADF7021_SCLK = 0;

/*
	ADF7021_SCLK = 1;
	ADF7021_SCLK = 0;
*/
    ADF7021_SLE = 0;
    // All port lines left low
    num=(DATA_7021_byte[2]*256+DATA_7021_byte[3])*2;
    num1=(num & 0x0F00)>>8;
    num = num & 0x00FF;
	num += gain_correction[num1] ;
    num = num /4;
	//RSSI(dBm) = rssi + 130
    return num;
}

void dd_set_TX_mode(void)
{
//        ADF7021_CE = 0;         //以下是测试
//        Delayus(20000);
//        ADF7021_CE = 1;
//        Delayus(20000);        //测试END
        
	ADF70XX_REG_T register_value;
        //write R1, turn on VCO
	register_value.whole_reg = 0x031BD011;
	dd_write_7021_reg(&register_value.byte[0]);
	Delayus(800);		//delay 800us

	//write R3, turn on TX/RX clocks
	register_value.whole_reg = 0x29915CD3;//0x2991A0D3;
	dd_write_7021_reg(&register_value.byte[0]);
        Delayus(40);		//delay 40us

	//write R0, turn on PLL
	//register_value.whole_reg = 0x0154DC30;       //CH=426.075MHz
        //register_value.whole_reg = 0x01575710;   //CH=429.175MHz
        //register_value.whole_reg = 0x01576140;//CH=429.225MHz
        //if(TX_Freq_CH==1)register_value.whole_reg = 0x0154DC30;       //CH=426.075MHz
        if(TX_Freq_CH==2)register_value.whole_reg = 0x01575710;   //CH=429.175MHz
        else if(TX_Freq_CH==4)register_value.whole_reg = 0x01575C30;  //CH=429.200MHz
        else if(TX_Freq_CH==6)register_value.whole_reg = 0x01576140;//CH=429.225MHz
	dd_write_7021_reg(&register_value.byte[0]);
	Delayus(40);		//delay 40us

	//write R2, turn on PA
	register_value.whole_reg = 0x00564882;//0x00536882;//0x006B6882;			//Ramp Rate = 16 codes/bit, TX level = 2;
	dd_write_7021_reg(&register_value.byte[0]);
        Delayus(40);		//delay 40us

	register_value.whole_reg = 0x00289A14;//0x00268614;
	dd_write_7021_reg(&register_value.byte[0]);
        Delayus(40);		//delay 40us

//	register_value.whole_reg = 0x00001915;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(40);		//delay 40us
//
//	register_value.whole_reg = 0x0504C986;
//	dd_write_7021_reg(&register_value.byte[0]);
//        Delayus(40);		//delay 40us
//
//	register_value.whole_reg = 0x329668EA;
//	dd_write_7021_reg(&register_value.byte[0]);
}

void dd_set_RX_mode(void)
{
	ADF70XX_REG_T register_value;

	//	for ADF7021DB2 864M

	//write R1, turn on VCO
	register_value.whole_reg = 0x031BD011;
	dd_write_7021_reg(&register_value.byte[0]);
        
	register_value.whole_reg =0x00500882; //0x00680882;
	dd_write_7021_reg(&register_value.byte[0]);        

	//write R3, turn on TX/RX clocks
	register_value.whole_reg = 0x29915CD3;
	dd_write_7021_reg(&register_value.byte[0]);

	//write R6 here, if fine IF filter cal is wanted


//	//write R5 to start IF filter cal
//	register_value.whole_reg = 0x00001915;	//write R5 to start IF filter cal
//	dd_write_7021_reg(&register_value.byte[0]);
//	Delay100us(2);		//delay 0.2ms
//        
//	register_value.whole_reg = 0x0504C986;
//	dd_write_7021_reg(&register_value.byte[0]);  
//        
//	register_value.whole_reg = 0x000231E9;
//	dd_write_7021_reg(&register_value.byte[0]);
//
//	//write R11, configure sync word detect
//	register_value.whole_reg = 0x329668EA;
//	dd_write_7021_reg(&register_value.byte[0]);
//        
//	register_value.whole_reg = 0x0000003B;
//	dd_write_7021_reg(&register_value.byte[0]);   
//        
//	register_value.whole_reg = 0x0000010C;
//	dd_write_7021_reg(&register_value.byte[0]); 

	//write R0, turn on PLL
	register_value.whole_reg = 0x0954C7B0;    //CH=426.075MHz
	dd_write_7021_reg(&register_value.byte[0]);
	Delayus(40);		//delay 40us

	//write R4, turn on demodulation
	register_value.whole_reg = 0x00289A14;//0x00268614;
	dd_write_7021_reg(&register_value.byte[0]);

}
void dd_set_ADF7021_Freq(UINT8 Mode,UINT8 CH)
{
  ADF70XX_REG_T register_value;
    if(Mode==1)     //ADF7021 TX Mode
    {
    	switch (CH){
            case 1:
                    register_value.whole_reg = 0x0154DC30; //CH=426.075MHz
                    break;
            case 2:
                    register_value.whole_reg = 0x01575710;   //CH=429.175MHz
                    break;
            case 3:
                    register_value.whole_reg = 0x015759A0;  //CH=429.1875MHz
                    break;
            case 4:
                    register_value.whole_reg = 0x01575C30;  //CH=429.200MHz
                    break;
            case 5:
                    register_value.whole_reg = 0x01575EC0; //CH=429.2125MHz
                    break;
            case 6:
                    register_value.whole_reg = 0x01576140;//CH=429.225MHz
                    break;
//            case 7:
//                    register_value.whole_reg = 0x015763D0;//CH=429.2375MHz
//                    break;
            default:
                   break;
	}
        dd_write_7021_reg(&register_value.byte[0]);
        Delayus(40);		//delay 40us
        	//write R2, turn on PA
	register_value.whole_reg = 0x00564882;//0x00536882;//0x006B6882;			//Ramp Rate = 16 codes/bit, TX level = 2;
	dd_write_7021_reg(&register_value.byte[0]);
        Delayus(40);		//delay 40us
    }
    else        //ADF7021 RX Mode
    {
    	switch (CH){
            case 1:
//                    #if defined(__Product_PIC32MX2_Receiver__)
//                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
//                    #endif
//                    #if defined(__Product_PIC32MX2_WIFI__)
//                    if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))
//                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
//                    else   register_value.whole_reg = 0x09574290;//CH=429.175MHz
//                    #endif
                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
                    break;
            case 2:
                    register_value.whole_reg = 0x09574290;//CH=429.175MHz
                    break;
            case 3:
//                    #if defined(__Product_PIC32MX2_Receiver__)
//                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
//                    #endif
//                    #if defined(__Product_PIC32MX2_WIFI__)
//                    if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))
//                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
//                    else   register_value.whole_reg = 0x095747B0;//CH=429.200MHz
//                    #endif
                    //register_value.whole_reg = 0x09574520;//CH=429.1875MHz
                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
                    break;
            case 4:
                    register_value.whole_reg = 0x095747B0;//CH=429.200MHz
                    break;
            case 5:
//                    #if defined(__Product_PIC32MX2_Receiver__)
//                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
//                    #endif
//                    #if defined(__Product_PIC32MX2_WIFI__)
//                    if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))
//                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
//                    else   register_value.whole_reg = 0x09574CD0;//CH=429.225MHz
//                    #endif
                    //register_value.whole_reg = 0x09574A40;//CH=429.2125MHz
                    register_value.whole_reg = 0x0954C7B0; //CH=426.075MHz
                    break;
            case 6:
                    register_value.whole_reg = 0x09574CD0;//CH=429.225MHz
                    break;
//            case 7:
//                    register_value.whole_reg = 0x09574F60;//CH=429.2375MHz
//                    break;
            default:
                   break;
	}
        dd_write_7021_reg(&register_value.byte[0]);
        Delayus(40);		//delay 40us 
        	//write R4, turn on demodulation
	register_value.whole_reg = 0x00289A14;//0x00268614;
	dd_write_7021_reg(&register_value.byte[0]);
    }
}
void dd_set_ADF7021_Power_on(void)
{
    UINT8 i;
	if (ADF7021_CE == 0)
	{
		ADF7021_CE = 1;
		//phy_state = PHY_POWERON;
		//if ( is_use_crystal == TRUE ) 	dd_short_delay(25);	
                Delayus(1000);             //delay 1ms
	}
        Delayus(20000);             //delay 1ms
        m_RFNormalBuf[0]=0xFF;
        m_RFNormalBuf[1]=0xFF;
        for(i=2;i<=21;i++)m_RFNormalBuf[i]=0x55;
}

void dd_set_ADF7021_ReInitial(void)
{
    ADF7021_CE = 0;
    Delayus(20);
    ADF7021_CE = 1;
    Delayus(20);
    dd_set_RX_mode();
}

void dd_read_RSSI(void)
{
	ADF70XX_REG_T RSSI_value;
        UINT8 i,DATA_7021_byte[4];

	RSSI_value = dd_read_7021_reg(0x14);

	RSSI_value.whole_reg += RSSI_value.whole_reg ;
        for (i=0; i<=3; i++)
        {
           DATA_7021_byte[i]=RSSI_value.byte[i];
        }

    rssi = DATA_7021_byte[2];
	rssi += gain_correction[DATA_7021_byte[3] & 0x0F] ;
    rssi = rssi /4;//RSSI(dBm) = - rssi

}

void ADF7021_change_TXorRX(void)
{
    UINT8 i;
    UINT8 HA_j=0;
 #if defined(__Product_PIC32MX2_Receiver__)
//   if(((HA_L_signal!=FLAG_HA_L_signal)||(HA_ERR_signal!=FLAG_HA_ERR_signal))&&(FLAG_HA_START==0)){
//       if((Freq_Scanning_CH_save==0)||((Freq_Scanning_CH_save==1)&&(Control_bak&0x80==0x00))){
//           if(HA_L_signal!=FLAG_HA_L_signal)FLAG_HA_L_signal=HA_L_signal;
//           if(HA_ERR_signal!=FLAG_HA_ERR_signal)FLAG_HA_ERR_signal=HA_ERR_signal;
//           if(HA_L_signal==1)HA_j=HA_j|0x20;
//           if(HA_ERR_signal==1)HA_j=HA_j|0x10;
//           FLAG_HA_START=1;
//           ID_data.IDL=DATA_Packet_ID;
//           Control_code=HA_j;
//       }
//   }

   if((HA_L_signal==1)&&(HA_ERR_signal==1)){
       TIMER60s=6000;
       if((DATA_Packet_Control==0x08)&&(FLAG_open==0)&&(FLAG_APP_TX==0)){
           FLAG_open=1;
           FLAG_close=0;
           FLAG_HA_ERR=0;
           HA_Status=1;
           if(Freq_Scanning_CH_save==0){ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;}
       }
   }
   else if((TIMER60s==0)&&(HA_ERR_signal==1)){
       if((FLAG_close==0)&&(FLAG_APP_TX==0)){
           FLAG_close=1;
           FLAG_open=0;
           FLAG_HA_ERR=0;
           HA_Status=2;
           if(Freq_Scanning_CH_save==0){ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;}
       }
   }
   if((HA_ERR_signal==0)&&(HA_L_signal==1)&&(FLAG_APP_TX==0)){
       if(FLAG_HA_ERR==0){
           FLAG_HA_ERR=1;
           FLAG_close=0;
           FLAG_open=0;
           HA_Status=3;
           if(Freq_Scanning_CH_save==0){ID_data.IDL=DATA_Packet_ID;Control_code=0;FLAG_HA_START=1;}
       }
   }
   else FLAG_HA_ERR=0;

   if((FLAG_SendTxData==0)&&(FLAG_APP_TX==0)){
       FLAG_SendTxData=1;
       FLAG_APP_RX=1;
       Receiver_LED_OUT=0;
       Receiver_LED_TX=0;
       Receiver_LED_RX=0;
       ADF7021_DATA_IO=1;
       dd_set_RX_mode();
   }

//   if((HA_Sensor_signal==0)&&(FLAG_APP_SW2==0)){
//       FLAG_APP_SW2=1;
//       FLAG_APP_TX=0;
//       FLAG_APP_RX=1;
//       Receiver_LED_TX=0;
//       Receiver_LED_RX=0;
//       Receiver_LED_OUT=0;
//       ADF7021_DATA_IO=1;
//       dd_set_RX_mode();
//   }
//   if(HA_Sensor_signal==1)FLAG_APP_SW2=0;

//   if((ADF7021_MUXOUT==1)&&(FLAG_APP_RX==1)){
//       rssi=dd_read_rssi_7021_reg(0x14);
////       if(rssi>25);//TIMER300ms=200;//Receiver_LED_OUT=0;
////       else ;//Receiver_LED_OUT=1;
//   }
//   if((Receiver_Login==0)&&(FLAG_APP_SW3==0)){
//       FLAG_APP_SW3=1;
//       Frequency_CH++;
//       if(Frequency_CH>7)Frequency_CH=1;
//       if(FLAG_APP_TX==1)dd_set_ADF7021_Freq(1,Frequency_CH);
//        else dd_set_ADF7021_Freq(0,Frequency_CH);
//   }
//   if(Receiver_Login==1)FLAG_APP_SW3=0;

//   if(FLAG_APP_RX==1){
//       if(Receiver_Login==0)Receiver_LED_TX=1;
//   }
//       if(TIMER1s==0)Receiver_LED_OUT=0;           //测试，测试完后需要删除
//       if(TIMER300ms==0)Receiver_LED_RX=0;
#endif
   
#if defined(__Product_PIC32MX2_WIFI__)
//   if((WIFI_L_Login==0)&&(FLAG_APP_TX==0)&&(FLAG_APP_SW1==0)){
//       FLAG_APP_SW1=1;
//       //WIFI_Useless1=1;
//       SendTxData();
//   }
//   if(WIFI_L_Login==1)FLAG_APP_SW1=0;
   if((FLAG_UART_R==1)&&(FLAG_APP_TX==0)){
       FLAG_UART_R=0;
       //WIFI_Useless1=1;
       UART_Decode();
   }
   if((FLAG_SendTxData==0)&&(FLAG_APP_TX==0)){
       FLAG_SendTxData=1;
       FLAG_APP_RX=1;
       //WIFI_Useless1=1;
       WIFI_LED_TX=0;
       WIFI_LED_RX=0;
       ADF7021_DATA_IO=1;
       dd_set_RX_mode();
   }

   //if(((WIFI_Useless0==0)&&(FLAG_APP_SW2==0))||((FLAG_APP_RX==0)&&(FLAG_APP_RX==0))){
//   if((WIFI_Useless0==0)&&(FLAG_APP_SW2==0)){
//       FLAG_APP_SW2=1;
//       FLAG_APP_TX=0;
//       FLAG_APP_RX=1;
//       WIFI_Useless1=1;
//       WIFI_LED_RX=1;
//       ADF7021_DATA_IO=1;
//       dd_set_RX_mode();
//   }
//   if(WIFI_Useless0==1)FLAG_APP_SW2=0;

//   if((Receiver_Login==0)&&(FLAG_APP_SW3==0)){
//       FLAG_APP_SW3=1;
//       Frequency_CH++;
//       if(Frequency_CH>7)Frequency_CH=1;
//       if(FLAG_APP_TX==1)dd_set_ADF7021_Freq(1,Frequency_CH);
//        else dd_set_ADF7021_Freq(0,Frequency_CH);
//   }
//   if(Receiver_Login==1)FLAG_APP_SW3=0;

//   if(FLAG_APP_RX==1){
//       if(Receiver_Login==0)Receiver_LED_TX=1;
//   }
//       if(TIMER1s==0)WIFI_LED_RX=0;           //测试，测试完后需要删除
 #endif

   if((FLAG_UART_ok==1)||(FLAG_HA_START==1)){
       if(FLAG_rssi_Freq==0){
           rssi_TIME=1;    //发射时10ms间隔搜索空信道
           FLAG_rssi_Freq=1;
           rssi_COUNT=0;
           TX_Freq_CH=TX_Freq_CH+2;
           if(TX_Freq_CH>6)TX_Freq_CH=2;
           dd_set_ADF7021_Freq(0,TX_Freq_CH);
       }
       if(rssi_TIME==0){
           FLAG_rssi_Freq=0;
           if(rssi_COUNT>=10){FLAG_UART_ok=0;FLAG_HA_START=0;SendTxData();TX_Freq_CH=0;}
       }
//       TX_Freq_CH=1;
//       FLAG_UART_ok=0;FLAG_HA_START=0;SendTxData();TX_Freq_CH=0;
   }

    if((ADF7021_MUXOUT==1)&&(FLAG_APP_RX==1)){
       rssi=dd_read_rssi_7021_reg(0x14);
       if(rssi<=34){
           rssi_COUNT++;
           if(rssi_COUNT>10)rssi_COUNT=10;
       }
       else rssi_COUNT=0;
   }

}


