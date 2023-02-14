/*******************************************/

#include "9D_header.h"

int main (void){

float FPN, FPN_bkp;
char sign;
char Digits_before_dp, Digits_after_dp;
char print_string[15];

setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific numbers \
& terminate with Return key.\r\n");
FPN = Scientific_number_from_KBD(&sign);



Serial.write("Number of digits before the decimal point (0-9)?\t");
Digits_before_dp = waitforkeypress_A() - '0'; Serial.write(Digits_before_dp + '0');
Serial.write("\r\nNumber of digits after the decimal point (0-9)?\t");
Digits_after_dp = waitforkeypress_A() - '0' ; Serial.write(Digits_after_dp + '0');Serial.write("\r\n");

FPN_to_String(FPN, Digits_before_dp, Digits_after_dp, '\r', print_string);

Serial.write (print_string);
SW_reset;}




void FPN_to_String(float FPN, char pre_dp, char post_dp, char next_char, char * print_string){

char tens_expnt, print_expnt;
int twos_expnt;
char sign;
long FPN_as_long, FPN_as_long_bkp;
long Denominator;
long int_part_max;
int Num_digits;
float round_denom;
float FPN_bkp;


 Num_digits = pre_dp + post_dp; 

if (*(long*)&FPN & (unsigned long) 0x80000000){(*(long*)&FPN &= 0x7FFFFFFF);sign = '-';} else sign = '+';
tens_expnt = 0;
int_part_max = 1;
for(int m = 0; m < pre_dp; m++) int_part_max *= 10.0;

FPN_bkp = FPN;

if(FPN_bkp >= (float)int_part_max){
while  (FPN >= (float)int_part_max){FPN /= 10.0; tens_expnt += 1;}
print_expnt = tens_expnt;}

if(FPN_bkp < (float)int_part_max){
while  (FPN < (float)int_part_max){FPN *= 10.0; tens_expnt -= 1;}
print_expnt = tens_expnt+1;}

while (FPN >= 1.0){FPN /= 10.0; tens_expnt += 1;}



/*****************************************Build the number 0.000000005 used for rounding*********************************/  
round_denom = 1.0;
for(int m = 0; m <= Num_digits; m++)round_denom *= 10.0; 
FPN = FPN + (5.0/round_denom);

/*****************************************Obtain the number 12345678 in binary form***************************************/
if (sign == '-')  *(long*)& FPN |= (unsigned long) 0x80000000;
FPN_as_long = unpack_FPN(FPN, &twos_expnt, &sign);
FPN_as_long = FPN_as_long >> 4 ; 
Denominator = 0x8000000 << (-twos_expnt);

/****************************************Convert 12345678 to string form***************************************************/
{int p = 0;
for(int m = 0; m < 15; m++)print_string[m] = 0;
Denominator /= 10.0; 

if(sign == '-')
{print_string[0] = '-'; p += 1;}
if (!(pre_dp))print_string[p++] = '0'; 
for (int m = 0; m < Num_digits; m++){
FPN_as_long_bkp = FPN_as_long/Denominator;

if(m == pre_dp)print_string[p++] = '.'; 
if(FPN_as_long_bkp){print_string[p] = (FPN_as_long_bkp + '0'); 
FPN_as_long = FPN_as_long%Denominator;} 
else print_string[p] = '0'; 
p += 1;
FPN_as_long *= 10;}
print_string[p++] = 'E'; 
itoa(print_expnt, print_string+p, 10);}}









/*********************************************************************************************************************************/
float Scientific_number_from_KBD(char *sign){

char digits[15];
long Significand;
long  twos_denominator;
char   tens_expnt;
int twos_expnt;
long FPN_digits;
float FPN;
char sign_local;


Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator, &sign_local);              //Can be positive or negative
FPN_digits = Fraction_to_Binary_Signed(Significand, twos_denominator);                            //0.1011000011.... for example
FPN = Assemble_FPN(FPN_digits, twos_expnt);
FPN = Scientifc_num_to_FPN(FPN, tens_expnt);
if (sign_local == '-'){*(long*)&FPN |= (unsigned long) 0x80000000; }
*sign = sign_local;
return FPN;}



/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
