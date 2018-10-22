
/***********************************************************************/
/*  FILE        :ADF7021.c                                             */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// ����C����
#include "initial.h"		// ��ʼ��
#include "pcf8563.h"
void dd_set_ADF7021_ReInitial(void);

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
#if defined(__Product_PIC32MX2_WIFI__)
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
#endif
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
   UINT8 i;
   ADF70XX_REG_T register_value;

     for(i=0;i<5;i++){
        if(i==2)register_value.whole_reg =RF_SET_TX_REGISTER_0[TX_Freq_CH-1];
        else register_value.whole_reg =RF_SET_TX_DATA[i];
        dd_write_7021_reg(&register_value.byte[0]);

        if(i==0)Delayus(800);
        else  Delayus(40);
    }

}

void dd_set_ADF7021_Freq(UINT8 Mode,UINT8 CH)
{
    UINT8 i;
    ADF70XX_REG_T register_value;

    if(Mode==0)        //ADF7021 RX Mode
    {
        for(i=0;i<6;i++){
            if(i==3)register_value.whole_reg =RF_SET_RX_REGISTER_0[CH-1];
            else if(i==5)register_value.whole_reg =RF_SET_RX_REGISTER_A[CH-1];
            else  register_value.whole_reg =RF_SET_RX_DATA[i];
            dd_write_7021_reg(&register_value.byte[0]);

            if((i==3)||(i==5))Delayus(40);
        }
    }
}
void dd_set_ADF7021_Power_on(void)
{
    UINT8 i;
//	if (ADF7021_CE == 0)
//	{
//		ADF7021_CE = 1;
//		//phy_state = PHY_POWERON;
//		//if ( is_use_crystal == TRUE ) 	dd_short_delay(25);
//                Delayus(1000);             //delay 1ms
//	}
        ADF7021_CE = 0;
        Delay100us(200);             //delay 1ms
        ADF7021_CE = 1;
        Delay100us(200);             //delay 1ms
        m_RFNormalBuf[0]=0xFF;
        m_RFNormalBuf[1]=0xFF;
        for(i=2;i<=21;i++)m_RFNormalBuf[i]=0x55;
}

void dd_set_ADF7021_ReInitial(void)
{
    ADF7021_CE = 0;
    Delayus(200);
    ADF7021_CE = 1;
    Delayus(20);    
}

void dd_read_RSSI(void)
{

#if defined(__Product_PIC32MX2_WIFI__)
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
#endif
}

