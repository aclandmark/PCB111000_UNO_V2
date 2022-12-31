
/**************Proj_6G_Acquire_and process_Integer_Data_from_the _KBD********************/

/*
 * Prints colum of numbers each 1.5 times the previous until 99999999 or -9999999 is reached
 * then returns by dividing by 1.5
 * 
*/



#include "INT_IO_to_display_header.h"


int main (void){
long Num_1;
char digits[8];
int counter = 0;

setup_HW_Arduino_IO;

Serial.write("\r\nDATA FROM I/O");
Serial.write(message_1);

while((switch_1_down) || (switch_2_down) ||(switch_3_down));        //wait for switch release

Num_1 = number_from_IO();

do{
Serial.print(++counter); Serial.write('\t');
Serial.print(Num_1); Serial.write("\r\n");

I2C_Tx_long(Num_1);                                           //Sends number to the display
_delay_ms(100);

while(switch_1_down);

Num_1 = (Num_1 / 2) *3;} 
while ((Num_1 < 99999999) && (Num_1 > -9999999));                   //Do some arithmetic

while(switch_1_up);

Num_1 = (Num_1 / 3) *2; 

do{Num_1 = (Num_1 / 3) *2;                                           //Do the arithmetic in reverse
Serial.print(--counter); Serial.write('\t');
Serial.print(Num_1); 
Serial.write("\r\n");                                             
I2C_Tx_long(Num_1);
_delay_ms(100);
while(switch_1_down);}while (counter-1);

while(switch_1_up);
SW_reset;}





/******************************************************************************************/
long number_from_IO(void){

set_up_pci;
enable_pci;

Init_display_for_pci_data_entry;                                    //Set digit[0] to zero and display it.
while(!(Data_Entry_complete));                                      //Line A. wait here for pci interrupts used to enter data
Data_Entry_complete = 0;
disable_pci;
return I2C_displayToNum();}                                         //Acquire binary value of the display and return it.



/********************************************************************************************************************/
ISR(PCINT2_vect) {                                                //input number: store keypresses in array -digits
while(switch_1_down){                                             //Use switch 1 to select keypress 0...9
    switch(digits[0]){                                            //Test digits[0]
      case '9': if(!(digits[1]))                                  //If its 9 scroll onto "-"
      digits[0] = '-';                                            //but only if this is the first entry (i.e. digits[1] is 0.
      else digits[0] = '0';break;                                 //otherwise scroll from zero to 9
      case '-': digits[0] = '0';break;                             //If digits[0] is "-" scroll onto '0'.
      default: digits[0]++; break;}                                //Otherwise increment it
    I2C_Tx_8_byte_array(digits);                                  //Send value to the display
    Timer_T0_10mS_delay_x_m(25);}                                 //return to Line A when sw1 is released

if(switch_2_down) {                                               //Use switch 3 to shift display left
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
