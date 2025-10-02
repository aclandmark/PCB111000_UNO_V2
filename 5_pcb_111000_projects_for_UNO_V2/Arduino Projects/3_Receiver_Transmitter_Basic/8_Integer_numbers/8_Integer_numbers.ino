


//Calculates factors of an integer number



/************************************************************************************************************/
#include "Integer_numbers_header.h"

#define message_1 "\r\nEnter integer number:  PCB returns all factors of that number.(Press sw1 to cancel.)\r\n"
#define message_2 "\r\nNew number?\r\n"


//Type main routine here




/******************************************************************************************/
long Int_from_PC_Local(char digits[]){
char keypress;
for(int n = 0; n<=7; n++) digits[n] = 0; 

do
{keypress =  waitforkeypress_Basic();} 
while (!(decimal_digit_Basic(keypress)));                                      //(non_decimal_char(keypress));  //Not -,0,1,2,3,4,5,6,7,8 or 9
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);

while(1){
if ((keypress = wait_for_return_key_Basic())  =='\r')break;
if (decimal_digit_Basic (keypress))                                           //012345678or9  :Builds up the number one keypress at a time
{for(int n = 7; n>=1; n--)
digits[n] = digits[n-1];                                                //Shifts display left for each keypress
digits[0] = keypress;
I2C_Tx_8_byte_array(digits);}}
            
return I2C_displayToNum();}





/******************************************************************************************/
