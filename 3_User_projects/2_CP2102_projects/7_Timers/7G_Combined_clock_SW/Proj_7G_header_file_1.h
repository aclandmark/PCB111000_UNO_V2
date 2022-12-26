
#include "Proj_7G_header_file_2.h"


char watch_dog_reset = 0;
char MCUSR_copy;
char User_response;
char num_as_string[12];



/*****************************************************************************/
#include <avr/wdt.h>



/*****************************************************************************/
#define setup_HW_for_clock_SW \
CLKPR = (1 << CLKPCE);                        /*Reduce 16MHz crystal clock to 8MHz*/\
CLKPR = (1 << CLKPS0);\
\
MCUSR_copy = \
eeprom_read_byte((uint8_t*)0x3FC);            /*Saved to EEPROM by the bootloader*/\
if (MCUSR_copy & (1 << PORF))                 /*Power on reset flag set*/\
{MCUSR_copy = (1 << PORF);\
eeprom_write_byte((uint8_t*)0x3F5,0);}        /*Initialise random generator memory */\
setup_watchdog;\
\
set_up_I2C;                                   /*UNO hosts the slave I2C*/\
ADMUX |= (1 << REFS0);                        /*Set analogue reference to +5V*/\
set_up_switched_inputs;\
Unused_I_O;\
set_up_activity_leds;\
\
Serial.begin(115200);\
while (!Serial);\
sei();



/*****************************************************************************/
#define setup_watchdog \
if (MCUSR_copy & (1 << WDRF))watch_dog_reset = 1;\
wdr();\
MCUSR &= ~(1<<WDRF);\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;


#define wdr()  __asm__ __volatile__("wdr")



/*****************************************************************************/
#define SW_reset {wdt_enable(WDTO_30MS);while(1);}



/*****************************************************************************/
#define set_up_I2C \
TWAR = 0x02;


/*****************************************************************************/
#define set_up_switched_inputs \
MCUCR &= (~(1 << PUD));\
DDRD &= (~((1 << PD2)|(1 << PD7)));\
PORTD |= ((1 << PD2) | (1 << PD7));\
DDRB &= (~(1 << PB6));\
PORTB |= (1 << PB6);


/*****************************************************************************/
#define Unused_I_O \
MCUCR &= (~(1 << PUD));\
DDRB &= (~((1 << PB2)|(1 << PB7)));\
DDRC &= (~((1 << PC0)|(1 << PC1)|(1 << PC2)));\
DDRD &= (~((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6)));\
PORTB |= ((1 << PB2)|(1 << PB7));\
PORTC |= ((1 << PC0)|(1 << PC1)|(1 << PC2));\
PORTD |= ((1 << PD3)|(1 << PD4)|(1 << PD5)|(1 << PD6));


/*****************************************************************************/
#define OSC_CAL_328 \
if ((eeprom_read_byte((uint8_t*)0x3FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x3FE) < 0xF0) && (eeprom_read_byte((uint8_t*)0x3FE)\
== eeprom_read_byte((uint8_t*)0x3FF))) {OSCCAL = eeprom_read_byte((uint8_t*)0x3FE);}


//Note: both WinAVR and Arduino read the EEPROM as unsigned 8 bit chars


/*****************************************************************************/
#define User_prompt \
while(1){\
do{Serial.write("R?    ");}  while((isCharavailable_A(250) == 0));\
User_response = Serial.read();\
if((User_response == 'R') || (User_response == 'r'))break;} Serial.write("\r\n");



/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);


#include "UNO_proj_resources/Chip2chip_comms/I2C_slave_Rx_Tx.c"
#include "UNO_proj_resources/Chip2chip_comms/I2C_subroutines_1.c"
#include "UNO_proj_resources/Subroutines/HW_timers.c"
#include "UNO_proj_resources/PC_comms/Basic_Rx_Tx_Arduino.c"
#include "UNO_proj_resources/PC_comms/Arduino_Rx_Tx_UNO_pcb.c"



/******************************************************************************/
