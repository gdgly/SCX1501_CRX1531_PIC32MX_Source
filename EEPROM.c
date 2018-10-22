/***********************************************************************/
/*  FILE        :EEPROM.c                                              */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// ����C����
#include "initial.h"		// ��ʼ��

UINT8 ack = 0;//Ӧ���ź�

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

void ID_learn(void)
{
    UINT16 i;
    UINT8 xp[3]={0};
 #if defined(__Product_PIC32MX2_Receiver__)
 if(FG_10ms){
     FG_10ms = 0;
     if(TIME_Receiver_Login_restrict)--TIME_Receiver_Login_restrict;
       else if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1));
          else {TIME_Receiver_Login=0;COUNT_Receiver_Login=0;}

         if(Receiver_Login==0){
             TIME_Receiver_Login++;
             TIME_Receiver_Login_restrict=350;
             if(COUNT_Receiver_Login>=2)FLAG_ID_Login=1;
             if(((FLAG_ID_Erase_Login==1)&&(COUNT_Receiver_Login>=1))||
                ((FLAG_ID_Login==1)&&(COUNT_Receiver_Login>=3))){FLAG_ID_Login_EXIT=1;FLAG_ID_Login_OK=0;FLAG_ID_Login_OK_bank=0;FLAG_ID_Login=0;FLAG_ID_Erase_Login=0;Receiver_LED_OUT=0;}
         }
         if(Receiver_Login==1){
             if(TIME_Receiver_Login>3)COUNT_Receiver_Login++;
             if(FLAG_ID_Login_EXIT==1){FLAG_ID_Login_EXIT=0;COUNT_Receiver_Login=0;}
             TIME_Receiver_Login=0;
         }
         if(TIME_Receiver_Login>=300){
             TIME_Receiver_Login=0;
             FLAG_ID_Erase_Login=1;
         }
         if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1)){
             TIME_Receiver_Login_led++;
             if(TIME_Receiver_Login_led>=30){
                 TIME_Receiver_Login_led=0;
                 Receiver_LED_OUT=!Receiver_LED_OUT;
             }
             if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0)){
                 FLAG_ID_Login_OK_bank=1;
                 if(FLAG_IDCheck_OK==1)FLAG_IDCheck_OK=0;
                 else{
                     Receiver_LED_OUT=1;
                     for(i=0;i<4160;i++){
                         Receiver_Buzzer=!Receiver_Buzzer;   //������Ƶ��4.17KHZ
                         Delayus(190);
                     }
                     Receiver_Buzzer=0;
                     Receiver_LED_OUT=0;
                     if(FLAG_ID_Login==1)ID_EEPROM_write();
                     else if(FLAG_ID_Erase_Login==1){ID_DATA_PCS=0;ID_EEPROM_write();}
                 }//end else
             }//  end  if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0))
         } //end if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))
 }
#endif
#if defined(__Product_PIC32MX2_WIFI__)
 if(FG_10ms){
     FG_10ms = 0;
     if(TIME_Receiver_Login_restrict)--TIME_Receiver_Login_restrict;
       else if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1));
          else {TIME_Receiver_Login=0;COUNT_Receiver_Login=0;}

         if(WIFI_L_Login==0){
             TIME_Receiver_Login++;
             TIME_Receiver_Login_restrict=350;
             if(COUNT_Receiver_Login>=2)FLAG_ID_Login=1;
             if(((FLAG_ID_Erase_Login==1)&&(COUNT_Receiver_Login>=1))||
                ((FLAG_ID_Login==1)&&(COUNT_Receiver_Login>=3))){FLAG_ID_Login_EXIT=1;FLAG_ID_Login_OK=0;FLAG_ID_Login_OK_bank=0;FLAG_ID_Login=0;FLAG_ID_Erase_Login=0;WIFI_LED_RX=0;}
         }
         if(WIFI_L_Login==1){
             if(TIME_Receiver_Login>3)COUNT_Receiver_Login++;
             if(FLAG_ID_Login_EXIT==1){FLAG_ID_Login_EXIT=0;COUNT_Receiver_Login=0;}
             TIME_Receiver_Login=0;
         }
         if(TIME_Receiver_Login>=300){
             TIME_Receiver_Login=0;
             FLAG_ID_Erase_Login=1;
         }
         if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1)){
             TIME_Receiver_Login_led++;
             if(TIME_Receiver_Login_led>=30){
                 TIME_Receiver_Login_led=0;
                 WIFI_LED_RX=!WIFI_LED_RX;
             }
             if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0)){
                 FLAG_ID_Login_OK_bank=1;
                     WIFI_LED_RX=1;
                     for(i=0;i<8000;i++){        //4160
                         Delayus(190);       //4.17KHZ
                     }
                     WIFI_LED_RX=0;
                 if(FLAG_IDCheck_OK==1)FLAG_IDCheck_OK=0;
                 else{
                     if(FLAG_ID_Login==1)ID_EEPROM_write();
                     else if(FLAG_ID_Erase_Login==1){ID_DATA_PCS=0;ID_EEPROM_write();}
                 }//end else
             }//  end  if((FLAG_ID_Login_OK==1)&&(FLAG_ID_Login_OK_bank==0))
         } //end if((FLAG_ID_Erase_Login==1)||(FLAG_ID_Login==1))
 }
