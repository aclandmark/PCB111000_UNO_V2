

#include <avr/wdt.h>

char User_response;
char reset_status;
char copy_of_reset_ctl_reg;



/**********************************************************************************/
#define  OSC_CAL \
if ((eeprom_read_byte((uint8_t*)0x3FF) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x3FF) < 0xF0) && (eeprom_read_byte((uint8_t*)0x3FF)\
== eeprom_read_byte((uint8_t*)0x3FE))) {OSCCAL = eeprom_read_byte((uint8_t*)0x3FE);}

//If the internal clock has been calibrated, a calibration byte will be found in EEPROM locations 0x3FF/E



/************************************************************************************************************************************/
#define setup_328_HW_Arduino_IO \
\
setup_watchdog;\
ADMUX |= (1 << REFS0);\
initialise_IO;\
OSC_CAL;\
\
comms_cal;\
set_up_pin_change_interrupt_on_PC5;\
\
setup_one_wire_comms;\
set_up_activity_leds;\
sei();\
Serial.begin(115200);\
while (!Serial);\
determine_reset_source;\
One_25ms_WDT_with_interrupt;

//The reset control switch is connected to PC5



/************************************************************************************************************************************/
#define setup_328_HW_Basic_IO \
\
setup_watchdog;\
ADMUX |= (1 << REFS0);\
initialise_IO;\
OSC_CAL;\
\
comms_cal;\
set_up_pin_change_interrupt_on_PC5;\
\
setup_one_wire_comms;\
set_up_activity_leds;\
sei();\
setup_PC_comms(0,16);\
_delay_ms(10);\
determine_reset_source;\
One_25ms_WDT_with_interrupt;



/************************************************************************************************************************************/
#define wdr()  __asm__ __volatile__("wdr")

#define setup_watchdog \
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;


#define SW_reset {Signal_SW_reset;wdt_enable(WDTO_30MS);while(1);}
#define WDTout {wdt_enable(WDTO_30MS);while(1);}


#define One_25ms_WDT_with_interrupt \
wdr();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) |  (1 << WDP0)  |  (1 << WDP1);



/************************************************************************************************************************************/
#define initialise_IO \
MCUCR &= (~(1 << PUD));\
DDRB = 0;\
DDRC = 0;\
DDRD = 0;\
PORTB = 0xFF;\
PORTC = 0xFF;\
PORTD = 0xFF;

//All ports are initialised to weak pull up (WPU)



/************************************************************************************************************************************/
#define User_prompt_Arduino \
{while(1){\
do{Serial.write("R?    ");}   while((isCharavailable_A(35) == 0));\
User_response = Serial.read();\
if((User_response == 'R') || (User_response == 'r'))break;} Serial.write("\r\n");}



/**********************************************************************************/
#define reset_ctl_reg                         0x3FC
#define Signal_flagged_WDTout                 eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0x10)
#define Signal_WDTout_with_interrupt          eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0x20)
#define Signal_SW_reset                       eeprom_write_byte((uint8_t*)reset_ctl_reg,(eeprom_read_byte((uint8_t*)reset_ctl_reg) & ~0x40))
#define clear_reset_ctl_reg                   eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0)

#define Reset_control_switch_up               (PINC & 0x20)




/************************************************************************************************************************************/
#define determine_reset_source \
switch (eeprom_read_byte((uint8_t*)reset_ctl_reg))\
{case ((byte)~1): reset_status = 1; break;\
case ((byte)~0x42): reset_status = 2; break;\
case ((byte)~8): reset_status = 3; break;\
case ((byte) ~0x52):  reset_status = 4; break;\
case ((byte) ~0x22):  reset_status = 5; break;\
case ((byte) ~0x2):  reset_status = 6; break;\
case ((byte) ~0): reset_status = 7; break;}\
copy_of_reset_ctl_reg = eeprom_read_byte((uint8_t*)reset_ctl_reg);\
clear_reset_ctl_reg;

/*
reset_status:
1 POR                     bit 0 of reset control register
2 SW_reset                bits 6 and 1 
3 prtD                    bit 3
4 Flaged WDTout           bit 6,4 and 1
5 WDTout with ISR         bit 5 and 1
6 WDTout                  bit 1
7 As 5 but ISR missing    No bits

Note : Set bit 2 to generate the prtD... prompt
*/


/************************************************************************************************************************************/
#define fail_safe \
if(reset_status == 5)\
{Serial.write("\r\nProgram halted (WDTout with ISR)\r\n");\
Char_to_PC_as_binary((byte)copy_of_reset_ctl_reg);\
while(1)wdr();}\
if(reset_status == 6)\
{Serial.write("\r\nWDTout\r\n");while(1)wdr();}



/************************************************************************************************************************************/
#define set_up_PCI                  PCICR |= (1 << PCIE2);
#define reinstate_PCI               PCICR |= (1 << PCIE2);
#define enable_PCI_on_sw1           PCMSK2 |= (1 << PCINT18);
#define disable_PCI_on_sw1          PCMSK2 &= (~(1 << PCINT18));

#define switch_1_up               (PIND & 0x04)
#define switch_2_up               (PIND & 0x20)
#define switch_3_up               (PIND & 0x80)
#define switch_1_down             (PIND & 0x04)^0x04
#define switch_2_down             (PIND & 0x20)^0x20
#define switch_3_down             (PIND & 0x80)^0x80

#define newline                   Serial.write("\r\n");



/************************************************************************************************************************************/
#include "Resources_reset_controller\One_wire_header.h"
#include "Resources_reset_controller\One_wire_transactions.c"
#include "Resources_reset_controller\Basic_IO_and_Timer.c"
#include "Resources_reset_controller\display_driver_subroutines_KBD.c"
#include "Resources_reset_controller\Reset_project_subroutines.c"



/***************************************************************************************************************************************/
