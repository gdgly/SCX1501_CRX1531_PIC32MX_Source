
/***********************************************************************/
/*  FILE        :uart.c                                               */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义
#include "initial.h"		// 初始化
#include "pcf8563.h"

void HA_uart_send_APP(void);

#define BaudRate 64
#if defined(__Product_PIC32MX2_WIFI__)
const UINT8 wifi_uart[6]={0xBB,0x00,0x06,0x80,0x00,0x00};
const UINT8 Emial_uart[10]={0xBB,0x00,0x20,0x80,0x00,0x00,0x00,0x00,0x00,0x00};
const UINT8 HA_uart_open[5]={79,80,69,78,32};
const UINT8 HA_uart_close[5]={67,76,79,83,69};
const UINT8 HA_uart_err[5]={69,82,82,32,32};

 void UART_DATA_cope(void);
 void uart_send_APP_allID(void);
 void uart_send_APP_Head(void);
 void uart_send_APP_allalarm(void);
 void uart_send_APP_SUN(void);
 void uart_send_APP_Emial_time(void);
 void uart_send_APP_HA_Change(void);
 void uart_send_APP_Public(UINT8 Public_X,UINT8 Public_Y);
#endif

void Uart1_Init(void)
{
#if defined(__Product_PIC32MX2_WIFI__)
    RPA0R=1;         //Set RPA0-->U1TX
    U1RXR=4;         //Set U1RX-->RPB2
        
    U1BRG = BaudRate; //Set Baud rate
    U1STA = 0;
    U1MODEbits.ON = 1; //Enable UART for 8-bit data
    //no parity, 1 Stop bit
    U1STAbits.URXEN= 1; //Enable Transmit and Receive
    U1STAbits.UTXEN= 1;

    IPC8bits.U1IP = 3; // Set priority level=1
    IPC8bits.U1IS = 3; // Set Subpriority level=1
    // Can be done in a single operation by assigning PC2SET = 0x0000000D
    IFS1bits.U1RXIF = 0; // Clear the timer interrupt status flag
    IEC1bits.U1RXIE = 1; // Enable timer interrupts
#endif
}
#if defined(__Product_PIC32MX2_WIFI__)
void __ISR(_UART_1_VECTOR,ipl3)Uart1Handler(void)
{
    uni_i uart_x;
    UART_DATA_buffer[UART_DATA_cnt] = U1RXREG;
    if((FLAG_UART_0xBB==0)&&(UART_DATA_cnt>=1)){
        if(UART_DATA_buffer[UART_DATA_cnt]==0xBB)TIME_UART=2;      //解决开机UART收到乱码，APP正常控制时不能正确收到。
        else if((UART_DATA_buffer[UART_DATA_cnt-1]==0xBB)&&(UART_DATA_buffer[UART_DATA_cnt]==0x00)){TIME_UART=13;UART_DATA_cnt=1;UART_DATA_buffer[0]=0xBB;UART_DATA_buffer[1]=0x00;FLAG_UART_0xBB=1;}
    }
    UART_DATA_cnt++;
//            if(UART_DATA_buffer[6]==0x05){
//                if(UART_DATA_cnt>=15){
//                    UART_DATA_cnt=0;
//                    FLAG_UART_0xBB=0;
//                    for(UART_DATA_i=0;UART_DATA_i<15;UART_DATA_i++)UART1_DATA[UART_DATA_i]=UART_DATA_buffer[UART_DATA_i];
//                    FLAG_UART_R=1;
//                }
//            }
//            else if(UART_DATA_buffer[6]==0x08){
//                if(UART_DATA_cnt>=18){
//                    UART_DATA_cnt=0;
//                    FLAG_UART_0xBB=0;
//                    for(UART_DATA_i=0;UART_DATA_i<18;UART_DATA_i++)UART1_DATA[UART_DATA_i]=UART_DATA_buffer[UART_DATA_i];
//                    FLAG_UART_R=1;
//                }
//            }
//            else if(UART_DATA_buffer[6]==0x00){
//                if(UART_DATA_cnt>=10){
//                    UART_DATA_cnt=0;
//                    FLAG_UART_0xBB=0;
//                    /*****2013年11月22日修改  提高Emial稳定性****/
//                    for(UART_DATA_i=0;UART_DATA_i<10;UART_DATA_i++)UART1_DATA[UART_DATA_i]=UART_DATA_buffer[UART_DATA_i];
//                    FLAG_UART_R=1;
//                    /******************************************/
//                }
//            }
    if((UART_DATA_buffer[2]==0x06)&&(UART_DATA_buffer[3]==0x80)){
        uart_x.uc[0]=UART_DATA_buffer[8];
        uart_x.uc[1]=UART_DATA_buffer[9];
        switch(uart_x.ui){
            case 0x0101:                                           //卷帘门依次单个操作，HA状态取得
            case 0x0102:
                        UART_DATA_i=18;
                        if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        break;
            case 0x0105:                                       //RTC_write
                        UART_DATA_i=20;
                        if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        break;
            case 0x0106:                                    //控制定时器设定要求
                        if(UART_DATA_cnt>=18){
                            if(UART_DATA_cnt==18)UART_DATA_i=20+UART_DATA_buffer[17]*3;
                            if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        }
                        break;
            case 0x0107:                                    //控制定时器取得要求
            case 0x010A:                                    //邮件定时器取得要求
                        UART_DATA_i=14;
                        if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        break;
            case 0x0108:                                   //一齐操作
                        if(UART_DATA_cnt>=13){
                            if(UART_DATA_cnt==13)UART_DATA_i=15+UART_DATA_buffer[12]*3;
                            if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        }
                        break;
            case 0x0109:                                    //邮件定时器设定要求
                        UART_DATA_i=20;
                        if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        break;
            case 0x010B:                                    //日出日落设定要求
                        if(UART_DATA_cnt>=16){
                            if(UART_DATA_cnt==16)UART_DATA_i=18+UART_DATA_buffer[15]*3;
                            if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        }
                        break;
            case 0x0103:                                      //APP获取卷帘门ID全部
            case 0x0104:                                      //RTC_read
            case 0x010C:                                    //日出日落取得要求
            case 0x010E:                                   //卷帘门状态变化是否邮件送信取得
            case 0x010F:                                   //集中通讯机版本取得
            case 0x01F1:                                   //日出日落表格数据DATA取得
                        UART_DATA_i=13;
                        if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        break;
            case 0x010D:                                    //卷帘门状态变化是否邮件送信设定
                        UART_DATA_i=16;
                        if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        break;
            case 0x01F0:                                   //日出日落表格数据DATA设置
                        UART_DATA_i=493;
                        if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
                        break;
           default:
                        break;
        }
    }
    else if((UART_DATA_buffer[2]==0x20)&&(UART_DATA_buffer[3]==0x80)){
          UART_DATA_i=10;
          if(UART_DATA_cnt>=UART_DATA_i)UART_DATA_cope();
    }
    if((UART_DATA_cnt>=493)||((UART_DATA_cnt>2)&&(TIME_UART==0))){UART_DATA_cnt=1;FLAG_UART_0xBB=0;}    //UART_DATA_cnt=0-->1  解决UART收到乱码后，每第一次接收数据失败，后面就好了。
    IFS1bits.U1RXIF = 0;
}
 void UART_DATA_cope(void)
 {
     UINT16 UART_DATA_j;
        UART_DATA_cnt=0;
        FLAG_UART_0xBB=0;
        for(UART_DATA_j=0;UART_DATA_j<UART_DATA_i;UART_DATA_j++)UART1_DATA[UART_DATA_j]=UART_DATA_buffer[UART_DATA_j];
        FLAG_UART_R=1;
 }
