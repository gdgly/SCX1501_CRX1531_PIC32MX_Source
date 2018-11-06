
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
#include "Uart.h"
#include "pcf8563.h"
/******************无线DATA 数据说明******************************
 通信机429MHz发送DATA
    类型	        HEMS追加	      HEMS追加	    HEMS追加			   HEMS追加
   byte7          byte6           byte5           byte4    byte3   byte2  byte1    byte0
0：工作控制	   角度调整（关）    角度调整（开）	  半开	  OPEN	  STOP	CLOSE	 换气
1	        0 ：设置	      半开信号T秒设置，范围：0～60秒
1		0                0x3F  ：T秒查询


通信机429MHz接收DATA
   byte7          byte6           byte5          byte4    byte3   byte2  byte1    byte0
    1 :HA和DIP开关     DIP开关（3 BIT）                        HA状态（4 BIT）
    0              0：T秒返回值
***************************************************************/

void ID_Decode_function(void);
void  Freq_Scanning(void);
void ID_code_function(void);
void SetTxData(void);
UINT16 SetFixedLengthCode(UINT8 data );
void SendTxData(void);
void ID_Decode_OUT(void);
void Signal_DATA_Decode(UINT8 NUM_Type);
void BEEP_and_LED(void);

 #if defined(__Product_PIC32MX2_WIFI__)
void SWITCH_DIP_check_app(void);
void Email_check_app(void);
void Email_check_mail(void);
void HA_Cache_ha_switch(UINT32 ID_IN);
 #endif

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
    INTCONbits.INT2EP = 0; // clear the bit for falling edge trigger
    IPC2bits.INT2IP= 6; // Set priority level=6
    IPC2bits.INT2IS = 3; // Set Subpriority level=3
    IFS0bits.INT2IF = 0; // clear the interrupt flag
    IEC0bits.INT2IE = 1; // enable INT3
}

void __ISR(_EXTERNAL_2_VECTOR, ipl6) ExternalHandler(void)
{
    asm ("di");
 #if defined(__Product_PIC32MX2_Receiver__)
    if(FLAG_APP_RX==1)
        ID_Decode_function();
    else if(FLAG_APP_TX==1)ID_code_function();
    else if(FG_test_rx==1){
      X_COUNT ++;
      if((ADF7021_DATA_rx== X_HIS)&&(X_COUNT != 1))X_ERR++;
      X_HIS = ADF7021_DATA_rx;
  }
 #endif
 #if defined(__Product_PIC32MX2_WIFI__)
    if(FLAG_APP_RX==1)
        ID_Decode_function();
    else if(FLAG_APP_TX==1)ID_code_function();
    else if(FG_test_rx==1){
      X_COUNT ++;
      if((ADF7021_DATA_rx== X_HIS)&&(X_COUNT != 1))X_ERR++;
      X_HIS = ADF7021_DATA_rx;
  }
 #endif
    //ADF7021_CLKOUT=!ADF7021_CLKOUT;//测试，测试完后需要删除
    IFS0bits.INT2IF = 0;
    asm ("ei");
}

void ID_code_function(void)
{
    //UINT8 ID_INT_VALUE;
    if(txphase%8==0)ID_INT_CODE=m_RFNormalBuf[txphase/8];
//    ID_INT_VALUE=ID_INT_CODE>>7;
//    if(ID_INT_VALUE)ADF7021_DATA_tx=1;
//      else ADF7021_DATA_tx=0;
    if	( (ID_INT_CODE & 0b10000000)==0b10000000 )LATASET=0x0200;//ADF7021_DATA_tx=1;
    else LATACLR=0x0200;//ADF7021_DATA_tx=0;
//   ADF7021_DATA_tx=!ADF7021_DATA_tx;
    ID_INT_CODE<<=1;
    txphase++;
    if(txphase>=280){
        txphase=0;
        txphase_Repeat++;
        if(txphase_Repeat>=4){FLAG_APP_TX=0; FLAG_SendTxData=0;}
    }
}

