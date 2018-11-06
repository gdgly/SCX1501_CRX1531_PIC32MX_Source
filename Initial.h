/***********************************************************************/
/*  FILE        :initial.h                                             */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>				// 常用C定义

#if defined(__32MX250F128D__)
    #define __Product_PIC32MX2_WIFI__            //"0"代表DIV8  CPU Freq=10MHz  受信机
#endif

#if defined(__32MX230F064D__)
    #define __Product_PIC32MX2_WIFI__            //"0"代表DIV8  CPU Freq=10MHz  受信机
#endif

#if defined(__32MX210F016D__)
    #define __Product_PIC32MX2_Receiver__             //"0"代表DIV8  CPU Freq=10MHz  受信机
#endif

#if defined(__32MX220F032D__)
    #define __Product_PIC32MX2_Receiver__             //"0"代表DIV8  CPU Freq=10MHz  受信机

    #define		PIC32MX2_Receiver_mode   0              //模式选择  0=RX      1=TX and RX
#endif

    //============================================
    #define	UINT8		unsigned char
    #define	INT8		char
    #define	UINT16		unsigned short int
    #define	INT16		short int
    #define	UINT32		unsigned long
    #define	INT32		long
    #define     NOP()           asm ("nop")
    //--------------------------------------------

  typedef union{
    UINT32  whole_reg;
    UINT8   byte[4];    // Warning: Be endian-specific when accessing bytes

   } ADF70XX_REG_T;

  typedef union {												// ID No.
	UINT32	IDL ;
	UINT8	IDB[4];
  }uni_rom_id;
  typedef union {
        UINT16	ui ;
	UINT8	uc[2] ;
  }uni_i;

typedef union{
	UINT32 BYTE;	
	struct { 
		unsigned char	Bit0:	1;
		unsigned char	Bit1:	1;
		unsigned char	Bit2:	1;
		unsigned char	Bit3:	1;
		unsigned char	Bit4:	1;
		unsigned char	Bit5:	1;
		unsigned char	Bit6:	1;
		unsigned char	Bit7:	1;
		unsigned char	Bit8:	1;
		unsigned char	Bit9:	1;
		unsigned char	Bit10:	1;
		unsigned char	Bit11:	1;
		unsigned char	Bit12:	1;
		unsigned char	Bit13:	1;
		unsigned char	Bit14:	1;
		unsigned char	Bit15:	1;                
		unsigned char	Bit16:	1;
		unsigned char	Bit17:	1;
		unsigned char	Bit18:	1;
		unsigned char	Bit19:	1;
		unsigned char	Bit20:	1;
		unsigned char	Bit21:	1;
		unsigned char	Bit22:	1;
		unsigned char	Bit23:	1;
		unsigned char	Bit24:	1;
		unsigned char	Bit25:	1;
		unsigned char	Bit26:	1;
		unsigned char	Bit27:	1;
		unsigned char	Bit28:	1;
		unsigned char	Bit29:	1;
		unsigned char	Bit30:	1;
		unsigned char	Bit31:	1;                               
	}BIT; 	
}FLAG;

extern FLAG FLAG_APP;
	//************************************************
	#define 	FLAG_APP_BYTE		FLAG_APP.BYTE	 
	//------------------------------------------------
	#define		FLAG_AUTO_SEND_START		FLAG_APP.BIT.Bit0      
        #define		FLAG_AUTO_SEND_ok		FLAG_APP.BIT.Bit1      
        #define		Freq_Scanning_CH_save_HA	FLAG_APP.BIT.Bit2

//        #define		FLAG_APP_SW1		FLAG_APP.BIT.Bit0      //test use
//        #define		FLAG_APP_SW2		FLAG_APP.BIT.Bit1      //test use
//        #define		FLAG_APP_SW3		FLAG_APP.BIT.Bit2      //test use

	#define		FLAG_rssi_Freq		FLAG_APP.BIT.Bit3
	#define		FLAG_Receiver_BEEP	FLAG_APP.BIT.Bit4

	#define		FLAG_Signal_DATA_OK	FLAG_APP.BIT.Bit5
        #define		FLAG_Receiver_IDCheck 	FLAG_APP.BIT.Bit6
        #define		FLAG_Receiver_Scanning 	FLAG_APP.BIT.Bit7
        #define		FLAG_ID_Login 	        FLAG_APP.BIT.Bit8
        #define		FLAG_ID_Erase_Login 	FLAG_APP.BIT.Bit9
        #define		FLAG_ID_Login_EXIT 	FLAG_APP.BIT.Bit10

        #define		FLAG_ID_Login_OK 	FLAG_APP.BIT.Bit12
        #define		FLAG_ID_Login_OK_bank 	FLAG_APP.BIT.Bit13
        #define		FLAG_HA_START    	FLAG_APP.BIT.Bit14

        #define		FLAG_HA_L_signal    	FLAG_APP.BIT.Bit15
        #define		FLAG_HA_ERR_signal    	FLAG_APP.BIT.Bit16

        #define		FLAG_open    	        FLAG_APP.BIT.Bit17
        #define		FLAG_close    	        FLAG_APP.BIT.Bit18
        #define		FLAG_HA_ERR    	        FLAG_APP.BIT.Bit19
        #define		FLAG_APP_Reply    	FLAG_APP.BIT.Bit20

        #define		FLAG_HA_Emial    	FLAG_APP.BIT.Bit21
        #define		FLAG_HA_Inquiry    	FLAG_APP.BIT.Bit22

        #define		FG_10ms 	        FLAG_APP.BIT.Bit23

        #define		FLAG_all_Erase_loop    	FLAG_APP.BIT.Bit24
        #define		FLAG_all_Erase    	FLAG_APP.BIT.Bit25
        #define		FLAG_all_Erase_OK    	FLAG_APP.BIT.Bit26

        #define		FLAG_426MHz_Reply    	FLAG_APP.BIT.Bit27
        #define		FLAG_email_Repeat    	FLAG_APP.BIT.Bit28

        #define		FLAG_Email_check    	FLAG_APP.BIT.Bit29
        #define		FLAG_ID_Erase_Login_PCS   FLAG_APP.BIT.Bit30
	//************************************************