#endif
void UART_Decode(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i;
    UINT16 m=0;
    UINT16 n=0;
    uni_rom_id y;
    uni_i uart_y;
    if(UART1_DATA[2]==0x06){                          /*****2013年11月22日修改  提高Emial稳定性****/
            for(i=0;i<6;i++){
                if(UART1_DATA[i]==wifi_uart[i]);
                else {FLAG_UART_ok=0;return;}
            }
            uart_y.uc[0]=UART1_DATA[8];
            uart_y.uc[1]=UART1_DATA[9];
            switch(uart_y.ui){
                case 0x0101:                                           //卷帘门依次单个HA状态取得
                case 0x0102:                                           //卷帘门依次单个操作
                            for(i=8;i<16;i++)  m+=UART1_DATA[i];
                            n=UART1_DATA[16]+UART1_DATA[17]*256;
                            if(m==n){
                                ID_data.IDB[0]=UART1_DATA[11];
                                ID_data.IDB[1]=UART1_DATA[12];
                                ID_data.IDB[2]=UART1_DATA[13];
                                ID_data.IDB[3]=0x00;
                                //UART1_DATA[11];
                                Control_code=UART1_DATA[14];
                                eeprom_IDcheck_UART();
                                if(FLAG_IDCheck_OK==1){
                                    //if(Control_code==0x00){FLAG_HA_Inquiry=1;DATA_Packet_Control_0=0x00;}    //表示APP查询
                                    if((Control_code==0x00)||(Control_code==0x02)||(Control_code==0x08)){FLAG_HA_Inquiry=1;DATA_Packet_Control_0=0x00;}    //表示APP查询
                                    FLAG_IDCheck_OK=0;
                                    FLAG_UART_ok=1;}
                                else {
                                    HA_uart_app[8]=UART1_DATA[8];
                                    HA_uart_app[9]=UART1_DATA[9];
                                    HA_uart_app[10]=0x01;
                                    HA_uart_app[11]=UART1_DATA[11];
                                    HA_uart_app[12]=UART1_DATA[12];
                                    HA_uart_app[13]=UART1_DATA[13];
                                    HA_uart_app[14]=0x00;
                                    HA_uart_app[15]=0x00;
                                    n=n-UART1_DATA[14]+1;
                                    HA_uart_app[16]=n%256;
                                    HA_uart_app[17]=n/256;

                                    for(i=0;i<18;i++){
                                        U1TXREG=HA_uart_app[i];
                                        if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    }
                                }
                            }
                            else {uart_send_APP_Public(UART1_DATA[8],1);FLAG_UART_ok=0;}
                            break;
                case 0x0104:                                            //RTC_read
                case 0x0105:                                           //RTC_write
                            if(UART1_DATA[8]==0x04){
                                for(i=8;i<11;i++)  m+=UART1_DATA[i];
                                n=UART1_DATA[11]+UART1_DATA[12]*256;
                            }
                            else {
                                for(i=8;i<18;i++)  m+=UART1_DATA[i];
                                n=UART1_DATA[18]+UART1_DATA[19]*256;
                            }
                            if(m==n){
                                if(UART1_DATA[8]==0x05){
                                    Set_Time(&UART1_DATA[11]);       //==0x00  写时钟
                                    uart_send_APP_Public(0x05,0);
                                }
                                else if(UART1_DATA[8]==0x04){                //==0x01  读时钟
                                    Read_Time(number_time);
                                    uart_send_APP_Head();
                                    U1TXREG=10;
                                    U1TXREG=0x00;
                                    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    U1TXREG=0x04;
                                    U1TXREG=0x01;
                                    U1TXREG=0x00;
                                    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    m=0;
                                    for(i=0;i<7;i++){U1TXREG=number_time[i];m=m+number_time[i];}
                                    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    m=m+0x05;
                                    U1TXREG=m%256;
                                    U1TXREG=m/256;
                                }
                            }
                            else uart_send_APP_Public(UART1_DATA[8],1);
                            break;
                case 0x0106:                                            //控制定时器设定要求
                case 0x0107:                                           //控制定时器取得要求
                            if(UART1_DATA[8]==0x06){
                                for(i=8;i<18+UART1_DATA[17]*3;i++)  m+=UART1_DATA[i];
                                n=UART1_DATA[18+UART1_DATA[17]*3]+UART1_DATA[19+UART1_DATA[17]*3]*256;
                            }
                            else {
                                 for(i=8;i<12;i++)  m+=UART1_DATA[i];
                                 n=UART1_DATA[12]+UART1_DATA[13]*256;
                            }
                            if(m==n){
                                if(UART1_DATA[8]==0x06){
                                  //if((UART1_DATA[11]==0)||(UART1_DATA[11]>0x0A)||(UART1_DATA[12]>1)||(UART1_DATA[13]>9)||(UART1_DATA[14]>25)||(UART1_DATA[15]>60)||(UART1_DATA[17]==0))
                                  if((UART1_DATA[11]==0)||(UART1_DATA[11]>0x0A)||(UART1_DATA[14]>0x25)||(UART1_DATA[15]>0x60))uart_send_APP_Public(0x06,1);
                                  else {
                                        eeprom_IDcheck_Multiple(17);
                                        if(FLAG_IDCheck_OK==1){
                                            FLAG_IDCheck_OK=0;
                                            alarm_EEPROM_write();
                                            uart_send_APP_Public(0x06,0);
                                        }
                                       else uart_send_APP_Public(0x06,1);
                                  }
                                }
                                else uart_send_APP_allalarm();
                            }
                            else uart_send_APP_Public(UART1_DATA[8],1);
                            break;
                case 0x0109:                                            //邮件定时器设定要求
                case 0x010A:                                           //邮件定时器取得要求
                            if(UART1_DATA[8]==0x09){
                                for(i=8;i<18;i++)  m+=UART1_DATA[i];
                                n=UART1_DATA[18]+UART1_DATA[19]*256;
                            }
                            else {
                                 for(i=8;i<12;i++)  m+=UART1_DATA[i];
                                 n=UART1_DATA[12]+UART1_DATA[13]*256;
                            }
                            if(m==n){
                                if(UART1_DATA[8]==0x09){
                                  if((UART1_DATA[11]==0)||(UART1_DATA[11]>0x0A)||(UART1_DATA[13]>0x25)||(UART1_DATA[14]>0x60))uart_send_APP_Public(0x09,1);
                                  else {
                                            Emial_time_EEPROM_write();
                                            uart_send_APP_Public(0x09,0);
                                  }
                                }
                                else uart_send_APP_Emial_time();
                            }
                            else uart_send_APP_Public(UART1_DATA[8],1);
                            break;
                case 0x010B:                                            //日出日落设定要求
                case 0x010C:                                           //日出日落取得要求
                            if(UART1_DATA[8]==0x0B){
                                for(i=8;i<16+UART1_DATA[15]*3;i++)  m+=UART1_DATA[i];
                                n=UART1_DATA[16+UART1_DATA[15]*3]+UART1_DATA[17+UART1_DATA[15]*3]*256;
                            }
                            else {
                                 for(i=8;i<11;i++)  m+=UART1_DATA[i];
                                 n=UART1_DATA[11]+UART1_DATA[12]*256;
                            }
                            if(m==n){
                                if(UART1_DATA[8]==0x0B){
                                  if((UART1_DATA[11]>1)||(UART1_DATA[12]>1)||(UART1_DATA[13]>10)||(UART1_DATA[14]>0x80)||(UART1_DATA[15]>32))uart_send_APP_Public(0x0B,1);
                                  else {
                                        eeprom_IDcheck_Multiple(15);
                                        if(FLAG_IDCheck_OK==1){
                                            FLAG_IDCheck_OK=0;
                                            SUN_EEPROM_write();
                                            uart_send_APP_Public(0x0B,0);
                                        }
                                        else uart_send_APP_Public(0x0B,1);
                                  }
                                }
                                else uart_send_APP_SUN();
                            }
                            else uart_send_APP_Public(UART1_DATA[8],1);
                            break;
                case 0x010D:                                            //卷帘门状态变化是否邮件送信设定
                case 0x010E:                                           //卷帘门状态变化是否邮件送信取得
                            if(UART1_DATA[8]==0x0D){
                                for(i=8;i<14;i++)  m+=UART1_DATA[i];
                                n=UART1_DATA[14]+UART1_DATA[15]*256;
                            }
                            else {
                                 for(i=8;i<11;i++)  m+=UART1_DATA[i];
                                 n=UART1_DATA[11]+UART1_DATA[12]*256;
                            }
                            if(m==n){
                                if(UART1_DATA[8]==0x0D){
                                      HA_Change_EEPROM_write();
                                      uart_send_APP_Public(0x0D,0);
                                }
                                else uart_send_APP_HA_Change();
                            }
                            else uart_send_APP_Public(UART1_DATA[8],1);
                            break;
                case 0x0103:                                            //APP获取卷帘门ID全部
                            for(i=8;i<11;i++)  m+=UART1_DATA[i];
                            n=UART1_DATA[11]+UART1_DATA[12]*256;
                            if(m==n){
                                uart_send_APP_allID();
                            }
                            else uart_send_APP_Public(0x03,1);
                            break;
                case 0x010F:                                            //APP获取卷帘门ID全部
                            for(i=8;i<11;i++)  m+=UART1_DATA[i];
                            n=UART1_DATA[11]+UART1_DATA[12]*256;
                            if(m==n){
                                    uart_send_APP_Head();
                                    U1TXREG=0x09;
                                    U1TXREG=0x00;
                                    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    U1TXREG=0x0F;
                                    U1TXREG=0x01;
                                    U1TXREG=0x00;
                                    U1TXREG=0x56;      //V
                                    U1TXREG=0x65;      //e
                                    U1TXREG=0x72;      //r
                                    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    U1TXREG=0x32;      //2
                                    U1TXREG=0x2E;      //.
                                    U1TXREG=0x34;      //4
                                    U1TXREG=0xD1;     //0x16B+0x32+0x33
                                    U1TXREG=0x01;      
                            }
                            else uart_send_APP_Public(0x0F,1);
                            break;
                case 0x0108:                                            //一齐操作
                            for(i=8;i<13+UART1_DATA[12]*3;i++)  m+=UART1_DATA[i];
                            n=UART1_DATA[13+UART1_DATA[12]*3]+UART1_DATA[14+UART1_DATA[12]*3]*256;
                            if(m==n){
                                eeprom_IDcheck_Multiple(12);
                                if(FLAG_IDCheck_OK==1){
                                    FLAG_IDCheck_OK=0;
                                    uart_send_APP_Public(0x08,0);
                                    for(i=0;i<UART1_DATA[12];i++) APP_UART_OUT(i);
                                }
                                else uart_send_APP_Public(0x08,1);
                            }
                            else uart_send_APP_Public(0x08,1);
                            break;
                case 0x01F0:                                            //日出日落表格数据DATA设置
                            for(i=8;i<491;i++)  m+=UART1_DATA[i];
                            n=UART1_DATA[491]+UART1_DATA[492]*256;
                            if(m==n){
                                Sunrise_sunset_EEPROM_write();
                                uart_send_APP_Public(0xF0,0);
                            }
                            else uart_send_APP_Public(0xF0,1);
                            break;
                case 0x01F1:                                            //日出日落表格数据DATA取得
                            for(i=8;i<11;i++)  m+=UART1_DATA[i];
                            n=UART1_DATA[11]+UART1_DATA[12]*256;
                            if(m==n){
                                    uart_send_APP_Head();
                                    U1TXREG=0xE3;
                                    U1TXREG=0x01;
                                    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    U1TXREG=0xF1;
                                    U1TXREG=0x01;
                                    U1TXREG=0x00;
                                    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    m=0;
                                    for(i=0;i<480;i++){
                                        U1TXREG=Sunrise_sunset_DATA[i];
                                        m=m+Sunrise_sunset_DATA[i];
                                        if(i%6==5)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                                    }
                                    m=m+0xF2;
                                    U1TXREG=m%256;
                                    U1TXREG=m/256;
                            }
                            else uart_send_APP_Public(0xF1,1);
                            break;
                default:
                            break;
            }
    }
                      /*****2013年11月22日修改  提高Emial稳定性****/
    else if(UART1_DATA[2]==0x20){
            for(i=0;i<10;i++){
                    if(UART1_DATA[i]==Emial_uart[i]);
                    else return;
            }
            FLAG_email_Repeat=0;
    }
                   /******************************************/
#endif
}

