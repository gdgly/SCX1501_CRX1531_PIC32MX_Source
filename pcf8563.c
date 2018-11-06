/***********************************************************************/
/*  FILE        :.c                                               */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化

unsigned char number_time[7]={0,0,0,1,1,1,0x13};/*0-秒 1-分 2-小时 3-日期  4-周  5-月 6-年*/

#if defined(__Product_PIC32MX2_WIFI__)
//用法 Set_Time(number_time);或Read_Time(number_time);

//#define   IIC_DELAY             delay(50)
//#define   IIC_DELAY_LONG             delay(5000)
//#define IIC_SDA_HIGH()			LATCbits.LATC2 = 1
//#define IIC_SDA_LOW()			LATCbits.LATC2 = 0
//#define IIC_SDA                              ((PORTC&0X04)>>2)
//#define IIC_SCL_HIGH()			LATCbits.LATC1 = 1
//#define IIC_SCL_LOW()			LATCbits.LATC1 = 0
//#define IIC_SDA_Output()                 TRISCbits.TRISC2 = 0;//输出
//#define IIC_SDA_Input()                 TRISCbits.TRISC2 = 1;//输入
//#define IIC_SCL_Output()                 TRISCbits.TRISC1 = 0;//输出
//#define READ_SDA()                      (PORTC&0X04)