void ID_Decode_function(void)
{
    UINT16 DATA_Packet_Syn_bak=0;
     TIME_EMC=10;
     switch (rxphase){
        case 0:
                DATA_Packet_Syn=DATA_Packet_Syn<<1;
                if(ADF7021_DATA_rx)DATA_Packet_Syn+=1;
                if(TIMER18ms==0){
                                 DATA_Packet_Syn_bak=DATA_Packet_Syn&0x0000FFFF;
                                 if((DATA_Packet_Syn_bak==0x5555)||(DATA_Packet_Syn_bak==0xAAAA));
                                 else FLAG_Receiver_Scanning=1;
                                }
                //if(DATA_Packet_Syn==0x55555555){rxphase=1;TIMER18ms=65;DATA_Packet_Syn=0;DATA_Packet_Head=0;}
 #if defined(__Product_PIC32MX2_WIFI__)
                if((DATA_Packet_Syn&0xFFFFFFFF)==0x55555555){rxphase=1;TIMER18ms=2000;DATA_Packet_Syn=0;DATA_Packet_Head=0;}
 #endif
 #if defined(__Product_PIC32MX2_Receiver__)
                if((DATA_Packet_Syn&0xFFFFFFFF)==0x55555555){rxphase=1;TIMER18ms=800;DATA_Packet_Syn=0;DATA_Packet_Head=0;
                                                             Receiver_LED_RX=FLAG_PORT_LEDoutput_allow;
                                                             TIMER300ms=500; //if(TIMER300ms==0)TIMER300ms=100;
                }
 #endif                                                            
                break;
	case 1:
                DATA_Packet_Head=DATA_Packet_Head<<1;
                if(ADF7021_DATA_rx)DATA_Packet_Head+=1;
                //DATA_Packet_Head=DATA_Packet_Head&0x0000FFFF;
                if(TIMER18ms==0)rxphase=0;
                if(DATA_Packet_Head==0x5515){rxphase=2;DATA_Packet_Syn=0;DATA_Packet_Head=0;DATA_Packet_Code_i=0;}
		break;
        case 2:
                DATA_Packet_Code_g=DATA_Packet_Code_i/32;
                DATA_Packet_Code[DATA_Packet_Code_g]=DATA_Packet_Code[DATA_Packet_Code_g]<<1;
                if(ADF7021_DATA_rx)DATA_Packet_Code[DATA_Packet_Code_g]+=1;
                DATA_Packet_Code_i++;
                if(DATA_Packet_Code_i==96){
                    if((DATA_Packet_Code[1]&0x0000FFFF)==0x5556);
                    else rxphase=3;
                }
                else if(DATA_Packet_Code_i>=192)rxphase=3;
                break;
        case 3:
                FLAG_Receiver_IDCheck=1;
                if((Freq_Scanning_CH==1)||(Freq_Scanning_CH==3)||(Freq_Scanning_CH==5))Freq_Scanning_CH_bak=0;   //暂时记录下收到信号的频率信道,0代表426M
                else Freq_Scanning_CH_bak=1;       //                       1代表429M
                rxphase=0;
                DATA_Packet_Syn=0;
                TIMER18ms=0;   //0ms，接受可靠稳定
                break;
        default:
               break;
	}
}
void ID_Decode_IDCheck(void)
{
   UINT8 i,value_num;
    if(FLAG_Receiver_IDCheck)
    {
        FLAG_Receiver_IDCheck=0;
        Signal_DATA_Decode(0);
        if(FLAG_Signal_DATA_OK==1)
        {
            eeprom_IDcheck();
            #if defined(__Product_PIC32MX2_Receiver__)
            if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1)){  
            #endif
            #if defined(__Product_PIC32MX2_WIFI__)
            if(((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))&&(TIME_ID_Login_delay==0)){   //20150430 japan修改1
            #endif
             #if defined(__Product_PIC32MX2_WIFI__)
                if((FLAG_ID_Login_OK==0)&&(DATA_Packet_Control_buf!=0x40)&&(DATA_Packet_ID_buf!=0)&&(DATA_Packet_Control_buf==0x04)){            //2015.4.1修正1 在登录模式下 不允许自动送信登录，只允许手动送信登录

                       if(ID_DATA_PCS>=32){
                           eeprom_IDcheck_0x00();
                           if(FLAG_IDCheck_OK_0x00==0)FLAG_ID_Login_OK=0;
                           else {FLAG_ID_Login_OK=1;ID_Receiver_Login=DATA_Packet_ID_buf;}
                       }
                       else {FLAG_ID_Login_OK=1;ID_Receiver_Login=DATA_Packet_ID_buf;}
                }
            #endif
            #if defined(__Product_PIC32MX2_Receiver__)
               if((FLAG_ID_Login_OK==0)&&(DATA_Packet_Control_buf!=0x40)&&(DATA_Packet_ID_buf!=0)&&(Freq_Scanning_CH_bak==0)){     //2015.4.1修正1 在登录模式下 不允许自动送信登录，只允许手动送信登录
                    FLAG_ID_Login_OK=1;
                    ID_Receiver_Login=DATA_Packet_ID_buf;
                   
               }
            #endif
            }
            else if((FLAG_IDCheck_OK==1)||(DATA_Packet_ID_buf==0xFFFFFE))
            {
                FLAG_IDCheck_OK=0;
                if(Freq_Scanning_CH_bak==0){Freq_Scanning_CH_save=1;Freq_Scanning_CH_save_HA=0; }  //当前收到426M控制   但保存记录下收到信号的频率信道,0代表426M
                else Freq_Scanning_CH_save_HA=1;  //                       1代表429M
              #if defined(__Product_PIC32MX2_Receiver__)
                 if(DATA_Packet_ID_buf==0xFFFFFE)DATA_Packet_Control=DATA_Packet_Control_buf;
                 DATA_Packet_Control_0=DATA_Packet_Control;
              #endif
              #if defined(__Product_PIC32MX2_WIFI__)
                //DATA_Packet_Control_0=DATA_Packet_Control;
                if(DATA_Packet_Control>=0x80){
                    SWITCH_DIP=(DATA_Packet_Control&0x70)>>4;
                    DATA_Packet_Control_0=DATA_Packet_Control&0x8F;      //以下2015.08.21追加
//                    value_num=DATA_Packet_Control&0x8F;
//                    if(value_num==0x80){DATA_Packet_soft_ver=1;DATA_Packet_Control_0=0x88;}
//                    else if(value_num>0x88){DATA_Packet_soft_ver=1;DATA_Packet_Control_0=value_num-8;}
//                    else {DATA_Packet_soft_ver=0;DATA_Packet_Control_0=value_num;}
                    DATA_Packet_Control=DATA_Packet_Control_0;
                }
             #endif
             #if defined(__Product_PIC32MX2_Receiver__)
                if(DATA_Packet_Control==0x08)DATA_Packet_Control_err=0x08;
                else if(((DATA_Packet_Control&0xDF)>0x80)&&((DATA_Packet_Control&0x20)==0x00))DATA_Packet_Control_err=DATA_Packet_Control;
                if(DATA_Packet_Control==0x02){DATA_Packet_Control_err=0x02;FLAG_HA_ERR_bit=0;}
                else if(DATA_Packet_Control==0x01){DATA_Packet_Control_err=0x01;FLAG_HA_ERR_bit=0;}   //20150425追加
                else if (((DATA_Packet_Control&0xDF)>0x80)&&((DATA_Packet_Control&0x40)==0x40))FLAG_HA_ERR_bit=0;  //2015.12.29追加，在半开、半闭动作中，受信机的状态变成异常1的时候，让停止继电器不动作
             #endif
                if(((DATA_Packet_Code[1]&0x0000FFFF)==0x5556)&&(Freq_Scanning_CH_bak==0)){
                    Signal_DATA_Decode(1);
                    if(FLAG_Signal_DATA_OK==1){
                            eeprom_IDcheck();
                            if(DATA_Packet_Control_buf==0xFF){
                                if(FLAG_IDCheck_OK==1)FLAG_IDCheck_OK=0;
                                 else if(ID_DATA_PCS<256){
                                    #if defined(__Product_PIC32MX2_Receiver__)
                                     BEEP_and_LED();         //2014.10.11修改
                                     ID_Receiver_Login=DATA_Packet_ID_buf;
                                     ID_EEPROM_write();
                                     #endif
                                 }//end else
                            }
                            else if(DATA_Packet_Control_buf==0x00){
                                if(FLAG_IDCheck_OK==1){
                                     FLAG_IDCheck_OK=0;
                                     BEEP_and_LED();
                                     ID_EEPROM_write_0x00();
                                 }
                            }
                    }
                }
                else
                   {
#if defined(__Product_PIC32MX2_WIFI__)
                    TIMER1s=500;//1000
                    FLAG_TIME_No_response=0;
#endif
#if defined(__Product_PIC32MX2_Receiver__)
                    if(Freq_Scanning_CH_bak==0){
                       if((DATA_Packet_Control==0x40)&&(Manual_override_TIMER==0)){
                          FG_auto_manual_mode=1;
                          TIME_auto_out=890;    // 900
                          if(FG_First_auto==0){
                              FG_First_auto=1;
                              TIMER1s=3000;    //2500
                          }
                        }
                        else if(DATA_Packet_Control==0x40);
			else{
			   FG_auto_out=0;
			   TIME_auto_close=0;
                           FG_auto_open_time=0;
                           if(FG_auto_manual_mode==1)//Manual_override_TIMER=13500;   //2分30秒自动无效
                              Manual_override_TIMER=24480;   //4分30秒自动无效
		           if((DATA_Packet_Control&0x14)==0x14){if(TIMER1s==0)TIMER1s=3800-30;}
			   else  TIMER1s=1000;
			}

                    }
                    else {
                        if(((DATA_Packet_Control&0xA0)==0x20)||((DATA_Packet_Control&0xC0)==0x40))TIMER1s=500;
                        else if(((DATA_Packet_Control&0xDF)>0x80)&&((DATA_Packet_Control&0x20)==0x00)){
                            TIMER1s=1000;//(TIMER_Semi_open+1)*1000;
                            if((DATA_Packet_Control&0xDF)<0xC0)TIMER_Semi_open=(DATA_Packet_Control&0x1F)+4;
                            else TIMER_Semi_close=(DATA_Packet_Control&0x1F)+4;
                        }
                        else  TIMER1s=1000;
                    }
                    TIMER300ms=500;
                    Receiver_LED_RX=FLAG_PORT_LEDoutput_allow;
#endif
                   }
            }
        }   
    }
}