extern FLAG FLAG_PORT;
//************************************************
	#define 	FLAG_PORT_BYTE		FLAG_PORT.BYTE	 
	//------------------------------------------------
	#define		FLAG_PORT_LEDoutput_allow		FLAG_PORT.BIT.Bit0 
    #define		FLAG_PORT_LEDoutput_NOallow		FLAG_PORT.BIT.Bit1 

//extern UINT32 EEPROM_Receiver_ID;

extern UINT16 rssi;
extern UINT8 Frequency_CH;
extern UINT8 rxphase;
extern UINT16 txphase;
extern UINT8 txphase_Repeat;
extern UINT8 Syn_count;
extern UINT32 DATA_Packet_Syn;    //A部
extern UINT16 DATA_Packet_Head;   //B部
extern UINT32 DATA_Packet_Code[6];  //C部
extern UINT8  DATA_Packet_Code_g;
extern UINT8  DATA_Packet_Code_i;
extern UINT32 DATA_Packet_ID;
extern UINT8  DATA_Packet_Control;
extern UINT32 DATA_Packet_ID_buf;
extern UINT8  DATA_Packet_Control_buf;
extern UINT8  DATA_Packet_Control_0;
extern UINT8  DATA_Packet_Control_err;
extern UINT8  Control_bak;
extern UINT16  TIMER1s;
extern UINT16  TIMER300ms;
extern UINT16  TIMER18ms;
extern UINT16   TIMER250ms_STOP;
extern UINT16  TIMER60s;
extern UINT8   HA_Status;
extern UINT8   Emial_Control;
extern UINT32  Emial_ID;
extern UINT8  Freq_Scanning_CH;
extern UINT8  Freq_Scanning_CH_bak;
extern UINT8  Freq_Scanning_CH_save;
extern UINT8  m_RFNormalBuf[35]; // Buffer for data
                               /* m_RFNormalBuf[0] 帧间隔时间10ms(LSB)
                                * m_RFNormalBuf[1] 帧间隔时间10ms(MSB)
                                * m_RFNormalBuf[2] SYN同步时间160BIT 第20个字节(LSB)
                                *    .。。。。。。。。。。
                                * m_RFNormalBuf[21] SYN同步时间160BIT 第0个字节(MSB)
                                * m_RFNormalBuf[22] 同步码0X15
                                * m_RFNormalBuf[23] ID code(LSB)
                                * m_RFNormalBuf[24] ID code
                                * m_RFNormalBuf[25] ID code
                                * m_RFNormalBuf[26] ID code
                                * m_RFNormalBuf[27] ID code
                                * m_RFNormalBuf[28] ID code(MSB)
                                * m_RFNormalBuf[29] Control code(LSB)
                                * m_RFNormalBuf[30] Control code(MSB)
                                * m_RFNormalBuf[31] SUM code(LSB)
                                * m_RFNormalBuf[32] SUM code
                                * m_RFNormalBuf[33] SUM code
                                * m_RFNormalBuf[34] SUM code(MSB)*/
extern uni_rom_id ID_data;
extern UINT8 Control_code;
extern UINT8 ID_INT_CODE;

extern UINT16 UART_DATA_i;
extern UINT16  UART_DATA_cnt;

extern UINT8  TIME_UART;
extern UINT8  UART_send_count;
extern UINT16 TIME_email_Repeat;

extern UINT8  TIME_10ms;
extern UINT8  COUNT_Receiver_Login;
extern UINT16  TIME_Receiver_Login;
extern UINT16 TIME_Receiver_Login_led;
extern UINT16 TIME_Receiver_Login_restrict;
extern UINT16 TIME_Login_EXIT_rest;
extern UINT32 ID_Receiver_Login;
extern UINT32 ID_Receiver_DATA[256];//写入EEPROM ID的数据
extern UINT16 ID_DATA_PCS;
extern UINT16 INquiry;

extern UINT8  rssi_COUNT;
extern UINT8  rssi_TIME;
extern UINT8  TX_Freq_CH;

extern UINT8 HA_uart_app[18];

extern UINT8 FLAG_APP_TX;
extern UINT8 FLAG_APP_RX;
extern UINT8 FLAG_SendTxData;
extern UINT8  FLAG_UART_R;
extern UINT8 FLAG_UART_0xBB;
extern UINT8  FLAG_UART_ok;
extern UINT8  FLAG_ADF7021_ReInitial;
extern UINT8 FLAG_IDCheck_OK;
extern UINT16 time_3sec;

