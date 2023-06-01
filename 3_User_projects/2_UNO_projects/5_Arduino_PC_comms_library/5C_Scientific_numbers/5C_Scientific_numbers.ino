
/*
Here a number such as 1.25 is considered to be a real number and 1.25E8 a scientific number 
In C the real number can have up to about 6 digits
The exponent can be as any thing between +38 and -38 (at least).

The Arduino function "Serial.print" accepts scientific numbers within a limited range and prints 
them out with the decimal point in the required position.

Project subroutine Sc_Num_from_PC_A() uses the Arduino library to accept the full range scientific 
numbers in string format. It also uses the -C- library function "atof" (askii to float) 
to generate the required floating point number.


Here when printing a scientific number greater than about 10^7 we must first repetitively divide it by 10 
while at the same time incrementing the exponent.  We can then use "Serial.print" to print the resulting 
real number followed by the exponent.  

In project 5C we enter numbers at the KBD. These are raised to a power and the result is 
returned to the PC. A SW_reset is generated if a result exceeds the limits allowed 
for by 32 bit FP numbers.


Subroutines Sc_Num_from_PC_A() and Sc_Num_to_PC_A() can be found in  UNO_proj_resources\PC_comms\Arduino_Rx_Tx_UNO_pcb.c
UNO_proj_resources\PC_comms\Basic_Rx_Tx_Arduino.c
*/



#include "Scientific_numbers_header.h"

#define Buff_Length 30                     //Buffer length


/************************************************************************************************************/



int main (void)  
  { 
    char num_string[Buff_Length + 2];
    float  num_1, num_2, num_3;
    float index;
    char pre_dp;
 
 setup_HW_Arduino_IO;
    
   if (watch_dog_reset == 1) {watch_dog_reset = 0; User_prompt_A;}
  else {Serial.write("\r\n\r\nUsing Arduino functions to receive and print scientific numbers.\r\n");}
   Serial.write("\r\nEnter scientific number\r\nthen the number of digits before the decimal point.\r\n");
   
num_1 = Sc_Num_from_PC_A(num_string, Buff_Length);

while(1){pre_dp = waitforkeypress_A();
if ((pre_dp < '0')||(pre_dp > '9'))Serial.write("!");
else break;}
pre_dp -= '0';

newline_A;
  Sc_Num_to_PC_A(num_1,pre_dp,6,'\r');

if (num_1 < 0.0) index = 3;                                   //Raise negative numbers to the power of 3
else {
  if((num_1 > 1.0e-10) && (num_1 < 1.0e10))index = 1.5;
  else index = 0.75;}

while(1){
  while(!(Serial.available()))wdr();
Serial.read();                                            //The equivalent of waitforkeypress()
num_2 = pow (num_1,index);                                    //-C- library function
Sc_Num_to_PC_A_Local(num_2, pre_dp, 6, '\r');

if ((index < 0.0) || (index > 1.0));
else 
if (((num_2 > 1.0)&&(((num_1 - num_2) < 1.0))) ||
((num_2 < 1.0)&&(((num_2 - num_1) > 1.0e-1))))break;
num_1 = num_2;}

 SW_reset;
  return 1;}




/*****************************************************************************************/
void Sc_Num_to_PC_A_Local(float num, char pre_dp, char post_dp, char next_char)
{int A = 1;
char keypresses[12];
char sign = '+';
int Exp = 0;

Check_num_for_to_big_or_small(num);                       //SW_reset required to escape from infinity and zero      

if (num < 0){sign = '-'; num = num * (-1);}

/*
while(--pre_dp){A = A*10;} 
while (num >= A){num = num/10.0; Exp += 1;}               //Repetitively divide large numbers by 10
while (num <= A){num = num*10.0; Exp -= 1;}               //and multiply small ones by 10
*/
if(pre_dp){
while(pre_dp--)A = A*10;}
if (num >= 1.0)while (num >= A){num = num/10.0; Exp += 1;}
else {while (num*10.0 < A){num = num*10.0; Exp -= 1;}}



if(sign == '-')num = num * (-1);

Serial.print(num, post_dp);
if(Exp) {Serial.write ('E'); Serial.print(Exp);}
Serial.write(next_char);}


//Note Floating point numbers define zero (about 10^-38) and infinity (about 10^38).
//Multiplying or dividing these numbers does not change them.
//There will be no escape from the loops given in this subroutine and the program may crash.

 


/******************************************************************************************/
float Sc_Num_from_PC_A_Local(char * num_as_string, int BL)            //Library version supports backspace key
{char strln;                                                          //Length of a string of characters

Serial.flush();                                                       //Clear the Arduino serial buffer   
strln = Serial.readBytesUntil('\r',num_as_string, BL);                //Read upto 20 characters or until a -cr- is received 
num_as_string[strln] = 0;                                             //Terminate the string with the null character
//Serial.write(num_as_string);                                          //Print out the numerical string
//Serial.write(next_char);                                              //new-line, space, \t or other specified character
return atof(num_as_string);}                                          //"askii to float" -C- library function




/******************************************************************************************/
