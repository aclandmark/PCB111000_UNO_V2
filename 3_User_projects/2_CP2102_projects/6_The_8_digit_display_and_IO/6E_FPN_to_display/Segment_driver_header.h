


#include <avr/wdt.h>


char watch_dog_reset = 0;
char MCUSR_copy;
char User_response;
char num_as_string[12];                           //Required by pcb_A calibration routine to print out cal results

char PCMSK0_backup, PCMSK2_backup, float_display_mode;

#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)

#define pci_on_sw1_and_sw2_enabled (PCMSK2 & 0x84) == 0x84
#define pci_on_sw3_enabled (PCMSK0 & 0x04) == 0x04
#define PCIenabled ((pci_on_sw1_and_sw2_enabled) || (pci_on_sw3_enabled))
#define disable_pci_on_sw1_and_sw2  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define disable_pci_on_sw3  PCMSK0 &= (~(1 << PCINT2));


/*****************************************************************************/
#define setup_HW_Arduino_IO \
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
Serial.begin(115200);\
while (!Serial);\
sei();\
User_app_commentary_mode;\
\
if (((PINB & 0x04)^0x04) && \
((PIND & 0x04)^0x04))                         /*Press SW1 and SW3 to adjust intensity*/\
I2C_Tx_LED_dimmer_UNO();\
\
if(((PIND & 0x04)^0x04) && \
((PIND & 0x80)^0x80))                         /*Press SW1 and SW2 to trigger recalibration*/\
Cal_UNO_pcb_A_Arduino();



/*****************************************************************************/
#define setup_watchdog_for_UNO \
if (MCUSR_copy & (1 << WDRF))watch_dog_reset = 1;\
wdr();\
MCUSR &= ~(1<<WDRF);                          /*Line not really needed WD flag already reset by bootloader */\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = 0;


#define wdr()  __asm__ __volatile__("wdr")


#define SW_reset {wdt_enable(WDTO_30MS);while(1);}



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
#define User_prompt \
while(1){\
do{Serial.write("R?    ");}  while((isCharavailable_A (250) == 0));\
User_response = Serial.read();\
if((User_response == 'R') || (User_response == 'r'))break;} Serial.write("\r\n");



/*****************************************************************************/
#define waiting_for_I2C_master \
TWCR = (1 << TWEA) | (1 << TWEN);\
while (!(TWCR & (1 << TWINT)));\
TWDR;

#define clear_I2C_interrupt \
TWCR = (1 << TWINT);



/*****************************************************************************/
#define User_app_commentary_mode \
\
if(eeprom_read_byte((uint8_t*)0x3F6) == 0xFF)\
eeprom_write_byte((uint8_t*)0x3F6,0);\
\
if(eeprom_read_byte((uint8_t*)0x3F6) == 0x40){\
for(int m = 0; m < 10; m++)Serial.write("\r\n");\
Serial.write\
("Project commentary: Press 'X' to escape or AOK\r\n");\
\
eeprom_write_byte((uint8_t*)0x3F6,0x41);}\
\
if ((eeprom_read_byte((uint8_t*)0x3F6) & 0x40)){\
eeprom_write_byte((uint8_t*)0x3F6,\
(eeprom_read_byte((uint8_t*)0x3F6) | 0x80));\
\
for(int m = 0; m < 4; m++)Serial.write("\r\n");\
\
asm("jmp 0x6C00");}                                     /*Go to Text_Verification.hex to print the next string*/ 



/*****************************************************************************/
#define pci_on_sw1_and_sw2_enabled (PCMSK2 & 0x84) == 0x84
#define pci_on_sw3_enabled (PCMSK0 & 0x04) == 0x04
#define PCIenabled ((pci_on_sw1_and_sw2_enabled) || (pci_on_sw3_enabled))
#define disable_pci_on_sw1_and_sw2  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));
#define disable_pci_on_sw3  PCMSK0 &= (~(1 << PCINT2));



/*****************************************************************************/
#define I2C_Tx_float_display_control \
{\
PCMSK0_backup= PCMSK0;\
PCMSK2_backup= PCMSK2;\
float_display_mode = '0';\
if (PCIenabled){disable_pci_on_sw3;disable_pci_on_sw1_and_sw2;}\
while(1){\
if(switch_3_down)float_display_mode = '1'; else float_display_mode = '0';\
if((switch_1_down)||(switch_2_down))float_display_mode = '2';\
waiting_for_I2C_master;\
send_byte_with_Nack(float_display_mode);\
clear_I2C_interrupt;\
if(float_display_mode == '2')break;}\
PCMSK0 = PCMSK0_backup;\
PCMSK2 = PCMSK2_backup;}

/**********************************************************************************/
#include "UNO_proj_resources\Chip2chip_comms\I2C_slave_Rx_Tx.c"
#include "UNO_proj_resources\Chip2chip_comms\I2C_subroutines_1.c"
#include "UNO_proj_resources\PC_comms\Basic_Rx_Tx_Arduino.c"
#include "UNO_proj_resources\Subroutines\HW_timers.c"
#include "UNO_proj_resources\PC_comms\KBD_to_display.c"

#include "UNO_proj_resources\Subroutines\FPN_subroutines.c"
#include "UNO_proj_resources\PC_comms\Arduino_Rx_Tx_UNO_pcb.c"
/**********************************************************************************/