extern UINT16 FLAG_all_Erase_time;

extern UINT8 TIME_EMC;

extern UINT16 TIME_No_response;
extern UINT8 FLAG_TIME_No_response;
extern UINT8 read_TIMER_Semi_open;

//*********以下为测试代码所用变量****************
extern UINT8 Tx_Rx_mode;
extern FLAG FLAG_test;
	//************************************************
	//************************************************
	#define 	FLAG_test_BYTE		FLAG_test.BYTE
	//------------------------------------------------
        //#define		FLAG_Receiver_Scanning	FLAG_test.BIT.Bit0
        #define		FG_test_tx_1010	        FLAG_test.BIT.Bit1
        //#define		X_HIS	                FLAG_test.BIT.Bit2    //历史记录   误码率测试用
        #define		FG_test_tx_on		FLAG_test.BIT.Bit3
        #define		FG_test_tx_off	        FLAG_test.BIT.Bit4
        #define		FG_test_mode	        FLAG_test.BIT.Bit5
        //#define		FG_test1	        FLAG_test.BIT.Bit6
        #define		FG_test_rx		FLAG_test.BIT.Bit7
extern UINT32 RF_SET_TX_carrier_test[6];
extern UINT32 RF_SET_TX_1010pattern[6];
extern UINT32 RF_SET_RX_test[6];
extern UINT16 X_COUNT ;
extern UINT16 X_ERR ;//记录错误的个数
extern UINT8 X_HIS; //历史记录   误码率测试用

extern UINT16 TIME_Fine_Calibration;   //窄带下中频滤波器100KHz精校
//*********************************************

#if defined(__Product_PIC32MX2_Receiver__)
extern UINT16  TIMER_err_1s;
extern UINT16  TIMER_Sensor_open_1s;
extern UINT16  TIMER_Sensor_close_1s;
extern UINT8 FLAG_open_Sensor;
extern UINT8 FLAG_close_Sensor;
extern UINT8 FLAG_HA_ERR_Sensor;
extern UINT8 FLAG_HA_ERR_bit;
extern UINT16 TIMER_Semi_open;
extern UINT16 TIMER_Semi_close;
extern UINT8 FLAG__Semi_open_T;
extern UINT8 FLAG__Semi_close_T;

extern UINT16  TIME_auto_out;
extern UINT16  TIME_auto_close;
extern UINT8 FG_auto_out;
extern UINT8 HA_Status_buf;
extern UINT8 DIP_switch_data_bak;
extern UINT8 DIP_switch_data;
extern UINT8 FLAG_DIP_switch;
extern UINT8 TIME_DIP_switch;
extern UINT8 FLAG_POER_on;

extern UINT8 FG_OUT_OPEN_CLOSE;    //2015.3.23修改
extern UINT8 TIME_OUT_OPEN_CLOSE;
extern UINT16 TIME_Login_EXIT_Button;
extern UINT16 TIME_Receiver_LED_OUT;
extern UINT16 time_Login_exit_256;
extern UINT16 Manual_override_TIMER;
extern UINT8 FG_auto_manual_mode;
extern UINT8 FG_First_auto;
extern UINT8 FG_auto_open_time;
#endif

