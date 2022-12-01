
/**************Proj_5D_Acquire_and process_Integer_Data_from_the _KBD********************/

/*
Here we leave the mini_OS (the Attiny1606) to deal with all the details of displaying a number.
We can use one_wire_comms to send it a string or binary number.
If we send it a string then it will return the number in binary form.

Obviously Arduino do not supply library functions to drive the display and therefore a
bit of DIY programming is required.
*/



#include "INT_KBD_to_displsay_header.h"


int main (void){
long Num_1;
char digits[8];
int counter = 0;

setup_HW_Arduino_IO;


Serial.write("\r\nEnter positive number \
& terminate with Return key.\r\n");
Num_1 = Int_KBD_to_display(digits);                                  //Acquires data from keyboard

do{
Serial.print(++counter); Serial.write('\t');
Serial.print(Num_1); Serial.write("\r\n");

I2C_Tx_long(Num_1);                                           //Sends number to the display

waitforkeypress_A();

Num_1 = (Num_1 / 2) *3;} while (Num_1 < 66666666);                   //Do some arithmetic

Num_1 = (Num_1 / 3) *2; 

do{Num_1 = (Num_1 / 3) *2;                                           //Do the arithmetic in reverse
Serial.print(--counter); Serial.write('\t');
Serial.print(Num_1); 
Serial.write("\r\n");                                             
I2C_Tx_long(Num_1);

waitforkeypress_A();}while (counter-1);
SW_reset;}





/******************************************************************************************/
long Int_KBD_to_display_Local(char display_buffer[]){               //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;


for(int n = 0; n<=8; n++) display_buffer[n] = 0;                    //Clear the buffer used for the string

while(1){
if ((keypress = wait_for_return_key_A()) == '\r')break;             //Detect return key press (i.e \r or\r\n)

if (decimal_digit_A(keypress))                                        //Ignore non decimal keypresses except -cr-.
{
{for(int n = 8; n>=1; n--)                                          //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;  }                                    //Add new keypress           

I2C_Tx_8_byte_array(display_buffer);}}                                //Update display                                                

I2C_Tx_any_segment_clear_all();
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();
return Long_Num_from_mini_OS;
}





/************************************************************************************************************************/