void HA_uart_email(UINT8 EMIAL_id_PCS_x)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT8 h,l;
    UINT8 h1,bc[8];
    UINT16 m,i,j;
    UINT32 h0;

    //uart_send_APP_Public(0xFF,0);               //测试是否发送了邮件
    Delay100us(30);

//    HA_uart[8]=HA_Change_send_email[1];       //调用该该函数之前  对接收邮件地址编号进行设置
//    HA_uart[9]=HA_Change_send_email[2];

    Read_Time(number_time);           //计算邮件标题
    for(i=1;i<7;i++){
        if(i!=4){
            h=number_time[i];
            if(i<=3)j=24-(i-1)*3;
            else j=24-(i-2)*3;
            l=h&0x0F;
            HA_uart[j+1]=l+0x30;
            h=h&0xF0;
            h=h>>4;
            HA_uart[j]=h+0x30;
        }
    }
    HA_uart[26]=0x00;   //邮件标题结束符

    HA_uart_Length=63+8;
    //HA_uart_Length=63+8+2;
    for(j=0;j<EMIAL_id_PCS_x;j++){                      //计算邮件内容的atatus=...部分
        if((EMIAL_id_HA[j]==0x81)||(EMIAL_id_HA[j]==0x85)){
            HA_uart[HA_uart_Length]=111;      //open
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=112;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=101;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=110;
            HA_uart_Length++;
        }
        else if((EMIAL_id_HA[j]==0x82)||(EMIAL_id_HA[j]==0x86)){
            HA_uart[HA_uart_Length]=99;      //close
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=108;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=111;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=115;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=101;
            HA_uart_Length++;
        }
        else if((EMIAL_id_HA[j]==0x83)||(EMIAL_id_HA[j]==0x87)){
            HA_uart[HA_uart_Length]=101;      //error
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=114;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=114;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=111;
            HA_uart_Length++;
            HA_uart[HA_uart_Length]=114;
            HA_uart_Length++;
        }
        if(j!=(EMIAL_id_PCS_x-1)){
            HA_uart[HA_uart_Length]=44;   //,
            HA_uart_Length++;
        }
    }

    HA_uart[HA_uart_Length]=38;   //&id=
    HA_uart_Length++;
    HA_uart[HA_uart_Length]=105;
    HA_uart_Length++;
    HA_uart[HA_uart_Length]=100;
    HA_uart_Length++;
    HA_uart[HA_uart_Length]=61;
    HA_uart_Length++;      

    for(j=0;j<EMIAL_id_PCS_x;j++){      //计算邮件内容的&id=...部分
        h0=EMIAL_id_data[j];
        h1=0;
        for(i=8;i>0;i--){
            h=h0%10;
            bc[h1]=h+0x30;
            h1++;
            h0=h0/10;
            if(h0==0)i=1;
        }
        for(i=h1;i>0;i--){
            HA_uart[HA_uart_Length]=bc[i-1];
            HA_uart_Length++;
        }
        if(j!=(EMIAL_id_PCS_x-1)){
            HA_uart[HA_uart_Length]=44;   //,
            HA_uart_Length++;
        }
    }
    HA_uart[HA_uart_Length]=0x00;   //邮件内容结束符
    HA_uart_Length++;

    m=HA_uart_Length-8;            //计算数据长度
    HA_uart[6]=m%256;
    HA_uart[7]=m/256;

    m=0;                       //计算CRC16
    for(i=8;i<HA_uart_Length;i++)m=m+HA_uart[i];
    HA_uart[HA_uart_Length]=m%256;
    HA_uart[HA_uart_Length+1]=m/256;

    j=HA_uart_Length+2;
        for(i=0;i<j;i++){
            U1TXREG=HA_uart[i];
            if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
        }
       Delay100us(300);
       TIME_email_Repeat=9000;
       FLAG_email_Repeat=1;
       UART_send_count=0;

       for(i=0;i<64;i++){
           Email_check_ID[i]=EMIAL_id_data[i];
           EMIAL_id_data[i]=0;
           Emial_check_Control[i]=EMIAL_id_HA[i];
           EMIAL_id_HA[i]=0;
       }
       EMIAL_id_PCS=0;
  // HA_uart_send_APP();


