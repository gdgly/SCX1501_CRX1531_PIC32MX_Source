
/***********************************************************************/
/*  FILE        :EEPROM.c                                              */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化

/******************EEPROM 数据说明******************************
24LC32 PAGE=32Bits
addr:0x000～0x01D      ID1～ID10    一个ID号有3个字节 低字节在前
     0x01E 0x01F       未用
     0x020～0x03D      ID11～ID20
     0x03E 0x03F       未用
     ...
     ...  ～0x33F      最大256PCS
addr:0x5E0～0x7BF      按照initial.C文件中Sunrise_sunset_DATA数值的顺序依次存储
addr:0x7DC             存储日出日落表格数据DATA来源   =0 RAM的数据    =1 EEPROM的数据
     0x7DD             日出ON/OFF  ON=1  OFF=0
     0x7DE             日落ON/OFF  ON=1  OFF=0
     0x7DF             日本地点  1=东北, 2=关东,3=关西,4=九州
addr:0x7ED             卷帘门状态变化是否邮件件送信   0x00->OFF    0x01->ON
     0x7EE             卷帘门状态变化邮件通知地址低字节
     0x7EF             卷帘门状态变化邮件通知地址高字节
addr:0x7FE 0x7FF       学习过的ID个数  低字节在前   ID_DATA_PCS
addr:0x800～0x866      控制定时1数据   具体数据参照《集中通讯机APP协议规则第9版》及以后版本
     0x867～0x87F      未用
     0x880～0x8E6      控制定时2数据   
     0x8E7～0x8FF      未用
     ...
     ...  ～0xCFF      最大控制定时器10PCS
addr:0xD80～0xDE6      日出定时器数据   具体数据参照《集中通讯机APP协议规则第9版》及以后版本
     0xDE7～0xDFF      未用
     0xE00～0xE66      日落定时器数据
     0xE67～0xE7F      未用
addr:0xF00～0xF1B      邮件定时器1，2，3，4数据  一个邮件定时器有7个字节 具体数据参照《集中通讯机APP协议规则第9版》及以后版本
     0xF1C～0xF1F      未用
     0xF20～0xF3B      邮件定时器5，6，7，8数据
     0xF3C～0xF3F      未用
     0xF40～0xF4D      邮件定时器9，10数据
     0xF4E～0xF5F      未用
***************************************************************/
UINT8 ack = 0;//应答信号

void start_i2c(void);
void stop_i2c(void);
void Mack(void);
void N0ack(char a);
void send_byte(UINT8 c);
void Write(UINT8 *s,UINT16 suba,UINT8 n);
UINT8 receive_byte(void);
void Read(UINT8 *s,UINT16 suba,UINT8 n);
void eeprom_IDcheck(void);
void eeprom_IDcheck_UART(void);
void ID_EEPROM_write(void);
void ID_Login_EXIT_Initial(void);

#if defined(__Product_PIC32MX2_WIFI__)
void alarm_EEPROM_write(void);
void all_Erase_EEPROM_next(void);
void eeprom_IDcheck_Multiple(UINT8 value_m);
void Sunrise_sunset_EEPROM_write(void);
void SUN_time_get(UINT8 value);
void SUN_EEPROM_write(void);
void Emial_time_EEPROM_write(void);
void HA_Change_EEPROM_write(void);
#endif

