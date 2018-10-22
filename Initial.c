
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

UINT32 EEPROM_Receiver_ID=2000307;//13040292;//13040451;//2000307;//13186823;

FLAG FLAG_APP;
UINT16 rssi;
UINT8 Frequency_CH;

UINT8 rxphase=0;
UINT16 txphase=0;
UINT8 txphase_Repeat=0;
UINT8 Syn_count=0;
UINT32 DATA_Packet_Syn=0;     //A部
UINT16 DATA_Packet_Head=0;    //B部
UINT32 DATA_Packet_Code[6]={0};   //C部
UINT8  DATA_Packet_Code_g=0;
UINT8  DATA_Packet_Code_i=0;
UINT32 DATA_Packet_ID=0;
UINT8  DATA_Packet_Control=0;
UINT8  DATA_Packet_Control_0=0;
UINT8  Control_bak=0;
UINT16  TIMER1s=0;
UINT16  TIMER300ms=0;
UINT16  TIMER18ms=0;
UINT8   TIMER250ms_STOP=0;
UINT16  TIMER60s=0;
UINT8   HA_Status=0;
UINT8   Emial_Control=0;
UINT32  Emial_ID=0;
UINT8   Freq_Scanning_CH=0;
UINT8   Freq_Scanning_CH_bak=0;
UINT8   Freq_Scanning_CH_save=0;
UINT8  m_RFNormalBuf[35];
uni_rom_id ID_data;
UINT8 Control_code=0;
UINT8 ID_INT_CODE=0;

UINT16 UART_DATA_i=0;
UINT8  UART_DATA_cnt=0;
UINT8  UART1_DATA[27]={0};
UINT8  UART_DATA_buffer[27]={0};
UINT8  TIME_UART=0;
UINT8  UART_send_count=0;
UINT16 TIME_email_Repeat=0;

UINT8  TIME_10ms=0;
UINT8  COUNT_Receiver_Login=0;
UINT16 TIME_Receiver_Login=0;
UINT16 TIME_Receiver_Login_led=0;
UINT16 TIME_Receiver_Login_restrict=0;
UINT16 TIME_Login_EXIT_rest=0;
UINT32 ID_Receiver_Login=0;
UINT32 ID_Receiver_DATA[256] = {0};//写入EEPROM ID的数据
UINT16 ID_DATA_PCS=0;
UINT16 INquiry=0;

UINT8  rssi_COUNT=0;
UINT8  rssi_TIME=0;
UINT8  TX_Freq_CH=0;

UINT8 HA_uart_app[15]={0xBB,0x00,0x07,0x00,0x00,0x00,0x5,0x00,
                   0x00,0x00,0x00,0x00,0x00,
                   0x00,0x00};

UINT8 FLAG_APP_TX=0;
UINT8 FLAG_APP_RX=0;
UINT8 FLAG_SendTxData=0;
UINT8 FLAG_UART_R=0;
UINT8 FLAG_UART_0xBB=0;
UINT8 FLAG_UART_ok=0;
UINT8 FLAG_ADF7021_ReInitial=0;
UINT8 FLAG_IDCheck_OK=0;
UINT16 time_3sec=0;

UINT8 TIME_EMC=0;   //静电测试

