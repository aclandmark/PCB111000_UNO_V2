
/*
 Explore the operation of the OR, AND, Exclusive OR and NOT bitwise operations (|, & ^ and ~)
*/


#include "Bit_ops_1_header.h"



int main (void){

char comp;
char digits[8];
unsigned char lfsr;
char BWop;                                                                   //bit wise operation and complement (i.e. swap ones and zeros)
char PRN_counter = 0;


setup_HW_basic;
User_prompt_Basic;
for(int m = 0; m <= 7; m++)digits[m] = 0;
lfsr = (PRN_8bit_GEN (0, &PRN_counter));                                     //Generate a new PRN (0) tells subroutine to use the EEPROM

while(1){
String_to_PC_Basic("\r\nSelect OP:  |   ^   &   ~|  ~^  or  ~&");
BWop = waitforkeypress_Basic(); 
if (BWop == '~') 
{comp = 1; BWop = waitforkeypress_Basic();}else comp = 0;                    //detect complement operator
if ((BWop != '|') && (BWop != '^') && (BWop != '&'))
SW_reset;                                                                    //reset if duff char was sent 

do{
digits[0] = PRN_8bit_GEN(lfsr, &PRN_counter);
digits[1] = PRN_8bit_GEN(digits[0], &PRN_counter);                          //Second random number
digits[2] =  Op(digits[0] , digits[1], comp, BWop);                         //Process the numbers
lfsr = digits[1];
I2C_Tx_BWops(digits);
}
while (waitforkeypress_Basic() !='x'); }                                     //Press 'x' to escape               
SW_reset;}




/*************************************************************************************/
unsigned char Op(unsigned char A, unsigned char B, char comp, char BWOp)
{char result=0;
switch (BWOp){
case '|': result = A | B; break;
case '^': result = A ^ B; break;
case '&': result = A & B; break;}
if (comp == 1) result = ~result;
return result;}






/******************************************************************/