void ID_learn(void)
{
    UINT16 i;
 #if defined(__Product_PIC32MX2_Receiver__)
 if(FG_10ms){
     FG_10ms = 0;

//     if(time_3sec)--time_3sec;
     if(TIME_EMC)--TIME_EMC;

     if(rssi_TIME)--rssi_TIME;
     if(TIMER60s)--TIMER60s;
     if(TIME_Receiver_Login_restrict)--TIME_Receiver_Login_restrict;
       else if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1));
          else {TIME_Receiver_Login=0;COUNT_Receiver_Login=0;}

         if(Receiver_Login==0){
             TIME_Receiver_Login++;
             TIME_Receiver_Login_restrict=350;
             if(COUNT_Receiver_Login>=2){FLAG_ID_Login=1;TIME_Login_EXIT_rest=6000;}
             if(((FLAG_ID_Erase_Login==1)&&(COUNT_Receiver_Login>=1))||
                ((FLAG_ID_Login==1)&&(COUNT_Receiver_Login>=3)))ID_Login_EXIT_Initial();
         }
         if(Receiver_Login==1){
             if(TIME_Receiver_Login>3)COUNT_Receiver_Login++;
             if(FLAG_ID_Login_EXIT==1){FLAG_ID_Login_EXIT=0;COUNT_Receiver_Login=0;}
             TIME_Receiver_Login=0;
         }
         if(TIME_Receiver_Login>=300){
             TIME_Receiver_Login=0;
             FLAG_ID_Erase_Login=1;
             FLAG_ID_Erase_Login_PCS=1;    //追加多次ID登录
             TIME_Login_EXIT_rest=6000;
         }
         if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1)){
             TIME_Receiver_Login_led++;
             if(TIME_Receiver_Login_led>=30){
                 TIME_Receiver_Login_led=0;
                 Receiver_LED_OUT=!Receiver_LED_OUT;
             }
             if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0)){
                 //FLAG_ID_Login_OK_bank=1;             //追加多次ID登录
                 FLAG_ID_Login_OK=0;                   //追加多次ID登录
                 if(FLAG_IDCheck_OK==1)FLAG_IDCheck_OK=0;
                 else{
                     BEEP_and_LED();
                     TIME_Login_EXIT_rest=6000;       //追加多次ID登录
                     if(FLAG_ID_Login==1)ID_EEPROM_write();
                     else if(FLAG_ID_Erase_Login==1){
                         if(FLAG_ID_Erase_Login_PCS==1){FLAG_ID_Erase_Login_PCS=0;ID_DATA_PCS=0;}      //追加多次ID登录
                         ID_EEPROM_write();
                     }
                 }//end else
             }//  end  if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0))
             if(TIME_Login_EXIT_rest)--TIME_Login_EXIT_rest;
              else ID_Login_EXIT_Initial();
         } //end if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))
 }
#endif
#if defined(__Product_PIC32MX2_WIFI__)
 if(FG_10ms){
     FG_10ms = 0;
     if(rssi_TIME)--rssi_TIME;
     if(TIME_UART)--TIME_UART;
     if(TIME_alarm_AUTO)--TIME_alarm_AUTO;
     if(TIME_email_Repeat)--TIME_email_Repeat;
     if(TIME_email_send)--TIME_email_send;
     if(FLAG_all_Erase_time)--FLAG_all_Erase_time;
     if(HA_Change_email_time)--HA_Change_email_time;

     if(FLAG_all_Erase==0)          //EEPROM所有数据擦出时，以下CODE不执行
     {
             if(TIME_Receiver_Login_restrict)--TIME_Receiver_Login_restrict;
               else if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1));
                  else {TIME_Receiver_Login=0;COUNT_Receiver_Login=0;}

                 if(WIFI_L_Login==0){
                     TIME_Receiver_Login++;
                     TIME_Receiver_Login_restrict=350;
                     if(COUNT_Receiver_Login>=2){FLAG_ID_Login=1;TIME_Login_EXIT_rest=6000;}
                     if(((FLAG_ID_Erase_Login==1)&&(COUNT_Receiver_Login>=1))||
                        ((FLAG_ID_Login==1)&&(COUNT_Receiver_Login>=3)))ID_Login_EXIT_Initial();
                 }
                 if(WIFI_L_Login==1){
                     if(TIME_Receiver_Login>3)COUNT_Receiver_Login++;
                     if(FLAG_ID_Login_EXIT==1){FLAG_ID_Login_EXIT=0;COUNT_Receiver_Login=0;}
                     TIME_Receiver_Login=0;
                 }
        //         if(TIME_Receiver_Login>=300){                  //取消擦出登录功能
        //             TIME_Receiver_Login=0;
        //             FLAG_ID_Erase_Login=1;
        //             FLAG_ID_Erase_Login_PCS=1;    //追加多次ID登录
        //             TIME_Login_EXIT_rest=6000;
        //         }
                 if(TIME_Receiver_Login>=300){                  //取消擦出登录功能
                     TIME_Receiver_Login=0;
                     COUNT_Receiver_Login=0;
                     WIFI_LED_TX=1;
                     while(WIFI_L_Login==0)ClearWDT(); // Service the WDT
                     uart_send_APP_allID();
                    WIFI_LED_TX=0;
                 }
                 if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1)){
                     TIME_Receiver_Login_led++;
                     if(TIME_Receiver_Login_led>=30){
                         TIME_Receiver_Login_led=0;
                         WIFI_LED_RX=!WIFI_LED_RX;
                     }
                     if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0)){
                         //FLAG_ID_Login_OK_bank=1;     //追加多次ID登录
                         FLAG_ID_Login_OK=0;                   //追加多次ID登录
                         if(FLAG_IDCheck_OK==1)FLAG_IDCheck_OK=0;
                         else{
                             BEEP_and_LED();
                             TIME_Login_EXIT_rest=6000;       //追加多次ID登录
                             if(FLAG_ID_Login==1)ID_EEPROM_write();
                             else if(FLAG_ID_Erase_Login==1){
                                 if(FLAG_ID_Erase_Login_PCS==1){FLAG_ID_Erase_Login_PCS=0;ID_DATA_PCS=0;}      //追加多次ID登录
                                 ID_EEPROM_write();
                             }
                         }//end else
                     }//  end  if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0))
                     if(TIME_Login_EXIT_rest)--TIME_Login_EXIT_rest;
                      else ID_Login_EXIT_Initial();
                 } //end if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))
     }
 }