#if defined(__Product_PIC32MX2_WIFI__)
    //extern UINT8  UART1_DATA[106];
    //extern UINT8  UART_DATA_buffer[106];
    extern UINT8  UART1_DATA[493];
    extern UINT8  UART_DATA_buffer[493];
    //extern UINT8 WIFI_alarm_data[200][10];
    extern UINT8 WIFI_alarm_data[12][103];
    extern UINT8 WIFI_alarm_data_planning[103];
    extern UINT8 WIFI_alarm_data_PCS;
    extern UINT8 Emial_time_data[10][7];
    extern UINT8 Emial_time_data_PCS;
    extern UINT8 WIFI_alarm_Hours_Minutes[2];
    extern UINT8 AUTO_SEND_DATA[200][4];
    extern UINT8 AUTO_SEND_DATA_pcs;
    extern UINT16 TIME_alarm_AUTO;
    extern UINT8 AUTO_HA_Inquiry;
    extern UINT8 HA_uart[1200];
    //extern UINT8 HA_uart_ios[45];
    extern UINT8 HA_uart_ios[49];
    extern UINT32 EMIAL_id_data[35];
    extern UINT32 EMIAL_id_data_chek;
    extern UINT8  EMIAL_id_HA[35];
    extern UINT8  EMIAL_id_PCS;
    extern UINT8  FLAG_email_send;
    extern UINT16 TIME_email_send;
    extern UINT32 Email_check_ID[35];
    extern UINT8  Emial_check_Control[35];
    extern UINT32 SWITCH_DIP_id_data[35];
    extern UINT8  SWITCH_DIP_id_DIP[35];
    extern UINT8 SWITCH_DIP;
    extern UINT8 SWITCH_DIP_bak;
    extern UINT32 SWITCH_DIP_id_data_bak;
    extern UINT8  Sunrise_sunset_DATA[480];
    extern UINT8 SUN_ON_OFF_seat[3];
    extern UINT8 SUN_Weekdays_alarm;
    extern UINT8 HA_Change_send_email[3];
    extern UINT16 HA_Change_email_time;
    extern UINT8 HA_Change_email_Step;
    extern UINT8 FLAG_HA_Change_ERROR;
    extern UINT8 FLAG_Emial_time;
    extern UINT8 Emial_time_place;
    extern UINT16 HA_uart_Length;

    extern UINT8 FLAG_Write_Read_compare;
    extern UINT8 uart_Control_code;

    extern UINT32 APP_check_ID;              //2014.10.11修改
    extern UINT8  APP_check_Control;
    extern UINT8  APP_check_char;
    extern UINT16  DATA_SUN_time_get;

    extern UINT8 TIME_WIFI_LAN_SELECT;
    extern UINT8 FLAG_WIFI_LAN_SELECT;

    extern UINT16 INquiry_0x00;
    extern UINT8 FLAG_IDCheck_OK_0x00;

    extern UINT8 FG_WIFI_SWITCH_DIP;
    extern UINT16 TIME_APP_Inquiry_HA;

    extern UINT8 FG_send_Faile_again;      //2015.1.30追加修改自动某ID发送一次失败，追加再发送一次
    extern UINT8 FG_Second;      //2015.3.5追加修改自动某ID发送一次失败，追加再发送一次,第二次的标志位
    extern UINT32 TIME_one_hour;         //2015.1.30追加修改1小时查询一次HA状态
    extern UINT8 FG_mial_com_fail;   //在邮件中表示429MHz通讯失败，
    extern UINT8 FG_send_Faile_notice;  //2015.3.31追加修改 2次发送都失败，SIG绿色LED 1Hz通知
    extern UINT16 TIME_send_Faile_notice; //2015.3.31追加修改   1Hz LED通知

    extern UINT8 Emial_Cache_HA;   //Start       //2015.4.1修正3 由于APP查询受信器HA状态需要很长的时间，所以追加指令查询缓存在通信机里面的HA状态
    extern UINT8 Emial_Cache_SWITCH;   
    extern UINT8 HA_Cache_SWITCH_DIP[35];   
    extern UINT8 HA_Cache_ha[35];   
    extern UINT32 HA_Cache_IDdata[35];   
    extern UINT8 HA_Cache_SWITCH_DIP_bak;   
    extern UINT8 HA_Cache_ha_bak;   
    extern UINT8 HA_Cache_ha_1Hz[35];  
    extern UINT8 HA_Cache_ha_1Hz_bak;   //end
    extern UINT8 FG_HA_Inquiry_NO_again_send;

    extern UINT16 time_APP_Start_up;   //2015.04.27修正
    extern UINT8 TIME_ID_Login_delay; //20150430 japan修改1
    extern uni_rom_id ID_data_uart_CMD0101_01;   //20150501 JAPAN追加

                                      //以下2015.08.21追加
    extern UINT8 ID_DATA_To[32];
    extern UINT8 ID_DATA_Tc[32];
    extern uni_rom_id ID_data_uart_CMD0111;
    extern UINT8 ID_DATA_To_or_Tc_place;
    extern UINT8  DATA_Packet_soft_ver;
    extern UINT8 CMD0102_To_or_Tc_place;
    extern UINT8 CMD0102_To_or_Tc_HA;
    extern UINT8 FLAG_IDcheck_CMD0102_HA;
    extern UINT8 HA_uart_send_APP_number;
#endif


extern void VHF_GPIO_INIT(void);		// CPU端口设置
extern void Delayus(UINT16 timer);
extern void Delay100us(unsigned int timer);
extern void RF_test_mode(void );

//以下是IO 方向定义
#if defined(__Product_PIC32MX2_Receiver__)
    /* ADF7021 register interface */
    #define ADF7021_SCLK_IO               TRISCbits.TRISC4   // Output
    #define ADF7021_SDATA_IO              TRISBbits.TRISB7   // Output
    #define ADF7021_SLE_IO                TRISBbits.TRISB8  // Output
    #define ADF7021_SREAD_IO              TRISCbits.TRISC5 // Input

    /* Other ADF7021 connections */
    #define ADF7021_CE_IO                 TRISBbits.TRISB9 // Output
    #define ADF7021_MUXOUT_IO             TRISAbits.TRISA4 // Input

    /* GPIO access to ADF7021 data lines */
    #define ADF7021_DATA_CLK_IO           TRISCbits.TRISC3 // Input
    #define ADF7021_DATA_IO               TRISAbits.TRISA9 // Output

    #define ADF7021_CLKOUT_IO             TRISBbits.TRISB4 // Input
    #define ADF7021_INT_LOCK_IO           TRISAbits.TRISA8 // Input

   #define	SDAIO                   TRISCbits.TRISC2 // Input AND output
   #define	SCLIO                   TRISCbits.TRISC1 // output


    /* 受信机使用的IO*/
    #define  HA_L_signal_IO        TRISAbits.TRISA7 // Input   HA 下限信号   高电平有效
    #define  HA_ERR_signal_IO      TRISBbits.TRISB15 // Input   HA 异常信号  高电平有效
    #define  HA_Sensor_signal_IO   TRISAbits.TRISA10 // Input   HA 传感器信号  高电平有效
    #define  Receiver_Login_IO     TRISCbits.TRISC7 // Input   受信机登录键   低电平有效
    #define  Receiver_Buzzer_IO    TRISBbits.TRISB13 // Output   受信机蜂鸣器  高电平有效
    #define  Receiver_LED_OUT_IO   TRISCbits.TRISC6 // Output   受信机继电器动作输出  低电平有效
    #define  Receiver_LED_TX_IO    TRISCbits.TRISC9 // Output   受信机送信指示  低电平有效
    #define  Receiver_LED_RX_IO    TRISCbits.TRISC8 // Output   受信机受信指示  低电平有效
