
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

//UINT32 EEPROM_Receiver_ID=2000307;//13040292;//13040451;//2000307;//13186823;

FLAG FLAG_APP;
FLAG FLAG_PORT;
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
UINT32 DATA_Packet_ID_buf=0;
UINT8  DATA_Packet_Control_buf=0;
UINT8  DATA_Packet_Control_0=0;
UINT8  DATA_Packet_Control_err=0;
UINT8  Control_bak=0;
UINT16  TIMER1s=0;
UINT16  TIMER300ms=0;
UINT16  TIMER18ms=0;
UINT16   TIMER250ms_STOP=0;
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
UINT16  UART_DATA_cnt=0;

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

UINT8 HA_uart_app[18]={0xBB,0x00,0x07,0x00,0x00,0x00,0x08,0x00,
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

UINT16 FLAG_all_Erase_time=0;
UINT8 TIME_EMC=0;   //静电测试
UINT8 read_TIMER_Semi_open=0;

//*********以下为测试代码所用变量****************
UINT8 Tx_Rx_mode=0;
FLAG FLAG_test;
#if defined(__Product_PIC32MX2_Receiver__)
                            /****说明 REG1     REG3         REG0      REG2       REG4      REGf******/
UINT32 RF_SET_TX_carrier_test[6]={0x031B5011,0x29915CD3,0x01575710,0x00566882,0x00289A14,0x0000010F};     //天线是内部天线
                            /****说明 REG1     REG3         REG0      REG2       REG4      REGf******/
UINT32 RF_SET_TX_1010pattern[6]={0x031B5011,0x29915CD3,0x01575710,0x00566882,0x00289A14,0x0000040F};    //天线是内部天线
#endif
#if defined(__Product_PIC32MX2_WIFI__)
                            /****说明 REG1     REG3         REG0      REG2       REG4      REGf******/
//UINT32 RF_SET_TX_carrier_test[6]={0x031B5011,0x29915CD3,0x01575710,0x00566882,0x00289A14,0x0000010F};     //天线是内部天线
UINT32 RF_SET_TX_carrier_test[6]={0x031B5011,0x29915CD3,0x01575710,0x00571882,0x00289A14,0x0000010F};       //天线是外置天线
                            /****说明 REG1     REG3         REG0      REG2       REG4      REGf******/
//UINT32 RF_SET_TX_1010pattern[6]={0x031B5011,0x29915CD3,0x01575710,0x00566882,0x00289A14,0x0000040F};    //天线是内部天线
UINT32 RF_SET_TX_1010pattern[6]={0x031B5011,0x29915CD3,0x01575710,0x00571882,0x00289A14,0x0000040F};      //天线是外置天线
#endif
                    /****说明 REG1      REG2        REG3        REG0    REG4    REGA******/
UINT32 RF_SET_RX_test[6]={0x031B5011,0x00D00882,0x29920893,0x0954C7B0,0x8024E294,0x049668EA};
UINT16 X_COUNT = 0;
UINT16 X_ERR  = 0 ;//记录错误的个数
UINT8 X_HIS=0; //历史记录   误码率测试用

UINT16 TIME_Fine_Calibration=0;   //窄带下中频滤波器100KHz精校
//*********************************************

#if defined(__Product_PIC32MX2_Receiver__)
UINT16  TIMER_err_1s=0;
UINT16  TIMER_Sensor_open_1s=0;
UINT16  TIMER_Sensor_close_1s=0;
UINT8 FLAG_open_Sensor=0;
UINT8 FLAG_close_Sensor=0;
UINT8 FLAG_HA_ERR_Sensor=0;
UINT8 FLAG_HA_ERR_bit=0;
UINT16 TIMER_Semi_open;
UINT16 TIMER_Semi_close;
UINT8 FLAG__Semi_open_T=0;
UINT8 FLAG__Semi_close_T=0;

UINT16  TIME_auto_out=0;
UINT16  TIME_auto_close=0;
UINT8 FG_auto_out=0;
UINT8 HA_Status_buf=0;
UINT8 DIP_switch_data_bak=0;
UINT8 DIP_switch_data=0;
UINT8 FLAG_DIP_switch=0;
UINT8 TIME_DIP_switch=0;
UINT8 FLAG_POER_on=0;

UINT8 FG_OUT_OPEN_CLOSE=0;    //2015.3.23修改
UINT8 TIME_OUT_OPEN_CLOSE=0;
UINT16 TIME_Login_EXIT_Button=0;
UINT16 TIME_Receiver_LED_OUT=0;
UINT16 time_Login_exit_256=0;
UINT16 Manual_override_TIMER=0;
UINT8 FG_auto_manual_mode=0;
UINT8 FG_First_auto=0;
UINT8 FG_auto_open_time=0;
#endif

#if defined(__Product_PIC32MX2_WIFI__)
    //UINT8  UART1_DATA[106]={0};
    //UINT8  UART_DATA_buffer[106]={0};
    UINT8  UART1_DATA[493]={0};
    UINT8  UART_DATA_buffer[493]={0};
    //UINT8 WIFI_alarm_data[200][10]={0};
    UINT8 WIFI_alarm_data[12][103]={0};
    UINT8 WIFI_alarm_data_planning[103];
    UINT8 WIFI_alarm_data_PCS=0;
    UINT8 Emial_time_data[10][7]={0};
    UINT8 Emial_time_data_PCS=0;
    UINT8 WIFI_alarm_Hours_Minutes[2]={0xFF,0xFF};
    UINT8 AUTO_SEND_DATA[200][4]={0};
    UINT8 AUTO_SEND_DATA_pcs=0;
    UINT16 TIME_alarm_AUTO=0;
    UINT8 AUTO_HA_Inquiry=0;
//    UINT8 HA_uart[1200]={0xBB,0x00,0x20,0x00,0x00,0x00,0x23,0x00,          //头
//                       0x32,0x30,0x00,0x00,       //年
//                       0x2E,0x00,0x00,          //月
//                       0x2E,0x00,0x00,         //日
//                       0x20,0x00,0x00,        //小时
//                       0x3A,0x00,0x00,       //分钟
//                       0x00,      //标题结束符
//                       //以下邮件内容
//                       0x49,0x44,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,  //ID=xxxxxxxx
//                       0x00,0x00,0x00,0x00,0x00,0x0d, //   如：OPEN
//                       0x00,       //邮件内容结束符
//                       0x00,0x00   //校验码
//                       };
    /***********
    UINT8 HA_uart[1200]={0xBB,0x00,0x20,0x00,0x00,0x00,          //头
                   0x23,0x00,                //长度
                   0x00,0x00,                //接收邮件地址编号
                   0x32,0x30,0x00,0x00,       //年
                   0x2E,0x00,0x00,          //月
                   0x2E,0x00,0x00,         //日
                   0x20,0x00,0x00,        //小时
                   0x3A,0x00,0x00,       //分钟
                   0x00,      //标题结束符
                   //以下邮件内容
                   //0x42,0x58,0x0D,    //BX
                   //0x68,0x74,0x74,0x70,0x3A,0x2F,0x2F,0x77,0x77,0x77,0x2E,0x62,0x75,0x6E,0x6B,0x61,0x2D,0x73,0x2E,0x63,0x6F,0x2E,0x6A,0x70,0x2F,//http://www.bunka-s.co.jp/
                   0x73,0x65,0x6c,0x65,0x63,0x6f,0x6e,0x6e,0x65,0x63,0x74,0x3A,0x2F,0x2F,0x77,0x77,0x77,0x2E,0x62,0x75,0x6E,0x6B,0x61,0x2D,0x73,0x2E,0x63,0x6F,0x2E,0x6A,0x70,0x2F,//seleconnect://www.bunka-s.co.jp/
                   0x73,0x68,0x75,0x74,0x74,0x65,0x72,0x69,0x6E,0x66,0x6F,0x3F,0x73,0x74,0x61,0x74,0x75,0x73,0x3D,//shutterinfo?status=
                   0x49,0x44,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,  //ID=xxxxxxxx
                   0x00,0x00,0x00,0x00,0x00,0x0d, //   如：OPEN
                   0x00,       //邮件内容结束符
                   0x00,0x00   //校验码
                   };
     ****************/


//    UINT8 HA_uart[1200]={0xBB,0x00,0x20,0x00,0x00,0x00,          //头
//                   0x23,0x00,                //长度
//                   0x00,0x00,                //接收邮件地址编号
//                   0x32,0x30,0x00,0x00,       //年
//                   0x2E,0x00,0x00,          //月
//                   0x2E,0x00,0x00,         //日
//                   0x20,0x00,0x00,        //小时
//                   0x3A,0x00,0x00,       //分钟
//                   0x00,      //标题结束符     此次为止有27个长度
//                   //以下邮件内容
//                   65,110,100,114,111,105,100,13,104,116,116,112,58,47,47,98,120,45,109,97,105,108,0x2F,//  Android回车http://bx-mail/       该行长度为23
//                   0x73,0x68,0x75,0x74,0x74,0x65,0x72,0x69,0x6E,0x66,0x6F,0x3F,0x73,0x74,0x61,0x74,0x75,0x73,0x3D,//shutterinfo?status=     该行长度为19
//                   0x49,0x44,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,  //ID=xxxxxxxx
//                   0x00,0x00,0x00,0x00,0x00,0x0d, //   如：OPEN
//                   0x00,       //邮件内容结束符
//                   0x00,0x00   //校验码
//                   };
//    UINT8 HA_uart_ios[45]={
//                   105,79,83,13,0x73,0x65,0x6c,0x65,0x63,0x6f,0x6e,0x6e,0x65,0x63,0x74,58,47,47,98,120,45,109,97,105,108,0x2F,//  iOS回车seleconnect://bx-mail/       该行长度为26
//                   0x73,0x68,0x75,0x74,0x74,0x65,0x72,0x69,0x6E,0x66,0x6F,0x3F,0x73,0x74,0x61,0x74,0x75,0x73,0x3D//shutterinfo?status=     该行长度为19
//    };



    UINT8 HA_uart[1200]={0xBB,0x00,0x20,0x00,0x00,0x00,          //头
                   0x23,0x00,                //长度
                   0x00,0x00,                //接收邮件地址编号
                   0x32,0x30,0x00,0x00,       //年
                   0x2E,0x00,0x00,          //月
                   0x2E,0x00,0x00,         //日
                   0x20,0x00,0x00,        //小时
                   0x3A,0x00,0x00,       //分钟
                   0x00,      //标题结束符     此次为止有27个长度
                   //以下邮件内容
                   65,110,100,114,111,105,100,13,10,104,116,116,112,58,47,47,98,120,45,109,97,105,108,46,106,112,0x2F,//  Android回车http://bx-mail.jp/       该行长度为27
                   0x73,0x68,0x75,0x74,0x74,0x65,0x72,0x69,0x6E,0x66,0x6F,0x3F,0x73,0x74,0x61,0x74,0x75,0x73,0x3D,//shutterinfo?status=     该行长度为19
                   0x49,0x44,0x3D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,  //ID=xxxxxxxx
                   0x00,0x00,0x00,0x00,0x00,0x0d, //   如：OPEN
                   0x00,       //邮件内容结束符
                   0x00,0x00   //校验码
                   };
    UINT8 HA_uart_ios[49]={
                   105,79,83,13,10,0x73,0x65,0x6c,0x65,0x63,0x6f,0x6e,0x6e,0x65,0x63,0x74,58,47,47,98,120,45,109,97,105,108,46,106,112,0x2F,//  iOS回车seleconnect://bx-mail.jp/       该行长度为30
                   0x73,0x68,0x75,0x74,0x74,0x65,0x72,0x69,0x6E,0x66,0x6F,0x3F,0x73,0x74,0x61,0x74,0x75,0x73,0x3D//shutterinfo?status=     该行长度为19
    };

    UINT32 EMIAL_id_data[35];
    UINT32 EMIAL_id_data_chek;
    UINT8  EMIAL_id_HA[35];
    UINT8  EMIAL_id_PCS=0;
    UINT8  FLAG_email_send=0;
    UINT16 TIME_email_send=0;
    UINT32 Email_check_ID[35]={0x00};
    UINT8  Emial_check_Control[35]={0x00};
    UINT32 SWITCH_DIP_id_data[35];
    UINT8  SWITCH_DIP_id_DIP[35];
    UINT8 SWITCH_DIP=0;
    UINT8 SWITCH_DIP_bak=0;
    UINT32 SWITCH_DIP_id_data_bak=0;
//                                     //东北OPEN ,东北CLOSE, 关东OPEN ,关东CLOSE,关西OPEN ,关西CLOSE ,九州OPEN ,九州CLOSE,北海道OPEN,北海CLOSE,冲绳OPEN ,冲绳CLOSE,预备1OPEN ,预1CLOSE ,预备2OPEN,预2CLOSE ,预备3OPEN,预备3CLOSE,预备4OPEN,预4CLOSE ,
//    UINT8  Sunrise_sunset_DATA[480]={  0x07,0x00,0x16,0x55,0x06,0x58,0x17,0x06,0x07,0x13,0x17,0x25,0x07,0x31,0x17,0x47,0x07,0x11,0x16,0x41,0x07,0x28,0x18,0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //一月份
//                                       0x06,0x31,0x17,0x30,0x06,0x33,0x17,0x37,0x06,0x49,0x17,0x55,0x07,0x08,0x18,0x17,0x06,0x36,0x17,0x22,0x07,0x11,0x18,0x35,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //二月份
//                                       0x05,0x51,0x17,0x58,0x05,0x55,0x18,0x02,0x06,0x12,0x18,0x19,0x06,0x33,0x18,0x39,0x05,0x49,0x17,0x56,0x06,0x44,0x18,0x50,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //三月份
//                                       0x05,0x05,0x18,0x27,0x05,0x13,0x18,0x28,0x05,0x31,0x18,0x44,0x05,0x53,0x19,0x03,0x04,0x56,0x18,0x32,0x06,0x12,0x19,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //四月份
//                                       0x04,0x32,0x18,0x55,0x04,0x43,0x18,0x53,0x05,0x02,0x19,0x07,0x05,0x25,0x19,0x25,0x04,0x17,0x19,0x06,0x05,0x51,0x19,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //五月份
//                                       0x04,0x23,0x19,0x13,0x04,0x35,0x19,0x10,0x04,0x55,0x19,0x24,0x05,0x18,0x19,0x41,0x04,0x05,0x19,0x27,0x05,0x47,0x19,0x34,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //六月份
//                                       0x04,0x38,0x19,0x07,0x04,0x50,0x19,0x05,0x05,0x09,0x19,0x19,0x05,0x32,0x19,0x37,0x04,0x23,0x19,0x19,0x05,0x59,0x19,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //七月份
//                                       //0x04,0x38,0x19,0x07,0x09,0x20,0x17,0x25,0x05,0x09,0x19,0x19,0x05,0x32,0x19,0x37,0x04,0x23,0x19,0x19,0x05,0x59,0x19,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //七月份
//                                       0x05,0x05,0x18,0x34,0x05,0x13,0x18,0x35,0x05,0x32,0x18,0x51,0x05,0x54,0x19,0x10,0x04,0x55,0x18,0x40,0x06,0x14,0x19,0x12,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //八月份
//                                       0x05,0x32,0x17,0x48,0x05,0x37,0x17,0x52,0x05,0x54,0x18,0x09,0x06,0x15,0x18,0x29,0x05,0x29,0x17,0x47,0x06,0x27,0x18,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //九月份
//                                       0x06,0x00,0x17,0x03,0x06,0x01,0x17,0x10,0x06,0x17,0x17,0x28,0x06,0x37,0x17,0x50,0x06,0x03,0x16,0x55,0x06,0x41,0x18,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //十月份
//                                       0x06,0x32,0x16,0x31,0x06,0x31,0x16,0x42,0x06,0x46,0x17,0x01,0x07,0x04,0x17,0x23,0x06,0x43,0x16,0x17,0x07,0x01,0x17,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //十一月份
//                                       0x06,0x59,0x16,0x29,0x06,0x56,0x16,0x41,0x07,0x10,0x17,0x00,0x07,0x28,0x17,0x24,0x07,0x12,0x16,0x12,0x07,0x22,0x17,0x52,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00     //十二月份
//                                     };


                                     //东北OPEN ,东北CLOSE, 关东OPEN ,关东CLOSE,关西OPEN ,关西CLOSE ,九州OPEN ,九州CLOSE,北海道OPEN,北海CLOSE,冲绳OPEN ,冲绳CLOSE,中部OPEN ,中部CLOSE ,中四国OPEN,中四国CLO,预备3OPEN,预备3CLOSE,预备4OPEN,预4CLOSE ,
    UINT8  Sunrise_sunset_DATA[480]={  0x07,0x00,0x16,0x35,0x06,0x58,0x16,0x46,0x07,0x13,0x17,0x05,0x07,0x31,0x17,0x27,0x07,0x11,0x16,0x21,0x07,0x28,0x17,0x53,0x07,0x09,0x16,0x58,0x07,0x25,0x17,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //一月份
                                       0x06,0x31,0x17,0x10,0x06,0x33,0x17,0x17,0x06,0x49,0x17,0x35,0x07,0x08,0x17,0x57,0x06,0x36,0x17,0x02,0x07,0x11,0x18,0x15,0x06,0x43,0x17,0x29,0x07,0x00,0x17,0x48,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //二月份
                                       0x05,0x51,0x17,0x38,0x05,0x55,0x17,0x42,0x06,0x12,0x17,0x59,0x06,0x33,0x18,0x19,0x05,0x49,0x17,0x36,0x06,0x44,0x18,0x30,0x06,0x07,0x17,0x54,0x06,0x25,0x18,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //三月份
                                       0x05,0x05,0x18,0x07,0x05,0x13,0x18,0x08,0x05,0x31,0x18,0x24,0x05,0x53,0x18,0x43,0x04,0x56,0x18,0x12,0x06,0x12,0x18,0x45,0x05,0x25,0x18,0x19,0x05,0x43,0x18,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //四月份
                                       0x04,0x32,0x18,0x35,0x04,0x43,0x18,0x33,0x05,0x02,0x18,0x47,0x05,0x25,0x19,0x05,0x04,0x17,0x18,0x46,0x05,0x51,0x19,0x01,0x04,0x55,0x18,0x43,0x05,0x15,0x18,0x59,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //五月份
                                       0x04,0x23,0x18,0x53,0x04,0x35,0x18,0x50,0x04,0x55,0x19,0x04,0x05,0x18,0x19,0x21,0x04,0x05,0x19,0x07,0x05,0x47,0x19,0x14,0x04,0x48,0x19,0x00,0x05,0x08,0x19,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //六月份
                                       0x04,0x38,0x18,0x47,0x04,0x50,0x18,0x45,0x05,0x09,0x18,0x59,0x05,0x32,0x19,0x17,0x04,0x23,0x18,0x59,0x05,0x59,0x19,0x13,0x05,0x02,0x18,0x55,0x05,0x22,0x19,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //七月份
                                       0x05,0x05,0x18,0x14,0x05,0x13,0x18,0x15,0x05,0x32,0x18,0x31,0x05,0x54,0x18,0x50,0x04,0x55,0x18,0x20,0x06,0x14,0x18,0x52,0x05,0x25,0x18,0x26,0x05,0x44,0x18,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //八月份
                                       //0x05,0x05,0x18,0x14,0x16,0x29,0x16,0x31,0x05,0x32,0x18,0x31,0x05,0x54,0x18,0x50,0x04,0x55,0x18,0x20,0x06,0x14,0x18,0x52,0x05,0x25,0x18,0x26,0x05,0x44,0x18,0x42,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //八月份  test
                                       0x05,0x32,0x17,0x28,0x05,0x37,0x17,0x32,0x05,0x54,0x17,0x49,0x06,0x15,0x18,0x09,0x05,0x29,0x17,0x27,0x06,0x27,0x18,0x19,0x05,0x48,0x17,0x43,0x06,0x06,0x18,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //九月份
                                       //0x06,0x00,0x16,0x43,0x11,0x04,0x11,0x08,0x06,0x17,0x17,0x08,0x06,0x37,0x17,0x30,0x06,0x03,0x16,0x35,0x06,0x41,0x17,0x47,0x06,0x12,0x17,0x02,0x06,0x29,0x17,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //十月份  test
                                       0x06,0x00,0x16,0x43,0x06,0x01,0x16,0x50,0x06,0x17,0x17,0x08,0x06,0x37,0x17,0x30,0x06,0x03,0x16,0x35,0x06,0x41,0x17,0x47,0x06,0x12,0x17,0x02,0x06,0x29,0x17,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //十月份
                                       0x06,0x32,0x16,0x11,0x06,0x31,0x16,0x22,0x06,0x46,0x16,0x41,0x07,0x04,0x17,0x03,0x06,0x43,0x15,0x57,0x07,0x01,0x17,0x28,0x06,0x41,0x16,0x34,0x06,0x58,0x16,0x54,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     //十一月份
                                       0x06,0x59,0x16,0x09,0x06,0x56,0x16,0x21,0x07,0x10,0x16,0x40,0x07,0x28,0x17,0x04,0x07,0x12,0x15,0x52,0x07,0x22,0x17,0x32,0x07,0x06,0x16,0x33,0x07,0x22,0x16,0x53,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00     //十二月份
                                    };
    UINT8 SUN_ON_OFF_seat[3]={0x00,0x00,0x00};
    UINT8 SUN_Weekdays_alarm=0;
    UINT8 HA_Change_send_email[3]={0x00,0x00,0x00};
    UINT16 HA_Change_email_time=0;
    UINT8 HA_Change_email_Step=0;
    UINT8 FLAG_HA_Change_ERROR=0;
    UINT8 FLAG_Emial_time=0;
    UINT8 Emial_time_place=0;
    UINT16 HA_uart_Length=0;

    UINT8 FLAG_Write_Read_compare=0;


    UINT16 TIME_No_response=0;
    UINT8 FLAG_TIME_No_response=0;
    UINT8 uart_Control_code;

    UINT32 APP_check_ID=0;      //2014.10.11修改
    UINT8  APP_check_Control=0;
    UINT8  APP_check_char=0;
    UINT16  DATA_SUN_time_get=0;

    UINT8 TIME_WIFI_LAN_SELECT=0;
    UINT8 FLAG_WIFI_LAN_SELECT=0;

    UINT16 INquiry_0x00=0;
    UINT8 FLAG_IDCheck_OK_0x00=0;

    UINT8 FG_WIFI_SWITCH_DIP=0;
    UINT16 TIME_APP_Inquiry_HA=0;

    UINT8 FG_send_Faile_again=0;      //2015.1.30追加修改自动某ID发送一次失败，追加再发送一次
    UINT8 FG_Second=0;      //2015.3.5追加修改自动某ID发送一次失败，追加再发送一次,第二次的标志位
    UINT32 TIME_one_hour=0;         //2015.1.30追加修改1小时查询一次HA状态
    UINT8 FG_mial_com_fail=0;   //在邮件中表示429MHz通讯失败，
    UINT8 FG_send_Faile_notice=0;  //2015.3.31追加修改 2次发送都失败，SIG绿色LED 1Hz通知
    UINT16 TIME_send_Faile_notice=0;  //2015.3.31追加修改   1Hz LED通知

    UINT8 Emial_Cache_HA=0;   //Start        //2015.4.1修正3 由于APP查询受信器HA状态需要很长的时间，所以追加指令查询缓存在通信机里面的HA状态
    UINT8 Emial_Cache_SWITCH=0;   
    UINT8 HA_Cache_SWITCH_DIP[35];  
    UINT8 HA_Cache_ha[35];  
    UINT32 HA_Cache_IDdata[35];   
    UINT8 HA_Cache_SWITCH_DIP_bak=0;  
    UINT8 HA_Cache_ha_bak=0;  
    UINT8 HA_Cache_ha_1Hz[35];  
    UINT8 HA_Cache_ha_1Hz_bak=0;   //end
    UINT8 FG_HA_Inquiry_NO_again_send=0;

    UINT16 time_APP_Start_up=0;   //2015.04.27修正
    UINT8 TIME_ID_Login_delay=0; //20150430 japan修改1
    uni_rom_id ID_data_uart_CMD0101_01;   //20150501 JAPAN追加

                                  //以下2015.08.21追加
    UINT8 ID_DATA_To[32];
    UINT8 ID_DATA_Tc[32];
    uni_rom_id ID_data_uart_CMD0111;
    UINT8 ID_DATA_To_or_Tc_place;
    UINT8  DATA_Packet_soft_ver;
    UINT8 CMD0102_To_or_Tc_place;
    UINT8 CMD0102_To_or_Tc_HA;
    UINT8 FLAG_IDcheck_CMD0102_HA;
    UINT8 HA_uart_send_APP_number=0;
#endif


void VHF_GPIO_INIT(void){	
    // CPU端口设置
    ANSELA = 0x0000; // Turn off all ADC analog intput pins
    ANSELB = 0x0000;
    ANSELC = 0x0000;
 #if defined(__Product_PIC32MX2_Receiver__)
    CNPUA=0xFFFF;
    CNPUB=0xFFFF;
    CNPUC=0xFFFF;
    TRISBbits.TRISB10=1;
    TRISBbits.TRISB14=1;
        //HA_Status=0x81;
        //FLAG_open=1;
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
      
      FLAG_PORT_LEDoutput_allow=0;      //低电平有效
      FLAG_PORT_LEDoutput_NOallow=1;
      Receiver_LED_OUT_IO=0;// Output   受信机继电器动作输出 
      Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
      Receiver_LED_TX_IO=0;// Output   受信机送信指示  
      Receiver_LED_TX=FLAG_PORT_LEDoutput_NOallow;
      Receiver_LED_RX_IO=0;// Output   受信机受信指示  
      Receiver_LED_RX=FLAG_PORT_LEDoutput_NOallow;
      
      Receiver_OUT_OPEN_IO=0;  // Output   受信机继电器OPEN  高电平有效
      Receiver_OUT_OPEN=0;
      Receiver_OUT_CLOSE_IO=0;  // Output   受信机继电器CLOSE  高电平有效
      Receiver_OUT_CLOSE=0;
      Receiver_OUT_STOP_IO=0;  // Output   受信机继电器STOP  高电平有效
      Receiver_OUT_STOP=0;
      Receiver_OUT_VENT_IO=0;
      Receiver_OUT_VENT=0;

      Receiver_test_IO=1;
      DIP_switch1_IO=1;
      DIP_switch2_IO=1;
      DIP_switch3_IO=1;
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
       //ADF7021_CLKOUT_IO=1;     // Input
       WIFI_test_IO=1;
       CNPUBbits.CNPUB4=1;
       ADF7021_INT_LOCK_IO=1;   // Input
       SDAIO=0; // Input AND output
       SCLIO=0; // output

       PCF8563_INT_IO=1;// Input    追加定时OPEN CLOSE
       CNPUCbits.CNPUC0=1;
       //CNPDCbits.CNPDC0=0;
       
     WIFI_L_Login_IO=1;// Input   wifi集中通讯机登录键   低电平有效
     WIFI_USBLogin_IO=1;// Input   wifi集中通讯机USB升级键   低电平有效                                                 //测试，测试完后需要删除
     WIFI_USBOC_IO=1;//Input   wifi集中USB保护监测   低电平有效
     WIFI_LED_RX_IO=0;//output   wifi集中通讯机受信指示 低电平有效
     WIFI_LED_RX=0;
     WIFI_LED_TX_IO=0;// output   wifi集中通讯机送信指示 低电平有效
     WIFI_LED_TX=0;

     WIFI_LAN_SELECT_IO=1;  // Input   有线LAN、WIFI切换
     WIFI_POWER_IO=0;   // output  WIFI电源  低电平有效
     //WIFI_POWER=1;
     LATASET=0x0400;
     LAN_POWER_IO=0;// output  LAN电源  低电平有效
     LAN_POWER=1;
   #endif

}

void RF_test_mode(void )
{
  UINT8 uart_data,Boot_i;

#if defined(__Product_PIC32MX2_Receiver__)
  Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
  for(Boot_i=0;Boot_i<2;Boot_i++){
      for(time_3sec=0;time_3sec<1500;time_3sec++){
         Delayus(240);   //80us
         ClearWDT(); // Service the WDT
      }
      Receiver_LED_OUT=!Receiver_LED_OUT;
  }
  Receiver_LED_OUT=FLAG_PORT_LEDoutput_NOallow;



    while(Receiver_test==0){
        ClearWDT(); // Service the WDT
        if(HA_Sensor_signal==0)Receiver_LED_TX=FLAG_PORT_LEDoutput_allow;
        else Receiver_LED_TX=FLAG_PORT_LEDoutput_NOallow;

	if(HA_ERR_signal==0){
	  if(HA_L_signal==0)Tx_Rx_mode=0;
	  else Tx_Rx_mode=1;
	}
	else{
	  if(HA_L_signal==0)Tx_Rx_mode=2;
	  else Tx_Rx_mode=3;
	}
	if((Tx_Rx_mode==0)||(Tx_Rx_mode==1)){
	  FG_test_rx=0;
	  Receiver_LED_RX=FLAG_PORT_LEDoutput_NOallow;
	  FG_test_tx_off=0;
	  //if(HA_L_signal==0){    //发载波，无调制信号
	  if(Tx_Rx_mode==0){
	    Receiver_LED_OUT=FLAG_PORT_LEDoutput_allow;
	    FG_test_mode=0;
	    FG_test_tx_1010=0;
	    if(FG_test_tx_on==0){FG_test_tx_on=1;ADF7021_DATA_IO=1;dd_set_TX_mode_carrier();}
	  }
	  else {    //发载波，有调制信号
	    if(TIMER1s==0){
	      TIMER1s=500;
	      Receiver_LED_OUT=!Receiver_LED_OUT;
	    }
	    FG_test_mode=1;
	    FG_test_tx_on=0;
	    if(FG_test_tx_1010==0){FG_test_tx_1010=1;ADF7021_DATA_IO=0;dd_set_TX_mode_1010pattern();}
	  }
	}
        //else  {           //test ADF7021 RX
	if((Tx_Rx_mode==2)||(Tx_Rx_mode==3)){
	  FG_test_rx=1;
	  Receiver_LED_OUT=FLAG_PORT_LEDoutput_NOallow;
	  FG_test_mode=0;
	  FG_test_tx_on=0;
	  FG_test_tx_1010=0;
	  if(FG_test_tx_off==0){FG_test_tx_off=1;dd_set_RX_mode();ADF7021_DATA_IO=1;}    //dd_set_RX_mode_test()   //2015.3.31修改
	  //if(HA_L_signal==0){
	  if(Tx_Rx_mode==2)
	    if(TIMER1s==0){
	      TIMER1s=500;
	      Receiver_LED_RX=!Receiver_LED_RX;
	    }
	  if(Tx_Rx_mode==3){
            if(X_COUNT >= 1200){
              X_COUNT = 0;
	      if(X_ERR>=60)Receiver_LED_RX=FLAG_PORT_LEDoutput_NOallow;
	      else Receiver_LED_RX=FLAG_PORT_LEDoutput_allow;
//              uart_data = (X_ERR/1000) + 48;//48;//（X_ERR/1000) + 48;
//	      Send_char(uart_data);
//              X_ERR = X_ERR%1000;
//              uart_data = (X_ERR/100) + 48;//X_ERR/256;
//	      Send_char(uart_data);
//              X_ERR = X_ERR%100;
//              uart_data =(X_ERR/10) + 48;
//	      Send_char(uart_data);
//              X_ERR = X_ERR%10;
//              uart_data = X_ERR +48;
//	      Send_char(uart_data);
//              uart_data = 13;//|字符
//	      Send_char(uart_data);
              X_ERR = 0;
            }
	  }
	}
//	PC_PRG();	       // PC控制


    }
//    UART1_end();
    FG_test_rx=0;
    TIMER1s=0;
    Receiver_LED_TX=FLAG_PORT_LEDoutput_NOallow;
    Receiver_LED_RX=FLAG_PORT_LEDoutput_NOallow;
//    FG_Receiver_LED_RX=FLAG_PORT_LEDoutput_NOallow;
    Receiver_LED_OUT=FLAG_PORT_LEDoutput_NOallow;


    DIP_switch_data=0;
    DIP_switch_Get();
    DIP_switch_data_bak=DIP_switch_data;

    if(HA_ERR_signal==0)HA_Status=0x83;
    if((HA_L_signal==1)&&(HA_Sensor_signal==0)) HA_Status=0x81;
    if((HA_L_signal==0)||(HA_Sensor_signal==1))HA_Status=0x82;

    FLAG_426MHz_Reply=1;
    Freq_Scanning_CH=1;
    dd_set_RX_mode();
    TIME_Fine_Calibration=9000;
#endif


#if defined(__Product_PIC32MX2_WIFI__)
    while(WIFI_test==0){
        ClearWDT(); // Service the WDT
        //if(HA_ERR_signal==0){      //test ADF7021 TX
	if(WIFI_L_Login==0){
	  if(WIFI_USBLogin==0)Tx_Rx_mode=0;
	  else Tx_Rx_mode=1;
	}
	else{
	  if(WIFI_USBLogin==0)Tx_Rx_mode=2;
	  else Tx_Rx_mode=3;
	}
	if((Tx_Rx_mode==0)||(Tx_Rx_mode==1)){
	  FG_test_rx=0;
	  WIFI_LED_RX=0;
	  FG_test_tx_off=0;
	  //if(HA_L_signal==0){    //发载波，无调制信号
	  if(Tx_Rx_mode==0){
	    WIFI_LED_TX=1;
	    FG_test_mode=0;
	    FG_test_tx_1010=0;
	    if(FG_test_tx_on==0){FG_test_tx_on=1;ADF7021_DATA_IO=1;dd_set_TX_mode_carrier();}
	  }
	  else {    //发载波，有调制信号
	    if(TIMER1s==0){
	      TIMER1s=500;
	      WIFI_LED_TX=!WIFI_LED_TX;
	    }
	    FG_test_mode=1;
	    FG_test_tx_on=0;
	    if(FG_test_tx_1010==0){FG_test_tx_1010=1;ADF7021_DATA_IO=0;dd_set_TX_mode_1010pattern();}
	  }
	}
        //else  {           //test ADF7021 RX
	if((Tx_Rx_mode==2)||(Tx_Rx_mode==3)){
	  FG_test_rx=1;
	  WIFI_LED_TX=0;
	  FG_test_mode=0;
	  FG_test_tx_on=0;
	  FG_test_tx_1010=0;
	  if(FG_test_tx_off==0){FG_test_tx_off=1;dd_set_RX_mode();ADF7021_DATA_IO=1;}    //dd_set_RX_mode_test()    //2015.3.31修改
	  //if(HA_L_signal==0){
	  if(Tx_Rx_mode==2)
	    if(TIMER1s==0){
	      TIMER1s=500;
	      WIFI_LED_RX=!WIFI_LED_RX;
	    }
	  if(Tx_Rx_mode==3){
            if(X_COUNT >= 1200){
              X_COUNT = 0;
	      if(X_ERR>=60)WIFI_LED_RX=0;
	      else WIFI_LED_RX=1;
//              uart_data = (X_ERR/1000) + 48;//48;//（X_ERR/1000) + 48;
//	      Send_char(uart_data);
//              X_ERR = X_ERR%1000;
//              uart_data = (X_ERR/100) + 48;//X_ERR/256;
//	      Send_char(uart_data);
//              X_ERR = X_ERR%100;
//              uart_data =(X_ERR/10) + 48;
//	      Send_char(uart_data);
//              X_ERR = X_ERR%10;
//              uart_data = X_ERR +48;
//	      Send_char(uart_data);
//              uart_data = 13;//|字符
//	      Send_char(uart_data);
              X_ERR = 0;
            }
	  }
	}
//	PC_PRG();	       // PC控制


    }
//    UART1_end();
    FG_test_rx=0;
    TIMER1s=0;
    WIFI_LED_TX=0;
    WIFI_LED_RX=0;


    TIME_WIFI_LAN_SELECT=10;
    SWITCH_DIP_bak=0xFF;
    TIME_one_hour=328000;    //1000     //2015.1.30追加修改1小时查询一次HA状态

    Freq_Scanning_CH=2;
    dd_set_RX_mode();
    TIME_Fine_Calibration=9000;
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
     for(y=0;y<=200;y++);   //特别说明：该行采用XC32的0级优化，即无优化
     //for(y=0;y<=600;y++);   //特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
#endif
#if defined(__Product_PIC32MX2_WIFI__)
     for(y=0;y<=400;y++);   //特别说明：该行采用XC32的0级优化，即无优化    时间=timer*0.9ms
     //for(y=0;y<=1800;y++);    //特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。    时间=timer*0.9ms
#endif
   }
}
void Delayus(UINT16 timer)
{
  UINT16 x;            //延时T=(timer)us     //SYSCLK=20M
#if defined(__Product_PIC32MX2_Receiver__)
     for(x=0;x<2*timer;x++);  //特别说明：该行采用XC32的0级优化，即无优化
   //for(x=0;x<10*timer;x++);  //特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。
#endif
#if defined(__Product_PIC32MX2_WIFI__)
     for(x=0;x<4*timer;x++);    //特别说明：该行采用XC32的0级优化，即无优化    时间=0.7us+timer*1.2us
    //for(x=0;x<26*timer;x++);    //特别说明：该行采用XC32的1级优化，C编译器优化后延时函数的延时时间被改变了，请注意。    时间=0.7us+timer*1.2us
#endif
}