/****************************************************************************
/*                    #define Functions                             */
/*  FILE        :pcf8563 .h                                                  */
/*  DATE        :Mar, 2013                                                  */
/*  Programmer	:xiang 'R                                                   */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                               */
/*  Mark        :ver 1.0                                                    */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>				// ����C����

#if defined(__Product_PIC32MX2_WIFI__)
extern unsigned char number_time[7];
void PCF8563_Init(void);
void Set_Time(unsigned char *);
void Read_Time(unsigned char *);
#endif