#endif
}

void ID_Login_EXIT_Initial(void)
{
 #if defined(__Product_PIC32MX2_Receiver__)
     FLAG_ID_Login_EXIT=1;
     FLAG_ID_Login_OK=0;
     FLAG_ID_Login_OK_bank=0;
     FLAG_ID_Login=0;
     FLAG_ID_Erase_Login=0;
     Receiver_LED_OUT=0;
#endif
#if defined(__Product_PIC32MX2_WIFI__)
     FLAG_ID_Login_EXIT=1;
     FLAG_ID_Login_OK=0;
     FLAG_ID_Login_OK_bank=0;
     FLAG_ID_Login=0;
     FLAG_ID_Erase_Login=0;
     WIFI_LED_RX=0;
#endif
}
void all_Erase_EEPROM_next(void)
{
    UINT16 i;
    UINT8 xm[32]={0};
 #if defined(__Product_PIC32MX2_WIFI__)
    if(FLAG_all_Erase==1){
        if((FLAG_all_Erase_time>0)&&((WIFI_L_Login!=0)||(WIFI_USBLogin!=0)))FLAG_all_Erase=0;
        else if((FLAG_all_Erase_time==0)&&(FLAG_all_Erase_loop==0)){FLAG_all_Erase_loop=1;TIMER1s=5000;TIMER300ms=300;WIFI_LED_TX=1;WIFI_LED_RX=0;}
        if(FLAG_all_Erase_loop==1){
            if(TIMER300ms==0){TIMER300ms=300;WIFI_LED_TX=!WIFI_LED_TX;WIFI_LED_RX=!WIFI_LED_RX;}
            if(FLAG_all_Erase_OK==0){
                FLAG_all_Erase_OK=1;
//                xm[0]=0xFF;
//                xm[1]=0xFF;
//                Write(&xm[0],0x7FE,2);
//                Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//                xm[0]=0xFF;
//                Write(&xm[0],0xFDE,1);
                for(i=0;i<32;i++)xm[i]=0xFF;
                for(i=0;i<128;i++){
                    Write(&xm[0],i*32,32);
                    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
                    ClearWDT(); // Service the WDT
                    if(TIMER300ms==0){TIMER300ms=300;WIFI_LED_TX=!WIFI_LED_TX;WIFI_LED_RX=!WIFI_LED_RX;}
                }
            }
            if(TIMER1s==0);
            else ClearWDT(); // Service the WDT
        }
    }
    else ClearWDT(); // Service the WDT
 #endif
 #if defined(__Product_PIC32MX2_Receiver__)
    ClearWDT();
#endif
}
void all_Erase_EEPROM(void)
{
#if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i;
    UINT8 xm[32]={0};

    FLAG_APP_BYTE=0;
    for(i=0;i<64;i++){EMIAL_id_data[i]=0;EMIAL_id_HA[i]=0;EMIAL_id_PCS=0;Email_check_ID[i]=0xff;Emial_check_Control[i]=0xff;}

    if((WIFI_L_Login==0)&&(WIFI_USBLogin==0)){FLAG_all_Erase=1;FLAG_all_Erase_time=300;}
    else {FLAG_all_Erase=0;TIMER300ms=0;TIMER1s=0;}
#endif
}
void ID_EEPROM_Initial(void)
{
    UINT16 i,j,RTC_Minutes,HA_Change_i;
    UINT8 xm[103]={0};
    UINT16 m1,m2,m3;
    uni_rom_id xn;
    for(i=0;i<256;i++)ID_Receiver_DATA[i]=0;//ID_Receiver_DATA[ID_DATA_PCS]=0;
    Read(&xm[0],0x7FE,2);
    ID_DATA_PCS=xm[1]*256+xm[0];
    if(ID_DATA_PCS==0xFFFF)ID_DATA_PCS=0;
    for(i=0;i<ID_DATA_PCS;i++){
        m2=i/10;
        m3=i%10;
        Read(&xm[0],32*m2+m3*3,3);
        xn.IDB[0]=xm[0];
        xn.IDB[1]=xm[1];
        xn.IDB[2]=xm[2];
        xn.IDB[3]=0;
        ID_Receiver_DATA[i]=xn.IDL;
    }
#if defined(__Product_PIC32MX2_WIFI__)
//    Write(&Sunrise_sunset_DATA[0],0xD00,32);
//    Delay100us(100);

    Read(&xm[0],0x7DC,4);       //0x7DC存储日出日落表格数据DATA来源   =0 RAM的数据    =1 EEPROM的数据
    if(xm[1]>1)xm[1]=0;
    if(xm[2]>1)xm[2]=0;
    if(xm[3]>10)xm[3]=0;
    SUN_ON_OFF_seat[0]=xm[1];   //0x7DD 日出ON/OFF  ON=1  OFF=0
    SUN_ON_OFF_seat[1]=xm[2];  //0x7DE 日落ON/OFF  ON=1  OFF=0
    SUN_ON_OFF_seat[2]=xm[3];  //0x7DF 日本地点  1=东北, 2=关东,3=关西,4=九州

    Read(&xm[0],0xD80,32);
    Delay100us(100);
    for(i=0;i<13;i++){
       ClearWDT(); // Service the WDT
       if(i!=10){
               if(i==11){Read(&xm[0],0x800+12*128,103);xm[0]=11;xm[1]=SUN_ON_OFF_seat[0];xm[2]=0x08;}
               else Read(&xm[0],0x800+i*128,103);
               if(i>10)m1=i-1;
               else m1=i;
               if((xm[0]==0x00)||(xm[0]>12)||(xm[1]>1)||(xm[2]>8)||(xm[3]>24)||(xm[4]>60)||(xm[5]>0x80)||(xm[6]>0x20)){
                   WIFI_alarm_data[m1][0]=m1+1;
                   for(j=1;j<103;j++)WIFI_alarm_data[m1][j]=0x00;
               }
               else {
                   WIFI_alarm_data[m1][0]=m1+1;
                   for(j=1;j<103;j++)WIFI_alarm_data[m1][j]=xm[j];
               }
       }
    }

    Read(&xm[0],0x7DC,4);       //0x7DC存储日出日落表格数据DATA来源   =0 RAM的数据    =1 EEPROM的数据
    if(xm[0]==1){
             //Read(&Sunrise_sunset_DATA[0],0x5E0,480);
             for(i=0;i<15;i++){
                Read(&Sunrise_sunset_DATA[i*32],0x5E0+i*32,32);    //写入数据到24LC16
                Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
            }
    }
    SUN_time_get(SUN_ON_OFF_seat[2]);

    for(i=0;i<10;i++){
       ClearWDT(); // Service the WDT
       m2=i/4;
       m3=i%4;       
       Read(&xm[0],0xF00+32*m2+m3*7,7);       
       if((xm[0]==0x00)||(xm[0]>10)||(xm[1]>1)||(xm[2]>24)||(xm[3]>60)||(xm[4]>0x80)){
           Emial_time_data[i][0]=i+1;
           for(j=1;j<103;j++)Emial_time_data[i][j]=0x00;
       }
       else {
           Emial_time_data[i][0]=i+1;
           for(j=1;j<103;j++)Emial_time_data[i][j]=xm[j];
       }             
    }

    Read_Time(&xm[0]);
    RTC_Minutes=xm[2]*60+xm[1];
    NEW_set_alarm_pcf8563(RTC_Minutes);

    Read(&xm[0],0x7ED,3);       //0x7ED      卷帘门状态变化是否邮件件送信   0x00->OFF    0x01->ON
    if(xm[0]>1)xm[0]=0;
    HA_Change_i=xm[2]*256+xm[1];
    if(HA_Change_i>0x7FF){xm[1]=0;xm[2]=0;}
    HA_Change_send_email[0]=xm[0];   //
    HA_Change_send_email[1]=xm[1];  //0x7EE  卷帘门状态变化邮件通知地址低字节
    HA_Change_send_email[2]=xm[2];  //0x7EF  卷帘门状态变化邮件通知地址高字节


//    for(i=0;i<100;i++)
//        for(j=0;j<10;j++)
//        {
//            WIFI_alarm_data[i][j]=0;
//        }
//    Read(&xm[0],0xFDE,1);
//    WIFI_alarm_data_PCS=xm[0];
//    if(WIFI_alarm_data_PCS==0xFF){WIFI_alarm_data_PCS=0;xm[0]=0;Write_pcf8563(&xm[0],0x01,1);}
//    Read_Time(&xm[0]);
//    RTC_Minutes=xm[2]*60+xm[1];
//    for(i=0;i<WIFI_alarm_data_PCS;i++){
//        m2=i/3;
//        m3=i%3;
//        Read(&xm[0],0x800+m2*32+m3*10,10);
//        for(j=0;j<10;j++)WIFI_alarm_data[i][j]=xm[j];
//    }
//    NEW_set_alarm_pcf8563(RTC_Minutes);
#endif
}

