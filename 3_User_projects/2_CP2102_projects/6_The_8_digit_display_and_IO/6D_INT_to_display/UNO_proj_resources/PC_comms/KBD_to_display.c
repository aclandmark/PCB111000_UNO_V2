
/*
Obviously Arduino does not provide functions to drive the display directly using keyboard entry.
The DIY versions provided here indicate the level of complexity required.
Tests have shown that they are generally OK but there is probably a bug some where in there.
*/

int strLength (char *);
char Float_num_string_from_KBD(char *);



/******************************************************************************************************************************************/
long Int_KBD_to_display(char display_buffer[]){              	//Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;

for(int n = 0; n<=8; n++) display_buffer[n] = 0;           		//Clear the buffer used for the string

while(1){
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-'))continue;                            		//Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}
 
I2C_Tx_8_byte_array(display_buffer);										//See "Chip2chip_comms\Display_driver_header.h"
while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;        	//Detect return key press (i.e \r or\r\n)
if ((decimal_digit_A(keypress)) || (keypress == '\b')\
 || (keypress == '-'))

{if (keypress == '\b'){                         				//Backspace key
for (int n = 0; n <= 7; n++)
display_buffer[n] = display_buffer[n + 1];}
else
{for(int n = 8; n>=1; n--)                                    	//Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;  }                             	//Add new keypress           

I2C_Tx_8_byte_array(display_buffer);}}                                     //Update display includes "cr_keypress"                                                 

I2C_Tx_any_segment_clear_all();
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();
return Long_Num_from_mini_OS;
return Long_Num_from_mini_OS;}




/********************************************************************************************************************************************/
/***********************************************************************************************************************/
long fpn_from_KBD(char digits[], char *expnt, long *Denominator ){

long num_1=0, num_2 = 0;
 char sign = '+';

*Denominator = 1;
*expnt =  Float_num_string_from_KBD(digits); 
if (digits[0]== '-'){for (int m = 0; m <= 13; m++)digits[m] =  digits[m + 1];
sign = '-';}
num_1 = atol(digits);
num_2 = num_1;
while(num_2){(*expnt)++; *Denominator *=10; num_2 /= 10; }

if (sign == '-') num_1 = num_1 * (-1);
return num_1;}



/***********************************************************************************************************************/
void reverse (char s[]){
    int c,i,j;
  for (i=0, j = strLength(s) - 1; i < j; i++, j--){
    c = s[i];
    s[i] = s[j];
    s[j] = c;}}



/***********************************************************************************************************************/
  int strLength (char s[]){
int i = 0;
while (s[i] != 0) ++i;
return i;}
  




/*****************************************************************************************************************************/
char Float_num_string_from_KBD(char display_buffer[]){              //Operation is similar to that of Int_KBD_to_display()
char keypress;
float Float_from_mini_OS = 0;
char decimal_place_counter = 0;
char keypress_E = 0;
char expt;
char exp_string[15];

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

{if(display_buffer[0] == '.')decimal_place_counter = 1;
if((keypress == 'E') || (keypress == 'e'))keypress_E = 1;
  
  if(keypress == '\b'){for (int n = 0; n <= 11; n++)                                  //Backspace keypress
display_buffer[n] = display_buffer[n + 1];
I2C_Tx_8_byte_array(display_buffer);}

else

{{for(int n = 14; n>=1; n--)                                                            //Shift display for each new keypress except '.'
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;}                                                      //Add new keypress           
I2C_Tx_8_byte_array(display_buffer);}}                                                        //Update display includes "cr_keypress"                                                 


I2C_Tx_any_segment_clear_all();
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);


if((decimal_place_counter) && (!(keypress_E))) decimal_place_counter += 1;}
expt = -(decimal_place_counter);
if(expt) (expt) += 1;

reverse (display_buffer);

{int m, n;
for (m = 0; m <=14; m++)if(display_buffer[m] == 'e')break;
if(display_buffer[m] == 'e'){
  display_buffer[m] = 0;
 n = m+1; 
  for (int p = n; p <= 14; p++){exp_string[p - n] = display_buffer[++m];}}}

expt += atoi(exp_string);

{int m,p;
for (m = 0; m <=14; m++)if(display_buffer[m] == '.')break;
if(display_buffer[m] == '.'){for (int p = m; p <= 14; p++)display_buffer[p] = display_buffer[p+1];}}


return expt;
}



