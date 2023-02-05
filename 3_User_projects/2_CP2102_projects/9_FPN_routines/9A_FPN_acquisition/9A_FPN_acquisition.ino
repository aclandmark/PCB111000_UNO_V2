/**********************SIGN bit not implemented yet*********************/

#include "9A_header.h"

int main (void){


float FPN;

setup_HW_Arduino_IO;

Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n");

while(1){FPN = Scientific_number_from_KBD();
/*Significand = Get_fpn_from_KBD(digits, &twos_expnt, &tens_expnt, &twos_denominator);              //Can be positive or negative
FPN_digits = Fraction_to_Binary_Signed(Significand, twos_denominator);                            //0.1011000011.... for example
FPN = Assemble_FPN(FPN_digits, twos_expnt);
FPN = Scientifc_num_to_FPN(FPN, tens_expnt);*/

Serial.write("\r\n");Print_long_as_binary(*(long*)&FPN);

Serial.write("\t\t");
Sc_Num_to_PC(FPN, 1, 6, '\r');
//I2C_FPN_to_display(FPN);
}
SW_reset;}





/**********************************************************************************************************************/
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



/***********************************************************************************************************************/
void Print_long_as_binary(long L_num){

for(int m = 0; m <=31; m++){if((m == 1) || (m== 9))Serial.write(' ');
if (L_num  &  ((unsigned long)0x80000000 >> m))Serial.write('1'); else Serial.write('0');}}



/***********************************************************************************************************************/
void Print_char_as_binary(unsigned char *num){
  for (int m = 0; m <=7; m++){
  if (*num & ((unsigned char) 0x80 >> m))Serial.write('1'); else Serial.write('0');}Serial.write(' ');}




/*************************************************************************************************************************/

/***********************************************************************************************************************************/