#if defined(__Product_PIC32MX2_WIFI__)
    UINT8 WIFI_alarm_data[200][10]={0};
    UINT8 WIFI_alarm_data_PCS=0;
    UINT8 WIFI_alarm_Hours_Minutes[2]={0xFF,0xFF};
    UINT8 AUTO_SEND_DATA[200][4]={0};
    UINT8 AUTO_SEND_DATA_pcs=0;
    UINT16 TIME_alarm_AUTO=0;
    UINT8 AUTO_HA_Inquiry=0;
    UINT8 HA_uart[1200]={0xBB,0x00,0x20,0x00,0x00,0x00,0x23,0x00,          //头
                       0x32,0x30,0x00,0x00,       //年
                       0x2E,0x00,0x00,          //月
                       0x2E,0x00,0x00,         //日
                       0x20,0x00,0x00,        //小时
                       0x3A,0x00,0x00,       //分钟
                       0x00,      //标题结束符
                       //以下邮件内容
                       0x49,0x44,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,  //ID=xxxxxxxx
                       0x00,0x00,0x00,0x00,0x00,0x0d, //   如：OPEN
                       0x00,       //邮件内容结束符
                       0x00,0x00   //校验码
                       };
    UINT32 EMIAL_id_data[64];
    UINT8  EMIAL_id_HA[64];
    UINT8  EMIAL_id_PCS=0;
    UINT8  FLAG_email_send=0;
    UINT16 TIME_email_send=0;
    UINT32 Email_check_ID[64]={0x00};
    UINT8  Emial_check_Control[64]={0x00};
#endif


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
        HA_Status=0x81;
        FLAG_open=1;
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
      Receiver_Buzzer=0;
      Receiver_LED_OUT_IO=0;// Output   受信机继电器动作输出  低电平有效
      Receiver_LED_OUT=1;
      Receiver_LED_TX_IO=0;// Output   受信机送信指示  低电平有效
      Receiver_LED_TX=0;
      Receiver_LED_RX_IO=0;// Output   受信机受信指示  低电平有效
      Receiver_LED_RX=0;
      Receiver_OUT_OPEN_IO=0;  // Output   受信机继电器OPEN  高电平有效
      Receiver_OUT_OPEN=0;
      Receiver_OUT_CLOSE_IO=0;  // Output   受信机继电器CLOSE  高电平有效
      Receiver_OUT_CLOSE=0;
      Receiver_OUT_STOP_IO=0;  // Output   受信机继电器STOP  高电平有效
      Receiver_OUT_STOP=0;
      Receiver_OUT_VENT_IO=0;
      Receiver_OUT_VENT=0;
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

       PCF8563_INT_IO=1;// Input    追加定时OPEN CLOSE
       CNPUCbits.CNPUC0=1;
       //CNPDCbits.CNPDC0=0;
       
     WIFI_L_Login_IO=1;// Input   wifi集中通讯机登录键   低电平有效
     WIFI_USBLogin_IO=1;// Input   wifi集中通讯机USB升级键   低电平有效
     WIFI_Useless0_IO=1;// Input  样机板后面没有用
     WIFI_Useless1_IO=0;// output  样机板后面没有用    测试版作为LED2       //测试，测试完后需要删除
     WIFI_Useless1=1;                                                      //测试，测试完后需要删除
     WIFI_USBOC_IO=1;//Input   wifi集中USB保护监测   低电平有效
     WIFI_LED_RX_IO=0;//output   wifi集中通讯机受信指示 低电平有效
     WIFI_LED_RX=0;
     WIFI_LED_TX_IO=0;// output   wifi集中通讯机送信指示 低电平有效
     WIFI_LED_TX=0;
   #endif

}



//===================Delay100us()延时===============//
void Delay100us(unsigned int timer)
{
unsigned int x;
unsigned int y;                    //延时T=(timer)100us
 for(x=0;x<timer;x++)
  {
#if defined(__Product_PIC32MX2_Receiver__)
     //for(y=0;y<=200;y++);   //特别说明：该行采用XC32的0级优化，即无优化
     for(y=0;y<=600;y++);   //特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
#endif
#if defined(__Product_PIC32MX2_WIFI__)
     for(y=0;y<=400;y++);
#endif
   }
}
void Delayus(unsigned int timer)
{
  unsigned int x;            //延时T=(timer)us     //SYSCLK=20M
#if defined(__Product_PIC32MX2_Receiver__)
    // for(x=0;x<2*timer;x++);  //特别说明：该行采用XC32的0级优化，即无优化
   for(x=0;x<10*timer;x++);  //特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
#endif
#if defined(__Product_PIC32MX2_WIFI__)
     for(x=0;x<4*timer;x++);
#endif
}