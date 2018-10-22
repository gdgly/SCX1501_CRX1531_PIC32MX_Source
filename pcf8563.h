/****************************************************************************
/*                    #define Functions                             */
/*  FILE        :pcf8563 .h                                                  */
/*  DATE        :Mar, 2013                                                  */
/*  Programmer	:xiang 'R                                                   */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                               */
/*  Mark        :ver 1.0                                                    */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>				// 常用C定义

#if defined(__Product_PIC32MX2_WIFI__)
extern unsigned char number_time[7];
void PCF8563_Init(void);
#endif

extern unsigned char number_time[7];
extern void Read_Time(unsigned char *time_arr);
extern void Set_Time(unsigned char *time_arr);
extern void alarm_pcf8563(unsigned char *time_alarm);
extern void NEW_set_alarm_pcf8563(UINT16 value0);
extern void alarm_OUT_bak(UINT16 value10);
extern void alarm_OUT_to_AUTO(UINT16 value10,UINT16 value11);
extern void APP_UART_OUT(UINT16 value10);
extern void Emial_time_OUT(UINT16 value10);