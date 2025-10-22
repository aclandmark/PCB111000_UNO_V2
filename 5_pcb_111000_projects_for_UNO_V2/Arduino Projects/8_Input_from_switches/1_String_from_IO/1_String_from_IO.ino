//Enter a number from the IO.
//It scrolls accross the display
//Type in the main routine

#include "String_IO_to_display_header.h"
#include "Local_subroutines.c"

void rotate_display(void);

//Type main routine here





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
