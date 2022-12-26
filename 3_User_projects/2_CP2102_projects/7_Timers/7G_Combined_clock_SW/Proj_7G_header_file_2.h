

/************Define clock memory locations*******************************************************************************/
#define HoursH digits[7]
#define HoursL digits[6]
#define MinsH digits[5]
#define MinsL digits[4]
#define SecsH digits[3]
#define SecsL digits[2]
#define msecsH digits[1]
#define msecsL digits[0]



/***********Define clock display functions and modes***********************************************************************/
#define shift_display_left   for (int p = 7;  (p); p--)\
digits[p] = digits[p-1];

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
#define disable_pci_on_sw2  PCMSK2 &= (~(1 << PCINT23));
#define disable_pci_on_sw3  PCMSK0 &= (~(1 << PCINT2));
#define hold_PCI_on_sw1_and_sw2    PCICR &= (~(1 << PCIE2));
#define restore_PCI_on_sw1_and_sw2  PCICR |= (1 << PCIE2);



/*************************************************************************************************************************/
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



/****************************************************************************************************************************/
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



/***************************************************************************************************************************/
#define User_instructions \
Serial.write(message_1);\
Serial.write(message_2);\
Serial.write(message_3);\
Serial.write(message_4);\
Serial.write(message_5);\
Serial.write(message_6);



# define message_1 \
"\r\nCOMBINED CLOCK/SW:\r\n\
Conect to pc, press sw1 and enter time (or reset and repeat)\r\n\
Power cycle to 5V supply if wanted and press sw1 then sw2 to start\r\n"


# define message_2 \
"Toggle sw2 to switch between clock and new SW and press sw 3 to select old SW\r\n\
Press sw1 to pause, blank or resume display\r\n"


# define message_3 \
"\r\n\
Start by pressing sw3 in place of sw1 \
to enable 2 stop watches and no clock\r\n\r\n\
To adjust time: Press SW2 to restore clock time "


# define message_4 \
"then immediately press SW1 to induce a WDTout\r\n\
Release sw1 to increment time or sw2 to decrement it then both switches to restore normal operation.\r\n"


# define message_5 \
"\r\n\
Bug: Having pressed SW3 when no timer is running \
it is neccessary to press SW2 twice to restore normal operation\r\n"

# define message_6 \
"\r\n\
To force reset in clock mode press sw3 and then \
press reset switch.\r\n"


#define Update_sec_counter  if(sec_counter_save){cli(); sec_counter = sec_counter_V;\
sei();sec_counter_save=0;}

#define setup_64ms_WDT_with_interrupt \
wdr();\
WDTCSR |= (1 <<WDCE) | (1<< WDE);\
WDTCSR = (1<< WDE) | (1 << WDIE) | (1 << WDP1);

#define Arduino_non_WDTout \
Char_from_EEPROM(0x3EE)


#define set_Arduino_WDTout \
Char_to_EEPROM( 0x3EE, 0);

#define clear_Arduino_WDT_flag \
Char_to_EEPROM( 0x3EE, 0xFF);


volatile char mode;
volatile char  TWI_flag;

volatile char ms_counter;                                  //Increments every 10mS 
volatile char old_mode;                                   //Used to restore mode when display is re-activated
char digits[8], charH, charL;                             //Holds characters to be displayed
char Hours, Minutes, Seconds;
  
long sec_counter;                                         //Display time is based on sec_counter 
long sec_counter_V;                                       //Volatile version of sec_counter (updated by TWI ISR)
long clock_time_long;                                     //Saves time (sec + ms * 100); used to switch between clock * SW 
char payload;                                             //No of characters to send over I2C (8 every sec & 2 otherwise)
unsigned char sec_counter_save;                                   //Set to 1 every second (indicates that sec_counter is to be updated from sec_counter_V)
unsigned char display_clear;                                        //Records status of display (blank or active)


void set_time(void);                                      //If mode is 255:   Enables user to enter start; converts it to seconds and saves it in seconds in EEPROM 
                                                          //If mode is zero:  Reads time back out from EEPROM locations
                                                          //If mode is 0:   Reads time back from EEPROM locations 3 - 6; saves resut in "sec_counter"
                                                          //Stores mode in EEPROM locarion 2
void Calculate_time(void);                                //Converts seconds ("sec_counter") to hours, minutes and seconds 
void timer_utoa(char);                                    //Converts hours, minutes and seconds to strings

void Timer(void);                                         //Calls "update_7_seg_display", to close one I2C transaction and sets payload to initiate the next one          
void update_7_seg_display(void);                          //I2C controler: supervises transmission of timer data to the mini OS.
long clockTOstop_watch(void);                             //Saves time before selecting stop watch display (i.e. reseting time to zero)
long restore_Clock(void);                                 //Restores time by adding "clock_time" to the current SW time
long restore_stop_watch(void);                            //Restores SW time by subtracting the curent time from the value stored in "clock_time"
void restore_display(void);                               //Re-activate Stop watch/clock display
void clear_display(void);                                 //Blanks the display
char sw_de_bounce(void);                                  //User Timer with interrupt to control SW bounce
void print_out_bkp(void);                                 //For test only: prints out contents of BKP store EEPROM 7 - 18




/*************************************************************************************************************************/
