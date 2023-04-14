



char watch_dog_reset = 0;
char User_response;
char num_as_string[12];


/*****************************************************************************/
#include <avr/wdt.h>


#define newline       String_to_PC_Basic("\r\n");

/*****************************************************************************/
#define SW_reset                     {eeprom_write_byte((uint8_t*)(0x3FC),0);wdt_enable(WDTO_30MS);while(1);}
#define SW_reset_detected           !(eeprom_read_byte((uint8_t*)(0x3FC)))
#define clear_reset_eeprom           eeprom_write_byte((uint8_t*)(0x3FC),0xFF);


/*****************************************************************************/
#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)


#define set_up_PCI \
PCICR |= ((1 << PCIE0) | (1 << PCIE2));

#define enable_pci  PCMSK0 |= (1 << PCINT2);\
PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);


/*****************************************************************************/
#define setup_HW_basic \
CLKPR = (1 << CLKPCE);                        /*Reduce 16MHz crystal clock to 8MHz*/\
CLKPR = (1 << CLKPS0);\
\
setup_watchdog;\
\
set_up_I2C;                                   /*UNO hosts the slave I2C*/\
ADMUX |= (1 << REFS0);                        /*Set analogue reference to +5V*/\
set_up_switched_inputs;\
Unused_I_O;\
set_up_activity_leds;\
\
setup_PC_comms_Basic(0,16);\
_delay_ms(10);\
\
if (((PINB & 0x04)^0x04) && \
((PIND & 0x04)^0x04))                         /*Press SW1 and SW3 to adjust intensity*/\
I2C_Tx_LED_dimmer_UNO();\
\
if(((PIND & 0x04)^0x04) && \
((PIND & 0x80)^0x80))                         /*Press SW1 and SW2 to trigger recalibration*/\
Cal_UNO_pcb_A_Basic();



/*****************************************************************************/
#define setup_watchdog \
if (SW_reset_detected){watch_dog_reset = 1;clear_reset_eeprom;}\
wdr();\
MCUSR &= ~(1<<WDRF);                          /*Line not needed WD flag already reset by bootloader */\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;


#define wdr()  __asm__ __volatile__("wdr")

#define wd_timer_off \
wdr();\
MCUSR &= (~(1 << WDRF));\
WDTCSR |= (1<<WDCE) | (1<<WDE);\
WDTCSR = 0x00;



/*****************************************************************************/
#define set_up_I2C \
TWAR = 0x02;                                     /*Address of slave I2C*/



/*****************************************************************************/
#define set_up_switched_inputs                  /*Set pins to weak pull up  */\
MCUCR &= (~(1 << PUD));                         /*Clear pull-up dissable */\
DDRD &= (~((1 << PD2)|(1 << PD7)));             /*Ports D2 and D7 configured for input*/\
PORTD |= ((1 << PORTD2) | (1 << PORTD7));        /*Set Port data registers high */\
DDRB &= (~(1 << PB2));                           /*Repeat for PORTB2*/\
PORTB |= (1 << PB2);




/*****************************************************************************/
#define  set_up_activity_leds                   /*Common cathode dual LEDs connected between B0 and B1 and 0V*/\
DDRB |= (1 << DDB0) | (1 << DDB1);               /*Ports B0 and B1 set to output*/\
LED_1_off;\
LED_2_off;

#define LED_1_off   PORTB &= (~(1 << PB1));       //Output low
#define LED_1_on    PORTB |= (1 << PB1);          //Output high

#define LED_2_off   PORTB &= (~(1 << PB0));
#define LED_2_on    PORTB |= (1 << PB0);

#define Toggle_LED_1 \
if (PORTB & (1 << PB1)){LED_1_off;}                 /*If output high set low*/\
else {PORTB |= (1 << PB1);}




/*****************************************************************************/
#define Unused_I_O                                  /*Set all unused ports to weak pull up*/\
MCUCR &= (~(1 << PUD));\
DDRC &= (~((1 << PC0)|(1 << PC1)|(1 << PC2)));\
DDRD &= (~((1 << PD3)|(1 << PD4)|(1 << PD5)));\
PORTC |= ((1 << PC0)|(1 << PC1)|(1 << PC2));\
PORTD |= ((1 << PD3)|(1 << PD4)|(1 << PD5));


/*
Note: The hex_text_bootloader reads PD6 to control the reset operation.
It should really be weak pull up but has been left in its default condition (tri-state) 
This is OK because it is always connected to a defined logic level
*/



/*****************************************************************************/
#define User_prompt_Basic \
while(1){\
do{String_to_PC_Basic("R?    ");}  while((isCharavailable_Basic(250) == 0));\
User_response = Char_from_PC_Basic();\
if((User_response == 'R') || (User_response == 'r'))break;} String_to_PC_Basic("\r\n");



/*****************************************************************************/
#define Initialise_display \
PORT_1 = 0b0000000000000001;\
PORT_2 = 0b1000000000000000;\
I2C_Tx_2_integers(PORT_1, PORT_2);



#define address_of_last_string \
((eeprom_read_byte((uint8_t*)(0x3E9))) << 8)\
+ eeprom_read_byte((uint8_t*)(0x3EA))

#define save_string_address \
eeprom_write_byte((uint8_t*)(0x3E9), eep_address >> 8);\
eeprom_write_byte((uint8_t*)(0x3EA), eep_address);

#define inc_display \
*PORT_1 = *PORT_1 << 1;\
*PORT_2 = *PORT_2 >> 1;\
I2C_Tx_2_integers(*PORT_1, *PORT_2);\
if (*PORT_2 == 1)\
{*PORT_1 = 0b0000000000000001;\
*PORT_2 = 0b1000000000000000;}



/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);



/*********************************************************************************/
#include "Arduino_proj_resources/PC_comms/Basic_Rx_Tx_Basic.c"
#include "Arduino_proj_resources/Subroutines/Random_and_prime_nos.c"
#include "Arduino_proj_resources/Subroutines/HW_timers.c"
#include "Arduino_proj_resources/Chip2chip_comms/I2C_subroutines_1.c"
#include "Arduino_proj_resources/Chip2chip_comms/I2C_slave_Rx_Tx.c"



/**********************************************************************************/
