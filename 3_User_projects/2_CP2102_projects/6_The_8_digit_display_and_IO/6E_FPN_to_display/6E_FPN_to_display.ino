
/**************Proj_5E_Acquire_and process_FPN_Data_from_the _KBD********************/

/*
Like 5D but processes floating point data

Use the term Significand in place of FPN_digits

*/



#include "FPN_KBD_to_displsay_header.h"


int main (void){
float Num_1;
float power;
char digits[15];

long Significand;
long  Denominator;
char  expnt;



setup_HW_Arduino_IO;


Serial.write("\r\nEnter scientific number \
& terminate with Return key.\r\n");

Significand = fpn_from_KBD(digits, &expnt, &Denominator);
Num_1 = Significand_to_FPN((float)Significand, Denominator, expnt);
//Sc_Num_to_PC(Num_1, 1, 6, '\r');
if(Num_1 > 0.0)power = 1.5; else power = 3.0;


while (1){
Sc_Num_to_PC(Num_1,1,6 ,'\r');                            //Send number to PC
Significand = FPN_to_Significand(Num_1, &Denominator, &expnt);
Significand = Fraction_to_Binary_Signed(Significand, Denominator);
I2C_Tx_float_num(Significand, expnt);
I2C_Tx_float_display_control;

while(switch_1_down);

Num_1 = pow(Num_1, power);                                  //Do some arithmetic
}                                                         //Generates reset if result of arithmetic is too large or small

SW_reset;}
