



char watch_dog_reset = 0;
char MCUSR_copy;
char User_response;
char num_as_string[12];


/*****************************************************************************/
#include <avr/wdt.h>


#define newline       String_to_PC_Basic("\r\n");

unsigned char PRN_8bit_GEN(unsigned char, char *);


/*****************************************************************************/
#define SW_reset {wdt_enable(WDTO_30MS);while(1);}



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
#include "UNO_proj_resources_with_mini_OS/PC_comms/Basic_Rx_Tx_Basic.c"

#include "UNO_proj_resources_with_mini_OS/Chip2chip_comms/I2C_subroutines_1.c"
#include "UNO_proj_resources_with_mini_OS/Chip2chip_comms/I2C_slave_Rx_Tx.c"

#include "UNO_proj_resources_with_mini_OS/Subroutines/Random_and_prime_nos.c"
#include "UNO_proj_resources_with_mini_OS/Subroutines/HW_timers.c"


/**********************************************************************************/