//    #define  Receiver_LED_TX_IO    TRISCbits.TRISC8 // Output   受信机送信指示  低电平有效
//    #define  Receiver_LED_RX_IO    TRISCbits.TRISC9 // Output   受信机受信指示  低电平有效
    #define  Receiver_OUT_OPEN_IO    TRISAbits.TRISA1  // Output 受信机继电器OPEN  高电平有效
    #define  Receiver_OUT_CLOSE_IO   TRISBbits.TRISB2  // Output 受信机继电器close 高电平有效
    #define  Receiver_OUT_STOP_IO    TRISCbits.TRISC0  // Output 受信机继电器STOP  高电平有效
    #define  Receiver_OUT_VENT_IO    TRISBbits.TRISB3  // Output 受信机继电器STOP  高电平有效

    #define  Receiver_test_IO    TRISBbits.TRISB10  // INPUT 受信机测试脚  高电平有效
    #define  DIP_switch1_IO         TRISBbits.TRISB11 // Input   DIP_switch1  低电平有效
    #define  DIP_switch2_IO         TRISBbits.TRISB5 // Input   DIP_switch2  低电平有效
    #define  DIP_switch3_IO         TRISAbits.TRISA0 // Input   DIP_switch3  低电平有效
#endif
#if defined(__Product_PIC32MX2_WIFI__)
    /* ADF7021 register interface */
    #define ADF7021_SCLK_IO               TRISCbits.TRISC4   // Output
    #define ADF7021_SDATA_IO              TRISBbits.TRISB7   // Output
    #define ADF7021_SLE_IO                TRISBbits.TRISB8  // Output
    #define ADF7021_SREAD_IO              TRISCbits.TRISC5 // Input

    /* Other ADF7021 connections */
    #define ADF7021_CE_IO                 TRISBbits.TRISB9 // Output
    #define ADF7021_MUXOUT_IO             TRISAbits.TRISA4 // Input

    /* GPIO access to ADF7021 data lines */
    #define ADF7021_DATA_CLK_IO           TRISCbits.TRISC3 // Input
    #define ADF7021_DATA_IO               TRISAbits.TRISA9 // Output

    //#define ADF7021_CLKOUT_IO             TRISBbits.TRISB4 // Input
    #define ADF7021_INT_LOCK_IO           TRISAbits.TRISA8 // Input

   #define	SDAIO                   TRISCbits.TRISC2 // Input AND output
   #define	SCLIO                   TRISCbits.TRISC1 // output

   #define	PCF8563_INT_IO          TRISCbits.TRISC0 // Input    追加定时OPEN CLOSE

    /* wifi集中通讯机使用的IO*/
    #define  WIFI_L_Login_IO      TRISAbits.TRISA7 // Input   wifi集中通讯机登录键   低电平有效
    #define  WIFI_USBLogin_IO     TRISCbits.TRISC7 // Input   wifi集中通讯机USB升级键   低电平有效
    #define  WIFI_USBOC_IO        TRISBbits.TRISB3 // Input   wifi集中USB保护监测   低电平有效
//    #define  WIFI_LED_RX_IO       TRISCbits.TRISC8 // Input   wifi集中通讯机受信指示 低电平有效
//    #define  WIFI_LED_TX_IO       TRISCbits.TRISC9 // Input   wifi集中通讯机送信指示 低电平有效
    #define  WIFI_LED_RX_IO       TRISCbits.TRISC9 // Input   wifi集中通讯机受信指示 低电平有效
    #define  WIFI_LED_TX_IO       TRISCbits.TRISC8 // Input   wifi集中通讯机送信指示 低电平有效

    #define  WIFI_LAN_SELECT_IO   TRISCbits.TRISC6 // Input   有线LAN、WIFI切换
    #define  WIFI_POWER_IO        TRISAbits.TRISA10 // output  WIFI电源  低电平有效
    #define  LAN_POWER_IO         TRISBbits.TRISB13 // output  LAN电源  低电平有效

    #define WIFI_test_IO          TRISBbits.TRISB4  // INPUT WIFI测试脚  低电平有效
#endif