void ADF7021_change_TXorRX(void)
{
    UINT8 i,i_m,i_n;
    UINT8 HA_j=0;
    UINT8 xmv[10]={0};
    UINT8 Weekday_alarm;
    UINT16 Minutes_x;
 #if defined(__Product_PIC32MX2_Receiver__)
//   if((HA_L_signal==1)&&(HA_ERR_signal==1)){
//       TIMER60s=5;
//       if((DATA_Packet_Control==0x08)&&(FLAG_open==0)&&(FLAG_APP_TX==0)){
//           FLAG_open=1;
//           FLAG_close=0;
//           FLAG_HA_ERR=0;
//           HA_Status=0x81;
//           if(Freq_Scanning_CH_save_HA==0)FLAG_426MHz_Reply=1;//{ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;}
//           //ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;
//       }
//   }
//   else if((TIMER60s==0)&&(HA_ERR_signal==1)){
//       if((FLAG_close==0)&&(FLAG_APP_TX==0)){
//           FLAG_close=1;
//           FLAG_open=0;
//           FLAG_HA_ERR=0;
//           HA_Status=0x82;
//           if(Freq_Scanning_CH_save_HA==0)FLAG_426MHz_Reply=1;//{ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;}
//           //ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;
//       }
//   }
//   if((HA_ERR_signal==0)&&(HA_L_signal==1)&&(FLAG_APP_TX==0)){
//       if(FLAG_HA_ERR==0){
//           FLAG_HA_ERR=1;
//           FLAG_close=0;
//           FLAG_open=0;
//           HA_Status=0x83;
//           //FLAG_426MHz_Reply=1;    //��ң�ذ��APP�����£�EER�����
//           if(Freq_Scanning_CH_save_HA==0)FLAG_426MHz_Reply=1;//{ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;}
//           else FLAG_APP_Reply=1;
//           //ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;
//       }
//   }

    if(HA_L_signal==1){
       TIMER60s=200;//20;//200;//6000;
           FLAG_open=1;
           FLAG_close=0;
           FLAG_HA_ERR=0;
   }
   else if(TIMER60s==0){
           FLAG_close=1;
           FLAG_open=0;
           FLAG_HA_ERR=0;
   }
    if(HA_ERR_signal==1){
       TIMER_err_1s=120;//20;//120;
   }
   else if(TIMER_err_1s==0){
           FLAG_close=0;
           FLAG_open=0;
           FLAG_HA_ERR=1;
   }
    if(HA_Sensor_signal==1){
       TIMER_Sensor_open_1s=120;//20;//120;
       if(TIMER_Sensor_close_1s==0){
            FLAG_open_Sensor=0;
            FLAG_close_Sensor=1;
            FLAG_HA_ERR_Sensor=0;
       }
   }
    else {
        TIMER_Sensor_close_1s=120;
        if(TIMER_Sensor_open_1s==0){
            if(DATA_Packet_Control_err==0x08){FLAG_open_Sensor=1;FLAG_HA_ERR_Sensor=0;FLAG_close_Sensor=0;}
            if((DATA_Packet_Control_err==0x02)&&(FLAG_close_Sensor==1)){FLAG_open_Sensor=0;FLAG_HA_ERR_Sensor=1;FLAG_close_Sensor=0;}
//            FLAG_open_Sensor=1;FLAG_HA_ERR_Sensor=0;
//            FLAG_close_Sensor=0;
        }
    }
    if(((FLAG_HA_ERR==1)||(FLAG_HA_ERR_Sensor==1))&&(HA_Status!=0x83)&&(FLAG_HA_ERR_bit==0)){
           if(FLAG_HA_ERR==1)HA_Status=0x83;
           else HA_Status=0x84;
           FLAG_HA_ERR_bit=1;
           if(Freq_Scanning_CH_save_HA==0)FLAG_426MHz_Reply=1;
           else FLAG_APP_Reply=1;
    }
    if(((FLAG_open==1)&&(FLAG_open_Sensor==1))&&(HA_Status!=0x81)){
           HA_Status=0x81;
           if(Freq_Scanning_CH_save_HA==0)FLAG_426MHz_Reply=1;
           else FLAG_APP_Reply=1;     //ver2.12׷��
    }
    if(((FLAG_close==1)||(FLAG_close_Sensor==1))&&(HA_Status!=0x82)&&((FLAG_HA_ERR==0)||(FLAG_HA_ERR_Sensor==0))&&(FLAG_HA_ERR_bit==0)){
           HA_Status=0x82;
           if(Freq_Scanning_CH_save_HA==0)FLAG_426MHz_Reply=1;
           else FLAG_APP_Reply=1;
    }


   if((FLAG_SendTxData==0)&&(FLAG_APP_TX==0)){
       FLAG_SendTxData=1;
       FLAG_APP_RX=1;
       Receiver_LED_OUT=0;
       Receiver_LED_TX=0;
       Receiver_LED_RX=0;
       ADF7021_DATA_IO=1;
       //dd_set_RX_mode();
       dd_set_ADF7021_Freq(0,1);
   }
#endif
   
#if defined(__Product_PIC32MX2_WIFI__)
    if(FLAG_WIFI_LAN_SELECT!=WIFI_LAN_SELECT){TIME_WIFI_LAN_SELECT=10;FLAG_WIFI_LAN_SELECT=WIFI_LAN_SELECT;}
    else if(TIME_WIFI_LAN_SELECT==0){
        if(WIFI_LAN_SELECT==1){
            //WIFI_POWER=0;
            LATACLR=0x0400;
            LAN_POWER=1;
        }
        else {
            LAN_POWER=0;
            //WIFI_POWER=1;
            LATASET=0x0400;
        }
    }

   if((FLAG_UART_R==1)&&(FLAG_APP_TX==0)){
       FLAG_UART_R=0;
       UART_Decode();
   }
   if((FLAG_email_Repeat==1)&&(TIME_email_Repeat==0)){
       TIME_email_Repeat=9000;
       UART_send_count++;
       if(UART_send_count>3)FLAG_email_Repeat=0;
       HA_uart_email_Repeat();
   }
   if((FLAG_SendTxData==0)&&(FLAG_APP_TX==0)){
       FLAG_SendTxData=1;
       FLAG_APP_RX=1;
       WIFI_LED_TX=0;
       WIFI_LED_RX=0;
       ADF7021_DATA_IO=1;
       //dd_set_RX_mode();
       dd_set_ADF7021_Freq(0,1);
   }

    if(PCF8563_INT==0){
        Read_Time(&xmv[0]);
//        #if defined(__32MX250F128D__)
//        APP_OUT_TEST1(&xmv[0]);
//        #endif
        //Minutes_x=xmv[2]*60+xmv[1];
        Minutes_x=Hex_Decimal(xmv[2],xmv[1]);             //2014.10.11�޸�   ���TIMER��ʱ������
        if(SUN_Weekdays_alarm!=xmv[5]){
            SUN_time_get(SUN_ON_OFF_seat[2]);
            if((WIFI_alarm_Hours_Minutes[0]==xmv[2])&&(WIFI_alarm_Hours_Minutes[1]==xmv[1]));
            else NEW_set_alarm_pcf8563(Minutes_x);
        }
        if((WIFI_alarm_Hours_Minutes[0]==xmv[2])&&(WIFI_alarm_Hours_Minutes[1]==xmv[1])){
            for(i=0;i<22;i++){
                if(i<12){
                            if(WIFI_alarm_data[i][1]==0x01){
                                   Weekday_alarm=0x01;
                                   Weekday_alarm=Weekday_alarm<<xmv[4];
                                   if(((WIFI_alarm_data[i][5]&Weekday_alarm)==Weekday_alarm)&&(xmv[2]==WIFI_alarm_data[i][3])&&(xmv[1]==WIFI_alarm_data[i][4])){
                                       for(i_m=0;i_m<WIFI_alarm_data[i][6];i_m++)alarm_OUT_to_AUTO(i,i_m);
                                   }
                            }
                }
                else {
                            i_n=i-12;
                            if(Emial_time_data[i_n][1]==0x01){
                                   Weekday_alarm=0x01;
                                   Weekday_alarm=Weekday_alarm<<xmv[4];
                                   if(((Emial_time_data[i_n][4]&Weekday_alarm)==Weekday_alarm)&&(xmv[2]==Emial_time_data[i_n][2])&&(xmv[1]==Emial_time_data[i_n][3])){
                                       for(i_m=0;i_m<ID_DATA_PCS;i_m++)Emial_time_OUT(i_m);
                                       FLAG_Emial_time=1;
                                       Emial_time_place=i_n;
                                   }
                            }
                }
            }
            NEW_set_alarm_pcf8563(Minutes_x);
//            #if defined(__32MX250F128D__)
//            APP_OUT_TEST2(&WIFI_alarm_Hours_Minutes[0]);
//            #endif
        }
//        else {
//           xmv[0]=2;
//           Write_pcf8563(&xmv[0],0x01,1);
//        }
        else NEW_set_alarm_pcf8563(Minutes_x);
    }
   
//    if(PCF8563_INT==0){
//        Read_Time(&xmv[0]);
//        Minutes_x=xmv[2]*60+xmv[1];
//        if((WIFI_alarm_Hours_Minutes[0]==xmv[2])&&(WIFI_alarm_Hours_Minutes[1]==xmv[1])){
//            AUTO_SEND_DATA_pcs=0;
//            for(i=0;i<WIFI_alarm_data_PCS;i++){
//                if(WIFI_alarm_data[i][4]==0x10){
//                    if((xmv[6]==WIFI_alarm_data[i][5])&&(xmv[5]==WIFI_alarm_data[i][6])&&(xmv[3]==WIFI_alarm_data[i][7])
//                            &&(xmv[2]==WIFI_alarm_data[i][8])&&(xmv[1]==WIFI_alarm_data[i][9]))alarm_OUT_bak(i);
//                }
//                else if(WIFI_alarm_data[i][4]==0x20){
//                    Weekday_alarm=0x01;
//                    Weekday_alarm=Weekday_alarm<<xmv[4];
//                   if(((WIFI_alarm_data[i][7]&Weekday_alarm)==Weekday_alarm)&&(xmv[2]==WIFI_alarm_data[i][8])&&(xmv[1]==WIFI_alarm_data[i][9]))alarm_OUT_bak(i);
//                }
//            }
//            NEW_set_alarm_pcf8563(Minutes_x);
//        }
//        else {
//           xmv[0]=2;
//           Write_pcf8563(&xmv[0],0x01,1);
//        }
//    }

    if(FLAG_AUTO_SEND_START==1){
        if((TIME_alarm_AUTO==0)&&(FLAG_AUTO_SEND_ok==0)){
            ID_data.IDB[0]=AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0];
            ID_data.IDB[1]=AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1];
            ID_data.IDB[2]=AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2];
            ID_data.IDB[3]=0x00;
            Control_code=AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][3];
            if(ID_data.IDL!=0){                   //���ID=0�Ļ�����������
                TIME_alarm_AUTO=350;                  //2014.10.11�޸�  250
                FLAG_HA_Inquiry=1;
                DATA_Packet_Control_0=0x00;    //��ʾAPP��ѯ
                FLAG_AUTO_SEND_ok=1;
            }
            AUTO_SEND_DATA_pcs--;
            if(AUTO_SEND_DATA_pcs==0)FLAG_AUTO_SEND_START=0;
        }
    }
 #endif

   if((FLAG_UART_ok==1)||(FLAG_HA_START==1)||(FLAG_AUTO_SEND_ok==1)){
       if(FLAG_rssi_Freq==0){
           rssi_TIME=1;    //����ʱ10ms����������ŵ�
           FLAG_rssi_Freq=1;
           rssi_COUNT=0;
           TX_Freq_CH=TX_Freq_CH+2;
           if(TX_Freq_CH>6)TX_Freq_CH=2;
           dd_set_ADF7021_Freq(0,TX_Freq_CH);
       }
       if(rssi_TIME==0){
           FLAG_rssi_Freq=0;
           if(rssi_COUNT>=10){FLAG_UART_ok=0;FLAG_HA_START=0;FLAG_AUTO_SEND_ok=0;SendTxData();TX_Freq_CH=0;
                              #if defined(__Product_PIC32MX2_WIFI__)
                              TIME_No_response=300;FLAG_TIME_No_response=1;      //2014.10.11�޸�   150
                              #endif
           }
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


