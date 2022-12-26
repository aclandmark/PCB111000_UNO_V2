



#define message_1 \
"\r\nSTAND-a-LONE CLOCK\r\n\
Use sw1 and 2 to enter time (24Hr clock)\r\n\
and sw1 to start the clock.\r\n\
Press sw2 to toggle the display on and off\r\n"

#define message_2 \
"Press sw1 to pause or resume the clock\r\n\
To adjust the clock:\r\n"

#define message_3 \
"pulse sw3 then press sw1 and sw2 to advance the time\r\n\
or press and hold sw3 (for 500ms)\r\n\
then press sw1 and sw2 to retard the time\r\n\
Always pulse sw3 when the time is correct\r\n"

#define User_instructions \
Serial.write(message_1);\
Serial.write(message_2);\
Serial.write(message_3);



/*****************************************************************************************************************/
#define set_up_pci \
PCICR |= ((1 << PCIE0) | (1 << PCIE2));

#define enable_pci  PCMSK0 |= (1 << PCINT2);\
PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);

#define disable_pci_on_sw3  PCMSK0 &= (~(1 << PCINT2));
#define clear_PCI_on_sw1_and_sw2    PCIFR |= (1<< PCIF2);
#define disable_pci_on_sw1_and_sw2  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));


#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)



/*****************************************************************************************************************/
#define shift_display_left   for (int p = 7;  (p); p--)\
digits[p] = digits[p-1];

#define clear_display   { for (int m = 0; m < 8; m++)digits[m] = 0; }

#define save_to_eeprom    eeprom_write_byte((uint8_t*)(m+3),digits[0]);



/*****************************************************************************************************************/
#define AT_clock_mode 'C'
#define AT_exit_stop_watch 10, '1'
#define AT_mode_1 1

#define display_clock 'D', '1'
#define hide_clock 'D', '2'
#define pause_clock 'D', '3'
#define increment_seconds 'D', '5'
#define decrement_minutes 'D', '6'
#define decrement_seconds 'D', '7'
#define increment_minutes 'D', '8'

#define display_current_time 'E', '1'
#define display_stored_times 'E', '4'
#define store_time 'E', '2'
#define one100ms_mode 'E', '1'
#define ten_ms_mode 'E', '2'



/*****************************************************************************************************************/
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



/*****************************************************************************************************************/
//Test programmmer writes 0x40 to 0x3F4 before quitting 
//Text verification.c increments 0x3F4 each time a string is printed in commentary mode
//bit 7 of 0x3F4 tells the bootloader that the WDTout is not due to a user app

#define User_app_commentary_mode \
\
if(eeprom_read_byte((uint8_t*)0x3F4) == 0xFF)\
eeprom_write_byte((uint8_t*)0x3F4,0);\
\
if(eeprom_read_byte((uint8_t*)0x3F4) == 0x40){\
for(int m = 0; m < 10; m++)Serial.write("\r\n");\
Serial.write("Project commentary: Press 'X' to escape or AOK\r\n");\
\
eeprom_write_byte((uint8_t*)0x3F4,0x41);}\
\
if ((eeprom_read_byte((uint8_t*)0x3F4) & 0x40)){\
eeprom_write_byte((uint8_t*)0x3F4,\
(eeprom_read_byte((uint8_t*)0x3F4) | 0x80));\
\
for(int m = 0; m < 4; m++)Serial.write("\r\n");\
\
asm("jmp 0x6C00");}




/*****************************************************************************************************************/
