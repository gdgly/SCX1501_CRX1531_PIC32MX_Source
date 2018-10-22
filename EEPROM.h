/****************************************************************************
/*                    eeprom #define Functions                              */
/*  FILE        :EEPROM.h                                                   */
/*  DATE        :Mar, 2013                                                 */
/*  Programmer	:xiang 'R                                                  */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                              */
/*  Mark        :ver 1.0                                                   */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>				// 常用C定义

extern void ID_learn(void);
extern void ID_EEPROM_Initial(void);
extern void eeprom_IDcheck(void);
extern void eeprom_IDcheck_UART(void);
extern void ID_EEPROM_write_0x00(void);
extern void all_Erase_EEPROM(void);

#if defined(__Product_PIC32MX2_WIFI__)
extern void alarm_EEPROM_write(void);
#endif