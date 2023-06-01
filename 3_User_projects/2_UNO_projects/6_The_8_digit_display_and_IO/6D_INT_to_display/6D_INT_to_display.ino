
/**************Proj_6D_Acquire_and process_Integer_Data_from_the KBD********************/

/*
 * 
 * RUNNING AVERAGES
Here we leave the mini_OS (pcb_A) to deal with all the details of displaying a number.
We can use I2C comms to send it as string or binary number.
If we send it a string then it will return the number in binary form.

Obviously Arduino do not supply library functions to drive the display and therefore a
bit of DIY programming is required.
The result is the subroutine "long Int_KBD_to_display(char *)"
This accepts decimal digits from the KBD and sends them to the display.

*/



#include "INT_KBD_to_displsay_header.h"


int main (void){
float Arith_mean;
float Geom_mean;

char digits[8];
int counter = 1;
long num_from_KBD;


setup_HW_Arduino_IO;

if(watch_dog_reset == 1)Serial.write("\r\nAgain?\r\n");
else
Serial.write("\r\nRunning arithmetic & Geometric averages:\r\n\
Enter positive numbers \r\n\
& terminate with Return key.\r\n\
To display interim reults press SW1 before -cr-.\r\n\
Press sw1 twice to resume entering numbers.\r\n\
Note: Zero entry is ignored.\r\n\
Press SW3 to pause the display\r\n\
Press reset to repeat.\r\n");

num_from_KBD = Int_KBD_to_display_A(digits);
Arith_mean = (float)num_from_KBD;
Geom_mean = Arith_mean;

while(1){

while ((switch_1_down) || (switch_2_down) || (switch_3_down));
if ((num_from_KBD = Int_KBD_to_display_A(digits)))

{Arith_mean = Arith_mean * (float)counter;
Geom_mean = pow (Geom_mean, (float)counter);


Arith_mean += (float)num_from_KBD;
Geom_mean *= (float)num_from_KBD;

counter += 1;
Arith_mean = (Arith_mean) / (float)counter;
if(Geom_mean < 0.0)Geom_mean *= -1;
Geom_mean =  pow (Geom_mean , 1/(float)counter);}

if(switch_1_down)while(switch_1_down); else continue;

I2C_FPN_to_display(Arith_mean);
while(switch_1_down);

I2C_FPN_to_display(Geom_mean);
while(switch_1_down);
if (switch_3_down)break;}

SW_reset;}





/******************************************************************************************/
long Int_KBD_to_display_A_Local(char display_buffer[]){                //Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;

for(int n = 0; n<=8; n++) display_buffer[n] = 0;              

while(1){
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-'))continue;                                //Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}
 
I2C_Tx_8_byte_array(display_buffer);                    //See "Chip2chip_comms\Display_driver_header.h"
while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;         //Detect return key press (i.e \r or\r\n)
if ((decimal_digit_A(keypress)) || (keypress == '\b')\
 || (keypress == '-'))

 /*{if (keypress == '\b'){                                 //Backspace key
for (int n = 0; n <= 7; n++)
display_buffer[n] = display_buffer[n + 1];}
*/
{if (keypress == '\b'){                                 //Backspace key
for (int n = 0; n < 7; n++)
display_buffer[n] = display_buffer[n + 1];
display_buffer[7] = 0;}

else
{for(int n = 8; n>=1; n--)                                      //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;  }                              //Add new keypress           

I2C_Tx_8_byte_array(display_buffer);}}                                     //Update display includes "cr_keypress"                                                 

I2C_Tx_any_segment_clear_all();
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();

return Long_Num_from_mini_OS;}





/************************************************************************************************************************/