//    UINT8 h,l;
//    UINT16 m,i,j;
//    UINT32 h0;
//
//    m=EMIAL_id_PCS_x*18+18;            //计算数据长度
//    HA_uart[6]=m%256;
//    HA_uart[7]=m/256;
//
//    Read_Time(number_time);           //计算邮件标题
//    for(i=1;i<7;i++){
//        if(i!=4){
//            h=number_time[i];
//            if(i<=3)j=22-(i-1)*3;
//            else j=22-(i-2)*3;
//            l=h&0x0F;
//            HA_uart[j+1]=l+0x30;
//            h=h&0xF0;
//            h=h>>4;
//            HA_uart[j]=h+0x30;
//        }
//    }
//    HA_uart[24]=0x00;   //邮件标题结束符
//
//    for(j=0;j<EMIAL_id_PCS_x;j++){     //计算邮件内容
//        HA_uart[j*18+25]=HA_uart[25];
//        HA_uart[j*18+26]=HA_uart[26];
//        HA_uart[j*18+27]=HA_uart[27];
//        h0=EMIAL_id_data[j];
//        for(i=8;i>0;i--){
//            h=h0%10;
//            HA_uart[j*18+27+i]=h+0x30;
//            h0=h0/10;
//        }
//        HA_uart[j*18+36]=HA_uart[36];
//        for(i=0;i<5;i++){
//            if((EMIAL_id_HA[j]==0x81)||(EMIAL_id_HA[j]==0x85))HA_uart[37+j*18+i]=HA_uart_open[i];
//            else if((EMIAL_id_HA[j]==0x82)||(EMIAL_id_HA[j]==0x86))HA_uart[37+j*18+i]=HA_uart_close[i];
//            else if((EMIAL_id_HA[j]==0x83)||(EMIAL_id_HA[j]==0x87))HA_uart[37+j*18+i]=HA_uart_err[i];
//        }
//        HA_uart[j*18+42]=HA_uart[42];
//    }
//    HA_uart[(EMIAL_id_PCS_x-1)*18+43]=0x00;   //邮件内容结束符
//
//    m=0;                       //计算CRC16
//    j=EMIAL_id_PCS_x*18+26;
//    for(i=8;i<j;i++)m=m+HA_uart[i];
//    HA_uart[(EMIAL_id_PCS_x-1)*18+44]=m%256;
//    HA_uart[(EMIAL_id_PCS_x-1)*18+45]=m/256;
//
//    j=j+2;
//        for(i=0;i<j;i++){
//            U1TXREG=HA_uart[i];
//            if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
//        }
//       Delay100us(300);
//       TIME_email_Repeat=200;
//       FLAG_email_Repeat=0;
//       UART_send_count=0;
//
//       for(i=0;i<64;i++){
//           Email_check_ID[i]=EMIAL_id_data[i];
//           EMIAL_id_data[i]=0;
//           Emial_check_Control[i]=EMIAL_id_HA[i];
//           EMIAL_id_HA[i]=0;
//       }
//       EMIAL_id_PCS=0;
//  // HA_uart_send_APP();
    
 #endif
}
void HA_uart_email_Repeat(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT8 i;
        //uart_send_APP_Public(0xFF,0);               //测试是否发送了邮件
        for(i=0;i<HA_uart_Length+2;i++){
            U1TXREG=HA_uart[i];
            if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
        }
       Delay100us(300);
 #endif
}
void HA_uart_send_APP(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    uni_rom_id b0;
    UINT8 i;
    UINT16 m;

    HA_uart_app[9]=0x01;
    if(UART_DATA_buffer[8]==0x01)HA_uart_app[8]=0x01;
    else if(UART_DATA_buffer[8]==0x02)HA_uart_app[8]=0x02;
    else HA_uart_app[8]=0x01;
    HA_uart_app[10]=0x00;
    b0.IDL=DATA_Packet_ID;
    HA_uart_app[11]=b0.IDB[0];
    HA_uart_app[12]=b0.IDB[1];
    HA_uart_app[13]=b0.IDB[2];
    if((DATA_Packet_Control==0x81)||(DATA_Packet_Control==0x85))HA_uart_app[14]=01;
    else if((DATA_Packet_Control==0x82)||(DATA_Packet_Control==0x86))HA_uart_app[14]=02;
    else if((DATA_Packet_Control==0x83)||(DATA_Packet_Control==0x87))HA_uart_app[14]=03;
    HA_uart_app[15]=0x00;
    m=0;
    for(i=8;i<16;i++)m=m+HA_uart_app[i];
    HA_uart_app[16]=m%256;
    HA_uart_app[17]=m/256;

    for(i=0;i<18;i++){
        U1TXREG=HA_uart_app[i];
        if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
    }
 #endif
}