//void delay(unsigned int nCount)
//{
//  for (; nCount != 0; nCount--);
//}
// void  IIC_Start(void)
// {
//         IIC_DELAY;
//
//         IIC_DELAY;
//         IIC_SDA_Output();
//         IIC_DELAY;
//          IIC_SDA_HIGH();
//          IIC_SCL_HIGH();
//          IIC_DELAY;
//          IIC_SDA_LOW();
//          IIC_DELAY;
//          IIC_SCL_LOW();
// }
//void IIC_WaitAsk(void)
// {
//  		unsigned int Tp_i=0;
//
//               IIC_SDA_Output();
//                IIC_DELAY;
//                IIC_SDA_HIGH();
//               // IIC_SDA_LOW();
//                IIC_DELAY;
//               // IIC_SDA_Input();
//                //IIC_DELAY;
//                //IIC_SDA_HIGH();
//
//               IIC_SCL_HIGH();
//		IIC_DELAY;
//
//                IIC_SDA_Input();
//                IIC_DELAY;
//
//
//
//		while(READ_SDA())
//		{
//		   	Tp_i++;
//			if(Tp_i > 1000)
//			{
//			   break;
//		    }
//		}
//
//                 IIC_SCL_LOW();
//                IIC_DELAY;
//
//
// }
//
// void IIC_SendOneByte(unsigned char x_data)
// {
//       unsigned char  Tp_i;
//	   IIC_SDA_Output();
//           IIC_DELAY;
//	   for(Tp_i=0;Tp_i<8;Tp_i++)
//	   {
//	      if(x_data&0x80)
//		  {
//		      IIC_SDA_HIGH();
//		  }
//		  else
//		  {
//		      IIC_SDA_LOW();
//		  }
//		  IIC_DELAY;
//		  IIC_SCL_HIGH();
//		  IIC_DELAY;
//		  x_data = x_data<<1;
//		  IIC_SCL_LOW();
//	   }
//        IIC_WaitAsk();
//
// }
//
//
//void IIC_WriteAsk(unsigned char  ack)
//{
//     IIC_SDA_Output();
//     IIC_DELAY;
//     if(ack == 0)
//     {
//         IIC_SDA_LOW();
//     }
//     else
//     {
//        IIC_SDA_HIGH();
//     }
//
//    IIC_DELAY;
//     IIC_SCL_HIGH();
//    IIC_DELAY;
//     IIC_SCL_LOW();
//}
//
// void IIC_Over(void)
// {
//      IIC_DELAY;
//     IIC_SDA_Output();
//      IIC_DELAY;
//      IIC_SDA_LOW();
//      IIC_SCL_LOW();
//     IIC_DELAY;
//      IIC_SCL_HIGH();
//      IIC_DELAY;
//      IIC_SDA_HIGH();
//      IIC_DELAY;
// }
//
//
// unsigned char Readbyte()
//  {
//     unsigned char i,bytedata=0;
//      IIC_SDA_Output();
//      IIC_DELAY;
//      IIC_SDA_HIGH();
//      IIC_DELAY;
//      IIC_SDA_Input();
//      IIC_DELAY;
//     for(i=0;i<8;i++)
//     {
//
//
//         IIC_SCL_HIGH();
//         IIC_DELAY;
//         bytedata<<=1;
//         bytedata|=IIC_SDA ;
//         IIC_SCL_LOW();
//         IIC_DELAY;
//     }
//
//     return(bytedata);
// }
//
//void  IIC_SendData_pcf8563(unsigned char x_addr,unsigned char x_data)
// {
//          unsigned char tp_deviceaddr = 0xa2;
//
//
//	  IIC_Start();
//	  IIC_SendOneByte(tp_deviceaddr);
//	  IIC_SendOneByte(x_addr);
//	  IIC_SendOneByte(x_data);
//	  IIC_Over();
//
//}
//
//
//
//
//
//unsigned char  read_pcf8563(unsigned  char address) //读当时的时，分，钞
//{
//            unsigned char  rdata;
//            IIC_Start();
//            IIC_SendOneByte(0xa2); /*
//            写命令
//            */
//             IIC_SendOneByte(address); /*
//                写地址
//            */
//           IIC_Start();
//           IIC_SendOneByte(0xa3); /*
//读命令
//*/
//           rdata=Readbyte();
//          IIC_WriteAsk(1);
//          IIC_Over();
//         return(rdata);
//
// }
//void ReadData1(unsigned  char address,unsigned  char count,unsigned  char * buff) /*多字节*/
//{
//    unsigned  char i;
//    IIC_Start();
//    IIC_SendOneByte(0xa2); /*写命令*/
//    IIC_SendOneByte(address); /*写地址*/
//    IIC_Start();
//    IIC_SendOneByte(0xa3); /*读命令*/
//    for(i=0;i<count;i++)
//    {
//        buff[i]=Readbyte();
//        if(i<count-1)
//            IIC_WriteAsk(0);
//    }
//    IIC_WriteAsk(1);
//    IIC_Over();
//}
//
//
//
// void PCF8563_Init(void)
// {
//        IIC_SDA_Output();
//        IIC_SCL_Output();
//        IIC_SDA_HIGH();
//        IIC_SCL_HIGH();
//
//          //  IIC_SendData_pcf8563(0X00,0X00); //qidong
//          //  reg_08 = read_pcf8563(0x08);
//           //  while( reg_08!=0x13)
//           //  {}
//
// }
//==============Write()写N字节数据=================//
void Write_pcf8563(UINT8 *s,UINT16 suba,UINT8 n)                //24LC16B
{
 UINT8 i;
 start_i2c(); //启动总线
 send_byte(0xa2); //发送器件地址
 send_byte(suba); //发送字地址
 for (i = 0;i < n;i++)
     {
      send_byte(*s); //发送字节数据
      s++;
     }
 stop_i2c(); //发送停止位
}
void Read_pcf8563(UINT8 *s,UINT16 suba,UINT8 n)
{
 UINT8 i ,j;
 start_i2c(); //启动总线
 send_byte(0xa2);//发送器件地址
 send_byte(suba); //发送字地址
 start_i2c(); //重新启动总线
 send_byte(0xa3); //发送读命令和器件地址
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
#endif
void alarm_pcf8563(unsigned char *time_alarm)
{
#if defined(__Product_PIC32MX2_WIFI__)
     UINT8 data_i[4]={0x80,0x80,0x80,0x80};
          data_i[0]=time_alarm[0];
          //data_i[1]=time_alarm[1];
          Write_pcf8563(&data_i[0],0x09,4);
          Delayus(10);
          data_i[0]=2;
          Write_pcf8563(&data_i[0],0x01,1);
#endif
}
#if defined(__Product_PIC32MX2_WIFI__)
UINT16 Hex_Decimal(UINT8 value_h,UINT8 value_m)
{
    UINT8 x_h0,x_h1,x_m0,x_m1;
    UINT16 x_hm;
    x_h0=value_h>>4;
    x_h0=x_h0*10;
    x_h1=value_h&0x0F;
    x_h0=x_h0+x_h1;

    x_m0=value_m>>4;
    x_m0=x_m0*10;
    x_m1=value_m&0x0F;
    x_m0=x_m0+x_m1;

    x_hm=x_h0*60+x_m0;
    return x_hm;
}
#endif
void NEW_set_alarm_pcf8563(UINT16 value0)
{
#if defined(__Product_PIC32MX2_WIFI__)
    UINT8 i,i1;
    UINT16 alarm_Minutes,time_differ;
    UINT16 Compare_data0=0xFFFF;
    UINT8 xl[2]={0xFF,0xFF};

    for(i=0;i<22;i++){
        if(i<12){
                if((WIFI_alarm_data[i][1]==0x00)||(WIFI_alarm_data[i][3]==0xFF)||(WIFI_alarm_data[i][4]==0xFF));
                else if(WIFI_alarm_data[i][1]==0x01){
                    //alarm_Minutes=WIFI_alarm_data[i][3]*60+WIFI_alarm_data[i][4];
                    alarm_Minutes=Hex_Decimal(WIFI_alarm_data[i][3],WIFI_alarm_data[i][4]);    //2014.10.11修改   解决TIMER有时不动作
                    if(alarm_Minutes>value0)time_differ=alarm_Minutes-value0;
                    //else if(alarm_Minutes<value0)time_differ=alarm_Minutes+1440-value0;
                    //else if(alarm_Minutes==value0);//相等时控制输出，后面追加控制输出代码
                    else if(alarm_Minutes<=value0)time_differ=alarm_Minutes+1440-value0;
                    if((time_differ<Compare_data0)&&(time_differ>0)){Compare_data0=time_differ;xl[1]=WIFI_alarm_data[i][3];xl[0]=WIFI_alarm_data[i][4];}
                }
        }
        else {
                i1=i-12;
                if((Emial_time_data[i1][1]==0x00)||(Emial_time_data[i1][2]==0xFF)||(Emial_time_data[i1][3]==0xFF));
                else if(Emial_time_data[i1][1]==0x01){
                    //alarm_Minutes=Emial_time_data[i1][2]*60+Emial_time_data[i1][3];
                     alarm_Minutes=Hex_Decimal(Emial_time_data[i1][2],Emial_time_data[i1][3]);    //2014.10.11修改   解决TIMER有时不动作
                    if(alarm_Minutes>value0)time_differ=alarm_Minutes-value0;
                    //else if(alarm_Minutes<value0)time_differ=alarm_Minutes+1440-value0;
                    //else if(alarm_Minutes==value0);//相等时控制输出，后面追加控制输出代码
                    else if(alarm_Minutes<=value0)time_differ=alarm_Minutes+1440-value0;
                    if((time_differ<Compare_data0)&&(time_differ>0)){Compare_data0=time_differ;xl[1]=Emial_time_data[i1][2];xl[0]=Emial_time_data[i1][3];}
                }
        }
    }
    if((xl[0]!=0xFF)&&(xl[1]!=0xFF)){
        alarm_pcf8563(&xl[0]);
        WIFI_alarm_Hours_Minutes[0]=xl[1];
        WIFI_alarm_Hours_Minutes[1]=xl[0];
    }

//    for(i=0;i<WIFI_alarm_data_PCS;i++){
//        alarm_Minutes=WIFI_alarm_data[i][8]*60+WIFI_alarm_data[i][9];
//        if(alarm_Minutes>value0)time_differ=alarm_Minutes-value0;
//        else if(alarm_Minutes<value0)time_differ=alarm_Minutes+1440-value0;
//        else if(alarm_Minutes==value0);//相等时控制输出，后面追加控制输出代码
//        if((time_differ<Compare_data0)&&(time_differ>0)){Compare_data0=time_differ;xl[1]=WIFI_alarm_data[i][8];xl[0]=WIFI_alarm_data[i][9];}
//    }
//    if((xl[0]!=0xFF)&&(xl[1]!=0xFF)){
//        alarm_pcf8563(&xl[0]);
//        WIFI_alarm_Hours_Minutes[0]=xl[1];
//        WIFI_alarm_Hours_Minutes[1]=xl[0];
//    }
#endif
}
//void alarm_OUT_bak(UINT16 value10)
//{
//#if defined(__Product_PIC32MX2_WIFI__)
//    UINT16 APP_i;
//
//    FLAG_AUTO_SEND_START=1;
//    //FLAG_AUTO_SEND_ok=0;
//    AUTO_SEND_DATA_pcs++;
//    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0]=WIFI_alarm_data[value10][0];
//    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1]=WIFI_alarm_data[value10][1];
//    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2]=WIFI_alarm_data[value10][2];
//    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][3]=WIFI_alarm_data[value10][3];
//        if(AUTO_SEND_DATA_pcs>1){
//            for(APP_i=0;APP_i<AUTO_SEND_DATA_pcs-1;APP_i++){
//                    if((AUTO_SEND_DATA[APP_i][0]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0])&&(AUTO_SEND_DATA[APP_i][1]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1])&&(AUTO_SEND_DATA[APP_i][2]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2])){
//                        AUTO_SEND_DATA[APP_i][0]=0;
//                        AUTO_SEND_DATA[APP_i][1]=0;
//                        AUTO_SEND_DATA[APP_i][2]=0;
//                        AUTO_SEND_DATA[APP_i][3]=0;
//                    }
//            }
//        }
//#endif
//}
void alarm_OUT_to_AUTO(UINT16 value10,UINT16 value11)
{
#if defined(__Product_PIC32MX2_WIFI__)
    UINT16 APP_i;

    FLAG_AUTO_SEND_START=1;
    //FLAG_AUTO_SEND_ok=0;
    AUTO_SEND_DATA_pcs++;
    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0]=WIFI_alarm_data[value10][7+value11*3];
    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1]=WIFI_alarm_data[value10][8+value11*3];
    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2]=WIFI_alarm_data[value10][9+value11*3];
    AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][3]=WIFI_alarm_data[value10][2];
        if(AUTO_SEND_DATA_pcs>1){
            for(APP_i=0;APP_i<AUTO_SEND_DATA_pcs-1;APP_i++){
                    if((AUTO_SEND_DATA[APP_i][0]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0])&&(AUTO_SEND_DATA[APP_i][1]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1])&&(AUTO_SEND_DATA[APP_i][2]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2])){
                        AUTO_SEND_DATA[APP_i][0]=0;
                        AUTO_SEND_DATA[APP_i][1]=0;
                        AUTO_SEND_DATA[APP_i][2]=0;
                        AUTO_SEND_DATA[APP_i][3]=0;
                    }
            }
        }
