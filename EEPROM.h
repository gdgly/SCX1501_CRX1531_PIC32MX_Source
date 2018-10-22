/****************************************************************************
/*                    eeprom #define Functions                              */
/*  FILE        :EEPROM.h                                                   */
/*  DATE        :Mar, 2013                                                 */
/*  Programmer	:xiang 'R                                                  */
/*  CPU TYPE    :PIC32MX2     Crystal: 10/40M                              */
/*  Mark        :ver 1.0                                                   */
/***********************************************************************/
#include <stdlib.h>
#include <plib.h>				// ����C����

extern void ID_learn(void);
extern void ID_EEPROM_Initial(void);
extern void eeprom_IDcheck(void);
extern void eeprom_IDcheck_UART(void);
extern void ID_EEPROM_write_0x00(void);
extern void all_Erase_EEPROM(void);

#if defined(__Product_PIC32MX2_WIFI__)
extern void alarm_EEPROM_write(void);
extern void all_Erase_EEPROM_next(void);
extern void eeprom_IDcheck_Multiple(UINT8 value_m);
extern void Sunrise_sunset_EEPROM_write(void);
extern void SUN_time_get(UINT8 value);
extern void SUN_EEPROM_write(void);
extern void Emial_time_EEPROM_write(void);
extern void HA_Change_EEPROM_write(void);
#endif