//以下是IO data定义
#if defined(__Product_PIC32MX2_Receiver__)
    /* ADF7021 register interface */
    #define ADF7021_SCLK               LATCbits.LATC4     // Output
    #define ADF7021_SDATA              LATBbits.LATB7     // Output
    #define ADF7021_SLE                LATBbits.LATB8     // Output
    #define ADF7021_SREAD              PORTCbits.RC5     // Input

    /* Other ADF7021 connections */
    #define ADF7021_CE                 LATBbits.LATB9     // Output
    #define ADF7021_MUXOUT             PORTAbits.RA4    // Input

    /* GPIO access to ADF7021 data lines */
    #define ADF7021_DATA_CLK          PORTCbits.RC3     // Input
    #define ADF7021_DATA_tx           LATAbits.LATA9     // Output
    #define ADF7021_DATA_rx           PORTAbits.RA9     // Input

    //#define ADF7021_CLKOUT             PORTBbits.RB4 // Input
    #define ADF7021_CLKOUT             LATBbits.LATB4 // Output  测试
    #define ADF7021_INT_LOCK           PORTAbits.RA8 // Input

    #define	SDA                     LATCbits.LATC2     // Output
    #define	SDA_I                   PORTCbits.RC2 // Input
    #define	SCL                     LATCbits.LATC1     // Output

     /* 受信机使用的IO*/
    #define  HA_L_signal        PORTAbits.RA7     // Input   HA 下限信号   高电平有效
    #define  HA_ERR_signal      PORTBbits.RB15   // Input   HA 异常信号  高电平有效
    #define  HA_Sensor_signal   PORTAbits.RA10   // Input   HA 传感器信号  高电平有效
    #define  Receiver_Login     PORTCbits.RC7   // Input   受信机登录键   低电平有效
    #define  Receiver_Buzzer    LATBbits.LATB13  // Output   受信机蜂鸣器  高电平有效
    #define  Receiver_LED_OUT   LATCbits.LATC6  // Output   受信机继电器动作输出  低电平有效
    #define  Receiver_LED_TX    LATCbits.LATC9 // Output   受信机送信指示  低电平有效
    #define  Receiver_LED_RX    LATCbits.LATC8  // Output   受信机受信指示  低电平有效
//    #define  Receiver_LED_TX    LATCbits.LATC8 // Output   受信机送信指示  低电平有效
//    #define  Receiver_LED_RX    LATCbits.LATC9  // Output   受信机受信指示  低电平有效
    #define  Receiver_OUT_OPEN    LATAbits.LATA1  // Output   受信机继电器OPEN  高电平有效
    #define  Receiver_OUT_CLOSE   LATBbits.LATB2  // Output   受信机继电器close  高电平有效
    #define  Receiver_OUT_STOP    LATCbits.LATC0  // Output   受信机继电器stop  高电平有效
    #define  Receiver_OUT_VENT    LATBbits.LATB3   // Output 受信机继电器STOP  高电平有效

    #define  Receiver_test    PORTBbits.RB10   // Input 受信机测试脚  高电平有效
    #define  DIP_switch1         PORTBbits.RB11 // Input   DIP_switch1  低电平有效
    #define  DIP_switch2         PORTBbits.RB5 // Input   DIP_switch2  低电平有效
    #define  DIP_switch3         PORTAbits.RA0 // Input   DIP_switch3  低电平有效
//特别注意，在追加I/O时，用到PA口，请不要用LATAbits寄存器，请使用LATACLR，LATASET。
#endif
#if defined(__Product_PIC32MX2_WIFI__)
    /* ADF7021 register interface */
    #define ADF7021_SCLK               LATCbits.LATC4     // Output
    #define ADF7021_SDATA              LATBbits.LATB7     // Output
    #define ADF7021_SLE                LATBbits.LATB8     // Output
    #define ADF7021_SREAD              PORTCbits.RC5     // Input

    /* Other ADF7021 connections */
    #define ADF7021_CE                 LATBbits.LATB9     // Output
    #define ADF7021_MUXOUT             PORTAbits.RA4    // Input

    /* GPIO access to ADF7021 data lines */
    #define ADF7021_DATA_CLK          PORTCbits.RC3     // Input
    #define ADF7021_DATA_tx           LATAbits.LATA9     // Output
    #define ADF7021_DATA_rx           PORTAbits.RA9     // Input

    //#define ADF7021_CLKOUT             PORTBbits.RB4 // Input
    #define ADF7021_INT_LOCK           PORTAbits.RA8 // Input

    #define	SDA                     LATCbits.LATC2     // Output
    #define	SDA_I                   PORTCbits.RC2 // Input
    #define	SCL                     LATCbits.LATC1     // Output

   #define	PCF8563_INT           PORTCbits.RC0 // Input    追加定时OPEN CLOSE

    /* wifi集中通讯机使用的IO*/
    #define  WIFI_L_Login      PORTAbits.RA7 // Input   wifi集中通讯机登录键   低电平有效
    #define  WIFI_USBLogin     PORTCbits.RC7 // Input   wifi集中通讯机USB升级键   低电平有效
    #define  WIFI_USBOC        PORTBbits.RB3 // Input   wifi集中USB保护监测   低电平有效
//    #define  WIFI_LED_RX       LATCbits.LATC8 // Output   wifi集中通讯机受信指示 低电平有效
//    #define  WIFI_LED_TX       LATCbits.LATC9 // Output   wifi集中通讯机送信指示 低电平有效
    #define  WIFI_LED_RX       LATCbits.LATC9 // Output   wifi集中通讯机受信指示 低电平有效
    #define  WIFI_LED_TX       LATCbits.LATC8 // Output   wifi集中通讯机送信指示 低电平有效

    #define  WIFI_LAN_SELECT  PORTCbits.RC6 // Input   有线LAN、WIFI切换
    #define  WIFI_POWER       LATAbits.LATA10  // output  WIFI电源  低电平有效
    #define  LAN_POWER        LATBbits.LATB13  // output  LAN电源  低电平有效

    #define  WIFI_test        PORTBbits.RB4   // Input WIFI测试脚  低电平有效