void eeprom_IDcheck(void)
{
    UINT16 i;
   for(i=0;i<ID_DATA_PCS;i++){
       if(ID_Receiver_DATA[i]==DATA_Packet_ID){INquiry=i;i=ID_DATA_PCS;FLAG_IDCheck_OK=1;}
       if((FLAG_ID_Erase_Login==1)&&(FLAG_ID_Erase_Login_PCS==1)){i=ID_DATA_PCS;FLAG_IDCheck_OK=0;}         //追加多次ID登录
   }
}
void eeprom_IDcheck_UART(void)
{
    UINT16 i;
   for(i=0;i<ID_DATA_PCS;i++){
       if(ID_Receiver_DATA[i]==ID_data.IDL){i=ID_DATA_PCS;FLAG_IDCheck_OK=1;}
       if(FLAG_ID_Erase_Login==1){i=ID_DATA_PCS;FLAG_IDCheck_OK=0;}
   }
}

#if defined(__Product_PIC32MX2_WIFI__)
void eeprom_IDcheck_Multiple(UINT8 value_m)
{
    UINT16 m_i,m_j;
    uni_rom_id data_m;
    for(m_i=0;m_i<UART1_DATA[value_m];m_i++){
            FLAG_IDCheck_OK=0;
            data_m.IDB[0]=UART1_DATA[value_m+1+m_i*3];
            data_m.IDB[1]=UART1_DATA[value_m+2+m_i*3];
            data_m.IDB[2]=UART1_DATA[value_m+3+m_i*3];
            data_m.IDB[3]=0x00;
            for(m_j=0;m_j<ID_DATA_PCS;m_j++){
                if(ID_Receiver_DATA[m_j]==data_m.IDL){FLAG_IDCheck_OK=1;m_j=ID_DATA_PCS;}
            }
            if(FLAG_IDCheck_OK==0)return;
    }
}
void Sunrise_sunset_EEPROM_write(void){         //日出日落表格数据DATA设置保存
    UINT16 i,j,RTC_Minutes;
    UINT8 xm[2]={0x01,0x00};
    UINT8 xn[10]={0x00};

    Write(&xm[0],0x7DC,1);     //0x7DC存储日出日落表格数据DATA来源   =0 RAM的数据    =1 EEPROM的数据
    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    for(i=11;i<491;i++)Sunrise_sunset_DATA[i-11]=UART1_DATA[i];
    for(i=0;i<15;i++){
        Write(&Sunrise_sunset_DATA[i*32],0x5E0+i*32,32);    //写入数据到24LC16
        Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    }
    SUN_time_get(SUN_ON_OFF_seat[2]);
    Read_Time(&xn[0]);
    RTC_Minutes=xn[2]*60+xn[1];
    NEW_set_alarm_pcf8563(RTC_Minutes);
}
void SUN_time_get(UINT8 value){         //查询某月某地在日出日落表格数据DATA中的OPEN CLOSE时间
    UINT8 xm[10]={0};
    UINT8 i;
    if((value>0)&&(value<=10)){
        Read_Time(&xm[0]);
        i=(xm[5]-1)*40+(value-1)*4;
        WIFI_alarm_data[10][3]=Sunrise_sunset_DATA[i];
        WIFI_alarm_data[10][4]=Sunrise_sunset_DATA[i+1];
        WIFI_alarm_data[11][3]=Sunrise_sunset_DATA[i+2];
        WIFI_alarm_data[11][4]=Sunrise_sunset_DATA[i+3];
        SUN_Weekdays_alarm=xm[5];
    }
}
void alarm_EEPROM_write(void)
{
    uni_rom_id xmm;
    UINT16 i,j;
    UINT8 FLAG_i,FLAG_j=0;
    UINT8 xm[10]={0};
    UINT16 m1,m2,m3;
    UINT16 RTC_Minutes;

    m1=UART1_DATA[11];
    m2=18+UART1_DATA[17]*3;
    for(i=11;i<m2;i++)WIFI_alarm_data[m1-1][i-11]=UART1_DATA[i];
    for(j=i;j<103;j++)WIFI_alarm_data[m1-1][j]=0x00;
    Write(WIFI_alarm_data[m1-1],0x800+(m1-1)*128,32);    //写入数据到24LC16
    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    Write(&WIFI_alarm_data[m1-1][32],0x800+(m1-1)*128+32,32);
    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    Write(&WIFI_alarm_data[m1-1][64],0x800+(m1-1)*128+64,32);
    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    Write(&WIFI_alarm_data[m1-1][96],0x800+(m1-1)*128+96,7);
    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    Read_Time(&xm[0]);
    RTC_Minutes=xm[2]*60+xm[1];
    NEW_set_alarm_pcf8563(RTC_Minutes);    

//    xmm.IDB[0]=UART1_DATA[8];
//    xmm.IDB[1]=UART1_DATA[9];
//    xmm.IDB[2]=UART1_DATA[10];
//    xmm.IDB[3]=0x00;
//    if(xmm.IDL==0)return;
//    for(i=0;i<ID_DATA_PCS;i++){
//       if(ID_Receiver_DATA[i]==xmm.IDL){i=ID_DATA_PCS;FLAG_i=1;}
//    }
//    if(FLAG_i==1){
//        FLAG_i=0;
//        for(i=0;i<WIFI_alarm_data_PCS;i++){
//            if((WIFI_alarm_data[i][0]==UART1_DATA[8])&&(WIFI_alarm_data[i][1]==UART1_DATA[9])&&(WIFI_alarm_data[i][2]==UART1_DATA[10])){
//                for(j=8;j<18;j++){WIFI_alarm_data[i][j-8]=UART1_DATA[j];WIFI_alarm_data[i+1][j-8]=UART1_DATA[j];}
//                for(j=18;j<25;j++)WIFI_alarm_data[i+1][j-15]=UART1_DATA[j];
//                m2=i/3;
//                m3=i%3;
//                Write(WIFI_alarm_data[i],0x800+m2*32+m3*10,10);//写入数据到24LC16
//                Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//                m2=(i+1)/3;
//                m3=(i+1)%3;
//                Write(WIFI_alarm_data[i+1],0x800+m2*32+m3*10,10);//写入数据到24LC16
//                Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//                i=WIFI_alarm_data_PCS;
//                FLAG_j=1;
//            }
//        }
//     }
//        if(FLAG_j==0){
//             WIFI_alarm_data_PCS++;
//             WIFI_alarm_data_PCS++;
//             xm[0]=WIFI_alarm_data_PCS;
//             Write(&xm[0],0xFDE,1);
//             Delay100us(100);
//            for(j=8;j<18;j++){WIFI_alarm_data[WIFI_alarm_data_PCS-2][j-8]=UART1_DATA[j];WIFI_alarm_data[WIFI_alarm_data_PCS-1][j-8]=UART1_DATA[j];}
//            for(j=18;j<25;j++)WIFI_alarm_data[WIFI_alarm_data_PCS-1][j-15]=UART1_DATA[j];
//            m2=(WIFI_alarm_data_PCS-2)/3;
//            m3=(WIFI_alarm_data_PCS-2)%3;
//            Write(WIFI_alarm_data[WIFI_alarm_data_PCS-2],0x800+m2*32+m3*10,10);//写入数据到24LC16
//            Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//            m2=(WIFI_alarm_data_PCS-1)/3;
//            m3=(WIFI_alarm_data_PCS-1)%3;
//            Write(WIFI_alarm_data[WIFI_alarm_data_PCS-1],0x800+m2*32+m3*10,10);//写入数据到24LC16
//            Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//        }
//        Read_Time(&xm[0]);
//        RTC_Minutes=xm[2]*60+xm[1];
//        NEW_set_alarm_pcf8563(RTC_Minutes);
}
void SUN_EEPROM_write(void)
{
    UINT8 i,j;
    UINT8 xm[10]={0};
    UINT8 xmmmmmm[103]={0};
    UINT16 m1,m2,m3;
    UINT16 RTC_Minutes;

    SUN_ON_OFF_seat[0]=UART1_DATA[11];   //0x7DD 日出ON/OFF  ON=1  OFF=0
    SUN_ON_OFF_seat[1]=UART1_DATA[12];  //0x7DE 日落ON/OFF  ON=1  OFF=0
    SUN_ON_OFF_seat[2]=UART1_DATA[13];  //0x7DF 日本地点  1=东北, 2=关东,3=关西,4=九州
    Write(&SUN_ON_OFF_seat[0],0x7DD,3);
    WIFI_alarm_data[10][0]=11;
    WIFI_alarm_data[10][1]=UART1_DATA[11];
    WIFI_alarm_data[10][2]=0x08;
    WIFI_alarm_data[10][3]=0;         //在EEPROM中的日出日落时间数据为0，是靠SUN_time_get(SUN_ON_OFF_seat[2])函数在表中查询获取
    WIFI_alarm_data[10][4]=0;
    WIFI_alarm_data[11][0]=12;
    WIFI_alarm_data[11][1]=UART1_DATA[12];
    WIFI_alarm_data[11][2]=0x02;
    WIFI_alarm_data[11][3]=0;       //在EEPROM中的日出日落时间数据为0，是靠SUN_time_get(SUN_ON_OFF_seat[2])函数在表中查询获取
    WIFI_alarm_data[11][4]=0;
    m2=16+UART1_DATA[15]*3;
    for(i=14;i<m2;i++){WIFI_alarm_data[10][i-9]=UART1_DATA[i];WIFI_alarm_data[11][i-9]=UART1_DATA[i];}
    for(i=m2-9;i<103;i++){WIFI_alarm_data[10][i]=0x00;WIFI_alarm_data[11][i]=0x00;}
//    Write(&WIFI_alarm_data[10][0],0x800+11*128,32);    //写入数据到24LC16
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//    Write(&WIFI_alarm_data[10][32],0x800+11*128+32,32);
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//    Write(&WIFI_alarm_data[10][64],0x800+11*128+64,32);
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//    Write(&WIFI_alarm_data[10][96],0x800+11*128+96,7);
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//    Write(&WIFI_alarm_data[11][0],0x800+12*128,32);    //写入数据到24LC16
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//    Write(&WIFI_alarm_data[11][32],0x800+12*128+32,32);
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//    Write(&WIFI_alarm_data[11][64],0x800+12*128+64,32);
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
//    Write(&WIFI_alarm_data[11][96],0x800+12*128+96,7);
//    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    for(i=11;i<13;i++){
        Write(&WIFI_alarm_data[i-1][0],0x800+i*128,32);    //写入数据到24LC16
        Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
        Write(&WIFI_alarm_data[i-1][32],0x800+i*128+32,32);
        Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
        Write(&WIFI_alarm_data[i-1][64],0x800+i*128+64,32);
        Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
        Write(&WIFI_alarm_data[i-1][96],0x800+i*128+96,7);
        Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms        
    }

    SUN_time_get(SUN_ON_OFF_seat[2]);
    Read_Time(&xm[0]);
    RTC_Minutes=xm[2]*60+xm[1];
    NEW_set_alarm_pcf8563(RTC_Minutes);
}
void Emial_time_EEPROM_write(void)
{
    UINT16 i,j;
    UINT8 xm[10]={0};
    UINT16 m1,m2,m3;
    UINT16 RTC_Minutes;

    m1=UART1_DATA[11];
    m2=18;
    for(i=11;i<m2;i++)Emial_time_data[m1-1][i-11]=UART1_DATA[i];
    m1=UART1_DATA[11]-1;
    m2=m1/4;
    m3=m1%4;
    Write(Emial_time_data[m1],0xF00+32*m2+m3*7,7);//写入数据到24LC16
    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
    Read_Time(&xm[0]);
    RTC_Minutes=xm[2]*60+xm[1];
    NEW_set_alarm_pcf8563(RTC_Minutes);
}
void HA_Change_EEPROM_write(void)
{
    HA_Change_send_email[0]=UART1_DATA[11];
    HA_Change_send_email[1]=UART1_DATA[12];
    HA_Change_send_email[2]=UART1_DATA[13];
    Write(&HA_Change_send_email[0],0x7ED,3);    //写入数据到24LC16
    Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
}
#endif




