
#define F_CPU 8000000UL 

#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/eeprom.h>
#include <util/delay.h>


#include "../../../6_Resources_UNO\Hex_txt_bootloader.h"
#include "../../../6_Resources_UNO\Hex_txt_bootloader_SW_subs.c"
#include "../../../6_Resources_UNO\Basic_IO_subroutines.c"