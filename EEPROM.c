
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
void all_Erase_EEPROM(void)
{
#if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i;
    UINT8 xm[3]={0};

    for(i=0;i<64;i++){EMIAL_id_data[i]=0;EMIAL_id_HA[i]=0;EMIAL_id_PCS=0;Email_check_ID[i]=0xff;Emial_check_Control[i]=0xff;}
    
    TIMER1s=3000;
    while((WIFI_L_Login==0)&&(WIFI_USBLogin==0)){
        if((TIMER1s==0)&&(FLAG_all_Erase==0)){FLAG_all_Erase=1;TIMER1s=5000;TIMER300ms=300;WIFI_LED_TX=1;WIFI_LED_RX=0;break;}
        ClearWDT(); // Service the WDT
    }
    while(FLAG_all_Erase==1){
        if(TIMER300ms==0){TIMER300ms=300;WIFI_LED_TX=!WIFI_LED_TX;WIFI_LED_RX=!WIFI_LED_RX;}
        if(FLAG_all_Erase_OK==0){
            FLAG_all_Erase_OK=1;
            xm[0]=0xFF;
            xm[1]=0xFF;
            Write(&xm[0],0x7FE,2);
            Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
            xm[0]=0xFF;
            Write(&xm[0],0xFDE,1);
        }
        if(TIMER1s==0){WIFI_LED_TX=0;WIFI_LED_RX=0;FLAG_all_Erase=0;break;}
        ClearWDT(); // Service the WDT
    }
    TIMER300ms=0;
    TIMER1s=0;
#endif
}
void ID_EEPROM_Initial(void)
{
    UINT16 i,j,RTC_Minutes;
    UINT8 xm[10]={0};
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
    for(i=0;i<100;i++)
        for(j=0;j<10;j++)
        {
            WIFI_alarm_data[i][j]=0;
        }
    Read(&xm[0],0xFDE,1);
    WIFI_alarm_data_PCS=xm[0];
    if(WIFI_alarm_data_PCS==0xFF){WIFI_alarm_data_PCS=0;xm[0]=0;Write_pcf8563(&xm[0],0x01,1);}
    Read_Time(&xm[0]);
    RTC_Minutes=xm[2]*60+xm[1];
    for(i=0;i<WIFI_alarm_data_PCS;i++){
        m2=i/3;
        m3=i%3;
        Read(&xm[0],0x800+m2*32+m3*10,10);
        for(j=0;j<10;j++)WIFI_alarm_data[i][j]=xm[j];
    }
    NEW_set_alarm_pcf8563(RTC_Minutes);
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
void alarm_EEPROM_write(void)
{
    uni_rom_id xmm;
    UINT16 i,j;
    UINT8 FLAG_i,FLAG_j=0;
    UINT8 xm[10]={0};
    UINT16 m1,m2,m3;
    UINT16 RTC_Minutes;
    xmm.IDB[0]=UART1_DATA[8];
    xmm.IDB[1]=UART1_DATA[9];
    xmm.IDB[2]=UART1_DATA[10];
    xmm.IDB[3]=0x00;
    if(xmm.IDL==0)return;
    for(i=0;i<ID_DATA_PCS;i++){
       if(ID_Receiver_DATA[i]==xmm.IDL){i=ID_DATA_PCS;FLAG_i=1;}
    }
    if(FLAG_i==1){
        FLAG_i=0;
        for(i=0;i<WIFI_alarm_data_PCS;i++){
            if((WIFI_alarm_data[i][0]==UART1_DATA[8])&&(WIFI_alarm_data[i][1]==UART1_DATA[9])&&(WIFI_alarm_data[i][2]==UART1_DATA[10])){
                for(j=8;j<18;j++){WIFI_alarm_data[i][j-8]=UART1_DATA[j];WIFI_alarm_data[i+1][j-8]=UART1_DATA[j];}
                for(j=18;j<25;j++)WIFI_alarm_data[i+1][j-15]=UART1_DATA[j];
                m2=i/3;
                m3=i%3;
                Write(WIFI_alarm_data[i],0x800+m2*32+m3*10,10);//写入数据到24LC16
                Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
                m2=(i+1)/3;
                m3=(i+1)%3;
                Write(WIFI_alarm_data[i+1],0x800+m2*32+m3*10,10);//写入数据到24LC16
                Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
                i=WIFI_alarm_data_PCS;
                FLAG_j=1;
            }
        }
     }
        if(FLAG_j==0){
             WIFI_alarm_data_PCS++;
             WIFI_alarm_data_PCS++;
             xm[0]=WIFI_alarm_data_PCS;
             Write(&xm[0],0xFDE,1);
             Delay100us(100);
            for(j=8;j<18;j++){WIFI_alarm_data[WIFI_alarm_data_PCS-2][j-8]=UART1_DATA[j];WIFI_alarm_data[WIFI_alarm_data_PCS-1][j-8]=UART1_DATA[j];}
            for(j=18;j<25;j++)WIFI_alarm_data[WIFI_alarm_data_PCS-1][j-15]=UART1_DATA[j];
            m2=(WIFI_alarm_data_PCS-2)/3;
            m3=(WIFI_alarm_data_PCS-2)%3;
            Write(WIFI_alarm_data[WIFI_alarm_data_PCS-2],0x800+m2*32+m3*10,10);//写入数据到24LC16
            Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
            m2=(WIFI_alarm_data_PCS-1)/3;
            m3=(WIFI_alarm_data_PCS-1)%3;
            Write(WIFI_alarm_data[WIFI_alarm_data_PCS-1],0x800+m2*32+m3*10,10);//写入数据到24LC16
            Delay100us(100);            //写周期时间  24LC为5ms,24c或24wc为10ms
        }
        Read_Time(&xm[0]);
        RTC_Minutes=xm[2]*60+xm[1];
        NEW_set_alarm_pcf8563(RTC_Minutes);
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
     xm[0]=0;
     xm[1]=0;
     xm[2]=0;
     ID_Receiver_DATA[INquiry]=0x00;
     Write(&xm[0],INquiry*3,3);//写入数据到24LC16
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
 UINT8 i;
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
 UINT8 i ,j;
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
