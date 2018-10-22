/****************************************************************************
/*                    ADF7021 #define Functions                             */
/*  FILE        :ADF7021.h                                                  */
/*  DATE        :Mar, 2013                                                  */
/*  Programmer	:xiang 'R                                                   */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                               */
/*  Mark        :ver 1.0                                                    */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>				// 常用C定义



extern void dd_set_TX_mode(void);
void dd_write_7021_reg(unsigned char* reg_bytes);
extern void dd_set_ADF7021_Power_on(void);
extern void dd_set_RX_mode(void);
extern void dd_set_ADF7021_Freq(UINT8 Mode,UINT8 CH);
ADF70XX_REG_T dd_read_7021_reg(UINT8 readback_config);
extern UINT8 dd_read_rssi_7021_reg(UINT8 readback_config);
extern void dd_read_RSSI(void);
extern void ADF7021_change_TXorRX(void);
extern void dd_set_ADF7021_ReInitial(void);
extern void DIP_switch_Get(void);

//*********以下为测试代码所用****************
extern void dd_set_TX_mode_carrier(void);
extern void dd_set_TX_mode_1010pattern(void);
extern void dd_set_RX_mode_test(void);