#endif
}

#if defined(__Product_PIC32MX2_WIFI__)
void one_hour_HA_Inquiry_Function(void)        //2015.1.30追加修改1小时查询一次HA状态
{
    UINT8 i;
    uni_rom_id fg;
    FLAG_AUTO_SEND_START=1;
    for(i=0;i<ID_DATA_PCS;i++){
        AUTO_SEND_DATA_pcs++;
        fg.IDL=ID_Receiver_DATA[i];
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0]=fg.IDB[0];
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1]=fg.IDB[1];
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2]=fg.IDB[2];
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][3]=0x00;
    }
}
#endif

void Emial_time_OUT(UINT16 value10)
{
#if defined(__Product_PIC32MX2_WIFI__)
    uni_rom_id xg;
    UINT16 APP_i;
    UINT8 FLAG0;
    if(ID_Receiver_DATA[value10]!=0){
        xg.IDL=ID_Receiver_DATA[value10];
        FLAG_AUTO_SEND_START=1;
        //FLAG_AUTO_SEND_ok=0;
//        AUTO_SEND_DATA_pcs++;
//        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0]=xg.IDB[0];
//        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1]=xg.IDB[1];
//        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2]=xg.IDB[2];
//        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][3]=0x00;

            FLAG0=0;
            if(AUTO_SEND_DATA_pcs>0){
                    for(APP_i=0;APP_i<AUTO_SEND_DATA_pcs;APP_i++){
                        if((AUTO_SEND_DATA[APP_i][0]==xg.IDB[0])&&(AUTO_SEND_DATA[APP_i][1]==xg.IDB[1])&&(AUTO_SEND_DATA[APP_i][2]==xg.IDB[2]))FLAG0=1;
                    }
                    if(FLAG0==0){
                        for(APP_i=AUTO_SEND_DATA_pcs;APP_i>0;APP_i--){
                             AUTO_SEND_DATA[APP_i][0]=AUTO_SEND_DATA[APP_i-1][0];
                             AUTO_SEND_DATA[APP_i][1]=AUTO_SEND_DATA[APP_i-1][1];
                             AUTO_SEND_DATA[APP_i][2]=AUTO_SEND_DATA[APP_i-1][2];
                             AUTO_SEND_DATA[APP_i][3]=AUTO_SEND_DATA[APP_i-1][3];
                        }
                        AUTO_SEND_DATA_pcs++;
                        AUTO_SEND_DATA[0][0]=xg.IDB[0];
                        AUTO_SEND_DATA[0][1]=xg.IDB[1];
                        AUTO_SEND_DATA[0][2]=xg.IDB[2];
                        AUTO_SEND_DATA[0][3]=0x00;
                    }
            }
            else {
                    AUTO_SEND_DATA_pcs++;
                    AUTO_SEND_DATA[0][0]=xg.IDB[0];
                    AUTO_SEND_DATA[0][1]=xg.IDB[1];
                    AUTO_SEND_DATA[0][2]=xg.IDB[2];
                    AUTO_SEND_DATA[0][3]=0x00;
            }

    }
#endif
}
void APP_UART_OUT(UINT16 value10)
{
#if defined(__Product_PIC32MX2_WIFI__)
    UINT16 APP_i;
    UINT8 FLAG0;

    FLAG_AUTO_SEND_START=1;
    //FLAG_AUTO_SEND_ok=0;
    if(UART1_DATA[11]==0){
        FLAG0=0;
        if(AUTO_SEND_DATA_pcs>0){
                for(APP_i=0;APP_i<AUTO_SEND_DATA_pcs;APP_i++){
                    if((AUTO_SEND_DATA[APP_i][0]==UART1_DATA[13+value10*3])&&(AUTO_SEND_DATA[APP_i][1]==UART1_DATA[14+value10*3])&&(AUTO_SEND_DATA[APP_i][2]==UART1_DATA[15+value10*3]))FLAG0=1;
                }
                if(FLAG0==0){
                    for(APP_i=AUTO_SEND_DATA_pcs;APP_i>0;APP_i--){
                         AUTO_SEND_DATA[APP_i][0]=AUTO_SEND_DATA[APP_i-1][0];
                         AUTO_SEND_DATA[APP_i][1]=AUTO_SEND_DATA[APP_i-1][1];
                         AUTO_SEND_DATA[APP_i][2]=AUTO_SEND_DATA[APP_i-1][2];
                         AUTO_SEND_DATA[APP_i][3]=AUTO_SEND_DATA[APP_i-1][3];
                    }
                    AUTO_SEND_DATA_pcs++;
                    AUTO_SEND_DATA[0][0]=UART1_DATA[13+value10*3];
                    AUTO_SEND_DATA[0][1]=UART1_DATA[14+value10*3];
                    AUTO_SEND_DATA[0][2]=UART1_DATA[15+value10*3];
                    AUTO_SEND_DATA[0][3]=uart_Control_code;//UART1_DATA[11];
                }
        }
        else {
                AUTO_SEND_DATA_pcs++;
                AUTO_SEND_DATA[0][0]=UART1_DATA[13+value10*3];
                AUTO_SEND_DATA[0][1]=UART1_DATA[14+value10*3];
                AUTO_SEND_DATA[0][2]=UART1_DATA[15+value10*3];
                AUTO_SEND_DATA[0][3]=uart_Control_code;//UART1_DATA[11];
        }
    }
    else {
        AUTO_SEND_DATA_pcs++;
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0]=UART1_DATA[13+value10*3];
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1]=UART1_DATA[14+value10*3];
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2]=UART1_DATA[15+value10*3];
        AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][3]=uart_Control_code;//UART1_DATA[11];
        if((AUTO_SEND_DATA_pcs>1)&&(UART1_DATA[11]!=0)){
            for(APP_i=0;APP_i<AUTO_SEND_DATA_pcs-1;APP_i++){
                    if((AUTO_SEND_DATA[APP_i][0]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][0])&&(AUTO_SEND_DATA[APP_i][1]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][1])&&(AUTO_SEND_DATA[APP_i][2]==AUTO_SEND_DATA[AUTO_SEND_DATA_pcs-1][2])){
                        AUTO_SEND_DATA[APP_i][0]=0;
                        AUTO_SEND_DATA[APP_i][1]=0;
                        AUTO_SEND_DATA[APP_i][2]=0;
                        AUTO_SEND_DATA[APP_i][3]=0;
                    }
            }
        }
    }
