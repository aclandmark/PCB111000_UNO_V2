/*******************************************/

#include "9C_header.h"

#define Buff_Length  20

int main (void){

float FPN_1, FPN_2, Result;
char sign;
char num_as_string[Buff_Length + 2]; 


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific numbers \
& terminate with Return key.\r\n");

FPN_1 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);
Sc_Num_to_PC_A(FPN_1, 1, 6, ' '); Serial.write(" + ");

while(1){
 FPN_2 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);
Sc_Num_to_PC_A(FPN_2, 1, 6, ' '); Serial.write("= "); 

Result = FPN_add_local(FPN_1, FPN_2);
FPN_to_String(Result, 1, 5, '\r', num_as_string);
Serial.write(num_as_string);

if ((*(long *)&Result))I2C_FPN_to_display(Result);
FPN_1 = Result;}

SW_reset;}



/********************************************************************************************************************************/
float FPN_add_local (float FPN_1, float FPN_2){

int twos_expnt_1, twos_expnt_2, twos_expnt_3;
long FPN_digits_1, FPN_digits_2, FPN_digits_3;
char sign_1, sign_2, sign_3 = '+';
float Result;

sign_1 = '+';
sign_2 = '+';

FPN_digits_1 = unpack_FPN(FPN_1 , &twos_expnt_1, &sign_1);
FPN_digits_2 = unpack_FPN(FPN_2 , &twos_expnt_2, &sign_2);

if (sign_1 == '-')FPN_digits_1 = -FPN_digits_1;
if (sign_2 == '-')FPN_digits_2 = -FPN_digits_2;

twos_expnt_3 = twos_expnt_1;

if (twos_expnt_1 > twos_expnt_2){
  while(twos_expnt_1 > twos_expnt_2){twos_expnt_2 += 1; FPN_digits_2 = FPN_digits_2 >> 1;}twos_expnt_3 = twos_expnt_2;}

if (twos_expnt_1 < twos_expnt_2){
  while(twos_expnt_1 < twos_expnt_2){twos_expnt_1 += 1; FPN_digits_1 = FPN_digits_1 >> 1;}twos_expnt_3 = twos_expnt_1;}

FPN_digits_3 = FPN_digits_1 + FPN_digits_2;

if (sign_1 != sign_2)
{
    if (!(FPN_digits_3)){*(long*)(&Result) = 0x0; return Result;}
  
 if (FPN_digits_3 & (unsigned long) 0x80000000){FPN_digits_3 *= -1; sign_3 = '-';}
while (!(FPN_digits_3 & (0x40000000))){FPN_digits_3 = FPN_digits_3 << 1; twos_expnt_3 -= 1;}}

if(sign_1 == sign_2){

if(twos_expnt_3 == 128){Serial.write("Infinity");SW_reset;}

switch(sign_1){
case '+': break;

case '-': 
FPN_digits_3 =    FPN_digits_1 * -1 + FPN_digits_2 * -1; ; 
sign_3 = '-';break;}

if (FPN_digits_3 & (unsigned long) 0x80000000)
{FPN_digits_3 = (unsigned long)FPN_digits_3  >> 1; twos_expnt_3 += 1;}}

Result = Assemble_FPN((unsigned long) FPN_digits_3, twos_expnt_3, sign_3);
if (sign_3 == '-')
*(long*)&Result = *(long*)&Result | (unsigned long)0x80000000; 
return Result;}



/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
