/**********************SIGN bit not implemented yet*********************/




#include "9A_header.h"

#define Buff_Length  20

int main (void){


float FPN;
char sign;
int twos_expnt;
long RHS_of_binary_point;
char digits[Buff_Length + 2];

setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n");

while(1){
  FPN = Scientific_number_from_KBD(digits, &sign, Buff_Length );  
  Sc_Num_to_PC_A(FPN, 1, 6, '\t');
  Print_long_as_binary(*(long*)&FPN);
 Serial.write("  \t");

RHS_of_binary_point = unpack_FPN(FPN, &twos_expnt, &sign);
Serial.print(twos_expnt); Serial.write('\t');
Print_RHS_of_binary_point(RHS_of_binary_point);
FPN = Assemble_FPN((unsigned long) RHS_of_binary_point, twos_expnt, sign);

/*
Serial.write("\r\n");
Print_long_as_binary(*(long*)&FPN);
Serial.write("\r\n");
*/

Serial.write("  \t  ");
Sc_Num_to_PC_A(FPN, 1, 6, '\r');
I2C_FPN_to_display(FPN);
}
SW_reset;}





/**********************************************************************************************************************/
float Scientific_number_from_KBD_Local(char * digits, char * sign, int BL){

//char digits[15];
long Significand;
long  twos_denominator;
char   tens_expnt;
int twos_expnt;
long FPN_digits;
float FPN;
char sign_local;


Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator, &sign_local, BL);              //Can be positive or negative
FPN_digits = Fraction_to_Binary_Signed(Significand, twos_denominator);                            //0.1011000011.... for example
FPN = Assemble_FPN(FPN_digits, twos_expnt, sign_local);
FPN = Scientifc_num_to_FPN(FPN, tens_expnt);
//if (sign_local == '-'){*(long*)&FPN |= (unsigned long) 0x80000000; }
*sign = sign_local;
return FPN;}


/***********************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <= 31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/***********************************************************************************************************************/
void Print_char_as_binary(unsigned char *num){
  for (int m = 0; m <=7; m++){
  if (*num & ((unsigned char) 0x80 >> m))Serial.write('1'); else Serial.write('0');}Serial.write(' ');}




/*************************************************************************************************************************/
void Print_RHS_of_binary_point(long L_num){

for(int m = 1; m <= 24; m++){if(m==2)Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}




/***********************************************************************************************************************************/
