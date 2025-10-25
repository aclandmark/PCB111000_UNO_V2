
/*Printing out a real number


First of all we acquire a real number from the KBD as we did for project "A_FPN_from_KBD"
and print it out using the Arduino library function
We then print it out using our own DIY function
    We simply divide the integer result by 2 raised to the power of the exponent (x-1)
     This gives us the integer part of the result
     We then continue by multiplying the remainder by 10 and repeating the division to get the first decimal place
     This multiplication and division process can be repeated until the required number of decimal places has been printed out.*/





//https://www.h-schmidt.net/FloatConverter/IEEE754.html


#include "FPN_to_String_header.h"


void print_fpn_as_binary(float, char);

int main (void){
  char digits[12];
long dividend, divisor;
int twos_expt;
long int_result;
char decimal_places;
 float fpn_result;
long Div, mod;
char num_string[12];

     
setup_HW_A;
if(watch_dog_reset != 1)Serial.write("Enter real number (i.e. with decimal point)\r\n");

dividend = Real_Num_from_PC_Local(digits, &divisor, &decimal_places);
Serial.write ('\t');
Int_Num_to_PC_A_Local(dividend, num_string, ' ');
Serial.write (" / ");
Int_Num_to_PC_A_Local(divisor, num_string, ' ');

int_result = int_divide (dividend, divisor, &twos_expt);

Serial.write ('\t');
Int_Num_to_PC_A_Local(int_result, num_string, ' ');
Serial.write (" x 2 exp ");
Int_Num_to_PC_A_Local(twos_expt, num_string, ' ');

fpn_result = assemble_fpn (int_result,twos_expt);

Serial.write ('\t');
print_fpn_as_binary(fpn_result, '\t');


Serial.print(fpn_result,decimal_places);          //decimal_places);

_delay_ms(25);
/**********************************/

Serial.write('\t');
 
 if (twos_expt < 0){
 divisor = 1;
 twos_expt = twos_expt * (-1);

 while (twos_expt >= 31){twos_expt -= 1; int_result = int_result >> 1;}
 for (int m = 0; m < twos_expt; m++) divisor *= 2;

 real_divide(int_result, divisor, &Div, &mod); 
 Int_Num_to_PC_A_Local(Div, num_string, ' ');
 Serial.write('.');
 for(int m = 0; m < decimal_places; m++){

 while (mod >= 0x7FFFFFFF/10){mod = mod/10; divisor /= 10;}
  
 real_divide(mod*10, divisor, &Div, &mod);
Serial.write(Div + '0');}}


else {while(twos_expt) {int_result *= 2; twos_expt -= 1;}Int_Num_to_PC_A_Local(int_result, num_string, ' ');}
 
SW_reset;}
/*************************************************************************************************************/



void Int_Num_to_PC_A_Local
(long Int_num, char * num_as_string, char next_char)                //Same as Unsigned_Int_to_PC()
{
ltoa(Int_num, num_as_string, 10);                                   //Long to askii
Serial.print(num_as_string);Serial.print(next_char);}



/*************************************************************************************************************/
//Type subroutine Real_Num_from_PC_Local()

/********************************************************************************************************************/
