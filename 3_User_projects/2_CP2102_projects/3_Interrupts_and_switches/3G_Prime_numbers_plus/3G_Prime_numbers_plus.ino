
/*Proj_3G_Prime_numbers_plus
****************************************************************/


/*IT INTRODUCES


1.  An attempt to have a bit of fun with prime numbers.

2.  Project subroutine "prime_no_generator_plus()".

This generates prime numbers plus a few extras that do look like primes.
Try and guess which are primes and which are not.


USER INSTRUCTIONS

The numbers are sent to the PC screen.
Type in any that look non-prime terminating each with a "return" keypress.
Enter zero to exit and press sw3 when requested.

Switch location SW1(PD2) - SW2(PD7) – SW3(PB2).*/



#include "Prime_numbers_plus_header.h"

void Num_to_PC_Basic (long);
volatile int start_1=0;

int main (void){
int l, start, counter_1, counter_2, array_size=100; 
int  array[array_size];
int factor[10], number;
char digit_array[8], line_counter=0;



setup_HW_basic;
User_prompt;
//setup_and_enable_PCI;

set_up_PCI_on_sw3;
enable_pci_on_sw3;

User_instructions;
sei();
start = 0;

while(1){
counter_1 = 0;                                                  //Counts non-prime numbers
counter_2 = 0;                                                  //Counts all non zero numbers
l=start;
while(l < (start+5)){ 
for(int k =0; k < array_size; k++)
{array[k] = k+1+array_size*l;}                                  //populates array with sequence of numbers i.e. 100, 101, 102,....199
prime_no_generator_plus(l,array_size,array);                    //sets non primes_plus to zero
{int j=0; 
while(j<array_size){if (array[j] != 0)
{Int_to_PC_Basic(array[j]);I2C_Tx_long(array[j]);                  //Display non primes_plus
counter_1++; counter_2++; Char_to_PC_Basic(' ');                      //Count the numbers
Timer_T1_sub(T1_delay_100ms);}  
j++;}}
l++;} newline_Basic();                                                //repeat 5 times (i.e. for numbers 0 to 499)
  
l=start;
while(l < (start+5)){                                           //Repeat but obtain real prime numbers only
for(int k =0; k < array_size; k++) 
{array[k] = k+1+array_size*l;}
prime_no_generator(l,array_size,array);
{int j=0; while(j<array_size)
{if (array[j] != 0)counter_1--;                                 //Adjust counter_1 for number of non_prime numbers
j++;}}l++;}

Int_to_PC_Basic(counter_2);String_to_PC_Basic (" numbers including ");
Int_to_PC_Basic(counter_1); String_to_PC_Basic (" non prime numbers\r\n");
String_to_PC_Basic("Enter number (prime or non-prime?) or 0 to exit\r\n");

line_counter=0;

while(1){

  
number = Int_from_PC_Basic(digit_array);                             //Enter trial number at the keyboard

if(!(number)) {String_to_PC_Basic("Sw3 to continue.\r\n");break;}
Int_to_PC_Basic(number);String_to_PC_Basic("  ");                       //Echo the number
{int  n = 0; int m=0;
do{
factor[n] = Product_search(number);                             //Search for factors
number = number/factor[n];n++;} while (number != 1);  
if (n==1) {String_to_PC_Basic("Prime"); line_control;}               //Only one factor: Print Prime
else {m=n; for(n=0; n<m; n++){Int_to_PC_Basic(factor[n]);
Char_to_PC_Basic(' ');}line_control;}}}                              //Several factors: print them out
while(start == start_1);                                      //wait for sw3 keypress
start = start_1;}}




/*************************************************************************************/
ISR(PCINT2_vect){ 
if(switch_1_up)return; while(switch_1_down);return;}            //Press sw_1 to pause display



/*************************************************************************************/
ISR (PCINT0_vect){
if(switch_3_up) return;
while (switch_3_down){start_1+=5; Int_to_PC_Basic(start_1);        //Hold sw_3 down to increment start point
Char_to_PC_Basic(' ');Timer_T1_sub(T1_delay_1sec);}                   //for random number generator
newline_Basic(); 
return;}


/*******************************************************************************/
/*long Num_from_KBD_Local(char digits[]){
char keypress;
for(int n = 0; n<=7; n++) digits[n] = 0; 

do
{keypress =  waitforkeypress_Basic();} 
while (!(decimal_digit(keypress)));                                      //(non_decimal_char(keypress));  //Not -,0,1,2,3,4,5,6,7,8 or 9
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);

while(1){
if ((keypress = wait_for_return_key_Basic())  =='\r')break;
if (decimal_digit (keypress))                                           //012345678or9  :Builds up the number one keypress at a time
{for(int n = 7; n>=1; n--)
digits[n] = digits[n-1];                                                //Shifts display left for each keypress
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);}}
            
return I2C_displayToNum();}
*/


/*******************************************************************************/
/*void Num_to_PC_Basic (long number)
{ int i = 0;
  char s[12];
   do
  { s[i++] = number % 10 + '0';
  }
  while ((number = number / 10) > 0);
  s[i] = '\0';
  for (int m = i; m > 0; m--)Char_to_PC_Basic(s[m - 1]);
  Char_to_PC_Basic(' ');
}
 */