void Signal_DATA_Decode(UINT8 NUM_Type)
{
    UINT32 data_in;
    UINT16 data_out;
    UINT16 data_NRZ[3];
    UINT8 i,j;
        for(i=0;i<3;i++)
        {
            if(NUM_Type==0)data_in=DATA_Packet_Code[i];
            else data_in=DATA_Packet_Code[i+3];
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
      if(data_NRZ[2]==((data_NRZ[0]+data_NRZ[1])&0xFFFF)){
          FLAG_Signal_DATA_OK=1;
          DATA_Packet_ID_buf=(data_NRZ[1]&0x00FF)*65536+data_NRZ[0];
          if(DATA_Packet_ID_buf==0)FLAG_Signal_DATA_OK=0;    //2014.3.21追加  不允许使用ID=0
          DATA_Packet_Control_buf=(data_NRZ[1]&0xFF00)>>8;
//          if(Freq_Scanning_CH_bak==1)
//              Control_bak=DATA_Packet_Control;
      }
      else FLAG_Signal_DATA_OK=0;
}
void BEEP_and_LED(void)
{
   UINT16 i;
 #if defined(__Product_PIC32MX2_Receiver__)
     Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
     for(i=0;i<4160;i++){
         Receiver_Buzzer=!Receiver_Buzzer;   //蜂鸣器频率2.08KHZ
         //Delayus(190);     //特别说明：该行采用XC32的0级优化，即无优化
         //Delayus(240);//特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
         Delayus(164);//窗式卷帘用无线模块  蜂鸣器频率2.73KHZ  特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
         ClearWDT(); // Service the WDT
     }
     Receiver_Buzzer=0;
     //Receiver_LED_OUT=FLAG_PORT_LEDoutput_NOallow;   //2015.3.23修改
     TIME_Receiver_LED_OUT=185;   //2015.3.23修改
#endif
#if defined(__Product_PIC32MX2_WIFI__)
     WIFI_LED_RX=1;
     for(i=0;i<12000;i++){
         Delayus(190);       //2.08KHZ
         ClearWDT(); // Service the WDT
     }
     WIFI_LED_RX=0;
#endif
}

#if defined(__Product_PIC32MX2_WIFI__)
void Confirm_BEEP_and_LED(void)
{
   UINT16 i,j;

     WIFI_LED_RX=1;
     for(i=0;i<30;i++){
         for(j=0;j<100;j++){
             Delayus(190);       //2.08KHZ
             ClearWDT(); // Service the WDT
         }
         WIFI_LED_RX=!WIFI_LED_RX;         
     }
     WIFI_LED_RX=0;
}
#endif

void Receiver_BEEP(void)
{
#if defined(__Product_PIC32MX2_Receiver__)
   UINT16 i,j;
   if(FLAG_Receiver_BEEP==0)
   {
       FLAG_Receiver_BEEP=1;
       for(j=0;j<3;j++){
         for(i=0;i<1800;i++){
             Receiver_Buzzer=!Receiver_Buzzer;   //蜂鸣器频率2.08KHZ
             //Delayus(190);     //特别说明：该行采用XC32的0级优化，即无优化
             Delayus(240);//特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
             ClearWDT(); // Service the WDT
         }
         for(i=0;i<1800;i++){
             Receiver_Buzzer=0;   //蜂鸣器频率2.08KHZ
             //Delayus(190);     //特别说明：该行采用XC32的0级优化，即无优化
             Delayus(240);//特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
             ClearWDT(); // Service the WDT
         }
       }
       Receiver_Buzzer=0;
   }
#endif
}


void ID_Decode_OUT(void)
{
    UINT8 Control_i,data0,data_sum,data_xm[2];
    UINT16 i_xm;
 #if defined(__Product_PIC32MX2_Receiver__)
//    if(Freq_Scanning_CH_bak==0) Control_i=DATA_Packet_Control&0xFF;
//    else Control_i=DATA_Packet_Control&0x0E;
//    if(HA_Sensor_signal==1)Receiver_LED_TX=FLAG_PORT_LEDoutput_NOallow;                      //test 接近信号回路
//     else Receiver_LED_TX=FLAG_PORT_LEDoutput_allow;
    if(time_Login_exit_256!=0)return;
    Control_i=DATA_Packet_Control&0xFF;
    if(TIMER1s){
                switch (Control_i){
                     case 0x14:                     //stop+login
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                TIMER250ms_STOP=250;
                                Receiver_OUT_STOP=1;
                                Receiver_OUT_VENT=0;
                                if(TIMER1s<3550){//Receiver_OUT_OPEN=1;
                                                 LATASET=0x0002;
                                                 Receiver_OUT_CLOSE=1;Receiver_BEEP();}
                                break;
                     case 0x02:                        //close
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                //Receiver_OUT_OPEN=0;
                                LATACLR=0x0002;
                                Receiver_OUT_VENT=0;
                                Receiver_OUT_STOP=0;
                                Receiver_OUT_CLOSE=1;
                                break;
                     case 0x04:                      //stop
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                //Receiver_OUT_OPEN=0;
                                LATACLR=0x0002;
                                Receiver_OUT_VENT=0;
                                Receiver_OUT_CLOSE=0;
                                Receiver_OUT_STOP=1;
                                break;
                     case 0x08:                      //open
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                Receiver_OUT_STOP=0;
                                Receiver_OUT_CLOSE=0;
                                Receiver_OUT_VENT=0;
                                //Receiver_OUT_OPEN=1;
                                LATASET=0x0002;
                                break;
                    case 0x09:                       //vent+OPEN
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                Receiver_OUT_STOP=0;
				Receiver_OUT_CLOSE=0;
                                //Receiver_OUT_OPEN=1;
                                LATASET=0x0002;
                                Receiver_OUT_VENT=1;
                                break;
                     case 0x03:                       //vent+close
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                Receiver_OUT_STOP=0;
				//Receiver_OUT_OPEN=0;
                                LATACLR=0x0002;
                                Receiver_OUT_CLOSE=1;
                                Receiver_OUT_VENT=1;
                                break;
                     case 0x0C:                    //open+stop
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                TIMER250ms_STOP=250;
                                Receiver_OUT_CLOSE=0;
                                Receiver_OUT_VENT=0;
                                Receiver_OUT_STOP=1;
				if(FG_OUT_OPEN_CLOSE==0){FG_OUT_OPEN_CLOSE=1;TIME_OUT_OPEN_CLOSE=25;}          //2015.3.23修改
                                if(TIME_OUT_OPEN_CLOSE==0)LATASET=0x0002;  //Receiver_OUT_OPEN=1;
                                break;
                     case 0x06:                  //close+stop
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                TIMER250ms_STOP=250;
                                //Receiver_OUT_OPEN=0;
                                LATACLR=0x0002;
                                Receiver_OUT_VENT=0;
                                Receiver_OUT_STOP=1;
				if(FG_OUT_OPEN_CLOSE==0){FG_OUT_OPEN_CLOSE=1;TIME_OUT_OPEN_CLOSE=25;}       //2015.3.23修改
                                if(TIME_OUT_OPEN_CLOSE==0)Receiver_OUT_CLOSE=1;
                                break;
                     case 0x01:                  //VENT
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                if(Freq_Scanning_CH_bak==1){             //429M   换气
                                    //Receiver_OUT_OPEN=1;
                                    LATASET=0x0002;
                                    Receiver_OUT_STOP=0;
                                    Receiver_OUT_CLOSE=1;
                                }
                                else {                   //426M  换气
                                    Receiver_OUT_VENT=1;
                                    //Receiver_OUT_OPEN=0;
                                    LATACLR=0x0002;
                                    Receiver_OUT_STOP=0;
                                    Receiver_OUT_CLOSE=0;
                                }
                                break;
                     case 0x20:
                                if(Freq_Scanning_CH_bak==1){          //429M  角度调整（开）
                                        Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                        //Receiver_OUT_OPEN=1;
                                        LATASET=0x0002;
                                        Receiver_OUT_STOP=0;
                                        Receiver_OUT_CLOSE=0;
                                }
                                break;
                     case 0x0A:                       //close+OPEN
                                Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                Receiver_OUT_STOP=0;
				Receiver_OUT_VENT=0;
                                //Receiver_OUT_OPEN=1;
                                LATASET=0x0002;
                                Receiver_OUT_CLOSE=1;
                                break;
                     case 0x40:
                                if(Freq_Scanning_CH_bak==1){          //429M  角度调整（关）
                                        Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                        //Receiver_OUT_OPEN=0;
                                        LATACLR=0x0002;
                                        Receiver_OUT_STOP=0;
                                        Receiver_OUT_CLOSE=1;
                                }
                                else{
                                    if((FG_auto_out==0)&&(Manual_override_TIMER==0)){                  //自动送信
                                        Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                        TIMER250ms_STOP=0;
                                        Receiver_OUT_VENT=0;
                                        Receiver_OUT_CLOSE=0;
                                        if(TIMER1s>2000){Receiver_OUT_STOP=1;LATACLR=0x0002;}
                                        else if(TIMER1s>1000){Receiver_OUT_STOP=0;LATACLR=0x0002;}
                                        else {FG_auto_open_time=1;Receiver_OUT_STOP=0;LATASET=0x0002;}
                                    }
                                }
                                break;
//                     case 0x10:
//                                if(Freq_Scanning_CH_bak==1){             //429M   半开信号
//                                    Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
//                                    Receiver_OUT_STOP=0;
//                                    Receiver_OUT_CLOSE=0;
//                                    //Receiver_OUT_OPEN=1;
//                                    LATASET=0x0002;
//                                    TIMER250ms_STOP=(TIMER_Semi_open+1)*1000;
//                                }
//                                break;
                     default:
                                break;
                     }
                if(Freq_Scanning_CH_bak==1){             //429M   半开信号/半闭
                    if(((DATA_Packet_Control&0xDF)>0x80)&&((DATA_Packet_Control&0x20)==0x00)){
                                if((DATA_Packet_Control&0xDF)<0xC0){
                                    FLAG__Semi_open_T=1;
                                    FLAG__Semi_close_T=0;
                                    Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                    Receiver_OUT_STOP=0;
                                    Receiver_OUT_CLOSE=0;
                                    //Receiver_OUT_OPEN=1;
                                    LATASET=0x0002;
                                    TIMER250ms_STOP=((TIMER_Semi_open+1)*1000/107)*100;
                                }
                                else {
                                    FLAG__Semi_open_T=0;
                                    FLAG__Semi_close_T=1;
                                    Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
                                    Receiver_OUT_STOP=0;
                                    Receiver_OUT_CLOSE=1;
                                    //Receiver_OUT_OPEN=0;
                                    LATACLR=0x0002;
                                    TIMER250ms_STOP=((TIMER_Semi_close+1)*1000/107)*100;
                                }
                      }
                }
                if(((DATA_Packet_Control==0x00)||(DATA_Packet_Control==0x02)||(DATA_Packet_Control==0x04)||(DATA_Packet_Control==0x08)||(DATA_Packet_Control==0x01)
                     ||(DATA_Packet_Control==0x20)||(DATA_Packet_Control==0x40)||((FLAG__Semi_open_T==1)||(FLAG__Semi_close_T==1)))&&(FLAG_APP_Reply==0)&&(Freq_Scanning_CH_save_HA==1))
                     FLAG_APP_Reply=1;
                if((FLAG__Semi_open_T==1)||(FLAG__Semi_close_T==1)){
                    if((DATA_Packet_Control==0x02)||(DATA_Packet_Control==0x04)||(DATA_Packet_Control==0x08)||(DATA_Packet_Control==0x01)||(DATA_Packet_Control==0x20)||(DATA_Packet_Control==0x40)
                     ||(DATA_Packet_Control==0x9)||(DATA_Packet_Control==0x03)||(DATA_Packet_Control==0x0C)||(DATA_Packet_Control==0x06)||(DATA_Packet_Control==0x0A)){
                        //2015.12.29追加，在半开、半闭动作中，送信机（开+闭）信号，让停止继电器不动作
                        FLAG__Semi_open_T=0;FLAG__Semi_close_T=0;TIMER250ms_STOP=0;
                    }
                }


               }       
     else {
               if((FG_auto_out==1)&&(TIME_auto_out==0)){FG_auto_out=0;TIME_auto_close=270;Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;}   //300
               if(TIME_auto_close){
                    if(TIME_auto_close>180){Receiver_OUT_STOP=1;Receiver_OUT_CLOSE=0;}  //200
                    else if(TIME_auto_close>90){Receiver_OUT_STOP=0;Receiver_OUT_CLOSE=0;}   //100
                    else {Receiver_OUT_STOP=0;Receiver_OUT_CLOSE=1;}	     
               }
               else   {FG_auto_manual_mode=0;Receiver_OUT_CLOSE=0;}
               FG_First_auto=0;
               LATACLR=0x0002;
               Receiver_OUT_VENT=0;
               if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1)||(TIME_auto_close));
               else  if(TIME_Receiver_LED_OUT>0)Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;    //2015.3.23修改
	       else Receiver_LED_OUT=FLAG_PORT_LEDoutput_NOallow;
               if(FG_auto_open_time==1){FG_First_auto=0;FG_auto_out=1;FG_auto_open_time=0;}
               if((FLAG__Semi_open_T==1)||(FLAG__Semi_close_T==1)){
                   if(HA_Status==0x83)TIMER250ms_STOP=0;     //2015.12.29追加，在半开、半闭动作中，受信机的状态变成异常1的时候，让停止继电器不动作
                   if((TIMER250ms_STOP<1000)&&(TIMER250ms_STOP>0)){Receiver_OUT_STOP=1;Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;}
                   else if(TIMER250ms_STOP==0){Receiver_OUT_STOP=0;FLAG__Semi_open_T=0;FLAG__Semi_close_T=0;}
               }
               else if((TIMER250ms_STOP==0)&&(TIME_auto_close==0)){Receiver_OUT_STOP=0;FG_OUT_OPEN_CLOSE=0;}    //2015.3.23修改

//           if(FLAG_APP_Reply==1){FLAG_APP_Reply=0;ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status;FLAG_HA_START=1;}
//           if(FLAG_426MHz_Reply==1){FLAG_426MHz_Reply=0;ID_data.IDL=DATA_Packet_ID;Control_code=HA_Status+4;FLAG_HA_START=1;}   //受信器自动发送HA状态码为实际HA码+4
           if((FLAG_APP_Reply==1)||(FLAG_426MHz_Reply==1)){
//               if(FLAG_APP_Reply==1){FLAG_APP_Reply=0;HA_Status_buf=HA_Status+8;}
//               if(FLAG_426MHz_Reply==1){FLAG_426MHz_Reply=0;
//                                        if(HA_Status==0x84)HA_Status_buf=0x80;
//                                        else HA_Status_buf=HA_Status+12;}    //受信器自动发送HA状态码为实际HA码+4
               if(FLAG_APP_Reply==1){FLAG_APP_Reply=0;HA_Status_buf=HA_Status;}
               if(FLAG_426MHz_Reply==1){FLAG_426MHz_Reply=0;HA_Status_buf=HA_Status+4;}    //受信器自动发送HA状态码为实际HA码+4

               ID_data.IDL=DATA_Packet_ID;
                   if(DIP_switch1==1)HA_Status_buf=HA_Status_buf&0xBF;
                      else HA_Status_buf=HA_Status_buf|0x40;
                   if(DIP_switch2==1)HA_Status_buf=HA_Status_buf&0xDF;
                      else HA_Status_buf=HA_Status_buf|0x20;
                   if(DIP_switch3==1)HA_Status_buf=HA_Status_buf&0xEF;
                      else HA_Status_buf=HA_Status_buf|0x10;
                   Control_code=HA_Status_buf;
               FLAG_HA_START=1;
           }
           FLAG_Receiver_BEEP=0;
           //if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1));
           //else Receiver_LED_OUT=FLAG_PORT_LEDoutput_NOallow;
           //Receiver_OUT_OPEN=0;
           //LATACLR=0x0002;
           //Receiver_OUT_CLOSE=0;
           //if(TIMER250ms_STOP==0)Receiver_OUT_STOP=0;
          }
    if(TIMER300ms==0)Receiver_LED_RX=FLAG_PORT_LEDoutput_NOallow;
 #endif
 #if defined(__Product_PIC32MX2_WIFI__)
     if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1));
     else if(TIMER1s) WIFI_LED_RX=1;
     else WIFI_LED_RX=0;

  #if defined(__32MX250F128D__)
     if((FLAG_TIME_No_response==1)&&(TIME_No_response==0)){
         HA_uart_send_APP_number=1;   //test
         HA_uart_send_APP();
         FLAG_TIME_No_response=0;
         if(FG_HA_Inquiry_NO_again_send==1)FG_mial_com_fail=0;
         else FG_mial_com_fail=1;
     }
    if((((SWITCH_DIP_bak!=SWITCH_DIP)||(SWITCH_DIP_id_data_bak!=DATA_Packet_ID))&&(Freq_Scanning_CH_save_HA==1))){
        HA_uart_send_APP_number=2;   //test
        SWITCH_DIP_check_app(); //检测现在的值与缓存中SWITCH_DIP,如果变化FG_WIFI_SWITCH_DIP=1;
        HA_uart_send_APP();
    }
 #endif
 /********************以下是遥控板和APP一起邮件送信**********************/
      //if((FLAG_HA_Inquiry==1)||(DATA_Packet_Control_0==0x83)||(DATA_Packet_Control_0==0x85)||(DATA_Packet_Control_0==0x86)||(DATA_Packet_Control_0==0x87)){
         if((FG_mial_com_fail==1)||((DATA_Packet_Control_0>=0x81)&&(DATA_Packet_Control_0<=0x84))||((DATA_Packet_Control_0>=0x85)&&(DATA_Packet_Control_0<=0x88))){
             FLAG_HA_Inquiry=0;
             FLAG_TIME_No_response=0;
             HA_uart_send_APP_number=3;   //test
             HA_uart_send_APP();
             if(FG_mial_com_fail==1){FG_mial_com_fail=0;EMIAL_id_data_chek=ID_data.IDL;DATA_Packet_Control=0xFF;}
             else EMIAL_id_data_chek=DATA_Packet_ID;
             Email_check_app();             
             if((DATA_Packet_Control_0==0x83)||(DATA_Packet_Control_0==0x87)||(DATA_Packet_Control_0==0x84)||(DATA_Packet_Control_0==0x88))FLAG_HA_Change_ERROR=1;
             DATA_Packet_Control_0=0;
             FLAG_email_send=1;
             TIME_email_send=650;
             //if((HA_Change_email_Step==0)&&(HA_Change_send_email[0]==1)){HA_Change_email_time=18000;HA_Change_email_Step=1;}//3分钟
             if((HA_Change_email_Step==0)&&(HA_Change_send_email[0]==1)){HA_Change_email_time=16530;HA_Change_email_Step=1;}//3分钟     2014年4月24日文化修改
         }
    if((FLAG_email_send==1)&&(TIME_email_send==0)){
        FLAG_email_send=0;
        Email_check_mail();
        if(FLAG_Emial_time==1){FLAG_Emial_time=0;HA_Change_email_Step=0;FLAG_HA_Change_ERROR=0;HA_uart[8]=Emial_time_data[Emial_time_place][5];HA_uart[9]=Emial_time_data[Emial_time_place][6];HA_uart_email(EMIAL_id_PCS);} //邮件定时器到时 邮件送信
        //2014.10.11修改
        else if((FLAG_Email_check==0)&&(HA_Change_email_Step==2))HA_Change_email_Step=0;
        else if((FLAG_Email_check==1)&&(HA_Change_email_Step==2)){HA_Change_email_Step=0;FLAG_HA_Change_ERROR=0;HA_uart[8]=HA_Change_send_email[1];HA_uart[9]=HA_Change_send_email[2];HA_uart_email(EMIAL_id_PCS);}     //HA状态变化通知  上次邮件发送内容和本次内容不一样
        else if((FLAG_Email_check==1)&&(HA_Change_send_email[0]==1)&&(FLAG_HA_Change_ERROR==1)){HA_Change_email_Step=0;FLAG_HA_Change_ERROR=0;HA_uart[8]=HA_Change_send_email[1];HA_uart[9]=HA_Change_send_email[2];HA_uart_email(EMIAL_id_PCS);}
    }
    //if((HA_Change_send_email[0]==1)&&(HA_Change_email_time==0)&&(HA_Change_email_Step==1)){
    if((HA_Change_email_time==0)&&(HA_Change_email_Step==1)){    //3分钟     2014年4月24日文化修改
        //Email_check_mail();
        //if(FLAG_Email_check==1){
            //WIFI_LED_RX=1;
            HA_Change_email_Step=2;
            for(i_xm=0;i_xm<ID_DATA_PCS;i_xm++)Emial_time_OUT(i_xm);
        //}
        //else HA_Change_email_Step=0;
    }
 #endif    

}


 #if defined(__Product_PIC32MX2_WIFI__)
