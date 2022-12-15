/*

sw1 is PIND2  The left hand switch
sw2 is PIND7  The middle switch
sw3 is PINB2  The right hand switch


Switches sw1 and sw2 generate a call to ISR(PCINT2_vect) 
Switch sw3 generates a call to ISR(PCINT0_vect)*/


char watch_dog_reset = 0;
char MCUSR_copy;
char User_response;
char num_as_string[12];


void I2C_Tx_long(long );
int Int_from_PC_Basic(char *);
void factors_to_display(int *, int);
/*****************************************************************************/
#include <avr/wdt.h>



/*****************************************************************************/
#define SW_reset {wdt_enable(WDTO_30MS);while(1);}



/*****************************************************************************/
#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)

#define enable_pci_on_sw1         PCMSK2 |= (1 << PCINT18);
#define enable_pci_on_sw3         PCMSK0 |= (1 << PCINT2);
#define disable_pci_on_sw1        PCMSK2 &= (~(1 << PCINT18));
#define disable_pci_on_sw3        PCMSK0 &= (~(1 << PCINT2));


#define set_up_PCI \
PCICR |= ((1 << PCIE0) | (1 << PCIE2));


/*****************************************************************************/
#define setup_HW_basic \
CLKPR = (1 << CLKPCE);                        /*Reduce 16MHz crystal clock to 8MHz*/\
CLKPR = (1 << CLKPS0);\
\
MCUSR_copy = \
eeprom_read_byte((uint8_t*)0x3FC);            /*Saved to EEPROM by the bootloader*/\
if (MCUSR_copy & (1 << PORF))                 /*Power on reset flag set*/\
{MCUSR_copy = (1 << PORF);\
eeprom_write_byte((uint8_t*)0x3F5,0);}        /*Initialise random generator memory */\
setup_watchdog_for_UNO;\
\
set_up_I2C;                                   /*UNO hosts the slave I2C*/\
ADMUX |= (1 << REFS0);                        /*Set analogue reference to +5V*/\
set_up_switched_inputs;\
Unused_I_O;\
set_up_activity_leds;\
\
setup_PC_comms_Basic(0,16);\
_delay_ms(10);\
User_app_commentary_mode;\
\
if (((PINB & 0x04)^0x04) && \
((PIND & 0x04)^0x04))                         /*Press SW1 and SW3 to adjust intensity*/\
I2C_Tx_LED_dimmer_UNO();\
\
if(((PIND & 0x04)^0x04) && \
((PIND & 0x80)^0x80))                         /*Press SW1 and SW2 to trigger recalibration*/\
Cal_UNO_pcb_A_Basic();



/*****************************************************************************/
/*Users press -t- to upload the project commentry and hex file
 They then press -r- to print out the commentary line by line and -X- to run the project
 Having printed the commentary once pressing -r- again will run the project immediately
 with no commentary
 EEPROM 0x3F6 controls printing the project commentary.
After each line has been printed 0x3F6 increments and the program resets.
When X is pressed when program control jumps to the user app.
*/


#define User_app_commentary_mode \
\
if(eeprom_read_byte((uint8_t*)0x3F6) == 0xFF)\
eeprom_write_byte((uint8_t*)0x3F6,0);\
\
if(eeprom_read_byte((uint8_t*)0x3F6) == 0x40){\
for(int m = 0; m < 10; m++)String_to_PC_Basic("\r\n");\
String_to_PC_Basic\
("Project commentary: Press 'X' to escape or AOK\r\n");\
\
eeprom_write_byte((uint8_t*)0x3F6,0x41);}\
\
if ((eeprom_read_byte((uint8_t*)0x3F6) & 0x40)){\
eeprom_write_byte((uint8_t*)0x3F6,\
(eeprom_read_byte((uint8_t*)0x3F6) | 0x80));\
\
for(int m = 0; m < 4; m++)String_to_PC_Basic("\r\n");\
\
asm("jmp 0x6C00");}                                     /*Go to Text_Verification.hex to print the next string*/ 






/*****************************************************************************/
#define setup_watchdog_for_UNO \
if (MCUSR_copy & (1 << WDRF))watch_dog_reset = 1;\
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
#define OSC_CAL_328                                /*User cal bytes if set are stored in EEPROM locations 0x3FF and 0x3FE*/\
if ((eeprom_read_byte((uint8_t*)0x3FE) > 0x0F)\
&&  (eeprom_read_byte((uint8_t*)0x3FE) < 0xF0)\
&& (eeprom_read_byte((uint8_t*)0x3FE)\
== eeprom_read_byte((uint8_t*)0x3FF)))\
{OSCCAL = eeprom_read_byte((uint8_t*)0x3FE);}       //At reset the micro reads register OSCCAL to obtain the calibration byte


//Note: Arduino reads the EEPROM as unsigned 8 bit chars


/*****************************************************************************/
#define User_prompt \
while(1){\
do{String_to_PC_Basic("R?    ");}  while((isCharavailable_Basic(250) == 0));\
User_response = Char_from_PC_Basic();\
if((User_response == 'R') || (User_response == 'r'))break;} String_to_PC_Basic("\r\n");



/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);



/*********************************************************************************/
#include "UNO_proj_resources/PC_comms/Basic_Rx_Tx_Basic.c"

#include "UNO_proj_resources/Chip2chip_comms/I2C_subroutines_1.c"
#include "UNO_proj_resources/Chip2chip_comms/I2C_slave_Rx_Tx.c"

#include "UNO_proj_resources/Subroutines/Random_and_prime_nos.c"
#include "UNO_proj_resources/Subroutines/HW_timers.c"


/**********************************************************************************/

#define line_control {if(line_counter==4) {line_counter = 0;newline_Basic();}\
 else {Char_to_PC_Basic('\t');line_counter++;}}

#define User_instructions \
String_to_PC_Basic(message_1);\
String_to_PC_Basic(message_2);

#define message_1 \
"\r\nUses a modified prime number generator that includes an percentage of non_prime numbers\r\n\
Review the numbers and try to guess the non-prime ones\r\n"
#define message_2 \
"Type them in terminating each with a -return- keypress\r\n\
Do not try 1\r\n\
Enter zero to exit and press sw3 when requested\r\n"
