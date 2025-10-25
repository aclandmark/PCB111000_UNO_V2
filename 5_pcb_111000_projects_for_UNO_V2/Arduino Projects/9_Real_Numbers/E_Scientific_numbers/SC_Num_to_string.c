
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

long int_divide(long, long, int *);
void real_divide(long, long, long *, long *);
long unpack_FPN(float, int *, char *);



/********************************************************************************************************************************************/
void Real_num_to_string_with_rounding_Local(char * num_string, long int_num, int twos_expt, char decimal_places, char * string_offset){
long  divisor;
long Div, mod;
char string_length;
char leading_zeros = 0;
char digit_counter;
long Integer_Result;

 divisor = 1;
 twos_expt = twos_expt * (-1);                                                  // We divide not multiply by 2^twos_expt

 while (twos_expt >= 31){twos_expt -= 1; int_num = int_num >> 1;}               //Long numbers only have room for 32 bits
 for (int m = 0; m < twos_expt; m++) divisor *= 2;                              //Convert 2^twos_expt to a number (i.e. 2^3 to 8) such as 2, 4, 8, 16....


//*****************************Obtain integer result and remainder then caculate decimal places*******************************************
real_divide(int_num, divisor, &Div, &mod);                                   //Divide the integer part by the exponent part to get the integer result

ltoa (Div, num_string, 10);                                                     //Populate string with the integer result (ltoa: "long to askii" is a C library function)
*string_offset = 0;                                                              //Calculate the number of digits in the string
while(num_string[*string_offset])*string_offset += 1;                             //Do not add the decimal point                          

if (Div){
for(int m = 0; m <= decimal_places; m++){                                       //Obtain one extra decimal place for rounding
if (*string_offset == 9){decimal_places = m-1; break;}
while (mod >= 0x7FFFFFFF/10){mod = mod/10; divisor /= 10;} 
real_divide(mod*10, divisor, &Div, &mod);                                        //Place decimal digits in num_string along side the integer result and decimal place
num_string[*string_offset] = Div + '0';  *string_offset += 1; }                                    //String offset points to the next free location in num_string
num_string[*string_offset] = 0;                                                  //Terminate the string       

Integer_Result = atol(num_string) + 0;
for(int m = 0; m <= 11; m++)num_string[m] = 0;
ltoa ((Integer_Result + 6), num_string, 10);                                   //Use C function Askii to Long Int to convert num_string to an integer, add 5 and convert it back to a string

*string_offset = 0; 
while(num_string[*string_offset])*string_offset += 1;                             //Calculate the number of digits in the string
*string_offset -=1;
 num_string[*string_offset] = 0;}}



/****************************************************************************************************************************************/
long unpack_FPN(float FPN, int *twos_expnt, char * sign)
{long FPN_digits;
int shift;

FPN_digits = (*(long*)&FPN);                                      //Read the FPN using a long pointer
if( FPN_digits & (((unsigned long)0x80000000)))
{ FPN_digits = FPN_digits & (~((unsigned long)0x80000000));        //If negative remove sign bit
*sign = '-';}else *sign = '+';

*twos_expnt = (FPN_digits >> 23) - 127;                           //Expponent occupies bits 0 to 7

if(*twos_expnt >=-126){       
FPN_digits = (FPN_digits & 0x7FFFFF);                             //Isolate bits zero to 22
FPN_digits |= ((unsigned long)0x80000000 >> 8);                   //Add the unsaved 1 to bit 23
*twos_expnt += 1;
FPN_digits = FPN_digits << 7;                                     //Shift left 7 places until bit 30 is a 1
return FPN_digits;}

if(*twos_expnt == -127){                                          //Bit 23 will not be set to 1, 2's exponent is unknown
if(!(FPN_digits))return 0;                                        //If exponent is -127 shift is 9 MSB is bit 21
else                                                              //If exponent is -128 shift is 10 MSB is bit 20
{shift = 0;
while (!(FPN_digits & 0x40000000)){FPN_digits <<= 1; shift += 1;}
*twos_expnt = -(118 + shift);
return FPN_digits;}}}






/*****************************************************************************************************************************************************************/