void SWITCH_DIP_check_app(void)
{
    UINT8 i;
        for(i=0;i<35;i++)
        {
            if(SWITCH_DIP_id_data[i]==0x00){SWITCH_DIP_id_data[i]=EMIAL_id_data_chek;SWITCH_DIP_id_DIP[i]=SWITCH_DIP;FG_WIFI_SWITCH_DIP=1;break;}
            if(SWITCH_DIP_id_data[i]==EMIAL_id_data_chek)
            {
                if(SWITCH_DIP==SWITCH_DIP_id_DIP[i])break;
                else {SWITCH_DIP_id_DIP[i]=SWITCH_DIP;FG_WIFI_SWITCH_DIP=1;break;}
            }
        }
}

void Email_check_app(void)
{
//    UINT8 i;
//        for(i=0;i<35;i++)
//        {
//            if(EMIAL_id_data[i]==0x00){ EMIAL_id_data[i]=DATA_Packet_ID;EMIAL_id_HA[i]=DATA_Packet_Control;EMIAL_id_PCS++;break;}
//            if(EMIAL_id_data[i]==DATA_Packet_ID)
//            {
//                if(DATA_Packet_Control==EMIAL_id_HA[i])break;
//                else {EMIAL_id_HA[i]=DATA_Packet_Control;break;}
//            }
//        }

                                    //20150430 japan修改2     以下所有都是
    UINT8 i;
    for(i=0;i<ID_DATA_PCS;i++)
        EMIAL_id_data[i]=ID_Receiver_DATA[i];
    EMIAL_id_PCS=ID_DATA_PCS;
        for(i=0;i<35;i++)
        {
            if(EMIAL_id_data[i]==0x00){ EMIAL_id_data[i]=DATA_Packet_ID;EMIAL_id_HA[i]=DATA_Packet_Control;
                          //EMIAL_id_PCS++;   
                          break;}
            if(EMIAL_id_data[i]==DATA_Packet_ID)
            {
                if(DATA_Packet_Control==EMIAL_id_HA[i])break;
                else {EMIAL_id_HA[i]=DATA_Packet_Control;break;}
            }
        }
}
void HA_Cache_ha_switch(UINT32 ID_IN)  //2015.4.1修正3 由于APP查询受信器HA状态需要很长的时间，所以追加指令查询缓存在通信机里面的HA状态
{
    UINT8 i;
        for(i=0;i<35;i++)
        {
            if(HA_Cache_IDdata[i]==0x00){ HA_Cache_IDdata[i]=ID_IN;HA_Cache_ha[i]=HA_Cache_ha_bak;HA_Cache_ha_1Hz[i]=HA_Cache_ha_1Hz_bak;HA_Cache_SWITCH_DIP[i]=HA_Cache_SWITCH_DIP_bak;i=35;}
            if(HA_Cache_IDdata[i]==ID_IN){HA_Cache_ha[i]=HA_Cache_ha_bak;HA_Cache_ha_1Hz[i]=HA_Cache_ha_1Hz_bak;HA_Cache_SWITCH_DIP[i]=HA_Cache_SWITCH_DIP_bak;i=35;}
//            if(HA_Cache_IDdata[i]==0x00){ HA_Cache_IDdata[i]=DATA_Packet_ID;HA_Cache_ha[i]=DATA_Packet_Control;HA_Cache_SWITCH_DIP[i]=SWITCH_DIP;break;}
//            if(HA_Cache_IDdata[i]==DATA_Packet_ID){HA_Cache_ha[i]=DATA_Packet_Control;HA_Cache_SWITCH_DIP[i]=SWITCH_DIP;}
        }
}

