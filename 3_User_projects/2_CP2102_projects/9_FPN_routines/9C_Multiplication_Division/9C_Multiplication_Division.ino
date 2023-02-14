/*******************************************/

#include "9C_header.h"

int main (void){

float FPN_1, FPN_2, Result;
char keypress;
char sign;
setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific numbers \
& terminate with Return key.\r\n");

FPN_1 = Scientific_number_from_KBD(&sign);
Sc_Num_to_PC_A(FPN_1, 1, 6, ' '); 

while(1){
Serial.write(" (x/?)\t");
keypress = waitforkeypress_A();
Serial.write(keypress);
 FPN_2 = Scientific_number_from_KBD(&sign);
Sc_Num_to_PC_A(FPN_2, 1, 6, ' '); Serial.write("= "); 
if (keypress == '/')Result = Foating_point_division(FPN_1, FPN_2);
if (keypress == 'x')Result = Foating_point_multiplication(FPN_1, FPN_2);

Serial.write('\t');Sc_Num_to_PC_A(Result,2,6, '\r');
//I2C_FPN_to_display(Result);
FPN_1 = Result;}

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

//Serial.write("\r\nC");Print_long_as_binary(FPN_part_1);Serial.write('\t');Serial.print(twos_expnt_1);
//Serial.write("\r\nD");Print_long_as_binary(FPN_part_2);Serial.write('\t');Serial.print(twos_expnt_2);


while (FPN_part_1 >= FPN_part_2){FPN_part_1 >>= 1; twos_expnt_1 += 1;}
//Serial.write("\r\nE");Print_long_as_binary(FPN_part_1);Serial.write('\t');Serial.print(twos_expnt_1);


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
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
