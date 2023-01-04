

#include "9A_header.h"

int main (void){
float Num_1;
float power;
char digits[15];

long Significand;
long  twos_denominator;
char  twos_expnt, tens_expnt;
char counter = 0;


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n");

Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator);

Serial.print (twos_denominator);Serial.write('\t');
Serial.print ((int)twos_expnt);Serial.write('\t');
Serial.print(Significand);Serial.write('\t');
Serial.print ((int)tens_expnt); Serial.write("\r\n");
//////////////////////Num_1 = Significand_to_FPN((float)Significand, twos_denominator, twos_expnt);

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

Num_1 = pow(Num_1, power);  counter += 1;}

power = 1.0/power;
do{
Int_Num_to_PC(counter,num_as_string, '\t'); 
Sc_Num_to_PC(Num_1,1,6 ,'\r');                                            //Send number to PC

if (!(counter%5)){I2C_FPN_to_display(Num_1);
while(switch_1_down);}

Num_1 = pow(Num_1, power);  counter -= 1; }                               //Do some arithmetic
while(counter+1);


SW_reset;}




char Get_Float_num_string_from_KBD(char display_buffer[]){            //Operation is similar to that of Int_KBD_to_display()
char keypress;
float Float_from_mini_OS = 0;
char decimal_place_counter = 0;
char keypress_E = 0;
char tens_expt;
char tens_exp_string[15];

 for(int m = 0; m <=14; m++)tens_exp_string[m] = 0;
for(int n = 0; n<=14; n++) display_buffer[n] = 0;                         //Clear the buffer used to the string

while(1){                                                                 //Remain in loop until a valid character is received
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-')
&& (keypress != '.'))continue;                                            //Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}

I2C_Tx_8_byte_array(display_buffer);                                      //Update display with the first key press

while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;                   //Stay in loop until return key press is detected

if ((decimal_digit_A(keypress)) || (keypress == '.')                      //Check for valid keypresses
|| (keypress == '\b')|| (keypress == '-')
|| (keypress == 'E') || (keypress == 'e'))

{if(display_buffer[0] == '.')decimal_place_counter = 1;
if((keypress == 'E') || (keypress == 'e'))keypress_E = 1;
  
  if(keypress == '\b'){for (int n = 0; n <= 11; n++)                      //Backspace keypress
display_buffer[n] = display_buffer[n + 1];
I2C_Tx_8_byte_array(display_buffer);}

else

{{for(int n = 14; n>=1; n--)                                              //Shift display for each new keypress except '.'
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;}                                            //Add new keypress           
I2C_Tx_8_byte_array(display_buffer);}}                                    //Update display includes "cr_keypress"                                                 

if((decimal_place_counter) && (!(keypress_E))) 
decimal_place_counter += 1;}

I2C_Tx_any_segment_clear_all();                                           //Flash display
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

tens_expt = -(decimal_place_counter);
if(tens_expt) (tens_expt) += 1;                           //Value of exponent based on number of decimal places

reverse (display_buffer);

{int m, n;
for (m = 0; m <=14; m++)if(display_buffer[m] == 'e')break;          //Find location of E
if(display_buffer[m] == 'e'){
  display_buffer[m] = 0;
 n = m+1; 
  for (int p = n; p <= 14; p++)
  {tens_exp_string[p - n] = display_buffer[++m];}}}

tens_expt += atoi(tens_exp_string);                         //Value of expnt adjusted to allow for value entered at KBD

{int m,p;
for (m = 0; m <=14; m++)if(display_buffer[m] == '.')break;          //Remove decimal point
if(display_buffer[m] == '.')
{for (int p = m; p <= 14; p++)
display_buffer[p] = display_buffer[p+1];}}
return tens_expt;}


/***********************************************************************************************************************/
long Get_fpn_from_KBD(char digits[], char *twos_expnt, char *tens_expnt, long *twos_denominator ){   //Defines FPN in terms of significant, denominate and exponent

long num_1=0, num_2 = 0;
 char sign = '+';

*twos_denominator = 1;
*twos_expnt = 0;

*tens_expnt =  Get_Float_num_string_from_KBD(digits); 
if (digits[0]== '-'){for (int m = 0; m <= 13; m++)digits[m] =  digits[m + 1];
sign = '-';}
num_1 = atol(digits);
num_2 = num_1;
while(num_2){(*twos_expnt)++; *twos_denominator *=2; num_2 /= 2; }

if (sign == '-') num_1 = num_1 * (-1);
return num_1;}