#endif
}
 void Set_Time(unsigned char *time_arr)
 {
#if defined(__Product_PIC32MX2_WIFI__)
     UINT8 data_i[2]={0x00,0x00};
//          IIC_SendData_pcf8563(0X00,0X00);//guanbi
//          IIC_DELAY_LONG;
//           IIC_SendData_pcf8563(0X01,0X00);//guanbi
//          IIC_DELAY_LONG;
//           IIC_SendData_pcf8563(0X02,time_arr[0]);//sec 0
//          IIC_DELAY_LONG;
//           IIC_SendData_pcf8563(0X03,time_arr[1]);//min 0
//           IIC_DELAY_LONG;
//          IIC_SendData_pcf8563(0X04,time_arr[2]);//hour 0x10
//           IIC_DELAY_LONG;
//            IIC_SendData_pcf8563(0X05,time_arr[3]);//day 0X06
//          IIC_DELAY_LONG;
//          IIC_SendData_pcf8563(0X06,time_arr[4]);//week 0x03
//          IIC_DELAY_LONG;
//           IIC_SendData_pcf8563(0X07,time_arr[5]);//month 0x10
//           IIC_DELAY_LONG;
//           IIC_SendData_pcf8563(0X08,time_arr[6]);//year
//             IIC_DELAY_LONG;

          Write_pcf8563(&data_i[0],0x00,2);
          Delayus(10);
          Write_pcf8563(&time_arr[0],0x02,7);
#endif
 }

 void Read_Time(unsigned char *time_arr)
 {
#if defined(__Product_PIC32MX2_WIFI__)
//        ReadData1(0x02,7,time_arr);
          Read_pcf8563(&time_arr[0],0x02,7);
          time_arr[0]= time_arr[0]&0x7F;
          time_arr[1]= time_arr[1]&0x7F;
          time_arr[2]= time_arr[2]&0x3F;
          time_arr[3]= time_arr[3]&0x3F;
          time_arr[4]= time_arr[4]&0x07;
          time_arr[5]=time_arr[5]&0x1F;
          time_arr[6]= time_arr[6];
#endif
 }