void uart_send_APP_allID(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i;
    UINT16 d0,d1=0;
    UINT8 xp[3]={0};
    UINT8 xc[4]={0};
    uni_rom_id xk;
             uart_send_APP_Head();
             for(i=0;i<ID_DATA_PCS;i++){
                 if(ID_Receiver_DATA[i]!=0)d1++;
             }
             d0=d1*3+3;
             U1TXREG=d0%256;
             U1TXREG=d0/256;
             Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
             U1TXREG=0x03;
             U1TXREG=0x01;
             U1TXREG=0x00;
             Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
             d0=0x04;
             for(i=0;i<ID_DATA_PCS;i++){
                if(ID_Receiver_DATA[i]!=0){
                    xk.IDL=ID_Receiver_DATA[i];
                    xc[0]=xk.IDB[0];
                    d0=d0+xc[0];
                    U1TXREG=xc[0];
                    xc[1]=xk.IDB[1];
                    d0=d0+xc[1];
                    U1TXREG=xc[1];
                    xc[2]=xk.IDB[2];
                    d0=d0+xc[2];
                    U1TXREG=xc[2];
                    xk.IDB[3]=0;
                    if(i%2==1)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                }
            }
            U1TXREG=d0%256;
            U1TXREG=d0/256;
 #endif
}

