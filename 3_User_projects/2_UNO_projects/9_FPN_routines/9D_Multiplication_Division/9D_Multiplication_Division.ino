/*******************************************/

#include "9D_header.h"

#define Buff_Length  20

int main (void){

float FPN_1, FPN_2, Result;
char keypress;
char sign;
char num_as_string[Buff_Length + 2]; 


setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific numbers \
& terminate with Return key.\r\n");
FPN_1 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);
FPN_to_String(FPN_1, 1, 6, '\t', num_as_string); 
Serial.write(num_as_string);

while(1){Check_num_for_to_big_or_small(FPN_1);
Serial.write(" (x/?)\t");
keypress = waitforkeypress_A();
Serial.write(keypress);
FPN_2 = Scientific_number_from_KBD(num_as_string, &sign, Buff_Length);
FPN_to_String(FPN_2, 1, 6, ' ', num_as_string);
Serial.write(num_as_string);

Serial.write("= "); 
if (keypress == '/')Result = FPN_div_Local(FPN_1, FPN_2);
if (keypress == 'x')Result = FPN_mult_Local(FPN_1, FPN_2);

Serial.write('\t');
FPN_to_String(Result, 2, 4, '\r', num_as_string);
Serial.write(num_as_string);

FPN_1 = Result;}
SW_reset;}



/********************************************************************************************************************************/
float FPN_div_Local (float FPN_1, float FPN_2){

int twos_expnt_1, twos_expnt_2, twos_expnt_3;
long FPN_digits_1, FPN_digits_2, FPN_digits_3;
char sign_1, sign_2, sign_3 = '+';
float Result;
int FPN_shift;

sign_1 = '+';
sign_2 = '+';

FPN_digits_1 = unpack_FPN(FPN_1 , &twos_expnt_1, &sign_1);
FPN_digits_2 = unpack_FPN(FPN_2 , &twos_expnt_2, &sign_2);

while (FPN_digits_1 >= FPN_digits_2){FPN_digits_1 >>= 1; twos_expnt_1 += 1;}
FPN_digits_3 = Fraction_to_Binary_Signed(FPN_digits_1, FPN_digits_2);

if (sign_1 == sign_2)sign_3 = '+'; else sign_3 = '-';
twos_expnt_3 = twos_expnt_1 - twos_expnt_2;

if(twos_expnt_3 >= 127){Serial.write("Infinity");SW_reset;}

Result = Assemble_FPN((unsigned long) FPN_digits_3, twos_expnt_3,  sign_3);
if((!(*(long*)&Result)) || (*(long*)&Result == 0x80000000)) { Serial.write("Zero"); SW_reset;}

return Result;}



/********************************************************************************************************************************/
float FPN_mult_Local(float FPN_1, float FPN_2){
float Reciprocal;
float Result;
  Reciprocal = FPN_div_Local(1.0, FPN_2);
 Result = FPN_div_Local(FPN_1, Reciprocal);
return Result;}


/***********************************************************************************************************************************/




/*********************************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/**************************************************************************************************************************************/
