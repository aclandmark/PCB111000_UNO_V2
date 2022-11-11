
/*
Here a number such as 1.25 is considered to be a real number and 1.25E8 a scientific number 
In C the real number can have up to about 6 digits
The exponent can be as any thing between +38 and -38 (at least).

The Arduino library can accept the full range of scientific numbers in string format and
the -C- library function "atof" (askii to float) can generate the required floating point number.

The Arduino function "Serial.print" accepts a floating point number and prints it out with the 
decimal point in the required position.

Here when printing a scientific number greater than about 10^7 we first repetitively divide it by 10 
while at the same time incrementing the exponent.  We then use "Serial.print" to print the resulting 
real number followed by the exponent.  
*/



#include "Scientific_numbers_header.h"

/************************************************************************************************************/



int main (void)  
  { 
    char num_string[12];
    float  num;
    float index;
 
 setup_328_HW_Arduino_IO;
    
   if (reset_status == 1) User_prompt;
 
   Serial.write("\r\nScientific number\r\n");
   
num = Sc_Num_from_PC_local(num_string, '\r');

if (num < 1.0) index = 3;                                   //Raise small numbers and negative ones to the power of 3
else index = 1.5;                                           //Raise remaining numbers to the power of 1.5

while(1){
  while(!(Serial.available()))wdr();
Serial.read();                                            //The equivalent of waitforkeypress()
num = pow (num,index);                                    //-C- library function
Sc_Num_to_PC_local(num, 2, 4, '\r');
}

 SW_reset;
  return 1;
  }




/*****************************************************************************************/
void Sc_Num_to_PC_local(float num, char pre_dp, char post_dp, char next_char)
{int A = 1;
char keypresses[12];
char sign = '+';
int Exp = 0;

Check_num_for_to_big_or_small(num);                       //SW_reset required to escape from infinity and zero      

if (num < 0){sign = '-'; num = num * (-1);}

while(--pre_dp){A = A*10;} 
while (num >= A){num = num/10.0; Exp += 1;}               //Repetitively divide large numbers by 10
while (num <= A){num = num*10.0; Exp -= 1;}               //and multiply small ones by 10

if(sign == '-')num = num * (-1);

Serial.print(num, post_dp);
if(Exp) {Serial.write ('E'); Serial.print(Exp);}
Serial.write(next_char);}


//Note Floating point numbers define zero (about 10^-38) and infinity (about 10^38).
//Multiplying or dividing these numbers does not change them.
//There will be no escape from the loops given in this subroutine and the program will crash.

 


/******************************************************************************************/
float Sc_Num_from_PC_local(char * num_as_string,char next_char)
{char strln;                                                          //Length of a string of characters

pause_WDT;                                                            //Allow time for number to be entered at the keyboard
Serial.flush();                                                       //Clear the Arduino serial buffer   
strln = Serial.readBytesUntil('\r',num_as_string, 20);                //Read upto 20 characters or until a -cr- is received 
resume_WDT;
num_as_string[strln] = 0;                                             //Terminate the string with the null character
Serial.write(num_as_string);                                          //Print out the numerical string
Serial.write(next_char);                                              //new-line, space, \t or other specified character
return atof(num_as_string);}                                          //"askii to float" -C- library function




/******************************************************************************************/
