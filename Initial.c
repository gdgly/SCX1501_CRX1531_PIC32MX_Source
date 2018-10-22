/***********************************************************************/
/*  FILE        :initial.C                                             */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化

UINT32 EEPROM_Receiver_ID=13186823;

FLAG FLAG_APP;
UINT16 rssi;
UINT8 Frequency_CH;

UINT8 rxphase=0;
UINT16 txphase=0;
UINT8 txphase_Repeat=0;
UINT8 Syn_count=0;
UINT32 DATA_Packet_Syn=0;     //A部
UINT16 DATA_Packet_Head=0;    //B部
UINT32 DATA_Packet_Code[3]={0};   //C部
UINT8  DATA_Packet_Code_g=0;
UINT8  DATA_Packet_Code_i=0;
UINT32 DATA_Packet_ID=0;
UINT16  TIMER1s=0;
UINT16  TIMER300ms=0;
UINT8   TIMER18ms=0;
UINT8   Freq_Scanning_CH=0;
UINT8  m_RFNormalBuf[35];
uni_rom_id ID_data;
UINT8 Control_code=0;
UINT8 ID_INT_CODE=0;
UINT8 FLAG_APP_TX=0;
UINT8 FLAG_APP_RX=0;
UINT8 FLAG_SendTxData=0;

void VHF_GPIO_INIT(void){	
    // CPU端口设置
    ANSELA = 0x0000; // Turn off all ADC analog intput pins
    ANSELB = 0x0000;
    ANSELC = 0x0000;
 #if defined(__Product_PIC32MX2_Receiver__)
    TRISBbits.TRISB5=1;
    TRISBbits.TRISB10=1;
    TRISBbits.TRISB11=1;
    TRISBbits.TRISB14=1;
    TRISAbits.TRISA0=1;
 #endif
//    RTCCONbits.ON=0;
//    RTCCONbits.RTCCLKON=0;
//    PMCONbits.ON=0;
//    CNCONAbits.ON=0;
//    CNCONBbits.ON=0;
//    CNCONCbits.ON=0;
//
//    /* Input Pins Group 1 */
//    INT4Rbits.INT4R=8;
//    T2CKRbits.T2CKR=8;
//    IC4Rbits.IC4R=8;
//    SS1Rbits.SS1R=8;
//    REFCLKIRbits.REFCLKIR=8;
//    /* Input Pins Group 2 */
//    INT3Rbits.INT3R=8;
//    T3CKRbits.T3CKR=8;
//    IC3Rbits.IC3R=8;
//    U1CTSRbits.U1CTSR=8;
//    U2RXRbits.U2RXR=8;
//    SDI1Rbits.SDI1R=8;
//    /* Input Pins Group 3 */
//    INT2Rbits.INT2R=8;
//    T4CKRbits.T4CKR=8;
//    IC1Rbits.IC1R=8;
//    IC5Rbits.IC5R=8;
//    U1RXRbits.U1RXR=8;
//    U2CTSRbits.U2CTSR=8;
//    SDI2Rbits.SDI2R=8;
//    OCFBRbits.OCFBR=8;
//    /* Input Pins Group 4 */
//    INT1Rbits.INT1R=8;
//    T5CKRbits.T5CKR=8;
//    IC2Rbits.IC2R=8;
//    SS2Rbits.SS2R=8;
//    OCFARbits.OCFAR=8;
    

    #if defined(__Product_PIC32MX2_Receiver__)
       ADF7021_SCLK_IO = 0;
       ADF7021_SCLK=0;
       ADF7021_SDATA_IO = 0;
       ADF7021_SDATA=0;
       ADF7021_SLE_IO = 0;
       ADF7021_SLE=0;
       ADF7021_SREAD_IO = 1;
       ADF7021_CE_IO = 0;
       ADF7021_CE=0;
       ADF7021_MUXOUT_IO = 1;
       ADF7021_DATA_CLK_IO = 1;
       ADF7021_DATA_IO = 1;
       ADF7021_CLKOUT_IO=1;     // Input
       ADF7021_INT_LOCK_IO=1;   // Input
       SDAIO=0; // Input AND output
       SCLIO=0; // output

      HA_L_signal_IO=1; // Input   HA 下限信号   高电平有效
      HA_ERR_signal_IO=1;// Input   HA 异常信号  高电平有效
      HA_Sensor_signal_IO=1;// Input   HA 传感器信号  高电平有效
      Receiver_Login_IO=1;// Input   受信机登录键   低电平有效
      Receiver_Buzzer_IO=0;// Output   受信机蜂鸣器  高电平有效
      Receiver_LED_OUT_IO=0;// Output   受信机继电器动作输出  低电平有效
      Receiver_LED_OUT=1;
      Receiver_LED_TX_IO=0;// Output   受信机送信指示  低电平有效
      Receiver_LED_TX=1;
      Receiver_LED_RX_IO=0;// Output   受信机受信指示  低电平有效
      Receiver_LED_RX=1;
      Receiver_OUT_OPEN_IO=0;  // Output   受信机继电器OPEN  高电平有效
      Receiver_OUT_CLOSE_IO=0;  // Output   受信机继电器CLOSE  高电平有效
      Receiver_OUT_STOP_IO=0;  // Output   受信机继电器STOP  高电平有效
      Receiver_OUT_VENT_IO=0;
    #endif

   #if defined(__Product_PIC32MX2_WIFI__)
       ADF7021_SCLK_IO = 0;
       ADF7021_SCLK=0;
       ADF7021_SDATA_IO = 0;
       ADF7021_SDATA=0;
       ADF7021_SLE_IO = 0;
       ADF7021_SLE=0;
       ADF7021_SREAD_IO = 1;
       ADF7021_CE_IO = 0;
       ADF7021_CE=0;
       ADF7021_MUXOUT_IO = 1;
       ADF7021_DATA_CLK_IO = 1;
       ADF7021_DATA_IO = 1;
       ADF7021_CLKOUT_IO=1;     // Input
       ADF7021_INT_LOCK_IO=1;   // Input
       SDAIO=0; // Input AND output
       SCLIO=0; // output

     WIFI_L_Login_IO=1;// Input   wifi集中通讯机登录键   低电平有效
     WIFI_USBLogin_IO=1;// Input   wifi集中通讯机USB升级键   低电平有效
     WIFI_Useless0_IO=1;// Input  样机板后面没有用
     WIFI_Useless1_IO=0;// output  样机板后面没有用    测试版作为LED2       //测试，测试完后需要删除
     WIFI_Useless1=1;                                                      //测试，测试完后需要删除
     WIFI_USBOC_IO=1;//Input   wifi集中USB保护监测   低电平有效
     WIFI_LED_RX_IO=0;//output   wifi集中通讯机受信指示 低电平有效
     WIFI_LED_RX=1;
     WIFI_LED_TX_IO=0;// output   wifi集中通讯机送信指示 低电平有效
     WIFI_LED_TX=1;
   #endif

}



//===================Delay100us()延时===============//
void Delay100us(unsigned int timer)
{
unsigned int x;
unsigned int y;                    //延时T=(timer)ms
 for(x=0;x<timer;x++)
  {for(y=0;y<=200;y++);
   }
}
void Delayus(unsigned int timer)
{
  unsigned int x;            //延时T=(timer)ms
  for(x=0;x<timer;x++);
}