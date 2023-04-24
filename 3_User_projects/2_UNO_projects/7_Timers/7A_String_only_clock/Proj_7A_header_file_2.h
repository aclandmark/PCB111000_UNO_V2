

/************Define clock memory locations********************/
#define HoursH digits[7]
#define HoursL digits[6]
#define MinsH digits[5]
#define MinsL digits[4]
#define SecsH digits[3]
#define SecsL digits[2]
#define deci_SecsH digits[1]
#define deci_SecsL digits[0]



/***********Define clock display functions and modes***************/
#define reset_clock_1   {digits[0] = 0; for (int m = 1; m < 8; m++)digits[m] = '0'; }



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
//Test programmmer writes 0x40 to 0x3F4 before quitting 
//Text verification.c increments 0x3F4 each time a string is printed in commentary mode
//bit 7 of 0x3F4 tells the bootloader that the WDTout is not due to a user app

#define User_app_commentary_mode \
\
if(eeprom_read_byte((uint8_t*)0x3F6) == 0xFF)\
eeprom_write_byte((uint8_t*)0x3F6,0);\
\
if(eeprom_read_byte((uint8_t*)0x3F6) == 0x40){\
for(int m = 0; m < 10; m++)Serial.write("\r\n");\
Serial.write\
("Project commentary: Press 'X' to escape or AOK\r\n");\
waitforkeypress_A();\
\
eeprom_write_byte((uint8_t*)0x3F6,0x41);}\
\
if ((eeprom_read_byte((uint8_t*)0x3F6) & 0x40)){\
eeprom_write_byte((uint8_t*)0x3F6,\
(eeprom_read_byte((uint8_t*)0x3F6) | 0x80));\
\
for(int m = 0; m < 8; m++)Serial.write("\r\n");\
_delay_ms(10);\
asm("jmp 0x6C00");}                                     /*Go to Text_Verification.hex to print the next string*/ 





/**********************************************************************************************/
