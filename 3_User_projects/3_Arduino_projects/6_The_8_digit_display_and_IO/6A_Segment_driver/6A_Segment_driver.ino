
/*
 Enables users to drive the display manually
 This indicates that there is not necessarily any connection between 
 the display and an actual number.


 Note:  The PCB111000 bootloader enables user programs to distinguish between
 SW_resets and other resets (i.e POR and first time run following programming).
 This facility is exercised here.
 */



#include "Segment_driver_header.h"



#define message_1 "\r\nPress key a to g (x for next digit).\r\n\
Press sw_2 to clear the display.\r\n"
#define message_2 "  ?"



int main (void){

char letter = 0, digit_num;            

setup_HW_Arduino_IO;
I2C_Tx_any_segment_clear_all();

set_up_PCI_on_sw2;
  enable_pci_on_sw2;


if(!(watch_dog_reset))Serial.write(message_1);
else Serial.write(message_2);{watch_dog_reset = 0;}


while(1){digit_num=0;
while(digit_num < 8){

if(Serial.available())                                        //Wait up to 60mS for a key press.  In the absence of one
{switch(letter = Serial.read()){                              //the program reverts back to "while(digit_num < 8){"
case 'a': case 'A':                                           //otherwise it continues and updates the display
case 'b': case 'B':                                           //Program execution is not put on hold awaiting a keypress
case 'c': case 'C':
case 'd': case 'D':
case 'e': case 'E':
case 'f': case 'F':
case 'g': case 'G': 
I2C_Tx_any_segment(letter, digit_num); break;
case 'x': case 'X': digit_num += 1;break;
default: break;}}
wdr();
}}}


/***************************************************************************************************************/
ISR(PCINT2_vect)
{ if (switch_2_up)return;
  I2C_Tx_any_segment_clear_all();
  SW_reset;
}
