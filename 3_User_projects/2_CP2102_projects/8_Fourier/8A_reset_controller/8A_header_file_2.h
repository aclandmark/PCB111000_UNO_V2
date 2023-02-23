

/************Define clock memory locations*******************************************************************************/
#define reset_ctl_reg                         0x3ED
#define Signal_flagged_WDTout                 eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0x10)
#define Signal_WDTout_with_interrupt          eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0x20)
#define clear_reset_ctl_reg                   eeprom_write_byte((uint8_t*)reset_ctl_reg, ~0)
#define reset_ctl_reg_clear                   (eeprom_read_byte((uint8_t*)reset_ctl_reg) == (byte) ~0x0)
#define clear_MCUSR_copy                      eeprom_write_byte((uint8_t*)0x3FC, 0);



/***********Define clock display functions and modes***********************************************************************/
#define switch_1_down  ((PIND & 0x04)^0x04)
#define switch_1_up   (PIND & 0x04)
#define switch_2_down ((PIND & 0x80)^0x80)
#define switch_2_up   (PIND & 0x80)
#define switch_3_down ((PINB & 0x04)^0x04)
#define switch_3_up   (PINB & 0x04)


#define set_up_PCI \
PCICR |= ((1 << PCIE0) | (1 << PCIE2));

#define enable_pci_on_sw1               PCMSK2 |= (1 << PCINT18);
#define disable_pci_on_sw1  PCMSK2 &= (~(1 << PCINT18));



/*************************************************************************************************************************/
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



/************************************************************************************************************************************/
#define determine_reset_source \
 if(MCUSR_copy == (1 << PORF))reset_status = 1;\
 if((watch_dog_reset == 1) && (reset_ctl_reg_clear))reset_status = 2;\
 if(MCUSR_copy == (1 << EXTRF ))reset_status = 3;\
 if (eeprom_read_byte((uint8_t*)reset_ctl_reg) == (byte) ~0x10)reset_status = 4;\
 if (eeprom_read_byte((uint8_t*)reset_ctl_reg) == (byte) ~0x20)reset_status = 5;\
 if(!(MCUSR_copy))reset_status = 6;\
clear_reset_ctl_reg;\
clear_MCUSR_copy;



/*Reset status:
1:    POR
2:    SW_reset
3:    External reset or press -r- at h/t/r/D prompt
4     Flagged WDTout
5     WDTout with interrupt
6     WDT configured with interrupt but ISR missing.
      Jumps to 0x0000 not x7000
 */




/*************************************************************************************************************************/