//特别注意，在追加I/O时，用到PA口，请不要用LATAbits寄存器，请使用LATACLR，LATASET。

#endif





////以下是IO 方向定义
//#if defined(__Product_PIC32MX2_Receiver__)
//    /* ADF7021 register interface */
//    #define ADF7021_SCLK_IO               TRISCbits.TRISC3   // Output
//    #define ADF7021_SDATA_IO              TRISAbits.TRISA4   // Output
//    #define ADF7021_SLE_IO                TRISBbits.TRISB4  // Output
//    #define ADF7021_SREAD_IO              TRISAbits.TRISA9 // Input
//
//    /* Other ADF7021 connections */
//    #define ADF7021_CE_IO                 TRISAbits.TRISA8 // Output
//    #define ADF7021_MUXOUT_IO             TRISBbits.TRISB7 // Input
//
//    /* GPIO access to ADF7021 data lines */
//    #define ADF7021_DATA_CLK_IO           TRISCbits.TRISC5 // Input
//    #define ADF7021_DATA_IO               TRISCbits.TRISC4 // Output
//
//    #define ADF7021_CLKOUT_IO             TRISBbits.TRISB8 // Input
//    #define ADF7021_INT_LOCK_IO           TRISBbits.TRISB9 // Input
//
//   #define	SDAIO                   TRISCbits.TRISC2 // Input AND output
//   #define	SCLIO                   TRISCbits.TRISC1 // output
//
//
//    /* 受信机使用的IO*/
//    #define  HA_L_signal_IO        TRISAbits.TRISA7 // Input   HA 下限信号   高电平有效
//    #define  HA_ERR_signal_IO      TRISBbits.TRISB15 // Input   HA 异常信号  高电平有效
//    #define  HA_Sensor_signal_IO   TRISAbits.TRISA10 // Input   HA 传感器信号  高电平有效
//    #define  Receiver_Login_IO     TRISCbits.TRISC7 // Input   受信机登录键   低电平有效
//    #define  Receiver_Buzzer_IO    TRISBbits.TRISB13 // Output   受信机蜂鸣器  高电平有效
//    #define  Receiver_LED_OUT_IO   TRISCbits.TRISC6 // Output   受信机继电器动作输出  低电平有效
//    #define  Receiver_LED_TX_IO    TRISCbits.TRISC9 // Output   受信机送信指示  低电平有效
//    #define  Receiver_LED_RX_IO    TRISCbits.TRISC8 // Output   受信机受信指示  低电平有效
//    #define  Receiver_OUT_OPEN_IO    TRISAbits.TRISA1  // Output 受信机继电器OPEN  高电平有效
//    #define  Receiver_OUT_CLOSE_IO   TRISBbits.TRISB2  // Output 受信机继电器close 高电平有效
//    #define  Receiver_OUT_STOP_IO    TRISCbits.TRISC0  // Output 受信机继电器STOP  高电平有效
//#endif
//#if defined(__Product_PIC32MX2_WIFI__)
//    /* ADF7021 register interface */
//    #define ADF7021_SCLK_IO               TRISCbits.TRISC3   // Output
//    #define ADF7021_SDATA_IO              TRISAbits.TRISA4   // Output
//    #define ADF7021_SLE_IO                TRISBbits.TRISB4  // Output
//    #define ADF7021_SREAD_IO              TRISAbits.TRISA9 // Input
//
//    /* Other ADF7021 connections */
//    #define ADF7021_CE_IO                 TRISAbits.TRISA8 // Output
//    #define ADF7021_MUXOUT_IO             TRISBbits.TRISB7 // Input
//
//    /* GPIO access to ADF7021 data lines */
//    #define ADF7021_DATA_CLK_IO           TRISCbits.TRISC5 // Input
//    #define ADF7021_DATA_IO               TRISCbits.TRISC4 // Output
//
//    #define ADF7021_CLKOUT_IO             TRISBbits.TRISB8 // Input
//    #define ADF7021_INT_LOCK_IO           TRISBbits.TRISB9 // Input
//
//   #define	SDAIO                   TRISCbits.TRISC2 // Input AND output
//   #define	SCLIO                   TRISCbits.TRISC1 // output
//
//    /* wifi集中通讯机使用的IO*/
//    #define  WIFI_L_Login_IO      TRISAbits.TRISA7 // Input   wifi集中通讯机登录键   低电平有效
//    #define  WIFI_USBLogin_IO     TRISCbits.TRISC7 // Input   wifi集中通讯机USB升级键   低电平有效
//    #define  WIFI_Useless0_IO      TRISAbits.TRISA10 // Input  样机板后面没有用  测试版作为SW2
//    #define  WIFI_Useless1_IO     TRISCbits.TRISC6 // output  样机板后面没有用    测试版作为LED2
//    #define  WIFI_USBOC_IO        TRISBbits.TRISB3 // Input   wifi集中USB保护监测   低电平有效
//    #define  WIFI_LED_RX_IO       TRISCbits.TRISC8 // Input   wifi集中通讯机受信指示 低电平有效
//    #define  WIFI_LED_TX_IO       TRISCbits.TRISC9 // Input   wifi集中通讯机送信指示 低电平有效
//#endif
//
//
//
////以下是IO data定义
//#if defined(__Product_PIC32MX2_Receiver__)
//    /* ADF7021 register interface */
//    #define ADF7021_SCLK               LATCbits.LATC3     // Output
//    #define ADF7021_SDATA              LATAbits.LATA4     // Output
//    #define ADF7021_SLE                LATBbits.LATB4     // Output
//    #define ADF7021_SREAD              PORTAbits.RA9     // Input
//
//    /* Other ADF7021 connections */
//    #define ADF7021_CE                 LATAbits.LATA8     // Output
//    #define ADF7021_MUXOUT             PORTBbits.RB7    // Input
//
//    /* GPIO access to ADF7021 data lines */
//    #define ADF7021_DATA_CLK          PORTCbits.RC5     // Input
//    #define ADF7021_DATA_tx           LATCbits.LATC4     // Output
//    #define ADF7021_DATA_rx           PORTCbits.RC4     // Input
//
//    #define ADF7021_CLKOUT             PORTBbits.RB8 // Input
//    #define ADF7021_INT_LOCK           PORTBbits.RB9 // Input
//
//    #define	SDA                     LATCbits.LATC2     // Output
//    #define	SDA_I                   PORTCbits.RC2 // Input
//    #define	SCL                     LATCbits.LATC1     // Output
//
//     /* 受信机使用的IO*/
//    #define  HA_L_signal        PORTAbits.RA7     // Input   HA 下限信号   高电平有效
//    #define  HA_ERR_signal      PORTBbits.RB15   // Input   HA 异常信号  高电平有效
//    #define  HA_Sensor_signal   PORTAbits.RA10   // Input   HA 传感器信号  高电平有效
//    #define  Receiver_Login     PORTCbits.RC7   // Input   受信机登录键   低电平有效
//    #define  Receiver_Buzzer    LATBbits.LATB13  // Output   受信机蜂鸣器  高电平有效
//    #define  Receiver_LED_OUT   LATCbits.LATC6  // Output   受信机继电器动作输出  低电平有效
//    #define  Receiver_LED_TX    LATCbits.LATC9 // Output   受信机送信指示  低电平有效
//    #define  Receiver_LED_RX    LATCbits.LATC8  // Output   受信机受信指示  低电平有效
//    #define  Receiver_OUT_OPEN    LATAbits.LATA1  // Output   受信机继电器OPEN  高电平有效
//    #define  Receiver_OUT_CLOSE   LATBbits.LATB2  // Output   受信机继电器close  高电平有效
//    #define  Receiver_OUT_STOP    LATCbits.LATC0  // Output   受信机继电器stop  高电平有效
//#endif
//#if defined(__Product_PIC32MX2_WIFI__)
//    /* ADF7021 register interface */
//    #define ADF7021_SCLK               LATCbits.LATC3     // Output
//    #define ADF7021_SDATA              LATAbits.LATA4     // Output
//    #define ADF7021_SLE                LATBbits.LATB4     // Output
//    #define ADF7021_SREAD              PORTAbits.RA9     // Input
//
//    /* Other ADF7021 connections */
//    #define ADF7021_CE                 LATAbits.LATA8     // Output
//    #define ADF7021_MUXOUT             PORTBbits.RB7    // Input
//
//    /* GPIO access to ADF7021 data lines */
//    #define ADF7021_DATA_CLK          PORTCbits.RC5     // Input
//    #define ADF7021_DATA_tx           LATCbits.LATC4     // Output
//    #define ADF7021_DATA_rx           PORTCbits.RC4     // Input
//
//    #define ADF7021_CLKOUT             PORTBbits.RB8 // Input
//    #define ADF7021_INT_LOCK           PORTBbits.RB9 // Input
//
//    #define	SDA                     LATCbits.LATC2     // Output
//    #define	SDA_I                   PORTCbits.RC2 // Input
//    #define	SCL                     LATCbits.LATC1     // Output
//
//    /* wifi集中通讯机使用的IO*/
//    #define  WIFI_L_Login      PORTAbits.RA7 // Input   wifi集中通讯机登录键   低电平有效
//    #define  WIFI_USBLogin     PORTCbits.RC7 // Input   wifi集中通讯机USB升级键   低电平有效
//    #define  WIFI_Useless0     PORTAbits.RA10 // Input  样机板后面没有用  测试版作为SW2
//    #define  WIFI_Useless1     PORTCbits.RC6 // output  样机板后面没有用    测试版作为LED2
//    #define  WIFI_USBOC        PORTBbits.RB3 // Input   wifi集中USB保护监测   低电平有效
//    #define  WIFI_LED_RX       LATCbits.LATC8 // Output   wifi集中通讯机受信指示 低电平有效
//    #define  WIFI_LED_TX       LATCbits.LATC9 // Output   wifi集中通讯机送信指示 低电平有效
//#endif