void  Email_check_mail(void)
{
    UINT8 i,j;
        for(i=0;i<35;i++)
        {
            FLAG_Email_check=0;
            for(j=0;j<35;j++)
            {
                if((Email_check_ID[j]==EMIAL_id_data[i])&&(Emial_check_Control[j]==EMIAL_id_HA[i])){FLAG_Email_check=1;j=64;}
            }
            if(FLAG_Email_check==0){FLAG_Email_check=1;return;}
            else FLAG_Email_check=0;
        }

}
 #endif

void  Freq_Scanning(void)
{
    //if((FLAG_Receiver_Scanning==1)&&(FLAG_APP_RX==1)&&(FLAG_UART_ok==0))
 #if defined(__Product_PIC32MX2_WIFI__)
    //if(((FLAG_Receiver_Scanning==1)||(TIME_EMC==0))&&(FLAG_APP_RX==1)&&(FLAG_UART_ok==0))
    if(((FLAG_Receiver_Scanning==1)||(TIME_EMC==0)||(TIME_Fine_Calibration==0))&&(FLAG_APP_RX==1)&&(FLAG_UART_ok==0))
 #endif
 #if defined(__Product_PIC32MX2_Receiver__)
    if(((FLAG_Receiver_Scanning==1)||(TIME_EMC==0)||(TIME_Fine_Calibration==0))&&(FLAG_APP_RX==1)&&(FLAG_UART_ok==0))
 #endif
    {
        FLAG_Receiver_Scanning=0;
 #if defined(__Product_PIC32MX2_WIFI__)
//        if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))Freq_Scanning_CH=1;
//        else {
//               Freq_Scanning_CH=Freq_Scanning_CH+2;
//               if(Freq_Scanning_CH>4){Freq_Scanning_CH=2;dd_set_ADF7021_ReInitial();}
//        }
//        dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
//        TIMER18ms=18;//18;

        if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))Freq_Scanning_CH=1;
        else {
               if(Freq_Scanning_CH==1)Freq_Scanning_CH=2;
               else Freq_Scanning_CH=Freq_Scanning_CH+2;
               if(Freq_Scanning_CH>4)Freq_Scanning_CH=2;
        }
        if(TIME_Fine_Calibration==0){TIME_Fine_Calibration=9000;dd_set_ADF7021_ReInitial();dd_set_RX_mode();}
        dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
        TIMER18ms=18;//18;
 #endif
 #if defined(__Product_PIC32MX2_Receiver__)
        #if PIC32MX2_Receiver_mode               //TX and RX