void ID_EEPROM_write(void)
{
    UINT8 xm[3]={0};
    UINT16 m1,m2,m3;
    uni_rom_id xn;
     ID_DATA_PCS++;
     xm[0]=ID_DATA_PCS%256;
     xm[1]=ID_DATA_PCS/256;
     Write(&xm[0],0x7FE,2);
     Delay100us(100);
     ID_Receiver_DATA[ID_DATA_PCS-1]=ID_Receiver_Login;
     xn.IDL=ID_Receiver_Login;
     xm[0]=xn.IDB[0];
     xm[1]=xn.IDB[1];
     xm[2]=xn.IDB[2];
     m1=ID_DATA_PCS-1;
     m2=m1/10;
     m3=m1%10;
     Write(&xm[0],32*m2+m3*3,3);//写入数据到24LC16
     Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms

}
void ID_EEPROM_write_0x00(void)
{
   UINT8 xm[3]={0};
   UINT16 n2,n3;
     xm[0]=0;
     xm[1]=0;
     xm[2]=0;
     ID_Receiver_DATA[INquiry]=0x00;
     n2=INquiry/10;
     n3=INquiry%10;
     Write(&xm[0],32*n2+n3*3,3);//写入数据到24LC16
     Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
}
//===================start_i2c()起动总线函数=======//
void start_i2c(void)
{
 SDA=1; //发送启始条件的数据信号
 NOP();
 NOP();
 SCL=1;
 Delayus(5);  //>4.7us
 SDA=0; //发送起始信号
 Delayus(4);  //>4us
 SCL=0; //钳住I2C总线,准备发送数据或接收数据
 NOP();
 NOP();
 }
