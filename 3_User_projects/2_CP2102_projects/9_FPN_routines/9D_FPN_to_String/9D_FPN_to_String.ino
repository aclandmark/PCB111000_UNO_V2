/*******************************************/

#include "9D_header.h"

int main (void){

float FPN;
char tens_expnt;
int twos_expnt;
char sign;
char numerical_string[3];
long FPN_as_long, FPN_as_long_bkp;
//float base_num;
//int Digits_before_dp, Digits_after_dp;


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific numbers \
& terminate with Return key.\r\n");
FPN = Scientific_number_from_KBD();



 
/*
Serial.write("Number of digits before the decimal point?");
Digits_before_dp = Int_Num_from_PC_A(numerical_string, '\r');
Serial.write("Number of digits after the decimal point?\r\n");
Digits_after_dp = Int_Num_from_PC_A(numerical_string, '\r');
*/

tens_expnt = 0;
while (FPN >= 1.0){FPN /= 10.0; tens_expnt += 1;}
//Sc_Num_to_PC_A(FPN,1,7,'E');Serial.print((int)FPN_expnt);
FPN_as_long = unpack_FPN(FPN, &twos_expnt, &sign);
Serial.write('A');Serial.write(-twos_expnt + '0');
FPN_as_long = FPN_as_long >> (7 + (-twos_expnt));   /////////////////Shift 0x1000000 instead
Print_long_as_binary(FPN_as_long);Serial.write("\r\n");

for (int m = 0; m < 8; m++){
FPN_as_long *= 10;
FPN_as_long_bkp = FPN_as_long/0x1000000;

if(FPN_as_long_bkp){Serial.write (FPN_as_long_bkp + '0');FPN_as_long = FPN_as_long%0x1000000;} else Serial.write('0');


  
//FPN_as_long = FPN_as_long%0x1000000;
}
/*
base_num = 1;
for(int m = 0; m < Digits_before_dp; m++) base_num *= 10.0;
FPN_expnt = 0;
if(FPN >= base_num){
  while  (FPN >= base_num){FPN /= 10.0; FPN_expnt += 1;}
Sc_Num_to_PC_A(FPN, Digits_before_dp, Digits_after_dp, ' '); Serial.print ((int)FPN_expnt);}*/

SW_reset;}


/********************************************************************************************************************************/
float Foating_point_division (float FPN_1, float FPN_2){

int twos_expnt_1, twos_expnt_2, twos_expnt_3;
long FPN_part_1, FPN_part_2, FPN_part_3;
char sign_1, sign_2, sign_3 = '+';
float Result_as_long;

sign_1 = '+';
sign_2 = '+';

FPN_part_1 = unpack_FPN(FPN_1 , &twos_expnt_1, &sign_1);
FPN_part_2 = unpack_FPN(FPN_2 , &twos_expnt_2, &sign_2);

while (FPN_part_1 >= FPN_part_2){FPN_part_1 >>= 1; twos_expnt_1 += 1;}
FPN_part_3 = Fraction_to_Binary_Signed(FPN_part_1, FPN_part_2);
Result_as_long = Assemble_FPN((unsigned long) FPN_part_3, twos_expnt_1 - twos_expnt_2);
if (sign_1 == sign_2);
else
*(long*)&Result_as_long = *(long*)&Result_as_long | (unsigned long)0x80000000; 
return Result_as_long;}
/********************************************************************************************************************************/

float Foating_point_multiplication(float FPN_1, float FPN_2){
float Reciprocal;
float Result;
  Reciprocal = Foating_point_division(1.0, FPN_2);
 Result = Foating_point_division(FPN_1, Reciprocal);
return Result;}







/*********************************************************************************************************************************/
float Scientific_number_from_KBD(void){

char digits[15];
long Significand;
long  twos_denominator;
char   tens_expnt;
int twos_expnt;
long FPN_digits;
float FPN;

Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator);              //Can be positive or negative
FPN_digits = Fraction_to_Binary_Signed(Significand, twos_denominator);                            //0.1011000011.... for example
FPN = Assemble_FPN(FPN_digits, twos_expnt);
FPN = Scientifc_num_to_FPN(FPN, tens_expnt);
return FPN;}



/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
/*
 *   Serial.write("\r\nC");Print_long_as_binary(FPN_part_1);Serial.write('\t');Serial.print(twos_expnt_1);
Serial.write("\r\nD");Print_long_as_binary(FPN_part_2);Serial.write('\t');Serial.print(twos_expnt_2);
Serial.write("\r\nE");Print_long_as_binary(FPN_part_3);Serial.write('\t');Serial.print(twos_expnt_3);
 * 
Serial.write("\r\nA");Print_long_as_binary(*(long*)&FPN_1);
Serial.write("\r\nB");Print_long_as_binary(*(long*)&FPN_2);
Serial.write("\r\nC");Print_long_as_binary(FPN_part_1);Serial.write('\t');Serial.print(twos_expnt_1);
Serial.write("\r\nD");Print_long_as_binary(FPN_part_2);Serial.write('\t');Serial.print(twos_expnt_2);
Serial.write("\r\nE");Print_long_as_binary(FPN_part_1 + FPN_part_2);
*/
