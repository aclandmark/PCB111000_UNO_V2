
//Calculates square roots of integer numbers

#include "Square_roots_header.h"
#include "square_root_subroutines.c"


#define message_1 "\r\nDATA FROM I/O\r\n\
Press sw1 to populate digits[0]\r\nsw3 to shift display left\r\n\
sw2 to enter the number\r\nsw1 to pause the program and restart the program.\r\n"

#define message_2 "\r\nEnter new number\r\n"


//Enter the main routine here






/******************************************************************************************/
long number_from_IO(void){

set_up_pci;
enable_pci;

Init_display_for_pci_data_entry;                                    //Set digit[0] to zero and display it.
while(!(Data_Entry_complete))wdr();                                      //Line A. wait here for pci interrupts used to enter data
Data_Entry_complete = 0;
disable_pci;
return I2C_displayToNum();}                                         //Acquire binary value of the display and return it.



/********************************************************************************************************************/
ISR(PCINT2_vect) {                                                //input number: store keypresses in array -digits
if(switch_1_down)enable_pci_on_sw2;
while(switch_1_down){                                             //Use switch 1 to select keypress 0...9
    switch(digits[0]){                                            //Test digits[0]
      case '9': if(!(digits[1]))                                  //If its 9 scroll onto "-"
      digits[0] = '-';                                            //but only if this is the first entry (i.e. digits[1] is 0.
      else digits[0] = '0';break;                                 //otherwise scroll from zero to 9
      case '-': digits[0] = '0';break;                             //If digits[0] is "-" scroll onto '0'.
      default: digits[0]++; break;}                                //Otherwise increment it
    I2C_Tx_8_byte_array(digits);                                  //Send value to the display
    Timer_T0_10mS_delay_x_m(25);}                                 //return to Line A when sw1 is released

if(switch_2_down) {disable_pci_on_sw2;                           //Use switch 3 to shift display left
    shift_digits_left; digits[0] = '0';                             //shift the display and initialise digit[0] (make it display '0')
  I2C_Tx_8_byte_array(digits);}}                                  //update the display and return to Line A



/********************************************************************************************************************/
ISR(PCINT0_vect){
if(switch_3_up)return;                                           //Ignore switch release
I2C_Tx_any_segment_clear_all();
Timer_T0_10mS_delay_x_m(25);                                      //Flash display
I2C_Tx_8_byte_array(digits);
Data_Entry_complete=1;}                                           //Return to Line A



/************************************************************************************************************************/
