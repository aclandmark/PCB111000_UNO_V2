/*******************************************/

#include "9B_header.h"

#define Buff_Length  20

int main (void){

float FPN, FPN_bkp;
char sign;
char Digits_before_dp, Digits_after_dp;
char print_string[Buff_Length + 2];

setup_HW_Arduino_IO;

if(watch_dog_reset == 1)watch_dog_reset = 0;
else
{Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n\
Then key in num digits before/after the decimal point.\r\n");}

while(1){

FPN = Scientific_number_from_KBD(print_string, &sign,Buff_Length );
Sc_Num_to_PC_A(FPN, 1, 6, '\t');
Print_long_as_binary(*(long*)&FPN);
Serial.write(" \t ");

Digits_before_dp = waitforkeypress_A() - '0'; 
Digits_after_dp = waitforkeypress_A() - '0' ; 

FPN_to_String(FPN, Digits_before_dp, Digits_after_dp, '\r', print_string);

Serial.write (print_string);}
SW_reset;}




void FPN_to_String_Local(float FPN, char pre_dp, char post_dp, char next_char, char * print_string){

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
FPN = FPN + (5.0/(round_denom));

if(FPN >= 1.0){FPN /= 10.0;tens_expnt += 1; print_expnt = tens_expnt;}

//Serial.write("ABC");Serial.print (FPN,5);Serial.write(" \t ");

/*****************************************Obtain the number 12345678 in binary form***************************************/
if (sign == '-')  *(long*)& FPN |= (unsigned long) 0x80000000;
FPN_as_long = unpack_FPN(FPN, &twos_expnt, &sign);

//Serial.write("ABC");Serial.print (twos_expnt);Serial.write(" \t ");

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

if(print_expnt) {print_string[p++] = 'E'; 
itoa(print_expnt, print_string+p, 10);}
else print_string[p++] = '\0';

p = 0;
while (print_string[p++]); p -= 1;
print_string[p++] = next_char;
print_string[p] = '\0';}}









/*********************************************************************************************************************************/

/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
