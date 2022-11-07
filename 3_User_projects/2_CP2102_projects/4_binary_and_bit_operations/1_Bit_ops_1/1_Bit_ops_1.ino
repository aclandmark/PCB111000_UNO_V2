
/*
 Explore the operation of the OR, AND, Exclusive OR and NOT bitwise operations (|, & ^ and ~)
*/


#include "Bit_ops_1_header.h"



int main (void){

char comp;
char digits[8];
unsigned char lfsr;
char BWop;                                                                   //bit wise operation and complement (i.e. swap ones and zeros)

setup_HW_basic;
 _delay_ms(10);
 sei();

String_to_PC_Basic("\r\nSelect OP:  |   ^   &   ~|  ~^  or  ~&");
BWop = waitforkeypress_Basic(); 
if (BWop == '~') 
{comp = 1; BWop = waitforkeypress_Basic();}else comp = 0;                    //detect complement operator
if ((BWop != '|') && (BWop != '^') && (BWop != '&'))
SW_reset;                                                                    //reset if duff char was sent 
//Reset_ATtiny1606;
_delay_ms(25);                                                               //Mini-OS needs pause following reset
lfsr = PRN_8bit_GEN(0xF);                                                   //8 bit random number

do{
digits[0] = PRN_8bit_GEN(lfsr);
digits[1] = PRN_8bit_GEN(digits[0]);                                        //Second random number
digits[2] =  Op(digits[0] , digits[1], comp, BWop);                         //Process the numbers

for(int m = 3; m <= 7; m++)digits[m] = 0;

lfsr = digits[1];

I2C_Tx_BWops(digits);}
while (waitforkeypress_Basic() !='x');                                      //Press 'x' to escape               
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



/*****************************************************************/
unsigned char PRN_8bit_GEN(unsigned char lfsr){
unsigned int bit;

bit = (( lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 4)) & 1;
lfsr = (lfsr >> 1) | (bit << 7);
return lfsr;}





/******************************************************************/