void uart_send_APP_allalarm(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i,j;
    UINT16 d0,d1;
    UINT16 d2=0;
                 uart_send_APP_Head();
                 d1=UART1_DATA[11]-1;
                 d0=10+WIFI_alarm_data[d1][6]*3;
                 U1TXREG=d0%256;
                 U1TXREG=d0/256;
                 Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                 U1TXREG=0x07;
                 U1TXREG=0x01;
                 U1TXREG=0x00;
                 d0=0x08;
                 for(i=0;i<7;i++){
                     U1TXREG=WIFI_alarm_data[d1][i];
                     d0=d0+WIFI_alarm_data[d1][i];
                     if(i==2)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                 }
                 d2=WIFI_alarm_data[d1][6]*3;
                 for(i=0;i<d2;i++){
                        d0=d0+WIFI_alarm_data[d1][i+7];
                        U1TXREG=WIFI_alarm_data[d1][i+7];
                        if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                }
                U1TXREG=d0%256;
                U1TXREG=d0/256;


//             for(i=8;i<25;i++){
//                 if(UART1_DATA[i]!=0)d2=1;
//             }
//             if(d2==0){
//                 uart_send_APP_Head();
//                 d0=WIFI_alarm_data_PCS*10-(WIFI_alarm_data_PCS/2)*3;
//                 U1TXREG=d0%256;
//                 U1TXREG=d0/256;
//                 Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
//                 d0=0;
//                 d1=0;
//                 for(i=0;i<WIFI_alarm_data_PCS;i++)
//                     for(j=0;j<10;j++)
//                     {
//                        if((i%2==1)&&(j<3));
//                        else {
//                        d0=d0+WIFI_alarm_data[i][j];
//                        U1TXREG=WIFI_alarm_data[i][j];
//                        d1++;
//                        if(d1%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
//                        }
//                    }
//                U1TXREG=d0%256;
//                U1TXREG=d0/256;
//             }
 #endif
}
void uart_send_APP_SUN(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i,j;
    UINT16 d0,d1;
    UINT16 d2=0;
                 uart_send_APP_Head();
                 d0=8+WIFI_alarm_data[10][6]*3;
                 U1TXREG=d0%256;
                 U1TXREG=d0/256;
                 Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                 U1TXREG=0x0C;
                 U1TXREG=0x01;
                 U1TXREG=0x00;
                 d0=0x0C+0x01;
                 for(i=0;i<3;i++){
                     U1TXREG=SUN_ON_OFF_seat[i];
                     d0=d0+SUN_ON_OFF_seat[i];
                 }
                 Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                 for(i=0;i<2;i++){
                     U1TXREG=WIFI_alarm_data[10][i+5];
                     d0=d0+WIFI_alarm_data[10][i+5];
                 }
                 d2=WIFI_alarm_data[10][6]*3;
                 for(i=0;i<d2;i++){
                        d0=d0+WIFI_alarm_data[10][i+7];
                        U1TXREG=WIFI_alarm_data[10][i+7];
                        if(i%6==0)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                }
                U1TXREG=d0%256;
                U1TXREG=d0/256;
 #endif
}
void uart_send_APP_Emial_time(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i,j;
    UINT16 d0,d1;
    UINT16 d2=0;
                 uart_send_APP_Head();
                 d1=UART1_DATA[11]-1;
                 U1TXREG=0x0A;
                 U1TXREG=0x00;
                 Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                 U1TXREG=0x0A;
                 U1TXREG=0x01;
                 U1TXREG=0x00;
                 d0=0x0A+0x01;
                 for(i=0;i<7;i++){
                     U1TXREG=Emial_time_data[d1][i];
                     d0=d0+Emial_time_data[d1][i];
                     if(i==2)Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
                 }
                U1TXREG=d0%256;
                U1TXREG=d0/256;
 #endif
}
void uart_send_APP_HA_Change(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT16 i;
    UINT16 d0;
             uart_send_APP_Head();
             U1TXREG=0x06;
             U1TXREG=0x00;
             Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
             U1TXREG=0x0E;
             U1TXREG=0x01;
             U1TXREG=0x00;
             d0=0x0F;
             for(i=0;i<3;i++){
                 U1TXREG=HA_Change_send_email[i];
                 d0=d0+HA_Change_send_email[i];
             }
             Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
             U1TXREG=d0%256;
             U1TXREG=d0/256;
 #endif
}

void uart_send_APP_Head(void)
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT8 i;
   for(i=0;i<6;i++)U1TXREG=HA_uart_app[i];
 #endif
}
void uart_send_APP_Public(UINT8 Public_X,UINT8 Public_Y)     //Public_X ->指令类别低字节  Public_Y ->返回结果  0（OK） 1（NG）
{
 #if defined(__Product_PIC32MX2_WIFI__)
    UINT8 i_x;
    uart_send_APP_Head();
    U1TXREG=0x03;
    U1TXREG=0x00;
    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
    U1TXREG=Public_X;
    U1TXREG=0x01;
    U1TXREG=Public_Y;
    i_x=0x01+Public_X+Public_Y;
    U1TXREG=i_x;
    U1TXREG=0x00;
    Delay100us(30);//延时2.1mS以上，缓冲区是8级FIFO
 #endif
}