
/**************Proj_6E_Acquire_and process_FPN_Data_from_the KBD********************/

/*
 * PRINTS column of numbers each one the square root of the previous
 * until within 0.005 of unity then continues but calculates the squares
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
& terminate with Return key.\r\n");

Num_1 = FPN_KBD_to_display_Local(digits); //digits, &expnt, &Denominator);

while (1){
Int_Num_to_PC(counter,num_as_string, '\t');  
Sc_Num_to_PC(Num_1,1,6 ,'\r'); 

if (!(counter%5)){FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter += 1;                                //Do some arithmetic
if ((Num_1 < (1 + 5.0e-3)) && (Num_1 > (1 - 5.0e-3)))break;}

power = 1.0/power;
do{
Int_Num_to_PC(counter,num_as_string, '\t'); 
Sc_Num_to_PC(Num_1,1,6 ,'\r');                            //Send number to PC

if (!(counter%5)){FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter -= 1; }                               //Do some arithmetic
while(counter+1);

SW_reset;}




/************************************************************************************************/


/***********************************************************************************************************************/
float FPN_KBD_to_display_Local(char digits[]){      //char digits[], char *expnt, long *Denominator ){

char  expnt;
float num_1 = 0;
 char sign = '+';

expnt =  FPN_string_KBD_to_display_Local(digits); 

if (digits[0]== '-'){for (int m = 0; m <= 13; m++)digits[m] =  digits[m + 1];
sign = '-';}
num_1 = atof(digits);

if (sign == '-') num_1 = num_1 * (-1);
return num_1;}



/********************************************************************************************************************************************/
char FPN_string_KBD_to_display_Local(char display_buffer[]){              //Operation is similar to that of Int_KBD_to_display()
char keypress;
float Float_from_mini_OS = 0;
char decimal_place_counter = 0;
char keypress_counter = 1;
char expt;
char * exp_string;

 for(int m = 0; m <=14; m++)exp_string[m] = 0;
for(int n = 0; n<=14; n++) display_buffer[n] = 0;                                    //Clear the buffer used to the string

while(1){                                                                         //Remain in loop until a valid character is received
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-')
&& (keypress != '.'))continue;                                                //Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}

I2C_Tx_8_byte_array(display_buffer);                                                      //Update display with the first key press

while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;                                     //Stay in loop until return key press is detected

if ((decimal_digit_A(keypress)) || (keypress == '.')                                    //Check for valid keypresses
|| (keypress == '\b')|| (keypress == '-')
|| (keypress == 'E') || (keypress == 'e'))

{
switch (keypress){

case '\b':  for (int n = 0; n <= 11; n++)                                  //Backspace keypress
display_buffer[n] = display_buffer[n + 1];
I2C_Tx_8_byte_array(display_buffer); break;


case 'E':
case 'e': exp_string =  display_buffer + keypress_counter + 1; 

default:
for(int n = 14; n>=1; n--)                                                            //Shift display for each new keypress except '.'
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;                                                     //Add new keypress           
keypress_counter += 1;
I2C_Tx_8_byte_array(display_buffer); break;

} } }

I2C_Tx_any_segment_clear_all();
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

reverse (display_buffer);
expt = atoi(exp_string);

return expt;}