//==================stop_i2c()停止总线函数=========//
void stop_i2c(void)
{
 SDA = 0; //发送结束条件的数据信号
 Delayus(5);  //>4.7us
 SCL = 1;
 Delayus(4);//>4us
 SDA = 1;
 Delayus(5);  //>4.7us
}
//===============Mack()接收应答信号===============//
void Mack(void) //接收应答信号
{
 //SDA = 1;
 NOP();
 NOP();
 SDAIO = 1;
 NOP();
 NOP();
 Delayus(5); //>4.7us
 SCL = 1;
 Delayus(4); //>4us
 //SDAIO = 1;
 if(SDA_I == 1)
     ack = 0;
 else
     ack = 1; //用ASK=1为有应答信号
 SCL = 0;
 NOP();
 NOP();
 SDAIO = 0;
}
//=============send_byte()字节写函数===============//
void send_byte(UINT8 c)
{
 UINT8 bit_count;
 for(bit_count = 0;bit_count < 8;bit_count++)
     {
      if((c << bit_count) & 0x80) SDA = 1;
      else  SDA = 0;
      NOP();
      Delayus(5); //>4.7us
      SCL = 1;
      Delayus(4); //>4us
      SCL = 0;
     }
 Mack();
}
//==============Write()写N字节数据=================//
void Write(UINT8 *s,UINT16 suba,UINT8 n)                //24LC16B
{
 UINT16 i;
 start_i2c(); //启动总线
 /*************以下是24LC16对应的SOFT*************/
// i=suba/256;
// i=i*2+0xa0;
// send_byte(i); //发送器件地址
// i=suba%256;
// send_byte(i); //发送字地址
 /************************************************/
 /*************以下是24LC32对应的SOFT*************/
 send_byte(0xa0); //发送器件地址
 i=suba;
 send_byte(i/256); //发送字高地址
 send_byte(i%256); //发送字低地址
 /************************************************/
 for (i = 0;i < n;i++)
     {
      send_byte(*s); //发送字节数据
      s++;
     }
 stop_i2c(); //发送停止位
}
//===============receive_byte()字节读函数=================//
UINT8 receive_byte(void)
{
 UINT8 retc, bit_count;
 retc = 0;
 for(bit_count = 0;bit_count < 8;bit_count++)
     {
      NOP();
      SCL = 0;
      Delayus(5);  //>4.7us
      //SDA =1;
      SCL = 1;
      Delayus(5); //>4.7us
      retc = retc << 1;
      if(SDA_I == 1)
          retc = retc + 1;
     }
 SCL = 0;
 NOP();
 NOP();
 return(retc);
}
//===============NOack()=======================//
void N0ack(char a) //发送应答位
{
 SDAIO = 0;
 NOP();
 if(a==0)SDA = 0;
 else  SDA = 1;
 Delayus(5);  //>4.7us
 SCL = 1;
 Delayus(4);//>4us
 SCL = 0;
}
//===============//
void Read(UINT8 *s,UINT16 suba,UINT8 n)
{
 UINT16 i ,j;
 start_i2c(); //启动总线
 /*************以下是24LC16对应的SOFT*************/
// i=suba/256;
// i=i*2+0xa0;
// send_byte(i);//发送器件地址
// j=suba%256;
// send_byte(j); //发送字地址
// start_i2c(); //重新启动总线
// i=i+1;
// send_byte(i); //发送读命令和器件地址
 /************************************************/
 /*************以下是24LC32对应的SOFT*************/
 send_byte(0xa0); //发送器件地址
 i=suba;
 send_byte(i/256); //发送字高地址
 send_byte(i%256); //发送字低地址
 start_i2c(); //重新启动总线
 send_byte(0xa1); //发送读命令
 /************************************************/
 for(i = 0;i < n - 1;i++)
     {
      SDAIO = 1;
      *s = receive_byte(); //获得数据
      s++;
      N0ack(0); //发送应答信号
     }
 SDAIO = 1;
 *s = receive_byte(); //获得数据
 N0ack(1); //发送非应答位
 stop_i2c(); //发送停止位
}