//            Freq_Scanning_CH++;
//            if(Freq_Scanning_CH>6){Freq_Scanning_CH=1;dd_set_ADF7021_ReInitial();}
//            dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
//            if((Freq_Scanning_CH==1)||(Freq_Scanning_CH==3)||(Freq_Scanning_CH==5))TIMER18ms=36; //36
//            else TIMER18ms=18;
        
            Freq_Scanning_CH++;
            //if(Freq_Scanning_CH==3)Freq_Scanning_CH=4;
            //if(Freq_Scanning_CH>4){Freq_Scanning_CH=1;dd_set_ADF7021_ReInitial();}
            //if(TIME_EMC==0){dd_set_ADF7021_ReInitial();dd_set_RX_mode();}
            if(TIME_Fine_Calibration==0){TIME_Fine_Calibration=9000;dd_set_ADF7021_ReInitial();dd_set_RX_mode();}
            if(Freq_Scanning_CH>4)Freq_Scanning_CH=1;
            dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
            TIMER18ms=18;
        #else                                   //RX
//            Freq_Scanning_CH=1;
//            dd_set_ADF7021_ReInitial();
//            dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
//            TIMER18ms=18;

//            Freq_Scanning_CH=1;
//            if(TIME_EMC==0){dd_set_ADF7021_ReInitial();dd_set_RX_mode();}
//            dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
//            TIMER18ms=18;

            Freq_Scanning_CH=1;
            //if(Freq_Scanning_CH==3)Freq_Scanning_CH=4;
            //if(Freq_Scanning_CH>4){Freq_Scanning_CH=1;dd_set_ADF7021_ReInitial();}
            //if(TIME_EMC==0){dd_set_ADF7021_ReInitial();dd_set_RX_mode();}
            if(TIME_Fine_Calibration==0){TIME_Fine_Calibration=9000;dd_set_ADF7021_ReInitial();dd_set_RX_mode();}
            dd_set_ADF7021_Freq(0,Freq_Scanning_CH);
            TIMER18ms=18;
        #endif
 #endif
    }
}

