
#define User_instructions \
Serial.write(message_1);\
Serial.write(message_2);


#define message_1 \
"\r\nSTOP WATCH: sw2 selects 100ms clock, sw1 a 10ms clock\r\n\
Press sw1 to pause SW and save time\r\n\
Press sw2 the read back saved times\r\n"

#define message_2 \
"While pressing sw2 press sw1 to restore SW\r\n\
Press sw3 to re-initialise SW\r\n"




/**********************************************************************************************************************/
#define shift_display_left   for (int p = 7;  (p); p--)\
digits[p] = digits[p-1];

#define clear_display       {for (int m = 0; m < 8; m++)digits[m] = 0; }
#define save_to_eeprom      eeprom_write_byte((uint8_t*)(m+3),digits[0]);


#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)


#define set_up_PCI \
PCICR |= ((1 << PCIE0) | (1 << PCIE2));

#define enable_pci  PCMSK0 |= (1 << PCINT2);    PCMSK2 |= (1 << PCINT18) | (1 << PCINT23);
#define disable_pci_on_sw1_and_sw2  PCMSK2 &= (~((1 << PCINT18) | (1 << PCINT23)));



/***********Define clock display functions and modes***************/
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



/******************************************************************************************/
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



/*****************************************************************************************/
