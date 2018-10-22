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

#if defined(__Product_PIC32MX2_WIFI__)
//用法 Set_Time(number_time);或Read_Time(number_time);
#define   IIC_DELAY             delay(50)
#define   IIC_DELAY_LONG             delay(5000)
#define IIC_SDA_HIGH()			LATCbits.LATC2 = 1
#define IIC_SDA_LOW()			LATCbits.LATC2 = 0
#define IIC_SDA                              ((PORTC&0X04)>>2)
#define IIC_SCL_HIGH()			LATCbits.LATC1 = 1
#define IIC_SCL_LOW()			LATCbits.LATC1 = 0
#define IIC_SDA_Output()                 TRISCbits.TRISC2 = 0;//输出
#define IIC_SDA_Input()                 TRISCbits.TRISC2 = 1;//输入
#define IIC_SCL_Output()                 TRISCbits.TRISC1 = 0;//输出
#define READ_SDA()                      (PORTC&0X04)

unsigned char number_time[7]={0,0,0,1,1,1,0x13};/*0-秒 1-分 2-小时 3-日期  4-周  5-月 6-年*/

void delay(unsigned int nCount)
{
  for (; nCount != 0; nCount--);
}
 void  IIC_Start(void)
 {
         IIC_DELAY;

         IIC_DELAY;
         IIC_SDA_Output();
         IIC_DELAY;
          IIC_SDA_HIGH();
          IIC_SCL_HIGH();
          IIC_DELAY;
          IIC_SDA_LOW();
          IIC_DELAY;
          IIC_SCL_LOW();
 }
void IIC_WaitAsk(void)
 {
  		unsigned int Tp_i=0;

               IIC_SDA_Output();
                IIC_DELAY;
                IIC_SDA_HIGH();
               // IIC_SDA_LOW();
                IIC_DELAY;
               // IIC_SDA_Input();
                //IIC_DELAY;
                //IIC_SDA_HIGH();

               IIC_SCL_HIGH();
		IIC_DELAY;

                IIC_SDA_Input();
                IIC_DELAY;



		while(READ_SDA())
		{
		   	Tp_i++;
			if(Tp_i > 1000)
			{
			   break;
		    }
		}

                 IIC_SCL_LOW();
                IIC_DELAY;


 }

 void IIC_SendOneByte(unsigned char x_data)
 {
       unsigned char  Tp_i;
	   IIC_SDA_Output();
           IIC_DELAY;
	   for(Tp_i=0;Tp_i<8;Tp_i++)
	   {
	      if(x_data&0x80)
		  {
		      IIC_SDA_HIGH();
		  }
		  else
		  {
		      IIC_SDA_LOW();
		  }
		  IIC_DELAY;
		  IIC_SCL_HIGH();
		  IIC_DELAY;
		  x_data = x_data<<1;
		  IIC_SCL_LOW();
	   }
        IIC_WaitAsk();

 }


void IIC_WriteAsk(unsigned char  ack)
{
     IIC_SDA_Output();
     IIC_DELAY;
     if(ack == 0)
     {
         IIC_SDA_LOW();
     }
     else
     {
        IIC_SDA_HIGH();
     }

    IIC_DELAY;
     IIC_SCL_HIGH();
    IIC_DELAY;
     IIC_SCL_LOW();
}

 void IIC_Over(void)
 {
      IIC_DELAY;
     IIC_SDA_Output();
      IIC_DELAY;
      IIC_SDA_LOW();
      IIC_SCL_LOW();
     IIC_DELAY;
      IIC_SCL_HIGH();
      IIC_DELAY;
      IIC_SDA_HIGH();
      IIC_DELAY;
 }


 unsigned char Readbyte()
  {
     unsigned char i,bytedata=0;
      IIC_SDA_Output();
      IIC_DELAY;
      IIC_SDA_HIGH();
      IIC_DELAY;
      IIC_SDA_Input();
      IIC_DELAY;
     for(i=0;i<8;i++)
     {


         IIC_SCL_HIGH();
         IIC_DELAY;
         bytedata<<=1;
         bytedata|=IIC_SDA ;
         IIC_SCL_LOW();
         IIC_DELAY;
     }

     return(bytedata);
 }

void  IIC_SendData_pcf8563(unsigned char x_addr,unsigned char x_data)
 {
          unsigned char tp_deviceaddr = 0xa2;


	  IIC_Start();
	  IIC_SendOneByte(tp_deviceaddr);
	  IIC_SendOneByte(x_addr);
	  IIC_SendOneByte(x_data);
	  IIC_Over();

}





unsigned char  read_pcf8563(unsigned  char address) //读当时的时，分，钞
{
            unsigned char  rdata;
            IIC_Start();
            IIC_SendOneByte(0xa2); /*
            写命令
            */
             IIC_SendOneByte(address); /*
                写地址
            */
           IIC_Start();
           IIC_SendOneByte(0xa3); /*
读命令
*/
           rdata=Readbyte();
          IIC_WriteAsk(1);
          IIC_Over();
         return(rdata);

 }
void ReadData1(unsigned  char address,unsigned  char count,unsigned  char * buff) /*多字节*/
{
    unsigned  char i;
    IIC_Start();
    IIC_SendOneByte(0xa2); /*写命令*/
    IIC_SendOneByte(address); /*写地址*/
    IIC_Start();
    IIC_SendOneByte(0xa3); /*读命令*/
    for(i=0;i<count;i++)
    {
        buff[i]=Readbyte();
        if(i<count-1)
            IIC_WriteAsk(0);
    }
    IIC_WriteAsk(1);
    IIC_Over();
}



 void PCF8563_Init(void)
 {
        IIC_SDA_Output();
        IIC_SCL_Output();
        IIC_SDA_HIGH();
        IIC_SCL_HIGH();

          //  IIC_SendData_pcf8563(0X00,0X00); //qidong
          //  reg_08 = read_pcf8563(0x08);
           //  while( reg_08!=0x13)
           //  {}

 }


 void Set_Time(unsigned char *time_arr)
 {
          IIC_SendData_pcf8563(0X00,0X00);//guanbi
          IIC_DELAY_LONG;
           IIC_SendData_pcf8563(0X01,0X00);//guanbi
          IIC_DELAY_LONG;
           IIC_SendData_pcf8563(0X02,time_arr[0]);//sec 0
          IIC_DELAY_LONG;
           IIC_SendData_pcf8563(0X03,time_arr[1]);//min 0
           IIC_DELAY_LONG;
          IIC_SendData_pcf8563(0X04,time_arr[2]);//hour 0x10
           IIC_DELAY_LONG;
            IIC_SendData_pcf8563(0X05,time_arr[3]);//day 0X06
          IIC_DELAY_LONG;
          IIC_SendData_pcf8563(0X06,time_arr[4]);//week 0x03
          IIC_DELAY_LONG;
           IIC_SendData_pcf8563(0X07,time_arr[5]);//month 0x10
           IIC_DELAY_LONG;
           IIC_SendData_pcf8563(0X08,time_arr[6]);//year
             IIC_DELAY_LONG;
 }

 void Read_Time(unsigned char *time_arr)
 {
        ReadData1(0x02,7,time_arr);
          time_arr[0]= time_arr[0]&0x7F;
          time_arr[1]= time_arr[1]&0x7F;
          time_arr[2]= time_arr[2]&0x3F;
          time_arr[3]= time_arr[3]&0x3F;
          time_arr[4]= time_arr[4]&0x07;
          time_arr[5]=time_arr[5]&0x1F;
          time_arr[6]= time_arr[6];
 }
#endif