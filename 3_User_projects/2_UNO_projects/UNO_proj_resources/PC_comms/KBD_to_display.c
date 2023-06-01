
/*
Obviously Arduino does not provide functions to drive the display directly using keyboard entry.
The DIY versions provided here indicate the level of complexity required.
Tests have shown that they are generally OK but there is probably a bug some where in there.
*/

int strLength (char *);
void reverse (char *);
void FPN_string_KBD_to_display_A(char *, int);
char Get_Float_num_string_from_KBD(char *, int);


/******************************************************************************************************************************************/
long Int_KBD_to_display_A(char display_buffer[]){              	//Acquires an integer string from the keyboard and returns the binary equivalent
char keypress;
long Long_Num_from_mini_OS;

for(int n = 0; n<=8; n++) display_buffer[n] = 0;           		

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

//{if (keypress == '\b'){                         				//Backspace key
//for (int n = 0; n <= 7; n++)
//display_buffer[n] = display_buffer[n + 1];}
{if (keypress == '\b'){                         				//Backspace key
for (int n = 0; n < 7; n++)
display_buffer[n] = display_buffer[n + 1];
display_buffer[7] = 0;}

else
{for(int n = 8; n>=1; n--)                                    	//Shift display for each new keypress
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;  }                             	//Add new keypress           

I2C_Tx_8_byte_array(display_buffer);}}                                     //Update display includes "cr_keypress"                                                 

I2C_Tx_any_segment_clear_all();
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

Long_Num_from_mini_OS = I2C_displayToNum();

return Long_Num_from_mini_OS;}





/***********************************************************************************************************************/
float FPN_KBD_to_display_A(char digits[], int BL){			

float num_1 = 0;
 char sign = '+';

FPN_string_KBD_to_display_A(digits, BL); 

if (digits[0]== '-'){for (int m = 0; m < BL - 1; m++)digits[m] =  digits[m + 1];
sign = '-';}
num_1 = atof(digits);

if (sign == '-') num_1 = num_1 * (-1);
return num_1;}



/********************************************************************************************************************************************/
void FPN_string_KBD_to_display_A(char display_buffer[], int BL){    //Operation is similar to that of Int_KBD_to_display()
char keypress;

for(int n = 0; n < BL; n++) display_buffer[n] = 0;   

while(1){                                                          	//Remain in loop until a valid character is received
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-')
&& (keypress != '.'))continue;                                     	//Ignore keypress if it is not OK
display_buffer[0] = keypress;
break;}

I2C_Tx_8_byte_array(display_buffer);                               	//Update display with the first key press

while(1){
if ((keypress = wait_for_return_key_A())  =='\r')break;               //Stay in loop until return key press is detected

if (!(decimal_digit_A(keypress)) && (keypress != '.')                 //Check for valid keypresses
&& (keypress != '-') && (keypress != 'E') 
&& (keypress != 'e') &&  (keypress != '\b'))continue;

switch (keypress){

//case '\b':  for (int n = 0; n < BL - 1; n++)                             //Backspace keypress
//display_buffer[n] = display_buffer[n + 1];
//I2C_Tx_8_byte_array(display_buffer); break;
case '\b':  for (int n = 0; n < BL - 1; n++)                             //Backspace keypress
display_buffer[n] = display_buffer[n + 1];
display_buffer[BL - 1] = 0;
I2C_Tx_8_byte_array(display_buffer); break;

default:
for(int n = BL - 1; n>=1; n--)                                            //Shift display for each new keypress except '.'
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;                                         //Add new keypress to display           
I2C_Tx_8_byte_array(display_buffer); break;}}


I2C_Tx_any_segment_clear_all();											//Flash display
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

reverse (display_buffer);}



