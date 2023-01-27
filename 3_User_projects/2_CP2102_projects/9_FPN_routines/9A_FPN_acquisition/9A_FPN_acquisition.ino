/**********************SIGN bit not implemented yet*********************/

#include "9A_header.h"

int main (void){

char digits[15];

long Significand;
long  twos_denominator;
char   tens_expnt;
int twos_expnt;
char counter = 0;
char sign = '+';

long FPN_as_long;
float FPN;

setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n");

while(1){
tens_expnt = 0;
Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator);
if (Significand < 0) {Significand *= -1; sign = '-';}
FPN_as_long = Fraction_to_Binary_Signed(Significand, twos_denominator);
FPN_as_long = Assemble_FPN(FPN_as_long, twos_expnt);
Serial.write("\r\n");Serial.print(*(float*)&FPN_as_long);

FPN = Scientifc_num_to_FPN(FPN_as_long, tens_expnt);

Serial.write("\r\nF");Print_long_as_binary(*(long*)&FPN);

if (sign == '-'){FPN_as_long = *(long*)&FPN | 0x80000000;
FPN = *(float*)&FPN_as_long;}

Serial.write("\t\t");
Sc_Num_to_PC(FPN, 1, 6, '\r');
//I2C_FPN_to_display(FPN);
sign = '+';}
SW_reset;}



/***********************************************************************************************************************/
char Get_Float_num_string_from_KBD(char display_buffer[]){            //Operation is similar to that of Int_KBD_to_display()
char keypress;
char decimal_place_counter = 0;
char keypress_E = 0;
char tens_expt;
char tens_exp_string[15];

 for(int m = 0; m <=14; m++)tens_exp_string[m] = 0;
for(int n = 0; n<=14; n++) display_buffer[n] = 0;                         //Clear buffer space

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
  
if(keypress == '\b'){for (int n = 0; n <= 11; n++)                        //Backspace keypress
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
if(tens_expt) (tens_expt) += 1;                                           //Value of exponent based on number of decimal places

reverse (display_buffer);

{int m, n;
for (m = 0; m <=14; m++)if(display_buffer[m] == 'e')break;                  //Find location of E
if(display_buffer[m] == 'e'){
  display_buffer[m] = 0;
 n = m+1; 
for (int p = n; p <= 14; p++)
{tens_exp_string[p - n] = display_buffer[++m];}}}

tens_expt += atoi(tens_exp_string);                                       //Value of expnt adjusted to allow for value entered at KBD

{int m,p;
for (m = 0; m <=14; m++)if(display_buffer[m] == '.')break;                  //Remove decimal point
if(display_buffer[m] == '.')
{for (int p = m; p <= 14; p++)
display_buffer[p] = display_buffer[p+1];}}
return tens_expt;}



/***********************************************************************************************************************/
long Get_fpn_from_KBD(char digits[], int *twos_expnt, char *tens_expnt, long *twos_denominator ){   

/*Defines real number in terms of significant, denominator and twos exponent
For example 125 is converted to the form (125/128) * 2^7 = 0.97... * 2^7
*/

long num_1=0, num_2 = 0;
 char sign = '+';

*twos_denominator = 1;
*twos_expnt = 0;

*tens_expnt =  Get_Float_num_string_from_KBD(digits); 
if (digits[0]== '-'){for (int m = 0; m <= 13; m++)digits[m] =  digits[m + 1];
sign = '-';}
num_1 = atol(digits);
num_2 = num_1;
while(num_2){(*twos_expnt)++; *twos_denominator *=2; num_2 /= 2;}

if (sign == '-') num_1 = num_1 * (-1);
return num_1;}                                      



/***********************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/***********************************************************************************************************************/
void Print_char_as_binary(unsigned char *num){
  for (int m = 0; m <=7; m++){
  if (*num & ((unsigned char) 0x80 >> m))Serial.write('1'); else Serial.write('0');}Serial.write(' ');}



/***********************************************************************************************************************************/