void SendTxData(void)
{
       FLAG_APP_RX=0;
 #if defined(__Product_PIC32MX2_Receiver__)
      Receiver_LED_RX=FLAG_PORT_LEDoutput_NOallow;
      Receiver_LED_OUT=FLAG_PORT_LEDoutput_NOallow;
 #endif
 #if defined(__Product_PIC32MX2_WIFI__)
       WIFI_LED_RX=0;
 #endif
       ADF7021_DATA_IO=0;           //测试
       dd_set_TX_mode();
 #if defined(__Product_PIC32MX2_Receiver__)
       Receiver_LED_TX=FLAG_PORT_LEDoutput_allow;
 #endif
 #if defined(__Product_PIC32MX2_WIFI__)
       WIFI_LED_TX=1;
 #endif
       SetTxData();
       txphase=0;
       txphase_Repeat=0;
       ID_INT_CODE=0;
       FLAG_APP_TX=1;
}

void SetTxData(void)
{
  uni_i unii,unij,unik;
  	/*	ID set	*/
        m_RFNormalBuf[22]=0x15;
	unii.ui = SetFixedLengthCode(ID_data.IDB[0]) ;
	m_RFNormalBuf[23] = unii.uc[1] ;
	m_RFNormalBuf[24] = unii.uc[0] ;
	unii.ui = SetFixedLengthCode(ID_data.IDB[1]) ;
	m_RFNormalBuf[25] = unii.uc[1] ;
	m_RFNormalBuf[26] = unii.uc[0] ;
	unii.ui = SetFixedLengthCode(ID_data.IDB[2]) ;
	m_RFNormalBuf[27] = unii.uc[1] ;
	m_RFNormalBuf[28] = unii.uc[0] ;
	/*	Control code set	*/
	unii.ui = SetFixedLengthCode(Control_code) ;
	m_RFNormalBuf[29] = unii.uc[1] ;
	m_RFNormalBuf[30] = unii.uc[0] ;
	/*	SUM set	*/
	unii.uc[0] = ID_data.IDB[2] ;
	unii.uc[1] = Control_code;
	unij.uc[0] = ID_data.IDB[0] ;
	unij.uc[1] = ID_data.IDB[1] ;
	unik.ui = unii.ui + unij.ui ;
	unii.ui = SetFixedLengthCode(unik.uc[0]) ;
	m_RFNormalBuf[31] = unii.uc[1] ;
	m_RFNormalBuf[32] = unii.uc[0] ;
	unii.ui = SetFixedLengthCode(unik.uc[1]) ;
	m_RFNormalBuf[33] = unii.uc[1] ;
	m_RFNormalBuf[34] = unii.uc[0] ;
}