/***********************************************************************************************************************/
void reverse (char s[]){											//See Joe Pardue's book p 83
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
  



/**************************************************************************************************************************************************/
long Get_fpn_from_KBD(char digits[], int *twos_expnt, char *tens_expnt, long *twos_denominator , char *sign, int BL){   

/*Defines real number in terms of significant, denominator and twos exponent
For example 125 is converted to the form (125/128) * 2^7 = 0.97... * 2^7
*/

long num_1=0, num_2 = 0;											//Operation is similar to that of FPN_KBD_to_display();
 *sign = '+';														//but does not use the standard floating point library

*tens_expnt = 0;
*twos_denominator = 1;
*twos_expnt = 0;

*tens_expnt =  Get_Float_num_string_from_KBD(digits,  BL); 
if (digits[0]== '-'){for (int m = 0; m < BL - 1; m++)digits[m] =  digits[m + 1];
*sign = '-';}
num_1 = atol(digits);
num_2 = num_1;
while(num_2){(*twos_expnt)++; *twos_denominator *=2; num_2 /= 2;}

return num_1;} 														//Returns positive number plus sign                                     




/*************************************************************************************************************************************************/
char Get_Float_num_string_from_KBD(char display_buffer[], int BL){      //Operation is similar to that of FPN_string_KBD_to_display()
char keypress;															//but does not use the standard floating point library
char decimal_place_counter = 0;
char keypress_E = 0;
char tens_expt;
char tens_exp_string[5];

 for(int m = 0; m < 5; m++) tens_exp_string[m] = 0;
for(int n = 0; n < BL; n++) display_buffer[n] = 0;  

while(1){                                                                 //Remain in loop until a valid character is received
keypress = waitforkeypress_A();
if ((!(decimal_digit_A(keypress)))
&& (keypress != '-')
//&& (keypress != '.')
)continue;                                            //Ignore keypress if it is not OK
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
  
//if(keypress == '\b'){for (int n = 0; n < BL-1; n++)                        //Backspace keypress
//display_buffer[n] = display_buffer[n + 1];
//I2C_Tx_8_byte_array(display_buffer);}
if(keypress == '\b')  {for (int n = 0; n < BL - 1; n++)                             //Backspace keypress
display_buffer[n] = display_buffer[n + 1];
display_buffer[BL - 1] = 0;
I2C_Tx_8_byte_array(display_buffer);}

else

{{for(int n = BL-1; n>=1; n--)                                              //Shift display for each new keypress except '.'
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


for (int m = 0; m < BL; m++){if((display_buffer[m] != 'e')
&& (display_buffer[m] != 'E'));											//Find location of E or e
else {                						
  display_buffer[m] = 0;
for (int p = 0; p < 5; p++)
tens_exp_string[p] = display_buffer[++m];}}

tens_expt += atoi(tens_exp_string);                                       //Value of expnt adjusted to allow for value entered at KBD

{int m,p;
for (m = 0; m < BL; m++)if(display_buffer[m] == '.')break;                 //Remove decimal point
if(display_buffer[m] == '.')
{for (int p = m; p < BL-1; p++)
display_buffer[p] = display_buffer[p+1];}}
return tens_expt;}





/*************************************************************************************************************************************************/
char Float_num_string_from_KBD_Legacy(char display_buffer[]){            //Operation is similar to that of Int_KBD_to_display()
char keypress;
float Float_from_mini_OS = 0;
char decimal_place_counter = 0;
char keypress_E = 0;
char expt;
char exp_string[15];

 for(int m = 0; m <=14; m++)exp_string[m] = 0;
for(int n = 0; n<=14; n++) display_buffer[n] = 0;                         //REMOVE LINE	Clear the buffer used to the string

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
  
//if(keypress == '\b'){for (int n = 0; n <= 11; n++)                      //Backspace keypress
//display_buffer[n] = display_buffer[n + 1];
//I2C_Tx_8_byte_array(display_buffer);}
if(keypress == '\b'){for (int n = 0; n < 11; n++)                             //Backspace keypress
display_buffer[n] = display_buffer[n + 1];
display_buffer[11] = 0;
I2C_Tx_8_byte_array(display_buffer);}

else

{{for(int n = 14; n>=1; n--)                                              //Shift display for each new keypress except '.'
display_buffer[n] = display_buffer[n-1];
display_buffer[0] = keypress;}                                            //Add new keypress           
I2C_Tx_8_byte_array(display_buffer);}}                                    //Update display includes "cr_keypress"                                                 

if((decimal_place_counter) && (!(keypress_E))) 
decimal_place_counter += 1;}

I2C_Tx_any_segment_clear_all();												//Flash display
_delay_ms(100);
I2C_Tx_8_byte_array(display_buffer);

expt = -(decimal_place_counter);
if(expt) (expt) += 1;														//Value of exponent based on number of decimal places

reverse (display_buffer);

{int m, n;
for (m = 0; m <=14; m++)if(display_buffer[m] == 'e')break;					//Find location of E
if(display_buffer[m] == 'e'){
  display_buffer[m] = 0;
 n = m+1; 
  for (int p = n; p <= 14; p++)
  {exp_string[p - n] = display_buffer[++m];}}}

expt += atoi(exp_string);													//Value of expnt adjusted to allow for value enteredat KBD

{int m,p;
for (m = 0; m <=14; m++)if(display_buffer[m] == '.')break;					//Remove decimal point
if(display_buffer[m] == '.')
{for (int p = m; p <= 14; p++)
display_buffer[p] = display_buffer[p+1];}}

return expt;}


/***********************************************************************************************************************/
long fpn_from_KBD_Legacy(char digits[], char *expnt, long *Denominator ){		//Defines FPN in terms of significant, denominate and exponent

long num_1=0, num_2 = 0;
 char sign = '+';

*Denominator = 1;
*expnt =  Float_num_string_from_KBD_Legacy(digits); 
if (digits[0]== '-'){for (int m = 0; m <= 13; m++)digits[m] =  digits[m + 1];
sign = '-';}
num_1 = atol(digits);
num_2 = num_1;
while(num_2){(*expnt)++; *Denominator *=10; num_2 /= 10; }

if (sign == '-') num_1 = num_1 * (-1);
return num_1;}





/****************************************************************************************************************************/