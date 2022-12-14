
/**************Proj_6E_Acquire_and process_FPN_Data_from_the KBD********************/

/*Enter +ve or -ve numbers at the keyboard.  
 * 
 * For +ve numbers 
 * PRINTS column of numbers each one the square root of the previous
 * until within 0.005 of unity then continues but calculates the squares
 * 
 * For -ve numbers prints powers of 3 or 0.33333
 * 
 * Introduces subroutine subroutine "float FPN_KBD_to_display(char *)"
 * This is a development of subroutine "long Int_KBD_to_display(char *)"
 * It allows for decimal points, E and the backspace key
 * The library function "atof" is used to generate the binary form rather than the mini-OS
 * 
*/

#include "FPN_KBD_to_display_header.h"


int main (void){
float Num_1;
float power = 0.5;
char digits[15];

char counter = 0;


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n\
Press SW3 to pause the display.\r\n\
SW1 to scroll through the results.\r\n");

Num_1 = FPN_KBD_to_display_Local(digits); 

if(Num_1 > 0.0)power = 0.5;
else power = 3.0;

while (1){
Int_Num_to_PC(counter,num_as_string, '\t');  
Sc_Num_to_PC(Num_1,1,6 ,'\r'); 

if (power == 0.5)
{if (!(counter%5)){I2C_FPN_to_display(Num_1);
while(switch_1_down);}

if ((Num_1 < (1 + 5.0e-3)) && (Num_1 > (1 - 5.0e-3)))break;}



if (power == 3.0)
{if ((Num_1 < -1.0E20) || (Num_1 > -1.0e-20)){counter = 15; Num_1 *= -1.0; break;}
  I2C_FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter += 1;}                                //Do some arithmetic

power = 1.0/power;
do{
Int_Num_to_PC(counter,num_as_string, '\t'); 
Sc_Num_to_PC(Num_1,1,6 ,'\r');                                            //Send number to PC

if (!(counter%5)){I2C_FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter -= 1; }                               //Do some arithmetic
while(counter+1);

SW_reset;}





/***********************************************************************************************************************/
float FPN_KBD_to_display_Local(char digits[]){ 

//char  expnt;
float num = 0;
 char sign = '+';

FPN_string_KBD_to_display_Local(digits);                                 //Acquire FPN string

if (digits[0]== '-')                                                     //Detect negative sign
{for (int m = 0; m <= 13; m++)
digits[m] =  digits[m + 1];
sign = '-';}
num = atof(digits);                                                      //Convert to binary

if (sign == '-') num = num * (-1);                                       //Restore sign
return num;}



/********************************************************************************************************************************************/
void FPN_string_KBD_to_display_Local(char display_buffer[]){            //Operation is similar to that of Int_KBD_to_display()
char keypress;

for(int n = 0; n<=14; n++) display_buffer[n] = 0;                       //Clear the buffer used for the string

while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;                  //Stay in loop until return key press is detected

if (!(decimal_digit_A(keypress)) && (keypress != '.')                    //Check for valid keypresses
&& (keypress != '-') && (keypress != 'E') 
&& (keypress != 'e'))continue;                                            //Skip back to top of loop for invalid keypress

for(int n = 14; n>=1; n--)                                               //Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;                                            //Add new keypress           
I2C_Tx_8_byte_array(display_buffer); }

I2C_Tx_any_segment_clear_all();                                           //Flash display when -cr- is pressed
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

reverse (display_buffer);}





/****************************************************************************************************************/