UINT16 SetFixedLengthCode(UINT8 data )
{
	UINT16	code ;
	UINT8	i ;

	for	(i=0; i<8; i++)
	{
		code <<=2 ;
		if	( data & 0b00000001)		// '1' ?
		{					// Yes
			code |= 0x0002 ;
		}
		else
		{
			code |= 0x0001 ;      // '0'

		}
		data >>= 1 ;
	}
	return(code) ;
}


//void ID_Decode_Initial_CNx(void)
//{
//     /*The following code example illustrates a Change Notice interrupt configuration for pins
//    CNC5(PORTC.RC5).*/
//                      /* NOTE:disable vector interrupts prior to configuration */
//    CNCONCbits.ON= 1; // Enable Change Notice module
//    CNENCbits.CNIEC5 = 1; // Enable individual CN pins CNC5
//    //CNPUCbits.CNPUC5= 1; // Enable weak pull ups for pins CN5
//
//    ADF7021_DATA_CLK;             /* read port(s) to clear mismatch on change notice pins */
//
//    IPC8bits.CNIP = 5; // Set priority level=5
//    IPC8bits.CNIS = 3; // Set Subpriority level=3
//                         // Could have also done this in single
//                        // operation by assigning IPC6SET = 0x00170000
//    IFS1bits.CNCIF = 0;   // Clear the interrupt flag status bit
//    IEC1bits.CNCIE = 1;   // Enable Change Notice interrupts
//                        /* re-enable vector interrupts after configuration */
//}
//
///*
//The following code example demonstrates a simple interrupt service routine for CN
//interrupts.The user’s code at this vector can perform any application specific
//operations.The user’s code must read the CN corresponding PORT registers to clear the
//mismatch conditions before clearing the CN interrupt status flag.Finally, the CN
//interrupt status flag must be cleared before exiting.
//*/
//void __ISR(_CHANGE_NOTICE_VECTOR, ipl5) ChangeNoticeHandler(void)
//{
//    UINT8 CNC5_vlaue;
//    //... perform application specific operations in response to the interrupt
//    //ADF7021_DATA_CLK; // Read PORTC to clear CN1 mismatch condition
//    //Receiver_LED_TX= CNC5_vlaue;   //测试，测试完后需要删除
//    //Receiver_LED_TX= !Receiver_LED_TX;   //测试，测试完后需要删除
//    IFS1bits.CNCIF = 0;// Be sure to clear the CN interrupt status
//    // flag before exiting the service routine.
//}