#endif
}


void ID_EEPROM_Initial(void)
{
    UINT16 i;
    UINT8 xm[3]={0};
    uni_rom_id xn;
    for(i=0;i<256;i++)ID_Receiver_DATA[ID_DATA_PCS]=0;
    Read(&xm[0],0x7FE,2);
    ID_DATA_PCS=xm[1]*256+xm[0];
    if(ID_DATA_PCS==0xFFFF)ID_DATA_PCS=0;
    for(i=0;i<ID_DATA_PCS;i++){
        Read(&xm[0],i*3,3);
        xn.IDB[0]=xm[0];
        xn.IDB[1]=xm[1];
        xn.IDB[2]=xm[2];
        xn.IDB[3]=0;
        ID_Receiver_DATA[i]=xn.IDL;
    }
}

void eeprom_IDcheck(void)
{
    UINT16 i;
   for(i=0;i<ID_DATA_PCS;i++){
       if(ID_Receiver_DATA[i]==DATA_Packet_ID){i=ID_DATA_PCS;FLAG_IDCheck_OK=1;}
       if(FLAG_ID_Erase_Login==1){i=ID_DATA_PCS;FLAG_IDCheck_OK=0;}
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
void ID_EEPROM_write(void)
{
    UINT8 xm[3]={0};
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
     Write(&xm[0],(ID_DATA_PCS-1)*3,3);//д�����ݵ�24LC16
     Delay100us(100);            //д����ʱ��  24LCΪ5ms,24c��24wcΪ10ms

}

//===================start_i2c()�����ߺ���=======//
void start_i2c(void)
{
 SDA=1; //������ʼ�����������ź�
 NOP();
 NOP();
 SCL=1;
 Delayus(5);  //>4.7us
 SDA=0; //������ʼ�ź�
 Delayus(4);  //>4us
 SCL=0; //ǯסI2C����,׼���������ݻ��������
 NOP();
 NOP();
 }
//==================stop_i2c()ֹͣ���ߺ���=========//
void stop_i2c(void)
{
 SDA = 0; //���ͽ��������������ź�
 Delayus(5);  //>4.7us
 SCL = 1;
 Delayus(4);//>4us
 SDA = 1;
 Delayus(5);  //>4.7us
}
//===============Mack()����Ӧ���ź�===============//
void Mack(void) //����Ӧ���ź�
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
     ack = 1; //��ASK=1Ϊ��Ӧ���ź�
 SCL = 0;
 NOP();
 NOP();
 SDAIO = 0;
}
//=============send_byte()�ֽ�д����===============//
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
//==============Write()дN�ֽ�����=================//
void Write(UINT8 *s,UINT16 suba,UINT8 n)                //24LC16B
{
 UINT8 i;
 start_i2c(); //��������
 i=suba/256;
 i=i*2+0xa0;
 send_byte(i); //����������ַ
 i=suba%256;
 send_byte(i); //�����ֵ�ַ
 for (i = 0;i < n;i++)
     {
      send_byte(*s); //�����ֽ�����
      s++;
     }
 stop_i2c(); //����ֹͣλ
}
//===============receive_byte()�ֽڶ�����=================//
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
void N0ack(char a) //����Ӧ��λ
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
 start_i2c(); //��������
 i=suba/256;
 i=i*2+0xa0;
 send_byte(i);//����������ַ
 j=suba%256;
 send_byte(j); //�����ֵ�ַ
 start_i2c(); //������������
 i=i+1;
 send_byte(i); //���Ͷ������������ַ
 for(i = 0;i < n - 1;i++)
     {
      SDAIO = 1;
      *s = receive_byte(); //�������
      s++;
      N0ack(0); //����Ӧ���ź�
     }
 SDAIO = 1;
 *s = receive_byte(); //�������
 N0ack(1); //���ͷ�Ӧ��λ
 stop_i2c(); //����ֹͣλ
}
