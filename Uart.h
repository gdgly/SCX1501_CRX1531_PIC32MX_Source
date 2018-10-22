/***********************************************************************/
/*  FILE        :Uart.H                                             */
/*  DATE        :Mar, 2013                                             */
/*  Programmer	:xiang 'R                                              */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                          */
/*  Mark        :ver 1.0                                               */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>		// 常用C定义

extern void Uart1_Init(void);
extern void UART_Decode(void);
extern void HA_uart_send(void);
extern void HA_uart_send_APP(void);